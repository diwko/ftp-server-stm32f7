#include "memory_access.h"
#include "fatfs.h"
#include "ff.h"
#include "usb_host.h"
#include <string.h>

int8_t mount_usb() {	
	if(f_mount(&fs, "", 1) != FR_OK) {
		printf("usb mount error\r\n");		
		return 0;
	}
	printf("mounted USB\r\n");
	return 1;
}

int8_t unmount_usb() {
	if(f_mount(0, "", 0) != FR_OK) {
		printf("usb unmount error\r\n");
		return 0;
	}
	printf("unmounted USB\r\n");
	return 1;
}

int8_t is_full_path(const char *path) {
	if(path[0] == '/' ) {
		return 1;
	}
	return 0;
}

uint8_t get_full_path(char *current_path, const char *path) {
    if(is_full_path(path)) {
    	if(strlen(path) + 1 > MAX_PATH_LEN)
    		return 0;

        strcpy(current_path, path);

    } else if(current_path[0] == '\0') {
    	if(strlen(path) + 2 > MAX_PATH_LEN)
    		return 0;

        sprintf(current_path, "/%s", path);

    } else if(strcmp(current_path, "/") == 0) {
    	if(strlen(current_path) + strlen(path) + 1 > MAX_PATH_LEN)
    		return 0;

    	char new_path[MAX_PATH_LEN];
        sprintf(new_path, "%s%s", current_path, path);
        strcpy(current_path, path);

    } else {
    	if(strlen(current_path) + strlen(path) + 2 > MAX_PATH_LEN)
    		return 0;

    	char new_path[MAX_PATH_LEN];
        sprintf(new_path, "%s/%s", current_path, path);
        strcpy(current_path, path);
    }
    return 1;
}

uint8_t get_final_path(char *full_path) {
    if(strlen(BASIC_PATH) + strlen(full_path) + 1 > MAX_PATH_LEN)
    	return 0;

    char new_path[MAX_PATH_LEN];    
    sprintf(new_path, "%s%s", BASIC_PATH, full_path);
    strcpy(full_path, new_path);
    return 1;
}

uint8_t get_final_path_2(char *current_path, const char *path) {
    if(!get_full_path(current_path, path))
    	return 0;

    if(!get_final_path(current_path))
    	return 0;

    return 1;
}

int8_t directory_exist(const char *full_path) {
	char final_path[MAX_PATH_LEN]; 
	strcpy(final_path, full_path);	
    if(!get_final_path(final_path)) {
    	printf("cannot get directory path: %s\r\n", full_path);
    	return 0;
    }

    DIR dir;
    if(f_opendir(&dir, final_path) != FR_OK) {
		printf("cannot change to directory: %s\r\n", full_path);
		return 0;
	}
	f_closedir(&dir);
	return 1;  
}

uint8_t change_directory(char *current_path, const char *path) {
	char new_path[MAX_PATH_LEN]; 
	strcpy(new_path, current_path);

    if(!get_full_path(new_path, path))
    	return 0;

    if(!directory_exist(new_path)) {
    	FRESULT res = f_mkdir(new_path);
    	if(res != FR_OK) {
			printf("connot make directory: %s\r\n", new_path);
        	return 0;
    	}
    }

    strcpy(current_path, new_path);
    printf("directory changed: %s\r\n", current_path);
    return 1;
}

uint8_t list_directory(const char *current_path, char *buffer, uint16_t buffer_size) {
	char dir_path[MAX_PATH_LEN]; 
	strcpy(dir_path, current_path);	

    if(!get_final_path(dir_path))
    	return 0;

    DIR dir;
	if(f_opendir(&dir, dir_path) != FR_OK) {
		printf("cannot open directory: %s\r\n", dir_path);
		return 0;
	}

    buffer[0] = '\0';

    FILINFO finfo;
	while(1) {
		if(f_readdir(&dir, &finfo) != FR_OK || finfo.fname[0] == '\0')
			break;

		if(finfo.fattrib & AM_DIR )
			strcat(buffer, "+/");
		else 
			sprintf(buffer + strlen(buffer), "+r,s%lu", finfo.fsize);
		
		strcat(buffer,",\t");
		strcat(buffer, finfo.fname);
		strcat(buffer, "\r\n");
	}
	f_closedir(&dir);

	printf("directory list, success\n");
	return 1;
}

