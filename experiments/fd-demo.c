#include <stdio.h>
#include <fcntl.h>

int main() {

    int fd = open("fork-demo.c", O_RDONLY);

    if(fd == -1){
        perror("Open failed");
        return 1;
    }

    printf("fd = %d\n", fd);

    return 0;
}