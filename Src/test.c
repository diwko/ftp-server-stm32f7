#include "test.h"
#include "memory_access.h"
#include "ftp_server.h"
#include "fatfs.h"

void test() {
	printf("TEST BEGIN\r\n");
	ftp_start();
	printf("TEST END\r\n");
}