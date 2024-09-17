#include <stdio.h>

int any_dup_ints(void *buf, int buflen)
{
    int *buffer = (int *)buf; // convert buffer to int

    // loop through and see if any int values are equal
    for (int i = 0; i < buflen / sizeof(int); i++)
    {
        for (int j = 0; j < buflen / sizeof(int); j++)
        {
            int firstVal = buffer[i];
            int secondVal = buffer[j];
            if (firstVal == secondVal && i != j)
            {
                return 1;
            }
        }
    }
    return 0;
}

int main()
{

    // Test case 1: No duplicates
    int buf1[] = {1, 2, 3, 4, 5}; // No duplicates
    int buflen1 = sizeof(buf1);
    int result1 = any_dup_ints((void *)buf1, buflen1);
    printf("Test 1 result: %d (expected 0)\n", result1); // Add print statement
    // Should return 0 (no duplicates)

    // Test case 2: With duplicates
    int buf2[] = {1, 2, 3, 2, 5}; // Contains a duplicate '2'
    int buflen2 = sizeof(buf2);
    int result2 = any_dup_ints((void *)buf2, buflen2);
    printf("Test 2 result: %d (expected 1)\n", result2); // Add print statement
    // Should return 1 (duplicates found)

    // Test case 3: Empty buffer
    int buf3[] = {}; // Empty buffer
    int buflen3 = sizeof(buf3);
    int result3 = any_dup_ints((void *)buf3, buflen3);
    printf("Test 3 result: %d (expected 0)\n", result3); // Add print statement
    // Should return 0 (no duplicates)

    // Test case 4: One element (no duplicates possible)
    int buf4[] = {42}; // One element
    int buflen4 = sizeof(buf4);
    int result4 = any_dup_ints((void *)buf4, buflen4);
    printf("Test 4 result: %d (expected 0)\n", result4); // Add print statement
    // Should return 0 (no duplicates)

    printf("All test cases passed!\n");

    return 0;
}