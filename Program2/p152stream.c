// p152stream.c by Kyle Nguyen. Submitted for CSC 152 September 21, 2024 - Fall 2024.
// stream cipher made from p152 and mix functions
// Used https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm to learn about memcpy function
// Used https://www.tutorialspoint.com/c_standard_library/c_function_memset.htm to learn about memcpy function

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Static means that it is not exported for linking and so
// can only be called from this file.
static uint32_t rotl32(uint32_t x, int n)
{
    return (x << n) | (x >> (32 - n));
}

// ap - dp are references to four variables to be mixed.
// A good compiler will keep everything in registers, so even
// though it looks like we're reading memory, we probably aren't.
void mix(uint32_t *ap, uint32_t *bp, uint32_t *cp, uint32_t *dp)
{
    // Receive the four words to be mixed
    uint32_t a = *ap;
    uint32_t b = *bp;
    uint32_t c = *cp;
    uint32_t d = *dp;
    // Mix the four words

    // YOUR CODE HERE
    a = a + b;
    d = d ^ a;
    d = rotl32(d, 16);
    c = c + d;
    b = b ^ c;
    b = rotl32(b, 12);
    a = a + b;
    d = d ^ a;
    d = rotl32(d, 8);
    c = c + d;
    b = b ^ c;
    b = rotl32(b, 7);

    // Update the caller's values
    *ap = a;
    *bp = b;
    *cp = c;
    *dp = d;
}
void p152(void *in, void *out)
{

    uint32_t arr[16];    // array of 16 32bit uint32_t,  16x32 = 512 bits / 8 bits = 64 bytes
    memcpy(arr, in, 64); // copy the 64 bytes from in into arr

    for (int i = 0; i < 10; i++)
    { // perform 10 times

        // Calling mix 4x per column

        /*

            Diagonal 1 = 0,5,10,15
                     2 = 1,6,11,12 (wraps around)
                     3 = 2,7,8,13
                     4 = 3,4,9,14

             {0, 1, 2, 3}
             {4, 5, 6, 7}
             {8, 9, 10, 11}
            {12, 13, 14, 15}
              ^  ^    ^   ^
              |  |    |   |
        Column_1 C2   C3  C4
        */

        for (int j = 0; j < 4; j++)
        {
            mix(&arr[j], &arr[j + 4], &arr[j + 8], &arr[j + 12]);
        }

        mix(&arr[0], &arr[5], &arr[10], &arr[15]);
        mix(&arr[1], &arr[6], &arr[11], &arr[12]);
        mix(&arr[2], &arr[7], &arr[8], &arr[13]);
        mix(&arr[3], &arr[4], &arr[9], &arr[14]);
    }

    memcpy(out, arr, 64); // copy the 64 bytes from arr into out
}

// Increment buf as if it's a 64 byte big-endian integer
static void increment_block(void *buf)
{
    unsigned char *blk = buf;
    int i = 63;
    blk[i] += 1;
    while (blk[i] == 0)
    {
        i -= 1;
        blk[i] += 1;
    }
}

// k is 32 bytes, n is 16 bytes, 0* is 16 bytes
// Use blk = k || n || 0* to produce output as blk xor p152(blk), then increment blk
// This output is xor'd with the next 64 bytes of buf to encrypt/decrypt
void p152stream(void *k, void *n, void *inbuf, void *outbuf, int nbytes)
{
    unsigned char *pinbuf = inbuf;
    unsigned char *poutbuf = outbuf;
    unsigned char in[64];
    unsigned char out[64];
    // Init in as k || n || 0*
    memcpy(in, k, 32);
    memcpy(in + 32, n, 16);
    memset(in + 48, 0, 16);

    while (nbytes >= 64)
    {
        p152(in, out);

        // This is the next 64 bytes of stream cipher output
        for (int i = 0; i < 64; i++)
        {
            out[i] = out[i] ^ in[i];
        }

        // Next 64 bytes of inbuf (pinbuf since pointer)
        for (int i = 0; i < 64; i++)
        {
            out[i] = out[i] ^ pinbuf[i];
        }

        // write out to the next 64 bytes of outbuf
        memcpy(poutbuf, out, 64); // dest, src, size

        // Increment in using given function
        increment_block(in);

        // Decrement nbytes by 64, as we processed a block of 64 bytes
        nbytes -= 64;

        // Incrementing by another 64bytes for both pointers
        pinbuf += 64;
        poutbuf += 64;
    }

    if (nbytes > 0)
    {
        p152(in, out);

        // This is the next 64 bytes of stream cipher output
        for (int i = 0; i < 64; i++)
        {
            out[i] = out[i] ^ in[i];
        }

        for (int i = 0; i < nbytes; i++)
        {
            out[i] = out[i] ^ pinbuf[i];
        }
        memcpy(poutbuf, out, nbytes); //  write out to the next nbytes bytes of outbuf
    }
}

#if 1 // Set to 1 while testing and 0 for submission

#include <stdio.h>

int main()
{
    unsigned char inbuf[200] = {1};
    unsigned char outbuf[200];
    unsigned char k[32] = {1, 2, 3, 4};
    unsigned char n[16] = {1, 2, 3, 4};
    p152stream(k, n, inbuf, outbuf, 200);
    printf("Is       : ");
    for (int i = 0; i < 200; i++)
        printf("%02x", outbuf[i]);
    printf("\n");
    printf("Should be: d6c0e5ef8745f9fc4657510ce896e96b9f27c18ba5a8cadc7e2a"
           "6872c51c704983726c6633a9e924a5e9a75b8b9980cfad91501f74315fea6da"
           "0936286e5866ac66e8c3d766b6248f88ee99b468dd9fdcf2c4e65e6df35637b"
           "b245246e0cb97ce689c0b91dbd7212257f98744fae42484ea3afbd419db90ca"
           "38a96d4c6e68cd6c003af8b842733ad4162099b2b2d10bfd48a3fb6e8c5e5ea"
           "59dde8bae3206ce3e80f0acad1540e83e2858f39bccec0a4ece5172194f6d15"
           "e7fd5a26a05cb3b8b8fea979965daf5c1\n");
}

#endif