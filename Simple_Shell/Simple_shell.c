#include "Simple_shell.h"


void shell() {
    
    int fd_log = open("Simple_shell.log",O_WRONLY | O_APPEND | O_CREAT, 0755);
    if (fd_log < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    //读取标准输入到字符串中
    char command[100];
    ssize_t read_ret = read(STDIN_FILENO,command,(sizeof(command)-1)); 
    if(read_ret == -1){
        perror("read");
    }
    else if(read_ret == 0){
        return;
    }
    else{
        command[read_ret] = '\0';
        if(command[read_ret - 1] == '\n') {
            command[read_ret -1 ] = '\0';
        }
    }
    
    while(1) {

        pid_t child = fork();

        if (child == -1) {
            perror("fork");
        }
        else if (child == 0) {
            close(fd_log);
            char* token = strtok(command," ");


            char* args[MAX_ARGS];
            int i = 0;
            while (token != NULL && i < MAX_ARGS - 1)
            {
                args[i] = token;
                i++;
                token = strtok(NULL," ");
            }
            args[i] = NULL;

            if (args[0] == NULL) {
                _exit(EXIT_FAILURE);
            }
            execvp(args[0],args);

            perror("execvp");
            
        }
        else {

            //将命令，PID，退出状态写入文件

            // char* prefix01 = "执行命令:";
            // write(fd_log,prefix01,strlen(prefix01));
            // write(fd_log,command,read_ret);
            // write(fd_log,"\n\n",1);

            dprintf(fd_log,"执行命令: %s",command);

            // char* prefix02 = "子进程的PID:";
            // write(fd_log,prefix02,strlen(prefix02));
            // char child_pid[10] = {};
            // size_t len = snprintf(child_pid,sizeof(child_pid),"%d",child);
            // write(fd_log,child_pid,len);
            // write(fd_log,"\n\n",1);

            dprintf(fd_log,"子进程的PID: %d\n",child);

            int status;
            waitpid(child,&status,WNOHANG);
            if(WIFEXITED(status)) {

                // char* prefix03 = "正常退出，返回值:";
                // write(fd_log,prefix03,strlen(prefix03));

                // char status_code[10] = {};
                // size_t len = snprintf(status_code,sizeof(status_code),"%d",WEXITSTATUS(status));
                // write(fd_log,status_code,len);
                // write(fd_log,"\n\n",1);

                dprintf(fd_log,"正常退出，返回值: %d\n\n",WEXITSTATUS(status));
            }
            else {

                // char* prefix04 = "异常，被终止";
                // write(fd_log,prefix04,strlen(prefix04));
                // write(fd_log,"\n\n",1);

                dprintf(fd_log,"异常，被终止\n\n");
                close(fd_log);
                exit(EXIT_FAILURE);
            }
        }
    }

    close(fd_log);
    
}