int printf(const char *fmt, ...);

// Text wrapping and justification
// Wraps text to a given width, with left and justify modes

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

// Split text into words, return word count
// words array and lens array must be pre-allocated
int split_words(char *text, char words[32][32], int *lens) {
    int i = 0;
    int wc = 0;
    int wi = 0;

    while (text[i] != 0) {
        if (text[i] == ' ' || text[i] == '\n') {
            if (wi > 0) {
                words[wc][wi] = 0;
                lens[wc] = wi;
                wc = wc + 1;
                wi = 0;
            }
        } else {
            words[wc][wi] = text[i];
            wi = wi + 1;
        }
        i = i + 1;
    }
    if (wi > 0) {
        words[wc][wi] = 0;
        lens[wc] = wi;
        wc = wc + 1;
    }
    return wc;
}

// Wrap text to width, left-aligned
// Each output line stored in lines array
int wrap_left(char *text, int width, char lines[16][80]) {
    char words[32][32];
    int lens[32];
    int nwords = split_words(text, words, lens);
    int line_count = 0;
    int col = 0;
    int i;
    int j;

    for (i = 0; i < 16; i = i + 1) {
        lines[i][0] = 0;
    }

    for (i = 0; i < nwords; i = i + 1) {
        if (col == 0) {
            // First word on line
            for (j = 0; j < lens[i]; j = j + 1) {
                lines[line_count][col] = words[i][j];
                col = col + 1;
            }
            lines[line_count][col] = 0;
        } else if (col + 1 + lens[i] <= width) {
            // Word fits on current line
            lines[line_count][col] = ' ';
            col = col + 1;
            for (j = 0; j < lens[i]; j = j + 1) {
                lines[line_count][col] = words[i][j];
                col = col + 1;
            }
            lines[line_count][col] = 0;
        } else {
            // Start new line
            line_count = line_count + 1;
            col = 0;
            for (j = 0; j < lens[i]; j = j + 1) {
                lines[line_count][col] = words[i][j];
                col = col + 1;
            }
            lines[line_count][col] = 0;
        }
    }
    if (col > 0) {
        line_count = line_count + 1;
    }
    return line_count;
}

// Count line length
int line_len(char *line) {
    int n = 0;
    while (line[n] != 0) n = n + 1;
    return n;
}

void test_split() {
    char words[32][32];
    int lens[32];
    int n;

    n = split_words("hello world foo", words, lens);
    printf("split count: %d\n", n);
    // EXPECT: split count: 3
    printf("w0: %s len=%d\n", words[0], lens[0]);
    // EXPECT: w0: hello len=5
    printf("w1: %s len=%d\n", words[1], lens[1]);
    // EXPECT: w1: world len=5
    printf("w2: %s len=%d\n", words[2], lens[2]);
    // EXPECT: w2: foo len=3

    n = split_words("  extra   spaces  ", words, lens);
    printf("extra spaces: %d\n", n);
    // EXPECT: extra spaces: 2
    printf("es0: %s\n", words[0]);
    // EXPECT: es0: extra
    printf("es1: %s\n", words[1]);
    // EXPECT: es1: spaces

    n = split_words("single", words, lens);
    printf("single: %d\n", n);
    // EXPECT: single: 1
}

void test_wrap() {
    char lines[16][80];
    int n;
    int i;

    n = wrap_left("the quick brown fox jumps over the lazy dog", 15, lines);
    printf("wrap lines: %d\n", n);
    // EXPECT: wrap lines: 3
    for (i = 0; i < n; i = i + 1) {
        printf("L%d: %s\n", i, lines[i]);
    }
    // EXPECT: L0: the quick brown
    // EXPECT: L1: fox jumps over
    // EXPECT: L2: the lazy dog

    n = wrap_left("short", 20, lines);
    printf("short lines: %d\n", n);
    // EXPECT: short lines: 1
    printf("S0: %s\n", lines[0]);
    // EXPECT: S0: short

    n = wrap_left("aa bb cc dd ee ff", 8, lines);
    printf("narrow lines: %d\n", n);
    // EXPECT: narrow lines: 2
    for (i = 0; i < n; i = i + 1) {
        printf("N%d: %s\n", i, lines[i]);
    }
    // EXPECT: N0: aa bb cc
    // EXPECT: N1: dd ee ff
}

void test_wrap_exact() {
    char lines[16][80];
    int n;
    int i;

    n = wrap_left("ab cd ef gh ij kl", 5, lines);
    printf("exact lines: %d\n", n);
    // EXPECT: exact lines: 3
    for (i = 0; i < n; i = i + 1) {
        printf("E%d: %s\n", i, lines[i]);
    }
    // EXPECT: E0: ab cd
    // EXPECT: E1: ef gh
    // EXPECT: E2: ij kl
}

void test_line_lengths() {
    char lines[16][80];
    int n;
    int i;

    n = wrap_left("one two three four five six", 12, lines);
    printf("len lines: %d\n", n);
    // EXPECT: len lines: 3
    for (i = 0; i < n; i = i + 1) {
        int ll = line_len(lines[i]);
        printf("len L%d: %d ok=%d\n", i, ll, ll <= 12);
    }
    // EXPECT: len L0: 7 ok=1
    // EXPECT: len L1: 10 ok=1
    // EXPECT: len L2: 8 ok=1

    printf("wrap tests done\n");
    // EXPECT: wrap tests done
}

int main() {
    test_split();
    test_wrap();
    test_wrap_exact();
    test_line_lengths();
    return 0;
}
