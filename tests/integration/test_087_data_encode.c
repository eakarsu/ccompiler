int printf(const char *fmt, ...);
// EXPECT: === Base64-like Encoding Tests ===\nB64 enc_len: 4\nB64 encoded: 16 20 9 3\nB64 decoded: 65 66 67\nB64 roundtrip: 1\nB64 enc_len2: 8\nB64 roundtrip2: 1\nB64 single_enc_len: 4\nB64 single_decoded: 42\nB64 zeros: 0 0 0 0\n=== RLE Integer Tests ===\nRLE1 pairs: 5\nRLE1 val=1 count=3\nRLE1 val=2 count=2\nRLE1 val=3 count=1\nRLE1 val=4 count=4\nRLE1 val=5 count=2\nRLE1 beneficial: 1\nRLE1 longest_run: 4\nRLE1 most_freq: 4\nRLE1 dec_len: 12\nRLE1 roundtrip: 1\nRLE2 pairs: 5\nRLE2 beneficial: 0\nRLE3 pairs: 1\nRLE3 val=7 count=8\nRLE3 beneficial: 1\n=== Delta Encoding Tests ===\nDelta1: 10 2 3 -1 6 5\nDelta1 roundtrip: 1\nDelta1 beneficial: 1\nDelta2: 1000 10 10 10 10\nDelta2 beneficial: 1\nDDelta3: 1000 -990 0 0 0\nDDelta3 roundtrip: 1\nDelta4: 5 0 0 0\nDelta5: 100 -10 -15 -25 -30\nDelta5 roundtrip: 1\n=== Zigzag Encoding Tests ===\nZZ encode 0: 0\nZZ encode -1: 1\nZZ encode 1: 2\nZZ encode -2: 3\nZZ encode 2: 4\nZZ encode -100: 199\nZZ encode 100: 200\nZZ decode 0: 0\nZZ decode 1: -1\nZZ decode 2: 1\nZZ decode 3: -2\nZZ decode 4: 2\nZZ encoded: 9 6 19 0 14 1\nZZ roundtrip: 1\nDZ encoded: 200 4 5 12 7\nDZ roundtrip: 1\n=== VLQ Encoding Tests ===\nVLQ 0: 1 bytes, val=0\nVLQ 127: 1 bytes, val=127\nVLQ 128: 2 bytes, val0=129 val1=0\nVLQ 16383: 2 bytes\nVLQ 16384: 3 bytes\nVLQ decode 42: val=42 consumed=1\nVLQ decode [129,0]: val=128 consumed=2\nVLQ array total_bytes: 6\nVLQ array decoded_count: 4\nVLQ array roundtrip: 1\n=== Gray Code Tests ===\nGray 0 -> 0 -> 0\nGray 1 -> 1 -> 1\nGray 2 -> 3 -> 2\nGray 3 -> 2 -> 3\nGray 4 -> 6 -> 4\nGray 5 -> 7 -> 5\nGray 6 -> 5 -> 6\nGray 7 -> 4 -> 7\nGray 8 -> 12 -> 8\nGray 9 -> 13 -> 9\nGray 10 -> 15 -> 10\nGray 11 -> 14 -> 11\nGray 12 -> 10 -> 12\nGray 13 -> 11 -> 13\nGray 14 -> 9 -> 14\nGray 15 -> 8 -> 15\nGray adjacent_single_bit: 1
// ============================================================
// test_087_data_encode.c
// Data encoding/decoding algorithms:
//   1. Base64-like encoding (simplified, integer-based)
//   2. Run-length encoding on integer arrays
//   3. Delta encoding/decoding
//   4. Zigzag encoding/decoding
//   5. Variable-length encoding (VLQ-like)
// ============================================================

// ---- Utility: print array ----
void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");
}

// ---- Utility: arrays equal ----
int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

