#include <err.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1,"expected 1 argument");
	}

	int pf[2];

	if(pipe(pf) == -1)
	{
		err(2,"error pipe");
	}

	pid_t pid = fork();

	if(pid == -1)
	{
		err(3,"error fork");
	}

	if(pid == 0)
	{
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("find", "find", argv[1],"-printf", "%C@ %p\n", (char*)NULL) == -1)
		{
			err(4,"execlp error");
		}
	}
	int pf2[2];

	if(pipe(pf) == -1)
	{
		err(2,"Pipe failed");
	}

	pid_t pid2 = fork();

	if(pid2 == -1)
	{
		err(3,"Fork failed");
	}

	if(pid2 == 0)
	{
		close(pf[1]);
		close(pf2[0]);
		dup2(pf[0],0);
		dup2(pf[1],1);
		if(execlp("sort", "sort", "-nr", "-k1", (char*)NULL) == -1)
		{
			err(3,"execlp failed");
		}
	}

	close(pf[0]);
	close(pf[1]);
	close(pf2[1]);
	dup2(pf2[0],0);
	if(execlp("head","head","-1",(char*)NULL) == -1)
	{
		err(3,"execlp failed");
	}	
}
