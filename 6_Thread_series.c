#include <pthread.h>
#include <stdio.h>
void* fun(void* ptr) {
int i = *((int *) ptr);
printf("%d ", i);
return NULL;
}
int main() {
int i;
pthread_t tid[10];
for(i =0; i < 10; i++) {
pthread_create(&tid[i], NULL, fun, &i);
pthread_join(tid[i], NULL);
}
//for(i =0; i < 10; i++) {
//pthread_join(tid[i], NULL);
//}
pthread_exit(NULL);
}
