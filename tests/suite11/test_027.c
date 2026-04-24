int printf(const char *fmt, ...);

// Bloom filter simulation using multiple hash functions (integer-based)

struct BloomFilter {
    int bits[64];
    int size;
    int num_items;
};

void bloom_init(struct BloomFilter *bf, int size) {
    int i;
    bf->size = size;
    bf->num_items = 0;
    for (i = 0; i < 64; i++) {
        bf->bits[i] = 0;
    }
}

int hash1(int key, int size) {
    int h = key * 31;
    if (h < 0) h = -h;
    return h % size;
}

int hash2(int key, int size) {
    int h = key * 37 + 7;
    if (h < 0) h = -h;
    return h % size;
}

int hash3(int key, int size) {
    int h = key * 41 + 13;
    if (h < 0) h = -h;
    return h % size;
}

void bloom_add(struct BloomFilter *bf, int key) {
    int h1 = hash1(key, bf->size);
    int h2 = hash2(key, bf->size);
    int h3 = hash3(key, bf->size);
    bf->bits[h1] = 1;
    bf->bits[h2] = 1;
    bf->bits[h3] = 1;
    bf->num_items = bf->num_items + 1;
}

int bloom_check(struct BloomFilter *bf, int key) {
    int h1 = hash1(key, bf->size);
    int h2 = hash2(key, bf->size);
    int h3 = hash3(key, bf->size);
    return bf->bits[h1] && bf->bits[h2] && bf->bits[h3];
}

int bloom_count_set(struct BloomFilter *bf) {
    int count = 0;
    int i;
    for (i = 0; i < bf->size; i++) {
        if (bf->bits[i]) {
            count = count + 1;
        }
    }
    return count;
}

int bloom_false_positive_test(struct BloomFilter *bf, int start, int end) {
    int fp = 0;
    int i;
    for (i = start; i <= end; i++) {
        if (bloom_check(bf, i)) {
            fp = fp + 1;
        }
    }
    return fp;
}

int main() {
    struct BloomFilter bf;
    int i;
    int fp;
    int set_bits;

    bloom_init(&bf, 64);
    printf("initial set bits: %d\n", bloom_count_set(&bf));     // EXPECT: initial set bits: 0

    bloom_add(&bf, 10);
    printf("check 10: %d\n", bloom_check(&bf, 10));             // EXPECT: check 10: 1
    printf("check 20: %d\n", bloom_check(&bf, 20));             // EXPECT: check 20: 0

    bloom_add(&bf, 20);
    bloom_add(&bf, 30);
    bloom_add(&bf, 40);
    bloom_add(&bf, 50);
    printf("items: %d\n", bf.num_items);                         // EXPECT: items: 5
    printf("check 10: %d\n", bloom_check(&bf, 10));             // EXPECT: check 10: 1
    printf("check 30: %d\n", bloom_check(&bf, 30));             // EXPECT: check 30: 1
    printf("check 50: %d\n", bloom_check(&bf, 50));             // EXPECT: check 50: 1

    set_bits = bloom_count_set(&bf);
    printf("set bits: %d\n", set_bits);                          // EXPECT: set bits: 13

    // Check false positives in range 100-119
    fp = bloom_false_positive_test(&bf, 100, 119);
    printf("false positives 100-119: %d\n", fp);                 // EXPECT: false positives 100-119: 2

    // Add more items and check fill rate
    for (i = 100; i < 110; i++) {
        bloom_add(&bf, i);
    }
    printf("items after bulk: %d\n", bf.num_items);              // EXPECT: items after bulk: 15
    set_bits = bloom_count_set(&bf);
    printf("set bits after bulk: %d\n", set_bits);               // EXPECT: set bits after bulk: 35

    // Verify all added items are found (no false negatives)
    printf("check 100: %d\n", bloom_check(&bf, 100));           // EXPECT: check 100: 1
    printf("check 105: %d\n", bloom_check(&bf, 105));           // EXPECT: check 105: 1
    printf("check 109: %d\n", bloom_check(&bf, 109));           // EXPECT: check 109: 1

    // Test with larger range for false positives
    fp = bloom_false_positive_test(&bf, 200, 219);
    printf("false positives 200-219: %d\n", fp);                 // EXPECT: false positives 200-219: 2

    // Reset and test with different data
    bloom_init(&bf, 64);
    bloom_add(&bf, 1);
    bloom_add(&bf, 2);
    bloom_add(&bf, 3);
    printf("after reset items: %d\n", bf.num_items);             // EXPECT: after reset items: 3
    printf("check 1: %d\n", bloom_check(&bf, 1));               // EXPECT: check 1: 1
    printf("check 99: %d\n", bloom_check(&bf, 99));             // EXPECT: check 99: 0

    return 0;
}
