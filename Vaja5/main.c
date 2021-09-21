#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

void branjeRead(int velpom, int fd){
    char buffer[velpom];
    int ascii[255] = {0};
    while(1){
        int n = read(fd, buffer, velpom);
        if(n == 0)
            break;
        //printf("buffer = %s , n = %d\n", buffer, n);
        for(int i = 0; i < n; i++){
            ascii[(int)buffer[i]]++;
        }
    }
    for(int i = 65; i < 123; i++) {
        if(ascii[i] != 0){
            printf("%c - %d \n", (char)i, ascii[i]);
        }
    }
}

void branjeMmap(int fd){
    char *file;
    int size;

    // dobi file size in ga shrani v  strukturo.st_size
    struct stat s;
    int status = fstat(fd, &s);
    size = s.st_size;
    // printf("%d \n", size);
    char *buffer = malloc(size);
    int ascii[255] = {0};
    file = (char*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    for (int i = 0; i < size; i++) {
        buffer[i] = file[i];
        ascii[(int)buffer[i]]++;
    }
    for(int i = 65; i < 123; i++) {
        if(ascii[i] != 0){
            printf("%c - %d \n", (char)i, ascii[i]);
        }
    }
    munmap(file, size);
}


int main(int argc, char* argv[])  {

    int fd = 0;
    int velpom = 0;
    
    if(argc == 6){
        if (access(argv[5], F_OK) == 0){
            fd = open(argv[5], O_RDONLY);
        } else {
            printf("Napaka pri odpretju datoteke!");
            exit(1);
        }
        if(strcmp(argv[4], "read") == 0){
            //read
            velpom = atoi(argv[2]);
            branjeRead(velpom, fd);
        } else{
            //mmap
            branjeMmap(fd);
        }
    }
    close(fd);
    return 0; 
}