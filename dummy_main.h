#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/wait.h>  
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

int dummy_main(int argc, char **argv);


int main(int argc, char **argv) {
    int i=0;
    while(argv[i+1]!=NULL){
        i++;
    }
    int NCPUs= atoi(argv[i]);
    typedef struct {
        int arr[NCPUs];
        sem_t mutex;
        int count;
    }flag_array;


    int fd2 =shm_open("sm3",O_RDWR, 0666);
    flag_array* terminated_array = mmap(NULL, sizeof(flag_array), PROT_READ | PROT_WRITE, MAP_SHARED, fd2 , 0);
    close(fd2);
    int child_pid=fork();
    if(child_pid==0){
        int res=dummy_main(argc, argv);
        exit(0);
    }
    else{    
        wait(NULL);
        // printf("In the process - %d\n",getpid());
        sem_wait(&terminated_array->mutex);
        terminated_array->arr[terminated_array->count]=getpid();
        terminated_array->count++;
        sem_post(&terminated_array->mutex);
        exit(0);
    }
    int ret = dummy_main(argc, argv);
    return ret;
}


#define main dummy_main


