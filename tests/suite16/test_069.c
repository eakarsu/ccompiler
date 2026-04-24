int printf(const char *fmt, ...);

// Key-Value Config File Parser
// Parses "key = value" lines, supports comments (#), blank lines
// State machine processes character-by-character

enum config_state {
    CFG_LINE_START,
    CFG_COMMENT,
    CFG_KEY,
    CFG_AFTER_KEY,
    CFG_EQUALS,
    CFG_BEFORE_VALUE,
    CFG_VALUE,
    CFG_DONE
};

struct config_entry {
    char key[32];
    char value[32];
    int key_len;
    int val_len;
};

struct config_parser {
    int state;
    struct config_entry entries[16];
    int entry_count;
    char cur_key[32];
    char cur_val[32];
    int cur_key_len;
    int cur_val_len;
};

void config_init(struct config_parser *p) {
    p->state = CFG_LINE_START;
    p->entry_count = 0;
    p->cur_key_len = 0;
    p->cur_val_len = 0;
    int i;
    for (i = 0; i < 32; i++) {
        p->cur_key[i] = 0;
        p->cur_val[i] = 0;
    }
}

void config_emit(struct config_parser *p) {
    int idx = p->entry_count;
    if (idx >= 16) return;
    // Trim trailing spaces from value
    while (p->cur_val_len > 0 && p->cur_val[p->cur_val_len - 1] == 32) {
        p->cur_val[p->cur_val_len - 1] = 0;
        p->cur_val_len = p->cur_val_len - 1;
    }
    int i;
    for (i = 0; i < 32; i++) {
        p->entries[idx].key[i] = 0;
        p->entries[idx].value[i] = 0;
    }
    for (i = 0; i < p->cur_key_len && i < 31; i++) {
        p->entries[idx].key[i] = p->cur_key[i];
    }
    for (i = 0; i < p->cur_val_len && i < 31; i++) {
        p->entries[idx].value[i] = p->cur_val[i];
    }
    p->entries[idx].key_len = p->cur_key_len;
    p->entries[idx].val_len = p->cur_val_len;
    p->entry_count = p->entry_count + 1;
}

void config_reset_cur(struct config_parser *p) {
    p->cur_key_len = 0;
    p->cur_val_len = 0;
    int i;
    for (i = 0; i < 32; i++) {
        p->cur_key[i] = 0;
        p->cur_val[i] = 0;
    }
}

int is_alpha_or_under(int c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
}

int is_alnum_or_under(int c) {
    return is_alpha_or_under(c) || (c >= 48 && c <= 57) || c == 46;
}

void config_feed(struct config_parser *p, char c) {
    int st = p->state;

    if (st == CFG_LINE_START) {
        if (c == 35) { // '#'
            p->state = CFG_COMMENT;
        } else if (c == 10 || c == 13) {
            // blank line, stay
        } else if (c == 32 || c == 9) {
            // skip leading whitespace
        } else if (is_alpha_or_under(c)) {
            config_reset_cur(p);
            p->cur_key[p->cur_key_len] = c;
            p->cur_key_len = p->cur_key_len + 1;
            p->state = CFG_KEY;
        }
    } else if (st == CFG_COMMENT) {
        if (c == 10) {
            p->state = CFG_LINE_START;
        }
    } else if (st == CFG_KEY) {
        if (is_alnum_or_under(c)) {
            if (p->cur_key_len < 31) {
                p->cur_key[p->cur_key_len] = c;
                p->cur_key_len = p->cur_key_len + 1;
            }
        } else if (c == 32 || c == 9) {
            p->state = CFG_AFTER_KEY;
        } else if (c == 61) { // '='
            p->state = CFG_BEFORE_VALUE;
        } else {
            p->state = CFG_LINE_START;
        }
    } else if (st == CFG_AFTER_KEY) {
        if (c == 61) {
            p->state = CFG_BEFORE_VALUE;
        } else if (c == 10) {
            p->state = CFG_LINE_START;
        }
    } else if (st == CFG_BEFORE_VALUE) {
        if (c == 32 || c == 9) {
            // skip whitespace
        } else if (c == 10 || c == 0) {
            config_emit(p);
            p->state = CFG_LINE_START;
        } else {
            p->cur_val[p->cur_val_len] = c;
            p->cur_val_len = p->cur_val_len + 1;
            p->state = CFG_VALUE;
        }
    } else if (st == CFG_VALUE) {
        if (c == 10 || c == 0) {
            config_emit(p);
            p->state = CFG_LINE_START;
        } else {
            if (p->cur_val_len < 31) {
                p->cur_val[p->cur_val_len] = c;
                p->cur_val_len = p->cur_val_len + 1;
            }
        }
    }
}

