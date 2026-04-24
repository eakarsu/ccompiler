int printf(const char *fmt, ...);

/* Diffie-Hellman key exchange simulation with small primes */

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

/* Check if g is a primitive root modulo p (for small p) */
int is_primitive_root(int g, int p) {
    int seen[64]; /* p must be < 64 for this */
    int i;
    int val;

    i = 0;
    while (i < p) {
        seen[i] = 0;
        i = i + 1;
    }

    i = 1;
    while (i < p) {
        val = mod_pow(g, i, p);
        if (val < p) {
            seen[val] = 1;
        }
        i = i + 1;
    }

    /* Check all values 1..p-1 are generated */
    i = 1;
    while (i < p) {
        if (!seen[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Find the order of g modulo p */
int find_order(int g, int p) {
    int val;
    int order;
    val = g % p;
    order = 1;
    while (val != 1 && order < p) {
        val = (val * g) % p;
        order = order + 1;
    }
    return order;
}

/* GCD using Euclidean algorithm */
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    int p;
    int g;
    int alice_priv;
    int bob_priv;
    int alice_pub;
    int bob_pub;
    int alice_shared;
    int bob_shared;
    int eve_priv;
    int eve_pub;

    /* Test 1: Modular exponentiation basics */
    printf("pow_2_10_1000: %d\n", mod_pow(2, 10, 1000));
    /* 2^10 = 1024, 1024%1000 = 24 */
    // EXPECT: pow_2_10_1000: 24

    printf("pow_3_5_7: %d\n", mod_pow(3, 5, 7));
    /* 3^5 = 243, 243%7 = 243 - 34*7 = 243-238 = 5 */
    // EXPECT: pow_3_5_7: 5

    printf("pow_5_0_13: %d\n", mod_pow(5, 0, 13));
    /* x^0 = 1 */
    // EXPECT: pow_5_0_13: 1

    printf("pow_7_1_11: %d\n", mod_pow(7, 1, 11));
    /* 7^1 % 11 = 7 */
    // EXPECT: pow_7_1_11: 7

    /* Test 2: GCD */
    printf("gcd_12_8: %d\n", gcd(12, 8));
    // EXPECT: gcd_12_8: 4
    printf("gcd_17_13: %d\n", gcd(17, 13));
    // EXPECT: gcd_17_13: 1
    printf("gcd_100_75: %d\n", gcd(100, 75));
    // EXPECT: gcd_100_75: 25

    /* Test 3: Primitive root check for p=7, g=3 */
    /* 3^1=3, 3^2=2, 3^3=6, 3^4=4, 3^5=5, 3^6=1 (all mod 7) -> primitive root */
    printf("prim_3_7: %d\n", is_primitive_root(3, 7));
    // EXPECT: prim_3_7: 1

    /* 2 is not a primitive root of 7: 2^1=2, 2^2=4, 2^3=1 -> only generates {1,2,4} */
    printf("prim_2_7: %d\n", is_primitive_root(2, 7));
    // EXPECT: prim_2_7: 0

    /* Test 4: Order of elements */
    printf("order_3_7: %d\n", find_order(3, 7));
    // EXPECT: order_3_7: 6
    printf("order_2_7: %d\n", find_order(2, 7));
    // EXPECT: order_2_7: 3

    /* Test 5: Diffie-Hellman key exchange with p=23, g=5 */
    p = 23;
    g = 5;

    /* 5 is a primitive root mod 23 */
    printf("prim_5_23: %d\n", is_primitive_root(5, 23));
    // EXPECT: prim_5_23: 1

    /* Alice picks private key 6, Bob picks 15 */
    alice_priv = 6;
    bob_priv = 15;

    /* Public keys: g^priv mod p */
    alice_pub = mod_pow(g, alice_priv, p);
    /* 5^6 mod 23 = 15625 mod 23. 15625/23=679.xx, 679*23=15617, 15625-15617=8 */
    printf("alice_pub: %d\n", alice_pub);
    // EXPECT: alice_pub: 8

    bob_pub = mod_pow(g, bob_priv, p);
    /* 5^15 mod 23. Let's compute using repeated squaring:
       5^1=5, 5^2=25%23=2, 5^4=(2*2)%23=4, 5^8=(4*4)%23=16
       5^15 = 5^8 * 5^4 * 5^2 * 5^1 = 16*4*2*5 = 640, 640%23 = 640-27*23=640-621=19 */
    printf("bob_pub: %d\n", bob_pub);
    // EXPECT: bob_pub: 19

    /* Shared secret: other's_pub^my_priv mod p */
    alice_shared = mod_pow(bob_pub, alice_priv, p);
    /* 19^6 mod 23. 19^2=361%23=361-15*23=361-345=16
       19^4 = 16^2%23 = 256%23 = 256-11*23=256-253=3
       19^6 = 19^4 * 19^2 = 3*16 = 48%23 = 2 */
    printf("alice_shared: %d\n", alice_shared);
    // EXPECT: alice_shared: 2

    bob_shared = mod_pow(alice_pub, bob_priv, p);
    /* 8^15 mod 23. 8^1=8, 8^2=64%23=64-2*23=18, 8^4=18^2%23=324%23=324-14*23=324-322=2
       8^8=2^2%23=4, 8^15=8^8*8^4*8^2*8^1=4*2*18*8=1152, 1152%23=1152-50*23=1152-1150=2 */
    printf("bob_shared: %d\n", bob_shared);
    // EXPECT: bob_shared: 2

    /* Test 6: Both shared secrets must be equal */
    printf("shared_eq: %d\n", alice_shared == bob_shared ? 1 : 0);
    // EXPECT: shared_eq: 1

    /* Test 7: Different private keys */
    alice_priv = 3;
    bob_priv = 10;
    alice_pub = mod_pow(g, alice_priv, p);
    /* 5^3 mod 23 = 125%23 = 125-5*23=125-115=10 */
    printf("alice_pub2: %d\n", alice_pub);
    // EXPECT: alice_pub2: 10

    bob_pub = mod_pow(g, bob_priv, p);
    /* 5^10 mod 23. 5^1=5, 5^2=2, 5^4=4, 5^8=16, 5^10=5^8*5^2=16*2=32%23=9 */
    printf("bob_pub2: %d\n", bob_pub);
    // EXPECT: bob_pub2: 9

    alice_shared = mod_pow(bob_pub, alice_priv, p);
    /* 9^3 mod 23 = 729%23 = 729-31*23=729-713=16 */
    printf("shared2_a: %d\n", alice_shared);
    // EXPECT: shared2_a: 16

    bob_shared = mod_pow(alice_pub, bob_priv, p);
    /* 10^10 mod 23. 10^1=10, 10^2=100%23=100-4*23=8, 10^4=8^2=64%23=18
       10^8=18^2%23=324%23=2, 10^10=10^8*10^2=2*8=16 */
    printf("shared2_b: %d\n", bob_shared);
    // EXPECT: shared2_b: 16

    printf("shared2_eq: %d\n", alice_shared == bob_shared ? 1 : 0);
    // EXPECT: shared2_eq: 1

    /* Test 8: Eavesdropper can't compute shared secret without private key */
    /* Eve sees alice_pub=10, bob_pub=9, p=23, g=5 */
    /* Eve tries a random private key 7 */
    eve_priv = 7;
    eve_pub = mod_pow(g, eve_priv, p);
    /* 5^7 mod 23 = 5^4*5^2*5^1 = 4*2*5 = 40%23 = 17 */
    printf("eve_pub: %d\n", eve_pub);
    // EXPECT: eve_pub: 17

    /* Eve's "shared" with Alice using Bob's public key */
    printf("eve_diff: %d\n", mod_pow(bob_pub, eve_priv, p) != alice_shared ? 1 : 0);
    // EXPECT: eve_diff: 1

    /* Test 9: Fermat's little theorem: a^(p-1) = 1 mod p for prime p */
    printf("fermat_5: %d\n", mod_pow(5, 22, 23));
    // EXPECT: fermat_5: 1
    printf("fermat_7: %d\n", mod_pow(7, 22, 23));
    // EXPECT: fermat_7: 1

    /* Test 10: Order divides p-1 */
    printf("order_5_23: %d\n", find_order(5, 23));
    // EXPECT: order_5_23: 22
    /* p-1=22 is divisible by 22 -> 5 is a primitive root */

    printf("dh_done\n");
    // EXPECT: dh_done
    return 0;
}
