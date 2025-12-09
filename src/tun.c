/* Author: Ahmed Bouzaffour 2025-12-08
 * Handles the creation and configuration of the Virtual Interface
 */

#include "common.h"



/*
 * Open the TUN device and register it with the kernel
 */
int 
tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;
	
	/* spawn a new viritual network interface */
	fd = open("/dev/net/tun", O_RDWR);
	if (fd < 0)
	{
        	perror("Opening /dev/net/tun");
        	return fd;
    	}

	/* set all bytes to 0 */
	memset(&ifr, 0, sizeof(ifr));

	/*
	 * Flags:
	 * 	IFF_TUN: the IP packet
	 * 	IFF_NO_PI: no metadata
	*/
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
	 
	if (*dev)
	{
		/* setting the name provided */
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}
	 
	/*
	 * Registering the device
	 * TUBNSETIFF: flag to set interface
	*/
	err = ioctl(fd, TUNSETIFF, (void *) &ifr);
	if(err < 0)
	{
		perror("[PANIC] ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}

	/*copy the actunal name back to the caller*/
	strcpy(dev, ifr.ifr_name);

	return fd;
}
