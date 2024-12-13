#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

// Write 12+ptlen bytes to ct (ciphertext preceded by 12 byte random nonce)
void chat_encrypt(void *k, void *pt, int ptlen, void *ct);

// Write ctlen-12 bytes to pt (using the first 12 bytes of ct as the nonce)
void chat_decrypt(void *k, void *ct, int ctlen, void *pt);

int main() {
    // Define a 256-bit key (32 bytes)
    unsigned char key[32];
    RAND_bytes(key, sizeof(key));


    // Define some plaintext (example message)
    unsigned char plaintext[] = "Hello, this is a test message!";
    int ptlen = strlen((char *)plaintext);

    // Allocate space for ciphertext (plaintext length + 12 bytes for the nonce)
    unsigned char ciphertext[ptlen + 12];

    // Allocate space for decrypted plaintext (+1 for null terminator)
    unsigned char decrypted[ptlen + 1];

    // Encrypt the plaintext
    chat_encrypt(key, plaintext, ptlen, ciphertext);
    printf("Original Plaintext: %s\n", plaintext);
    printf("Ciphertext in hex: ");
    for (int i = 0; i < ptlen + 12; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Decrypt the ciphertext
    chat_decrypt(key, ciphertext, ptlen + 12, decrypted);

    // Null-terminate the decrypted buffer
    decrypted[ptlen] = '\0';

    // Print the decrypted plaintext
    printf("Decrypted Ciphertext: %s\n", decrypted);

    return 0;
}