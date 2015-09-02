#ifndef _CHAT_H_
#define _CHAT_H_

#define LOGGER_PORT 9999
#define SERVER_PORT 8888

typedef enum {
	debug,
	info,
	warning,
	error
} type;

typedef struct {
	int level;
	type t;
	unsigned char syslog;
	char message[1024];
} log_message;

#endif //_CHAT_H_
