/*
implement, using the C programming language, the following function:
int envelop_MAC(RSA *rsa_keypair, char *message, int message_len, char *key, int keylenght, char *result);
1) double_SHA256 of the concat of a message with a symmetric key;
2) RSA encrypt the result of the last step
3) returns 0 in case of success, 1 if errors, and the res of the RSA enc by reference.
*/
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define MAXBUF 1024

int envelop_MAC(RSA *rsa_keypair, char *message, int message_len, char *key, int keylenght, char *result) {

/* Load the human readable error strings for libcrypto */
ERR_load_crypto_strings();
/* Load all digest and cipher algorithms */
OpenSSL_add_all_algorithms();

unsigned char double_hash[SHA256_DIGEST_LENGHT] ; 
strcat(message, key);
EVP_MD_CTX *md;

md = EVP_MD_CTX_new();

if(!EVP_DigestInit(md, EVP_sha256())){
    return 1;
}    

if(EVP_DigestUpdate(md, message, strlen(message))) {
    return 1 ; 
}

unsigned char md_value[20];
int md_len;

if(!EVP_DigestFinal_ex(md, md_value, &md_len)){
    return 1;
}

if(EVP_DigestUpdate(md, &md_value, strlen(message))) { // to check 
    return 1 ; 
}

if(!EVP_DigestFinal_ex(md, md_value, &md_len)){
    return 1;
}

int encrypted_data_len;
unsigned char encrypted_data[RSA_size(keypair)];


if((encrypted_data_len = RSA_public_encrypt(strlen(md_value)+1, md_value, result, key, RSA_PKCS1_OAEP_PADDING)) == -1) 
    return 1;

return 0; 

}

// to be checked 