#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <unistd.h>

int n = 0;
int *np;
int count = 0;

void printHelp(int argc, char** argv);
void submergeSortSimple(int* array, int min1, int max1, int min2, int max2);
void submergeSortProc(int* array, int min1, int max1, int min2, int max2);
void submergeSortThread(int* array, int min1, int max1, int min2, int max2);
void mergeSort(int *array, int min, int max, void(*submergeSort)(int *, int, int, int, int));
void merge(int *arr,int min,int mid,int max);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct{
    int *tabela;
    int zacetek;
    int konec;
}typedef thread_args;

// preprosta implementacija mergeSort rekurzije,
// samo klicemo margeSort za levo in desno polovico
// v istem procesu/isti niti
void submergeSortSimple(int* array, int min1, int max1, int min2, int max2){
    mergeSort(array, min1, max1, submergeSortSimple);
    mergeSort(array, min2, max2, submergeSortSimple);
}

// TODO: funkcija ki paralelizira sortiranje z uporabo procesov
// za preprosto paralelizacijo samo izvedemo vsak klic mergeSort 
// funkcije v svojem procesu, in počakamo, da se klica zaključita
void submergeSortProc(int* array, int min1, int max1, int min2, int max2){
//if(count < n && count < 32000 || n == 0)
//{
	sem_t* sem;
	sem_unlink("semafor");
	sem = sem_open("semafor", O_RDWR|O_CREAT|O_EXCL, 0660, 1);
        if(sem == SEM_FAILED){
        	perror("Problem pri ustvarjanju semaforja.");
        	return;
   	}
	printf ("Semafor ustavarjen.\n");

	pid_t pid1, pid2;
	pid1 = fork();
       	switch(pid1){
       	case -1:
		sem_unlink("semafor");
		sem_close(sem);  
       	 	perror("Težava pri ustvarjanju otroka.");
		exit(-1);
       	case 0:
		sem_wait(sem);
		printf("Jaz sem otrok, sortiram prvo polovico.\n");
		mergeSort(array, min1, max1, submergeSortSimple);
		sem_post(sem);
     	        exit(0);
     	default:
		printf("Jaz sem starš, čakam.\n");
		wait(NULL);
	}
	pid2 = fork();
	switch(pid2){
       	case -1:
		sem_unlink("semafor");
		sem_close(sem);  
       	 	perror("Težava pri ustvarjanju otroka.");
		exit(-1);
       	case 0:
		sem_wait(sem);
		printf("Jaz sem otrok, sortiram drugo polovico.\n");
		mergeSort(array, min2, max2, submergeSortSimple);
		sem_post(sem);
     	        exit(0);
     	default:
		printf("Jaz sem starš, čakam.\n");
		wait(NULL);
	}

	if(sem_unlink("semafor") == -1){
        	perror("tezava pri brisanju semaforja");
        	exit(-1);
    	}
	if(sem_close(sem) == -1){
        	perror("tezava pri zapiranju semaforja");
        	exit(-1);
    	}
	/*count++;
	count++;

} else {
mergeSort(array, min1, max1, submergeSortSimple);
mergeSort(array, min2, max2, submergeSortSimple);
}*/

}
/*A Mutex is a lock that we set before using a shared resource and release after using it.
When the lock is set, no other thread can access the locked region of code.*/
void *thread_sort(void *args)
{	 
	if(pthread_mutex_lock(&lock) != 0){
		perror("Pthread lock neuspešen.");
    	}
	thread_args *thr_arg = (thread_args*)args;
	mergeSort((*thr_arg).tabela, (*thr_arg).zacetek, (*thr_arg).konec, submergeSortSimple);
	if(pthread_mutex_unlock(&lock) != 0){
		perror("Pthread unlock neuspešen");
   	}
}

// TODO: funkcija, ki paralelizira sortiranje z uporabo niti
// za preprosto paralelizacijo samo izvedemo vsak klic mergeSort 
// funkcije v svoji niti, in počakamo, da se klica zaključita
void submergeSortThread(int* array, int min1, int max1, int min2, int max2){
//if(count < n && count < 15540 || n == 0)
//{
    pthread_t tid1, tid2;
    thread_args arg1, arg2;

    arg1.tabela = array;
    arg1.zacetek = min1;
    arg1.konec = max1;

    arg2.tabela = array;
    arg2.zacetek = min2;
    arg2.konec = max2;

    printf("Prva nit, sortiram prvo polovico.\n");
    if(pthread_create(&tid1, 0, thread_sort, &arg1) != 0){
	perror("Pthread create neuspešen");
    }
    printf("Druga nit, sortiram drugo polovico.\n");
    if(pthread_create(&tid2, 0, thread_sort, &arg2) != 0){
	perror("Pthread create neuspešen");
    }
    if(pthread_join(tid1, 0) != 0){
	perror("Pthread join neuspešen");
    }
    if(pthread_join(tid2, 0) != 0){
	perror("Pthread join neuspešen");
    }
    if(pthread_mutex_destroy(&lock) != 0){
	perror("Pthread destroy neuspešen");
    }
    printf("Sortirano\n");
    
   /* count++;
    count++;
} else {
mergeSort(array, min1, max1, submergeSortSimple);
mergeSort(array, min2, max2, submergeSortSimple);
}*/
}

// mergeSort in merge funkciji
// ti dve izvajata dejansko sortiranje

