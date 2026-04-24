int printf(const char *fmt, ...);

// Burrows-Wheeler Transform (BWT)

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

// Compare two rotations of a string
// Rotation i: string starting at position i, wrapping around
int rotation_cmp(char *s, int len, int rot_a, int rot_b) {
    int i = 0;
    while (i < len) {
        int ca = s[(rot_a + i) % len];
        int cb = s[(rot_b + i) % len];
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        i = i + 1;
    }
    return 0;
}

// Sort rotation indices
void sort_rotations(char *s, int len, int *indices) {
    int i = 0;
    while (i < len) {
        indices[i] = i;
        i = i + 1;
    }
    // Bubble sort rotations
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        i = 0;
        while (i < len - 1) {
            if (rotation_cmp(s, len, indices[i], indices[i + 1]) > 0) {
                int tmp = indices[i];
                indices[i] = indices[i + 1];
                indices[i + 1] = tmp;
                swapped = 1;
            }
            i = i + 1;
        }
    }
}

// Perform BWT: returns the index of the original string in sorted rotations
int bwt_encode(char *input, char *output) {
    int len = str_len(input);
    int indices[64];

    sort_rotations(input, len, indices);

    int original_idx = -1;
    int i = 0;
    while (i < len) {
        // Last column: character at position (indices[i] + len - 1) % len
        int last_pos = (indices[i] + len - 1) % len;
        output[i] = input[last_pos];

        if (indices[i] == 0) {
            original_idx = i;
        }
        i = i + 1;
    }
    output[len] = 0;
    return original_idx;
}

// Inverse BWT
void bwt_decode(char *bwt, int len, int original_idx, char *output) {
    // Count occurrences of each character
    int counts[128];
    int i = 0;
    while (i < 128) { counts[i] = 0; i = i + 1; }

    i = 0;
    while (i < len) {
        int c = bwt[i];
        counts[c] = counts[c] + 1;
        i = i + 1;
    }

    // Compute starting positions for each character in sorted first column
    int starts[128];
    int total = 0;
    i = 0;
    while (i < 128) {
        starts[i] = total;
        total = total + counts[i];
        counts[i] = 0; // reset for reuse
        i = i + 1;
    }

    // Build LF mapping (last-to-first column mapping)
    int lf[64];
    i = 0;
    while (i < len) {
        int c = bwt[i];
        lf[i] = starts[c] + counts[c];
        counts[c] = counts[c] + 1;
        i = i + 1;
    }

    // Follow the LF mapping to reconstruct the string
    int idx = original_idx;
    i = len - 1;
    while (i >= 0) {
        output[i] = bwt[idx];
        idx = lf[idx];
        i = i - 1;
    }
    output[len] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

// Count runs in a string (for measuring compressibility)
int count_runs(char *s) {
    int len = str_len(s);
    if (len == 0) return 0;
    int runs = 1;
    int i = 1;
    while (i < len) {
        if (s[i] != s[i - 1]) runs = runs + 1;
        i = i + 1;
    }
    return runs;
}

int main() {
    char bwt_out[64];
    char decoded[64];

    // Test 1: Simple BWT
    char input1[16];
    input1[0] = 'b'; input1[1] = 'a'; input1[2] = 'n';
    input1[3] = 'a'; input1[4] = 'n'; input1[5] = 'a';
    input1[6] = 0;

    int orig_idx = bwt_encode(input1, bwt_out);
    printf("BWT of banana: %s\n", bwt_out); // EXPECT: BWT of banana: nnbaaa
    printf("Original index: %d\n", orig_idx); // EXPECT: Original index: 3

    // Inverse BWT
    int len1 = str_len(input1);
    bwt_decode(bwt_out, len1, orig_idx, decoded);
    printf("Inverse BWT: %s\n", decoded); // EXPECT: Inverse BWT: banana

    int match1 = str_equal(input1, decoded);
    printf("Roundtrip match: %d\n", match1); // EXPECT: Roundtrip match: 1

    // Test 2: BWT groups similar characters together
    int orig_runs = count_runs(input1);
    int bwt_runs = count_runs(bwt_out);
    printf("Original runs: %d\n", orig_runs); // EXPECT: Original runs: 6
    printf("BWT runs: %d\n", bwt_runs); // EXPECT: BWT runs: 3
    int fewer = (bwt_runs <= orig_runs) ? 1 : 0;
    printf("BWT fewer runs: %d\n", fewer); // EXPECT: BWT fewer runs: 1

    // Test 3: Already sorted input
    char input3[8];
    input3[0] = 'a'; input3[1] = 'b'; input3[2] = 'c';
    input3[3] = 'd'; input3[4] = 0;

    int idx3 = bwt_encode(input3, bwt_out);
    printf("BWT of abcd: %s\n", bwt_out); // EXPECT: BWT of abcd: dabc
    printf("Index abcd: %d\n", idx3); // EXPECT: Index abcd: 0

    bwt_decode(bwt_out, 4, idx3, decoded);
    int match3 = str_equal(input3, decoded);
    printf("abcd roundtrip: %d\n", match3); // EXPECT: abcd roundtrip: 1

    // Test 4: Repeated characters
    char input4[8];
    input4[0] = 'a'; input4[1] = 'a'; input4[2] = 'b';
    input4[3] = 'b'; input4[4] = 'a'; input4[5] = 0;

    int idx4 = bwt_encode(input4, bwt_out);
    printf("BWT of aabba: %s\n", bwt_out); // EXPECT: BWT of aabba: baaba
    printf("Index aabba: %d\n", idx4); // EXPECT: Index aabba: 1

    bwt_decode(bwt_out, 5, idx4, decoded);
    int match4 = str_equal(input4, decoded);
    printf("aabba roundtrip: %d\n", match4); // EXPECT: aabba roundtrip: 1

    // Test 5: Run count improvement
    int orig4_runs = count_runs(input4);
    char bwt4_copy[8];
    int k = 0;
    while (bwt_out[k] != 0) { bwt4_copy[k] = bwt_out[k]; k = k + 1; }
    bwt4_copy[k] = 0;
    int bwt4_runs = count_runs(bwt4_copy);
    printf("aabba orig runs: %d\n", orig4_runs); // EXPECT: aabba orig runs: 3
    printf("aabba bwt runs: %d\n", bwt4_runs); // EXPECT: aabba bwt runs: 4

    printf("BWT done\n"); // EXPECT: BWT done
    return 0;
}
