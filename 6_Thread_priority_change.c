#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/time.h>

int main(){
	
	pid_t pid = getpid();
	int priority = getpriority(PRIO_PROCESS,pid);
	printf("Thread is %d\n",pid);
	printf("The original priority is %d\n",priority);
	setpriority(PRIO_PROCESS,pid,15);
	pid_t pid_new = getpriority(PRIO_PROCESS,pid);
	printf("The new priority is %d\n",pid_new);
	//while(1);
	
return 0;
}
