#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t request_access;
sem_t rmutex;
sem_t order_of_arrival;
int readcount=0;
int data=0; //shared resource

void *writer_func(void *arg) {
	long f = (long)arg;
	sem_wait(&order_of_arrival);
	sem_wait(&request_access);
	sem_post(&order_of_arrival);
	data++;
	printf("writer thread %ld writes %d\n",f,data);
	sleep(1);
	sem_post(&request_access);
}

void *reader_func(void *arg) {
	long f = (long)arg;
	sem_wait(&order_of_arrival);
	sem_wait(&rmutex);
	if(readcount == 0)	sem_wait(&request_access);
	readcount++;
	sem_post(&order_of_arrival);
	sem_post(&rmutex);

	data--;
	printf("reader thread %ld reads %d\n", f, data);
	sleep(1);
	sem_wait(&rmutex);
	readcount--;
	if(readcount == 0)	sem_post(&request_access);
	sem_post(&rmutex);
}

int main(){
	long i,b;
	pthread_t rtid[10],wtid[10];
	sem_init(&order_of_arrival,0,1);
	sem_init(&request_access,0,1);
	sem_init(&rmutex,0,1);
	for(i=0;i<10;i++){
		pthread_create(&wtid[i],NULL,writer_func,(void *)i);
		pthread_create(&rtid[i],NULL,reader_func,(void *)i);
	}
	for(i=0;i<10;i++){
		pthread_join(wtid[i],NULL);
		pthread_join(rtid[i],NULL);
	}
 return 0;
}

