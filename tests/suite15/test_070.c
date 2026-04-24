int printf(const char *fmt, ...);

// Spell checker: edit distance based suggestions
// Uses Levenshtein distance to find closest words in a dictionary

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void str_cpy(char *dst, char *src) {
    int i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

int min2(int a, int b) {
    return a < b ? a : b;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

// Levenshtein edit distance
// Uses a flat 2D array: dp[(m+1)*(n+1)]
// Max word length: 15
int dp_table[256]; // 16*16

int edit_distance(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int cols = n + 1;
    int i;
    int j;

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0) {
                dp_table[i * cols + j] = j;
            } else if (j == 0) {
                dp_table[i * cols + j] = i;
            } else {
                int cost = (a[i - 1] != b[j - 1]) ? 1 : 0;
                int del = dp_table[(i - 1) * cols + j] + 1;
                int ins = dp_table[i * cols + (j - 1)] + 1;
                int sub = dp_table[(i - 1) * cols + (j - 1)] + cost;
                dp_table[i * cols + j] = min3(del, ins, sub);
            }
        }
    }
    return dp_table[m * cols + n];
}

struct Dictionary {
    char words[32][16];
    int count;
};

struct Suggestion {
    char word[16];
    int distance;
};

struct SuggestionList {
    struct Suggestion items[8];
    int count;
};

void add_word(struct Dictionary *dict, char *word) {
    str_cpy(dict->words[dict->count], word);
    dict->count++;
}

// Find suggestions: words within max_dist edit distance
void find_suggestions(struct Dictionary *dict, char *word, int max_dist, struct SuggestionList *out) {
    out->count = 0;
    int i;
    for (i = 0; i < dict->count; i++) {
        int d = edit_distance(word, dict->words[i]);
        if (d <= max_dist && d > 0) {
            int idx = out->count;
            if (idx < 8) {
                str_cpy(out->items[idx].word, dict->words[i]);
                out->items[idx].distance = d;
                out->count++;
            }
        }
    }
}

// Find the single best suggestion (smallest distance, first in dict order)
int find_best(struct Dictionary *dict, char *word, char *best_out) {
    int best_dist = 999;
    int best_idx = -1;
    int i;
    for (i = 0; i < dict->count; i++) {
        int d = edit_distance(word, dict->words[i]);
        if (d < best_dist) {
            best_dist = d;
            best_idx = i;
        }
    }
    if (best_idx >= 0) {
        str_cpy(best_out, dict->words[best_idx]);
    } else {
        best_out[0] = 0;
    }
    return best_dist;
}

// Check if word is in dictionary
int is_correct(struct Dictionary *dict, char *word) {
    int i;
    for (i = 0; i < dict->count; i++) {
        if (str_eq(dict->words[i], word)) return 1;
    }
    return 0;
}

// Count misspelled words in a sentence (space-separated)
int count_misspelled(struct Dictionary *dict, char *sentence) {
    int count = 0;
    int len = str_len(sentence);
    int i = 0;

    while (i < len) {
        while (i < len && sentence[i] == ' ') i++;
        if (i >= len) break;
        int start = i;
        while (i < len && sentence[i] != ' ') i++;
        char word_buf[16];
        int wlen = i - start;
        if (wlen > 15) wlen = 15;
        int j;
        for (j = 0; j < wlen; j++) word_buf[j] = sentence[start + j];
        word_buf[wlen] = 0;

        if (!is_correct(dict, word_buf)) count++;
    }
    return count;
}

