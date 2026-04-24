int printf(const char *fmt, ...);

// Bloom filter simulation with multiple hash functions

struct BloomFilter {
    int bits[64];
    int size;
    int num_inserted;
};

void bloom_init(struct BloomFilter *bf) {
    int i;
    bf->size = 64;
    bf->num_inserted = 0;
    for (i = 0; i < 64; i++) {
        bf->bits[i] = 0;
    }
}

int hash1(int value, int size) {
    int h;
    h = value * 7;
    if (h < 0) h = -h;
    return h % size;
}

int hash2(int value, int size) {
    int h;
    h = value * 13 + 17;
    if (h < 0) h = -h;
    return h % size;
}

int hash3(int value, int size) {
    int h;
    h = value * 31 + 5;
    if (h < 0) h = -h;
    return h % size;
}

void bloom_insert(struct BloomFilter *bf, int value) {
    int h1;
    int h2;
    int h3;
    h1 = hash1(value, bf->size);
    h2 = hash2(value, bf->size);
    h3 = hash3(value, bf->size);
    bf->bits[h1] = 1;
    bf->bits[h2] = 1;
    bf->bits[h3] = 1;
    bf->num_inserted = bf->num_inserted + 1;
}

int bloom_maybe_contains(struct BloomFilter *bf, int value) {
    int h1;
    int h2;
    int h3;
    h1 = hash1(value, bf->size);
    h2 = hash2(value, bf->size);
    h3 = hash3(value, bf->size);
    return bf->bits[h1] && bf->bits[h2] && bf->bits[h3];
}

int bloom_count_set_bits(struct BloomFilter *bf) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < bf->size; i++) {
        if (bf->bits[i]) {
            count = count + 1;
        }
    }
    return count;
}

int bloom_definitely_not_contains(struct BloomFilter *bf, int val) {
    return !bloom_maybe_contains(bf, val);
}

int main() {
    struct BloomFilter bf;
    int i;
    int false_positives;
    int true_negatives;
    int set_bits;

    bloom_init(&bf);

    printf("Initial set bits: %d\n", bloom_count_set_bits(&bf));
    // EXPECT: Initial set bits: 0

    bloom_insert(&bf, 42);
    printf("Contains 42: %d\n", bloom_maybe_contains(&bf, 42));
    // EXPECT: Contains 42: 1

    printf("Definitely not 99: %d\n", bloom_definitely_not_contains(&bf, 99));
    // EXPECT: Definitely not 99: 1

    bloom_insert(&bf, 10);
    bloom_insert(&bf, 20);
    bloom_insert(&bf, 30);

    printf("Contains 10: %d\n", bloom_maybe_contains(&bf, 10));
    // EXPECT: Contains 10: 1

    printf("Contains 20: %d\n", bloom_maybe_contains(&bf, 20));
    // EXPECT: Contains 20: 1

    printf("Contains 30: %d\n", bloom_maybe_contains(&bf, 30));
    // EXPECT: Contains 30: 1

    printf("Inserted count: %d\n", bf.num_inserted);
    // EXPECT: Inserted count: 4

    set_bits = bloom_count_set_bits(&bf);
    printf("Set bits after 4 inserts: %d\n", set_bits);
    // EXPECT: Set bits after 4 inserts: 12

    // Test hash function values directly
    printf("hash1(42,64): %d\n", hash1(42, 64));
    // EXPECT: hash1(42,64): 38
    printf("hash2(42,64): %d\n", hash2(42, 64));
    // EXPECT: hash2(42,64): 51
    printf("hash3(42,64): %d\n", hash3(42, 64));
    // EXPECT: hash3(42,64): 27

    // Insert more values
    bloom_insert(&bf, 100);
    bloom_insert(&bf, 200);
    bloom_insert(&bf, 300);

    printf("Contains 100: %d\n", bloom_maybe_contains(&bf, 100));
    // EXPECT: Contains 100: 1

    printf("Contains 200: %d\n", bloom_maybe_contains(&bf, 200));
    // EXPECT: Contains 200: 1

    printf("Contains 300: %d\n", bloom_maybe_contains(&bf, 300));
    // EXPECT: Contains 300: 1

    // Count false positives for values 500-509
    false_positives = 0;
    for (i = 500; i < 510; i++) {
        if (bloom_maybe_contains(&bf, i)) {
            false_positives = false_positives + 1;
        }
    }
    printf("False positives (500-509): %d\n", false_positives);
    // EXPECT: False positives (500-509): 0

    // Count true negatives for 1000-1009
    true_negatives = 0;
    for (i = 1000; i < 1010; i++) {
        if (bloom_definitely_not_contains(&bf, i)) {
            true_negatives = true_negatives + 1;
        }
    }
    printf("True negatives (1000-1009): %d\n", true_negatives);
    // EXPECT: True negatives (1000-1009): 8

    set_bits = bloom_count_set_bits(&bf);
    printf("Final set bits: %d\n", set_bits);
    // EXPECT: Final set bits: 21

    printf("Total inserted: %d\n", bf.num_inserted);
    // EXPECT: Total inserted: 7

    return 0;
}
