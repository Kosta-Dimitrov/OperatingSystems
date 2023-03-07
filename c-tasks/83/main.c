#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>

struct data{
	char name[8];
	uint32_t offset;
	uint32_t length;
};
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1,"error in args");
	}

	int fd = open(argv[1],O_RDONLY);
	int count = 0;

	if(fd == -1)
	{
		err(2,"Error opening");
	}

	int pf[2];

	if(pipe(pf) == -1)
	{
		close(fd);
		err(3,"error pipe");
	}

	int rs;
	pid_t pid;
	struct data data;

	while((rs = read(fd,&data,sizeof(data))) != 0)
	{
		if(rs != sizeof(data))
		{
			close(fd);
			err(4,"Error reading");
		}

		if(data.name[7] != 0x00)
		{
			close(fd);
			errx(6,"Error in file");
		}

		pid = fork();
	    if(pid == -1)
		{
			close(fd);
			err(5,"Error fork");
		}

		count++;
	    if(pid == 0)
	    {
		    break;
	    }   
	}

	if(pid == 0)
	{
		close(pf[1]);

		int file = open(data.name, O_RDONLY);

		if(file == -1)
		{
			close(fd);
			err(7,"error opening file");
		}

		if(lseek(file, data.offset*sizeof(uint16_t),SEEK_SET) == -1)
		{
			close(fd);
			err(8,"Error in lseek");
		}

		uint16_t buff;

		for(int i=0;i<data.length;i++)
		{
			if(read(file, &buff, sizeof(buff)) != sizeof(buff))
			{
				close(fd);
				err(9,"Error reading");
			}

			rs = rs ^ buff;
		}

		if(write(pf[1], &rs,sizeof(rs)) != sizeof(rs))
		{
			close(fd);
			err(10,"Error writing");
		}

		close(file);
		close(pf[0]);
		exit(0);
	}

	close(pf[1]);

	int final = 0;

	for(int i=0;i<count;i++)
	{
		uint16_t buff;
		
		if(read(pf[0],&buff,sizeof(buff)) != sizeof(buff))
		{
			close(fd);
			err(9,"error read");
		}
		final = final ^ buff;
	}

	if(write(0,&final,sizeof(final)) != sizeof(final))
	{
		close(fd);
		err(10,"Error writing");
	}

	close(pf[0]);
	close(fd);
	exit(0);
}
