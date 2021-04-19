#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/* Программа работает следующим образом:
 * Начальное значение семафора равно 0 из-за
 * чего дочерний процесс уходит в ожидание
 * так как он пытается отнять от значения семафора
 * единицу. После чего родительский процесс
 * отправляет сообщение ребенку и увеличивает
 * значение семафора на 1 тем самым давая возможность
 * дочернему процессу начать писать сообщение
 * родителю, но перед этим дочерний процесс уменьшает
 * значение семафора и не дает родительскому процессу
 * прочитать сообщение, пока дочерний процесс его
 * не отправил. После отправления сообщения дочерний
 * процесс увеличивает семафор на 1 и дает возможность
 * прочитать родительскому процессу переданное сообщение
 * а сам дочерний процесс ожидает ответа от родителя
 * и так это циклично повторяется*/

int main()
{
  int     fd[2], result;
  size_t size;
  key_t key;
  char  resstring[14];
  char pathname[] = "05-3.c";
  struct sembuf addBuf;
  struct sembuf decBuf;
  struct sembuf waitZero;
  int semid;

  //Init buffers
  addBuf.sem_op = 1;
  addBuf.sem_flg = 0;
  addBuf.sem_num = 0;

  decBuf.sem_op = -1;
  decBuf.sem_flg = 0;
  decBuf.sem_num = 0;

  waitZero.sem_op = 0;
  waitZero.sem_flg = 0;
  waitZero.sem_num = 0;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("can\'t create key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666)) < 0) {
    printf("Can\'t find semaphore\n");
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      printf("Can\'t get semid\n");
      exit(-1);
    }
    printf("Semophore successfully created\n");
  }

//inc
      if (semop(semid, &addBuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();


  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {
    /* Parent process */
    int n;
    printf("Enter count of message:\n");
    scanf("%d", &n);
    if (n < 2) {
      printf("Count of message must be more or equals 2\n");
      exit(-1);
    }

    for (size_t i = 0; i < n; i++) {

      char* msg = "Hello, child!";
      size = write(fd[1], msg, 14);

      if (size != 14) {
        printf("parent:\tCan\'t write all string to pipe\n");
        exit(-1);
      }

      printf("parent:\tI writing message...\n");
      for(long long i = 0; i < 300000000L; ++i);
      printf("parent:\tI send message #%d to child: %s\n", i + 1, msg);
      printf("parent:\tWait answer from child\n");

      //inc
      if (semop(semid, &addBuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }
      //waitZero
      if (semop(semid, &waitZero, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }
      decBuf.sem_op = -2;
      //dec
      if (semop(semid, &decBuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }

      size = read(fd[0], resstring, 14);
      if (size != 14){
        printf("parent:\tCan\'t read message from child\n");
        exit(-1);
      }

      printf("parent: I read message from child: %s\n", resstring);
    }

    
    close(fd[0]);
    printf("Parent exit\n");

  } else {
    /* Child process */
    int counter = 0;
    while(1) {
      decBuf.sem_op = -2;

      //dec
      if (semop(semid, &decBuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }
      size = read(fd[0], resstring, 14);

      if (size < 0) {
        close(fd[1]);
        close(fd[0]);

        printf("All processes are closed");
        return 0;
      }

      printf("child:\tI get message #%d from parent: %s\n", ++counter, resstring);
      char* msg = "Hello, parent";      

      size = write(fd[1], msg, 14);
      if (size != 14) {
        printf("child:\tI can\'t write all string");
        exit(-1);
      }

      printf("child:\tI writing message...\n");
      for(long long i = 0; i < 300000000L; ++i);
      printf("child:\tI send message #%d to parent: %s\n", counter, msg);
      printf("child:\tWait answer from parent\n");

      //inc
      if (semop(semid, &addBuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }
    }
  }

  return 0;
}
