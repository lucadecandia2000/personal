/**
 * Alice wants to confidentially send Bob the content of a 1MB file through an insecure
 * channel.
 * 
 * Write a program in C, using the OpenSSL library, which Alice can execute to send
 * Bob the file.
 * 
 * Assume that:
 * - Bob's public key is stored into the RSA *bob_pubkey data structure;
 * - The file to send is available in the FILE *file_in data structure;
 * - Alice cannot establish TLS channels or resort to other protocols 
 * - You have access to a high-level communication primitive that sends and receives data
 * and probably format them (e.g., based on a BIO), so that you don't have to think about
 * the communication issues for this exercise
 *
 **/

// since asym is generally slow we want to encrypt using normal aes_128() and then encrypt the key with bob's pub_key so that the operations are both fast and secure ;


#include <stdio.h> 
#include <openssl/rsa.h> 
#include <openssl/err.h>
#include <openssl/rand.h>

#define MAX 16 // bytes 
#define ENCRYPT 1 // more readable like this 
#define DECRYPT 0
#define MAX_ENC_LEN 1000000
#define MAX_BUFFER 1024


void handle_errors() {
    ERR_print_errors_fp(stderr) ; 
    abort() ; 
}

int main() {

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms() ; 


    // assuming the following structures are already filled 
    RSA *bob_pubkey ; 
    FILE *file_in ;

    //encrypt the data with AES (symm)
    // need Key and IV

    unsigned char key[MAX]; // 128 bits 
    unsigned char iv[MAX]; 

     /* Generate key and IV random of 128 bytes */
    if(RAND_load_file("/dev/random", 64) != 64)  //load 64 bytes from dev/random 
        handle_errors();
    
    if(!RAND_bytes(key,MAX)) // key = destination , MAX = size 
        handle_errors();
        
    if(!RAND_bytes(iv,MAX))
        handle_errors();

    // actually do the encryption using AES 

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new() ; // need the context in OpenSSL 

    if (ctx == NULL) {
        handle_errors() ; 
    }

    if(!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, ENCRYPT)) { // init it to use aes_128_cbc() ;   
        handle_errors() ; 
    }

    unsigned char ciphertext[MAX_ENC_LEN]; // create a space for the ciphertext 

    int update_len, final_len; // to be used during the enc
    int ciphertext_len=0; // starting value 
    int n_read; // size actually read in a step
    unsigned char buffer[MAX_BUFFER]; // def of the buffer

    while ((n_read = fread(buffer, 1, MAX_BUFFER, f_in))>0 ){ // file is not actually opened 
        if(ciphertext_len > MAX_ENC_LEN - n_read - EVP_CIPHER_CTX_block_size(ctx)) {
            fprintf(stderr, "File too large") ; 
            abort() ; 
        }
        if(!EVP_CipherUpdate(ctx, ciphertext+ciphertext_len, &update_len, buffer, n_read)) // ctx, where to store, address where to store the size of the updated data, input data, size of data to be processed 
            handle_errors() ; 
        ciphertext_len += update_len ; 
    }
    
    fclose(file_in);

    if(!EVP_CipherFinal_ex(ctx,ciphertext+ciphertext_len,&final_len))
        handle_errors();

    ciphertext_len+=final_len;

    EVP_CIPHER_CTX_free(ctx);

    // encrypt the symm key with BOB pub key 

    int encrypted_data_len;
    unsigned char encrypted_data[RSA_size(bob_pubkey)]; // assuming it is in the struct previously defined 

    if((encrypted_data_len = RSA_public_encrypt(strlen(key), key , encrypted_data, bob_pubkey,  RSA_PKCS1_OAEP_PADDING)) == -1)
    // (int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);
    // please note that the 4th value is a pointer to the struct and not the pubkey itself 
        handle_errors() ;
    
    RSA_free(bob_pubkey) ;

    /* Send message and then the encrypted symm key to Bob and the IV */
    send_bob(ciphertext); // will be able to decrypt after finding the key below
    send_bob(iv); // needed for the symm decription 
    send_bob(encrypted_data); // will be able to decrypt using his private key 

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();


    return 0 ;     
}