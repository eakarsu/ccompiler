int printf(const char *fmt, ...);

// SHA-like Hash Accumulator - simplified hash function

struct HashState {
    int h0;
    int h1;
    int h2;
    int h3;
};

void hash_init(struct HashState *hs) {
    hs->h0 = 1732584193;   // 0x67452301
    hs->h1 = -271733879;   // 0xEFCDAB89 as signed
    hs->h2 = -1732584194;  // 0x98BADCFE as signed
    hs->h3 = 271733878;    // 0x10325476
}

int rotate_left(int val, int shift) {
    // Simplified rotation for positive values
    int left = val << shift;
    int right = (val >> (32 - shift)) & ((1 << shift) - 1);
    return left | right;
}

void hash_process_byte(struct HashState *hs, int byte_val) {
    int t0 = hs->h0;
    int t1 = hs->h1;
    int t2 = hs->h2;
    int t3 = hs->h3;

    // Mix the byte into the state
    t0 = t0 + byte_val;
    t0 = rotate_left(t0, 5);
    t0 = t0 ^ t1;

    t1 = t1 + (byte_val * 31);
    t1 = rotate_left(t1, 7);
    t1 = t1 ^ t2;

    t2 = t2 + (byte_val * 127);
    t2 = rotate_left(t2, 11);
    t2 = t2 ^ t3;

    t3 = t3 + (byte_val * 8191);
    t3 = rotate_left(t3, 13);
    t3 = t3 ^ t0;

    hs->h0 = t0;
    hs->h1 = t1;
    hs->h2 = t2;
    hs->h3 = t3;
}

void hash_string(char *input, struct HashState *hs) {
    hash_init(hs);
    int i = 0;
    while (input[i] != 0) {
        hash_process_byte(hs, (int)input[i]);
        i = i + 1;
    }
    // Finalize: process the length
    hash_process_byte(hs, i);
}

// Reduce hash to a single integer for easy comparison
int hash_to_int(struct HashState *hs) {
    return hs->h0 ^ hs->h1 ^ hs->h2 ^ hs->h3;
}

// Hash a sequence of integers
void hash_int_array(int *arr, int len, struct HashState *hs) {
    hash_init(hs);
    int i;
    for (i = 0; i < len; i++) {
        // Break int into bytes
        int val = arr[i];
        hash_process_byte(hs, val & 255);
        hash_process_byte(hs, (val >> 8) & 255);
        hash_process_byte(hs, (val >> 16) & 255);
        hash_process_byte(hs, (val >> 24) & 255);
    }
}

// Simple checksum for comparison
int simple_checksum(char *input) {
    int sum = 0;
    int i = 0;
    while (input[i] != 0) {
        sum = sum + (int)input[i];
        i = i + 1;
    }
    return sum;
}

int main() {
    struct HashState hs;

    // Test 1: hash of empty-ish string "A"
    hash_string("A", &hs);
    int h1 = hash_to_int(&hs);
    printf("Hash A computed: %d\n", h1 != 0);
    // EXPECT: Hash A computed: 1

    // Test 2: same input produces same hash
    struct HashState hs2;
    hash_string("A", &hs2);
    int h2 = hash_to_int(&hs2);
    printf("Deterministic: %d\n", h1 == h2);
    // EXPECT: Deterministic: 1

    // Test 3: different inputs produce different hashes
    hash_string("B", &hs);
    int h3 = hash_to_int(&hs);
    printf("A vs B differ: %d\n", h1 != h3);
    // EXPECT: A vs B differ: 1

    // Test 4: similar strings produce different hashes
    hash_string("hello", &hs);
    int hh1 = hash_to_int(&hs);
    hash_string("hellp", &hs2);
    int hh2 = hash_to_int(&hs2);
    printf("hello vs hellp differ: %d\n", hh1 != hh2);
    // EXPECT: hello vs hellp differ: 1

    // Test 5: order matters
    hash_string("ab", &hs);
    int hab = hash_to_int(&hs);
    hash_string("ba", &hs2);
    int hba = hash_to_int(&hs2);
    printf("ab vs ba differ: %d\n", hab != hba);
    // EXPECT: ab vs ba differ: 1

    // Test 6: hash array of ints
    int arr1[4];
    arr1[0] = 1; arr1[1] = 2; arr1[2] = 3; arr1[3] = 4;
    hash_int_array(arr1, 4, &hs);
    int ha1 = hash_to_int(&hs);

    int arr2[4];
    arr2[0] = 1; arr2[1] = 2; arr2[2] = 3; arr2[3] = 5;
    hash_int_array(arr2, 4, &hs2);
    int ha2 = hash_to_int(&hs2);
    printf("Array hashes differ: %d\n", ha1 != ha2);
    // EXPECT: Array hashes differ: 1

    // Test 7: simple checksum vs hash - checksum has collision, hash shouldn't
    int cs1 = simple_checksum("ab");
    int cs2 = simple_checksum("ba");
    printf("Checksum collision: %d\n", cs1 == cs2);
    // EXPECT: Checksum collision: 1

    printf("Hash no collision: %d\n", hab != hba);
    // EXPECT: Hash no collision: 1

    // Test 8: longer strings
    hash_string("The quick brown fox jumps", &hs);
    int hlong1 = hash_to_int(&hs);
    hash_string("The quick brown fox jump", &hs2);
    int hlong2 = hash_to_int(&hs2);
    printf("Long strings differ: %d\n", hlong1 != hlong2);
    // EXPECT: Long strings differ: 1

    // Test 9: hash state independence
    struct HashState independent1;
    struct HashState independent2;
    hash_string("test1", &independent1);
    hash_string("test2", &independent2);
    hash_string("test1", &hs);
    int ind_ok = (hash_to_int(&independent1) == hash_to_int(&hs));
    printf("Independence: %d\n", ind_ok);
    // EXPECT: Independence: 1

    // Test 10: hash of numbers as strings
    hash_string("12345", &hs);
    int hn1 = hash_to_int(&hs);
    hash_string("12346", &hs2);
    int hn2 = hash_to_int(&hs2);
    printf("Number strings differ: %d\n", hn1 != hn2);
    // EXPECT: Number strings differ: 1

    printf("SHA-like hash tests passed\n");
    // EXPECT: SHA-like hash tests passed

    return 0;
}
