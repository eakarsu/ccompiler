int printf(const char *fmt, ...);

/* Huffman frequency counting and code length estimation */

struct FreqEntry {
    int ch;
    int count;
};

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

/* Count character frequencies in a string */
int count_frequencies(char *input, struct FreqEntry *freq) {
    int counts[128];
    int i;
    int len;
    int num_unique;
    for (i = 0; i < 128; i = i + 1) {
        counts[i] = 0;
    }
    len = str_len(input);
    for (i = 0; i < len; i = i + 1) {
        int c;
        c = input[i];
        if (c >= 0 && c < 128) {
            counts[c] = counts[c] + 1;
        }
    }
    num_unique = 0;
    for (i = 0; i < 128; i = i + 1) {
        if (counts[i] > 0) {
            freq[num_unique].ch = i;
            freq[num_unique].count = counts[i];
            num_unique = num_unique + 1;
        }
    }
    return num_unique;
}

/* Sort frequencies in descending order (bubble sort) */
void sort_freq_desc(struct FreqEntry *freq, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (freq[j].count < freq[j + 1].count) {
                struct FreqEntry tmp;
                tmp = freq[j];
                freq[j] = freq[j + 1];
                freq[j + 1] = tmp;
            }
        }
    }
}

/* Estimate Huffman code lengths using a simplified method:
   Sort by frequency, assign shorter codes to more frequent chars.
   This is a rough approximation, not a real Huffman tree. */
void estimate_code_lengths(struct FreqEntry *freq, int n, int *lengths) {
    int i;
    int depth;
    int nodes_at_depth;
    int assigned;
    if (n <= 0) return;
    if (n == 1) {
        lengths[0] = 1;
        return;
    }
    depth = 1;
    nodes_at_depth = 1;
    assigned = 0;
    /* Simple approximation: double available codes at each depth level */
    for (i = 0; i < n; i = i + 1) {
        if (assigned >= nodes_at_depth) {
            depth = depth + 1;
            nodes_at_depth = nodes_at_depth * 2;
            assigned = 0;
        }
        lengths[i] = depth;
        assigned = assigned + 1;
    }
}

/* Calculate entropy-like metric: sum of freq * code_length */
int weighted_path_length(struct FreqEntry *freq, int *lengths, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + freq[i].count * lengths[i];
    }
    return sum;
}

void test_basic_frequency(void) {
    struct FreqEntry freq[128];
    int n;
    n = count_frequencies("aabbbcccc", freq);
    sort_freq_desc(freq, n);
    // EXPECT: unique chars=3
    printf("unique chars=%d\n", n);
    // EXPECT: most frequent: count=4
    printf("most frequent: count=%d\n", freq[0].count);
    // EXPECT: second: count=3
    printf("second: count=%d\n", freq[1].count);
    // EXPECT: third: count=2
    printf("third: count=%d\n", freq[2].count);
}

void test_single_char(void) {
    struct FreqEntry freq[128];
    int n;
    n = count_frequencies("aaaa", freq);
    // EXPECT: single unique=1
    printf("single unique=%d\n", n);
    // EXPECT: single count=4
    printf("single count=%d\n", freq[0].count);
}

void test_all_unique(void) {
    struct FreqEntry freq[128];
    int n;
    int i;
    int all_one;
    n = count_frequencies("abcdefgh", freq);
    // EXPECT: all unique=8
    printf("all unique=%d\n", n);
    all_one = 1;
    for (i = 0; i < n; i = i + 1) {
        if (freq[i].count != 1) all_one = 0;
    }
    // EXPECT: all count 1: 1
    printf("all count 1: %d\n", all_one);
}

void test_code_lengths(void) {
    struct FreqEntry freq[128];
    int lengths[128];
    int n;
    n = count_frequencies("aaabbc", freq);
    sort_freq_desc(freq, n);
    estimate_code_lengths(freq, n, lengths);
    // EXPECT: code len most freq=1
    printf("code len most freq=%d\n", lengths[0]);
    // EXPECT: code len second=2
    printf("code len second=%d\n", lengths[1]);
    // EXPECT: code len third=2
    printf("code len third=%d\n", lengths[2]);
}

void test_weighted_path(void) {
    struct FreqEntry freq[128];
    int lengths[128];
    int n;
    int wpl;
    n = count_frequencies("aaabbc", freq);
    sort_freq_desc(freq, n);
    estimate_code_lengths(freq, n, lengths);
    wpl = weighted_path_length(freq, lengths, n);
    // EXPECT: wpl aaabbc=9
    printf("wpl aaabbc=%d\n", wpl);
}

void test_space_and_punctuation(void) {
    struct FreqEntry freq[128];
    int n;
    int i;
    int space_count;
    n = count_frequencies("hello world", freq);
    space_count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (freq[i].ch == 32) {
            space_count = freq[i].count;
        }
    }
    // EXPECT: space count=1
    printf("space count=%d\n", space_count);
    // EXPECT: hello world unique=8
    printf("hello world unique=%d\n", n);
}

void test_sorting(void) {
    struct FreqEntry freq[128];
    int n;
    int i;
    int sorted;
    n = count_frequencies("the quick brown fox", freq);
    sort_freq_desc(freq, n);
    sorted = 1;
    for (i = 0; i < n - 1; i = i + 1) {
        if (freq[i].count < freq[i + 1].count) {
            sorted = 0;
        }
    }
    // EXPECT: sorted desc=1
    printf("sorted desc=%d\n", sorted);
}

void test_longer_text(void) {
    struct FreqEntry freq[128];
    int lengths[128];
    int n;
    int wpl_orig;
    int wpl_shuffled;
    int total;
    int i;
    char *text;
    text = "to be or not to be that is the question";
    n = count_frequencies(text, freq);
    sort_freq_desc(freq, n);
    total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + freq[i].count;
    }
    // EXPECT: total chars=39
    printf("total chars=%d\n", total);
    // EXPECT: unique in quote=13
    printf("unique in quote=%d\n", n);
    estimate_code_lengths(freq, n, lengths);
    wpl_orig = weighted_path_length(freq, lengths, n);
    // EXPECT: wpl positive=1
    printf("wpl positive=%d\n", wpl_orig > 0);
}

void test_empty(void) {
    struct FreqEntry freq[128];
    int n;
    n = count_frequencies("", freq);
    // EXPECT: empty unique=0
    printf("empty unique=%d\n", n);
}

int main(void) {
    test_basic_frequency();
    test_single_char();
    test_all_unique();
    test_code_lengths();
    test_weighted_path();
    test_space_and_punctuation();
    test_sorting();
    test_longer_text();
    test_empty();
    // EXPECT: done
    printf("done\n");
    return 0;
}
