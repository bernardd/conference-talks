#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define LD(m) log(1, debug, false, m);
#define LI(m) log(1, info, false, m);

int logger_sock;
char [512][512] users;
int nusers = 0;

int main(void)
{
	setup_logger();
	read_data();

	run_server();
}

void setup_logger()
{
	if (!fork())
		exec("logger");

	logger_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent *server = gethostbyname("localhost");
	struct sockaddr_in addr = {};
	bcopy((char*)server->h_addr, (char*)&addr.sin_addr.saddr, server->h_length);
	addr.sin_port = htons(LOGGER_PORT);
	while (connect(sock, (struct sockaddr*)&addr, sizeof(addr))) {}
}

void read_data()
{
	LI("Opening data file");
	FILE *f = fopen("data.txt", "r");
	while (fgets(users[nusers++], sizeof(users[0]), f) != EOF) {}
	nusers--;
	fclose(f);
}

void log(int level, type t, bool syslog, char *m)
{
	log_message m = {};
	m.level = level;
	m.type = type;
	m.syslog = syslog;
	strncpy(m.message, m, sizeof(m.message)-1);
}

void run_server()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr = {};
	addr.sin_port = htons(SERVER_PORT);
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));

}
