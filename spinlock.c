#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

pthread_spinlock_t lock;
int pshared = PTHREAD_PROCESS_PRIVATE;
volatile int counter = 0; 
int loops;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
      pthread_spin_lock(&lock);
      ++counter;
      pthread_spin_unlock(&lock);
    }
    pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    if (argc != 2) {
      exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    pthread_spin_init(&lock, pshared);
    gettimeofday(&tv1, NULL);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("result value   : %d\n", counter);
    pthread_spin_destroy(&lock);
    gettimeofday(&tv2, NULL);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Result - %ld.%d\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}