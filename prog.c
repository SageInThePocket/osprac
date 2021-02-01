#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	printf("User Id: %d\nGroup Id: %d\n", (int)getuid(), (int)getgid);
	return 0;
}
