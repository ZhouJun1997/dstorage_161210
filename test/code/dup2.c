#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define MAXLINE 1024

int fdfs_upload_by_filename(const char* filename, char* file_id)
{
    int ret = 0;
    int pipefd[2];
    pid_t cpid;
    char file_id[MAXLINE]={0};

    if(argc != 2){
        fprintf(stderr,"Usage: Error parameters <string>\n",);
        exit(EXIT_FAILURE);
    }

    if(pipe(pipefd) == -1){
        perror("pipe err\n");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if(cpid == -1){
        perror("fork err\n");
        exit(EXIT_FAILURE);
    }

    if(cpid == 0){
        close(pipefd[0]);
        dup2(pipefd[1],STDIN_FILENO);
        execlp("fdfs_upload_file","ls","/etc/fdfs/client.conf",filename,NULL);
        perror("execlp err\n");
        exit(EXIT_FAILURE);
    }
    else if(cpid > 0){
        close(pipefd[1]);
        dup2(pipefd[0],STDOUT_FILENO);
        do{
            ret = read(pipefd[0],&file_id,1);
            if(ret == -1){
                printf("read err\n");
                break;
            }
        }while(ret > 0);
        printf("%s",file_id);
        close(pipefd[0]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
