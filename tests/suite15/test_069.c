int printf(const char *fmt, ...);

// Word wrapping at a given column width
// Break text into lines that don't exceed the column width,
// breaking at word boundaries when possible

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void str_ncpy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
    dst[i] = 0;
}

struct WrappedLine {
    char text[80];
    int len;
};

struct WrappedText {
    struct WrappedLine lines[16];
    int line_count;
};

void init_wrapped(struct WrappedText *wt) {
    wt->line_count = 0;
}

void add_wrapped_line(struct WrappedText *wt, char *text, int len) {
    int idx = wt->line_count;
    str_ncpy(wt->lines[idx].text, text, len);
    wt->lines[idx].len = len;
    wt->line_count++;
}

// Word wrap text at the given width
void word_wrap(char *text, int width, struct WrappedText *result) {
    int tlen = str_len(text);
    int pos = 0;

    init_wrapped(result);

    while (pos < tlen) {
        // Skip leading spaces at start of line
        while (pos < tlen && text[pos] == ' ') pos++;
        if (pos >= tlen) break;

        int line_start = pos;
        int last_space = -1;
        int line_len = 0;

        // Scan forward up to width characters
        while (pos < tlen && line_len < width) {
            if (text[pos] == ' ') {
                last_space = pos;
            }
            pos++;
            line_len++;
        }

        // If we haven't reached end of text
        if (pos < tlen) {
            // If current char is space, break here
            if (text[pos] == ' ') {
                add_wrapped_line(result, text + line_start, pos - line_start);
                pos++; // skip the space
            }
            // If we found a space within the line, break there
            else if (last_space > line_start) {
                add_wrapped_line(result, text + line_start, last_space - line_start);
                pos = last_space + 1;
            }
            // No space found - forced break
            else {
                add_wrapped_line(result, text + line_start, pos - line_start);
            }
        } else {
            // Reached end of text
            add_wrapped_line(result, text + line_start, pos - line_start);
        }
    }
}

// Count words in text
int count_words(char *text) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (text[i]) {
        if (text[i] == ' ') {
            in_word = 0;
        } else {
            if (!in_word) count++;
            in_word = 1;
        }
        i++;
    }
    return count;
}

// Find longest word length
int longest_word(char *text) {
    int maxlen = 0;
    int curlen = 0;
    int i = 0;
    while (text[i]) {
        if (text[i] == ' ') {
            if (curlen > maxlen) maxlen = curlen;
            curlen = 0;
        } else {
            curlen++;
        }
        i++;
    }
    if (curlen > maxlen) maxlen = curlen;
    return maxlen;
}

// Check if all lines respect the width
int check_width(struct WrappedText *wt, int width) {
    int i;
    for (i = 0; i < wt->line_count; i++) {
        if (wt->lines[i].len > width) return 0;
    }
    return 1;
}

