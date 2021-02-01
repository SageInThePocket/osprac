#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	printf("Process Id: %d\n", (int)getpid());
	printf("Parent's process Id: %d\n", (int)getppid());
}
