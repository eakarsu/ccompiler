int printf(const char *fmt, ...);

// Delta Encoding and Decoding

void delta_encode(int *input, int *output, int len) {
    int i = 0;
    if (len > 0) {
        output[0] = input[0];
        i = 1;
    }
    while (i < len) {
        output[i] = input[i] - input[i - 1];
        i = i + 1;
    }
}

void delta_decode(int *input, int *output, int len) {
    int i = 0;
    if (len > 0) {
        output[0] = input[0];
        i = 1;
    }
    while (i < len) {
        output[i] = output[i - 1] + input[i];
        i = i + 1;
    }
}

int arrays_equal(int *a, int *b, int len) {
    int i = 0;
    while (i < len) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int sum_abs(int *arr, int len) {
    int total = 0;
    int i = 0;
    while (i < len) {
        total = total + abs_val(arr[i]);
        i = i + 1;
    }
    return total;
}

int max_val(int *arr, int len) {
    int mx = arr[0];
    int i = 1;
    while (i < len) {
        if (arr[i] > mx) mx = arr[i];
        i = i + 1;
    }
    return mx;
}

int min_val(int *arr, int len) {
    int mn = arr[0];
    int i = 1;
    while (i < len) {
        if (arr[i] < mn) mn = arr[i];
        i = i + 1;
    }
    return mn;
}

void double_delta_encode(int *input, int *temp, int *output, int len) {
    delta_encode(input, temp, len);
    delta_encode(temp, output, len);
}

void double_delta_decode(int *input, int *temp, int *output, int len) {
    delta_decode(input, temp, len);
    delta_decode(temp, output, len);
}

int main() {
    // Test 1: Monotonically increasing
    int data1[8];
    data1[0] = 10; data1[1] = 13; data1[2] = 17; data1[3] = 22;
    data1[4] = 28; data1[5] = 35; data1[6] = 43; data1[7] = 52;

    int encoded[8];
    delta_encode(data1, encoded, 8);

    printf("Delta encoded:");
    int i = 0;
    while (i < 8) {
        printf(" %d", encoded[i]);
        i = i + 1;
    }
    printf("\n"); // EXPECT: Delta encoded: 10 3 4 5 6 7 8 9

    // Compression benefit: sum of abs deltas vs sum of abs originals
    int orig_sum = sum_abs(data1, 8);
    int delta_sum = sum_abs(encoded, 8);
    printf("Original sum: %d\n", orig_sum); // EXPECT: Original sum: 220
    printf("Delta sum: %d\n", delta_sum); // EXPECT: Delta sum: 52

    // Decode and verify
    int decoded[8];
    delta_decode(encoded, decoded, 8);
    int match1 = arrays_equal(data1, decoded, 8);
    printf("Decode match: %d\n", match1); // EXPECT: Decode match: 1

    // Test 2: Constant sequence (best case for delta)
    int data2[6];
    data2[0] = 5; data2[1] = 5; data2[2] = 5;
    data2[3] = 5; data2[4] = 5; data2[5] = 5;

    int enc2[6];
    delta_encode(data2, enc2, 6);
    printf("Constant deltas: %d %d %d %d %d %d\n", enc2[0], enc2[1], enc2[2], enc2[3], enc2[4], enc2[5]);
    // EXPECT: Constant deltas: 5 0 0 0 0 0

    int dec2[6];
    delta_decode(enc2, dec2, 6);
    int match2 = arrays_equal(data2, dec2, 6);
    printf("Constant match: %d\n", match2); // EXPECT: Constant match: 1

    // Test 3: Decreasing values
    int data3[5];
    data3[0] = 100; data3[1] = 90; data3[2] = 75; data3[3] = 55; data3[4] = 30;

    int enc3[5];
    delta_encode(data3, enc3, 5);
    printf("Decreasing deltas: %d %d %d %d %d\n", enc3[0], enc3[1], enc3[2], enc3[3], enc3[4]);
    // EXPECT: Decreasing deltas: 100 -10 -15 -20 -25

    int dec3[5];
    delta_decode(enc3, dec3, 5);
    int match3 = arrays_equal(data3, dec3, 5);
    printf("Decreasing match: %d\n", match3); // EXPECT: Decreasing match: 1

    // Test 4: Double delta encoding (good for linear sequences)
    int data4[6];
    data4[0] = 2; data4[1] = 5; data4[2] = 8;
    data4[3] = 11; data4[4] = 14; data4[5] = 17;

    int temp4[6];
    int dd_enc[6];
    double_delta_encode(data4, temp4, dd_enc, 6);
    printf("Double delta: %d %d %d %d %d %d\n", dd_enc[0], dd_enc[1], dd_enc[2], dd_enc[3], dd_enc[4], dd_enc[5]);
    // EXPECT: Double delta: 2 1 0 0 0 0

    int temp4b[6];
    int dd_dec[6];
    double_delta_decode(dd_enc, temp4b, dd_dec, 6);
    int match4 = arrays_equal(data4, dd_dec, 6);
    printf("Double delta match: %d\n", match4); // EXPECT: Double delta match: 1

    // Test 5: Range reduction
    int range_orig = max_val(data1, 8) - min_val(data1, 8);
    int range_delta = max_val(encoded, 8) - min_val(encoded, 8);
    printf("Original range: %d\n", range_orig); // EXPECT: Original range: 42
    printf("Delta range: %d\n", range_delta); // EXPECT: Delta range: 7

    printf("Delta encoding done\n"); // EXPECT: Delta encoding done
    return 0;
}
