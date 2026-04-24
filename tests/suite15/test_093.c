int printf(const char *fmt, ...);

/* Huffman Coding - Build a Huffman tree from character frequencies,
   then compute code lengths for each character.
   Uses an array-based tree (no malloc). */

struct HuffNode {
    int freq;
    int ch;       /* -1 for internal nodes */
    int left;     /* index or -1 */
    int right;    /* index or -1 */
};

struct HuffNode nodes[32];
int node_count;

int code_length[128];  /* code length per character */

int new_leaf(int ch, int freq) {
    int idx = node_count;
    nodes[idx].freq = freq;
    nodes[idx].ch = ch;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

int new_internal(int left, int right) {
    int idx = node_count;
    nodes[idx].freq = nodes[left].freq + nodes[right].freq;
    nodes[idx].ch = -1;
    nodes[idx].left = left;
    nodes[idx].right = right;
    node_count = node_count + 1;
    return idx;
}

/* Find minimum frequency node among active set */
int find_min(int *active, int n) {
    int min_idx = -1;
    int min_freq = 999999;
    int i;
    for (i = 0; i < n; i++) {
        if (active[i] >= 0 && nodes[active[i]].freq < min_freq) {
            min_freq = nodes[active[i]].freq;
            min_idx = i;
        }
    }
    return min_idx;
}

int build_huffman(int *freqs, int *chars, int n) {
    int active[16];
    int i;
    int m1;
    int m2;
    int n1;
    int n2;
    int merged;
    int remaining;

    node_count = 0;
    for (i = 0; i < n; i++) {
        active[i] = new_leaf(chars[i], freqs[i]);
    }

    remaining = n;
    while (remaining > 1) {
        /* Find two minimums */
        m1 = find_min(active, n);
        n1 = active[m1];
        active[m1] = -1;

        m2 = find_min(active, n);
        n2 = active[m2];

        merged = new_internal(n1, n2);
        active[m2] = merged;
        remaining = remaining - 1;
    }

    /* Return root index */
    for (i = 0; i < n; i++) {
        if (active[i] >= 0) return active[i];
    }
    return -1;
}

void compute_lengths(int node_idx, int depth) {
    if (node_idx < 0) return;
    if (nodes[node_idx].ch >= 0) {
        code_length[nodes[node_idx].ch] = depth;
        return;
    }
    compute_lengths(nodes[node_idx].left, depth + 1);
    compute_lengths(nodes[node_idx].right, depth + 1);
}

int weighted_path_length(int *freqs, int *chars, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + freqs[i] * code_length[chars[i]];
    }
    return total;
}

void test_basic(void) {
    /* Characters: a(1), b(1), c(2), d(4)
       Frequencies sum = 8.
       Huffman tree:
         a=1, b=1 merge -> (2)
         c=2, (2) merge -> (4)
         d=4, (4) merge -> (8)
       Code lengths: d=1, c=2, a=3, b=3
       WPL = 1*3 + 1*3 + 2*2 + 4*1 = 3+3+4+4 = 14
    */
    int freqs[4];
    int chars[4];
    int root;
    int wpl;
    int i;

    freqs[0] = 1; chars[0] = 'a';
    freqs[1] = 1; chars[1] = 'b';
    freqs[2] = 2; chars[2] = 'c';
    freqs[3] = 4; chars[3] = 'd';

    for (i = 0; i < 128; i++) code_length[i] = 0;

    root = build_huffman(freqs, chars, 4);
    compute_lengths(root, 0);

    printf("=== Basic Huffman ===\n");
    // EXPECT: === Basic Huffman ===
    printf("d length: %d\n", code_length['d']);
    // EXPECT: d length: 1
    printf("c length: %d\n", code_length['c']);
    // EXPECT: c length: 2
    printf("a length: %d\n", code_length['a']);
    // EXPECT: a length: 3
    printf("b length: %d\n", code_length['b']);
    // EXPECT: b length: 3

    wpl = weighted_path_length(freqs, chars, 4);
    printf("WPL: %d\n", wpl);
    // EXPECT: WPL: 14
}

void test_equal(void) {
    /* All equal frequencies: a(1), b(1), c(1), d(1)
       With 4 symbols, Huffman gives depth 2 for all.
       Merge a+b->(2), c+d->(2), then (2)+(2)->(4)
       All at depth 2. WPL = 4*1*2 = 8
    */
    int freqs[4];
    int chars[4];
    int root;
    int wpl;
    int i;

    freqs[0] = 1; chars[0] = 'w';
    freqs[1] = 1; chars[1] = 'x';
    freqs[2] = 1; chars[2] = 'y';
    freqs[3] = 1; chars[3] = 'z';

    for (i = 0; i < 128; i++) code_length[i] = 0;

    root = build_huffman(freqs, chars, 4);
    compute_lengths(root, 0);

    printf("=== Equal Frequencies ===\n");
    // EXPECT: === Equal Frequencies ===
    printf("w length: %d\n", code_length['w']);
    // EXPECT: w length: 2
    printf("x length: %d\n", code_length['x']);
    // EXPECT: x length: 2
    printf("y length: %d\n", code_length['y']);
    // EXPECT: y length: 2
    printf("z length: %d\n", code_length['z']);
    // EXPECT: z length: 2

    wpl = weighted_path_length(freqs, chars, 4);
    printf("WPL: %d\n", wpl);
    // EXPECT: WPL: 8
}

void test_skewed(void) {
    /* Very skewed: a(1), b(2), c(4), d(8), e(16)
       Merge a+b->(3), (3)+c->(7), (7)+d->(15), (15)+e->(31)
       e=1, d=2, c=3, b=4, a=4
       WPL = 16*1 + 8*2 + 4*3 + 2*4 + 1*4 = 16+16+12+8+4 = 56
    */
    int freqs[5];
    int chars[5];
    int root;
    int wpl;
    int i;

    freqs[0] = 1;  chars[0] = 'a';
    freqs[1] = 2;  chars[1] = 'b';
    freqs[2] = 4;  chars[2] = 'c';
    freqs[3] = 8;  chars[3] = 'd';
    freqs[4] = 16; chars[4] = 'e';

    for (i = 0; i < 128; i++) code_length[i] = 0;

    root = build_huffman(freqs, chars, 5);
    compute_lengths(root, 0);

    printf("=== Skewed Huffman ===\n");
    // EXPECT: === Skewed Huffman ===
    printf("e length: %d\n", code_length['e']);
    // EXPECT: e length: 1
    printf("d length: %d\n", code_length['d']);
    // EXPECT: d length: 2
    printf("c length: %d\n", code_length['c']);
    // EXPECT: c length: 3
    printf("b length: %d\n", code_length['b']);
    // EXPECT: b length: 4
    printf("a length: %d\n", code_length['a']);
    // EXPECT: a length: 4

    wpl = weighted_path_length(freqs, chars, 5);
    printf("WPL: %d\n", wpl);
    // EXPECT: WPL: 56

    /* Verify total nodes: 5 leaves + 4 internal = 9 */
    printf("Total nodes: %d\n", node_count);
    // EXPECT: Total nodes: 9
}

int main(void) {
    test_basic();
    test_equal();
    test_skewed();

    printf("All Huffman tests passed!\n");
    // EXPECT: All Huffman tests passed!
    return 0;
}
