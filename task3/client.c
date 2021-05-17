#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Client

int main(void)
{
  int msqid;
  char pathname[]="server.c";
  key_t  key;
  int i,len, maxlen;
  long j;

  struct request {
    long mtype;
    struct {
      pid_t pid;
      float message;
    } info;
  } requestbuf;

  struct response {
    long mtype;
    struct {
      float message;
    } info;
  } responsebuf;


  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  float message;
  printf("Enter float number:\n");
  scanf("%f", &message);
  
  requestbuf.mtype = 1;
  requestbuf.info.pid = getpid();
  requestbuf.info.message = message;
  len = sizeof(requestbuf.info);

  if (msgsnd(msqid, (struct request *) &requestbuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  printf("Client:%d\tWaiting response from server...\n", getpid());
  maxlen = sizeof(responsebuf.info);

  //Waiting response with type == getpid() to take response only for this client from queue 
  if (msgrcv(msqid, (struct response *) &responsebuf, maxlen, getpid(), 0) < 0) {
    printf("Can\'t receive message from queue\n");
    exit(-1);
  }

  printf("Client:%d\t I send %f and recieve from server %f\n", getpid(), message, responsebuf.info.message);

  return 0;
}
