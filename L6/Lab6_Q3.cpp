#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct shm_struct {
    char ch;    
    int flag;   
};

int main() {
    key_t key = ftok("shmfile", 65); 
    int shmid = shmget(key, sizeof(shm_struct), 0666 | IPC_CREAT);
    if (shmid == -1) {
        cerr << "Shared memory creation failed!" << endl;
        return 1;
    }

    
    shm_struct* shm_ptr = (shm_struct*) shmat(shmid, nullptr, 0);
    shm_ptr->flag = 0; 

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }
    else if (pid == 0) {

        while (shm_ptr->flag != 1); 

        char received = shm_ptr->ch;
        cout << "Child received: " << received << endl;

    
        char next_ch = (received == 'Z') ? 'A' :
                       (received == 'z') ? 'a' :
                       received + 1;

        shm_ptr->ch = next_ch; 
        shm_ptr->flag = 2;    

        shmdt(shm_ptr); 
    }
    else {

        char ch;
        cout << "Parent: Enter an English alphabet: ";
        cin >> ch;

        shm_ptr->ch = ch;  
        shm_ptr->flag = 1; 

        
        while (shm_ptr->flag != 2);

        cout << "Parent received reply from child: " << shm_ptr->ch << endl;

        shmdt(shm_ptr);              
        shmctl(shmid, IPC_RMID, nullptr);

        wait(NULL); 
    }

    return 0;
}
