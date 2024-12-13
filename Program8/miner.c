//miner.c by Kyle Nguyen (kyletnguyen@csus.edu) for CSC 152 Fall 2024 October 27, 2024
//OpenSSL Program that will take a n integer and output both a plaintext and its hash beginning with n or more 0 bits
//Used OpenSSL's EVP Message Digest page to construct a hash https://wiki.openssl.org/index.php/EVP_Message_Digests
//Refered to StackOveflow about Null termination https://stackoverflow.com/questions/8202897/null-terminated-string-in-c


//NOTE: Once input is >= 32 bits (4 bytes), program runs for extremely long to compute hash


/*
    Example:

    > miner 4
    abcdefghijklmn
    0653c7e992d7aad40cb2635738b870e4c154afb346340d02c797d490dd52d5f9

    Since in the hash outputs in hex, and we have a 4, the first 0 represents 4 bits which matches our input.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

//Handling errors and incorrect inputs
void handleErrors(void) {
    fprintf(stderr, "An error occurred.\n");
    exit(EXIT_FAILURE);  // Exit the program with a failure status
}

//Given function to print memory as a sequence of hex
void pbuf(void *buf, int len)
{
    unsigned char *p = (unsigned char *)buf;
    for (int i = 0; i < len; i++)
        printf("%02x", p[i]);
    printf("\n");
}

//Constructing a hash using SHA256
void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len) {
                            //digest is a pointer to a pointer where the memory for the digest (hash) is allocated into
    EVP_MD_CTX *mdctx;

    if ((mdctx = EVP_MD_CTX_new()) == NULL)     //Allocates and initializes a new EVP_MD_CTX structure
        handleErrors();

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))      //Tells the digest context to use a specific hash function
        handleErrors();

    if (1 != EVP_DigestUpdate(mdctx, message, message_len))     //Used to feed the messaged into the hash algorithm
        handleErrors();

    if ((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)     //Allocates memory to store the hash result
        handleErrors();

    if (1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))        //Finalizes the hash and stores it into memory in the digest pointer
        handleErrors();

    EVP_MD_CTX_free(mdctx);
}

//Function to check if input == n or more 0 bits in hash
int has_n_leading_zero_bits(const unsigned char *hash, int n) {
    int byte_count = n / 8;     //Checks amount of bytes (1 byte = 8 bits)
    int remaining_bits = n % 8; //Checks remaining bits left

    //Check leading zero bytes (skips if n == 4)
    for (int i = 0; i < byte_count; i++) {
        if (hash[i] != 0) {
            return 0;
        }
    }

    if (remaining_bits == 4) {
        if ((hash[byte_count] & 0xF0) != 0) { //Check remaning bits (4 bits) is zero with masking in hex (F0 is 1111 0000)
            return 0; 
        }
    }

    return 1; //n leading bits are zero
}


//Function to increment ASCII sequence a-z, A-Z, 0-9, max 60 chars
void increment_ascii_sequence(unsigned char *str, int max_len) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";   //Create array containing all ASCII characters
    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < max_len; i++) {
        char *pos = strchr(charset, str[i]); //Find current character in charset
        if (pos == NULL) {
            str[i] = charset[0]; //Start with the first character in the set
            continue;
        }
        
        int index = pos - charset;
        if (index < charset_size - 1) {
            str[i] = charset[index + 1]; //Move to the next character in the charset
            return;
        } else {
            str[i] = charset[0]; //Reset current position and carry to next position
        }
    }

    //Appending a new character to the end if length isn't enough
    str[max_len - 1] = charset[0];
    str[max_len] = '\0';
}




int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <positive integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);  //Converts the cli input into an integer
    
    //Ensures user enters a positive number
    if (n <= 0) {
        printf("Error: Please enter a positive integer.\n"); 
        return EXIT_FAILURE;  
    }

    //Ensure user enters a number that is a multiple of 4
    if(n % 4 != 0){
        printf("Error: Please enter a multiple of 4.\n");
        return EXIT_FAILURE;
    }

    //printf("You entered: %d\n", n);   TESTING input here

    int max_len = 60;                  //Max ASCII sequence
    unsigned char str[61] = {'a'};      //Allocate enough space for max length + null terminator
    memset(str, 'a', 30);               //Start with 30 'a' characters (refer to Piazza question)
    str[30] = '\0';                     //Null-terminate the string

    unsigned char *digest = NULL;       //Pointer for the hashed digest
    unsigned int digest_len = 0;

    //Create a loop that will hash the ASCII sequence using the digest_message function until input == n or more 0 leading bits of hash
    while (1) {
        //Print the current string being hashed
        //printf("Trying string: %s\n", str);

        //Hash the current sequence
        digest_message(str, strlen((char *)str), &digest, &digest_len);

        //Check if hash meets input criteria
        if (has_n_leading_zero_bits(digest, n)) {
            //printf("Found ASCII sequence: %s\n", str);
            printf("%s\n", str);
            //printf("Hash in hex: ");
            pbuf(digest, digest_len);
            OPENSSL_free(digest);
            break;                  //If yes then exit loop
        }

        //If no, redo using next ASCII sequence
        increment_ascii_sequence(str, max_len);
        OPENSSL_free(digest); //Free memory for the next iteration
    }

    return 0;
}
