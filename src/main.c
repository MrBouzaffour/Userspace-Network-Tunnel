/* Author: Ahmed Bouzaffour 2025-12-08
 * Enty point for this application
 * Currently:
 * 	1/ initlize the TUN interface
 * 	2/ Event loop for packet processing
 * */

#include "common.h"

int
main()
{

	char tun_name[IFNAMSIZ];	// interface name
	
	/*set the desired name*/
	strcpy(tun_name, "tun0");

	printf("[INFO] attempting to create TUN device...\n");
	
	int tun_fd = tun_alloc(tun_name);

	if (tun_fd < 0)
	{
		fprintf(stderr, "[PANIC] failed to allocate TUN interface\n");
		return 1;
	}

	printf("[SUCCESS] Interface '%s' allocated. FD: %d\n", tun_name, tun_fd);

	while(1)
	{
		sleep(10);
	}
	return 0;
}
