#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	char *msg;
	int shmid;
	char pathname[] = "writer.c";
	key_t key;
	
	if ((key = ftok(pathname, 0)) < 0) {
	    printf("Error: cant\'t generate key\n");
	    exit(-1);
	}
	
	if ((shmid = shmget(key, 10000 * sizeof(char), 0666|IPC_CREAT)) < 0) {
	    printf("Error: cant\'t create shared memory\n");
	    exit(-1);
	}
	
	if ((msg = (char*)shmat(shmid, NULL, 0)) == (char*)(-1)) {
		printf("Error: can\'t attach shared memory\n");
		exit(-1);
	}
	
	printf("The message has been recieved: \n%s\n", msg);
	
	if (shmdt(msg) < 0) {
		printf("Error: can\'t detach shared memory\n");
		exit(-1);
	}
	
	if (shmctl(shmid, 0, NULL) == -1) {
		printf("Error: can\'t delete shared memory\n");
		exit(-1);
	}
	
	return 0;
}
