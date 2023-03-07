#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

struct header{
	uint32_t unused;
	uint16_t count;
	uint32_t unused2;
	uint32_t unused3;
	uint16_t unused4;
}__attribute__((packed));

struct interval {
	uint16_t start;
	uint16_t num;
};

struct complect {
	struct interval postfix_interval;
	struct interval prefix_interval;
	struct interval infix_interval;
	struct interval suffix_interval;
}__attribute__((packed));
int main(int argc, char* argv[])
{
	if(argc != 7)
	{
		errx(1,"Expected 6 arguments");
	}
	int fd[6];

	for(int i=1; i < 6; i++)
	{
		fd[i - 1] = open(argv[i],O_RDONLY);
		if(fd[i - 1] == -1)
		{
			err(2,"Error opening");
		}
	}

	fd[5] = open(argv[6],O_CREAT | O_TRUNC | O_WRONLY, 0644);


	struct complect compl;
	int rs;
	while((rs = read(fd[0],&compl,sizeof(compl))) != 0)
	{
		if(rs != sizeof(compl))
		{
			err(3,"Error reading");
		}

		if(lseek(fd[1],compl.postfix_interval.start*sizeof(uint32_t), SEEK_SET) == -1)
		{
			err(4,"Error in lseek");
		}
		for(int i = 0;i<compl.postfix_interval.num;i++)
		{
			uint32_t buff;
			if(read(fd[1],&buff,sizeof(buff)) != sizeof(buff))
			{
				err(3,"Error in lseek");
			}
			
			if(write(fd[5],&buff,sizeof(buff)) != sizeof(buff))
			{
				err(5,"Eror write");
			}
		}

		if(lseek(fd[2],compl.prefix_interval.start*sizeof(uint8_t),SEEK_SET)== -1)
		{
			err(4,"error in lseek");
		}

		for(int i = 0;i<compl.prefix_interval.num;i++)
		{
			uint8_t buffer;
			if(read(fd[2],&buffer,sizeof(buffer)) != sizeof(buffer))
			{
				err(3,"Error read");
			}
			if(write(fd[5],&buffer,sizeof(buffer)) != sizeof(buffer))
			{
				err(4,"Error write");
			}
		}


		if(lseek(fd[3],compl.infix_interval.start*sizeof(uint16_t),SEEK_SET)    == -1)
          {
              err(4,"error in lseek");
          }
 
          for(int i = 0;i<compl.infix_interval.num;i++)
          {
              uint16_t buffer;
              if(read(fd[3],&buffer,sizeof(buffer)) != sizeof(buffer))
              {
                  err(3,"Error read");
              }
              if(write(fd[5],&buffer,sizeof(buffer)) != sizeof(buffer))
			  {
				  err(4,"Error write");
			  }
          }


		  if(lseek(fd[4],compl.suffix_interval.start*sizeof(uint64_t),SEEK_SET)    == -1)
          {
              err(4,"error in lseek");
          }
 
          for(int i = 0;i<compl.suffix_interval.num;i++)
          {
              uint64_t buffer;
              if(read(fd[4],&buffer,sizeof(buffer)) != sizeof(buffer))
              {
                  err(3,"Error read");
              }
              if(write(fd[5],&buffer,sizeof(buffer)) != sizeof(buffer))
			  {
				  err(4,"Error write");
			  }
          }
	}
	for(int i = 0; i < 6; i++)
	{
		close(fd[i]);
	}
	exit(0);
}