// ---- Utility: copy array ----
void copy_array(int *src, int *dst, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

// ---- Utility: sum array ----
int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

// ============================================================
// 1. Base64-like Encoding (Simplified Integer-Based)
// ============================================================
// Takes a sequence of integers (0-255 byte values) and encodes them
// into groups of 6-bit values (0-63).
// Each 3 bytes (24 bits) become 4 encoded values (6 bits each).

// Encode 3 bytes into 4 base64-like values
void b64_encode_triple(int b0, int b1, int b2, int *out) {
    // Combine 3 bytes into 24-bit value
    int combined = (b0 * 256 * 256) + (b1 * 256) + b2;

    // Extract 4 groups of 6 bits
    out[3] = combined % 64;
    combined = combined / 64;
    out[2] = combined % 64;
    combined = combined / 64;
    out[1] = combined % 64;
    combined = combined / 64;
    out[0] = combined % 64;
}

// Decode 4 base64-like values back into 3 bytes
void b64_decode_quad(int e0, int e1, int e2, int e3, int *out) {
    int combined = e0 * 64 * 64 * 64 + e1 * 64 * 64 + e2 * 64 + e3;
    out[2] = combined % 256;
    combined = combined / 256;
    out[1] = combined % 256;
    combined = combined / 256;
    out[0] = combined % 256;
}

// Encode an array of bytes
// input: array of byte values (0-255)
// output: encoded values (0-63)
// Returns output length
int b64_encode(int *input, int in_len, int *output) {
    int out_idx = 0;
    int i;
    for (i = 0; i < in_len; i = i + 3) {
        int b0 = input[i];
        int b1 = (i + 1 < in_len) ? input[i + 1] : 0;
        int b2 = (i + 2 < in_len) ? input[i + 2] : 0;

        int quad[4];
        b64_encode_triple(b0, b1, b2, quad);

        int j;
        for (j = 0; j < 4; j++) {
            output[out_idx] = quad[j];
            out_idx++;
        }
    }
    return out_idx;
}

// Decode an array of encoded values back to bytes
int b64_decode(int *input, int in_len, int *output) {
    int out_idx = 0;
    int i;
    for (i = 0; i < in_len; i = i + 4) {
        int e0 = input[i];
        int e1 = (i + 1 < in_len) ? input[i + 1] : 0;
        int e2 = (i + 2 < in_len) ? input[i + 2] : 0;
        int e3 = (i + 3 < in_len) ? input[i + 3] : 0;

        int triple[3];
        b64_decode_quad(e0, e1, e2, e3, triple);

        int j;
        for (j = 0; j < 3; j++) {
            output[out_idx] = triple[j];
            out_idx++;
        }
    }
    return out_idx;
}

// ============================================================
// 2. Run-Length Encoding on Integer Arrays
// ============================================================
// Encode: [1,1,1,2,2,3] -> [(1,3),(2,2),(3,1)] stored as [1,3,2,2,3,1]

// Encode run-length: output is pairs (value, count)
// Returns number of pairs
int rle_encode_int(int *input, int in_len, int *values, int *counts) {
    if (in_len == 0) return 0;

    int pairs = 0;
    int i = 0;
    while (i < in_len) {
        int val = input[i];
        int count = 0;
        while (i < in_len && input[i] == val) {
            count++;
            i++;
        }
        values[pairs] = val;
        counts[pairs] = count;
        pairs++;
    }
    return pairs;
}

// Decode run-length encoding back to original
int rle_decode_int(int *values, int *counts, int pairs, int *output) {
    int out_idx = 0;
    int i;
    int j;
    for (i = 0; i < pairs; i++) {
        for (j = 0; j < counts[i]; j++) {
            output[out_idx] = values[i];
            out_idx++;
        }
    }
    return out_idx;
}

// Compute compression ratio (in_len vs 2*pairs)
// Returns 1 if compressed is smaller, 0 otherwise
int rle_is_beneficial(int in_len, int pairs) {
    return (2 * pairs) < in_len;
}

// Find the most frequent run value
int rle_most_frequent(int *values, int *counts, int pairs) {
    int max_count = 0;
    int max_val = 0;
    int i;
    for (i = 0; i < pairs; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_val = values[i];
        }
    }
    return max_val;
}

// Find the longest run length
int rle_longest_run(int *counts, int pairs) {
    int max_count = 0;
    int i;
    for (i = 0; i < pairs; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
        }
    }
    return max_count;
}

// ============================================================
// 3. Delta Encoding/Decoding
// ============================================================
// Delta encoding stores the first value, then differences between consecutive values
// [10, 12, 15, 14, 20] -> [10, 2, 3, -1, 6]

// Encode using delta encoding
void delta_encode(int *input, int in_len, int *output) {
    if (in_len == 0) return;
    output[0] = input[0]; // First value stored as-is
    int i;
    for (i = 1; i < in_len; i++) {
        output[i] = input[i] - input[i - 1];
    }
}

// Decode delta encoding back to original
void delta_decode(int *input, int in_len, int *output) {
    if (in_len == 0) return;
    output[0] = input[0];
    int i;
    for (i = 1; i < in_len; i++) {
        output[i] = output[i - 1] + input[i];
    }
}

