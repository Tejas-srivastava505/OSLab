#include <stdio.h>
#include <stdlib.h>

// Function to check if the page is present in a frame
int isPresent(int *frames, int frameCount, int page) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page)
            return i; // Return the index where it's found
    }
    return -1; // Return -1 if page is not found (Miss)
}

// Function to find the Least Recently Used frame index
int findLRU(int *time, int frameCount) {
    int min = time[0], index = 0;
    for (int i = 1; i < frameCount; i++) {
        if (time[i] < min) {
            min = time[i];
            index = i;
        }
    }
    return index;
}

void simulateLRU(int *pages, int n, int frameCount) {
    // Dynamic allocation for frames and their access timestamps
    int *frames = (int *)malloc(frameCount * sizeof(int));
    int *time = (int *)malloc(frameCount * sizeof(int));
    
    int pageFaults = 0, hits = 0, counter = 0;

    // Initialize frames with -1 (empty)
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
    }

    printf("\nRef String | Frames\n");
    printf("-----------|-----------\n");

    for (int i = 0; i < n; i++) {
        counter++; // Increment time at every step
        int pageIndex = isPresent(frames, frameCount, pages[i]);

        if (pageIndex != -1) {
            // Page Hit
            hits++;
            time[pageIndex] = counter; // Update timestamp
        } else {
            // Page Fault
            pageFaults++;
            int lruIndex;

            // Check if there's an empty frame first
            int foundEmpty = 0;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    lruIndex = j;
                    foundEmpty = 1;
                    break;
                }
            }

            // If no empty frames, find the LRU index
            if (!foundEmpty) {
                lruIndex = findLRU(time, frameCount);
            }

            frames[lruIndex] = pages[i];
            time[lruIndex] = counter;
        }

        // Print current frame state
        printf("%10d | ", pages[i]);
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf("\n");
    }

    float hitRatio = (float)hits / n;

    printf("\nTotal Page Faults: %d", pageFaults);
    printf("\nTotal Hits: %d", hits);
    printf("\nHit Ratio: %.2f (or %.1f%%)\n", hitRatio, hitRatio * 100);

    free(frames);
    free(time);
}

int main() {
    int n, frameCount;

    printf("Enter number of pages: ");
    if (scanf("%d", &n) != 1) return 1;

    int *pages = (int *)malloc(n * sizeof(int));
    printf("Enter page reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    simulateLRU(pages, n, frameCount);

    free(pages);
    return 0;
}