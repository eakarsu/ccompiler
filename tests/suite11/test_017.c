int printf(const char *fmt, ...);

// Word frequency counter
// Counts occurrences of each unique word in text

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0) ? 1 : 0;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

// Convert char to lowercase
char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

int is_alpha(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

struct WordFreq {
    char words[32][20];
    int counts[32];
    int nwords;
};

// Extract words from text, converting to lowercase
int extract_words(char *text, char out[32][20]) {
    int i = 0;
    int wc = 0;
    int wi = 0;

    while (text[i] != 0) {
        if (is_alpha(text[i])) {
            if (wi < 19) {
                out[wc][wi] = to_lower(text[i]);
                wi = wi + 1;
            }
        } else {
            if (wi > 0) {
                out[wc][wi] = 0;
                wc = wc + 1;
                wi = 0;
            }
        }
        i = i + 1;
    }
    if (wi > 0) {
        out[wc][wi] = 0;
        wc = wc + 1;
    }
    return wc;
}

void count_frequencies(char *text, struct WordFreq *freq) {
    char all_words[32][20];
    int total = extract_words(text, all_words);
    int i;
    int j;
    int found;

    freq->nwords = 0;

    for (i = 0; i < total; i = i + 1) {
        found = 0;
        for (j = 0; j < freq->nwords; j = j + 1) {
            if (str_eq(all_words[i], freq->words[j])) {
                freq->counts[j] = freq->counts[j] + 1;
                found = 1;
                break;
            }
        }
        if (found == 0) {
            str_copy(freq->words[freq->nwords], all_words[i]);
            freq->counts[freq->nwords] = 1;
            freq->nwords = freq->nwords + 1;
        }
    }
}

// Find the most frequent word, return its index
int most_frequent(struct WordFreq *freq) {
    int best = 0;
    int best_count = 0;
    int i;

    for (i = 0; i < freq->nwords; i = i + 1) {
        if (freq->counts[i] > best_count) {
            best_count = freq->counts[i];
            best = i;
        }
    }
    return best;
}

// Sort frequencies in descending order (bubble sort)
void sort_by_freq(struct WordFreq *freq) {
    int i;
    int j;
    int tmp_count;
    char tmp_word[20];

    for (i = 0; i < freq->nwords - 1; i = i + 1) {
        for (j = 0; j < freq->nwords - 1 - i; j = j + 1) {
            if (freq->counts[j] < freq->counts[j + 1]) {
                tmp_count = freq->counts[j];
                freq->counts[j] = freq->counts[j + 1];
                freq->counts[j + 1] = tmp_count;
                str_copy(tmp_word, freq->words[j]);
                str_copy(freq->words[j], freq->words[j + 1]);
                str_copy(freq->words[j + 1], tmp_word);
            }
        }
    }
}

void test_extract() {
    char words[32][20];
    int n;

    n = extract_words("Hello World", words);
    printf("extract: %d\n", n);
    // EXPECT: extract: 2
    printf("w0: %s\n", words[0]);
    // EXPECT: w0: hello
    printf("w1: %s\n", words[1]);
    // EXPECT: w1: world

    n = extract_words("one, two; three!", words);
    printf("punct: %d\n", n);
    // EXPECT: punct: 3
    printf("p0: %s\n", words[0]);
    // EXPECT: p0: one
    printf("p1: %s\n", words[1]);
    // EXPECT: p1: two
    printf("p2: %s\n", words[2]);
    // EXPECT: p2: three
}

void test_frequency() {
    struct WordFreq freq;

    count_frequencies("the cat sat on the mat the cat", &freq);
    printf("unique: %d\n", freq.nwords);
    // EXPECT: unique: 5

    sort_by_freq(&freq);
    printf("top: %s=%d\n", freq.words[0], freq.counts[0]);
    // EXPECT: top: the=3
    printf("second: %s=%d\n", freq.words[1], freq.counts[1]);
    // EXPECT: second: cat=2
}

void test_single() {
    struct WordFreq freq;

    count_frequencies("hello", &freq);
    printf("single unique: %d\n", freq.nwords);
    // EXPECT: single unique: 1
    printf("single word: %s=%d\n", freq.words[0], freq.counts[0]);
    // EXPECT: single word: hello=1
}

void test_repeated() {
    struct WordFreq freq;
    int idx;

    count_frequencies("go go go stop go", &freq);
    printf("rep unique: %d\n", freq.nwords);
    // EXPECT: rep unique: 2

    idx = most_frequent(&freq);
    printf("most freq: %s=%d\n", freq.words[idx], freq.counts[idx]);
    // EXPECT: most freq: go=4
}

void test_case() {
    struct WordFreq freq;

    count_frequencies("Hello HELLO hello", &freq);
    printf("case unique: %d\n", freq.nwords);
    // EXPECT: case unique: 1
    printf("case word: %s=%d\n", freq.words[0], freq.counts[0]);
    // EXPECT: case word: hello=3
}

void test_sorted() {
    struct WordFreq freq;
    int i;

    count_frequencies("a b c a b a d d d d", &freq);
    sort_by_freq(&freq);
    printf("sorted:\n");
    // EXPECT: sorted:
    for (i = 0; i < freq.nwords; i = i + 1) {
        printf("  %s: %d\n", freq.words[i], freq.counts[i]);
    }
    // EXPECT:   d: 4
    // EXPECT:   a: 3
    // EXPECT:   b: 2
    // EXPECT:   c: 1

    printf("freq tests done\n");
    // EXPECT: freq tests done
}

int main() {
    test_extract();
    test_frequency();
    test_single();
    test_repeated();
    test_case();
    test_sorted();
    return 0;
}
