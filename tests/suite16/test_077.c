int printf(const char *fmt, ...);

// Test 077: Move-to-front transform (MTF) - used as preprocessing for compression

void mtf_init(int *alphabet, int size) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        alphabet[i] = i;
    }
}

int mtf_find(int *alphabet, int size, int value) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        if (alphabet[i] == value) return i;
    }
    return -1;
}

void mtf_move_to_front(int *alphabet, int pos) {
    int val = alphabet[pos];
    int i;
    for (i = pos; i > 0; i = i - 1) {
        alphabet[i] = alphabet[i - 1];
    }
    alphabet[0] = val;
}

void mtf_encode(int *input, int len, int *output, int alpha_size) {
    int alphabet[256];
    mtf_init(alphabet, alpha_size);
    int i;
    for (i = 0; i < len; i = i + 1) {
        int pos = mtf_find(alphabet, alpha_size, input[i]);
        output[i] = pos;
        mtf_move_to_front(alphabet, pos);
    }
}

void mtf_decode(int *encoded, int len, int *output, int alpha_size) {
    int alphabet[256];
    mtf_init(alphabet, alpha_size);
    int i;
    for (i = 0; i < len; i = i + 1) {
        int pos = encoded[i];
        output[i] = alphabet[pos];
        mtf_move_to_front(alphabet, pos);
    }
}

int count_zeros(int *arr, int len) {
    int c = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] == 0) c = c + 1;
    }
    return c;
}

int sum_array(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        s = s + arr[i];
    }
    return s;
}

int main() {
    // Input: repeated pattern with values 0-3
    // "ABACABA" = 0,1,0,2,0,1,0
    int input1[7];
    input1[0] = 0; input1[1] = 1; input1[2] = 0;
    input1[3] = 2; input1[4] = 0; input1[5] = 1; input1[6] = 0;
    int len1 = 7;

    int encoded[20];
    mtf_encode(input1, len1, encoded, 4);

    int i;
    for (i = 0; i < len1; i = i + 1) {
        printf("MTF[%d]: %d\n", i, encoded[i]); // EXPECT: MTF[0]: 0
        // EXPECT: MTF[1]: 1
        // EXPECT: MTF[2]: 1
        // EXPECT: MTF[3]: 2
        // EXPECT: MTF[4]: 1
        // EXPECT: MTF[5]: 2
        // EXPECT: MTF[6]: 1
    }

    // Decode and verify roundtrip
    int decoded[20];
    mtf_decode(encoded, len1, decoded, 4);
    int match = 1;
    for (i = 0; i < len1; i = i + 1) {
        if (input1[i] != decoded[i]) match = 0;
    }
    printf("MTF roundtrip: %d\n", match); // EXPECT: MTF roundtrip: 1

    // Count zeros and low values (measure of compressibility)
    int zeros = count_zeros(encoded, len1);
    printf("Zero count: %d\n", zeros); // EXPECT: Zero count: 1

    int orig_sum = sum_array(input1, len1);
    int enc_sum = sum_array(encoded, len1);
    printf("Orig sum: %d\n", orig_sum); // EXPECT: Orig sum: 4
    printf("Enc sum: %d\n", enc_sum); // EXPECT: Enc sum: 8

    // Test with highly repetitive input: all same value
    int input2[6];
    for (i = 0; i < 6; i = i + 1) input2[i] = 3;
    mtf_encode(input2, 6, encoded, 4);

    printf("Repeat enc[0]: %d\n", encoded[0]); // EXPECT: Repeat enc[0]: 3
    int rest_zero = 1;
    for (i = 1; i < 6; i = i + 1) {
        if (encoded[i] != 0) rest_zero = 0;
    }
    printf("Repeat rest zero: %d\n", rest_zero); // EXPECT: Repeat rest zero: 1

    mtf_decode(encoded, 6, decoded, 4);
    int match2 = 1;
    for (i = 0; i < 6; i = i + 1) {
        if (decoded[i] != 3) match2 = 0;
    }
    printf("Repeat roundtrip: %d\n", match2); // EXPECT: Repeat roundtrip: 1

    // Test with alternating values: 0,1,0,1,0,1
    int input3[6];
    for (i = 0; i < 6; i = i + 1) input3[i] = i % 2;
    mtf_encode(input3, 6, encoded, 4);

    printf("Alt enc[0]: %d\n", encoded[0]); // EXPECT: Alt enc[0]: 0
    printf("Alt enc[1]: %d\n", encoded[1]); // EXPECT: Alt enc[1]: 1
    // After first two, 0 and 1 are at positions [1,0,...], so alternating gives 1,1,...
    printf("Alt enc[2]: %d\n", encoded[2]); // EXPECT: Alt enc[2]: 1
    printf("Alt enc[3]: %d\n", encoded[3]); // EXPECT: Alt enc[3]: 1
    printf("Alt enc[4]: %d\n", encoded[4]); // EXPECT: Alt enc[4]: 1
    printf("Alt enc[5]: %d\n", encoded[5]); // EXPECT: Alt enc[5]: 1

    mtf_decode(encoded, 6, decoded, 4);
    int match3 = 1;
    for (i = 0; i < 6; i = i + 1) {
        if (decoded[i] != input3[i]) match3 = 0;
    }
    printf("Alt roundtrip: %d\n", match3); // EXPECT: Alt roundtrip: 1

    // Test with larger alphabet: values 0-9
    int input4[5];
    input4[0] = 9; input4[1] = 9; input4[2] = 5; input4[3] = 5; input4[4] = 9;
    mtf_encode(input4, 5, encoded, 10);
    printf("Big enc[0]: %d\n", encoded[0]); // EXPECT: Big enc[0]: 9
    printf("Big enc[1]: %d\n", encoded[1]); // EXPECT: Big enc[1]: 0
    printf("Big enc[2]: %d\n", encoded[2]); // EXPECT: Big enc[2]: 6
    printf("Big enc[3]: %d\n", encoded[3]); // EXPECT: Big enc[3]: 0
    printf("Big enc[4]: %d\n", encoded[4]); // EXPECT: Big enc[4]: 1

    mtf_decode(encoded, 5, decoded, 10);
    int match4 = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (decoded[i] != input4[i]) match4 = 0;
    }
    printf("Big roundtrip: %d\n", match4); // EXPECT: Big roundtrip: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
