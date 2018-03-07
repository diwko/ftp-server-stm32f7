#ifndef FTP_SERVER_H_
#define FTP_SERVER_H_

#include "api.h"
#include "lwip/netif.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"


#define COMMAND_PORT 8000
#define DATA_PORT 7999
#define CLIENTS_TASKS_NUMBER 3
#define CLIENTS_QUEUE_SIZE 10
#define BUFFER_LEN 512
#define MAX_RESPOND_LEN 128
#define MAX_LIST_DATA_LEN 64


extern struct netif gnetif;

struct netconn *command_conn;
struct netconn *data_conn;
QueueHandle_t clients_queue;

void ftp_start();
void ftp_start_paraller();

#endif /* FTP_SERVER_H_ */
