#include<string.h>
#include "parser.h"

CommandType detect_type(char *input){
    if (strstr(input, ">>"))
        return APPEND;

    if (strchr(input, '|'))
        return PIPE;

    if (strchr(input, '>'))
        return REDIRECT_OUT;

    if (strchr(input, '<'))
        return REDIRECT_IN;

    return NORMAL;
}

void parse_command(char *input, char *argv[]){
    int i = 0;

    char *token = strtok(input, " ");

    while(token != NULL && i < MAX_ARGS - 1){
        argv[i++] = token;
        token = strtok(NULL," ");
    }
    argv[i] = NULL;
}