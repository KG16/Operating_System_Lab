#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syscall.h>
#include<sys/wait.h>
#include<string.h>

// "find" command searches in subdirectories only!!!
int main(int argc, char **argv)
{
	
	int pfd[2];
	char buf[3000];
	pipe(pfd);
	int status;
	if(fork()==0)
	{
		dup2(pfd[1],1);
		close(pfd[0]);
		close(pfd[1]);
		if(execlp("find","find",".","-inum",argv[1],NULL)==-1)
		perror("EXECLP error");
	}
	else
	{
		wait(&status);
		read(pfd[0], buf, 3000);
		buf[(int)strlen(buf)-1]='\0';
		printf("File Adress: ");
		printf("%s \nlength= %d\n\n",buf,(int)strlen(buf));
		FILE *f;
		f=fopen(buf,"r");
		
		if(f)
		{
			char ch[10000];
			while(fscanf(f,"%c",ch)!=EOF)
			printf("%s",ch);
		
			fclose(f);
		}
		else
		printf("Error: file adress!\n");
	}
	//execlp("find","find",".","-inum",argv[1],NULL);
	//printf("%s",buff);
	//execlp("ls", "ls", NULL);
}
// 20449477
