#ifndef QEMU_AES_H
#define QEMU_AES_H

#include <stdint.h>

#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

#if !defined (ALIGN16)
# if defined (__GNUC__)
#  define ALIGN16  __attribute__  ( (aligned (16)))
# else
#  define ALIGN16 __declspec (align (16))
# endif
#endif

/*struct aes_key_st {
    uint32_t rd_key[4 *(AES_MAXNR + 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY_SW;*/

typedef struct KEY_SCHEDULE{
    ALIGN16 unsigned char KEY[16*15];
    unsigned int nr;
}AES_KEY;

extern void AES_CBC_encrypt (const unsigned char *in,
                 	 unsigned char *out,
					 unsigned char ivec[16],
					 unsigned long length,
                 	 const unsigned char *KS,
					 int nr);

extern void AES_CBC_decrypt (const unsigned char *in,
							unsigned char *out,
							unsigned char ivec[16],
							unsigned long length,
							const unsigned char *KS,
							int nr);

extern int AES_set_encrypt_key (const unsigned char *userKey,
        						const int bits,
								AES_KEY *key);

extern int AES_set_decrypt_key (const unsigned char *userKey,
                        const int bits,
                        AES_KEY *key);

int AES_set_encrypt_key_sw(const unsigned char *userKey, const int bits,
	AES_KEY *key);
int AES_set_decrypt_key_sw(const unsigned char *userKey, const int bits,
	AES_KEY *key);

void AES_encrypt(const unsigned char *in, unsigned char *out,
	const AES_KEY *key);
void AES_decrypt(const unsigned char *in, unsigned char *out,
	const AES_KEY *key);
void AES_cbc_encrypt_sw(const unsigned char *in, unsigned char *out,
		     const unsigned long length, const AES_KEY *key,
		     unsigned char *ivec, const int enc);

#endif
