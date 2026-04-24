int printf(const char *fmt, ...);

// Test 074: Delta encoding - stores differences between consecutive values

void delta_encode(int *input, int len, int *output) {
    int i;
    output[0] = input[0];
    for (i = 1; i < len; i = i + 1) {
        output[i] = input[i] - input[i - 1];
    }
}

void delta_decode(int *encoded, int len, int *output) {
    int i;
    output[0] = encoded[0];
    for (i = 1; i < len; i = i + 1) {
        output[i] = output[i - 1] + encoded[i];
    }
}

int arrays_equal(int *a, int *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int sum_abs(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        int v = arr[i];
        if (v < 0) v = -v;
        s = s + v;
    }
    return s;
}

// Double delta: delta of delta (good for linear sequences)
void double_delta_encode(int *input, int len, int *output) {
    int delta[20];
    delta_encode(input, len, delta);
    delta_encode(delta, len, output);
}

void double_delta_decode(int *encoded, int len, int *output) {
    int delta[20];
    delta_decode(encoded, len, delta);
    delta_decode(delta, len, output);
}

int main() {
    // Monotonically increasing data
    int data1[8];
    data1[0] = 10; data1[1] = 13; data1[2] = 17;
    data1[3] = 20; data1[4] = 25; data1[5] = 28;
    data1[6] = 33; data1[7] = 40;

    int encoded[8];
    delta_encode(data1, 8, encoded);

    printf("Delta[0]: %d\n", encoded[0]); // EXPECT: Delta[0]: 10
    printf("Delta[1]: %d\n", encoded[1]); // EXPECT: Delta[1]: 3
    printf("Delta[2]: %d\n", encoded[2]); // EXPECT: Delta[2]: 4
    printf("Delta[3]: %d\n", encoded[3]); // EXPECT: Delta[3]: 3
    printf("Delta[4]: %d\n", encoded[4]); // EXPECT: Delta[4]: 5
    printf("Delta[5]: %d\n", encoded[5]); // EXPECT: Delta[5]: 3
    printf("Delta[6]: %d\n", encoded[6]); // EXPECT: Delta[6]: 5
    printf("Delta[7]: %d\n", encoded[7]); // EXPECT: Delta[7]: 7

    int decoded[8];
    delta_decode(encoded, 8, decoded);
    int match = arrays_equal(data1, decoded, 8);
    printf("Roundtrip: %d\n", match); // EXPECT: Roundtrip: 1

    // Compare sum of absolute values (proxy for compressibility)
    int orig_sum = sum_abs(data1, 8);
    int delta_sum = sum_abs(encoded, 8);
    printf("Orig abs sum: %d\n", orig_sum); // EXPECT: Orig abs sum: 186
    printf("Delta abs sum: %d\n", delta_sum); // EXPECT: Delta abs sum: 40

    int better = (delta_sum < orig_sum) ? 1 : 0;
    printf("Delta smaller: %d\n", better); // EXPECT: Delta smaller: 1

    // Test with constant sequence (delta should be all zeros except first)
    int data2[5];
    int i;
    for (i = 0; i < 5; i = i + 1) data2[i] = 42;
    int enc2[5];
    delta_encode(data2, 5, enc2);
    printf("Const delta[0]: %d\n", enc2[0]); // EXPECT: Const delta[0]: 42
    int all_zero = 1;
    for (i = 1; i < 5; i = i + 1) {
        if (enc2[i] != 0) all_zero = 0;
    }
    printf("Const rest zero: %d\n", all_zero); // EXPECT: Const rest zero: 1

    // Test with linear sequence: double delta should be nearly all zeros
    int data3[6];
    for (i = 0; i < 6; i = i + 1) data3[i] = 10 + i * 3;
    int dbl_enc[6];
    double_delta_encode(data3, 6, dbl_enc);
    printf("DblDelta[0]: %d\n", dbl_enc[0]); // EXPECT: DblDelta[0]: 10
    printf("DblDelta[1]: %d\n", dbl_enc[1]); // EXPECT: DblDelta[1]: -7

    // All values from index 2 onward should be 0 for linear data
    int zeros_from_2 = 1;
    for (i = 2; i < 6; i = i + 1) {
        if (dbl_enc[i] != 0) zeros_from_2 = 0;
    }
    printf("Linear dbl-delta zeros: %d\n", zeros_from_2); // EXPECT: Linear dbl-delta zeros: 1

    // Roundtrip double delta
    int dbl_dec[6];
    double_delta_decode(dbl_enc, 6, dbl_dec);
    int dbl_match = arrays_equal(data3, dbl_dec, 6);
    printf("Dbl roundtrip: %d\n", dbl_match); // EXPECT: Dbl roundtrip: 1

    // Test with decreasing data
    int data4[4];
    data4[0] = 100; data4[1] = 80; data4[2] = 50; data4[3] = 10;
    int enc4[4];
    delta_encode(data4, 4, enc4);
    printf("Dec delta[1]: %d\n", enc4[1]); // EXPECT: Dec delta[1]: -20
    printf("Dec delta[2]: %d\n", enc4[2]); // EXPECT: Dec delta[2]: -30
    printf("Dec delta[3]: %d\n", enc4[3]); // EXPECT: Dec delta[3]: -40

    int dec4[4];
    delta_decode(enc4, 4, dec4);
    int match4 = arrays_equal(data4, dec4, 4);
    printf("Dec roundtrip: %d\n", match4); // EXPECT: Dec roundtrip: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
