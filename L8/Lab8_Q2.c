#include <stdio.h>
#include <stdbool.h>

#define P 5   // Number of processes
#define R 3   // Number of resource types

int main() {
    // Allocation Matrix
    int allocation[P][R] = {
        {0,1,0},   // P0
        {2,0,0},   // P1
        {3,0,3},   // P2
        {2,1,1},   // P3
        {0,0,2}    // P4
    };

    // Request Matrix
    int request[P][R] = {
        {0,0,0},   // P0
        {2,0,2},   // P1
        {0,0,0},   // P2
        {1,0,0},   // P3
        {0,0,2}    // P4
    };

    // Available Resources
    int available[R] = {0,0,0};

    bool finish[P] = {false};
    int work[R];

    // Step 1: Initialize work = available
    for (int i = 0; i < R; i++)
        work[i] = available[i];

    // Step 2: Mark processes with zero allocation as finished
    for (int i = 0; i < P; i++) {
        bool zero = true;
        for (int j = 0; j < R; j++) {
            if (allocation[i][j] != 0) {
                zero = false;
                break;
            }
        }
        if (zero)
            finish[i] = true;
    }

    // Step 3: Find processes that can finish
    bool found;
    do {
        found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (request[i][j] > work[j])
                        break;
                }

                if (j == R) {
                    // Process can finish
                    for (int k = 0; k < R; k++)
                        work[k] += allocation[i][k];

                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);

    // Step 4: Check for deadlock
    printf("Deadlocked processes:\n");
    bool deadlock = false;
    for (int i = 0; i < P; i++) {
        if (!finish[i]) {
            printf("P%d ", i);
            deadlock = true;
        }
    }

    if (!deadlock)
        printf("No deadlock detected.\n");

    return 0;
}