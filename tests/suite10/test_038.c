int printf(const char *fmt, ...);

// Entropy Calculation (Integer Approximation)
// Uses integer-only log2 approximation and frequency counting

// Integer log2 (floor)
int ilog2(int n) {
    if (n <= 0) return 0;
    int result = 0;
    int v = n;
    while (v > 1) {
        v = v >> 1;
        result = result + 1;
    }
    return result;
}

// Count frequency of each byte in input
void count_freq(char *input, int len, int *freq) {
    int i = 0;
    while (i < 256) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < len) {
        int c = input[i];
        if (c < 0) c = c + 256;
        freq[c] = freq[c] + 1;
        i = i + 1;
    }
}

// Count number of unique symbols
int count_unique(int *freq) {
    int count = 0;
    int i = 0;
    while (i < 256) {
        if (freq[i] > 0) count = count + 1;
        i = i + 1;
    }
    return count;
}

// Approximate entropy in millibits (to avoid floating point)
// H = -sum(p_i * log2(p_i)) bits
// Using p_i = freq_i / total, and log2(p_i) = log2(freq_i) - log2(total)
// H * 1000 in integer approx
int entropy_millibits(int *freq, int total) {
    if (total <= 0) return 0;
    // H = log2(total) - (1/total) * sum(freq_i * log2(freq_i))
    // Multiply by 1000 for millibits
    int log2_total = ilog2(total);
    int sum_flog = 0;
    int i = 0;
    while (i < 256) {
        if (freq[i] > 0) {
            sum_flog = sum_flog + freq[i] * ilog2(freq[i]);
        }
        i = i + 1;
    }
    // H * total = total * log2(total) - sum(freq_i * log2(freq_i))
    int h_total = total * log2_total - sum_flog;
    // H = h_total / total, multiply by 1000 for millibits
    int h_milli = (h_total * 1000) / total;
    return h_milli;
}

// Calculate minimum bits needed to encode the data
int min_encoded_bits(int *freq, int total) {
    int entropy_m = entropy_millibits(freq, total);
    // Total bits = entropy * total / 1000
    return (entropy_m * total + 999) / 1000;
}

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int main() {
    int freq[256];

    // Test 1: ilog2 values
    printf("log2(1) = %d\n", ilog2(1)); // EXPECT: log2(1) = 0
    printf("log2(2) = %d\n", ilog2(2)); // EXPECT: log2(2) = 1
    printf("log2(4) = %d\n", ilog2(4)); // EXPECT: log2(4) = 2
    printf("log2(8) = %d\n", ilog2(8)); // EXPECT: log2(8) = 3
    printf("log2(16) = %d\n", ilog2(16)); // EXPECT: log2(16) = 4
    printf("log2(255) = %d\n", ilog2(255)); // EXPECT: log2(255) = 7
    printf("log2(256) = %d\n", ilog2(256)); // EXPECT: log2(256) = 8

    // Test 2: Uniform distribution (max entropy)
    // 4 symbols each appearing 4 times = 16 total
    // True entropy = log2(4) = 2.0 bits
    char data1[16];
    int i = 0;
    while (i < 4) { data1[i] = 'a'; i = i + 1; }
    while (i < 8) { data1[i] = 'b'; i = i + 1; }
    while (i < 12) { data1[i] = 'c'; i = i + 1; }
    while (i < 16) { data1[i] = 'd'; i = i + 1; }

    count_freq(data1, 16, freq);
    int unique1 = count_unique(freq);
    printf("Unique symbols: %d\n", unique1); // EXPECT: Unique symbols: 4

    int ent1 = entropy_millibits(freq, 16);
    printf("Uniform entropy (millibits): %d\n", ent1); // EXPECT: Uniform entropy (millibits): 2000

    // Test 3: All same (zero entropy)
    char data2[8];
    i = 0;
    while (i < 8) { data2[i] = 'x'; i = i + 1; }

    count_freq(data2, 8, freq);
    int unique2 = count_unique(freq);
    printf("All-same unique: %d\n", unique2); // EXPECT: All-same unique: 1

    int ent2 = entropy_millibits(freq, 8);
    printf("All-same entropy: %d\n", ent2); // EXPECT: All-same entropy: 0

    // Test 4: Skewed distribution
    // 'a' x 6, 'b' x 2 -> total 8
    // True entropy = -(6/8)*log2(6/8) - (2/8)*log2(2/8) ~ 0.81 bits
    char data3[8];
    i = 0;
    while (i < 6) { data3[i] = 'a'; i = i + 1; }
    while (i < 8) { data3[i] = 'b'; i = i + 1; }

    count_freq(data3, 8, freq);
    int ent3 = entropy_millibits(freq, 8);
    printf("Skewed entropy (millibits): %d\n", ent3); // EXPECT: Skewed entropy (millibits): 1250

    // Test 5: Two symbols equal frequency
    // 'a' x 4, 'b' x 4 -> entropy = 1.0 bits
    char data4[8];
    i = 0;
    while (i < 4) { data4[i] = 'a'; i = i + 1; }
    while (i < 8) { data4[i] = 'b'; i = i + 1; }

    count_freq(data4, 8, freq);
    int ent4 = entropy_millibits(freq, 8);
    printf("Binary entropy (millibits): %d\n", ent4); // EXPECT: Binary entropy (millibits): 1000

    // Test 6: Min encoded bits
    count_freq(data1, 16, freq);
    int min_bits_1 = min_encoded_bits(freq, 16);
    printf("Min bits uniform: %d\n", min_bits_1); // EXPECT: Min bits uniform: 32

    count_freq(data2, 8, freq);
    int min_bits_2 = min_encoded_bits(freq, 8);
    printf("Min bits all-same: %d\n", min_bits_2); // EXPECT: Min bits all-same: 0

    // Test 7: Entropy comparison
    int lower = (ent3 < ent1) ? 1 : 0;
    printf("Skewed < uniform: %d\n", lower); // EXPECT: Skewed < uniform: 1

    int zero_lowest = (ent2 < ent3) ? 1 : 0;
    printf("All-same < skewed: %d\n", zero_lowest); // EXPECT: All-same < skewed: 1

    printf("Entropy done\n"); // EXPECT: Entropy done
    return 0;
}
