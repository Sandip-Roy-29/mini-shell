#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "executor.h"
#include "builtins.h"

int main()
{

    while (1)
    {

        printf("mini-shell> ");
        fflush(stdout);

        char input[200];

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        CommandType type = detect_type(input);

        if (type == NORMAL)
        {

            char *argv[MAX_ARGS];

            parse_command(input, argv);

            if (handle_builtin(argv))
                continue;

            execute_normal(argv);
        }

        else if (type == PIPE)
        {

            char *pipe_pos = strchr(input, '|');

            *pipe_pos = '\0';

            char *left = input;
            char *right = pipe_pos + 1;

            execute_pipe(left, right);
        }

        else if (type == REDIRECT_OUT)
        {

            char *redir = strchr(input, '>');

            *redir = '\0';

            char *command = input;
            char *filename = redir + 1;

            while (*filename == ' ')
                filename++;

            if (*filename == '\0')
            {
                printf("missing filename\n");
                continue;
            }

            execute_redirect_out(command, filename);
        }

        else if (type == REDIRECT_IN)
        {

            char *redir = strchr(input, '<');

            *redir = '\0';

            char *command = input;
            char *filename = redir + 1;

            while (*filename == ' ')
                filename++;

            if (*filename == '\0')
            {
                printf("missing filename\n");
                continue;
            }

            execute_redirect_in(command, filename);
        }

        else if (type == APPEND)
        {

            char *redir = strstr(input, ">>");

            *redir = '\0';

            char *command = input;
            char *filename = redir + 2;

            while (*filename == ' ')
                filename++;

            if (*filename == '\0')
            {
                printf("missing filename\n");
                continue;
            }

            execute_append(command, filename);
        }
    }

    return 0;
}