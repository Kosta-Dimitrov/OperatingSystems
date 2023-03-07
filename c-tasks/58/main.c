struct data
{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
};
int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		errx(1, "Invalid number of parameters");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1)
	{
		err(2, "Error openin file");
	}
    int fd2 = open(argv[2], O_RDONLY);

	if(fd2 == -1)
	{
		close(fd1);
		err(2, "Error opening file 2");
	}

	int fd3 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);

	if(fd3 == -1)
	{
		close(fd1);
		close(fd2);
		err(2, "Error opening file 3");
	}

	uint8_t original;
	uint8_t new;
	uint16_t offset = 0;

	int rs;
	struct data dat;

	while((rs = read(fd1, &original, sizeof(original))) != 0)
	{
		if(rs == -1 || rs != sizeof(original))
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3,"Error reading");
		}

		if (read(fd2, &new, sizeof(new)) != sizeof(new)) 
		{
			close(fd1);
			close(fd2);
			close(fd3);
			err(3, "Reading failure!");
		}

		if(new != original)
		{
			dat.new = new;
			dat.offset = offset;
			dat.original = original;

			if(write(fd3, &dat, sizeof(dat)) != sizeof(dat))
			{
				close(fd1);
				close(fd2);
				close(fd3);
				err(4, "Writing error");
			}
		}

		offset++;

	}
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
