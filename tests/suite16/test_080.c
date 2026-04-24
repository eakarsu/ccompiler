int printf(const char *fmt, ...);

// Test 080: Entropy estimation and compression bound analysis

// Count character frequencies
void count_freq(int *data, int len, int *freq, int alpha_size) {
    int i;
    for (i = 0; i < alpha_size; i = i + 1) freq[i] = 0;
    for (i = 0; i < len; i = i + 1) {
        if (data[i] >= 0 && data[i] < alpha_size) {
            freq[data[i]] = freq[data[i]] + 1;
        }
    }
}

// Integer log2 (floor)
int int_log2(int n) {
    if (n <= 1) return 0;
    int bits = 0;
    int v = n;
    while (v > 1) {
        v = v / 2;
        bits = bits + 1;
    }
    return bits;
}

// Count number of unique symbols
int count_unique(int *freq, int alpha_size) {
    int c = 0;
    int i;
    for (i = 0; i < alpha_size; i = i + 1) {
        if (freq[i] > 0) c = c + 1;
    }
    return c;
}

// Naive encoding bits (fixed-width codes for unique symbols)
int naive_bits(int len, int unique) {
    int bits_per = int_log2(unique);
    if ((1 << bits_per) < unique) bits_per = bits_per + 1;
    if (unique <= 1) bits_per = 1;
    return len * bits_per;
}

// Estimate total bits using Shannon entropy formula (integer approximation)
// For each symbol, bits = -log2(freq/total) = log2(total) - log2(freq)
// Total bits = sum(freq[i] * (log2(total) - log2(freq[i])))
int entropy_total_bits(int *freq, int alpha_size, int len) {
    if (len <= 1) return 0;
    int total = 0;
    int log_len = int_log2(len);
    int i;
    for (i = 0; i < alpha_size; i = i + 1) {
        if (freq[i] > 1) {
            int log_f = int_log2(freq[i]);
            total = total + freq[i] * (log_len - log_f);
        } else if (freq[i] == 1) {
            total = total + log_len;
        }
    }
    return total;
}

// Compute max possible total bits (uniform distribution)
int max_entropy_bits(int len, int unique) {
    if (unique <= 1) return 0;
    return len * int_log2(unique);
}

int main() {
    // Test 1: Highly skewed distribution (very compressible)
    int data1[16];
    int i;
    // 12 A's, 2 B's, 1 C, 1 D
    for (i = 0; i < 12; i = i + 1) data1[i] = 0;
    data1[12] = 1; data1[13] = 1;
    data1[14] = 2; data1[15] = 3;

    int freq[256];
    count_freq(data1, 16, freq, 256);

    printf("Freq[0]: %d\n", freq[0]); // EXPECT: Freq[0]: 12
    printf("Freq[1]: %d\n", freq[1]); // EXPECT: Freq[1]: 2
    printf("Freq[2]: %d\n", freq[2]); // EXPECT: Freq[2]: 1
    printf("Freq[3]: %d\n", freq[3]); // EXPECT: Freq[3]: 1

    int unique1 = count_unique(freq, 256);
    printf("Unique symbols: %d\n", unique1); // EXPECT: Unique symbols: 4

    int ent_bits1 = entropy_total_bits(freq, 256, 16);
    printf("Entropy bits: %d\n", ent_bits1); // EXPECT: Entropy bits: 26

    int naive1 = naive_bits(16, unique1);
    printf("Naive bits: %d\n", naive1); // EXPECT: Naive bits: 32

    int orig_bits = 16 * 8;
    printf("Original bits: %d\n", orig_bits); // EXPECT: Original bits: 128

    int savings1 = naive1 - ent_bits1;
    printf("Savings over naive: %d\n", savings1); // EXPECT: Savings over naive: 6

    // Test 2: Uniform distribution (least compressible)
    int data2[16];
    for (i = 0; i < 16; i = i + 1) data2[i] = i % 4;
    count_freq(data2, 16, freq, 256);

    int unique2 = count_unique(freq, 256);
    printf("Uniform unique: %d\n", unique2); // EXPECT: Uniform unique: 4

    int ent_bits2 = entropy_total_bits(freq, 256, 16);
    printf("Uniform entropy bits: %d\n", ent_bits2); // EXPECT: Uniform entropy bits: 32

    int naive2 = naive_bits(16, unique2);
    printf("Uniform naive bits: %d\n", naive2); // EXPECT: Uniform naive bits: 32

    int savings2 = naive2 - ent_bits2;
    printf("Uniform savings: %d\n", savings2); // EXPECT: Uniform savings: 0

    // Test 3: Single symbol (most compressible, entropy = 0)
    int data3[10];
    for (i = 0; i < 10; i = i + 1) data3[i] = 5;
    count_freq(data3, 10, freq, 256);

    int ent_bits3 = entropy_total_bits(freq, 256, 10);
    printf("Single entropy bits: %d\n", ent_bits3); // EXPECT: Single entropy bits: 0

    int unique3 = count_unique(freq, 256);
    printf("Single unique: %d\n", unique3); // EXPECT: Single unique: 1

    // Test 4: Binary data (alphabet size 2, 8:2 split)
    int data4[10];
    data4[0]=0; data4[1]=0; data4[2]=0; data4[3]=0; data4[4]=0;
    data4[5]=0; data4[6]=0; data4[7]=0; data4[8]=1; data4[9]=1;
    count_freq(data4, 10, freq, 256);

    int ent_bits4 = entropy_total_bits(freq, 256, 10);
    printf("Binary entropy bits: %d\n", ent_bits4); // EXPECT: Binary entropy bits: 4

    int unique4 = count_unique(freq, 256);
    printf("Binary unique: %d\n", unique4); // EXPECT: Binary unique: 2

    int naive4 = naive_bits(10, unique4);
    printf("Binary naive bits: %d\n", naive4); // EXPECT: Binary naive bits: 10

    // Test int_log2
    printf("log2(1): %d\n", int_log2(1)); // EXPECT: log2(1): 0
    printf("log2(2): %d\n", int_log2(2)); // EXPECT: log2(2): 1
    printf("log2(4): %d\n", int_log2(4)); // EXPECT: log2(4): 2
    printf("log2(8): %d\n", int_log2(8)); // EXPECT: log2(8): 3
    printf("log2(16): %d\n", int_log2(16)); // EXPECT: log2(16): 4
    printf("log2(100): %d\n", int_log2(100)); // EXPECT: log2(100): 6
    printf("log2(256): %d\n", int_log2(256)); // EXPECT: log2(256): 8

    // Test max entropy
    int max1 = max_entropy_bits(16, 4);
    printf("Max entropy 16x4: %d\n", max1); // EXPECT: Max entropy 16x4: 32

    int max2 = max_entropy_bits(100, 8);
    printf("Max entropy 100x8: %d\n", max2); // EXPECT: Max entropy 100x8: 300

    int max3 = max_entropy_bits(20, 1);
    printf("Max entropy 20x1: %d\n", max3); // EXPECT: Max entropy 20x1: 0

    printf("Done\n"); // EXPECT: Done
    return 0;
}
