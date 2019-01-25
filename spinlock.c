#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

pthread_spinlock_t lock;
int pshared = PTHREAD_PROCESS_PRIVATE;
volatile int counter = 0; 
int loops;

void *increment(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
      pthread_spin_lock(&lock);
      ++counter;
      pthread_spin_unlock(&lock);
    }
    pthread_exit(NULL);
}

int fib_rec(int n){
  if (n < 2) {
    return 1;
  } else {
    return fib_rec(n - 1) + fib_rec(n - 2);
  }
}

void *fib(void *arg){
    int i;
    for (i = 0; i < loops; i++){
      pthread_spin_lock(&lock);
      int c = fib_rec(++counter);
      pthread_spin_unlock(&lock);
    }
    pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    if (argc != 3) {
      fprintf(stderr, "usage: ./spin [inc | fib] #loops\n");
      exit(1);
    }
    loops = atoi(argv[2]);
    pthread_t p1, p2;
    pthread_spin_init(&lock, pshared);
    gettimeofday(&tv1, NULL);
    if (strcmp(argv[1], "inc")==0){
      pthread_create(&p1, NULL, increment, NULL); 
      pthread_create(&p2, NULL, increment, NULL);
    } else if (strcmp(argv[1], "fib")==0){
      pthread_create(&p1, NULL, fib, NULL);
      pthread_create(&p2, NULL, fib, NULL);
    } else {
      fprintf(stderr, "usage: ./spin [inc | fib] #loops\n");
      exit(1);
    }
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    gettimeofday(&tv2, NULL);
    printf("result value   : %d\n", counter);
    pthread_spin_destroy(&lock);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Time cost - %ld.%ld\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}