/*
 * Deadlock Detection Algorithm
 * Resources: A, B, C (Available = 0,0,0)
 * Processes: P0-P4
 *
 * Allocation:        Request:
 * P0: 0 1 0          0 0 0
 * P1: 2 0 0          2 0 2
 * P2: 3 0 3          0 0 0
 * P3: 2 1 1          1 0 0
 * P4: 0 0 2          0 0 2
 */

#include <stdio.h>
#include <stdbool.h>

#define N 5   // number of processes
#define R 3   // number of resources

int allocation[N][R] = {
    {0,1,0},
    {2,0,0},
    {3,0,3},
    {2,1,1},
    {0,0,2}
};

// Request matrix (what each process is currently requesting)
int request[N][R] = {
    {0,0,0},
    {2,0,2},
    {0,0,0},
    {1,0,0},
    {0,0,2}
};

int available[R] = {0, 0, 0};

// -------------------------------------------------------
// Detection Algorithm
// Returns true if NO deadlock (all can finish), false if deadlock
// deadlocked[] marks which processes are deadlocked
// -------------------------------------------------------
bool detect(bool deadlocked[]) {
    int work[R];
    bool finish[N];

    // Processes with zero allocation are marked finished immediately
    for (int i = 0; i < N; i++) {
        finish[i] = false;
        bool all_zero = true;
        for (int j = 0; j < R; j++)
            if (allocation[i][j] != 0) { all_zero = false; break; }
        if (all_zero) finish[i] = true;
    }

    for (int j = 0; j < R; j++)
        work[j] = available[j];

    // Find a process whose request can be satisfied
    bool progress = true;
    while (progress) {
        progress = false;
        for (int i = 0; i < N; i++) {
            if (finish[i]) continue;
            bool can_run = true;
            for (int j = 0; j < R; j++) {
                if (request[i][j] > work[j]) { can_run = false; break; }
            }
            if (can_run) {
                for (int j = 0; j < R; j++)
                    work[j] += allocation[i][j];
                finish[i] = true;
                progress = true;
            }
        }
    }

    bool deadlock_found = false;
    for (int i = 0; i < N; i++) {
        deadlocked[i] = !finish[i];
        if (!finish[i]) deadlock_found = true;
    }
    return !deadlock_found;
}

void printState(const char *label) {
    printf("\n%s\n", label);
    printf("%-5s  %-12s  %-12s\n", "", "Allocation", "Request");
    printf("%-5s  %-2s %-2s %-6s  %-2s %-2s %-2s\n",
           "", "A", "B", "C", "A", "B", "C");
    for (int i = 0; i < N; i++) {
        printf("P%-4d  %2d %2d %2d      %2d %2d %2d\n", i,
               allocation[i][0], allocation[i][1], allocation[i][2],
               request[i][0],    request[i][1],    request[i][2]);
    }
    printf("Available:  A=%d  B=%d  C=%d\n",
           available[0], available[1], available[2]);
}

// -------------------------------------------------------
// Main
// -------------------------------------------------------
int main() {

    // -------- Part (a): Is the system in a safe state? --------
    printf("========== (a) Deadlock Detection ==========\n");
    printState("Current System State:");

    bool deadlocked[N];
    bool no_deadlock = detect(deadlocked);

    if (no_deadlock) {
        printf("\nResult: System is in a SAFE STATE. No deadlock.\n");
    } else {
        printf("\nResult: DEADLOCK DETECTED!\n");
        printf("Deadlocked processes: ");
        for (int i = 0; i < N; i++)
            if (deadlocked[i]) printf("P%d ", i);
        printf("\n");
    }

    // -------- Part (b): P2 makes one additional request for C --------
    printf("\n========== (b) P2 additionally requests (0,0,1) for C ==========\n");

    // Update P2's request by adding 1 unit of C
    request[2][2] += 1;

    printState("Updated System State:");

    bool deadlocked2[N];
    bool no_deadlock2 = detect(deadlocked2);

    if (no_deadlock2) {
        printf("\nResult: System is STILL in a SAFE STATE. No deadlock.\n");
    } else {
        printf("\nResult: DEADLOCK DETECTED after P2's additional request!\n");
        printf("Deadlocked processes: ");
        for (int i = 0; i < N; i++)
            if (deadlocked2[i]) printf("P%d ", i);
        printf("\n");
    }

    return 0;
}