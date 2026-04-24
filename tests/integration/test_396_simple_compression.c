int printf(const char *fmt, ...);
// EXPECT: all zeros: 1 runs\nall ones: 1 runs\nalternating: 64 runs\nsparse: 3 runs\nround-trip match: 1\nsparse runs:\n  16 x 0\n  8 x 1\n  40 x 0\ncompression: 64 -> 96 (larger)\nblock pattern: 3 runs\nblock round-trip: 1
// Test: simple compression using run-length encoding on bits

// RLE compress: store runs of 0s and 1s
// Output format: (count, bit) pairs packed into array
int rle_compress(int *input, int num_bits, int *output) {
    int out_idx = 0;
    int i = 0;
    while (i < num_bits) {
        int word = i / 32;
        int pos = i % 32;
        int bit = (input[word] >> pos) & 1;
        int run = 0;
        while (i < num_bits) {
            int w = i / 32;
            int p = i % 32;
            int b = (input[w] >> p) & 1;
            if (b != bit) break;
            run++;
            i++;
        }
        // Pack: count in upper bits, bit value in bit 0
        output[out_idx] = (run << 1) | bit;
        out_idx++;
    }
    return out_idx;  // number of runs
}

// RLE decompress
void rle_decompress(int *compressed, int num_runs, int *output) {
    output[0] = 0; output[1] = 0;
    output[2] = 0; output[3] = 0;
    int bit_pos = 0;
    int i;
    for (i = 0; i < num_runs; i++) {
        int count = compressed[i] >> 1;
        int bit = compressed[i] & 1;
        int j;
        for (j = 0; j < count; j++) {
            if (bit) {
                int w = bit_pos / 32;
                int p = bit_pos % 32;
                output[w] = output[w] | (1 << p);
            }
            bit_pos++;
        }
    }
}

int get_bit(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

int main(void) {
    // Test 1: all zeros (highly compressible)
    int zeros[2];
    zeros[0] = 0; zeros[1] = 0;
    int compressed[64];
    int runs = rle_compress(zeros, 64, compressed);
    printf("all zeros: %d runs\n", runs);

    // Test 2: all ones
    int ones[2];
    ones[0] = -1; ones[1] = -1;
    runs = rle_compress(ones, 64, compressed);
    printf("all ones: %d runs\n", runs);

    // Test 3: alternating bits (worst case)
    int alt[2];
    alt[0] = 0x55555555;  // 01010101...
    alt[1] = 0x55555555;
    runs = rle_compress(alt, 64, compressed);
    printf("alternating: %d runs\n", runs);

    // Test 4: realistic pattern - mostly zeros with some ones
    int sparse[2];
    sparse[0] = 0x00FF0000;  // 8 ones in a row
    sparse[1] = 0;
    runs = rle_compress(sparse, 64, compressed);
    printf("sparse: %d runs\n", runs);

    // Verify round-trip for sparse pattern
    int decompressed[4];
    rle_decompress(compressed, runs, decompressed);
    printf("round-trip match: %d\n",
           decompressed[0] == sparse[0] && decompressed[1] == sparse[1]);

    // Print runs for sparse pattern
    printf("sparse runs:\n");
    int i;
    for (i = 0; i < runs; i++) {
        int count = compressed[i] >> 1;
        int bit = compressed[i] & 1;
        printf("  %d x %d\n", count, bit);
    }

    // Compression ratio
    int original_bits = 64;
    int compressed_bits = runs * 32;  // each run stored as int
    printf("compression: %d -> %d (%s)\n",
           original_bits, compressed_bits,
           compressed_bits < original_bits ? "smaller" : "larger");

    // Test 5: block pattern
    int block[4];
    block[0] = 0xFFFF0000;
    block[1] = 0x0000FFFF;
    block[2] = 0; block[3] = 0;
    runs = rle_compress(block, 64, compressed);
    printf("block pattern: %d runs\n", runs);
    rle_decompress(compressed, runs, decompressed);
    printf("block round-trip: %d\n",
           decompressed[0] == block[0] && decompressed[1] == block[1]);

    return 0;
}
