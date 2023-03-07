#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

struct data_header{
	uint32_t magic;
	uint32_t count;
};

struct comp_header{
	uint32_t magic1;
	uint16_t magic2;
	uint16_t reserved;
	uint64_t count;
};

struct tuple{
	uint16_t type;
	uint16_t reserved1;
	uint16_t reserved2;
	uint16_t reserved3;
	uint32_t offset1;
	uint32_t offset2;
};
int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		errx(1,"Invalid number of args");
	}

	int data = open(argv[1], O_RDWR);

	if(data == -1)
	{
		err(2,"Error opening");
	}

	int comp = open(argv[2],O_RDONLY);

	if(comp == -1)
	{
		close(data);
		err(2,"Error opening");
	}

	struct comp_header comp_hdr;
	struct data_header data_hdr;

	if(read(comp,&comp_hdr,sizeof(comp_hdr)) != sizeof(comp_hdr))
	{
		close(data);
		close(comp);
		err(3,"error reading");
	}

	if(read(data,&data_hdr,sizeof(data_hdr)) != sizeof(data_hdr))
	{
		close(data);
		close(comp);
		err(3,"Error reading");
	}	

	if(data_hdr.magic != 0x21796F4A || comp_hdr.magic1 != 0xAFBC7A37 || comp_hdr.magic2 != 0x1C27)
	{
		err(4,"Files not in correct format");
	}

	struct tuple tpl;

	int rs;
	while((rs = read(comp,&tpl,sizeof(tpl))) != 0)
	{
		if(rs != sizeof(tpl))
		{
			close(data);
			close(comp);
			err(3,"error reading");
		}

		if(tpl.type != 0 && tpl.type != 1)
		{
			close(data);
			close(comp);
			errx(99,"Inconsistent data");
		}
		uint64_t num1;
		uint64_t num2;

		if(lseek(data,tpl.offset1*sizeof(uint64_t) + sizeof(data_hdr),SEEK_SET) == -1)
		{
			close(data);
			close(comp);
			err(5,"error in lseek");
		}

		if(read(data,&num1,sizeof(num1)) != sizeof(num1))
		{
			close(data);
			close(comp);
			err(3,"error reading");
		}

		if(lseek(data,tpl.offset2*sizeof(uint64_t) + sizeof(data_hdr),SEEK_SET) == -1)
		{
			close(data);
			close(comp);
			err(5,"Error in lseek");
		}

		if(read(data,&num2,sizeof(num2)) != sizeof(num2))
		{
			close(data);
			close(comp);
			err(3,"Error reading");
		}

		if((tpl.type == 0 && num1>num2) || (tpl.type == 0 && num1 < num2))
		{
			if(lseek(data,(tpl.offset1*sizeof(uint64_t)) + sizeof(data_hdr) ,SEEK_SET) == -1 || (write(data,&num2,sizeof(num2)) != sizeof(num2)))
			{
				close(data);
				close(comp);
				err(6,"Error writing");
			}

			if(lseek(data, (tpl.offset2 * sizeof(uint64_t)) + sizeof(data_hdr), SEEK_SET) == -1 || (write(data,&num1,sizeof(num1)) != sizeof(num1)))
			{
				close(data);
				close(comp);
				err(6,"Error writing");
			}
		}

		if(tpl.type == 1 && num1 < num2)
		{
			//covered by other if
		}
	}
	close(data);
	close(comp);
	exit(0);
}
