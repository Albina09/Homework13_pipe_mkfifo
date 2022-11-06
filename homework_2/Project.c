/*Обмен данными через именнованный канал*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void errorExit(char err[]);

void errorExit(char err[]){
    perror(err);
    exit(EXIT_FAILURE);
}

int main(void){
    int fd;
    char str[] = "Обмен данными через именнованный канал\n";
    char buff[100];
    pid_t pid;

    unlink("canal"); 
    
    if(mkfifo("canal", S_IRWXU) == -1)
        errorExit("fifo");
        
    pid = fork();
    if(pid == -1)
        errorExit("fork");
        
    if(pid == 0){
        
        if((fd = open("canal", O_WRONLY)) == -1)
            errorExit("open");
        write(fd, str, sizeof(str));    
        close(fd);

        exit(EXIT_SUCCESS);
    
    }else{
        
        if((fd = open("canal", O_RDONLY|O_NONBLOCK)) == -1)
            errorExit("open");
            
        if(wait(NULL) == -1)
            errorExit("wait");
            
        if(read(fd, buff, sizeof(buff)) == -1)
            errorExit("read");
           
        printf("Прочитано из fifo: %s", buff);
        close(fd);

        exit(EXIT_SUCCESS);
        
    }
}
