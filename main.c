nclude <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "logger.h"

int main(int argc,const char*argv[])
{
    int ret = 0;

    char *filename = "/home/itheima/2222/epoll.c";
        
    char file_id[128] = {0};
    ret = int fdfs_upload_by_filename(const char*filename, char *file_id);
    if(ret != 0)
    {
        printf("fdfs_upload_by_filename err\n");
    }
    printf("fdfs_upload_by_filename succes\n");

    return 0;
}
