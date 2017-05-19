#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#include "fdfs_api.h"

int main(int argc, char *argv[])
{
    int retn = 0;
    char *filename = "abc.txt";

    char file_id[128] = {0};


    retn = fdfs_upload_by_filename(filename, file_id);
    if (retn != 0) {
        printf("uplaod error\n");
        exit(1);
    }
    printf("fileid = [%s]\n", file_id);


	return 0;
}
