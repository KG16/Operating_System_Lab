#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
sem_t mutex_read_count;  // no =1 in initailization. Automatic
sem_t mutex_write_count;
sem_t mutex_read_block;
sem_t reader;
sem_t writer;
int writecount=0,readcount=0;
int data=0; //shared resource

void *writer_func(void *arg) {
	long f = (long)arg;
	sem_wait(&mutex_write_count);
	writecount++;
	if(writecount==1)sem_wait(&reader);
	sem_post(&mutex_write_count);
	
	sem_wait(&writer);
	//write
	data++;
	printf("writer thread %d writes %d\n",f,data);
	sem_post(&writer);
	
	sem_wait(&mutex_write_count);
	writecount--;
	if(writecount==0)sem_post(&reader);
	sem_post(&mutex_write_count);
}

void *reader_func(void *arg) {
	sem_wait(&mutex_read_block);
		long f = (long)arg;
		sem_wait(&reader);
			sem_wait(&mutex_read_count);
			
			readcount++;
			if(readcount==1)sem_wait(&writer);
			sem_post(&mutex_read_count);
		sem_post(&reader);
	sem_post(&mutex_read_block);
	
	// read 
	printf("reader thread %d reads %d\n", f, data);

	sem_wait(&mutex_read_count);
	readcount--;
	if(readcount==0)sem_post(&writer);
	sem_post(&mutex_read_count);
}

int main(){
	long i,b;
	pthread_t rtid[10],wtid[10];
	sem_init(&mutex_read_count,0,1);
	sem_init(&mutex_write_count,0,1);
	sem_init(&reader,0,1);
	sem_init(&writer,0,1);
	sem_init(&mutex_read_block,0,1);
	for(i=0;i<=10;i++){
		pthread_create(&wtid[i],NULL,writer_func,(void *)i);
		pthread_create(&rtid[i],NULL,reader_func,(void *)i);
	}
	for(i=0;i<=2;i++){
		pthread_join(wtid[i],NULL);
		pthread_join(rtid[i],NULL);
	}
	return 0;
}
