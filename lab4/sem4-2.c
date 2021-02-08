#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = fork();
	if (pid == -1) {
		printf("Failed to create process");
	} else if (pid == 0) {
		printf("I'm a child and I'm starting an other program\n");
		char *file = "sem4-1";
		execle("/bin/cat", "/bin/cat", "sem4-1.c", 0, envp);
		printf("I have some problems");
		exit(-1);
	} else {
		printf("I'm a parent, my id: %d\n", (int)getpid());
		printf("My child is %d\n", (int)pid);
	}
	return 0;
}
