#include<err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

struct triple{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
};
int main(int argc,char* argv[])
{
	if(argc != 4)
	{
		err(1,"args");
	}
	int patch = open(argv[1],O_RDONLY);
	int f1 = open(argv[2], O_RDONLY);
	int f2 = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, 0644);

	if(f1 == -1 || f2 == -1 || patch == -1) 
	{
		close(f1);
		close(f2);
		close(patch);
		err(2,"error opening");
	}

	struct triple data;

	int rs;

	uint8_t temp;
	while((rs =(read(f1,&temp,sizeof(temp)))) != 0)
	{
		if(rs != sizeof(temp))
		{
			close(f1);
			close(f2);
			close(patch);
		}

		if(write(f2,&temp,sizeof(temp) != sizeof(temp)))
		{
			close(f1);
			close(f2);
			close(patch);
			err(5,"error writing");
		}
	}

	lseek(f1,0,SEEK_SET);
	lseek(f2,0,SEEK_SET);


	while((rs=(read(patch,&data, sizeof(data)))) != 0)
	{
		if(rs != sizeof(data))
		{
			close(f1);
			close(f2);
			close(patch);
			err(3,"error reading");
		}

		if(lseek(f1, data.offset*sizeof(uint8_t), SEEK_SET) == -1)
		{
			close(f1);
			close(f2);
			close(patch);
			err(4,"error lseek");
		}

		uint8_t tmp;

		if(read(f1,&tmp,sizeof(temp)) != sizeof(tmp))
		{
			close(f1);
			close(f2);
			close(patch);
			err(3,"error reading");
		}

		if(tmp != data.original)
		{
			close(f1);
			close(f2);
			close(patch);
			err(8,"error in file");
		}

		lseek(f2,data.offset*sizeof(uint8_t),SEEK_SET);

		if(write(f2,&data.new,sizeof(data.new)) != sizeof(data.new))
		{
			close(f1);
			close(f2);
			close(patch);
			err(5,"error writing");
		}

	}

	close(f1);
	close(f2);
	close(patch);
	exit(0);
}
