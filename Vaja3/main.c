#include "mymalloc.h"

int main() {
    int *velikopolje;
    int *polje;
    char *znak;

    polje = (int*)mymalloc(10*sizeof(int));
    for(int i = 0; i < 10; i++)
    {
        polje[i] = i;
    }

    znak = (char*)mymalloc(sizeof(char));
    velikopolje = (int*)mymalloc(1235);

    myfree(polje);
    myfree(znak);
    myfree(velikopolje);

    void* v1 = mymalloc(100);
    void* v2 = mymalloc(100);
    myfree(v1);
    myfree(v2);
    return 0;
}