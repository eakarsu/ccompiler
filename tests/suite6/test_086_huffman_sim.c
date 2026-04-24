int printf(const char *fmt, ...);

void freq_count(int *data, int len, int *freq, int nchars) {
    int i;
    for (i = 0; i < nchars; i = i + 1) freq[i] = 0;
    for (i = 0; i < len; i = i + 1) {
        freq[data[i]] = freq[data[i]] + 1;
    }
}

int find_min(int *freq, int *used, int nchars) {
    int min_val = 999999;
    int min_idx = -1;
    int i;
    for (i = 0; i < nchars; i = i + 1) {
        if (freq[i] > 0 && used[i] == 0 && freq[i] < min_val) {
            min_val = freq[i];
            min_idx = i;
        }
    }
    return min_idx;
}

int count_active(int *freq, int *used, int nchars) {
    int count = 0;
    int i;
    for (i = 0; i < nchars; i = i + 1) {
        if (freq[i] > 0 && used[i] == 0) count = count + 1;
    }
    return count;
}

int build_tree_steps(int *freq_in, int nchars) {
    int freq[32];
    int used[32];
    int i;
    int steps = 0;

    for (i = 0; i < nchars; i = i + 1) {
        freq[i] = freq_in[i];
        used[i] = 0;
    }

    int next_slot = nchars;

    while (count_active(freq, used, next_slot) > 1) {
        int m1 = find_min(freq, used, next_slot);
        used[m1] = 1;
        int m2 = find_min(freq, used, next_slot);
        used[m2] = 1;

        freq[next_slot] = freq[m1] + freq[m2];
        used[next_slot] = 0;
        next_slot = next_slot + 1;
        steps = steps + 1;
    }
    return steps;
}

void assign_code_lengths(int *freq_in, int nchars, int *code_len) {
    int freq[32];
    int used[32];
    int parent[32];
    int depth[32];
    int i;

    for (i = 0; i < 32; i = i + 1) {
        freq[i] = 0;
        used[i] = 0;
        parent[i] = -1;
        depth[i] = 0;
    }
    for (i = 0; i < nchars; i = i + 1) {
        freq[i] = freq_in[i];
    }

    int next_slot = nchars;

    while (count_active(freq, used, next_slot) > 1) {
        int m1 = find_min(freq, used, next_slot);
        used[m1] = 1;
        int m2 = find_min(freq, used, next_slot);
        used[m2] = 1;

        freq[next_slot] = freq[m1] + freq[m2];
        used[next_slot] = 0;
        parent[m1] = next_slot;
        parent[m2] = next_slot;
        next_slot = next_slot + 1;
    }

    for (i = 0; i < next_slot; i = i + 1) depth[i] = 0;
    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < next_slot; i = i + 1) {
            if (parent[i] >= 0) {
                int new_depth = depth[parent[i]] + 1;
                if (depth[i] != new_depth) {
                    depth[i] = new_depth;
                    changed = 1;
                }
            }
        }
    }

    for (i = 0; i < nchars; i = i + 1) {
        code_len[i] = (freq_in[i] > 0) ? depth[i] : 0;
    }
}

int main(void) {
    int data[16];
    int freq[16];
    int code_len[16];
    int i;

    data[0] = 0; data[1] = 0; data[2] = 0; data[3] = 0;
    data[4] = 1; data[5] = 1; data[6] = 2; data[7] = 3;

    freq_count(data, 8, freq, 4);
    // EXPECT: Freq[0]: 4
    printf("Freq[0]: %d\n", freq[0]);
    // EXPECT: Freq[1]: 2
    printf("Freq[1]: %d\n", freq[1]);
    // EXPECT: Freq[2]: 1
    printf("Freq[2]: %d\n", freq[2]);
    // EXPECT: Freq[3]: 1
    printf("Freq[3]: %d\n", freq[3]);

    int steps = build_tree_steps(freq, 4);
    // EXPECT: Tree build steps: 3
    printf("Tree build steps: %d\n", steps);

    assign_code_lengths(freq, 4, code_len);
    // EXPECT: Code len[0]: 1
    printf("Code len[0]: %d\n", code_len[0]);
    // EXPECT: Code len[1]: 2
    printf("Code len[1]: %d\n", code_len[1]);

    int cl2 = code_len[2];
    int cl3 = code_len[3];
    // EXPECT: Code len[2]+[3]: 6
    printf("Code len[2]+[3]: %d\n", cl2 + cl3);

    int total_bits = 0;
    for (i = 0; i < 4; i = i + 1) {
        total_bits = total_bits + freq[i] * code_len[i];
    }
    // EXPECT: Total bits: 14
    printf("Total bits: %d\n", total_bits);

    int fixed_bits = 8 * 2;
    // EXPECT: Fixed bits: 16
    printf("Fixed bits: %d\n", fixed_bits);

    // EXPECT: Huffman saves bits: 1
    printf("Huffman saves bits: %d\n", total_bits < fixed_bits ? 1 : 0);

    int uniform[8];
    int ufreq[4];
    for (i = 0; i < 8; i = i + 1) uniform[i] = i % 4;
    freq_count(uniform, 8, ufreq, 4);
    // EXPECT: Uniform freq: 2 2 2 2
    printf("Uniform freq: %d %d %d %d\n", ufreq[0], ufreq[1], ufreq[2], ufreq[3]);

    assign_code_lengths(ufreq, 4, code_len);
    // EXPECT: Uniform len[0]: 2
    printf("Uniform len[0]: %d\n", code_len[0]);
    // EXPECT: Uniform len[3]: 2
    printf("Uniform len[3]: %d\n", code_len[3]);

    int active = count_active(freq, code_len, 4);
    // EXPECT: Active nodes: 0
    printf("Active nodes: %d\n", active);

    return 0;
}
