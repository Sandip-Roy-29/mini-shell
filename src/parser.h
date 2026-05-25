#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 20

typedef enum {
    NORMAL,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    APPEND
} CommandType;

CommandType detect_type(char *input);
void parse_command(char *input, char *argv[]);

#endif