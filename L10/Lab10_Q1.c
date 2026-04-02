#include <stdio.h>
#include <stdlib.h>

// Function to check if page is present in frame
int isPresent(int *frames, int frameCount, int page) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

// FIFO Page Replacement
void fifo(int *pages, int n, int frameCount) {
    int *frames = (int *)malloc(frameCount * sizeof(int));
    int i, j, pageFaults = 0, index = 0;

    // Initialize frames
    for (i = 0; i < frameCount; i++)
        frames[i] = -1;

    for (i = 0; i < n; i++) {
        if (!isPresent(frames, frameCount, pages[i])) {
            frames[index] = pages[i];
            index = (index + 1) % frameCount;
            pageFaults++;
        }
    }

    printf("\nFIFO Page Faults = %d\n", pageFaults);
    free(frames);
}

// Find the optimal page to replace
int findOptimal(int *pages, int *frames, int n, int frameCount, int current) {
    int i, j, farthest = current, index = -1;

    for (i = 0; i < frameCount; i++) {
        int found = 0;
        for (j = current + 1; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                found = 1;
                break;
            }
        }
        // If page not found in future
        if (!found)
            return i;
    }

    return (index == -1) ? 0 : index;
}

// Optimal Page Replacement
void optimal(int *pages, int n, int frameCount) {
    int *frames = (int *)malloc(frameCount * sizeof(int));
    int i, j, pageFaults = 0;

    // Initialize frames
    for (i = 0; i < frameCount; i++)
        frames[i] = -1;

    for (i = 0; i < n; i++) {
        if (!isPresent(frames, frameCount, pages[i])) {
            int index;

            // Check for empty frame
            for (j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    index = j;
                    break;
                }
            }

            // If no empty frame, find optimal replacement
            if (j == frameCount)
                index = findOptimal(pages, frames, n, frameCount, i);

            frames[index] = pages[i];
            pageFaults++;
        }
    }

    printf("Optimal Page Faults = %d\n", pageFaults);
    free(frames);
}

// Main function
int main() {
    int n, frameCount;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int *pages = (int *)malloc(n * sizeof(int));

    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    fifo(pages, n, frameCount);
    optimal(pages, n, frameCount);

    free(pages);
    return 0;
}