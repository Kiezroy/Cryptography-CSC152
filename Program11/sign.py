# sign.py by Kyle Nguyen (kyletnguye@csus.edu) for CSC 152 Fall 2024
# generates an Elgamal signature
# used https://stackoverflow.com/questions/11175131/code-for-greatest-common-divisor-in-python to implement GCD
# used https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python to implement inverse mod and egcd

"""
Write a command-line program that takes arguments p, g, gd, d, and x and outputs r and s (all expressed as decimal integers). 
Here's an example that follows the example in the reading.

sign 101 2 14 10 5
59
35

"""

import random
import sys
from math import gcd

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def sign(p, g, gd, d, x):
    p = int(p)
    g = int(g)
    gd = int(gd)
    d = int(d)
    x = int(x)
    
    # 1st choose rand 0 < e < p - 1 such that gcd(e, p-1) = 1
    while True:
        e = random.randint(1, p - 1)
        if gcd(e, p - 1) == 1:
            break

    # 2nd do r = g^e mod p
    r = pow(g, e) % p

    # 3rd do s = (x-dr)e^-1 mod p - 1
    e_inv = modinv(e, p - 1)
    s = (e_inv * (x - d * r)) % (p - 1)

    return r, s

if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Usage: sign <p> <g> <gd> <d> <x>")
        sys.exit(1)

    p, g, gd, d, x = sys.argv[1:6]
    r, s = sign(p, g, gd, d, x)
    print(r)
    print(s)
