int printf(const char *fmt, ...);

// Huffman Tree Building - frequency counting and tree construction

struct HuffNode {
    int ch;
    int freq;
    int left;
    int right;
    int parent;
};

int count_frequencies(char *input, int *freq) {
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (input[i] != 0) {
        int c = input[i];
        freq[c] = freq[c] + 1;
        i = i + 1;
    }
    return i;
}

int build_huffman(int *freq, struct HuffNode *nodes) {
    int count = 0;
    int i = 0;
    while (i < 128) {
        if (freq[i] > 0) {
            nodes[count].ch = i;
            nodes[count].freq = freq[i];
            nodes[count].left = -1;
            nodes[count].right = -1;
            nodes[count].parent = -1;
            count = count + 1;
        }
        i = i + 1;
    }

    int total = count;
    int used[64];
    i = 0;
    while (i < 64) {
        used[i] = 0;
        i = i + 1;
    }

    int merges = count - 1;
    int m = 0;
    while (m < merges) {
        int min1 = -1;
        int min2 = -1;
        i = 0;
        while (i < total) {
            if (used[i] == 0 && nodes[i].parent == -1) {
                if (min1 == -1 || nodes[i].freq < nodes[min1].freq) {
                    min2 = min1;
                    min1 = i;
                } else if (min2 == -1 || nodes[i].freq < nodes[min2].freq) {
                    min2 = i;
                }
            }
            i = i + 1;
        }
        if (min1 == -1 || min2 == -1) break;

        nodes[total].ch = -1;
        nodes[total].freq = nodes[min1].freq + nodes[min2].freq;
        nodes[total].left = min1;
        nodes[total].right = min2;
        nodes[total].parent = -1;
        nodes[min1].parent = total;
        nodes[min2].parent = total;
        total = total + 1;
        m = m + 1;
    }
    return total;
}

int get_depth(struct HuffNode *nodes, int idx) {
    int depth = 0;
    int cur = idx;
    while (nodes[cur].parent != -1) {
        depth = depth + 1;
        cur = nodes[cur].parent;
    }
    return depth;
}

int main() {
    int freq[128];
    struct HuffNode nodes[64];

    char input[32];
    input[0] = 'a'; input[1] = 'a'; input[2] = 'a'; input[3] = 'a';
    input[4] = 'b'; input[5] = 'b';
    input[6] = 'c';
    input[7] = 'd';
    input[8] = 0;

    int len = count_frequencies(input, freq);
    printf("Input length: %d\n", len); // EXPECT: Input length: 8
    printf("Freq a: %d\n", freq[97]); // EXPECT: Freq a: 4
    printf("Freq b: %d\n", freq[98]); // EXPECT: Freq b: 2
    printf("Freq c: %d\n", freq[99]); // EXPECT: Freq c: 1
    printf("Freq d: %d\n", freq[100]); // EXPECT: Freq d: 1

    int total = build_huffman(freq, nodes);
    printf("Total nodes: %d\n", total); // EXPECT: Total nodes: 7

    // Find root (last node)
    int root = total - 1;
    printf("Root freq: %d\n", nodes[root].freq); // EXPECT: Root freq: 8

    // Check depths - 'a' should be shortest since it's most frequent
    int depth_a = -1;
    int depth_b = -1;
    int depth_c = -1;
    int depth_d = -1;
    int i = 0;
    while (i < total) {
        if (nodes[i].ch == 97) depth_a = get_depth(nodes, i);
        if (nodes[i].ch == 98) depth_b = get_depth(nodes, i);
        if (nodes[i].ch == 99) depth_c = get_depth(nodes, i);
        if (nodes[i].ch == 100) depth_d = get_depth(nodes, i);
        i = i + 1;
    }

    printf("Depth a: %d\n", depth_a); // EXPECT: Depth a: 1
    printf("Depth b: %d\n", depth_b); // EXPECT: Depth b: 2

    // c and d have same freq so depth should be equal and deepest
    int cd_equal = (depth_c == depth_d);
    printf("c d same depth: %d\n", cd_equal); // EXPECT: c d same depth: 1
    printf("Depth c: %d\n", depth_c); // EXPECT: Depth c: 3
    printf("Depth d: %d\n", depth_d); // EXPECT: Depth d: 3

    // Weighted path length (approximation of encoded bits)
    int wpl = freq[97] * depth_a + freq[98] * depth_b + freq[99] * depth_c + freq[100] * depth_d;
    printf("Weighted path len: %d\n", wpl); // EXPECT: Weighted path len: 14

    // Bits per symbol average (integer)
    int avg_bits = wpl / len;
    printf("Avg bits per sym: %d\n", avg_bits); // EXPECT: Avg bits per sym: 1

    printf("Huffman done\n"); // EXPECT: Huffman done
    return 0;
}
