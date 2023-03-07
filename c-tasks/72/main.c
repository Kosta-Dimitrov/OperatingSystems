struct header{
	uint16_t magic;
	uint16_t type;
	uint32_t count;
};
int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		errx(1,"Not right arguments");
	}

	int lst = open(argv[1],O_RDONLY);
	if(lst == -1)
	{
		err(2,"Error opening");
	}

	int data = open(argv[2],O_RDONLY);
	if(data == -1)
	{
		close(lst);
		err(2,"Error opening");
	}

	int out = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(out == -1)
	{
		close(lst);
		close(data);
		err(2,"Error opning");
	}

	struct header h_lst;
	struct header h_data;
	struct header h_out;

	if(read(lst,&h_lst,sizeof(lst)) != sizeof(lst))
	{
		err(3,"error reading");
	}

	if(read(data,&h_data,sizeof(h_data)) != sizeof(h_data))
	{
		err(3,"error reading");
	}

	if(h_lst.magic != 0X5A4D || h_lst.type != 1)
	{
		err(4,"Inconsistent files");
	}
	if(h_data.magic != 0X5A4D || h_data.type !=2)
	{
		err(4,"Inconsistent files");
	}

	h_out.count = h_lst.count;
	h_out.type = 3;
	h_out.magic = h_lst.magic;

	if(write(out,&h_out,sizeof(h_out)) != sizeof(h_out))
	{
		close(out);
		close(lst);
		close(data);
		err(5,"Error writing");
	}

	uint32_t offset;
	uint16_t value;

	int rs;
	while((rs = read(lst,&value,sizeof(value))) != 0)
	{
		if(rs != sizeof(value))
		{
			close(lst);
			close(data);
			close(out);
			err(4,"error reading");
		}

		if ((offset = lseek(lst,0,SEEK_CUR)) == -1)
		{
			close(lst);
			close(data);
			close(out);
			err(6,"error in lseek");
		}

		if(lseek(data, offset*sizeof(uint32_t)+sizeof(h_data),SEEK_SET) == -1)
		{
			close(lst);
			close(data);
			close(out);
			err(6,"error in lseek");
		}

	   uint32_t buff;
	   if(read(data,&buff,sizeof(buff)) != sizeof(buff))
	   {
		   close(lst);
		   close(data);
		   close(out);
		   err(4,"error reading");
	   }

	   if(buff == 0)
	   {
		   continue;
	   }

	   if(lseek(out,value*sizeof(uint64_t)+sizeof(h_out),SEEK_SET) == -1)
	   {
		   close(lst);
		   close(data);
		   close(out);
		   err(6,"error in lseek");
	   }

	   if(write(out,&buff,sizeof(buff)) != sizeof(buff))
	   {
		   close(lst);
		   close(data);
		   close(out);
		   err(5,"error writing");
	   }
	}
	close(lst);
	close(data);
	close(out);
}
