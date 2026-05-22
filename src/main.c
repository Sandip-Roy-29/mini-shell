#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>

int main() {
    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        char input[100];
        char *argv[10];

        if(fgets(input, sizeof(input), stdin) == NULL){
            break;
        }

        char *token = strtok(input, " \n");

        int i = 0;

        while (token != NULL) {
            argv[i++] = token;
            token = strtok(NULL, " \n");
        }

        argv[i] = NULL;

        if(argv[0] == NULL) continue;

        pid_t pid = fork();
        
        if(pid == 0){
            execvp(argv[0],argv);

            perror("excvp failed");
            return 1;
        }else{
            wait(NULL);
        }
    }
    return 0;
}