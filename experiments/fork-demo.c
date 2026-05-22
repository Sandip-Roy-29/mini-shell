#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Before fork\n");

    int pid = fork();

    if (pid == 0) {
        printf("I am child\n");
    } else {
        printf("I am parent, child pid = %d\n", pid);
    }

    printf("Shared line\n");
}