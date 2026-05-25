#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    char *argv[] = {"ls",NULL};

    pid_t pid = fork();

    if(pid == 0){
        int fd = open("out.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);

        dup2(fd,STDOUT_FILENO);

        close(fd);

        execvp(argv[0],argv);

        perror("excvp failed");
        return 1;
    } else {
        wait(NULL);
    }

    return 0;
}