#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t m;
volatile int counter = 0; 
int loops;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
      pthread_mutex_lock(&m);
      ++counter;
      pthread_mutex_unlock(&m);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    if (argc != 2) { 
      exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    pthread_mutex_init(&m, NULL);
    gettimeofday(&tv1, NULL);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    gettimeofday(&tv2, NULL);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Time cost - %ld.%ld\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}