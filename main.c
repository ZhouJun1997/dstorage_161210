#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "fdfs_api.h"
#include "make_log.h"

#define FDFS_MAIN "main"
#define FDFS_MAIN_PROC "fdfs_api"

int main(int argc, char *argv[])
{

    int retn = 0;
    char file_id[FILE_ID_LEN] = {0};

    retn = fdfs_upload_by_filename1("./test/abc.txt", file_id);
    if (retn != 0) {
        printf("fdfs_upload_error");
        return 1;
    }

    //printf("file_id = [%s]\n", file_id);
    LOG(FDFS_MAIN, FDFS_MAIN_PROC, "%s", file_id);

	return 0;
}
