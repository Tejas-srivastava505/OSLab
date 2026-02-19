#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

using namespace std;

// Process A wants to send a number to Process B. Once received, Process B has to check
// whether the number is palindrome or not. Write a C program to implement this interprocess
// communication using message queue
struct msg_buffer {
    long msg_type; 
    char msg_text[100];
};

bool isPalindrome(int num) {
    int original = num, rev = 0;
    while (num > 0) {
        rev = rev * 10 + num % 10;
        num /= 10;
    }
    return original == rev;
}

int main() {
    key_t key;
    int msgid;

   
    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        cerr << "Message queue creation failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } 
    else if (pid == 0) {

        msg_buffer message;


        msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);
        int number = atoi(message.msg_text);
        cout << "Process B received number: " << number << endl;


        if (isPalindrome(number))
            cout << "The number " << number << " is a palindrome." << endl;
        else
            cout << "The number " << number << " is not a palindrome." << endl;


        msgctl(msgid, IPC_RMID, nullptr);
    } 
    else {

        msg_buffer message;
        message.msg_type = 1;

        int num;
        cout << "Process A: Enter a number to send to Process B: ";
        cin >> num;
        snprintf(message.msg_text, sizeof(message.msg_text), "%d", num);

        msgsnd(msgid, &message, sizeof(message.msg_text), 0);
        cout << "Process A sent number: " << num << endl;
    }

    return 0;
}
