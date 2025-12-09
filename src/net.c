/* Author: Ahmed Bouzaffour 2025-12-08
 * Creating a UDP socket
*/

#include "common.h"

int
udp_create_socket()
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("[ERROR] Socket creation failed");
		return -1;
	}
	return sockfd;
}
