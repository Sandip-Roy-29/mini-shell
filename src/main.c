#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    while (1)
    {
        printf("mini-shell> ");
        fflush(stdout);

        char input[100];
        char *argv[10];

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        char *pipe_pos = strchr(input, '|');
        char *red1_pos = strchr(input, '>');
        char *red2_pos = strchr(input, '<');
        char *red3_pos = strchr(input, '>>');

        if (pipe_pos == NULL)
        {

            char *token = strtok(input, " ");
            int i = 0;

            while (token != NULL)
            {
                argv[i++] = token;
                token = strtok(NULL, " ");
            }

            argv[i] = NULL;
            if (argv[0] == NULL)
                continue;

            if (strcmp(argv[0], "cd") == 0)
            {
                if (argv[1] == NULL)
                {
                    printf("cd: missing arguments\n");
                }
                else
                {
                    if (chdir(argv[1]) != 0)
                    {
                        perror("cd failed");
                    }
                }
                continue;
            }

            pid_t pid = fork();

            if (pid == 0)
            {
                execvp(argv[0], argv);

                perror("excvp failed");
                return 1;
            }
            else
            {
                wait(NULL);
            }
        }
        else
        {
            *pipe_pos = '\0';

            char *cmd1 = input;
            char *cmd2 = pipe_pos + 1;

            char *argv1[10];
            char *token1 = strtok(cmd1, " ");
            int i = 0;

            while (token1 != NULL)
            {
                argv1[i++] = token1;
                token1 = strtok(NULL, " ");
            }

            argv1[i] = NULL;

            char *argv2[10];
            char *token2 = strtok(cmd2, " ");
            int j = 0;

            while (token2 != NULL)
            {
                argv2[j++] = token2;
                token2 = strtok(NULL, " ");
            }

            argv2[j] = NULL;

            if (argv1[0] == NULL || argv2[0] == NULL)
            {
                printf("invalid pipe command\n");
                continue;
            }

            int fd[2];

            if (pipe(fd) == -1)
            {
                perror("pipe failed");
                continue;
            }

            pid_t pid1 = fork();

            if (pid1 == 0)
            {
                dup2(fd[1], STDOUT_FILENO);

                close(fd[0]);
                close(fd[1]);

                execvp(argv1[0], argv1);

                perror("excvp failed");
                return 1;
            }

            pid_t pid2 = fork();

            if (pid2 == 0)
            {
                dup2(fd[0], STDIN_FILENO);

                close(fd[0]);
                close(fd[1]);

                execvp(argv2[0], argv2);

                perror("excvp failed");
                return 1;
            }

            close(fd[0]);
            close(fd[1]);

            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}