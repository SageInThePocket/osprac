#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
  int fd;
  size_t size;
  char fileName[] = "rw.fifo";
  (void)umask(0);
 
  printf("Creating FIFO\n");
  if (mknod(fileName, S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO\n");
    exit(-1);
  }

  printf("Open FIFO\n");
  if ((fd = open(fileName, O_WRONLY)) < 0){
    printf("Can't open FIFO!\n");
    exit(-1);
  }

  size = write(fd, "Hello, world!", 14);

  if (size != 14){
    printf("Can't write all strings!\n");
    exit(-1);
  }

  if (close(fd) < 0) {
    printf("Can\'t close FIFO\n");
    exit(-1);
  }

  printf("Writer has finished!");

  return 0;
}
