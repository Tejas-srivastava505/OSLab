#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

const int MAX_WORDS = 5;
const int WORD_SIZE = 20;

struct shm_struct {
    char words[MAX_WORDS][WORD_SIZE];
    int flag; // 0 = producer writes, 1 = consumer reads
};

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(shm_struct), 0666 | IPC_CREAT);
    if (shmid == -1) {
        cerr << "Shared memory creation failed!" << endl;
        return 1;
    }

    shm_struct* shm_ptr = (shm_struct*) shmat(shmid, nullptr, 0);
    if (shm_ptr == (void*) -1) {
        cerr << "Shared memory attach failed!" << endl;
        return 1;
    }

    shm_ptr->flag = 0;

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }
    else if (pid == 0) {
        // Consumer
        while (shm_ptr->flag != 1) usleep(1000); // small sleep to reduce CPU usage
        cout << "Consumer received words:\n";
        for (int i = 0; i < MAX_WORDS; i++) {
            cout << shm_ptr->words[i] << endl;
        }
        shmdt(shm_ptr);
    }
    else {
        // Producer
        cout << "Producer: Enter " << MAX_WORDS << " words:\n";
        for (int i = 0; i < MAX_WORDS; i++) {
            string word;
            cin >> word;
            strncpy(shm_ptr->words[i], word.c_str(), WORD_SIZE);
        }
        shm_ptr->flag = 1; // notify consumer
        wait(nullptr);
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, nullptr);
        cout << "Shared memory detached and deleted.\n";
    }

    return 0;
}