int main() {
    // Test basic edit distance
    int d;

    d = edit_distance("kitten", "sitting");
    printf("%d\n", d);
    // EXPECT: 3

    d = edit_distance("hello", "hello");
    printf("%d\n", d);
    // EXPECT: 0

    d = edit_distance("", "abc");
    printf("%d\n", d);
    // EXPECT: 3

    d = edit_distance("abc", "");
    printf("%d\n", d);
    // EXPECT: 3

    d = edit_distance("a", "b");
    printf("%d\n", d);
    // EXPECT: 1

    d = edit_distance("abc", "axc");
    printf("%d\n", d);
    // EXPECT: 1

    d = edit_distance("abc", "abcd");
    printf("%d\n", d);
    // EXPECT: 1

    d = edit_distance("abcd", "abc");
    printf("%d\n", d);
    // EXPECT: 1

    d = edit_distance("abc", "xyz");
    printf("%d\n", d);
    // EXPECT: 3

    // Build dictionary
    struct Dictionary dict;
    dict.count = 0;
    add_word(&dict, "hello");
    add_word(&dict, "help");
    add_word(&dict, "world");
    add_word(&dict, "word");
    add_word(&dict, "work");
    add_word(&dict, "the");
    add_word(&dict, "this");
    add_word(&dict, "that");
    add_word(&dict, "test");
    add_word(&dict, "text");
    add_word(&dict, "program");
    add_word(&dict, "project");

    printf("%d\n", dict.count);
    // EXPECT: 12

    // Check correct words
    printf("%d\n", is_correct(&dict, "hello"));
    // EXPECT: 1
    printf("%d\n", is_correct(&dict, "helo"));
    // EXPECT: 0
    printf("%d\n", is_correct(&dict, "world"));
    // EXPECT: 1

    // Find best suggestion for misspelled word
    char best[16];

    // "helo" -> "hello" (dist 1)
    d = find_best(&dict, "helo", best);
    printf("%s\n", best);
    // EXPECT: hello
    printf("%d\n", d);
    // EXPECT: 1

    // "wrld" -> "word" (dist 1) or "world" (dist 1)?
    // edit_distance("wrld", "word"): w-w, r->o(1), l->r(1), d-d = 2
    // edit_distance("wrld", "world"): w-w, r->o(1), l->r(1), d->l(1), ->d(1) = hmm
    // Actually: "wrld" vs "world": w=w, r vs o (sub 1), l vs r (sub 1), d vs l (sub 1), _ vs d (ins 1) = too much
    // Let me compute properly:
    //   wrld (4) vs world (5)
    //   dp:     ""  w  o  r  l  d
    //   ""       0  1  2  3  4  5
    //   w        1  0  1  2  3  4
    //   r        2  1  1  1  2  3
    //   l        3  2  2  2  1  2
    //   d        4  3  3  3  2  1
    // So dist("wrld","world")=1 (just insert 'o')
    // dist("wrld","word"):
    //   dp:     ""  w  o  r  d
    //   ""       0  1  2  3  4
    //   w        1  0  1  2  3
    //   r        2  1  1  1  2
    //   l        3  2  2  2  2
    //   d        4  3  3  3  2
    // dist=2
    // So best is "world" (dist 1), which comes first at dist 1
    d = find_best(&dict, "wrld", best);
    printf("%s\n", best);
    // EXPECT: world
    printf("%d\n", d);
    // EXPECT: 1

    // "tset" -> "test" (dist 2? Let's check)
    // dp:     ""  t  e  s  t
    // ""       0  1  2  3  4
    // t        1  0  1  2  3
    // s        2  1  1  1  2
    // e        3  2  1  2  2
    // t        4  3  2  2  2
    // dist("tset","test")=2
    // dist("tset","text"):
    // dp:     ""  t  e  x  t
    // ""       0  1  2  3  4
    // t        1  0  1  2  3
    // s        2  1  1  2  3
    // e        3  2  1  2  3
    // t        4  3  2  2  2
    // dist=2 also. "test" comes first in dict, so best="test"
    d = find_best(&dict, "tset", best);
    printf("%s\n", best);
    // EXPECT: the
    printf("%d\n", d);
    // EXPECT: 2

    // Find multiple suggestions
    struct SuggestionList sugg;
    find_suggestions(&dict, "helo", 2, &sugg);
    printf("%d\n", sugg.count);
    // EXPECT: 2
    // "hello" dist 1, "help" dist 2
    printf("%s\n", sugg.items[0].word);
    // EXPECT: hello
    printf("%d\n", sugg.items[0].distance);
    // EXPECT: 1
    printf("%s\n", sugg.items[1].word);
    // EXPECT: help
    printf("%d\n", sugg.items[1].distance);
    // EXPECT: 1

    // Count misspelled words in sentence
    int mc = count_misspelled(&dict, "hello world");
    printf("%d\n", mc);
    // EXPECT: 0

    mc = count_misspelled(&dict, "helo wrld");
    printf("%d\n", mc);
    // EXPECT: 2

    mc = count_misspelled(&dict, "hello wrld test");
    printf("%d\n", mc);
    // EXPECT: 1

    mc = count_misspelled(&dict, "xyz abc def");
    printf("%d\n", mc);
    // EXPECT: 3

    return 0;
}
