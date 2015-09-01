#ifndef _CHAT_H_
#define _CHAT_H_

#define LOGGER_PORT 9999
#define LOGGER_PORT 8888

enum type {
	debug,
	info,
	warning,
	error
};

typedef struct {
	int level;
	type t;
	bool syslog;
	char message[1024];
} log_message;

#endif //_CHAT_H_
