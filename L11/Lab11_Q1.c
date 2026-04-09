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

// FCFS Algorithm
void fcfs(int req[], int n, int head) {
    int seek_count = 0;

    printf("\nFCFS Seek Sequence: %d", head);

    for (int i = 0; i < n; i++) {
        printf(" -> %d", req[i]);
        seek_count += abs(head - req[i]);
        head = req[i];
    }

    printf("\nTotal Head Movement (FCFS): %d\n", seek_count);
}

// SSTF Algorithm
void sstf(int req[], int n, int head) {
    int seek_count = 0;
    int visited[MAX] = {0};

    printf("\nSSTF Seek Sequence: %d", head);

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

    printf("\nTotal Head Movement (SSTF): %d\n", seek_count);
}

// SCAN Algorithm
void scan(int req[], int n, int head, int disk_size, int direction) {
    int seek_count = 0;
    int left[MAX], right[MAX];
    int l = 0, r = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] < head)
            left[l++] = req[i];
        else
            right[r++] = req[i];
    }

    sort(left, l);
    sort(right, r);

    printf("\nSCAN Seek Sequence: %d", head);

    if (direction == 1) { // right
        for (int i = 0; i < r; i++) {
            printf(" -> %d", right[i]);
            seek_count += abs(head - right[i]);
            head = right[i];
        }

        printf(" -> %d", disk_size - 1);
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

        printf(" -> 0");
        seek_count += abs(head - 0);
        head = 0;

        for (int i = 0; i < r; i++) {
            printf(" -> %d", right[i]);
            seek_count += abs(head - right[i]);
            head = right[i];
        }
    }

    printf("\nTotal Head Movement (SCAN): %d\n", seek_count);
}

// C-SCAN Algorithm
void cscan(int req[], int n, int head, int disk_size) {
    int seek_count = 0;
    int left[MAX], right[MAX];
    int l = 0, r = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] < head)
            left[l++] = req[i];
        else
            right[r++] = req[i];
    }

    sort(left, l);
    sort(right, r);

    printf("\nC-SCAN Seek Sequence: %d", head);

    for (int i = 0; i < r; i++) {
        printf(" -> %d", right[i]);
        seek_count += abs(head - right[i]);
        head = right[i];
    }

    printf(" -> %d", disk_size - 1);
    seek_count += abs(head - (disk_size - 1));
    head = disk_size - 1;

    printf(" -> 0");
    seek_count += abs(head - 0);
    head = 0;

    for (int i = 0; i < l; i++) {
        printf(" -> %d", left[i]);
        seek_count += abs(head - left[i]);
        head = left[i];
    }

    printf("\nTotal Head Movement (C-SCAN): %d\n", seek_count);
}

// Main Function
int main() {
    int n, head, disk_size, direction;
    int req[MAX];

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter the request sequence:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &disk_size);

    printf("Enter direction for SCAN (0 = left, 1 = right): ");
    scanf("%d", &direction);

    fcfs(req, n, head);
    sstf(req, n, head);
    scan(req, n, head, disk_size, direction);
    cscan(req, n, head, disk_size);

    return 0;
}