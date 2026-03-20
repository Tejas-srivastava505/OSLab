/*
 * Banker's Algorithm for Deadlock Avoidance
 * Resources: A=10, B=5, C=7
 * Processes: P0-P4 (fixed snapshot from lab exercise)
 */

#include <stdio.h>
#include <stdbool.h>

#define N 5   // number of processes
#define R 3   // number of resources

int allocation[N][R] = {
    {0,1,0},
    {2,0,0},
    {3,0,2},
    {2,1,1},
    {0,0,2}
};

int max_matrix[N][R] = {
    {7,5,3},
    {3,2,2},
    {9,0,2},
    {2,2,2},
    {4,3,3}
};

int need[N][R];
int available[R] = {3, 3, 2}; // Total[10,5,7] - sum(Allocation)

void calculateNeed() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max_matrix[i][j] - allocation[i][j];
}

void printMatrix(const char *name, int matrix[][R]) {
    printf("\n%s Matrix:\n", name);
    printf("%-5s", "");
    for (int j = 0; j < R; j++)
        printf("  R%d", j);
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf("P%-4d", i);
        for (int j = 0; j < R; j++)
            printf("%4d", matrix[i][j]);
        printf("\n");
    }
}

void printAvailable() {
    printf("\nAvailable: ");
    for (int j = 0; j < R; j++)
        printf("R%d=%d  ", j, available[j]);
    printf("\n");
}

bool isSafe(int safe_seq[]) {
    int work[R];
    bool finish[N] = {false};

    for (int j = 0; j < R; j++)
        work[j] = available[j];

    int count = 0;
    while (count < N) {
        bool found = false;
        for (int i = 0; i < N; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int j = 0; j < R; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    safe_seq[count++] = i;
                    found = true;
                }
            }
        }
        if (!found) break;
    }
    return (count == N);
}

void resetSnapshot() {
    int alloc[N][R] = {
        {0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}
    };
    int mx[N][R] = {
        {7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}
    };
    for (int i = 0; i < N; i++)
        for (int j = 0; j < R; j++) {
            allocation[i][j] = alloc[i][j];
            max_matrix[i][j] = mx[i][j];
        }
    available[0] = 3; available[1] = 3; available[2] = 2;
    calculateNeed();
}

void resourceRequest(int process, int request[]) {
    printf("\n--- Resource Request for P%d ---\n", process);
    printf("Request: ");
    for (int j = 0; j < R; j++)
        printf("R%d=%d  ", j, request[j]);
    printf("\n");

    // Step 1: Check request <= need
    for (int j = 0; j < R; j++) {
        if (request[j] > need[process][j]) {
            printf("ERROR: Request exceeds maximum need for R%d.\n", j);
            return;
        }
    }

    // Step 2: Check request <= available
    for (int j = 0; j < R; j++) {
        if (request[j] > available[j]) {
            printf("Process P%d must WAIT. Resources not available.\n", process);
            return;
        }
    }

    // Step 3: Pretend to allocate
    for (int j = 0; j < R; j++) {
        available[j]           -= request[j];
        allocation[process][j] += request[j];
        need[process][j]       -= request[j];
    }

    // Step 4: Safety check
    int safe_seq[N];
    if (isSafe(safe_seq)) {
        printf("Request GRANTED. System remains in safe state.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < N; i++)
            printf("P%d%s", safe_seq[i], (i < N - 1) ? " -> " : "\n");
        printf("\nUpdated matrices after granting request:\n");
        printMatrix("Allocation", allocation);
        printMatrix("Need", need);
        printAvailable();
    } else {
        printf("Request DENIED. Granting it would lead to an UNSAFE state.\n");
        // Rollback
        for (int j = 0; j < R; j++) {
            available[j]           += request[j];
            allocation[process][j] -= request[j];
            need[process][j]       += request[j];
        }
    }
}

int main() {
    calculateNeed();

    // (a) Need matrix
    printf("========== (a) Need Matrix ==========");
    printMatrix("Need", need);

    // (b) Safety check
    printf("\n========== (b) Safety State Check ==========\n");
    int safe_seq[N];
    if (isSafe(safe_seq)) {
        printf("System is in a SAFE STATE.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < N; i++)
            printf("P%d%s", safe_seq[i], (i < N - 1) ? " -> " : "\n");
    } else {
        printf("System is in an UNSAFE STATE.\n");
    }

    // (c) P1 requests (1,0,2)
    printf("\n========== (c) P1 requests (1,0,2) ==========");
    resetSnapshot();
    int req_c[R] = {1, 0, 2};
    resourceRequest(1, req_c);

    // (d) P4 requests (3,3,0)
    printf("\n========== (d) P4 requests (3,3,0) ==========");
    resetSnapshot();
    int req_d[R] = {3, 3, 0};
    resourceRequest(4, req_d);

    // (e) P0 requests (0,2,0)
    printf("\n========== (e) P0 requests (0,2,0) ==========");
    resetSnapshot();
    int req_e[R] = {0, 2, 0};
    resourceRequest(0, req_e);

    return 0;
}