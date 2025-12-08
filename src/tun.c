/* Author: Ahmed Bouzaffour 2025-12-08*/

#include "common.h"

int 
tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;
	
	fd = open("/dev/net/tun", O_RDWR);
	if (fd < 0)
	{
        	perror("Opening /dev/net/tun");
        	return fd;
    	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

	if (*dev)
	{
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	err = ioctl(fd, TUNSETIFF, (void *) &ifr);
	if(err < 0)
	{
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}
	strcpy(dev, ifr.ifr_name);
	return fd;
}
