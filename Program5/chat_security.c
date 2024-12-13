//chat_security.c by Kyle Nguyen. Submitted for CSC 152 October 6, 2024
//encryption and decryption with AES256-CTR and OpenSSL

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

void chat_encrypt(void *k, void *pt, int ptlen, void *ct){
    int len;
    int ctlen;

    //cast to unsigned chars to work with bytes
    unsigned char *key = (unsigned char *)k;
    unsigned char *ciphertext = (unsigned char *)ct;

    unsigned char iv[16];
    // Write the nonce to the iv which is 12 bytes long of random characters
    RAND_bytes(iv, 12);

    // Set the remaining 4 bytes to 0
    memset(iv + 12, 0, 4);

    // Copy the IV to the beginning of the ciphertext
    memcpy(ciphertext, iv, 12);

    //Encryption process
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext + 12, &len, pt, ptlen);
    ctlen = len;

    EVP_EncryptFinal_ex(ctx, ciphertext + 12 + len, &len);
    ctlen = ctlen + len;
    EVP_CIPHER_CTX_free(ctx);
}

void chat_decrypt(void *k, void *ct, int ctlen, void *pt){
    int len;
    int ptlen;

    // Cast to unsigned chars to work with bytes
    unsigned char *key = (unsigned char *)k;
    unsigned char *ciphertext = (unsigned char *)ct;
    unsigned char *plaintext = (unsigned char *)pt;

    // Extract the IV (first 12 bytes of the ciphertext)
    unsigned char iv[16];
    memcpy(iv, ciphertext, 12);
    memset(iv + 12, 0, 4);  // Set remaining 4 bytes to 0

    // Decryption process
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext + 12, ctlen - 12);
    ptlen = len;

    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    ptlen += len;

    EVP_CIPHER_CTX_free(ctx);
}