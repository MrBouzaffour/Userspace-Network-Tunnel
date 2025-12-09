/* Author: Ahmed Bouzaffour 2025-12-08
 * Encryption
 */
#include "common.h"
#include <sodium.h>


unsigned char key[crypto_secretbox_KEYBYTES];

void 
crypto_init()
{
	if (sodium_init() == -1)
	{
		fprintf(stderr, "[ERROR] Libsodium init failed!\n");
		exit(1);
	}
	/* generate a random key*/
	crypto_secretbox_keygen(key);
	printf("[CRYPTO] Libsodium initialized\n");
}

int
encrypt_packet(unsigned char *plain, int len, unsigned char *cipher) 
{
	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	
	/* Generate random nonce*/
	randombytes_buf(nonce, sizeof(nonce));
	
	/* Encryption payload goes to cipher + 24 bytes*/
	int ret;
       	ret = crypto_secretbox_easy(
			cipher + sizeof(nonce), 
			plain, 
			len, 
			nonce, 
			key);

	/*Encryption failed*/
	if (ret != 0) 
		return -1;
	
	/* Prepend */
	memcpy(cipher, nonce, sizeof(nonce));
	return len + sizeof(nonce) + crypto_secretbox_MACBYTES;
}

int
decrypt_packet(unsigned char *cipher, int len, unsigned char *plain)
{
	unsigned char nonce[crypto_secretbox_NONCEBYTES];
    
	if (len < (int)(sizeof(nonce) + crypto_secretbox_MACBYTES))
		return -1;
	
	/*read the Nonce from the start of the packet */
	memcpy(nonce, cipher, sizeof(nonce));
	
	/*Decrypt using that nonce*/
	int ret;
       	ret = crypto_secretbox_open_easy(
			plain, 
			cipher + sizeof(nonce), 
			len - sizeof(nonce), 
			nonce, 
			key);
	/*Decryption failed*/
	if (ret != 0)
		return -1;
	
	return len - sizeof(nonce) - crypto_secretbox_MACBYTES;
}
