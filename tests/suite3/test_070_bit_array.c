int printf(const char *fmt, ...);

/* Bit array simulation using int array:
   set/get/clear bit N, count set bits in range, find first set bit */

#define BITS_PER_WORD 32
#define ARRAY_WORDS   8    /* 8 * 32 = 256 bits total */

/* Set bit n in the bit array */
void bit_set(unsigned int *arr, int n) {
    int word = n / BITS_PER_WORD;
    int bit  = n % BITS_PER_WORD;
    arr[word] = arr[word] | (1u << bit);
}

/* Clear bit n in the bit array */
void bit_clear(unsigned int *arr, int n) {
    int word = n / BITS_PER_WORD;
    int bit  = n % BITS_PER_WORD;
    arr[word] = arr[word] & ~(1u << bit);
}

/* Toggle bit n */
void bit_toggle(unsigned int *arr, int n) {
    int word = n / BITS_PER_WORD;
    int bit  = n % BITS_PER_WORD;
    arr[word] = arr[word] ^ (1u << bit);
}

/* Get bit n: returns 0 or 1 */
int bit_get(const unsigned int *arr, int n) {
    int word = n / BITS_PER_WORD;
    int bit  = n % BITS_PER_WORD;
    return (arr[word] >> bit) & 1;
}

/* Count set bits in range [lo, hi] inclusive */
int bit_count_range(const unsigned int *arr, int lo, int hi) {
    int count = 0;
    int i;
    for (i = lo; i <= hi; i++) {
        count += bit_get(arr, i);
    }
    return count;
}

/* Find position of first set bit starting from position start.
   Returns -1 if none found up to max_bit. */
int bit_find_first(const unsigned int *arr, int start, int max_bit) {
    int i;
    for (i = start; i <= max_bit; i++) {
        if (bit_get(arr, i)) return i;
    }
    return -1;
}

/* Count all set bits using popcount word-by-word */
int bit_count_all(const unsigned int *arr, int nwords) {
    int total = 0;
    int i;
    for (i = 0; i < nwords; i++) {
        unsigned int w = arr[i];
        while (w) { w = w & (w - 1); total++; }
    }
    return total;
}

/* Clear entire bit array */
void bit_clear_all(unsigned int *arr, int nwords) {
    int i;
    for (i = 0; i < nwords; i++) arr[i] = 0u;
}

