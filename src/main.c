/* Author: Ahmed Bouzaffour 2025-12-08*/

#include "common.h"

int
main()
{
	char tun_name[IFNAMSIZ];
	strcpy(tun_name, "tun0");

	printf("[LOG] attempting to create TUN device...\n");
	int tun_fd = tun_alloc(tun_name);

	if (tun_fd < 0)
	{
		fprintf(stderr, "[PANIC] failed to allocate TUN\n");
		return 1;
	}
	printf("[SUCCESS] Interface '%s' allocated. FD: %d\n", tun_name, tun_fd);

	while(1)
	{
		sleep(10);
	}
	return 0;
}
