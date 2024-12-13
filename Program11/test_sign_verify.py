import random
import sympy

import sign  # Import the entire sign module
import verify  # Import the entire verify module

prime_range = (100, 10_000)
tests_per_prime = 120

def main():
    passed = 0
    failed = 0
    random.seed()

    for p in sympy.primerange(prime_range[0], prime_range[1]):
        for _ in range(tests_per_prime):
            g = find_generator(p)
            d = random.randrange(1, p)
            gd = pow(g, d, p)
            x = random.randrange(0, p)

            # Corrected call to sign function
            r, s = sign.sign(p, g, gd, d, x)  # Use sign.sign instead of sign()

            # Corrected call to verify_signature function
            matches = verify.verify(p, g, gd, x, r, s)  # Use verify_signature instead of verify

            if matches:
                passed += 1
            else:
                failed += 1
    
    tests = passed + failed
    percentage = passed / tests

    print(f'{passed}/{tests} ({(percentage * 100):.2f}%)')


def find_generator(p):
    if not sympy.isprime(p):
        raise ValueError(f"{p} is not prime.")

    # Calculate p-1 and find its prime factors
    phi = p - 1
    factors = sympy.factorint(phi)  # Get prime factors of p - 1

    # Try each element from 2 to p-1 as a candidate for generator
    for g in range(2, p):
        
        # Check if g^((p-1)/q) % p != 1 for each prime factor q of p-1
        if all(pow(g, phi // q, p) != 1 for q in factors):
            return g  # g is a generator

    raise ValueError("No generator found.")


if __name__ == '__main__':
    main()
