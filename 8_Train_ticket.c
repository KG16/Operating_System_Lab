#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int ticket_no = 1001;
sem_t ticket_counter;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* ticket_buy(){
	pthread_mutex_lock(&mutex);
	if(sem_trywait(&ticket_counter) == 0){
		printf("This customer has bought ticket %d\n", ticket_no++);
	}
	else{
		printf("This customer could not get a ticket\n");
	}
	int value;
	sem_getvalue(&ticket_counter, &value);
	printf("..Number of berths left: %d\n", value);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* ticket_cancel(){
	pthread_mutex_lock(&mutex);
	sem_post(&ticket_counter);
	printf("One ticket has just been cancelled\n");
	int value;
	sem_getvalue(&ticket_counter, &value);
	printf("..Number of berths left: %d\n", value);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(){
	sem_init(&ticket_counter, 0, 10);
	pthread_t t[20];
	pthread_t cancel[3];

	int i;
	for(i = 0; i < 10; i++){
		pthread_create(&t[i], NULL, ticket_buy, NULL);
	}

	for(i = 0; i < 3; i++){
		pthread_create(&cancel[i], NULL, ticket_cancel, NULL);
	}

	for(i = 10; i < 20; i++){
		pthread_create(&t[i], NULL, ticket_buy, NULL);
	}
	return 0;
}
