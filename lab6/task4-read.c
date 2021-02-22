#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
  int fd;
  size_t size;
  char result[14]; 
  char fileName[] = "rw.fifo";

  if (mknod(fileName, S_IFIFO | 0666, 0) < 0) {
    printf("Can't create FIFO!\n");
    exit(-1);
  }

  if ((fd = open(fileName, O_RDONLY)) < 0) {
    printf("Can't open FIFO to read!");
    exit(-1);
  }

  size = read(fd, result, 14);
  
  if (size != 14) {
    printf("Can't read string!");
    exit(-1);
  }

  if (close(fd) < 0) {
    printf("Can\'t close FIFO\n");
    exit(-1);
  }

  printf("Resutl: %s\n", result);
  close(fd);

  return 0;
}
