#include<stdio.h>
#include<stdlib.h>

int main()
{
	for(int i=0;i<10;i++)
	{
		if(fork() == 0)
		{
			printf("child pid: %d is from parent pid: %d\n",getpid(),getppid());
			exit(0);
		}
	}

	for(int i=0;i<10;i++)
	wait(NULL);
}