#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

int answer;

int main() {
  int fd[2];
  size_t size = 1;
  char* str = malloc(1 * sizeof(char));

  if(pipe(fd) < 0) {
    printf("Can\'t create pipe\n");
    exit(-1);
  }

  fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

  printf("Start counting\n");  
  answer = 0;
  while (size == 1) {
    size = write(fd[1], str, 1);
    answer++;
  }

  printf("Count of bytes: %d\n", answer);

  if (close(fd[1]) < 0) {
    printf("Can\'t close writing side of pipe\n");
    exit(-1);
  }

  if (close(fd[0]) < 0) {
    printf("Can\'t close reading side of pipe\n");
    exit(-1);
  }

  return 0;
}
