#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "i2c_ioctl_test.h"

int main(void)
{
   int fd = 0;
   int cmd = -1;
   int arg = 0;
   int angle = -1;
   
   fd = open("/proc/cm_ifly_cmd/ifly_cmd", 0);
   if (fd < 0)
   {
       printf("Open ifly_cmd Error!\n");
       return -1;
   }

   cmd = ROBOTCHEERS_TH03_IOCWAKEUP_ANGLE;
   printf("ifly Call ROBOTCHEERS_TH03_IOCWAKEUP_ANGLE %d\n",cmd);
   angle = ioctl(fd, cmd, &arg);
   if (angle < 0)
   {
   		printf("ifly chip get wakeup angle error!\n");
   }
   else
   {
   		printf("ifly chip wakeup angle = %d\n",angle);
   }
   
   cmd = ROBOTCHEERS_TH03_IOCSTATUS;
   printf("ifly Call ROBOTCHEERS_TH03_IOCSTATUS %d\n",cmd);
   if (ioctl(fd, cmd, &arg) == 0)
   {
   		printf("ifly chip is normal status\n");
   }
   else if (ioctl(fd, cmd, &arg) == 1)
   {
   		printf("ifly chip is reset status\n");
   }
#if 1
   cmd = ROBOTCHEERS_TH03_IOCRESET;
   printf("ifly Call ROBOTCHEERS_TH03_IOCRESET %d\n",cmd);
   if (ioctl(fd, cmd, &arg) == 0)
   {
   		printf("ifly chip reset complte\n");
   }
   else
   {
   		printf("ifly chip reset fail\n");
   }

	printf("sleep 15 s ....\n");
	sleep(15);
	
	cmd = ROBOTCHEERS_TH03_IOCWAKEUP;
	printf("ifly Call ROBOTCHEERS_TH03_IOCWAKEUP %d\n",cmd);
	if (ioctl(fd, cmd, &arg) == 0)
	{
		printf("ifly chip wakeup complte\n");
	}
	else
	{
	    printf("ifly chip wakeup fail\n");
	}
#endif
   printf("<--- ifly test end %d --->\n\n");    

   close(fd);
   return 0;    
}
