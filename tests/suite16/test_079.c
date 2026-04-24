int printf(const char *fmt, ...);

// Test 079: Combined RLE + delta encoding pipeline for sensor data compression

void delta_encode(int *input, int len, int *output) {
    output[0] = input[0];
    int i;
    for (i = 1; i < len; i = i + 1) {
        output[i] = input[i] - input[i - 1];
    }
}

void delta_decode(int *encoded, int len, int *output) {
    output[0] = encoded[0];
    int i;
    for (i = 1; i < len; i = i + 1) {
        output[i] = output[i - 1] + encoded[i];
    }
}

// RLE on integers: store (value, count) pairs
int rle_encode_int(int *input, int len, int *vals, int *counts) {
    int pairs = 0;
    int i = 0;
    while (i < len) {
        int v = input[i];
        int c = 1;
        while (i + c < len && input[i + c] == v) {
            c = c + 1;
        }
        vals[pairs] = v;
        counts[pairs] = c;
        pairs = pairs + 1;
        i = i + c;
    }
    return pairs;
}

int rle_decode_int(int *vals, int *counts, int pairs, int *output) {
    int pos = 0;
    int i;
    for (i = 0; i < pairs; i = i + 1) {
        int j;
        for (j = 0; j < counts[i]; j = j + 1) {
            output[pos] = vals[i];
            pos = pos + 1;
        }
    }
    return pos;
}

// Full pipeline: delta -> RLE
int compress_pipeline(int *input, int len, int *rle_vals, int *rle_counts) {
    int delta[30];
    delta_encode(input, len, delta);
    return rle_encode_int(delta, len, rle_vals, rle_counts);
}

int decompress_pipeline(int *rle_vals, int *rle_counts, int pairs, int *output) {
    int delta[30];
    int len = rle_decode_int(rle_vals, rle_counts, pairs, delta);
    delta_decode(delta, len, output);
    return len;
}

int arrays_equal(int *a, int *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main() {
    // Simulated sensor data: slowly increasing with plateaus
    int sensor[20];
    sensor[0] = 100; sensor[1] = 100; sensor[2] = 100;
    sensor[3] = 101; sensor[4] = 101; sensor[5] = 101;
    sensor[6] = 101; sensor[7] = 102; sensor[8] = 102;
    sensor[9] = 103; sensor[10] = 103; sensor[11] = 103;
    sensor[12] = 103; sensor[13] = 103; sensor[14] = 104;
    sensor[15] = 104;
    int slen = 16;

    // Step 1: Delta encode
    int delta[20];
    delta_encode(sensor, slen, delta);
    printf("Delta[0]: %d\n", delta[0]); // EXPECT: Delta[0]: 100
    printf("Delta[1]: %d\n", delta[1]); // EXPECT: Delta[1]: 0
    printf("Delta[3]: %d\n", delta[3]); // EXPECT: Delta[3]: 1
    printf("Delta[7]: %d\n", delta[7]); // EXPECT: Delta[7]: 1

    // Step 2: RLE on delta values
    int rle_vals[20];
    int rle_counts[20];
    int pairs = rle_encode_int(delta, slen, rle_vals, rle_counts);
    printf("RLE pairs: %d\n", pairs); // EXPECT: RLE pairs: 10

    // Show the pairs
    int i;
    for (i = 0; i < pairs; i = i + 1) {
        printf("RLE %d: val=%d count=%d\n", i, rle_vals[i], rle_counts[i]);
    }
    // EXPECT: RLE 0: val=100 count=1
    // EXPECT: RLE 1: val=0 count=2
    // EXPECT: RLE 2: val=1 count=1
    // EXPECT: RLE 3: val=0 count=3
    // EXPECT: RLE 4: val=1 count=1
    // EXPECT: RLE 5: val=0 count=1
    // EXPECT: RLE 6: val=1 count=1
    // EXPECT: RLE 7: val=0 count=4
    // EXPECT: RLE 8: val=1 count=1
    // EXPECT: RLE 9: val=0 count=1

    // Compression: 16 values -> 10 pairs (20 values)
    int ratio = (pairs * 2 * 100) / slen;
    printf("Compression ratio: %d\n", ratio); // EXPECT: Compression ratio: 125

    // Full pipeline test
    int rle_v2[20];
    int rle_c2[20];
    int pairs2 = compress_pipeline(sensor, slen, rle_v2, rle_c2);
    printf("Pipeline pairs: %d\n", pairs2); // EXPECT: Pipeline pairs: 10

    int restored[20];
    int rlen = decompress_pipeline(rle_v2, rle_c2, pairs2, restored);
    printf("Restored len: %d\n", rlen); // EXPECT: Restored len: 16

    int match = arrays_equal(sensor, restored, slen);
    printf("Pipeline roundtrip: %d\n", match); // EXPECT: Pipeline roundtrip: 1

    // Test with constant data (best case for delta+RLE)
    int constant[10];
    for (i = 0; i < 10; i = i + 1) constant[i] = 50;
    int cp = compress_pipeline(constant, 10, rle_v2, rle_c2);
    printf("Constant pairs: %d\n", cp); // EXPECT: Constant pairs: 2

    decompress_pipeline(rle_v2, rle_c2, cp, restored);
    int cm = arrays_equal(constant, restored, 10);
    printf("Constant roundtrip: %d\n", cm); // EXPECT: Constant roundtrip: 1

    // Test with linear data (also good for delta+RLE)
    int linear[8];
    for (i = 0; i < 8; i = i + 1) linear[i] = 10 + i * 5;
    int lp = compress_pipeline(linear, 8, rle_v2, rle_c2);
    printf("Linear pairs: %d\n", lp); // EXPECT: Linear pairs: 2

    decompress_pipeline(rle_v2, rle_c2, lp, restored);
    int lm = arrays_equal(linear, restored, 8);
    printf("Linear roundtrip: %d\n", lm); // EXPECT: Linear roundtrip: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
