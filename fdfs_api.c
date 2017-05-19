/**
* Copyright (C) 2008 Happy Fish / YuQing
*
* FastDFS may be copied only under the terms of the GNU General
* Public License V3, which may be found in the FastDFS source kit.
* Please visit the FastDFS Home Page http://www.csource.org/ for more detail.
**/

#include "fdfs_api.h"


int fdfs_upload_by_filename1(const char *filename, char *file_id)
{
    int retn = 0;
    int pfd[2];

    retn = pipe(pfd);
    pid_t pid;


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
        execlp("fdfs_upload_file", "fdfs_upload_file", "/etc/fdfs/client.conf", filename, NULL);
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
        //printf("fileid=[%s]\n", file_id);
        close(pfd[0]);
    }


	return 0;
}


/* -------------------------------------------*/
/**
 * @brief  fdfs_upload_by_filename
 *
 * @param filename 要上传文件路径
 * @param file_id 得到已经上传成功的fileid
 *
 * @returns   
 *       0 succ, -1 fail
 */
/* -------------------------------------------*/
int fdfs_upload_by_filename(const char *filename, char *file_id)
{
	const char *local_filename;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;

	log_init();
	g_log_context.log_level = LOG_ERR;
	ignore_signal_pipe();

    //加载一些配置文件
	if ((result=fdfs_client_init("/etc/fdfs/client.conf")) != 0)
	{
		return result;
	}

    //通过配置问价拿到内容 得到一个tracker链接的句柄
	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

	local_filename = filename;
	*group_name = '\0';

    //通过询问tracker句柄 得到一个可用的storage句柄
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		return result;
	}

    //根据得到的storage句柄 上传本地local_filename
	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
        //上传succ
		printf("%s\n", file_id);
	}
	else
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

    //断开链接 回收资源
	tracker_disconnect_server_ex(pTrackerServer, true);
    
	fdfs_client_destroy();

	return result;
}

