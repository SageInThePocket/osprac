#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
	printf("args:\n");
	for (int i = 0; i < argc; ++i) {
		printf("arg[%d]: %s\n", i, argv[i]);
	}

	printf("\nenvp:\n");
	int i = 0;
	while (envp[i] != NULL) {
		printf("envp[%d]: %s\n", i, envp[i++]);
	}

	return 0;
}
