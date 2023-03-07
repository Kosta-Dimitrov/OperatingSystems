#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
	int count = atoi(argv[1]);
	pid_t p;
	for(int i=0;i < count; i++)
	{
		if((p=fork()) == -1) err(5,"err forking");
		if(p==0)
		{
			break;
		}
	}

	if( p == 0 )
	{
		printf("hello from child\n");
	}
	else
	{
		printf("hello from parent\n");
	}
}
