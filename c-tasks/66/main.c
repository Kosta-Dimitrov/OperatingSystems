#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct data{
	uint32_t uid;
	uint16_t useless;
	uint16_t useless2;
	uint32_t start;
	uint32_t end;
};
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1,"Exactly 1 argument expected");
	}

	int fd = open(argv[1], O_RDONLY);

	if(fd == -1)
	{
		err(2,"Error opening file");
	}

	int index = 0;
	int rs;
	struct data dat;
	uint32_t users[2048] = { 0 };
	uint32_t time[2048] = { 0 };
	uint32_t sum = 0;

	while((rs = read(fd, &dat, sizeof(dat))) != 0)
	{
		if(rs != sizeof(dat))
		{
			close(fd);
			err(3,"Error reading from file");
		}
		sum += dat.end - dat.start;
		users[index] = dat.uid;
		time[index] = dat.end - dat.start;
		index++;
	}

	uint32_t average = -1;

	if(index != 0)
	{
		average = sum / index;
	}

	uint32_t d = 0;

	lseek(fd, 0, SEEK_SET);

	while((rs = read(fd, &dat, sizeof(dat))) != 0)
	{
		if(rs != sizeof(dat))
		{
			close(fd);
			err(3,"Error reading from file");
		}
		d += (dat.end - dat.start - average) * (dat.end - dat.start - average);
	}

	if(index != 0)
	{
		d = d / index;
	}

	lseek(fd, 0, SEEK_SET);
	uint32_t visited_users[2048] = { 0 };
	int number_of_visited = 0;

	while ((rs = read(fd, &dat, sizeof(dat))) != 0) 
	{
		if (rs != sizeof(dat)) 
		{
			close(fd);
			err(3, "Error reading from file");
		}

		uint64_t time_spent = dat.end - dat.start;

		if(time_spent * time_spent > d)
		{
			bool found = false;
			for(int i=0; i < number_of_visited; i++)
			{
				if(visited_users[i] == dat.uid)
				{
					found = true;
					break;
				}
			}
			
			if(!found)
			{
				visited_users[number_of_visited] = dat.uid;
				number_of_visited++;

				uint32_t max = -1;

				for(int i = 0; i < index; i++)
				{
					if(dat.uid == users[i] && time[i] > max)
						{
							max = time[i];
						}
				}

				if (write(1, &dat.uid, sizeof(dat.uid)) != sizeof(dat.uid)) 
				{
					close(fd);
					err(4, "Writing failure!");
				}

				if (write(1, &max, sizeof(max)) != sizeof(max)) 
				{
					close(fd);
					err(4, "Writing failure!");
				}
				write(1,"\n",sizeof("\n"));
			}
		}
	}

	close(fd);
	exit(0);
}
