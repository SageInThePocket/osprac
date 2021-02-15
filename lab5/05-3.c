#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fromParent[2], fromChild[2], result;

  size_t size1, size2;
  char  resstringParent[22], resstringChild[23];

  if (pipe(fromParent) < 0) {
    printf("Can\'t open parent\'s pipe\n");
    exit(-1);
  }

  if (pipe(fromChild) < 0) {
    printf("Can\'t open child\'s pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fromParent[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe from parent\n"); exit(-1);
    }

    if (close(fromChild[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe from child\n");
      exit(-1);
    }

    size1 = write(fromParent[1], "Hello, I am your parent", 23);

    if (size1 != 23) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }

    if (close(fromParent[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe from parent\n"); exit(-1);
    }

    size2 = read(fromChild[0], resstringParent, 22);

    if (size2 < 0) {
      printf("parent: Can\'t read string from pipe\n");
      exit(-1);
    }

    printf("Parent exit, resstring: %s\n", resstringParent);

    if (close(fromChild[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe from child\n");
      exit(-1);
    }

  } else {

    /* Child process */

    if (close(fromChild[0]) < 0) {
      printf("child: Can\'t close reading side of pipe from child\n");
      exit(-1);
    }

    if (close(fromParent[1]) < 0) {
      printf("child: Can\'t close writing side of pipe from parent\n"); exit(-1);
    }

    size2 = write(fromChild[1], "Hello, I am your child", 22);

    if (size2 != 22) {
      printf("child: Can\'t write all string to pipe\n");
      exit(-1);
    }

    if (close(fromChild[1]) < 0) {
      printf("child: Can\'t close writing side of pipe from child\n");
    }

    size1 = read(fromParent[0], resstringChild, 23);

    if (size1 < 0) {
      printf("child: Can\'t read string from pipe\n");
      exit(-1);
    }

    printf("Child exit, resstring: %s\n", resstringChild);

    if (close(fromParent[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
  }

  return 0;
}