// void mergeSort(int *array, int min, int max, void(*submergeSort)(int *, int, int, int, int) )
//
// int *array
//   kazalec na tabelo števil, ki jih urejamo
//
// int min, int max
//   indeks prvega in zadnjega števila v tabeli, 
//   označujeta interval, ki ga sortiramo
//
// void (*submergeSort)(int *array, int min1, int max1, int min2, int max2)
//   kazalec na funkcijo, ki naj kliče mergeSort za dva podintervala 
//   in vrne, ko sta oba intervala sortirana
void mergeSort(int *array, int min, int max, void(*submergeSort)(int *, int, int, int, int) ){
    int mid;
    if(min < max){
        mid=(min+max)/2;
        
        submergeSort(array, min, mid, mid+1, max);

        merge(array, min, mid, max);
    }
}

// void merge(int *arr, int min,int mid,int max)
// 
// int *arr
//   kazalec na tabelo
//
// int min, int mid, int max
//   indeksi na del tabele, ki jih je potrebno združiti
//   min je začetni indeks prve podtabele, mid je zadnji indeks
//   prve podtabele in max je zadnji indeks druge podtabele
// 
// metoda zdruzi dve sosednji sortirani podtabeli,
// tako da je nova podtabela tudi sortirana
void merge(int *arr, int min,int mid,int max)
{
    // drugi korak algoritma mergeSort
    int *tmp = malloc((max-min+1)*sizeof(int));
    int i,j,k,m;
    j=min;
    m=mid+1;
    for(i=min; j<=mid && m<=max ; i++)
    {
        if(arr[j]<=arr[m])
        {
            tmp[i-min]=arr[j];
            j++;
        }
        else
        {
            tmp[i-min]=arr[m];
            m++;
        }
    }
    if(j>mid)
    {
        for(k=m; k<=max; k++)
        {
            tmp[i-min]=arr[k];
            i++;
        }
    }
    else
    {
        for(k=j; k<=mid; k++)
        {
            tmp[i-min]=arr[k];
            i++;
        }
    }
    for(k=min; k<=max; k++)
        arr[k]=tmp[k-min];

    free(tmp);
}


int main(int argc, char *argv[])
{
    #define NO_PAR 0 
    #define PROC_PAR 1
    #define THREAD_PAR 2
    int technique= NO_PAR;
    void (*submergeSortFun)(int *, int, int, int, int);
    submergeSortFun = submergeSortSimple;
    while(1){
        int c;
        c = getopt(argc, argv, "pt");
        if(c==-1){
            break;
        }
        switch(c){
            case 'p':
                technique = PROC_PAR;
                submergeSortFun = submergeSortProc;
                break;
            case 't':
                technique = THREAD_PAR;
                submergeSortFun = submergeSortThread;
                break;
            default:
                printHelp(argc, argv);
                return 0;
        }
    }

    int i;
    int size;
    int *arr;
    if(optind >= argc){
        printHelp(argc, argv);
        return -1;
    }

    size = atoi(argv[optind]);
    if(argc == 4){
    	n = atoi(argv[3]);
    }
    printf("N: %d \n", n);
    // TODO: inicializacija za razlicne tehnike 
    switch(technique){
        case NO_PAR:
            arr = malloc(sizeof(int)*size);
            break;
        case PROC_PAR:
            // inicializacija za uporabo procesov
            // ustvariti je potrebno deljen pomnilnik, semafor, ...
	            shm_unlink("pom");
            goto Here;
            Here: ;
     	    int pom;
	    pom = shm_open("pom", O_RDWR|O_CREAT|O_EXCL, 0660);
	    if(pom == -1){
      	  	perror("tezava pri ustvarjanju pomnilnika: ");
       	 	return -1;
       	    }
	    ftruncate(pom, sizeof(int)*size);
            close(pom);
	    np = mmap(0, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	    np = &n;
	    arr = mmap(0, sizeof(int)*size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, pom, 0);
	    if (arr == MAP_FAILED) {
       	      	perror("mmap failed");
		return -1;
	    }
            break;
        case THREAD_PAR:
            // inicializacija za uporabo niti
            // tukaj potrebujete morebitne sinhronizacijske strukture
	    arr = malloc(sizeof(int)*size);
            break;
    }

    char buffer[101];
    for(i=0; i < size; i+=1){
        // preberi binarne vrednosti
        read(0, &arr[i], 4);
    }

    for(i=0; i<size; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

    mergeSort(arr, 0, size-1, submergeSortFun);

    for(i=0; i<size; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

    // TODO: ciscenje za razlicnimi tehnikami
    switch(technique){
        case NO_PAR:
            free(arr);
            break;
        case PROC_PAR:
	    dprintf(2, "Ciščenje procesa.\n");
            shm_unlink("pom");
	    munmap(arr, (sizeof(int)*size));
	    if(munmap(arr, (sizeof(int)*size)) != 0){
		perror("Munmap neuspešen");
	    }
	    exit(0);
            break;
        case THREAD_PAR:
            dprintf(2, "Čiščenje niti.\n");
	    exit(0);
            break;
    }

    return 0;
}

void printHelp(int argc, char** argv){
    printf("uporaba\n");
    printf("%s <opcija> <n>\n",argv[0]);
    printf("\tn je število celih števil prebranih z standardnega vhoda\n");
    printf("\tfunkcije prebere n*4 bajtov v tabelo in jih sortira\n");
    printf("opcije:\n");
    printf("-p\n");
    printf("\tparalelizacija s pomočjo procesov\n");
    printf("-t\n");
    printf("\tparalelizacija s pomočjo niti\n");
}
