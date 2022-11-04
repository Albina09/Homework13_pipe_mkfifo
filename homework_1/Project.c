/*Обмен данными через не именнованный канал*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void errorExit(char err[]);

void errorExit(char err[]){
    perror(err);
    exit(EXIT_FAILURE);
}

int main(void)
{
    int fd[2];
    pid_t pid;
    char buff;
    int status;
    char wr_parent[] = "Родитель прочитал значение, и отправил его потомку, прочитал потомок и вывел на экран\n";

    if(pipe(fd) == -1)
        errorExit("pipe");
        
    pid = fork();
    if(pid == -1)
        errorExit("fork");

    if(pid == 0){
        close(fd[1]);                                //закрыли fd[1], тк потомок принимает данные от родителя по каналу
        
        while(read(fd[0], &buff, 1) > 0)             //потомок читает по одному символу пока не закончит
            printf("%c", buff);                      //выводит на экран
        close(fd[0]);

        exit(EXIT_SUCCESS);
    }else{
        close(fd[0]);                                 //закрыли fd[0], тк родитель посылает данные потомку
        write(fd[1], wr_parent, strlen(wr_parent));   //родитель прочитал значение wr_parent и передал его в fd[1]
        close(fd[1]);

        if (wait(NULL) == -1)
	        errorExit("wait");
	        
        exit(EXIT_SUCCESS);
    }


}