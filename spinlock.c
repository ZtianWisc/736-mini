#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

pthread_spinlock_t lock;
int pshared = PTHREAD_PROCESS_PRIVATE;
volatile int counter = 0; 
int loops;
size_t n_threads = 32;

void *increment(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
      pthread_spin_lock(&lock);
      ++counter;
      pthread_spin_unlock(&lock);
    }
    pthread_exit(NULL);
}

long long fib_rec(long long n){
  if (n < 2) {
    return (long) 1;
  } else {
    return fib_rec(n - 1) + fib_rec(n - 2);
  }
}

void *fib(void *arg){
    pthread_spin_lock(&lock);
    fib_rec(loops);
    pthread_spin_unlock(&lock);
    pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    if (argc != 4) {
      fprintf(stderr, "usage: ./spin [inc | fib] #loops #threads\n");
      exit(1);
    }
    loops = atoi(argv[2]);
    n_threads = atoi(argv[3]);
    pthread_t p[n_threads];
    pthread_spin_init(&lock, pshared);
    gettimeofday(&tv1, NULL);
    if (strcmp(argv[1], "inc")==0){
      for (int i = 0; i < n_threads; i++){
        pthread_create(&p[i], NULL, increment, NULL);
      }
    } else if (strcmp(argv[1], "fib")==0){
      for (int i = 0; i < n_threads; i++){
        pthread_create(&p[i], NULL, fib, NULL);
      }
    } else {
      fprintf(stderr, "usage: ./spin [inc | fib] #loops\n");
      exit(1);
    }
    for (int i = 0; i < n_threads; i++){
      pthread_join(p[i], NULL);
    }
    gettimeofday(&tv2, NULL);
    pthread_spin_destroy(&lock);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Time cost - %ld.%ld\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}