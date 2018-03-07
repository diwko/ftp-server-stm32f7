#ifndef REQUEST_H_
#define REQUEST_H_

#include "stdint.h"

#define COMMAND_SIZE 5
#define ARGS_SIZE 128

typedef struct {
	char command[COMMAND_SIZE];
	char args[ARGS_SIZE];
} Request;

Request get_request(char *msg);
uint8_t update_request(Request *req, char *msg);

#endif /* REQUEST_H_ */
