// The specification of the NONCENSE protocol includes the following operations:
//
// 1) Generate a random 256-bit number, name it r1
// 2) Generate a random 256-bit number, name it r2
// 3) Obtain a key by XOR-ing the two random numbers r1 and r2, name it key_symm
// 4) Generate an RSA keypair of at least 2048 bit modulus
// 5) Encrypt the generated RSA keypair using AES-256 with key_symm and obtain
// 	  the payload.
// Implement in C the protocol steps described above, make the proper decision when
// the protocol omits information.

#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

#define BITS 256

void handle_errors() {
    ERR_print_errors_fp(stderr) ; 
    abort() ; 

}

int main() {

    unsigned char r1[BITS/8], r2[BITS/8], key_symm[BITS/8];

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    if(RAND_load_file("/dev/random", 64) != 64) // used as seed for gen of random num 
        handle_errors();

    if(!RAND_bytes(r1,BITS/8)) // fill r1 with random bytes
        handle_errors();
	
	if(!RAND_bytes(r2,BITS/8)) // same as r1 
        handle_errors();

    for (int i = 0; i<BITS/8 ; i++) {
		key_symm[i] = r1[i]^r2[i]; // xor the 2 byte per byte 
	}

    RSA *rsa_keypair = RSA_new(); // create a pointer to a RSA struct  
    BIGNUM *bne = BN_new(); // create a pointer to a BIGNUM  struct

    if(!BN_set_word(bne,RSA_F4))
        handle_errors();
	
	if(!RSA_generate_key_ex(rsa_keypair, 2048 , bne, NULL)) // generate a RSA keypair (struct , bits, bnumber (set_word), callback)
        handle_errors();
	// The PEM_write_RSAPrivateKey function is part of the OpenSSL library and is used to write an RSA private key in PEM (Privacy-Enhanced Mail) format to a file or a buffer. 
	if(!PEM_write_RSAPrivateKey(stdout, rsa_keypair, EVP_aes_256_cbc(), key_symm, strlen(key_symm), NULL, NULL))
        handle_errors();

    RSA_free(rsa_keypair);
    BN_free(bne);
	
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    return 0 ; 

}