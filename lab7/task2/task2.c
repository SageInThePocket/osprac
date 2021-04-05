#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *mythread(void *dummy)
{
  pthread_t mythid;
  mythid = pthread_self();

  a = a+1;
  printf("Thread %u, Calculation result = %d\n", mythid, a);
  return NULL;
}

int main()
{
  pthread_t thid1, thid2, mythid;
  int       result;

	//Firts thread
  result = pthread_create (&thid1, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
    printf ("Error on thread create, return value = %d\n", result);
    exit(-1);
  }

	printf("Thread created, thid = %u\n", thid1);

	//Second thread
  result = pthread_create (&thid2, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}

  printf("Thread created, thid = %u\n", thid2);

  mythid = pthread_self();

  a = a+1;

  printf("Thread %u, Calculation result = %d\n", mythid, a);

  pthread_join(thid1, (void **)NULL);
	pthread_join(thid2, (void **)NULL);

  return 0;
}