// Double delta encoding (delta of delta)
void double_delta_encode(int *input, int in_len, int *output) {
    int delta1[100];
    delta_encode(input, in_len, delta1);
    delta_encode(delta1, in_len, output);
}

// Double delta decoding
void double_delta_decode(int *input, int in_len, int *output) {
    int delta1[100];
    delta_decode(input, in_len, delta1);
    delta_decode(delta1, in_len, output);
}

// Check if delta encoding is beneficial (smaller absolute values)
int delta_benefit(int *original, int *delta, int n) {
    int orig_sum = 0;
    int delta_sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        int ov = original[i];
        if (ov < 0) ov = -ov;
        orig_sum = orig_sum + ov;

        int dv = delta[i];
        if (dv < 0) dv = -dv;
        delta_sum = delta_sum + dv;
    }
    return delta_sum < orig_sum;
}

// ============================================================
// 4. Zigzag Encoding/Decoding
// ============================================================
// Maps signed integers to unsigned:
//   0 -> 0, -1 -> 1, 1 -> 2, -2 -> 3, 2 -> 4, ...
// Formula: encode(n) = (n << 1) ^ (n >> 31) for 32-bit
// Simplified for small values:

int zigzag_encode(int n) {
    // Map signed to unsigned: (n * 2) if n >= 0, (-n * 2 - 1) if n < 0
    if (n >= 0) {
        return n * 2;
    } else {
        return (-n) * 2 - 1;
    }
}

int zigzag_decode(int n) {
    // Reverse mapping
    if (n % 2 == 0) {
        return n / 2;
    } else {
        return -(n + 1) / 2;
    }
}

// Encode an array using zigzag encoding
void zigzag_encode_array(int *input, int n, int *output) {
    int i;
    for (i = 0; i < n; i++) {
        output[i] = zigzag_encode(input[i]);
    }
}

// Decode an array using zigzag decoding
void zigzag_decode_array(int *input, int n, int *output) {
    int i;
    for (i = 0; i < n; i++) {
        output[i] = zigzag_decode(input[i]);
    }
}

// Combined delta + zigzag encoding
void delta_zigzag_encode(int *input, int n, int *output) {
    int delta[100];
    delta_encode(input, n, delta);
    zigzag_encode_array(delta, n, output);
}

// Combined zigzag + delta decoding
void delta_zigzag_decode(int *input, int n, int *output) {
    int delta[100];
    zigzag_decode_array(input, n, delta);
    delta_decode(delta, n, output);
}

// ============================================================
// 5. Variable-Length Quantity (VLQ-like) Encoding
// ============================================================
// Encode integers using variable number of 7-bit groups
// Each group has a continuation bit (MSB)
// This is a simplified version working with small positive integers

// Encode a single value into VLQ bytes (stored as ints)
// Returns number of bytes used
int vlq_encode_single(int value, int *output) {
    if (value < 0) value = -value; // Handle only positive for simplicity

    if (value == 0) {
        output[0] = 0;
        return 1;
    }

    // Extract 7-bit groups
    int groups[10];
    int num_groups = 0;
    int v = value;
    while (v > 0) {
        groups[num_groups] = v % 128;
        v = v / 128;
        num_groups++;
    }

    // Output in big-endian order with continuation bits
    int i;
    for (i = num_groups - 1; i >= 0; i--) {
        if (i > 0) {
            output[num_groups - 1 - i] = groups[i] + 128; // Set continuation bit
        } else {
            output[num_groups - 1 - i] = groups[i]; // Last byte, no continuation
        }
    }
    return num_groups;
}

// Decode a single VLQ value
// Returns the decoded value
int vlq_decode_single(int *input, int *bytes_consumed) {
    int result = 0;
    int idx = 0;
    int cont = 1;
    while (cont) {
        int byte_val = input[idx];
        if (byte_val >= 128) {
            result = result * 128 + (byte_val - 128);
            cont = 1;
        } else {
            result = result * 128 + byte_val;
            cont = 0;
        }
        idx++;
    }
    *bytes_consumed = idx;
    return result;
}

// Encode an array of values using VLQ
int vlq_encode_array(int *input, int n, int *output) {
    int out_idx = 0;
    int i;
    for (i = 0; i < n; i++) {
        int bytes = vlq_encode_single(input[i], output + out_idx);
        out_idx = out_idx + bytes;
    }
    return out_idx;
}

