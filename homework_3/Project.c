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

int main(void){
    pid_t pid;
    int fd[2];
    char *argv[2];
    argv[0] = "ls";
    argv[1] = "-la";
    argv[2] = NULL;

    char *argv2[2];
    argv2[0] = "grep";
    argv2[1] = "a*";
    argv2[2] = NULL;

    if(pipe(fd) == -1)
        errorExit("pipe");
        

    pid = fork();
    if(pid == -1)
        errorExit("fork");
	    
    if(pid == 0){
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        
        execvp(argv2[0],argv2);
	    exit(EXIT_SUCCESS);
        
    }else{
        pid = fork();
        
        if (pid == 0){
            close(fd[0]);
            
            dup2(fd[1],1);
            
            close(fd[1]);
            execvp(argv[0], argv);

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