#include <pthread.h>
#include <stdio.h>

int k=0;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void* fun(void* ptr) {
	//
	pthread_t tid = pthread_self();int i;
	for(i=0;i<10;i++){
		pthread_mutex_lock(&lock);
		k++;
		printf("Thread id %ld, sum is %d\n",tid,k);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return NULL;
}

int main() {
	int i;
	pthread_t tid[2];
	for(i =0; i < 2; i++) {
		pthread_create(&tid[i], NULL, fun, &i);
	}
	for(i =0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}
	pthread_exit(NULL);
}
