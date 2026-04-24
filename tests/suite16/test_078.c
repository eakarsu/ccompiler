int printf(const char *fmt, ...);

// Test 078: Burrows-Wheeler-like transform for compression preprocessing

// Simple BWT-like rotation sort on small strings
// For each rotation, store the starting index
// Sort rotations lexicographically, output last column

int char_at_rotation(char *data, int len, int rot_start, int offset) {
    return data[(rot_start + offset) % len];
}

int compare_rotations(char *data, int len, int a, int b) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        int ca = char_at_rotation(data, len, a, i);
        int cb = char_at_rotation(data, len, b, i);
        if (ca < cb) return -1;
        if (ca > cb) return 1;
    }
    return 0;
}

void sort_rotations(char *data, int len, int *indices) {
    int i;
    for (i = 0; i < len; i = i + 1) indices[i] = i;

    // Bubble sort
    int j;
    for (i = 0; i < len - 1; i = i + 1) {
        for (j = 0; j < len - 1 - i; j = j + 1) {
            if (compare_rotations(data, len, indices[j], indices[j+1]) > 0) {
                int tmp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = tmp;
            }
        }
    }
}

void bwt_transform(char *data, int len, char *output, int *orig_idx) {
    int indices[20];
    sort_rotations(data, len, indices);

    int i;
    *orig_idx = -1;
    for (i = 0; i < len; i = i + 1) {
        // Last character of sorted rotation i
        output[i] = data[(indices[i] + len - 1) % len];
        if (indices[i] == 0) *orig_idx = i;
    }
}

// Count occurrences of each character
void count_chars(char *data, int len, int *counts) {
    int i;
    for (i = 0; i < 128; i = i + 1) counts[i] = 0;
    for (i = 0; i < len; i = i + 1) {
        int c = data[i];
        counts[c] = counts[c] + 1;
    }
}

// Check if BWT output has more runs than original
int count_runs(char *data, int len) {
    if (len == 0) return 0;
    int runs = 1;
    int i;
    for (i = 1; i < len; i = i + 1) {
        if (data[i] != data[i-1]) runs = runs + 1;
    }
    return runs;
}

int main() {
    // Test with "banana$"
    char data1[7];
    data1[0] = 'b'; data1[1] = 'a'; data1[2] = 'n';
    data1[3] = 'a'; data1[4] = 'n'; data1[5] = 'a';
    data1[6] = '$';
    int len1 = 7;

    char bwt_out[20];
    int orig_idx;
    bwt_transform(data1, len1, bwt_out, &orig_idx);

    printf("BWT orig idx: %d\n", orig_idx); // EXPECT: BWT orig idx: 4

    int i;
    for (i = 0; i < len1; i = i + 1) {
        printf("BWT[%d]: %d\n", i, bwt_out[i]); // EXPECT: BWT[0]: 97
        // EXPECT: BWT[1]: 110
        // EXPECT: BWT[2]: 110
        // EXPECT: BWT[3]: 98
        // EXPECT: BWT[4]: 36
        // EXPECT: BWT[5]: 97
        // EXPECT: BWT[6]: 97
    }

    // Character frequencies should be preserved
    int orig_counts[128];
    int bwt_counts[128];
    count_chars(data1, len1, orig_counts);
    count_chars(bwt_out, len1, bwt_counts);

    int freq_match = 1;
    for (i = 0; i < 128; i = i + 1) {
        if (orig_counts[i] != bwt_counts[i]) freq_match = 0;
    }
    printf("Freq preserved: %d\n", freq_match); // EXPECT: Freq preserved: 1

    // Count runs in original vs BWT
    int orig_runs = count_runs(data1, len1);
    int bwt_runs = count_runs(bwt_out, len1);
    printf("Orig runs: %d\n", orig_runs); // EXPECT: Orig runs: 7
    printf("BWT runs: %d\n", bwt_runs); // EXPECT: BWT runs: 5

    // Test with "abcabc"
    char data2[6];
    data2[0] = 'a'; data2[1] = 'b'; data2[2] = 'c';
    data2[3] = 'a'; data2[4] = 'b'; data2[5] = 'c';
    int len2 = 6;

    bwt_transform(data2, len2, bwt_out, &orig_idx);
    printf("BWT2 orig idx: %d\n", orig_idx); // EXPECT: BWT2 orig idx: 0

    for (i = 0; i < len2; i = i + 1) {
        printf("BWT2[%d]: %d\n", i, bwt_out[i]); // EXPECT: BWT2[0]: 99
        // EXPECT: BWT2[1]: 99
        // EXPECT: BWT2[2]: 97
        // EXPECT: BWT2[3]: 97
        // EXPECT: BWT2[4]: 98
        // EXPECT: BWT2[5]: 98
    }

    int runs2 = count_runs(bwt_out, len2);
    printf("BWT2 runs: %d\n", runs2); // EXPECT: BWT2 runs: 3

    int orig_runs2 = count_runs(data2, len2);
    printf("Orig2 runs: %d\n", orig_runs2); // EXPECT: Orig2 runs: 6

    int improvement = orig_runs2 - runs2;
    printf("Run reduction: %d\n", improvement); // EXPECT: Run reduction: 3

    // Test with all same chars
    char data3[4];
    for (i = 0; i < 4; i = i + 1) data3[i] = 'x';
    bwt_transform(data3, 4, bwt_out, &orig_idx);
    int runs3 = count_runs(bwt_out, 4);
    printf("Same char runs: %d\n", runs3); // EXPECT: Same char runs: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
