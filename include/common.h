/* Author: Ahmed Bouzaffour 2025-12-08
 * This file contains the standard libraries required for Linux
 * Networking and the function prototypes
 * */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>		// inet_addr, htons
#include <fcntl.h>		// open, O_RDWR
#include <sys/ioctl.h>		// ioctl
#include <sys/socket.h>		// socket, bind, connect
#include <linux/if.h>		// struct ifreq
#include <linux/if_tun.h>	// IFF_TUN, IFF_NO_PI (Tunnel flags)

#define MTU 	  1400		// max transmission unit
#define PORT      55555		// The UDP port where the server listens
#define SERVER_IP "127.0.0.1"	// IP address
/*
 * *Allocates a TUN interface.
 *	param:
 *		dev:  The name of the device (e.g., "tun0") (Can be empty)
 * 	return:
 * 		int File Descriptor (fd) on success, -1 on error.
 */
int tun_alloc(char *dev);


int udp_create_socket();


void crypto_init();
int encrypt_packet(unsigned char *plain, int len, unsigned char *cipher);
int decrypt_packet(unsigned char *cipher, int len, unsigned char *plain);


#endif

