#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

    int fd = open("output.txt", O_WRONLY | O_CREAT, 0644);

    dup2(fd, STDOUT_FILENO);

    printf("hello\n");

    return 0;
}