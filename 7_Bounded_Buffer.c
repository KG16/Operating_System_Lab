#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFF_SIZE 5
#define NO_OF_COSTUMERS 5
#define NO_OF_PRODUCERS 5
#define ITEMS 10

struct sbuf_t{
	int buff[BUFF_SIZE];
	int in;
	int out;
	sem_t full;
	sem_t empty;
	pthread_mutex_t mutex;
};

struct sbuf_t shared;

void* producer(void* t){
	//printf("Producer %d\n",*(int *)t);
	for(int i=0;i<ITEMS;i++){
		sem_wait(&shared.empty);
		pthread_mutex_lock(&shared.mutex);
		//Entering CS
		shared.buff[shared.in] = i+1;
		shared.in = (++shared.in)%BUFF_SIZE;
		printf("Produced %d by Producer %d\n",i+1,*(int *)t);
		//Exiting CS
		for(int j=0;j<BUFF_SIZE;j++){
			printf("%d ",shared.buff[j]);
		}
		printf("\n");
		pthread_mutex_unlock(&shared.mutex);
		sem_post(&shared.full);
	}
	return NULL;
}

void* consumer(void* t){
	//printf("Consumer %d\n",*(int *)t);
	for(int i=0;i<ITEMS;i++){
		sem_wait(&shared.full);
		pthread_mutex_lock(&shared.mutex);
		//Entering CS
		printf("Consumed %d by Consumer %d\n",shared.buff[shared.out],*(int *)t);
		shared.buff[shared.out] = 0;
		shared.out = (++shared.out)%BUFF_SIZE;
		//Exiting CS
		for(int j=0;j<BUFF_SIZE;j++){
			printf("%d ",shared.buff[j]);
		}
		printf("\n");
		pthread_mutex_unlock(&shared.mutex);
		sem_post(&shared.empty);
	}
	return NULL;
}

int main(){
	
	pthread_t p,c;
	shared.in = 0;
	shared.out = 0;
	sem_init(&shared.full,0,0);
	sem_init(&shared.empty,0,BUFF_SIZE);
	pthread_mutex_init(&shared.mutex,NULL);
	for(int i=0;i<NO_OF_PRODUCERS;i++){
		int index = i;
		pthread_create(&p,NULL,producer,&index);
	}
	for(int i=0;i<NO_OF_COSTUMERS;i++){
		int index = i;
		pthread_create(&p,NULL,consumer,&index);
	}
	pthread_exit(NULL);
	
return 0;
}

