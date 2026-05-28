#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

void execute_normal(char *argv[])
{

    pid_t pid = fork();

    if (pid == 0)
    {

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}

void execute_multi_pipe(char *input)
{
    char *commands[MAX_ARGS];
    int numberOfCommands = 0;

    char *token = strtok(input, "|");

    while (token != NULL)
    {
        commands[numberOfCommands++] = token;
        token = strtok(NULL, "|");
    }

    if (numberOfCommands == 1)
    {
        char *argv[MAX_ARGS];
        parse_command(commands[0], argv);
        execute_normal(argv);
        return;
    }

    int fd[MAX_ARGS][2];

    for (int i = 0; i < numberOfCommands - 1; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe failed");
            return;
        }
    }

    for (int i = 0; i < numberOfCommands; i++)
    {
        char *argv[MAX_ARGS];
        parse_command(commands[i], argv);

        pid_t pid = fork();

        if (pid == 0)
        {
            if (i > 0)
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            if (i < numberOfCommands - 1)
            {
                dup2(fd[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < numberOfCommands - 1; j++)
            {
                close(fd[j][0]);
                close(fd[j][1]);
            }

            execvp(argv[0], argv);

            perror("excvp failed");
            exit(1);
        }
    }

    for (int j = 0; j < numberOfCommands - 1; j++)
    {
        close(fd[j][0]);
        close(fd[j][1]);
    }

    for (int j = 0; j < numberOfCommands; j++)
    {
        wait(NULL);
    }
}

void execute_redirect_out(char *command, char *filename)
{

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0)
    {

        int fd = open(filename,
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

        if (fd == -1)
        {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}

void execute_redirect_in(char *command, char *filename)
{

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0)
    {

        int fd = open(filename, O_RDONLY);

        if (fd == -1)
        {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDIN_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}

void execute_append(char *command, char *filename)
{

    char *argv[MAX_ARGS];

    parse_command(command, argv);

    pid_t pid = fork();

    if (pid == 0)
    {

        int fd = open(filename,
                      O_WRONLY | O_CREAT | O_APPEND,
                      0644);

        if (fd == -1)
        {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);

        execvp(argv[0], argv);

        perror("execvp failed");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}