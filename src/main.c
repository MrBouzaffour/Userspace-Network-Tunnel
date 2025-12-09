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
	struct sockaddr_in dest_addr;
	int sock_fd;

	/*set the desired name*/
	strcpy(tun_name, "tun0");
	
	/*initilize Crypto*/
	crypto_init();


	printf("[INFO] attempting to create TUN device...\n");
	
	int tun_fd = tun_alloc(tun_name);

	if (tun_fd < 0)
	{
		fprintf(stderr, "[PANIC] failed to allocate TUN interface\n");
		return 1;
	}

	printf("[SUCCESS] Interface '%s' allocated. FD: %d\n", tun_name, tun_fd);

	/* running a shell commands to assign an IP (10.0.0.1)*/
	char cmd[256];
	sprintf(cmd, "ip addr add 10.0.0.1/24 dev %s && ip link set %s up",
			tun_name, tun_name);
	printf("[INFO] Running command: %s\n", cmd);
	system(cmd);

	/* UDP socket */
	printf("[INFO] Creating UDP socket... \n");
	sock_fd = udp_create_socket();
	if (sock_fd < 0)
	{
		fprintf(stderr, "[PANIC] failed to create a UDP socket\n");
	}

	/*Setting up the destination*/
	memset(&dest_addr, 0, sizeof(dest_addr));
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &dest_addr.sin_addr);

	printf("[INFO] Tunnel is UP. Forwarding packets to %s:%d\n", SERVER_IP, PORT);
	
	fd_set read_set;
	int max_fd = (tun_fd > sock_fd) ? tun_fd : sock_fd;

	// Encrypted buffer with extra space for Nonce (24) + MAC (16)
	unsigned char encrypted_buffer[MTU + 100];

	/* main loop*/
	while(1)
	{
		int activity;
		/* Reset*/
		FD_ZERO(&read_set);
		FD_SET(tun_fd, &read_set);
		FD_SET(sock_fd, &read_set);

		/* wait for data on either interface*/
		
		activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
		
		if (activity < 0)
		{
			perror("[Panic] Select failed");
			break;
		}
		
		/* EVENT A: Packet from Kernel (TUN) -> Send to UDP*/
		if (FD_ISSET(tun_fd, &read_set))
		{
			ssize_t len;
			len = read( tun_fd, buffer, sizeof(buffer));
			
			if (len > 0) 
			{
				/* ENCRYPT */
				int enc_len;
			       	enc_len = encrypt_packet(
						(unsigned char*)buffer, 
						len, 
						encrypted_buffer);
				
				if (enc_len > 0) 
				{
					sendto(
					 	sock_fd, 
						encrypted_buffer, 
						enc_len, 
						0, 
						(struct sockaddr*)&dest_addr, 
						sizeof(dest_addr));
					printf("[OUT] Encrypted %ld bytes -> %d bytes sent\n", len, enc_len);
				}
			}
		}
		
		/* EVENT B: Packet from UDP (Internet) -> Write to Kernel */
		if (FD_ISSET(sock_fd, &read_set)) 
		{
			struct sockaddr_in src_addr;
			ssize_t len;
			socklen_t addr_len;
		    	addr_len = sizeof(src_addr);
			
			len = recvfrom(sock_fd,
				       buffer, 
				       sizeof(buffer), 
				       0, 
				       (struct sockaddr*)&src_addr, 
				       &addr_len);
			if (len > 0)
			{
                		int dec_len;
			       	dec_len = decrypt_packet(
						encrypted_buffer, 
						len, 
						(unsigned char*)buffer);
				
				if (dec_len > 0) 
				{
					write(tun_fd, buffer, dec_len);
					printf("[IN] Decrypted %ld bytes -> %d bytes to Kernel\n", len, dec_len);
				} else 
				{
					printf("[ERROR] Decryption failed!\n");
				}
			}
		}
	}

    close(tun_fd);
    close(sock_fd);
    return 0;
}
