#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFF_SIZE 10
int item =0;
int sum_r=0,sum_w=0;

struct sbuf_t{
	int buff[BUFF_SIZE];
	int write_h;
	int read_h;
//	int nItem;
	sem_t full;
	sem_t empty;
	pthread_mutex_t mutex;
};

struct sbuf_t shared;

void* producer(){
	for(int i=0;i<10;i++){
		sem_wait(&shared.empty);
		pthread_mutex_lock(&shared.mutex);
		//Entering CS
		int k=rand()%100+1;
		shared.buff[shared.write_h] =k;
		sum_w+=k;
		shared.write_h = (++shared.write_h)%BUFF_SIZE;
		printf("Produced %d \n",k);
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
	for(int i=0;i<10;i++){
		sem_wait(&shared.full);
		pthread_mutex_lock(&shared.mutex);
		//Entering CS
		printf("Consumed %d \n",shared.buff[shared.read_h]);
		sum_r+=shared.buff[shared.read_h];
		shared.buff[shared.read_h] = 0;
		shared.read_h = (++shared.read_h)%BUFF_SIZE;
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
	pthread_t p,c;int i=0;
	shared.read_h = 0;
	shared.write_h = 0;
	sem_init(&shared.full,0,0);
	sem_init(&shared.empty,0,BUFF_SIZE);
	pthread_mutex_init(&shared.mutex,NULL);
	pthread_create(&p,NULL,producer,NULL);
	pthread_create(&c,NULL,consumer,NULL);
	pthread_join(&p,NULL);
	pthread_join(&c,NULL);
	printf("%d,  %d ",sum_r, sum_w);
	pthread_exit(NULL);
	
return 0;
}