uint8_t open_dir(const char *current_path, DIR *dir) {
	char dir_path[MAX_PATH_LEN]; 
	strcpy(dir_path, current_path);	

    if(!get_final_path(dir_path))
    	return 0;

	if(f_opendir(dir, dir_path) != FR_OK) {
		printf("cannot open directory: %s\r\n", dir_path);
		return 0;
	}
	return 1;
}

uint8_t close_dir(DIR *dir) {
	f_closedir(dir);
	return 1;
}

uint8_t list_directory_part(DIR *dir, char *buffer, uint16_t buffer_size) {
	buffer[0] = '\0';

    FILINFO finfo;
	if(f_readdir(&dir, &finfo) != FR_OK || finfo.fname[0] == '\0')
		return 0;

	if(finfo.fattrib & AM_DIR )
		strcat(buffer, "+/");
	else 
		sprintf(buffer + strlen(buffer), "+r,s%lu", finfo.fsize);
		
	strcat(buffer,",\t");
	strcat(buffer, finfo.fname);
	strcat(buffer, "\r\n");	

	printf("directory list, success\n");
	return 1;
}

uint8_t open_file(const char *current_path, const char *filename, FIL *file) {
	char filepath[MAX_PATH_LEN]; 
	strcpy(filepath, current_path);	

    if(!get_final_path_2(filepath, filename))
    	return 0;    

    if(f_open(file, filepath, FA_OPEN_EXISTING | FA_READ) != FR_OK ) {
		printf("cannot open file %s\r\n", filepath);		
		return 0;
	}
    return 1;
}

uint8_t create_file(const char *current_path, const char *filename, FIL *file) {
    char filepath[MAX_PATH_LEN]; 
	strcpy(filepath, current_path);	

    if(!get_final_path_2(filepath, filename))
    	return 0;    

	if(f_open(file, filepath, FA_CREATE_NEW | FA_WRITE) != FR_OK ) {
		printf("cannot create file %s\r\n", filepath);		
		return 0;
	}
	return 1;
}

void close_file(FIL *file) {
	f_close(file);	
}

int8_t delete_file(const char *current_path, const char *filename) {
    char filepath[MAX_PATH_LEN]; 
	strcpy(filepath, current_path);	

    if(!get_final_path_2(filepath, filename))
    	return 0;    

	if(f_unlink(filepath) != FR_OK) {
		printf("cannot delete file: %s\r\n", filepath);
		return 0;
	}
	
	printf("deleted file: %s\r\n", filepath);
	return 1;
}

int8_t create_dir(const char *current_path, const char *dir_name) {
	char dir_path[MAX_PATH_LEN]; 
	strcpy(dir_path, current_path);	

    if(!get_final_path_2(dir_path, dir_name))
    	return 0;
  
    FRESULT res = f_mkdir(dir_path);
    if(res != FR_OK) {
		printf("connot make directory: %s\r\n", dir_path);
        return 0;
    }
    return 1;
}

unsigned int write_to_file(FIL *file, char *buf, uint16_t size) {
	unsigned int bw;
	if(f_write(file, buf, size, &bw) != FR_OK) {
		printf("write to file error\r\n");
		return -1;
	}
	return bw;
}

unsigned int read_file(FIL *file, char *buf, uint16_t size) {
	unsigned int br;
	if(f_read(file, buf, size, &br) != FR_OK) {
		printf("read file error\r\n");
		return -1;
	}
	return br;
}

void USB_Process(ApplicationTypeDef Appli_state) {
	 switch(Appli_state) {
		 case APPLICATION_START:
			 printf("Device connected.\r\n");
			 break;
		 case APPLICATION_READY:
			 mount_usb();
			 printf("Device ready.\r\n");
			 break;
		 case APPLICATION_DISCONNECT:
			 unmount_usb();
			 printf("Device disconnected.\r\n");
			 break;
		 default:
		 break;
	}
}
