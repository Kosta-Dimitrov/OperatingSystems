#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct record
{
	uint16_t offset;
	uint8_t length;
	uint8_t useless; 
}
int main(const int argc, char* argv[])
{
	if(argc != 5)
	{
		errx(1, "Wrong number of parameters");
	}

	int f1_dat = open(argv[1], O_RDWR);

	if(f1_dat == -1)
	{
		err(2, "Error while opening file");
	}

	int f1_idx = open(argv[2], O_RDWR);

    if(f1_idx == -1)
    {
		close(f1_dat);
		err(2, "Error while opening file");
    }

	int f2_dat = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY);

	if (f2_dat == -1) 
	{
		close(f1_idx);
		close(f1_dat);
		err(2, "Opening failure!");
	}

	int f2_idx = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY);

	if (f2_idx == -1) 
	{
		close(f1_idx);
		close(f1_dat);
		close(f2_dat);
		err(2, "Opening failure!");
	}
	
	struct record rec;
	int rs;

	while((rs = read(f1_idx, &rec, sizeof(rec))) != 0)
	{
		if(rs == -1)
		{
			close(f1_idx);
			close(f2_idx);
			close(f1_dat);
			close(f2_dat);
			err(3, "Error reading");
		}
		lseek(f1_dat, rec.offset, SEEK_SET);
		uint8_t* buff = (uint8_t*) malloc(rec.length);

		if(read(f1_dat, buff,sizeof(buff)) != sizeof(buff))
		{
			close(f1_dat);
			close(f2_dat);
			close(f1_idx);
			close(f2_idx);
			err(3, "Error reading");
		}

		if(buff[0] >= 0x41 && buff[0] <= 0x5A)
		{
			if(write(f2_dat, buff, sizeof(buff)) != sizeof(buff))
			{
			 close(f1_dat);
             close(f2_dat);
             close(f1_idx);
             close(f2_idx);
			 err(4, "Error writing");
			}

			if(write(f2_idx, &rec, sizeof(rec)) != sizeof(rec))
			{
			  close(f1_dat);
              close(f2_dat);
              close(f1_idx);
              close(f2_idx);
              err(4, "Error writing");

			}
		}
		free(buff);
	}
	close(f1_dat);
    close(f2_dat);
    close(f1_idx);
    close(f2_idx);
    exit(0);
}
