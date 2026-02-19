#include <iostream>
#include <unistd.h>   
#include <sys/wait.h> 
#include <queue>
#include <cstring>

using namespace std;
// Write a producer and consumer program in C using FIFO queue. The producer should write
// a set of 4 integers into the FIFO queue and the consumer should display the 4 integers.
int main() {
    int fd[2]; 
    if (pipe(fd) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } 
    else if (pid == 0) {
      
        close(fd[1]);

        int numbers[4];
        read(fd[0], numbers, sizeof(numbers)); 
        cout << "Consumer received integers from Producer: ";
        for (int i = 0; i < 4; i++) {
            cout << numbers[i] << " ";
        }
        cout << endl;

        close(fd[0]);
    } 
    else {

        close(fd[0]); 

        int numbers[4];
        cout << "Producer: Enter 4 integers: ";
        for (int i = 0; i < 4; i++) cin >> numbers[i];

        write(fd[1], numbers, sizeof(numbers)); 
        close(fd[1]);

        wait(NULL); 
        cout << "Producer finished." << endl;
    }

    return 0;
}
