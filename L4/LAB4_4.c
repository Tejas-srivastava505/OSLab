#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n;

/* Bubble Sort Thread */
void *bubble_sort(void *arg)
{
    int *a = (int *)arg;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Bubble Sort Time: %f seconds\n", time);
    pthread_exit(NULL);
}

/* Selection Sort Thread */
void *selection_sort(void *arg)
{
    int *a = (int *)arg;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[min])
                min = j;

        int t = a[i];
        a[i] = a[min];
        a[min] = t;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Selection Sort Time: %f seconds\n", time);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    if (argc < 3) {
        printf("Usage: %s <number_of_elements> <elements...>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    if (argc != n + 2) {
        printf("Error: Expected %d elements\n", n);
        return 1;
    }

    int a1[n], a2[n];

    for (int i = 0; i < n; i++) {
        a1[i] = atoi(argv[i + 2]);
        a2[i] = a1[i];
    }

    pthread_t t1, t2;

    pthread_create(&t1, NULL, bubble_sort, a1);
    pthread_create(&t2, NULL, selection_sort, a2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nFinal Sorted Array:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", a1[i]);
    printf("\n");

    return 0;
}
