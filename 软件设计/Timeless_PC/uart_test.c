#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>

unsigned char time_buff[8] = {0};
unsigned char time_rx_buff[8] = {0};

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if(tcgetattr(fd,&oldtio) != 0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
 
 	bzero(&newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 

    switch( nBits )
    {
	    case 7: newtio.c_cflag |= CS7;
				break;
		case 8: newtio.c_cflag |= CS8;
				break;
    }

    switch( nEvent )
    {
    	case 'O':	newtio.c_cflag |= PARENB; //O
					newtio.c_cflag |= PARODD;
					newtio.c_iflag |= (INPCK | ISTRIP);
        			break;
    	case 'E':   newtio.c_iflag |= (INPCK | ISTRIP);
					newtio.c_cflag |= PARENB;
					newtio.c_cflag &= ~PARODD;
					break;
    	case 'N':	newtio.c_cflag &= ~PARENB;
					break;
    }

	switch( nSpeed )
    {
		case 2400:	cfsetispeed(&newtio, B2400);
        			cfsetospeed(&newtio, B2400);
        			break;
    	
		case 4800:	cfsetispeed(&newtio, B4800);
        			cfsetospeed(&newtio, B4800);
        			break;
    	
		case 9600:	cfsetispeed(&newtio, B9600);
        			cfsetospeed(&newtio, B9600);
        			break;
    	
		case 19200: cfsetispeed(&newtio, B19200);
					cfsetospeed(&newtio, B19200);
					break;

		case 115200:cfsetispeed(&newtio, B115200);
        			cfsetospeed(&newtio, B115200);
        			break;

    	default:	cfsetispeed(&newtio, B9600);
        			cfsetospeed(&newtio, B9600);
        			break;
    }

	switch(nStop)
	{
		case 1: newtio.c_cflag &=  ~CSTOPB;
				break;
		default:newtio.c_cflag |=  CSTOPB;
				break;
	}

	newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);

	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int open_port(int fd,int comport)
{
    long  vdisable;

    if (comport == 1)
    {    
		fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port ttyUSB0");
            return(-1);
        }
        else 
        {
            printf("open ttyUSB0 .....\n");
        }
    }
    else if(comport==2)
    {    fd = open( "/dev/ttyUSB1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port ttyUSB1");
            return(-1);
        }
        else 
        {
            printf("open ttyUSB1 .....\n");
        }    
    }
    else if (comport==3)
    {
        fd = open( "/dev/ttyUSB2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port ttyUSB2");
            return(-1);
        }
        else 
        {
            printf("open ttyUSB2 .....\n");
        }
    }
    if(fcntl(fd, F_SETFL, 0)<0)
    {
        printf("fcntl failed!\n");
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(isatty(STDIN_FILENO)==0)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty success!\n");
    }
//    printf("fd-open=%d\n",fd);
    return fd;
}

int main(void)
{
    int fd;
    int nread, nwrite, i = 0, j = 0;
	time_t time_temp;
	struct tm *p;

    if((fd = open_port(fd, 1)) < 0)
    {
        perror("open_port error");
        return 0;
    }

    if((i = set_opt(fd, 9600, 8, 'N', 1)) < 0)
    {
        perror("set_opt error");
        return 0;
    }

	while(1)
	{
		memset(time_buff, 0, sizeof(time_buff)/sizeof(unsigned char));
		nread = 0,nwrite = 0;

		time(&time_temp);
		p = localtime(&time_temp);

//		printf("%d,%d,%d,",1900+p->tm_year-2000,1+p->tm_mon,p->tm_mday);	
//		printf("%d,%d,%d,%d\n",p->tm_hour,p->tm_min,p->tm_sec,p->tm_wday);
		time_buff[0] = 20;
		time_buff[1] = 1900+p->tm_year-2000;
		time_buff[2] = 1+p->tm_mon;
		time_buff[3] = p->tm_mday;
		time_buff[4] = p->tm_hour;
		time_buff[5] = p->tm_min;
		time_buff[6] = p->tm_sec;
		time_buff[7] = p->tm_wday;
		if((nwrite = write(fd, time_buff, sizeof(time_buff)/sizeof(unsigned char))) < 0)
		{
			perror("Write Error");
			continue;
		}
		//if((nread = read(fd,time_buff,sizeof(time_buff)/sizeof(unsigned char))) < 0)
		//{
		//	perror("Read Error");
		//	continue;
	//	}				
		for(i = 0;i<8;i++)
		{
			printf("%d,",time_buff[i]);
		}
		printf("\n");
		getchar();
		break;
	}

    close(fd);
    return 0;
}
