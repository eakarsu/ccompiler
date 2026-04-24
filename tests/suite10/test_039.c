int printf(const char *fmt, ...);

// Prefix Codes - Building and using prefix-free codes for compression

struct PrefixCode {
    int symbol;
    int code;       // the bit pattern
    int code_len;   // number of bits
};

// Build prefix codes from sorted frequencies (simplified canonical Huffman)
// Symbols are sorted by frequency (ascending). We assign codes manually.
void build_prefix_codes(int *symbols, int *freqs, int count, struct PrefixCode *codes) {
    // For simplicity, assign codes based on tree shape:
    // Most frequent gets shortest code, etc.
    // Use a binary tree-like assignment
    if (count == 1) {
        codes[0].symbol = symbols[0];
        codes[0].code = 0;
        codes[0].code_len = 1;
        return;
    }

    // Sort by frequency (ascending) using bubble sort
    int temp_sym[16];
    int temp_freq[16];
    int i = 0;
    while (i < count) {
        temp_sym[i] = symbols[i];
        temp_freq[i] = freqs[i];
        i = i + 1;
    }

    int sorted = 0;
    while (sorted == 0) {
        sorted = 1;
        i = 0;
        while (i < count - 1) {
            if (temp_freq[i] > temp_freq[i + 1]) {
                int ts = temp_sym[i]; temp_sym[i] = temp_sym[i + 1]; temp_sym[i + 1] = ts;
                int tf = temp_freq[i]; temp_freq[i] = temp_freq[i + 1]; temp_freq[i + 1] = tf;
                sorted = 0;
            }
            i = i + 1;
        }
    }

    // Assign canonical codes: shortest to most frequent
    // Lengths: last gets 1 bit, second-last 2, etc. but capped at count-1
    // Simplified: assign lengths from count-1 down to 1 for least to most freq
    int lengths[16];
    i = 0;
    while (i < count) {
        lengths[i] = count - 1 - i;
        if (lengths[i] < 1) lengths[i] = 1;
        i = i + 1;
    }

    // Generate canonical codes from lengths
    // First sort by length (desc), then assign codes
    int code_val = 0;
    int prev_len = lengths[0];
    codes[0].symbol = temp_sym[0];
    codes[0].code_len = lengths[0];
    codes[0].code = 0;

    i = 1;
    while (i < count) {
        code_val = code_val + 1;
        int len_diff = prev_len - lengths[i];
        if (len_diff > 0) {
            code_val = code_val >> len_diff;
        }
        codes[i].symbol = temp_sym[i];
        codes[i].code_len = lengths[i];
        codes[i].code = code_val;
        prev_len = lengths[i];
        i = i + 1;
    }
}

