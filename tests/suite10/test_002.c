int printf(const char *fmt, ...);

// NFA Simulator using Subset Construction
// NFA accepts strings over {a,b} that end with "ab"
// NFA states: 0 (start), 1 (saw 'a'), 2 (accept, saw 'ab')
// Symbol 0 = 'a', Symbol 1 = 'b'

struct NFA {
    int num_states;
    int num_symbols;
    int start_state;
    int transitions[5][2][5]; // transitions[state][symbol] = list of next states
    int trans_count[5][2];    // how many transitions from each state/symbol
    int accepting[5];
};

void init_nfa(struct NFA *nfa) {
    int i; int j; int k;
    nfa->num_states = 3;
    nfa->num_symbols = 2;
    nfa->start_state = 0;
    for (i = 0; i < 5; i++) {
        nfa->accepting[i] = 0;
        for (j = 0; j < 2; j++) {
            nfa->trans_count[i][j] = 0;
            for (k = 0; k < 5; k++) {
                nfa->transitions[i][j][k] = -1;
            }
        }
    }
    // State 0 on 'a': go to 0 and 1 (nondeterministic)
    nfa->transitions[0][0][0] = 0;
    nfa->transitions[0][0][1] = 1;
    nfa->trans_count[0][0] = 2;
    // State 0 on 'b': stay in 0
    nfa->transitions[0][1][0] = 0;
    nfa->trans_count[0][1] = 1;
    // State 1 on 'b': go to 2
    nfa->transitions[1][1][0] = 2;
    nfa->trans_count[1][1] = 1;
    // State 1 on 'a': no transitions
    nfa->trans_count[1][0] = 0;
    // State 2 on anything: no transitions (dead)
    nfa->trans_count[2][0] = 0;
    nfa->trans_count[2][1] = 0;
    // State 2 is accepting
    nfa->accepting[2] = 1;
}

int set_contains(int *set, int size, int val) {
    int i;
    for (i = 0; i < size; i++) {
        if (set[i] == val) return 1;
    }
    return 0;
}

int set_add(int *set, int size, int val) {
    if (set_contains(set, size, val)) return size;
    set[size] = val;
    return size + 1;
}

int run_nfa(struct NFA *nfa, int *input, int len) {
    int current[10];
    int next[10];
    int cur_size = 1;
    int next_size;
    int i; int j; int k;

    current[0] = nfa->start_state;

    for (i = 0; i < len; i++) {
        next_size = 0;
        int sym = input[i];
        for (j = 0; j < cur_size; j++) {
            int st = current[j];
            int tc = nfa->trans_count[st][sym];
            for (k = 0; k < tc; k++) {
                int ns = nfa->transitions[st][sym][k];
                next_size = set_add(next, next_size, ns);
            }
        }
        for (j = 0; j < next_size; j++) {
            current[j] = next[j];
        }
        cur_size = next_size;
    }

    for (i = 0; i < cur_size; i++) {
        if (nfa->accepting[current[i]]) return 1;
    }
    return 0;
}

void print_state_set(int *set, int size) {
    int i;
    printf("{");
    for (i = 0; i < size; i++) {
        if (i > 0) printf(",");
        printf("%d", set[i]);
    }
    printf("}");
}

int main() {
    struct NFA nfa;
    init_nfa(&nfa);

    // Test "ab" -> should accept (ends with ab)
    int t1[2]; t1[0] = 0; t1[1] = 1;
    int r1 = run_nfa(&nfa, t1, 2);
    // EXPECT: NFA 'ab': accepted=1
    printf("NFA 'ab': accepted=%d\n", r1);

    // Test "aab" -> should accept
    int t2[3]; t2[0] = 0; t2[1] = 0; t2[2] = 1;
    int r2 = run_nfa(&nfa, t2, 3);
    // EXPECT: NFA 'aab': accepted=1
    printf("NFA 'aab': accepted=%d\n", r2);

    // Test "ba" -> should not accept
    int t3[2]; t3[0] = 1; t3[1] = 0;
    int r3 = run_nfa(&nfa, t3, 2);
    // EXPECT: NFA 'ba': accepted=0
    printf("NFA 'ba': accepted=%d\n", r3);

    // Test "bab" -> should accept
    int t4[3]; t4[0] = 1; t4[1] = 0; t4[2] = 1;
    int r4 = run_nfa(&nfa, t4, 3);
    // EXPECT: NFA 'bab': accepted=1
    printf("NFA 'bab': accepted=%d\n", r4);

    // Test "b" -> should not accept
    int t5[1]; t5[0] = 1;
    int r5 = run_nfa(&nfa, t5, 1);
    // EXPECT: NFA 'b': accepted=0
    printf("NFA 'b': accepted=%d\n", r5);

    // Test "a" -> should not accept
    int t6[1]; t6[0] = 0;
    int r6 = run_nfa(&nfa, t6, 1);
    // EXPECT: NFA 'a': accepted=0
    printf("NFA 'a': accepted=%d\n", r6);

    // Test "abab" -> should accept
    int t7[4]; t7[0] = 0; t7[1] = 1; t7[2] = 0; t7[3] = 1;
    int r7 = run_nfa(&nfa, t7, 4);
    // EXPECT: NFA 'abab': accepted=1
    printf("NFA 'abab': accepted=%d\n", r7);

    // Test "bb" -> should not accept
    int t8[2]; t8[0] = 1; t8[1] = 1;
    int r8 = run_nfa(&nfa, t8, 2);
    // EXPECT: NFA 'bb': accepted=0
    printf("NFA 'bb': accepted=%d\n", r8);

    // Test "aabb" -> should not accept (ends with bb not ab)
    int t9[4]; t9[0] = 0; t9[1] = 0; t9[2] = 1; t9[3] = 1;
    int r9 = run_nfa(&nfa, t9, 4);
    // EXPECT: NFA 'aabb': accepted=0
    printf("NFA 'aabb': accepted=%d\n", r9);

    // Count accepted strings of length 3
    int count = 0;
    int bits[3];
    int a; int b; int c;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                bits[0] = a; bits[1] = b; bits[2] = c;
                if (run_nfa(&nfa, bits, 3)) {
                    count = count + 1;
                }
            }
        }
    }
    // EXPECT: Strings of length 3 accepted: 2
    printf("Strings of length 3 accepted: %d\n", count);

    return 0;
}
