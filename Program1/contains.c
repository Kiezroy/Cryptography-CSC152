// contains.c by Kyle Nguyen. Submitted for CSC 152 September 4, 2024 - Fall 2024.
// Implements contains function to check if b buffer exists in a buffer

#include <stdio.h>
#include <stdlib.h>

int is_equal(unsigned char *a_startPointer, unsigned char *bBuffer, int blen);
int contains(void *a, int alen, void *b, int blen);

/*// Delete main when submitting
int main()
{
    return 0;
}
*/

int contains(void *a, int alen, void *b, int blen)
{
    // declare unsigned char pointers a and b, then cast them from the void a and b
    unsigned char *p_a = (unsigned char *)a;
    unsigned char *p_b = (unsigned char *)b;

    if (blen == 0) // if b is empty, it is still a subset of a, return true
    {
        return 1;
    }
    if (blen > alen) // if b's length is > a, it cannot be a subset of a, return false
    {
        return 0;
    }

    for (int i = 0; i <= alen - blen; i++) // loop through to compare a and b buffers
    {
        // could also do &p_a[i] instead of p_a + i
        // we do p_a + i so the pointer starts at the next index each loop, resetting the search
        if (is_equal(p_a + i, p_b, blen))
        {
            return 1;
        }
    }
    return 0;
}

// function created to compare the a and b buffers, treated as an array of unsigned char
int is_equal(unsigned char *a_startPointer, unsigned char *bBuffer, int blen)
{

    for (int i = 0; i < blen; i++)
    {
        if (a_startPointer[i] != bBuffer[i]) // compares the values in a and b at each index, if mismatch, return false
        {
            return 0;
        }
    }

    return 1;
}
