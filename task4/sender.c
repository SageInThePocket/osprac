#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int isSuccessfully = 0;

void my_handler(int nsig) {
  isSuccessfully = 1;
}

int main(void) {
  int pid;
  int num;
  int bits[32];
  int sign;
  
  (void) signal(SIGUSR1, my_handler);

  printf("My pid: %d\n", getpid());
  printf("Enter pid: ");
  scanf("%d", &pid);
  printf("Enter int number: ");
  scanf("%d", &num);

  //Wait until another process is ready
  while (!isSuccessfully);

  //Determine the sign
  sign = num < 0;
  if (sign) {
    num = num * (-1);
    num--;
  }
  bits[0] = sign;

  //Get bits of number
  for (int i = 0; i < 31; ++i) {
    bits[31 - i] = (num % 2) ^ sign;
    num /= 2;
  }

  for (int i = 0; i < 32; ++i) {
    //Send the bit
    if (bits[i])
      kill(pid, SIGUSR2);
    else
      kill(pid, SIGUSR1);
    isSuccessfully = 0;
    //Wait until another process is ready to
    //get next bit
    while(!isSuccessfully);
  }

  printf("I sent the number\n");
  
  return 0;
}
