#ifndef MEMORY_ACCESS_H_
#define MEMORY_ACCESS_H_

#define MAX_PATH_LEN 128
#define BASIC_PATH ""

#include "memory_access.h"
#include "fatfs.h"
#include "ff.h"
#include "usb_host.h"
#include <string.h>

FATFS fs;

int8_t mount_usb();

int8_t unmount_usb();

int8_t is_full_path(const char *path);

uint8_t get_full_path(char *current_path, const char *path);

uint8_t get_final_path(char *full_path);

uint8_t get_final_path_2(char *current_path, const char *path);

int8_t directory_exist(const char *full_path);

uint8_t change_directory(char *current_path, const char *path);

uint8_t list_directory(const char *current_path, char *buffer, uint16_t buffer_size);

uint8_t open_file(const char *current_path, const char *filename, FIL *file);

uint8_t create_file(const char *current_path, const char *filename, FIL *file);

void close_file(FIL *file);

int8_t delete_file(const char *current_path, const char *filename);

int8_t create_dir(const char *current_path, const char *dir_name);

unsigned int write_to_file(FIL *file, char *buf, uint16_t size);

unsigned int read_file(FIL *file, char *buf, uint16_t size);

void USB_Process(ApplicationTypeDef Appli_state);

uint8_t open_dir(const char *current_path, DIR *dir);

uint8_t close_dir(DIR *dir);

uint8_t list_directory_part(DIR *dir, char *buffer, uint16_t buffer_size);


#endif /* MEMORY_ACCESS_H_ */
