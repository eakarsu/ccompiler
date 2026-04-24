int printf(const char *fmt, ...);
// EXPECT: Frequency of 'hello world':\n' ': 1\n'd': 1\n'e': 1\n'h': 1\n'l': 3\n'o': 2\n'r': 1\n'w': 1\nUnique chars: 8\nMost frequent: 'l'\nHuffman bits: 32\n\nFrequency of 'aaaaaabbbbbccccdddeef':\n'a': 6\n'b': 5\n'c': 4\n'd': 3\n'e': 2\n'f': 1\nUnique chars: 6\nHuffman bits: 51\n\nFixed 8-bit: 168 bits
// Test: Huffman-like frequency analysis

int freq[128];

void count_freq(char *s) {
    int i;
    for (i = 0; i < 128; i++) freq[i] = 0;
    for (i = 0; s[i] != 0; i++) {
        freq[(int)s[i]]++;
    }
}

void print_freq(void) {
    int i;
    for (i = 0; i < 128; i++) {
        if (freq[i] > 0) {
            if (i == 32)
                printf("' ': %d\n", freq[i]);
            else
                printf("'%c': %d\n", (char)i, freq[i]);
        }
    }
}

int count_unique(void) {
    int count = 0;
    int i;
    for (i = 0; i < 128; i++) {
        if (freq[i] > 0) count++;
    }
    return count;
}

char most_frequent(void) {
    int max_f = 0;
    char result = 0;
    int i;
    for (i = 0; i < 128; i++) {
        if (freq[i] > max_f) {
            max_f = freq[i];
            result = (char)i;
        }
    }
    return result;
}

int estimate_huffman_bits(char *s) {
    count_freq(s);
    int total = 0;
    int i;
    for (i = 0; s[i] != 0; i++) total++;

    int vals[128];
    int nvals = 0;
    for (i = 0; i < 128; i++) {
        if (freq[i] > 0) {
            vals[nvals] = freq[i];
            nvals++;
        }
    }

    int bits = 0;
    while (nvals > 1) {
        int min1 = 0, min2 = 1;
        if (vals[min1] > vals[min2]) { int t = min1; min1 = min2; min2 = t; }
        for (i = 2; i < nvals; i++) {
            if (vals[i] < vals[min1]) {
                min2 = min1;
                min1 = i;
            } else if (vals[i] < vals[min2]) {
                min2 = i;
            }
        }
        int combined = vals[min1] + vals[min2];
        bits = bits + combined;
        vals[min1] = combined;
        vals[min2] = vals[nvals - 1];
        nvals--;
    }
    return bits;
}

int main(void) {
    char s1[] = "hello world";
    count_freq(s1);
    printf("Frequency of '%s':\n", s1);
    print_freq();
    printf("Unique chars: %d\n", count_unique());
    printf("Most frequent: '%c'\n", most_frequent());
    printf("Huffman bits: %d\n", estimate_huffman_bits(s1));

    char s2[] = "aaaaaabbbbbccccdddeef";
    printf("\nFrequency of '%s':\n", s2);
    count_freq(s2);
    print_freq();
    printf("Unique chars: %d\n", count_unique());
    printf("Huffman bits: %d\n", estimate_huffman_bits(s2));

    printf("\nFixed 8-bit: %d bits\n", 8 * 21);

    return 0;
}
