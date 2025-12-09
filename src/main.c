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
	char buffer[MTU];		// the raw packet data
	
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

	char cmd[256];
	sprintf(cmd, "ip addr add 10.0.0.1/24 dev %s && ip link set %s up",
			tun_name, tun_name);
	
	printf("[INFO] Running command: %s\n", cmd);
	system(cmd);
	
	printf("[INFO] Interface configured. Listening for packets...\n");

	/*reading loop*/
	while(1)
	{
		/*blocks until the kernel has a packet*/
		ssize_t nread;

		nread = read(tun_fd, buffer, sizeof(buffer));
		if (nread < 0)
		{
			perror("[PANIC] reading from interface");
			close(tun_fd);
			exit(1);
		}
		
		printf("[PACKET] Read %ld bytes from tunnel\n", nread);
		if (nread > 0) {
			printf("         First byte: 0x%02X\n", (unsigned char)buffer[0]);
		}
	}
	return 0;
}
