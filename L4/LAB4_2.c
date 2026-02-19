#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    if (argc > 1 && strcmp(argv[1], "child") == 0)
    {
        printf("This is the LAB4_2 program logic\n");
        return 0;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Child process: executing LAB4_2...\n");

        execl("./LAB4_2", "LAB4_2", "child", NULL);

        perror("exec failed");
        return 1;
    }
    else
    {
        wait(NULL);
        printf("Parent process: execution complete.\n");
    }

    return 0;
}