// Decode a VLQ-encoded array
int vlq_decode_array(int *input, int total_bytes, int *output) {
    int in_idx = 0;
    int out_idx = 0;
    while (in_idx < total_bytes) {
        int consumed;
        output[out_idx] = vlq_decode_single(input + in_idx, &consumed);
        in_idx = in_idx + consumed;
        out_idx++;
    }
    return out_idx;
}

// ============================================================
// Additional: Gray code encoding/decoding
// ============================================================
int gray_encode(int n) {
    // Gray code: n XOR (n >> 1)
    // Since we can't use XOR directly, simulate it
    int half = n / 2;
    // XOR: for each bit, result is 1 if bits differ
    int result = 0;
    int bit = 1;
    while (n > 0 || half > 0) {
        int b1 = n % 2;
        int b2 = half % 2;
        if (b1 != b2) {
            result = result + bit;
        }
        bit = bit * 2;
        n = n / 2;
        half = half / 2;
    }
    return result;
}

int gray_decode(int g) {
    int result = 0;
    int mask = g;
    while (mask > 0) {
        // XOR result with mask
        int r = result;
        int m = mask;
        int new_result = 0;
        int bit = 1;
        while (r > 0 || m > 0) {
            int b1 = r % 2;
            int b2 = m % 2;
            if (b1 != b2) {
                new_result = new_result + bit;
            }
            bit = bit * 2;
            r = r / 2;
            m = m / 2;
        }
        result = new_result;
        mask = mask / 2;
    }
    return result;
}

// ============================================================
// Test functions
// ============================================================

void test_base64() {
    printf("=== Base64-like Encoding Tests ===\n");

    // Test 1: Encode simple values
    int input1[3];
    input1[0] = 65;  // 'A'
    input1[1] = 66;  // 'B'
    input1[2] = 67;  // 'C'

    int encoded[8];
    int enc_len = b64_encode(input1, 3, encoded);
    printf("B64 enc_len: %d\n", enc_len);
    printf("B64 encoded: ");
    print_array(encoded, enc_len);

    // Decode back
    int decoded[6];
    int dec_len = b64_decode(encoded, enc_len, decoded);
    printf("B64 decoded: ");
    print_array(decoded, 3); // Only first 3 are meaningful
    printf("B64 roundtrip: %d\n", arrays_equal(input1, decoded, 3));

    // Test 2: Encode 6 bytes (produces 8 encoded values)
    int input2[6];
    input2[0] = 0; input2[1] = 128; input2[2] = 255;
    input2[3] = 100; input2[4] = 200; input2[5] = 50;

    int encoded2[12];
    int enc_len2 = b64_encode(input2, 6, encoded2);
    printf("B64 enc_len2: %d\n", enc_len2);

    int decoded2[12];
    b64_decode(encoded2, enc_len2, decoded2);
    printf("B64 roundtrip2: %d\n", arrays_equal(input2, decoded2, 6));

    // Test 3: Single byte
    int input3[1];
    input3[0] = 42;
    int encoded3[4];
    int enc_len3 = b64_encode(input3, 1, encoded3);
    printf("B64 single_enc_len: %d\n", enc_len3);

    int decoded3[3];
    b64_decode(encoded3, enc_len3, decoded3);
    printf("B64 single_decoded: %d\n", decoded3[0]);

    // Test 4: All zeros
    int input4[3];
    input4[0] = 0; input4[1] = 0; input4[2] = 0;
    int encoded4[4];
    b64_encode(input4, 3, encoded4);
    printf("B64 zeros: ");
    print_array(encoded4, 4);
}

