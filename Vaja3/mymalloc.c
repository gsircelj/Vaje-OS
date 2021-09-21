#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mymalloc.h"

struct alloc_info{
    size_t alloc_size;
    int st_odsekov;
    void *prvi_prost;
};

struct odsek_info{
    size_t odsek_size;
    void *stran;
};

struct alloc_info *zadnja_stran = NULL;

void* mymalloc(size_t size){
    printf("Alociranje\n");

    // koliko prostora se rabi
	size_t size_odseka = size + sizeof(struct odsek_info); 
    // samo za izpis
	size_t size_prost = getpagesize();


    if (zadnja_stran == NULL)
    {
	    printf("Zadnja stran je NULL\n");
	    zadnja_stran = mmap(NULL, size_odseka, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    	zadnja_stran[0].alloc_size = size_odseka;
   	    zadnja_stran[0].st_odsekov = 0; // št odsekov na 0
   	    zadnja_stran[0].prvi_prost = zadnja_stran + 1; // zadnja dodeljena stran
   	}
	else
	{

		size_prost = getpagesize() - zadnja_stran[0].alloc_size - (zadnja_stran[0].prvi_prost - (void *)zadnja_stran);
        // če na strani ni dovolj prostora se
        // pokljiče mmap in rezevira novo stran
		if (size_prost < size_odseka)
		{
		    printf("Rezerviram novo stran\n");
		    zadnja_stran = mmap(NULL, size_odseka, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		    zadnja_stran[0].alloc_size = size_odseka;
           	zadnja_stran[0].st_odsekov = 0; // št odsekov na 0
           	zadnja_stran[0].prvi_prost = zadnja_stran + 1; // zadnja dodeljena stran
            // samo za izpis
            size_prost = getpagesize() - zadnja_stran[0].alloc_size - (zadnja_stran[0].prvi_prost - (void *)zadnja_stran);
		}
	}
	struct odsek_info *odsek_info_ptr = zadnja_stran[0].prvi_prost;
    odsek_info_ptr[0].stran = zadnja_stran;
    zadnja_stran[0].st_odsekov += 1; // povečujemo št odsekov
   	zadnja_stran[0].prvi_prost += size_odseka;	

	printf("Prosti size = %ld \n", size_prost);
    printf("Alociramo size = %ld \n", size_odseka);
    // samo za izpis
    size_prost = getpagesize() - zadnja_stran[0].alloc_size - (zadnja_stran[0].prvi_prost - (void *)zadnja_stran);
    printf("Novi prosti size = %ld \n", size_prost);
    printf("Št odsekov = %d \n", zadnja_stran[0].st_odsekov);

	return odsek_info_ptr + 1;
}

void myfree(void *ptr){
    printf("Sproščanje\n");
    struct odsek_info *odsek_info_ptr = ptr;
    odsek_info_ptr -= 1;
    struct alloc_info *stran = odsek_info_ptr[0].stran;

    // zmanjšujemo št odsekov
    stran[0].st_odsekov -= 1;
    printf("Št odsekov = %d \n", stran[0].st_odsekov);


    if (stran[0].st_odsekov == 0)
    {
        // če ni več odsekov se stran v celoti sprosti
        printf("Sproščanje strani\n");
        munmap(stran, stran[0].alloc_size);
        zadnja_stran = NULL;
    }
}