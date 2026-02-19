#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        printf("Child exiting\n");
        exit(0);
    }
    else {
        printf("Parent sleeping (no wait)\n");
        sleep(15);
    }
    return 0;
}
