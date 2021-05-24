#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h>

char* getNameOfFile(int i, int withTemp){
    char* filename = malloc(sizeof("100"));

    if(withTemp == 0)
        sprintf(filename, "files/%03d", i);
    else
        sprintf(filename, "%03d", i);

    return filename;
}

int createFile(char* name){
    FILE* fd = fopen(name, "w");

    if(fd == NULL){
        printf("File open failed!\n");
        return 1;
    }

    fclose(fd);

    return 0;
}

int main(int argc, char* argv[]){
    DIR *dir = opendir("files");

    if(dir == NULL) {
       printf("Create directory with name \"files\"\n");
       system("mkdir files");
    } else {
       printf("Directory \"files\" was deleted and created again\n");
       system("rm -r files");
       system("mkdir files");
    }

    char* firstName = getNameOfFile(0, 0);
    if(createFile(firstName)){
        free(firstName);
        printf("File open failed!\n");
        exit(1);
    }

    free(firstName);

    int counter = 1;

    while(1){
        char* prevFile = getNameOfFile(counter - 1, 1);
        char* nextFile = getNameOfFile(counter, 0);

        if(symlink(prevFile, nextFile)) {
            free(prevFile);
            free(nextFile);
            printf("Error with symlink\n");
            exit(-1);
        }

        FILE* newFile = fopen(nextFile, "r");
        if(newFile == NULL){
            free(prevFile);
            free(nextFile);
            break;
         }

        fclose(newFile);
        free(prevFile);
        free(nextFile);

        counter++;
    }

    //Do not take into account last file, so it isn't correct.
    printf("result: %d\n", --counter);
    closedir(dir);

    return 0;
}
