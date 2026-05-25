#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

void execute_normal(char *argv[]) {

    pid_t pid = fork();

    if (pid == 0) {

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else {
        wait(NULL);
    }
}

void execute_pipe(char *left, char *right) {

    char *argv1[MAX_ARGS];
    char *argv2[MAX_ARGS];

    parse_command(left, argv1);
    parse_command(right, argv2);

    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return;
    }

    pid_t pid1 = fork();

    if (pid1 == 0) {

        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);

        execvp(argv1[0], argv1);

        perror("execvp cmd1 failed");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) {

        dup2(fd[0], STDIN_FILENO);

        close(fd[0]);
        close(fd[1]);

        execvp(argv2[0], argv2);

        perror("execvp cmd2 failed");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);
}

void execute_redirect_out(char *command, char *filename) {

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0) {

        int fd = open(filename,
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

        if (fd == -1) {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else {
        wait(NULL);
    }
}

void execute_redirect_in(char *command, char *filename) {

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0) {

        int fd = open(filename, O_RDONLY);

        if (fd == -1) {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDIN_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else {
        wait(NULL);
    }
}

void execute_append(char *command, char *filename) {

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0) {

        int fd = open(filename,
                      O_WRONLY | O_CREAT | O_APPEND,
                      0644);

        if (fd == -1) {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else {
        wait(NULL);
    }
}