int main(void) {
    unsigned int bits[ARRAY_WORDS];
    int i;

    /* Initialize to zero */
    bit_clear_all(bits, ARRAY_WORDS);

    /* --- BASIC SET AND GET --- */
    printf("%d\n", bit_get(bits, 0));            // EXPECT: 0
    printf("%d\n", bit_get(bits, 31));           // EXPECT: 0
    printf("%d\n", bit_get(bits, 127));          // EXPECT: 0

    bit_set(bits, 0);
    printf("%d\n", bit_get(bits, 0));            // EXPECT: 1
    printf("%d\n", bit_get(bits, 1));            // EXPECT: 0

    bit_set(bits, 31);
    printf("%d\n", bit_get(bits, 31));           // EXPECT: 1
    printf("%u\n", bits[0]);                     // EXPECT: 2147483649
    /* bit 0 and bit 31: 1 + 2^31 = 1 + 2147483648 = 2147483649 */

    bit_set(bits, 32);   /* first bit of second word */
    printf("%d\n", bit_get(bits, 32));           // EXPECT: 1
    printf("%d\n", bits[1]);                     // EXPECT: 1

    /* --- CLEAR --- */
    bit_clear(bits, 0);
    printf("%d\n", bit_get(bits, 0));            // EXPECT: 0
    printf("%u\n", bits[0]);                     // EXPECT: 2147483648
    /* only bit 31 remains in word 0 */

    bit_clear(bits, 31);
    printf("%d\n", bits[0]);                     // EXPECT: 0

    /* --- TOGGLE --- */
    bit_clear_all(bits, ARRAY_WORDS);
    bit_toggle(bits, 5);
    printf("%d\n", bit_get(bits, 5));            // EXPECT: 1
    bit_toggle(bits, 5);
    printf("%d\n", bit_get(bits, 5));            // EXPECT: 0
    bit_toggle(bits, 5);
    bit_toggle(bits, 6);
    printf("%d\n", bits[0]);                     // EXPECT: 96
    /* bit5=32, bit6=64: 32+64=96 */

    /* --- CROSS-WORD OPERATIONS --- */
    bit_clear_all(bits, ARRAY_WORDS);
    /* Set bits at positions 0, 63, 64, 127, 128, 191, 255 */
    bit_set(bits, 0);
    bit_set(bits, 63);
    bit_set(bits, 64);
    bit_set(bits, 127);
    bit_set(bits, 128);
    bit_set(bits, 191);
    bit_set(bits, 255);

    printf("%d\n", bit_get(bits, 0));            // EXPECT: 1
    printf("%d\n", bit_get(bits, 63));           // EXPECT: 1
    printf("%d\n", bit_get(bits, 64));           // EXPECT: 1
    printf("%d\n", bit_get(bits, 127));          // EXPECT: 1
    printf("%d\n", bit_get(bits, 128));          // EXPECT: 1
    printf("%d\n", bit_get(bits, 191));          // EXPECT: 1
    printf("%d\n", bit_get(bits, 255));          // EXPECT: 1

    /* Bits adjacent to set bits should be 0 */
    printf("%d\n", bit_get(bits, 1));            // EXPECT: 0
    printf("%d\n", bit_get(bits, 62));           // EXPECT: 0
    printf("%d\n", bit_get(bits, 65));           // EXPECT: 0
    printf("%d\n", bit_get(bits, 126));          // EXPECT: 0
    printf("%d\n", bit_get(bits, 254));          // EXPECT: 0

    /* --- COUNT SET BITS --- */
    printf("%d\n", bit_count_all(bits, ARRAY_WORDS)); // EXPECT: 7

    /* Count in range [0, 127] */
    printf("%d\n", bit_count_range(bits, 0, 127));    // EXPECT: 4
    /* bits 0, 63, 64, 127 are set in [0,127] */

    /* Count in range [64, 191] */
    printf("%d\n", bit_count_range(bits, 64, 191));   // EXPECT: 4
    /* bits 64, 127, 128, 191 */

    /* Count in range [100, 200] */
    printf("%d\n", bit_count_range(bits, 100, 200));  // EXPECT: 3
    /* bits 127, 128, 191 */

    /* --- FIND FIRST SET BIT --- */
    printf("%d\n", bit_find_first(bits, 0, 255));     // EXPECT: 0
    printf("%d\n", bit_find_first(bits, 1, 255));     // EXPECT: 63
    printf("%d\n", bit_find_first(bits, 64, 255));    // EXPECT: 64
    printf("%d\n", bit_find_first(bits, 65, 255));    // EXPECT: 127
    printf("%d\n", bit_find_first(bits, 128, 255));   // EXPECT: 128
    printf("%d\n", bit_find_first(bits, 192, 255));   // EXPECT: 255
    printf("%d\n", bit_find_first(bits, 1, 62));      // EXPECT: -1
    /* No set bits in [1, 62] */

    /* --- SIEVE OF ERATOSTHENES USING BIT ARRAY --- */
    /* Find primes up to 63 using the bit array as a sieve */
    unsigned int sieve[2];  /* 64 bits */
    /* Mark all as potentially prime (set = composite, clear = prime) */
    sieve[0] = 0u;
    sieve[1] = 0u;
    /* Mark 0 and 1 as composite */
    sieve[0] = sieve[0] | 1u;  /* bit 0 */
    sieve[0] = sieve[0] | 2u;  /* bit 1 */
    /* Sieve: for each p from 2 to 7 (sqrt(63)), mark multiples */
    int p;
    for (p = 2; p <= 7; p++) {
        if (!((sieve[p / 32] >> (p % 32)) & 1)) {
            int mul;
            for (mul = p * p; mul < 64; mul += p) {
                sieve[mul / 32] = sieve[mul / 32] | (1u << (mul % 32));
            }
        }
    }
    /* Count primes up to 63 */
    int prime_count = 0;
    for (i = 0; i < 64; i++) {
        if (!((sieve[i / 32] >> (i % 32)) & 1)) prime_count++;
    }
    printf("%d\n", prime_count);                     // EXPECT: 18
    /* Primes <= 63: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61 = 18 */

    return 0;
}
