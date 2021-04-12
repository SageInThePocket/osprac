#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     *array;
  int     shmid;
  int	  semid;
  int     new = 1;
  char    pathname[] = "prog1.c";
  key_t   key;
  struct sembuf addbuf; //Для увеличения семафора
  struct sembuf decbuf; //Для уменьшения семафора
  long    i;

  //Задаем нужные значения
  addbuf.sem_num = 0;
  addbuf.sem_op = 1;
  addbuf.sem_flg = 0;

  decbuf.sem_num = 0;
  decbuf.sem_op = -1;
  decbuf.sem_flg = 0;

  //Получаем ключ
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t genete key\n");
    exit(-1);
  }

  //Пытаемся получить семафор и если его нет, то
  //создаем новый и сразу после создания прибавляем
  //ему единицу
  if ((semid = semget(key, 1, 0666)) < 0) {
    printf("Semaphore doesn\'t exist\n");
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      printf("Can\'t create new semaphore");
      exit(-1);
    }
    printf("Semaphore was created\n");
    if (semop(semid, &addbuf, 1) < 0) {
      printf("Can\'t wait for condition\n");
      exit(-1);
    }
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  //Уменьшаем семафор на 1 тем самым не давая
  //второму процессу изменить хранимые в
  //разделенной памяти данные
  if (semop(semid, &decbuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  printf("I am program \"a\" and I am starting to change data\n");
  if (new) {
    array[0] =  1;
    array[1] =  0;
    array[2] =  1;
  } else {
    //Изменение значений
    array[0] += 1;
    for (i = 0; i < 1000000000L; i++);
    array[2] += 1;
  }

  printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);

  //Увеличиваем семафор на 1, тем самым предоставляя
  //доступ второму процесу к данным разделяемой памяти
  if (semop(semid, &addbuf, 1) < 0) {
 		printf("Can\'t wait for condition\n");
   	exit(-1);
  }

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
