#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <string.h>

#include "chat.h"

#define LD(m) logmsg(1, debug, 0, m)
#define LI(m) logmsg(1, info, 0, m)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int logger_sock;
char users[512][512];
int nusers = 0;
fd_set fds;
int max_fd = 0;

void setup_logger();
void read_data();
void logmsg(int level, type t, unsigned char syslog, char *m);
void run_server();

int main(void)
{
	FD_ZERO(&fds);
	setup_logger();
	read_data();

	run_server();
}

void setup_logger()
{
	if (!fork())
		execl("logger", "%s", "logger");

	logger_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent *server = gethostbyname("localhost");
	struct sockaddr_in addr = {};
	bcopy((char*)server->h_addr, (char*)&addr.sin_addr.s_addr, server->h_length);
	addr.sin_port = htons(LOGGER_PORT);
	while (connect(logger_sock, (struct sockaddr*)&addr, sizeof(addr))) {}
}

void read_data()
{
	LI("Opening data file");
	FILE *f = fopen("data.txt", "r");
	while (fgets(users[nusers++], sizeof(users[0]), f) != NULL) {}
	nusers--;
	fclose(f);
}

void logmsg(int level, type t, unsigned char syslog, char *m)
{
	log_message msg = {};
	msg.level = level;
	msg.t = t;
	msg.syslog = syslog;
	strncpy(msg.message, m, sizeof(msg.message)-1);
}

void run_server()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {};
	addr.sin_port = htons(SERVER_PORT);
	bind(server_sock, (struct sockaddr*)&addr, sizeof(addr));
	listen(server_sock, 5);
	FD_ADD(server_sock, &fds);

	while (1) {
		select(server_sock+1, &fds, NULL, NULL, NULL);
		struct sockaddr new_addr;
		socklen_t len;
		int new_sock = accep(server_sock, &new_addr, &len);
		dump_data(new_sock);
		close(new_sock);
	}
}
