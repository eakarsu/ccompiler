int printf(const char *fmt, ...);

/* RSA-style encrypt/decrypt with tiny primes */

/* Modular exponentiation: (base^exp) % mod */
int mod_pow(int base, int exp, int mod) {
    int result;
    int b;
    result = 1;
    b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * b) % mod;
        }
        exp = exp / 2;
        b = (b * b) % mod;
    }
    return result;
}

/* GCD */
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Extended GCD: returns gcd, sets *x and *y such that a*x + b*y = gcd */
int ext_gcd(int a, int b, int *x, int *y) {
    int x1;
    int y1;
    int g;
    int q;
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    g = ext_gcd(b, a % b, &x1, &y1);
    q = a / b;
    *x = y1;
    *y = x1 - q * y1;
    return g;
}

/* Modular inverse: a^(-1) mod m */
int mod_inverse(int a, int m) {
    int x;
    int y;
    int g;
    g = ext_gcd(a, m, &x, &y);
    if (g != 1) return -1; /* no inverse */
    x = x % m;
    if (x < 0) x = x + m;
    return x;
}

/* Check if n is prime (simple trial division) */
int is_prime(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

/* Euler's totient for n = p*q where p,q are prime */
int totient_pq(int p, int q) {
    return (p - 1) * (q - 1);
}

/* RSA key generation result */
struct rsa_key {
    int n;     /* modulus = p * q */
    int e;     /* public exponent */
    int d;     /* private exponent */
    int p;     /* prime factor */
    int q;     /* prime factor */
};

/* Generate RSA keys from two primes */
void rsa_keygen(int p, int q, int e, struct rsa_key *key) {
    int phi;
    key->p = p;
    key->q = q;
    key->n = p * q;
    phi = totient_pq(p, q);
    key->e = e;
    key->d = mod_inverse(e, phi);
}

/* RSA encrypt: c = m^e mod n */
int rsa_encrypt(int msg, struct rsa_key *key) {
    return mod_pow(msg, key->e, key->n);
}

/* RSA decrypt: m = c^d mod n */
int rsa_decrypt(int cipher, struct rsa_key *key) {
    return mod_pow(cipher, key->d, key->n);
}

int main() {
    struct rsa_key key;
    int cipher;
    int plain;
    int i;
    int all_ok;

    /* Test 1: Primality testing */
    printf("prime_2: %d\n", is_prime(2));
    // EXPECT: prime_2: 1
    printf("prime_7: %d\n", is_prime(7));
    // EXPECT: prime_7: 1
    printf("prime_15: %d\n", is_prime(15));
    // EXPECT: prime_15: 0
    printf("prime_23: %d\n", is_prime(23));
    // EXPECT: prime_23: 1

    /* Test 2: GCD */
    printf("gcd_35_15: %d\n", gcd(35, 15));
    // EXPECT: gcd_35_15: 5
    printf("gcd_17_23: %d\n", gcd(17, 23));
    // EXPECT: gcd_17_23: 1

    /* Test 3: Modular inverse */
    /* 3^(-1) mod 7: 3*x = 1 mod 7, x=5 (3*5=15, 15%7=1) */
    printf("inv_3_7: %d\n", mod_inverse(3, 7));
    // EXPECT: inv_3_7: 5

    /* 7^(-1) mod 11: 7*x = 1 mod 11, x=8 (7*8=56, 56%11=1) */
    printf("inv_7_11: %d\n", mod_inverse(7, 11));
    // EXPECT: inv_7_11: 8

    /* Test 4: Totient */
    printf("phi_3_11: %d\n", totient_pq(3, 11));
    // EXPECT: phi_3_11: 20
    /* (3-1)*(11-1) = 2*10 = 20 */

    /* Test 5: RSA key generation with p=3, q=11, e=7 */
    /* n = 33, phi = 20, d = 7^(-1) mod 20 */
    /* 7*3=21, 21%20=1, so d=3 */
    rsa_keygen(3, 11, 7, &key);
    printf("n: %d\n", key.n);
    // EXPECT: n: 33
    printf("e: %d\n", key.e);
    // EXPECT: e: 7
    printf("d: %d\n", key.d);
    // EXPECT: d: 3

    /* Test 6: Encrypt and decrypt a message */
    /* Encrypt m=5: c = 5^7 mod 33 = 78125 mod 33
       78125/33 = 2367, 2367*33 = 78111, 78125-78111 = 14 */
    cipher = rsa_encrypt(5, &key);
    printf("enc_5: %d\n", cipher);
    // EXPECT: enc_5: 14

    /* Decrypt: m = 14^3 mod 33 = 2744 mod 33
       2744/33 = 83, 83*33 = 2739, 2744-2739 = 5 */
    plain = rsa_decrypt(cipher, &key);
    printf("dec_14: %d\n", plain);
    // EXPECT: dec_14: 5

    /* Test 7: Encrypt and decrypt multiple values */
    all_ok = 1;
    i = 1;
    while (i < 33) {
        if (gcd(i, 33) == 1) { /* only works for m coprime to n */
            cipher = rsa_encrypt(i, &key);
            plain = rsa_decrypt(cipher, &key);
            if (plain != i) all_ok = 0;
        }
        i = i + 1;
    }
    printf("all_rt: %d\n", all_ok);
    // EXPECT: all_rt: 1

    /* Test 8: Larger primes: p=5, q=17 */
    /* n=85, phi=(4)(16)=64, e=5 (gcd(5,64)=1) */
    /* d = 5^(-1) mod 64: 5*13=65, 65%64=1, d=13 */
    rsa_keygen(5, 17, 5, &key);
    printf("n2: %d\n", key.n);
    // EXPECT: n2: 85
    printf("d2: %d\n", key.d);
    // EXPECT: d2: 13

    cipher = rsa_encrypt(10, &key);
    /* 10^5 mod 85 = 100000 mod 85 = 100000/85=1176, 1176*85=99960, 100000-99960=40 */
    printf("enc_10: %d\n", cipher);
    // EXPECT: enc_10: 40

    plain = rsa_decrypt(cipher, &key);
    printf("dec_40: %d\n", plain);
    // EXPECT: dec_40: 10

    /* Test 9: p=7, q=13 */
    /* n=91, phi=72, e=5 (gcd(5,72)=1) */
    /* d = 5^(-1) mod 72: 5*29=145, 145%72=1, d=29 */
    rsa_keygen(7, 13, 5, &key);
    printf("n3: %d\n", key.n);
    // EXPECT: n3: 91
    printf("d3: %d\n", key.d);
    // EXPECT: d3: 29

    /* Encrypt m=42: 42^5 mod 91 */
    cipher = rsa_encrypt(42, &key);
    plain = rsa_decrypt(cipher, &key);
    printf("rt_42: %d\n", plain);
    // EXPECT: rt_42: 42

    /* Test 10: Verify e and d are inverses mod phi */
    printf("ed_mod_phi: %d\n", (key.e * key.d) % 72);
    // EXPECT: ed_mod_phi: 1

    /* Modular exponentiation property: (m^e)^d = m^(ed) mod n */
    /* ed = 5*29 = 145, 145 mod 72 = 1, so m^(ed) = m^(1 + k*72) = m * (m^72)^k = m mod n */
    printf("rsa_done\n");
    // EXPECT: rsa_done
    return 0;
}
