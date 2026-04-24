int printf(const char *fmt, ...);

// Test 073: Huffman-like frequency coding - build frequency table and assign codes

void count_frequencies(char *data, int len, int *freq) {
    int i;
    for (i = 0; i < 128; i = i + 1) {
        freq[i] = 0;
    }
    for (i = 0; i < len; i = i + 1) {
        int c = data[i];
        if (c >= 0 && c < 128) {
            freq[c] = freq[c] + 1;
        }
    }
}

// Simple sort: find top N most frequent characters
void find_top_n(int *freq, int *top_chars, int *top_freqs, int n) {
    int used[128];
    int i;
    for (i = 0; i < 128; i = i + 1) used[i] = 0;

    int k;
    for (k = 0; k < n; k = k + 1) {
        int best = -1;
        int best_freq = -1;
        for (i = 0; i < 128; i = i + 1) {
            if (!used[i] && freq[i] > best_freq) {
                best = i;
                best_freq = freq[i];
            }
        }
        if (best == -1) {
            top_chars[k] = -1;
            top_freqs[k] = 0;
        } else {
            top_chars[k] = best;
            top_freqs[k] = best_freq;
            used[best] = 1;
        }
    }
}

// Assign bit lengths based on frequency rank (simulated Huffman)
// Rank 0 gets 2 bits, rank 1 gets 2 bits, rank 2-3 get 3 bits, rest get 4+ bits
int assign_code_length(int rank, int total_symbols) {
    if (rank < 2) return 2;
    if (rank < 4) return 3;
    if (rank < 8) return 4;
    return 5;
}

// Estimate compressed size in bits
int estimate_size(int *freq, int *top_chars, int n) {
    int total_bits = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (top_chars[i] == -1) break;
        int bits = assign_code_length(i, n);
        total_bits = total_bits + freq[top_chars[i]] * bits;
    }
    return total_bits;
}

int count_unique(int *freq) {
    int count = 0;
    int i;
    for (i = 0; i < 128; i = i + 1) {
        if (freq[i] > 0) count = count + 1;
    }
    return count;
}

int main() {
    char data[30];
    // "AAAAABBBCCD" = A:5 B:3 C:2 D:1
    data[0] = 'A'; data[1] = 'A'; data[2] = 'A'; data[3] = 'A'; data[4] = 'A';
    data[5] = 'B'; data[6] = 'B'; data[7] = 'B';
    data[8] = 'C'; data[9] = 'C';
    data[10] = 'D';
    int len = 11;

    int freq[128];
    count_frequencies(data, len, freq);

    printf("Freq A: %d\n", freq[65]); // EXPECT: Freq A: 5
    printf("Freq B: %d\n", freq[66]); // EXPECT: Freq B: 3
    printf("Freq C: %d\n", freq[67]); // EXPECT: Freq C: 2
    printf("Freq D: %d\n", freq[68]); // EXPECT: Freq D: 1

    int unique = count_unique(freq);
    printf("Unique chars: %d\n", unique); // EXPECT: Unique chars: 4

    int top_chars[8];
    int top_freqs[8];
    find_top_n(freq, top_chars, top_freqs, 4);

    printf("Top 0: char=%d freq=%d\n", top_chars[0], top_freqs[0]); // EXPECT: Top 0: char=65 freq=5
    printf("Top 1: char=%d freq=%d\n", top_chars[1], top_freqs[1]); // EXPECT: Top 1: char=66 freq=3
    printf("Top 2: char=%d freq=%d\n", top_chars[2], top_freqs[2]); // EXPECT: Top 2: char=67 freq=2
    printf("Top 3: char=%d freq=%d\n", top_chars[3], top_freqs[3]); // EXPECT: Top 3: char=68 freq=1

    // Code lengths: A=2, B=2, C=3, D=3
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int bits = assign_code_length(i, 4);
        printf("Rank %d: %d bits\n", i, bits); // EXPECT: Rank 0: 2 bits
        // EXPECT: Rank 1: 2 bits
        // EXPECT: Rank 2: 3 bits
        // EXPECT: Rank 3: 3 bits
    }

    int est_bits = estimate_size(freq, top_chars, 4);
    printf("Estimated bits: %d\n", est_bits); // EXPECT: Estimated bits: 25

    int original_bits = len * 8;
    printf("Original bits: %d\n", original_bits); // EXPECT: Original bits: 88

    int savings = original_bits - est_bits;
    printf("Bits saved: %d\n", savings); // EXPECT: Bits saved: 63

    int pct = (savings * 100) / original_bits;
    printf("Savings pct: %d\n", pct); // EXPECT: Savings pct: 71

    // Test with uniform distribution
    char data2[20];
    for (i = 0; i < 20; i = i + 1) {
        data2[i] = 'A' + (i % 5);
    }
    count_frequencies(data2, 20, freq);
    int unique2 = count_unique(freq);
    printf("Uniform unique: %d\n", unique2); // EXPECT: Uniform unique: 5

    find_top_n(freq, top_chars, top_freqs, 5);
    int all_same = 1;
    for (i = 1; i < 5; i = i + 1) {
        if (top_freqs[i] != top_freqs[0]) all_same = 0;
    }
    printf("Uniform freqs equal: %d\n", all_same); // EXPECT: Uniform freqs equal: 1
    printf("Each freq: %d\n", top_freqs[0]); // EXPECT: Each freq: 4

    printf("Done\n"); // EXPECT: Done
    return 0;
}
