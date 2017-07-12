#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
        char buf[20] = "xiao bai";
	int fd;
	int ret;

        fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

        printf("sizeof(buf) = %d.\n",sizeof(buf));
        ret = write(fd, buf, 10);
        if (ret == -1)
	{	
		printf("errno = %d\n", errno);
	}
	else
	{
		printf("<app>write buf is [%s]\n", buf);
	}

	close(fd);
	return 0;
}
