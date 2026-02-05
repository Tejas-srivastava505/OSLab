#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *fib, n;

void *fibonacci(void *arg) {
    if (n >= 1) fib[0] = 0;
    if (n >= 2) fib[1] = 1;

    for (int i = 2; i < n; i++)
        fib[i] = fib[i-1] + fib[i-2];

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    /* Argument check */
    if (argc != 2) {
        printf("Usage: %s <number_of_fibonacci_numbers>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        printf("Error: Enter a positive number.\n");
        return 1;
    }

    /* Allocate memory */
    fib = malloc(n * sizeof(int));
    if (!fib) {
        perror("malloc failed");
        return 1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, fibonacci, NULL);

    /* Wait for child thread to finish */
    pthread_join(tid, NULL);

    /* Print Fibonacci series */
    printf("Fibonacci Series:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", fib[i]);
    printf("\n");

    free(fib);
    return 0;
}
