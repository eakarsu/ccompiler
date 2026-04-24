int printf(const char *fmt, ...);

// JSON-like structure parser: parse nested key-value pairs from a string
// Simplified: keys and values are identifiers or integers, nesting with { }

struct KVPair {
    char key[32];
    char value[32];
    int is_int;
    int int_val;
    int has_children;
    int child_start;
    int child_count;
};

struct JSONDoc {
    struct KVPair pairs[32];
    int count;
};

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

void copy_n(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n && i < 31; i++) dst[i] = src[i];
    dst[i] = 0;
}

int skip_ws(char *s, int pos) {
    while (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\t') pos++;
    return pos;
}

int read_ident(char *s, int pos, char *out) {
    int start = pos;
    while (is_alpha(s[pos]) || is_digit(s[pos])) pos++;
    copy_n(out, s + start, pos - start);
    return pos;
}

int read_int(char *s, int pos, int *val) {
    int v = 0;
    int neg = 0;
    if (s[pos] == '-') { neg = 1; pos++; }
    while (is_digit(s[pos])) {
        v = v * 10 + (s[pos] - '0');
        pos++;
    }
    if (neg) v = -v;
    *val = v;
    return pos;
}

int parse_object(char *s, int pos, struct JSONDoc *doc, int depth);

int parse_value(char *s, int pos, struct JSONDoc *doc, int idx) {
    pos = skip_ws(s, pos);
    if (s[pos] == '{') {
        doc->pairs[idx].has_children = 1;
        doc->pairs[idx].child_start = doc->count;
        int saved = doc->count;
        pos = parse_object(s, pos, doc, 1);
        doc->pairs[idx].child_count = doc->count - saved;
    } else if (is_digit(s[pos]) || s[pos] == '-') {
        int v;
        pos = read_int(s, pos, &v);
        doc->pairs[idx].is_int = 1;
        doc->pairs[idx].int_val = v;
        doc->pairs[idx].value[0] = 0;
    } else if (is_alpha(s[pos])) {
        pos = read_ident(s, pos, doc->pairs[idx].value);
        doc->pairs[idx].is_int = 0;
    }
    return pos;
}

int parse_object(char *s, int pos, struct JSONDoc *doc, int depth) {
    pos = skip_ws(s, pos);
    if (s[pos] == '{') pos++;
    pos = skip_ws(s, pos);

    while (s[pos] && s[pos] != '}') {
        int idx = doc->count;
        doc->pairs[idx].has_children = 0;
        doc->pairs[idx].child_start = 0;
        doc->pairs[idx].child_count = 0;
        doc->pairs[idx].is_int = 0;
        doc->pairs[idx].value[0] = 0;

        pos = skip_ws(s, pos);
        pos = read_ident(s, pos, doc->pairs[idx].key);
        pos = skip_ws(s, pos);
        if (s[pos] == ':') pos++;
        pos = skip_ws(s, pos);
        doc->count++;
        pos = parse_value(s, pos, doc, idx);
        pos = skip_ws(s, pos);
        if (s[pos] == ',') pos++;
        pos = skip_ws(s, pos);
    }
    if (s[pos] == '}') pos++;
    return pos;
}

int find_key(struct JSONDoc *doc, char *key, int start, int count) {
    int i;
    int end = start + count;
    if (count == 0) end = doc->count;
    for (i = start; i < end; i++) {
        int j = 0;
        int match = 1;
        while (doc->pairs[i].key[j] && key[j]) {
            if (doc->pairs[i].key[j] != key[j]) { match = 0; break; }
            j++;
        }
        if (match && doc->pairs[i].key[j] == key[j]) return i;
    }
    return -1;
}

int main() {
    struct JSONDoc doc;
    doc.count = 0;

    char input[256];
    // Build: { name: alice, age: 30, city: nyc }
    char *src = "{ name: alice, age: 30, city: nyc }";
    int k = 0;
    while (src[k]) { input[k] = src[k]; k++; }
    input[k] = 0;

    parse_object(input, 0, &doc, 0);

    printf("%d\n", doc.count);
    // EXPECT: 3
    printf("%s\n", doc.pairs[0].key);
    // EXPECT: name
    printf("%s\n", doc.pairs[0].value);
    // EXPECT: alice
    printf("%s\n", doc.pairs[1].key);
    // EXPECT: age
    printf("%d\n", doc.pairs[1].int_val);
    // EXPECT: 30
    printf("%d\n", doc.pairs[1].is_int);
    // EXPECT: 1

    int idx = find_key(&doc, "city", 0, 0);
    printf("%d\n", idx);
    // EXPECT: 2
    printf("%s\n", doc.pairs[idx].value);
    // EXPECT: nyc

    // Nested object
    struct JSONDoc doc2;
    doc2.count = 0;
    char input2[256];
    char *src2 = "{ user: { name: bob, level: 5 }, active: 1 }";
    k = 0;
    while (src2[k]) { input2[k] = src2[k]; k++; }
    input2[k] = 0;

    parse_object(input2, 0, &doc2, 0);
    printf("%d\n", doc2.count);
    // EXPECT: 4

    // user is pair 0, it should have children
    printf("%s\n", doc2.pairs[0].key);
    // EXPECT: user
    printf("%d\n", doc2.pairs[0].has_children);
    // EXPECT: 1
    printf("%d\n", doc2.pairs[0].child_count);
    // EXPECT: 2

    // Children of user
    int cs = doc2.pairs[0].child_start;
    printf("%s\n", doc2.pairs[cs].key);
    // EXPECT: name
    printf("%s\n", doc2.pairs[cs].value);
    // EXPECT: bob
    printf("%s\n", doc2.pairs[cs + 1].key);
    // EXPECT: level
    printf("%d\n", doc2.pairs[cs + 1].int_val);
    // EXPECT: 5

    // active is pair 3
    int ai = find_key(&doc2, "active", 0, 0);
    printf("%d\n", ai);
    // EXPECT: 3
    printf("%d\n", doc2.pairs[ai].int_val);
    // EXPECT: 1

    // Not found
    int nf = find_key(&doc2, "email", 0, 0);
    printf("%d\n", nf);
    // EXPECT: -1

    // Empty object
    struct JSONDoc doc3;
    doc3.count = 0;
    char input3[16];
    input3[0] = '{'; input3[1] = '}'; input3[2] = 0;
    parse_object(input3, 0, &doc3, 0);
    printf("%d\n", doc3.count);
    // EXPECT: 0

    return 0;
}
