#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1,"Wrong number of args");
	}
	int pf[2];
	if(pipe(pf) == -1)
	{
		err(1,"error pipe");
	}

	pid_t pid = fork();

	if(pid == -1)
	{
		err(2,"error fork");
	}
	if(pid == 0)
	{	
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1)
		{
			err(3,"error in execlp");
		}
	}
	else
	{
		close(pf[1]);
		char c;
		int rs;
		bool isEscaped = false;
		int fd = open(argv[2],O_CREAT | O_WRONLY, 0644);
		if(fd == -1)
		{
			err(5, "Error opening file");
		}

		while((rs=read(pf[0],&c,sizeof(c))) != 0)
		{
			if(rs == -1)
			{
				close(pf[0]);
				close(fd);
				err(4,"error reading from pipe");
			}

			if(c == 0x7D)
			{
				isEscaped = true;
				continue;
			}

			if(c == 0x55)
			{
				isEscaped = false;
				continue;
			}

			if(isEscaped)
			{
				c = c ^ 0x20; 
			}
			isEscaped = false;
			
			if(write(fd, &c, sizeof(c)) != sizeof(c))
			{
				err(6,"error writing");
			}	
		}
		close(pf[0]);
		close(fd);
	}
}
