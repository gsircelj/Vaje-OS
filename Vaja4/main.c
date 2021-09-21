#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void writeToFile(char* text, char* name);

void readFromFile(char* name, int zapis, char* zname)
{   
    FILE *file;
	file = fopen(name, "rb");
	if (!file)
	{
	    fprintf(stderr, "Error pri odpiranju datoteke %s", name);
	    fclose(file);
        exit(1);
	}

    FILE *fout;
    if(zapis != 0){
	    fout = fopen(zname, "wb");
	    if (!fout)
	    {
	        fprintf(stderr, "Error pri odpiranju datoteke %s", zname);
	        fclose(fout);
            exit(1);
	    }
    }
    int c; //int avtomatsko pretvori ascii character
    char* write;
    while ((c = fgetc(file)) != EOF)
    {
            printf("%u ", c);
            if(zapis != 0){
                fprintf(fout, "%u ", c);
            }
    }
    fclose(file);
    if(zapis != 0){
        fclose(fout);
    }
    exit(0);
}

void writeToFile(char* text, char* name){
    FILE *file;
    if ((file = fopen(name,"wb")) == NULL){
        printf("Error pri odpiranju datoteke %s", name);
        exit(1);
    }

    fwrite(text, 1000, 1, file); 
    fclose(file);
}

void readFromStdin(int i, char* name, int fd){
    FILE *file;
    file = stdin;
    if (file == NULL) 
    { 
        printf("Ni stdin \n"); 
        exit(0); 
    } 
    FILE *fileout;
    if(i == 1){
        if ((fileout = fopen(name,"wb")) == NULL){
            printf("Error pri odpiranju datoteke %s", name);
            exit(1);
        }
    }
    int c; //int avtomatsko pretvori ascii character
    while ((c = getchar()) != EOF)
    {
            printf("%u ", c);
            if(i == 1) {
            //-o
                fprintf(fileout, "%u ", c);
            } else if (i == 2){
            //-O
                //char str[4];
                //sprintf(str, "%u ", c);
                //printf("im here str = %s \n", str);
                //write(fd, &c, 1);
            }
    }
    if(i == 1){
        fclose(fileout);
    }
    fclose(file);
}

int main(int argc, char* argv[])  {
    int zapis = 0;
    int branje = 0;
    int oprimekzapis = 0;
    int oprimekbranje = 0;

    if (isatty(STDIN_FILENO) && argc == 1)
    {
        return 0;
    }
    if(argc == 1){
        readFromStdin(0, "0", 0);
    } else {
        for(int i = 1; i < argc; i++ ) {
            if(strcmp(argv[i],"-o")==0){
                zapis = i;
            }
            if(strcmp(argv[i],"-i")==0){
                branje = i;
            }
            if(strcmp(argv[i],"-O")==0){
                oprimekzapis = i;
            }
            if(strcmp(argv[i],"-I")==0){
                oprimekbranje = i;
            }
        }
        if(branje == 0 && oprimekbranje == 0){  
            if (zapis != 0){
            //o
                readFromStdin(1, argv[zapis+1], 0);
            } else if(oprimekzapis != 0){
            //O
                readFromStdin(2, "0", atoi(argv[oprimekzapis+1]));
            }
        } else {

        if(branje != 0){
            //i
            if(zapis != 0){
                readFromFile(argv[branje+1], zapis, argv[zapis+1]);
            } else {
                readFromFile(argv[branje+1], 0, "0");
            }
    
        }     
        if(oprimekbranje != 0){
            //I
            char buffer[10000];
            long int n;
            n = read(atoi(argv[oprimekbranje+1]), buffer, 10000);
            for(int i = 0; i < n; i++){
                printf("%d ", buffer[i]);
            }
            if(oprimekzapis != 0){
                //O
                for(int i = 0; i < n; i++)
                {
                    char str[strlen(&buffer[i])];
                    sprintf(str, "%u ", buffer[i]);
                    write(atoi(argv[oprimekzapis+1]), str, strlen(str));
                }
            }
        }
        } 
    }
    return 0; 
}