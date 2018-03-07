#ifndef FTPSERVER_CLIENT_THREAD_H
#define FTPSERVER_CLIENT_THREAD_H

#include "memory_access.h"

typedef struct {
	struct netconn *conn;
    char current_dir[MAX_PATH_LEN];
} ClientData;

void serve_client(void *client_conn);
void serve_client_task(void *arg);

#endif //FTPSERVER_CLIENT_THREAD_H
