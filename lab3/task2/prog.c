#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	pid_t pid = fork();
	if (pid == -1) {
		printf("Failed to create process");
	} else if (pid == 0) {
		printf("I'm a child and my id: %d\n", (int)getpid());
		printf("My parent is %d\n", (int)getppid());
		exit(0);
	} else {
		printf("I'm a parent, my id: %d\n", (int)getpid());
		printf("My child is %d\n", (int)pid);
	}
	return 0;
}
