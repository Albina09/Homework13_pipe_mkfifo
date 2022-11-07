#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void errorExit(char err[]);

void errorExit(char err[]){
    perror(err);
    exit(EXIT_FAILURE);
}

int main(){
    pid_t pid, pid2;
    int fd[2];
    char *argv[4];
    argv[0] = "ls";
    argv[1] = "-la";
    argv[2] = "tail";
    argv[3] = "-1";

    if(pipe(fd) == -1)
        errorExit("pipe");
        

    pid = fork();
    if(pid == -1)
        errorExit("fork");
	    
    if(pid == 0){
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        execlp(argv[2], argv[2], argv[3],(char*) NULL);
	    exit(EXIT_SUCCESS);
        
    }else{
        pid2 = fork();
        
        if (pid2 == 0){
            close(fd[0]);
            
            dup2(fd[1],1);
            
            close(fd[1]);
            execlp(argv[0],argv[0],argv[1],(char*) NULL); 
            exit(EXIT_SUCCESS);

        }else{
            close(fd[0]);
            close(fd[1]);      

            waitpid(-1, NULL, 0);
            waitpid(-1, NULL, 0);

            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}