void test_rle_int() {
    printf("=== RLE Integer Tests ===\n");

    // Test 1: Simple run-length
    int input1[12];
    input1[0] = 1; input1[1] = 1; input1[2] = 1;
    input1[3] = 2; input1[4] = 2;
    input1[5] = 3;
    input1[6] = 4; input1[7] = 4; input1[8] = 4; input1[9] = 4;
    input1[10] = 5; input1[11] = 5;

    int values[20];
    int counts[20];
    int pairs = rle_encode_int(input1, 12, values, counts);
    printf("RLE1 pairs: %d\n", pairs);
    int i;
    for (i = 0; i < pairs; i++) {
        printf("RLE1 val=%d count=%d\n", values[i], counts[i]);
    }
    printf("RLE1 beneficial: %d\n", rle_is_beneficial(12, pairs));
    printf("RLE1 longest_run: %d\n", rle_longest_run(counts, pairs));
    printf("RLE1 most_freq: %d\n", rle_most_frequent(values, counts, pairs));

    // Decode and verify
    int decoded[20];
    int dec_len = rle_decode_int(values, counts, pairs, decoded);
    printf("RLE1 dec_len: %d\n", dec_len);
    printf("RLE1 roundtrip: %d\n", arrays_equal(input1, decoded, 12));

    // Test 2: No runs
    int input2[5];
    input2[0] = 1; input2[1] = 2; input2[2] = 3; input2[3] = 4; input2[4] = 5;
    int pairs2 = rle_encode_int(input2, 5, values, counts);
    printf("RLE2 pairs: %d\n", pairs2);
    printf("RLE2 beneficial: %d\n", rle_is_beneficial(5, pairs2));

    // Test 3: All same
    int input3[8];
    for (i = 0; i < 8; i++) input3[i] = 7;
    int pairs3 = rle_encode_int(input3, 8, values, counts);
    printf("RLE3 pairs: %d\n", pairs3);
    printf("RLE3 val=%d count=%d\n", values[0], counts[0]);
    printf("RLE3 beneficial: %d\n", rle_is_beneficial(8, pairs3));
}

void test_delta() {
    printf("=== Delta Encoding Tests ===\n");

    // Test 1: Monotonically increasing sequence
    int input1[6];
    input1[0] = 10; input1[1] = 12; input1[2] = 15;
    input1[3] = 14; input1[4] = 20; input1[5] = 25;

    int delta1[6];
    delta_encode(input1, 6, delta1);
    printf("Delta1: ");
    print_array(delta1, 6);

    int restored1[6];
    delta_decode(delta1, 6, restored1);
    printf("Delta1 roundtrip: %d\n", arrays_equal(input1, restored1, 6));
    printf("Delta1 beneficial: %d\n", delta_benefit(input1, delta1, 6));

    // Test 2: Timestamps (linearly increasing)
    int input2[5];
    input2[0] = 1000; input2[1] = 1010; input2[2] = 1020;
    input2[3] = 1030; input2[4] = 1040;

    int delta2[5];
    delta_encode(input2, 5, delta2);
    printf("Delta2: ");
    print_array(delta2, 5);
    printf("Delta2 beneficial: %d\n", delta_benefit(input2, delta2, 5));

    // Test 3: Double delta on linear sequence
    int dd3[5];
    double_delta_encode(input2, 5, dd3);
    printf("DDelta3: ");
    print_array(dd3, 5);

    int restored3[5];
    double_delta_decode(dd3, 5, restored3);
    printf("DDelta3 roundtrip: %d\n", arrays_equal(input2, restored3, 5));

    // Test 4: Constant sequence
    int input4[4];
    input4[0] = 5; input4[1] = 5; input4[2] = 5; input4[3] = 5;
    int delta4[4];
    delta_encode(input4, 4, delta4);
    printf("Delta4: ");
    print_array(delta4, 4);

    // Test 5: Decreasing sequence
    int input5[5];
    input5[0] = 100; input5[1] = 90; input5[2] = 75;
    input5[3] = 50; input5[4] = 20;
    int delta5[5];
    delta_encode(input5, 5, delta5);
    printf("Delta5: ");
    print_array(delta5, 5);

    int restored5[5];
    delta_decode(delta5, 5, restored5);
    printf("Delta5 roundtrip: %d\n", arrays_equal(input5, restored5, 5));
}

