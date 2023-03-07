#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1,"Wrong arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);

	if(fd1 == -1)
	{
		err(2,"Error opening file");
	}

	int fd2 = open(argv[2], O_RDONLY);

	if(fd2 == -1)
	{
		close(fd1);
		err(2,"Error opening file");
	}

	int fd3 = open("high.sdl",O_CREAT | O_TRUNC | O_RDWR, 0644);

	if(fd3 == -1)
	{
		close(fd1);
		close(fd2);
		err(2,"Error opening file");
	}

	uint8_t level;
	uint16_t number;
	int rs;

	while((rs = read(fd1, &level, sizeof(level))) != 0)
	{
		if(rs == -1)
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3,"Error reading");
		}

		rs = read(fd2, &number, sizeof(number));

		if(rs == -1)
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3,"Error reading");
		}
		
		if(rs == 0)
		{
			break;
		}

		if(level == 1)
		{
			rs = write(fd3, &number, sizeof(number));
		   	if(rs != sizeof(number))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(4,"Error reading");
			}	
		}

	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
