#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int i = 31; //byte position
int res = 0; //result
int sign;
int pid; //sending process

//SIGUSR1 - 0
//SIGUSR2 - 1
void my_handler(int nsig) {
  if (i == 31)
    sign = nsig == 12;
  else {
    res = res << 1;
    if ((nsig == 12) ^ sign)
      res++;
  }
  i--;
  //Say the sending process that we have successfully
  //received the bit
  kill(pid, SIGUSR1);
}

int main(void) {
  printf("My pid: %d\n", getpid());
  printf("Enter pid: ");
  scanf("%d", &pid);

  (void) signal(SIGUSR1, my_handler);
  (void) signal(SIGUSR2, my_handler);
  //Say that we ready to recieve number
  kill(pid, SIGUSR1);

  while(i >= 0);

  if (sign)
    res = (res + 1) * (-1);
  printf("result: %d\n", res);

  return 0;
}
