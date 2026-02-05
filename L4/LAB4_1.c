//1. Write a C program to create a child process. Display different messages in parent
//process and child process. Display PID and PPID of both parent and child process.
//Block parent process until child completes using wait system call.

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(void){
	pid_t pid;
	pid=fork();

	if(pid<0){
		perror("fork");
		return 1;
	}
	else if(pid==0){
		printf("Child Process:\n");
		printf(" PID = %d\n",getpid());
		printf(" PPID = %d\n",getppid());

	}
	else{
		printf("Parent Process:\n");
		printf("PID =%d\n",getpid());
		printf("Child PID= %d\n",pid);

		wait(NULL);
		printf("Parent : Child Process has completed. \n");

	}
	return 0;
}
