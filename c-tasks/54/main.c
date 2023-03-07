#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdarg.h>
#include <stdint.h>

int main(const int argc, const char* argv[])
{
	if(argc!=2)
	{
		errx(1, "Wrong number of parameters");
	}

	int fd = open(argv[1],O_RDWR);

	if(fd==-1)
	{
		err(2,"Cannot open file");
	}

	uint8_t count_sort[256]={0};
	uint8_t current_num;
	int read_bytes;

	while((read_bytes=read(fd, &current_num, sizeof(current_num)))!= 0)
	{
		if(read_bytes == -1)
		{
			int err_code=errno;
			close(fd);
			errno=err_code;
			err(3,"Error while reading");
		}
		count_sort[current_num]++;
	}

	lseek(fd,0,SEEK_SET);

	for(uint8_t i=0;i<255;i++)
	{
		printf("%d:%d\n", i, count_sort[i]);
		for(uint8_t j=0;j<count_sort[i];j++)
		{
			if(write(fd,&i, sizeof(i))!=sizeof(i))
			{
				close(fd);
				err(4,"Error while writing");
			}
		}
	}

	close(fd);
	exit(0);
}
