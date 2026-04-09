#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Function to sort array
void sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print_header(char* title) {
    printf("\n========================================================\n");
    printf(" %s\n", title);
    printf("========================================================\n");
}

// FCFS Algorithm
void fcfs(int req[], int n, int head) {
    int seek_count = 0;
    print_header("First Come First Serve (FCFS)");
    printf(" Movement Path: [%d]", head);
    
    for (int i = 0; i < n; i++) {
        printf(" -> %d", req[i]);
        seek_count += abs(head - req[i]);
        head = req[i];
    }
    printf("\n\n Total Head Movement:- %d cylinders\n", seek_count);
}

// SSTF Algorithm
void sstf(int req[], int n, int head) {
    int seek_count = 0;
    int visited[MAX] = {0};
    print_header("Shortest Seek Time First (SSTF)");
    printf(" Movement Path: [%d]", head);

    for (int i = 0; i < n; i++) {
        int min = 999999, index = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int dist = abs(head - req[j]);
                if (dist < min) {
                    min = dist;
                    index = j;
                }
            }
        }
        visited[index] = 1;
        printf(" -> %d", req[index]);
        seek_count += abs(head - req[index]);
        head = req[index];
    }
    printf("\n\n  Total Head Movement:- %d cylinders\n", seek_count);
}

// SCAN Algorithm
void scan(int req[], int n, int head, int disk_size, int direction) {
    int seek_count = 0;
    int left[MAX], right[MAX];
    int l = 0, r = 0;
    
    for (int i = 0; i < n; i++) {
        if (req[i] < head) left[l++] = req[i];
        else right[r++] = req[i];
    }
    sort(left, l);
    sort(right, r);

    print_header("SCAN (Elevator Strategy)");
    printf(" Direction: %s\n", direction == 1 ? "Increasing (Right)" : "Decreasing (Left)");
    printf(" Movement Path: [%d]", head);

    if (direction == 1) { // right
        for (int i = 0; i < r; i++) {
            printf(" -> %d", right[i]);
            seek_count += abs(head - right[i]);
            head = right[i];
        }
        printf(" -> %d (End)", disk_size - 1);
        seek_count += abs(head - (disk_size - 1));
        head = disk_size - 1;
        for (int i = l - 1; i >= 0; i--) {
            printf(" -> %d", left[i]);
            seek_count += abs(head - left[i]);
            head = left[i];
        }
    } else { // left
        for (int i = l - 1; i >= 0; i--) {
            printf(" -> %d", left[i]);
            seek_count += abs(head - left[i]);
            head = left[i];
        }
        printf(" -> 0 (Start)");
        seek_count += abs(head - 0);
        head = 0;
        for (int i = 0; i < r; i++) {
            printf(" -> %d", right[i]);
            seek_count += abs(head - right[i]);
            head = right[i];
        }
    }
    printf("\n\n Total Head Movement:- %d cylinders\n", seek_count);
}

// C-SCAN Algorithm
void cscan(int req[], int n, int head, int disk_size) {
    int seek_count = 0;
    int left[MAX], right[MAX];
    int l = 0, r = 0;
    
    for (int i = 0; i < n; i++) {
        if (req[i] < head) left[l++] = req[i];
        else right[r++] = req[i];
    }
    sort(left, l);
    sort(right, r);

    print_header("Circular SCAN (C-SCAN)");
    printf(" Movement Path: [%d]", head);

    for (int i = 0; i < r; i++) {
        printf(" -> %d", right[i]);
        seek_count += abs(head - right[i]);
        head = right[i];
    }
    // Jump to the end, then reset to 0
    printf(" -> %d", disk_size - 1);
    seek_count += abs(head - (disk_size - 1));
    
    printf(" >> [Reset to 0]");
    head = 0;
    // (Note: Moving from disk_size-1 to 0 is often considered a single jump 
    // and sometimes doesn't add to seek_count depending on textbook definition)
    
    for (int i = 0; i < l; i++) {
        printf(" -> %d", left[i]);
        seek_count += abs(head - left[i]);
        head = left[i];
    }
    printf("\n\n Total Head Movement:- %d cylinders\n", seek_count);
}

int main() {
    int n, head, disk_size, direction;
    int req[MAX];

    printf("--- Disk Scheduling Simulator ---\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    printf("Enter the sequence (space separated): ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }
    
    printf("Initial head position: ");
    scanf("%d", &head);
    printf("Total disk size (e.g., 200): ");
    scanf("%d", &disk_size);
    printf("Direction (0 for Left, 1 for Right): ");
    scanf("%d", &direction);

    fcfs(req, n, head);
    sstf(req, n, head);
    scan(req, n, head, disk_size, direction);
    cscan(req, n, head, disk_size);

    printf("\n========================================================\n");
    printf(" Simulation Complete.\n");
    printf("========================================================\n");

    return 0;
}