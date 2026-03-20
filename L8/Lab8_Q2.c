#include <stdio.h>

#define P 5
#define R 3

void detectDeadlock(int alloc[P][R], int request[P][R], int avail[R]) {
    int work[R], finish[P];

    for(int i=0;i<R;i++)
        work[i] = avail[i];

    for(int i=0;i<P;i++)
        finish[i] = 0;

    int found;
    do {
        found = 0;

        for(int i=0;i<P;i++) {
            if(!finish[i]) {
                int j;
                for(j=0;j<R;j++) {
                    if(request[i][j] > work[j])
                        break;
                }

                if(j == R) {
                    for(int k=0;k<R;k++)
                        work[k] += alloc[i][k];

                    finish[i] = 1;
                    found = 1;
                }
            }
        }

    } while(found);

    int deadlock = 0;
    printf("\nDeadlock Detection Result:\n");

    for(int i=0;i<P;i++) {
        if(!finish[i]) {
            printf("Process P%d is in deadlock\n", i);
            deadlock = 1;
        }
    }

    if(!deadlock)
        printf("Deadlock is not detected\n");
}

void addRequest(int request[P][R], int process, int extra[R]) {
    for(int j=0;j<R;j++)
        request[process][j] += extra[j];
}

int main() {

    int alloc[P][R] = {
        {0,1,0},
        {2,0,0},
        {3,0,3},
        {2,1,1},
        {0,0,2}
    };

    int request[P][R] = {
        {0,0,0},
        {2,0,2},
        {0,0,0},
        {1,0,0},
        {0,0,2}
    };

    int avail[R] = {0,0,0};

    detectDeadlock(alloc, request, avail);

    int extra[R] = {0,0,1};
    printf("\nAdding extra request (0,0,1) to P2...\n");
    addRequest(request, 2, extra);

    detectDeadlock(alloc, request, avail);

    return 0;
}