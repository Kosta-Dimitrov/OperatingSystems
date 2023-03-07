#include <stdint.h>
#include<stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct header{
	uint32_t magic;
	uint8_t header;
	uint8_t version;
	uint16_t count;
	uint32_t reserved_1;
	uint32_t reserved_2;
};

struct data_zero{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
};

struct data_one{
	uint32_t offset;
	uint16_t original;
	uint16_t new;
};

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		errx(1,"Not correct parameters");
	}

	int fd1 = open(argv[1],O_RDWR);

	if(fd1 == -1)
	{
		err(2,"Error while opening file");
	}

	int fd2 = open(argv[2], O_RDWR);

	if(fd2 == -1)
	{
		close(fd1);
		err(2, "Error while opening file");
	}

	struct header h;

	if((read(fd1, &h, sizeof(h))) != sizeof(h))
	{
		close(fd1);
		close(fd2);
		err(3, "Error reading from file");
	}

	if(h.magic != 0xEFBEADDE || h.header != 0x01)
	{
		close(fd1);
		close(fd2);
		err(4, "Error in header");
	}

	int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, 0644 );

	if(fd3 == -1)
	{
		close(fd1);
		close(fd2);
		err(3, "Error opening file");
	}

	int rs = 0;

	if(h.version == 0x00)
	{
		struct data_zero data;
		while((rs = read(fd1, &data, sizeof(data))) != 0)
		{
			uint8_t buff;
			if(rs == -1 || rs != sizeof(data))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(4, "Error reading");
			}

			lseek(fd2, data.offset * sizeof(buff) ,SEEK_SET);

			if(read(fd2, &buff, sizeof(buff)) != sizeof(buff))
			{
				err(4,"Error reading");
			}

			if(buff != data.original)
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(5, "No such a byte");
			}

			if(lseek(fd3, data.offset * sizeof(buff), SEEK_SET) == -1)
			{
				err(6, "No such offset");
			}

			if(write(fd3, &data.new, sizeof(data.new)) != sizeof(data.new))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(4, "Writing failiure");
			}
		}
	}

	else if(h.version == 0x00)
	{
		uint16_t buff;

		struct data_one data;

		int res;
		while(res = (read(fd1, &data, sizeof(data))) != 0)
		{
			if(res == -1 || res != sizeof(data))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(3,"Error reading");
			}

			if(lseek(fd2, sizeof(buff) * data.offset, SEEK_SET) == -1)
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(2,"No such offset");
			}

			if(read(fd2, &buff, sizeof(buff)) != sizeof(buff))
			{
				close(fd1);
                close(fd2);
			   	close(fd3);
                err(2,"No such offset");
			}

			if(buff != data.old)
			{
				close(fd1);
                close(fd2);
				close(fd3);
				err(2,"No such offset");

			}

			if(write(fd3, &data.new, sizeof(data.new)) != sizeof(data.new))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(3, "error writing");
			}
		}
	}

	else
	{
		err(1,"error version");
	}
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);

}