void test_zigzag() {
    printf("=== Zigzag Encoding Tests ===\n");

    // Test individual values
    printf("ZZ encode 0: %d\n", zigzag_encode(0));
    printf("ZZ encode -1: %d\n", zigzag_encode(-1));
    printf("ZZ encode 1: %d\n", zigzag_encode(1));
    printf("ZZ encode -2: %d\n", zigzag_encode(-2));
    printf("ZZ encode 2: %d\n", zigzag_encode(2));
    printf("ZZ encode -100: %d\n", zigzag_encode(-100));
    printf("ZZ encode 100: %d\n", zigzag_encode(100));

    // Test decode
    printf("ZZ decode 0: %d\n", zigzag_decode(0));
    printf("ZZ decode 1: %d\n", zigzag_decode(1));
    printf("ZZ decode 2: %d\n", zigzag_decode(2));
    printf("ZZ decode 3: %d\n", zigzag_decode(3));
    printf("ZZ decode 4: %d\n", zigzag_decode(4));

    // Test roundtrip
    int input[6];
    input[0] = -5; input[1] = 3; input[2] = -10;
    input[3] = 0; input[4] = 7; input[5] = -1;

    int encoded[6];
    zigzag_encode_array(input, 6, encoded);
    printf("ZZ encoded: ");
    print_array(encoded, 6);

    int decoded[6];
    zigzag_decode_array(encoded, 6, decoded);
    printf("ZZ roundtrip: %d\n", arrays_equal(input, decoded, 6));

    // Test combined delta + zigzag
    int data[5];
    data[0] = 100; data[1] = 102; data[2] = 99;
    data[3] = 105; data[4] = 101;

    int dz_encoded[5];
    delta_zigzag_encode(data, 5, dz_encoded);
    printf("DZ encoded: ");
    print_array(dz_encoded, 5);

    int dz_decoded[5];
    delta_zigzag_decode(dz_encoded, 5, dz_decoded);
    printf("DZ roundtrip: %d\n", arrays_equal(data, dz_decoded, 5));
}

void test_vlq() {
    printf("=== VLQ Encoding Tests ===\n");

    // Test single values
    int output[10];
    int bytes;

    bytes = vlq_encode_single(0, output);
    printf("VLQ 0: %d bytes, val=%d\n", bytes, output[0]);

    bytes = vlq_encode_single(127, output);
    printf("VLQ 127: %d bytes, val=%d\n", bytes, output[0]);

    bytes = vlq_encode_single(128, output);
    printf("VLQ 128: %d bytes, val0=%d val1=%d\n", bytes, output[0], output[1]);

    bytes = vlq_encode_single(16383, output);
    printf("VLQ 16383: %d bytes\n", bytes);

    bytes = vlq_encode_single(16384, output);
    printf("VLQ 16384: %d bytes\n", bytes);

    // Test decode
    int consumed;
    int input_vlq[4];

    input_vlq[0] = 42;
    int val = vlq_decode_single(input_vlq, &consumed);
    printf("VLQ decode 42: val=%d consumed=%d\n", val, consumed);

    input_vlq[0] = 129; // 128 + 1 = continuation bit + 1
    input_vlq[1] = 0;   // 0 with no continuation
    val = vlq_decode_single(input_vlq, &consumed);
    printf("VLQ decode [129,0]: val=%d consumed=%d\n", val, consumed);

    // Test array encoding
    int arr[4];
    arr[0] = 10; arr[1] = 200; arr[2] = 5; arr[3] = 1000;

    int vlq_out[20];
    int total_bytes = vlq_encode_array(arr, 4, vlq_out);
    printf("VLQ array total_bytes: %d\n", total_bytes);

    int vlq_decoded[10];
    int decoded_count = vlq_decode_array(vlq_out, total_bytes, vlq_decoded);
    printf("VLQ array decoded_count: %d\n", decoded_count);
    printf("VLQ array roundtrip: %d\n", arrays_equal(arr, vlq_decoded, 4));
}

void test_gray_code() {
    printf("=== Gray Code Tests ===\n");

    // Test encoding 0-15
    int i;
    for (i = 0; i < 16; i++) {
        int g = gray_encode(i);
        int d = gray_decode(g);
        printf("Gray %d -> %d -> %d\n", i, g, d);
    }

    // Check property: adjacent gray codes differ by 1 bit
    int all_single_bit = 1;
    for (i = 0; i < 15; i++) {
        int g1 = gray_encode(i);
        int g2 = gray_encode(i + 1);
        // XOR g1 and g2, check if result is power of 2
        int diff = 0;
        int a = g1;
        int b = g2;
        int bit = 1;
        while (a > 0 || b > 0) {
            int b1 = a % 2;
            int b2 = b % 2;
            if (b1 != b2) diff = diff + bit;
            bit = bit * 2;
            a = a / 2;
            b = b / 2;
        }
        // Check if diff is a power of 2
        int is_pow2 = (diff > 0);
        int tmp = diff;
        while (tmp > 1) {
            if (tmp % 2 != 0) { is_pow2 = 0; break; }
            tmp = tmp / 2;
        }
        if (!is_pow2) all_single_bit = 0;
    }
    printf("Gray adjacent_single_bit: %d\n", all_single_bit);
}


int main() {
    test_base64();
    test_rle_int();
    test_delta();
    test_zigzag();
    test_vlq();
    test_gray_code();
    return 0;
}
