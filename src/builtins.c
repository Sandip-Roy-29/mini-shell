#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "builtins.h"

int handle_builtin(char *argv[]) {

    if (argv[0] == NULL) return 1;

    if (strcmp(argv[0], "cd") == 0) {

        if (argv[1] == NULL) {
            printf("cd: missing argument\n");
        }
        else if (chdir(argv[1]) != 0) {
            perror("cd failed");
        }

        return 1;
    }

    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }

    return 0;
}