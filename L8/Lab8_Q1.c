#include <stdio.h>
#include <stdbool.h>

#define P 5 // Number of processes
#define R 3 // Number of resources

int main() {
    int Allocation[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int Max[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int Available[R] = {3, 3, 2};
    int Need[P][R];

    // Step (a): Calculate Need matrix
    for(int i=0;i<P;i++){
        for(int j=0;j<R;j++){
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    printf("Need Matrix:\n");
    for(int i=0;i<P;i++){
        for(int j=0;j<R;j++){
            printf("%d ", Need[i][j]);
        }
        printf("\n");
    }

    // Function to check if system is in safe state
    bool isSafe() {
        int Work[R];
        bool Finish[P];
        for(int i=0;i<R;i++) Work[i] = Available[i];
        for(int i=0;i<P;i++) Finish[i] = false;

        bool found;
        int count = 0;
        do {
            found = false;
            for(int i=0;i<P;i++){
                if(!Finish[i]){
                    int j;
                    for(j=0;j<R;j++)
                        if(Need[i][j] > Work[j]) break;
                    if(j==R){
                        for(int k=0;k<R;k++) Work[k] += Allocation[i][k];
                        Finish[i] = true;
                        found = true;
                        count++;
                    }
                }
            }
        } while(found);

        return (count == P);
    }

    // Step (b): Check safe state
    if(isSafe())
        printf("The system is in a SAFE state.\n");
    else
        printf("The system is NOT in a safe state.\n");

    // Function to handle resource requests
    void requestResources(int process, int request[R]) {
        printf("\nRequest from P%d: ", process);
        for(int i=0;i<R;i++) printf("%d ", request[i]);
        printf("\n");

        // Check if request <= Need
        for(int i=0;i<R;i++){
            if(request[i] > Need[process][i]){
                printf("Error: Request exceeds Need.\n");
                return;
            }
        }

        // Check if request <= Available
        for(int i=0;i<R;i++){
            if(request[i] > Available[i]){
                printf("Request cannot be granted immediately (not enough available).\n");
                return;
            }
        }

        // Try to allocate temporarily
        for(int i=0;i<R;i++){
            Available[i] -= request[i];
            Allocation[process][i] += request[i];
            Need[process][i] -= request[i];
        }

        if(isSafe()){
            printf("Request can be granted.\n");
        } else {
            printf("Request cannot be granted (system would be unsafe).\n");
            // Rollback
            for(int i=0;i<R;i++){
                Available[i] += request[i];
                Allocation[process][i] -= request[i];
                Need[process][i] += request[i];
            }
        }

        // Display updated matrices
        printf("Updated Allocation Matrix:\n");
        for(int i=0;i<P;i++){
            for(int j=0;j<R;j++) printf("%d ", Allocation[i][j]);
            printf("\n");
        }

        printf("Updated Need Matrix:\n");
        for(int i=0;i<P;i++){
            for(int j=0;j<R;j++) printf("%d ", Need[i][j]);
            printf("\n");
        }

        printf("Updated Available Vector:\n");
        for(int i=0;i<R;i++) printf("%d ", Available[i]);
        printf("\n");
    }

    // Step (c), (d), (e)
    int req1[R] = {1,0,2};
    requestResources(1, req1);

    int req2[R] = {3,3,0};
    requestResources(4, req2);

    int req3[R] = {0,2,0};
    requestResources(0, req3);

    return 0;
}