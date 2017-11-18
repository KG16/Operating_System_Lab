#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int sum=0;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void* add_fx(){  // has to be void* not int
	int i=0;
	pthread_mutex_lock(&lock);
	for(i=0;i<5;i++){
		sum+=i;
	}
	//		pthread_t self;
	//	self = pthread_self();
	//	printf("2. Sum %d %ud\n", sum, self);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main(){
	pthread_t t[5];
	int i=0;
	for(i=0;i<5;i++){
		pthread_create(&t[i], NULL, add_fx, NULL);  // will return 0 if successfully created thread
		//sleep(1);
		printf("1. Sum %d\n", sum);
	}
	for(i=0;i<5;i++)pthread_join(t[i],NULL);
	return 0;
}
