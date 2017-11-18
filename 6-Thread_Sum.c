#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int sum=0;

void* add_fx(){  // has to be void* not int
	int i=0;
	for(i=0;i<5;i++){
		sum+=i;
	}
	return NULL;
}

int main(){
	pthread_t t[5];
	int i=0;
	for(i=0;i<5;i++){
		pthread_create(&t[i], NULL, add_fx, NULL);  // will return 0 if successfully created thread
		printf("1. Sum %d\n", sum);
		pthread_join(t[i],NULL);
		printf("2. Sum %d\n", sum);    //no &t[i] here
	}
	return 0;
}
