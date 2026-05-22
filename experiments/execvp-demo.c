#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    printf("Before fork\n");

    pid_t pid = fork();

    if(pid < 0){
        perror("Child running\n");
        return 1;
    }

    if(pid == 0){

        char *arg[] = {
            "sl",
            "-l",
            NULL
        };
        execvp("ls",arg);

        perror("excvp failed\n");
        return 1;
    } else{
        wait(NULL);
        printf("Parent resume after child\n");
    }

    return 0;
} 