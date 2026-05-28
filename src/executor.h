#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_normal(char *argv[]);
void execute_multi_pipe(char *input);
void execute_redirect_out(char *command, char *filename);
void execute_redirect_in(char *command, char *filename);
void execute_append(char *command, char *filename);

#endif