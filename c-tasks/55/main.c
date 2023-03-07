#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>

int main(const int argc, char* argv[])
{
	if( argc != 4)
	{
		errx(1, "Wrong number of arguments");
	}

	int fd1 = open(argv[1], O_RDWR);

	if(fd1 == -1)
	{
		err(2, "Error while opening file");
	}

	int fd2 = open(argv[2], O_RDWR);

	if(fd2 == -1)
	{
		int err_code = errno;
		close(fd1);
		errno = err_code;
		err(2, "Error while opening file");
	}

	int fd3 = open(argv[3], O_RDWR);

    if(fd3 == -1)
    {
         int err_code = errno;
         close(fd1);
		 close(fd2);
         errno = err_code;
         err(2, "Error while opening file");
    }

	uint32_t offset;
	uint32_t length;
	uint32_t current_num;
	int rs;
	
	while((rs = read(fd1, &offset, sizeof(offset))) != 0)
	{
		if(rs == -1)
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3, "Error reading from file");
		}

		if(read(fd1, &length, sizeof(length)) != sizeof(length))
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3, "Error reading from file");
		}

		lseek(fd2, offset * sizeof(offset), SEEK_SET);

		for(uint32_t i = 0; i < length; i++)
		{
		
			rs = read(fd2, &current_num, sizeof(current_num));
			if(rs != sizeof(current_num))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(3, "Error reading from file");
			}

			if((write(fd3, &current_num, sizeof(current_num)))!=current_num)
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(4, "Error writing in file");
			}

		}
	}
}
