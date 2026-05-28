#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    int fd1[2];
    int fd2[2];

    pipe(fd1);
    pipe(fd2);

    pid_t pid1 = fork();
    
    if(pid1 == 0){
        dup2(fd1[1], STDOUT_FILENO);
        
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        
        char *argv[] = {
            "ls",
            NULL
        };
        
        execvp("ls",argv);
        
        perror("excvp failed:");
        exit(1);
    }
    pid_t pid2 = fork();

    if(pid2 == 0){
        dup2(fd1[0], STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);
        
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        
        char *argv[] = {
            "grep",
            "c",
            NULL
        };
        
        execvp("grep",argv);
        
        perror("excvp failed:");
        exit(1);
    }
    pid_t pid3 = fork();
    
    if(pid3 == 0){
        dup2(fd2[0], STDIN_FILENO);

        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        char *argv[] = {
            "wc",
            NULL
        };

        execvp("wc",argv);

        perror("excvp failed:");
        exit(1);
    }

    close(fd1[0]);
    close(fd1[1]);

    close(fd2[0]);
    close(fd2[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}