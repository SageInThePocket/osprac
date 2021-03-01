#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/* Я сдал работу раньше начала семинара
 * так как во время семинара у меня появились
 * неотложные дела и у меня не было возможности
 * выполнить эту работу на семинаре */

int main()
{
	char *msg;
	int shmid;
	char pathname[] = "writer.c";
	FILE *fo = fopen(pathname, "r");
	char ch;
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
	
	int i = 0;
	if (fo == NULL) {
		printf("Error: can\'t open file for reading\n");
		exit(-1);
	} else {
		while(!feof(fo)) {
			ch = getc(fo);
			msg[i] = ch;
			i++;
		}
	}
	msg[i - 1] = 0;

	printf("The message has been sent\n");
	
	if (shmdt(msg) < 0) {
		printf("Error: can\'t detach shared memory\n");
		exit(-1);
	}
	
	return 0;
}
