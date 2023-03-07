#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	char ch;
	int rs;

	for(int i=1; i < argc; i++)
	{
		if((strcmp(argv[i], "-")) == 0)
		{
			while((rs=(read(0, &ch, sizeof(ch)))) != 0)
			{
				if(rs == -1)
				{
					err(1, "Error reading from stdin");
				}
				printf("%c", ch);
				if(strcmp(&ch, "\n")==0)
				{
					break;
				}
			}
		}
		else
		{
			int fd = open(argv[i], O_RDONLY);

			if(fd == -1)
			{
				err(2, "Error opening file");
			}

			while((rs = read(fd,&ch, sizeof(ch))) != 0)
			{
				if(rs == -1)
				{
					close(fd);
					err(3, "Error reading from file");
				}

				printf("%c", ch);
			}
			close(fd);	
		}
	}
	exit(0);
}
