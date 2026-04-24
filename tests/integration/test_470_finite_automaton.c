int printf(const char *fmt, ...);
// EXPECT: === Pattern: abc ===\nabc: 1\nab: 0\nabcd: 0\nxabc: 0\n=== Pattern: ab*c ===\nac: 1\nabc: 1\nabbc: 1\nabbbc: 1\nadc: 0\n=== Pattern: integer ===\n42: 1\n-7: 1\nabc: 0\n123: 1\n=== Find abc in text ===\nFound at pos=2 len=3\nNot found in xxabyy
// Finite automaton for pattern matching

struct DFA {
    int transitions[20][128];
    int accepting[20];
    int num_states;
    int start;
};

void dfa_init(struct DFA *dfa, int num_states, int start) {
    int i;
    int j;
    dfa->num_states = num_states;
    dfa->start = start;
    for (i = 0; i < 20; i++) {
        dfa->accepting[i] = 0;
        for (j = 0; j < 128; j++) {
            dfa->transitions[i][j] = -1;
        }
    }
}

void dfa_add_transition(struct DFA *dfa, int from, int ch, int to) {
    dfa->transitions[from][ch] = to;
}

void dfa_set_accepting(struct DFA *dfa, int state) {
    dfa->accepting[state] = 1;
}

int dfa_run(struct DFA *dfa, char *input) {
    int state = dfa->start;
    int i = 0;
    while (input[i] != 0) {
        int ch = input[i];
        if (ch < 0 || ch >= 128) return 0;
        state = dfa->transitions[state][ch];
        if (state == -1) return 0;
        i++;
    }
    return dfa->accepting[state];
}

int dfa_find(struct DFA *dfa, char *text, int *match_start, int *match_len) {
    int i;
    int j;
    int state;
    int tlen = 0;
    while (text[tlen] != 0) tlen++;
    for (i = 0; i < tlen; i++) {
        state = dfa->start;
        for (j = i; j < tlen; j++) {
            state = dfa->transitions[state][(int)text[j]];
            if (state == -1) break;
            if (dfa->accepting[state]) {
                *match_start = i;
                *match_len = j - i + 1;
                return 1;
            }
        }
    }
    return 0;
}

void build_dfa_abc(struct DFA *dfa) {
    dfa_init(dfa, 4, 0);
    dfa_add_transition(dfa, 0, 'a', 1);
    dfa_add_transition(dfa, 1, 'b', 2);
    dfa_add_transition(dfa, 2, 'c', 3);
    dfa_set_accepting(dfa, 3);
}

void build_dfa_ab_star_c(struct DFA *dfa) {
    dfa_init(dfa, 3, 0);
    dfa_add_transition(dfa, 0, 'a', 1);
    dfa_add_transition(dfa, 1, 'b', 1);
    dfa_add_transition(dfa, 1, 'c', 2);
    dfa_set_accepting(dfa, 2);
}

void build_dfa_int(struct DFA *dfa) {
    int c;
    dfa_init(dfa, 3, 0);
    dfa_add_transition(dfa, 0, '-', 1);
    for (c = '0'; c <= '9'; c++) {
        dfa_add_transition(dfa, 0, c, 2);
        dfa_add_transition(dfa, 1, c, 2);
        dfa_add_transition(dfa, 2, c, 2);
    }
    dfa_set_accepting(dfa, 2);
}

int main(void) {
    struct DFA dfa;
    int pos;
    int len;

    printf("=== Pattern: abc ===\n");
    build_dfa_abc(&dfa);
    printf("abc: %d\n", dfa_run(&dfa, "abc"));
    printf("ab: %d\n", dfa_run(&dfa, "ab"));
    printf("abcd: %d\n", dfa_run(&dfa, "abcd"));
    printf("xabc: %d\n", dfa_run(&dfa, "xabc"));

    printf("=== Pattern: ab*c ===\n");
    build_dfa_ab_star_c(&dfa);
    printf("ac: %d\n", dfa_run(&dfa, "ac"));
    printf("abc: %d\n", dfa_run(&dfa, "abc"));
    printf("abbc: %d\n", dfa_run(&dfa, "abbc"));
    printf("abbbc: %d\n", dfa_run(&dfa, "abbbc"));
    printf("adc: %d\n", dfa_run(&dfa, "adc"));

    printf("=== Pattern: integer ===\n");
    build_dfa_int(&dfa);
    printf("42: %d\n", dfa_run(&dfa, "42"));
    printf("-7: %d\n", dfa_run(&dfa, "-7"));
    printf("abc: %d\n", dfa_run(&dfa, "abc"));
    printf("123: %d\n", dfa_run(&dfa, "123"));

    printf("=== Find abc in text ===\n");
    build_dfa_abc(&dfa);
    if (dfa_find(&dfa, "xxabcyy", &pos, &len)) {
        printf("Found at pos=%d len=%d\n", pos, len);
    }
    if (!dfa_find(&dfa, "xxabyy", &pos, &len)) {
        printf("Not found in xxabyy\n");
    }

    return 0;
}
