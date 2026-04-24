int printf(const char *fmt, ...);
// EXPECT: search tests:\n  search(5) = 1\n  search(10) = 1\n  search(15) = 1\n  search(20) = 1\n  search(25) = 1\nnon-existent:\n  search(0) = 0\n  search(7) = 0\n  search(100) = 0\nmax XOR:\n  maxXOR(5) = 28\n  maxXOR(10) = 30\n  maxXOR(0) = 25\nbinary values:\n   5 = 00000101\n  10 = 00001010\n  15 = 00001111\n  20 = 00010100\n  25 = 00011001\nXOR pairs:\n  5 ^ 10 = 15\n  5 ^ 15 = 10\n  5 ^ 20 = 17\n  5 ^ 25 = 28\n  10 ^ 15 = 5\n  10 ^ 20 = 30\n  10 ^ 25 = 19\n  15 ^ 20 = 27\n  15 ^ 25 = 22\n  20 ^ 25 = 13
// Test: bit trie (binary trie for integers)

// Simple bit trie using array representation
// Each node: children[0] and children[1], plus a flag
// We use a flat array: node i has children at 2*i+1 (0-child) and 2*i+2 (1-child)
// Flag stored in separate bit array

int nodes[64];   // node existence flags (packed bits)
int values[64];  // end-of-number flags (packed bits)

void trie_init(void) {
    int i;
    for (i = 0; i < 64; i++) {
        nodes[i] = 0;
        values[i] = 0;
    }
}

void set_flag(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] | (1 << (idx % 32));
}

int get_flag(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

// Insert a number (using top 8 bits)
void trie_insert(int num) {
    int node = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int child = 2 * node + 1 + bit;
        set_flag(nodes, child);
        node = child;
    }
    set_flag(values, node);
}

// Search for a number
int trie_search(int num) {
    int node = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int child = 2 * node + 1 + bit;
        if (!get_flag(nodes, child)) return 0;
        node = child;
    }
    return get_flag(values, node);
}

// Find maximum XOR with any stored number
int trie_max_xor(int num) {
    int node = 0;
    int result = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int want = 1 - bit;  // want opposite bit for max XOR
        int child = 2 * node + 1 + want;
        if (get_flag(nodes, child)) {
            result = result | (1 << i);
            node = child;
        } else {
            node = 2 * node + 1 + bit;
        }
    }
    return result;
}

int main(void) {
    trie_init();

    // Insert values
    int vals[5];
    vals[0] = 5;   // 00000101
    vals[1] = 10;  // 00001010
    vals[2] = 15;  // 00001111
    vals[3] = 20;  // 00010100
    vals[4] = 25;  // 00011001

    int i;
    for (i = 0; i < 5; i++) {
        trie_insert(vals[i]);
    }

    // Search for inserted values
    printf("search tests:\n");
    for (i = 0; i < 5; i++) {
        printf("  search(%d) = %d\n", vals[i], trie_search(vals[i]));
    }

    // Search for non-existent values
    printf("non-existent:\n");
    printf("  search(0) = %d\n", trie_search(0));
    printf("  search(7) = %d\n", trie_search(7));
    printf("  search(100) = %d\n", trie_search(100));

    // Maximum XOR
    printf("max XOR:\n");
    printf("  maxXOR(5) = %d\n", trie_max_xor(5));
    printf("  maxXOR(10) = %d\n", trie_max_xor(10));
    printf("  maxXOR(0) = %d\n", trie_max_xor(0));

    // Verify binary representations
    printf("binary values:\n");
    for (i = 0; i < 5; i++) {
        printf("  %2d = ", vals[i]);
        int j;
        for (j = 7; j >= 0; j--) {
            printf("%d", (vals[i] >> j) & 1);
        }
        printf("\n");
    }

    // XOR verification
    printf("XOR pairs:\n");
    int j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            printf("  %d ^ %d = %d\n", vals[i], vals[j], vals[i] ^ vals[j]);
        }
    }

    return 0;
}