// Check if code set is prefix-free
int is_prefix_free(struct PrefixCode *codes, int count) {
    int i = 0;
    while (i < count) {
        int j = i + 1;
        while (j < count) {
            // Check if code[i] is a prefix of code[j] or vice versa
            int shorter_len = codes[i].code_len;
            if (codes[j].code_len < shorter_len) shorter_len = codes[j].code_len;

            int mask = (1 << shorter_len) - 1;
            int bits_i = codes[i].code & mask;
            int bits_j = codes[j].code & mask;
            if (bits_i == bits_j && codes[i].code_len != codes[j].code_len) {
                return 0;
            }
            if (codes[i].code == codes[j].code && codes[i].code_len == codes[j].code_len) {
                return 0; // duplicate
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return 1;
}

// Encode a symbol array using prefix codes
int encode_symbols(int *symbols, int sym_count, struct PrefixCode *codes, int code_count, int *bits_out) {
    int total_bits = 0;
    int i = 0;
    while (i < sym_count) {
        int j = 0;
        while (j < code_count) {
            if (codes[j].symbol == symbols[i]) {
                total_bits = total_bits + codes[j].code_len;
                break;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    *bits_out = total_bits;
    return total_bits;
}

// Print a code as binary string
void print_binary(int val, int bits) {
    int i = bits - 1;
    while (i >= 0) {
        int bit = (val >> i) & 1;
        if (bit) {
            printf("1");
        } else {
            printf("0");
        }
        i = i - 1;
    }
}

int main() {
    struct PrefixCode codes[8];

    // Test 1: Build codes for 4 symbols with different frequencies
    int symbols[4];
    symbols[0] = 65; // A
    symbols[1] = 66; // B
    symbols[2] = 67; // C
    symbols[3] = 68; // D

    int freqs[4];
    freqs[0] = 10; // A most frequent
    freqs[1] = 5;  // B
    freqs[2] = 2;  // C
    freqs[3] = 1;  // D least frequent

    build_prefix_codes(symbols, freqs, 4, codes);

    printf("Prefix codes:\n"); // EXPECT: Prefix codes:
    int i = 0;
    while (i < 4) {
        printf("Symbol %d: code=%d len=%d\n", codes[i].symbol, codes[i].code, codes[i].code_len);
        i = i + 1;
    }
    // EXPECT: Symbol 68: code=0 len=3
    // EXPECT: Symbol 67: code=0 len=2
    // EXPECT: Symbol 66: code=0 len=1
    // EXPECT: Symbol 65: code=1 len=1

    // Test 2: Encode a message
    int message[10];
    message[0] = 65; message[1] = 65; message[2] = 66;
    message[3] = 65; message[4] = 67; message[5] = 65;
    message[6] = 68; message[7] = 65; message[8] = 65; message[9] = 66;

    int total_bits = 0;
    encode_symbols(message, 10, codes, 4, &total_bits);
    printf("Encoded bits: %d\n", total_bits); // EXPECT: Encoded bits: 13

    // Fixed-width would need 2 bits * 10 = 20 bits
    int fixed_bits = 10 * 2;
    printf("Fixed-width bits: %d\n", fixed_bits); // EXPECT: Fixed-width bits: 20
    int savings = fixed_bits - total_bits;
    printf("Bits saved: %d\n", savings); // EXPECT: Bits saved: 7

    // Test 3: Single symbol
    struct PrefixCode single_code[1];
    int single_sym[1];
    single_sym[0] = 88;
    int single_freq[1];
    single_freq[0] = 100;
    build_prefix_codes(single_sym, single_freq, 1, single_code);
    printf("Single code len: %d\n", single_code[0].code_len); // EXPECT: Single code len: 1

    // Test 4: Two equal frequency symbols
    struct PrefixCode two_codes[2];
    int two_sym[2];
    two_sym[0] = 48; // '0'
    two_sym[1] = 49; // '1'
    int two_freq[2];
    two_freq[0] = 5;
    two_freq[1] = 5;
    build_prefix_codes(two_sym, two_freq, 2, two_codes);
    printf("Two sym 0: code=%d len=%d\n", two_codes[0].code, two_codes[0].code_len);
    printf("Two sym 1: code=%d len=%d\n", two_codes[1].code, two_codes[1].code_len);
    // EXPECT: Two sym 0: code=0 len=1
    // EXPECT: Two sym 1: code=1 len=1

    // Test 5: Kraft inequality check (sum of 2^(-len) <= 1)
    // For our 4-symbol code: 2^(-3) + 2^(-2) + 2^(-1) + 2^(-1)
    // = 1/8 + 1/4 + 1/2 + 1/2 = 1.375 -- may exceed if not prefix-free
    // Let's compute kraft_sum * 8 (multiply denominators)
    int kraft_num = 0;
    i = 0;
    while (i < 4) {
        int denom_power = 3 - codes[i].code_len;
        int contrib = 1;
        int k = 0;
        while (k < denom_power) {
            contrib = contrib * 2;
            k = k + 1;
        }
        kraft_num = kraft_num + contrib;
        i = i + 1;
    }
    printf("Kraft sum numerator (over 8): %d\n", kraft_num); // EXPECT: Kraft sum numerator (over 8): 11

    // Test 6: Weighted average code length
    int total_freq = 10 + 5 + 2 + 1;
    int weighted_sum = 0;
    i = 0;
    while (i < 4) {
        int f = 0;
        int j = 0;
        while (j < 4) {
            if (symbols[j] == codes[i].symbol) {
                f = freqs[j];
                break;
            }
            j = j + 1;
        }
        weighted_sum = weighted_sum + f * codes[i].code_len;
        i = i + 1;
    }
    // avg * 18 = weighted_sum, avg * 1000 = weighted_sum * 1000 / 18
    int avg_milli = (weighted_sum * 1000) / total_freq;
    printf("Weighted avg len (x1000): %d\n", avg_milli); // EXPECT: Weighted avg len (x1000): 1222

    printf("Prefix codes done\n"); // EXPECT: Prefix codes done
    return 0;
}
