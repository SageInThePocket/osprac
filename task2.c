#include <signal.h>
#include <stdio.h>

void handler(int nsig) {
  if(nsig == SIGINT)
    printf("CTRL + С were pressed");
  else if(nsig == SIGQUIT)
    printf("CTRL + 4 were pressed");
}

int main(void) {
  (void)signal(SIGINT, handler);
  (void)signal(SIGQUIT, handler);

  while(1);
  return 0;
}
