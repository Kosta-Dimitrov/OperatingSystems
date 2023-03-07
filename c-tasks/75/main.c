#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1,"Expected 1 arg");
	}

	int pf[2];

	if(pipe(pf) == -1)
	{
		err(2,"Pipe failed");
	}

	pid_t pid = fork();

	if(pid == -1)
	{
		err(3, "Fork failed");
	}

	if(pid == 0)
	{
		close(pf[0]);
		dup2(pf[1], 1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1)
		{
			err(4, "Execlp failed");
		}
	}

	close(pf[1]);
	dup2(pf[0], 0);
	if(execlp("sort", "sort", (char*)NULL) == -1)
	{
		err(4,"Execlp failed");
	}
}
