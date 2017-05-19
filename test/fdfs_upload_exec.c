#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/wait.h>


int main(int argc, char *argv[])
{
    int retn = 0;
    int pfd[2];
    char file_id[128] = {0};

    retn = pipe(pfd);
    pid_t pid;

    if (argc < 2) {
        printf("./a.out filename \n");
        exit(1);
    }

    if (retn < 0) {
        perror("pipe error\n");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        //child
        //关闭一些fd
        close(pfd[0]);
        //dup stdout ->pfd[1]
        dup2(pfd[1], STDOUT_FILENO);
        //exec ---> fdfs_upload_file  /etc/fdfs/client.conf  argv[1]
        execlp("fdfs_upload_file", "fdfs_upload_file", "/etc/fdfs/client.conf", argv[1], NULL);
        perror("exec fdfs_upload_file error");
    }
    else {
        int len = 0;
        //parent
        //关闭一些fd
        close(pfd[1]);
        //wait
        wait(NULL);

        //read(pfd[0]) --->buf
        read(pfd[0], file_id, 128);
        len = strlen(file_id);
        file_id[len-1]  = '\0';
        printf("fileid=[%s]\n", file_id);
        close(pfd[0]);
    }


	return 0;
}
