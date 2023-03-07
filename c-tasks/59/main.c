#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc,char* argv[])
{
	bool hasn = false;
	uint8_t line = 1;
	
	if(strcmp("-n",argv[1]) == 0)
	{
		hasn = true;
	}

	int startArgument = 1;
	
	if(hasn)
	{
		startArgument = 2;
	}

	for(int i = startArgument;i<argc;i++)
	{
		bool isStart = true;
		char buff;
		if(strcmp("-",argv[i]) == 0)
		{
			if(hasn && isStart)
			{
				printf("%d ",line);
				//if(write(1,&line,sizeof(line) != sizeof(line)))
				//{
				//	err(1,"error writing");
				//}

				//if(write(1," ",1) != 1)
				//{
				//	err(1,"error writing");
				//}
			}
			int rs;

			while((rs=read(0,&buff,sizeof(buff))) != 0)
			{
				if(rs == -1)
				{
					err(2,"error reading");
				}

				//if(write(1,&buff,sizeof(buff)) != sizeof(buff))
				//{
				//	err(2,"error reading");
				//}
				printf("%c",buff);

				if(strcmp("\n",&buff) == 0)
				{
					line++;
					break;
				}
			}
			continue;
		}

			int fd = open(argv[i],O_RDONLY);
			
			if(fd == -1)
			{
				err(3,"error opening file");
			}

			int rs;

			while((rs=read(fd,&buff,sizeof(buff))) != 0)
			{
				if(rs == -1)
				{
					err(2,"error reading");
				}

				if(isStart && hasn)
				{
					//if(write(1,&line,sizeof(line)) != sizeof(line))
					//{
					//	err(1,"vfv");
					//}

					printf("%d ",line);
					//if(write(1," ",1) != 1)
					//{
					//	err(1,"fd");
					//}
				}

				//if(write(1,&buff,sizeof(buff)) != sizeof(buff))
				//{
				//	err(1,"bgbg");
				//}
				printf("%c",buff);

				if(strcmp("\n",&buff)==0)
				{
					isStart = true;
					line++;
				}
				else
				{
					isStart = false;
				}
			}
			close(fd);
	}
}