void config_parse(struct config_parser *p, char *text, int len) {
    config_init(p);
    int i;
    for (i = 0; i < len; i++) {
        config_feed(p, text[i]);
    }
    // Finalize last line if no trailing newline
    if (p->state == CFG_VALUE || p->state == CFG_BEFORE_VALUE) {
        config_feed(p, 0);
    }
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main() {
    struct config_parser p;

    printf("=== Config Parser ===\n"); // EXPECT: === Config Parser ===

    // Test 1: Simple config
    // "host = localhost\nport = 8080\n"
    char c1[64];
    c1[0]='h'; c1[1]='o'; c1[2]='s'; c1[3]='t'; c1[4]=' '; c1[5]='='; c1[6]=' ';
    c1[7]='l'; c1[8]='o'; c1[9]='c'; c1[10]='a'; c1[11]='l'; c1[12]='h'; c1[13]='o'; c1[14]='s'; c1[15]='t';
    c1[16]=10;
    c1[17]='p'; c1[18]='o'; c1[19]='r'; c1[20]='t'; c1[21]=' '; c1[22]='='; c1[23]=' ';
    c1[24]='8'; c1[25]='0'; c1[26]='8'; c1[27]='0';
    c1[28]=10;
    c1[29]=0;

    config_parse(&p, c1, 29);
    printf("Entries: %d\n", p.entry_count);        // EXPECT: Entries: 2
    printf("K0: %s\n", p.entries[0].key);          // EXPECT: K0: host
    printf("V0: %s\n", p.entries[0].value);        // EXPECT: V0: localhost
    printf("K1: %s\n", p.entries[1].key);          // EXPECT: K1: port
    printf("V1: %s\n", p.entries[1].value);        // EXPECT: V1: 8080

    // Test 2: With comments and blank lines
    // "# comment\n\nname = test\n# another\nmode = debug\n"
    char c2[64];
    c2[0]='#'; c2[1]=' '; c2[2]='c'; c2[3]=10;   // # c\n
    c2[4]=10;                                       // blank line
    c2[5]='n'; c2[6]='a'; c2[7]='m'; c2[8]='e';
    c2[9]='='; c2[10]='t'; c2[11]='e'; c2[12]='s'; c2[13]='t'; c2[14]=10;
    c2[15]='#'; c2[16]='x'; c2[17]=10;
    c2[18]='m'; c2[19]='o'; c2[20]='d'; c2[21]='e';
    c2[22]=' '; c2[23]='='; c2[24]=' ';
    c2[25]='d'; c2[26]='e'; c2[27]='b'; c2[28]='u'; c2[29]='g';
    c2[30]=10; c2[31]=0;

    config_parse(&p, c2, 31);
    printf("Entries: %d\n", p.entry_count);        // EXPECT: Entries: 2
    printf("K0: %s\n", p.entries[0].key);          // EXPECT: K0: name
    printf("V0: %s\n", p.entries[0].value);        // EXPECT: V0: test
    printf("K1: %s\n", p.entries[1].key);          // EXPECT: K1: mode
    printf("V1: %s\n", p.entries[1].value);        // EXPECT: V1: debug

    // Test 3: No trailing newline
    // "key=val"
    char c3[16];
    c3[0]='k'; c3[1]='e'; c3[2]='y'; c3[3]='='; c3[4]='v'; c3[5]='a'; c3[6]='l'; c3[7]=0;
    config_parse(&p, c3, 7);
    printf("Entries: %d\n", p.entry_count);        // EXPECT: Entries: 1
    printf("K0: %s\n", p.entries[0].key);          // EXPECT: K0: key
    printf("V0: %s\n", p.entries[0].value);        // EXPECT: V0: val

    // Test 4: Empty value
    // "empty=\n"
    char c4[16];
    c4[0]='e'; c4[1]='m'; c4[2]='p'; c4[3]='t'; c4[4]='y'; c4[5]='='; c4[6]=10; c4[7]=0;
    config_parse(&p, c4, 7);
    printf("Entries: %d\n", p.entry_count);        // EXPECT: Entries: 1
    printf("K0: %s\n", p.entries[0].key);          // EXPECT: K0: empty
    printf("V0 len: %d\n", p.entries[0].val_len);  // EXPECT: V0 len: 0

    // Test 5: Dotted keys
    // "db.host = server\n"
    char c5[32];
    c5[0]='d'; c5[1]='b'; c5[2]='.'; c5[3]='h'; c5[4]='o'; c5[5]='s'; c5[6]='t';
    c5[7]=' '; c5[8]='='; c5[9]=' ';
    c5[10]='s'; c5[11]='e'; c5[12]='r'; c5[13]='v'; c5[14]='e'; c5[15]='r';
    c5[16]=10; c5[17]=0;
    config_parse(&p, c5, 17);
    printf("Entries: %d\n", p.entry_count);        // EXPECT: Entries: 1
    printf("K0: %s\n", p.entries[0].key);          // EXPECT: K0: db.host
    printf("V0: %s\n", p.entries[0].value);        // EXPECT: V0: server

    printf("Done\n"); // EXPECT: Done
    return 0;
}
