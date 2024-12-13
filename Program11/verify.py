# verify.py by Kyle Nguyen (kyletnguye@csus.edu) for CSC 152 Fall 2024
# verifies the Elagamal signature

"""
Write a command-line program that takes arguments p, g, gd, x, r, and s and outputs 1 if (r,s) is a valid signature for x and 0 otherwise (all expressed as decimal integers). Here's an example that follows the example in the reading.

verify 101 2 14 5 59 35
1

"""
import sys
from math import gcd
    
def verify(p, g, gd, x, r, s):
    p = int(p)
    g = int(g)
    gd = int(gd)
    x = int(x)
    r = int(r)
    s = int(s)

    # Verify that (g^d)^r x r^s mod p = g^x mod p

    # Calculate the right-hand side: g^x mod p
    right = pow(g, x) % p

    # 2. Calculate the left-hand side: ((g^d)^r * r^s) mod p
    left = (pow(gd, r) % p * pow(r, s, p)) % p

    # Check if both sides are equal
    if left == right:
        return 1
    else:
        return 0
    
if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("Usage: verify <p> <g> <gd> <x> <r> <s>")
        sys.exit(1)

    p, g, gd, x, r, s = sys.argv[1:7]
    result = verify(p, g, gd, x, r, s)
    print(result)