#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		errx(1,"Wrong args");
	}

	int N = atoi(argv[1]);
	int D = atoi(argv[2]);

	char ding[] = "DING\n";
	char dong[] = "DONG\n";

	int ctp[2];
	int ptc[2];

	if(pipe(ctp) == -1 || pipe(ptc) == -1)
	{
		err(2,"Error pipe");
	}

	char c;
	pid_t pid = fork();

	if(pid == 0)
	{
		close(ptc[1]);
		close(ctp[0]);

		for(int i=0; i < N; i++)
		{
			if(read(ptc[0],&c,sizeof(c)) != sizeof(c))
			{
				err(4,"error reading");
			}
			
			if(write(1,&dong,sizeof(dong)) != sizeof(dong))
			{
				err(5,"error writing");
			}

			if(write(ctp[1], &c ,sizeof(c)) != sizeof(c))
			{
				err(5, "Error writing");
			}

		}
	}

	else
	{
		close(ptc[0]);
		close(ctp[1]);
		for(int i=0; i < N; i++)
		{
			if(write(1,&ding, sizeof(ding)) != sizeof(ding))
			{
				err(3,"error writing");
			}

			if(write(ptc[1],&c,sizeof(c)) != sizeof(c))
			{
				err(3,"Error writing");
			}

			if(read(ctp[0],&c,sizeof(c)) != sizeof(c))
			{
				err(4,"Error reading");
			}
			sleep(D);
		}
	}

	exit(0);
}
