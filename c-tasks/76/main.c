#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
	while(true)
	{
		int pf[2];
		printf("Command:");
		char c;
		char buff[100];
		int rs;
		if(pipe(pf) ==-1)
		{
			err(1,"Pipe failed");
		}
		pid_t pid = fork();

		if(pid == -1)
		{
			err(2,"fork failed");
		}

		if(pid != 0)
		{
			close(pf[0]);
			int status;
			while((rs = read(0,&c,1)) != 0)
			{
				if(rs == -1)
				{
					err(2,"Error reading");
				}
				if(c == '\n')
				{
					close(pf[1]);
					wait(&status);
					printf("exit status: %d\n",WEXITSTATUS(status));
					if(WIFEXITED(status) && WEXITSTATUS(status) == 1)
					{
						exit(0);
					}
					else
					{
						break;
					}
				}
				else
				{
					write(pf[1],&c,1);
				}
			}

		}
		else
		{
			close(pf[1]);
			dup2(pf[0],0);
			if(read(0, buff,sizeof(buff)) == -1)
			{
				err(2,"error reading");
			}

			if(strcmp(buff,"exit") == 0)
			{
				exit(1);
			}

			if(execlp(buff, buff, (char*)NULL) == -1)
			{
				err(3, "Err execlp ");
			}
		}
	}
}
