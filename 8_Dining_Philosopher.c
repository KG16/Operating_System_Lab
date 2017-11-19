#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

#define PHIL 20
#define left (id)%(PHIL)
#define right (id+1)%(PHIL)

pthread_mutex_t spoons[PHIL];
pthread_mutex_t pr = PTHREAD_MUTEX_INITIALIZER;

int spoon_stat[PHIL]={0};

void think(int id){
	//printf("Philosopher %d is thinking!\n",id);
	//sleep(1);
}

void eat(int id){
	pthread_mutex_lock(&pr);
	printf("Philosopher %d is eating with spoons %d and %d!\n",id,left,right);
	spoon_stat[left]++;
	spoon_stat[right]++;
	int i;
	for(i=0;i<PHIL;i++){
		printf("%d ",spoon_stat[i]);
	}
	printf("\n");
	pthread_mutex_unlock(&pr);
	sleep(1);
	pthread_mutex_lock(&pr);
	spoon_stat[left]--;
	spoon_stat[right]--;
	pthread_mutex_unlock(&pr);
}

void hungry(int id){
	pthread_mutex_lock(&spoons[left]);
	pthread_mutex_lock(&spoons[right]);
	eat(id);
	pthread_mutex_unlock(&spoons[left]);
	pthread_mutex_unlock(&spoons[right]);
}


void* dp(void* id){
	int count=3;
	while(count--){
		think(*(int*)id);
		hungry(*(int*)id);
	}
}

int main(){
	
	int i;
	for(i=0;i<PHIL;i++){
		//spoons[i] = PTHREAD_MUTEX_INITIALIZER;
	}
	pthread_t p[PHIL];
	int ids[PHIL];
	for(i=0;i<PHIL;i++){
		ids[i]=i;
	}
	for(i=0;i<PHIL;i++){
		pthread_create(&p[i],NULL,dp,&ids[i]);
	}
	pthread_exit(NULL);
	
return 0;
}
