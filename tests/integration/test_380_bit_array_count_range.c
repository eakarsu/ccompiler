int printf(const char *fmt, ...);
// EXPECT: total set (every 3rd): 43\ncount [0,32) = 11\ncount [0,64) = 22\ncount [0,128) = 43\ncount [32,64) = 11\ncount [64,96) = 10\ncount [96,128) = 11\ncount [0,3) = 1\ncount [0,6) = 2\ncount [1,4) = 1\nfirst 16 bits set: 16\nfirst 32 bits set: 16\nbits 16-32: 0\ndense count = 64\nsparse count = 1\ndata[0] popcount = 16\ndata[1] popcount = 16\nrank queries on alternating pattern:\n  rank(8) = 4\n  rank(16) = 8\n  rank(32) = 16\n  rank(48) = 24\n  rank(64) = 32
// Test: bit array - count range

void set_bit(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] | (1 << (idx % 32));
}

int get_bit(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

int popcount(int x) {
    int count = 0;
    int v = x;
    while (v) {
        v = v & (v - 1);
        count++;
    }
    return count;
}

int count_range(int *arr, int start, int end) {
    int count = 0;
    int i;
    // Simple bit-by-bit for now
    for (i = start; i < end; i++) {
        count = count + get_bit(arr, i);
    }
    return count;
}

int count_total(int *arr, int words) {
    int count = 0;
    int i;
    for (i = 0; i < words; i++) {
        count = count + popcount(arr[i]);
    }
    return count;
}

int main(void) {
    // Create array with known pattern
    int bits[4];
    bits[0] = 0; bits[1] = 0; bits[2] = 0; bits[3] = 0;

    // Set every 3rd bit
    int i;
    for (i = 0; i < 128; i += 3) {
        set_bit(bits, i);
    }

    printf("total set (every 3rd): %d\n", count_total(bits, 4));

    // Count in various ranges
    printf("count [0,32) = %d\n", count_range(bits, 0, 32));
    printf("count [0,64) = %d\n", count_range(bits, 0, 64));
    printf("count [0,128) = %d\n", count_range(bits, 0, 128));
    printf("count [32,64) = %d\n", count_range(bits, 32, 64));
    printf("count [64,96) = %d\n", count_range(bits, 64, 96));
    printf("count [96,128) = %d\n", count_range(bits, 96, 128));

    // Small ranges
    printf("count [0,3) = %d\n", count_range(bits, 0, 3));
    printf("count [0,6) = %d\n", count_range(bits, 0, 6));
    printf("count [1,4) = %d\n", count_range(bits, 1, 4));

    // Reset and set specific pattern: bits 0-15 all set
    bits[0] = 0xFFFF;
    bits[1] = 0;
    bits[2] = 0;
    bits[3] = 0;
    printf("first 16 bits set: %d\n", count_range(bits, 0, 16));
    printf("first 32 bits set: %d\n", count_range(bits, 0, 32));
    printf("bits 16-32: %d\n", count_range(bits, 16, 32));

    // Dense vs sparse comparison
    int dense[2];
    int sparse[2];
    dense[0] = -1; dense[1] = -1;  // all 64 bits set
    sparse[0] = 1; sparse[1] = 0;  // only bit 0 set

    printf("dense count = %d\n", count_total(dense, 2));
    printf("sparse count = %d\n", count_total(sparse, 2));

    // Running count (prefix sums of bits)
    int data[2];
    data[0] = 0xAAAAAAAA;  // alternating 10101010...
    data[1] = 0x55555555;  // alternating 01010101...
    printf("data[0] popcount = %d\n", popcount(data[0]));
    printf("data[1] popcount = %d\n", popcount(data[1]));

    // Rank query: how many bits set before position N
    printf("rank queries on alternating pattern:\n");
    int positions[5];
    positions[0] = 8;
    positions[1] = 16;
    positions[2] = 32;
    positions[3] = 48;
    positions[4] = 64;
    for (i = 0; i < 5; i++) {
        printf("  rank(%d) = %d\n", positions[i],
               count_range(data, 0, positions[i]));
    }

    return 0;
}
