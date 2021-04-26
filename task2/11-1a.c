#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int msqid;
  char pathname[]="11-1a.c";
  key_t  key;
  int i,len, maxlen;
  long j;

  struct mymsgbuf
  {
    long mtype;
    struct {
        int intMsg;
        char mtext[81];
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("11-1a.c:\tI write messages very slow\n");

  /* Send information */

  for (i = 1; i <= 5; i++) {
    for(j = 0; j < 400000000L; ++j);

    mybuf.mtype = 1;
    mybuf.info.intMsg = i;
    strcpy(mybuf.info.mtext, "From 11-1a.c message");
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    printf("11-1a.c:\tI send message#%d with text: %s\n", i, mybuf.info.mtext);
  }

  for (i = 1; i <= 5; ++i) {
    maxlen = sizeof(mybuf.info);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    printf("11-1a.c:\tI get message#%d with text: %s\n", mybuf.info.intMsg, mybuf.info.mtext);
  }

  return 0;
}
