// p152.c by Kyle Nguyen. Submitted for CSC 152 September 21, 2024 - Fall 2024.
// Copies the bytes and uses mixing algorithm
// Used https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm to learn about memcpy function

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

#if 0 // Set to 1 while testing and 0 for submission

#include <stdio.h>

int main()
{
    unsigned char buf[64] = {1}; // Puts 1 in frist byte, rest get auto zeroed
    p152(buf, buf);
    // As a test, output the first 16 bytes of the output
    printf("Is       : ");
    for (int i = 0; i < 16; i++)
        printf("%02x", buf[i]);
    printf("\n");
    printf("Should be: 14627e9771052d97a8a025cc5531572f\n");
}

#endif