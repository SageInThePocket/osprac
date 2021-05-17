#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

//Server

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
  
  while(1) {  
    maxlen = sizeof(requestbuf.info);
    if (msgrcv(msqid, (struct request *) &requestbuf, maxlen, 0, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    
    if (requestbuf.mtype == 2) {
      printf("Server\tI was killed\n");
      exit(-1);
    }

    float num = requestbuf.info.message;

    printf("Server\tClient:%d send number %f\n", requestbuf.info.pid, num);
      
    responsebuf.mtype = requestbuf.info.pid;
    responsebuf.info.message = num * num;

    len = sizeof(responsebuf.info);

    if (msgsnd(msqid, (struct response *) &responsebuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    printf("Server\tI sent response to Client%d\n", requestbuf.info.pid);

  }
   

  return 0;
}