int main() {
    struct WrappedText wt;

    // Test 1: simple wrap at width 10
    // "hello world foo bar" (19 chars)
    // pos=0: scan 10 chars "hello worl", pos=10, text[10]='d' (not space)
    //   last_space=5, break at 5 -> line "hello"
    // pos=6: scan 10 chars "world foo " (actually 9 "world foo" then space at 15)
    //   wait: text[6..15] = "world foo ", that's 10 chars, pos=16, text[16]='b'
    //   last_space=15, break at 15 -> line "world foo"
    // pos=16: scan "bar" (3 chars), pos=19, end -> line "bar"
    word_wrap("hello world foo bar", 10, &wt);
    printf("%d\n", wt.line_count);
    // EXPECT: 3
    printf("%s\n", wt.lines[0].text);
    // EXPECT: hello
    printf("%s\n", wt.lines[1].text);
    // EXPECT: world foo
    printf("%s\n", wt.lines[2].text);
    // EXPECT: bar

    // Test 2: text fits on one line
    struct WrappedText wt2;
    word_wrap("short", 20, &wt2);
    printf("%d\n", wt2.line_count);
    // EXPECT: 1
    printf("%s\n", wt2.lines[0].text);
    // EXPECT: short

    // Test 3: exact fit
    struct WrappedText wt3;
    word_wrap("abcde fghij", 11, &wt3);
    printf("%d\n", wt3.line_count);
    // EXPECT: 1
    printf("%s\n", wt3.lines[0].text);
    // EXPECT: abcde fghij

    // Test 4: width smaller than word forces break
    // "abcdefghij klm" width=5
    // pos=0: scan 5 "abcde", pos=5, text[5]='f' (not space), no last_space -> forced break
    //   line "abcde"
    // pos=5: scan 5 "fghij", pos=10, text[10]=' ', break here
    //   line "fghij"
    // pos=11: skip space, pos=11 (already non-space 'k'), scan "klm" (3), pos=14, end
    //   line "klm"
    struct WrappedText wt4;
    word_wrap("abcdefghij klm", 5, &wt4);
    printf("%d\n", wt4.line_count);
    // EXPECT: 3
    printf("%s\n", wt4.lines[0].text);
    // EXPECT: abcde
    printf("%s\n", wt4.lines[1].text);
    // EXPECT: fghij
    printf("%s\n", wt4.lines[2].text);
    // EXPECT: klm

    // Test 5: "The quick brown fox jumps over the lazy dog", width 15
    // pos=0: scan 15 "The quick brown", pos=15, text[15]=' ', break here
    //   line "The quick brown"
    // pos=16: scan 15 "fox jumps over ", pos=31
    //   wait: text[16..30] = "fox jumps over " that's 15 chars, text[31]='t'
    //   last_space=30, break at 30 -> line "fox jumps over"
    // pos=31: scan "the lazy dog" (12), pos=43, end
    //   line "the lazy dog"
    struct WrappedText wt5;
    word_wrap("The quick brown fox jumps over the lazy dog", 15, &wt5);
    printf("%d\n", wt5.line_count);
    // EXPECT: 3
    printf("%s\n", wt5.lines[0].text);
    // EXPECT: The quick brown
    printf("%s\n", wt5.lines[1].text);
    // EXPECT: fox jumps over
    printf("%s\n", wt5.lines[2].text);
    // EXPECT: the lazy dog

    // Test 6: word count
    int wc = count_words("The quick brown fox");
    printf("%d\n", wc);
    // EXPECT: 4

    wc = count_words("hello");
    printf("%d\n", wc);
    // EXPECT: 1

    wc = count_words("  spaced  out  ");
    printf("%d\n", wc);
    // EXPECT: 2

    // Test 7: longest word
    int lw = longest_word("I am a programmer");
    printf("%d\n", lw);
    // EXPECT: 10

    lw = longest_word("hi there");
    printf("%d\n", lw);
    // EXPECT: 5

    // Test 8: width equals text length
    struct WrappedText wt8;
    word_wrap("exactly ten", 11, &wt8);
    printf("%d\n", wt8.line_count);
    // EXPECT: 1
    printf("%s\n", wt8.lines[0].text);
    // EXPECT: exactly ten

    // Test 9: single character words
    // "a b c d e f", width=3
    // pos=0: scan 3 "a b", pos=3, text[3]=' ', break -> line "a b"
    // pos=4: scan 3 "c d", pos=7, text[7]=' ', break -> line "c d"
    // pos=8: scan 3 "e f", pos=11, end -> line "e f"
    struct WrappedText wt9;
    word_wrap("a b c d e f", 3, &wt9);
    printf("%d\n", wt9.line_count);
    // EXPECT: 3
    printf("%s\n", wt9.lines[0].text);
    // EXPECT: a b
    printf("%s\n", wt9.lines[1].text);
    // EXPECT: c d
    printf("%s\n", wt9.lines[2].text);
    // EXPECT: e f

    // Test 10: check_width validation
    printf("%d\n", check_width(&wt, 10));
    // EXPECT: 1
    printf("%d\n", check_width(&wt5, 15));
    // EXPECT: 1
    printf("%d\n", check_width(&wt5, 10));
    // EXPECT: 0

    return 0;
}
