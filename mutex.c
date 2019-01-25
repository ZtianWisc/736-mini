#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t lock;
volatile int counter = 0; 
int loops;

void *increment(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
      pthread_mutex_lock(&lock);
      ++counter;
      pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

void *print(void *arg){
    int i;
    for (i = 0; i < loops; i++){
      pthread_mutex_lock(&lock);
      for (int k = 0; k < 9; k++){
        printf("%d, ", counter);
      }
      printf("%d\n", counter);
      ++counter;
      pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    if (argc != 3) {
      fprintf(stderr, "usage: ./mutex [increment | print] #loops\n");
      exit(1);
    }
    loops = atoi(argv[2]);
    pthread_t p1, p2;
    pthread_mutex_init(&lock, NULL);
    gettimeofday(&tv1, NULL);
    if (strcmp(argv[1], "increment")==0){
      pthread_create(&p1, NULL, increment, NULL); 
      pthread_create(&p2, NULL, increment, NULL);
    } else if (strcmp(argv[1], "print")==0){
      pthread_create(&p1, NULL, print, NULL);
      pthread_create(&p2, NULL, print, NULL);
    } else {
      fprintf(stderr, "usage: ./mutex [increment | print] #loops\n");
      exit(1);
    }
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    gettimeofday(&tv2, NULL);
    printf("Final value   : %d\n", counter);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Time cost - %ld.%ld\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}