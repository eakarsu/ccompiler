int printf(const char *fmt, ...);

// DFA Simulator
// Simulates a DFA that accepts binary strings divisible by 3
// States: 0 (remainder 0, accept), 1 (remainder 1), 2 (remainder 2)

struct DFA {
    int num_states;
    int num_symbols;
    int start_state;
    int transitions[10][2]; // transitions[state][symbol] = next_state
    int accepting[10];      // 1 if accepting state
};

void init_dfa(struct DFA *dfa) {
    dfa->num_states = 3;
    dfa->num_symbols = 2;
    dfa->start_state = 0;

    // State 0: remainder 0
    dfa->transitions[0][0] = 0; // 0 on '0' -> 0
    dfa->transitions[0][1] = 1; // 0 on '1' -> 1
    // State 1: remainder 1
    dfa->transitions[1][0] = 2; // 1 on '0' -> 2
    dfa->transitions[1][1] = 0; // 1 on '1' -> 0
    // State 2: remainder 2
    dfa->transitions[2][0] = 1; // 2 on '0' -> 1
    dfa->transitions[2][1] = 2; // 2 on '1' -> 2

    dfa->accepting[0] = 1;
    dfa->accepting[1] = 0;
    dfa->accepting[2] = 0;
}

int run_dfa(struct DFA *dfa, int *input, int len) {
    int state = dfa->start_state;
    int i;
    for (i = 0; i < len; i++) {
        state = dfa->transitions[state][input[i]];
    }
    return dfa->accepting[state];
}

int trace_dfa(struct DFA *dfa, int *input, int len, int *trace_buf) {
    int state = dfa->start_state;
    trace_buf[0] = state;
    int i;
    for (i = 0; i < len; i++) {
        state = dfa->transitions[state][input[i]];
        trace_buf[i + 1] = state;
    }
    return len + 1;
}

int main() {
    struct DFA dfa;
    init_dfa(&dfa);

    // Test: "110" = 6, divisible by 3
    int input1[3];
    input1[0] = 1; input1[1] = 1; input1[2] = 0;
    int r1 = run_dfa(&dfa, input1, 3);
    // EXPECT: DFA test '110': accepted=1
    printf("DFA test '110': accepted=%d\n", r1);

    // Test: "101" = 5, not divisible by 3
    int input2[3];
    input2[0] = 1; input2[1] = 0; input2[2] = 1;
    int r2 = run_dfa(&dfa, input2, 3);
    // EXPECT: DFA test '101': accepted=0
    printf("DFA test '101': accepted=%d\n", r2);

    // Test: "1001" = 9, divisible by 3
    int input3[4];
    input3[0] = 1; input3[1] = 0; input3[2] = 0; input3[3] = 1;
    int r3 = run_dfa(&dfa, input3, 4);
    // EXPECT: DFA test '1001': accepted=1
    printf("DFA test '1001': accepted=%d\n", r3);

    // Test: empty string = 0, divisible by 3
    int r4 = run_dfa(&dfa, input1, 0);
    // EXPECT: DFA test '': accepted=1
    printf("DFA test '': accepted=%d\n", r4);

    // Test: "1" = 1, not divisible by 3
    int input5[1];
    input5[0] = 1;
    int r5 = run_dfa(&dfa, input5, 1);
    // EXPECT: DFA test '1': accepted=0
    printf("DFA test '1': accepted=%d\n", r5);

    // Test: "11" = 3, divisible by 3
    int input6[2];
    input6[0] = 1; input6[1] = 1;
    int r6 = run_dfa(&dfa, input6, 2);
    // EXPECT: DFA test '11': accepted=1
    printf("DFA test '11': accepted=%d\n", r6);

    // Trace through "100100" = 36
    int input7[6];
    input7[0] = 1; input7[1] = 0; input7[2] = 0;
    input7[3] = 1; input7[4] = 0; input7[5] = 0;
    int trace[10];
    int tlen = trace_dfa(&dfa, input7, 6, trace);
    // EXPECT: Trace: S0 -> S1 -> S2 -> S1 -> S0 -> S0 -> S0
    printf("Trace: S%d", trace[0]);
    int t;
    for (t = 1; t < tlen; t++) {
        printf(" -> S%d", trace[t]);
    }
    printf("\n");

    int r7 = run_dfa(&dfa, input7, 6);
    // EXPECT: DFA test '100100': accepted=1
    printf("DFA test '100100': accepted=%d\n", r7);

    // Count how many 3-bit strings are accepted
    int count = 0;
    int bits[3];
    int a; int b; int c;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                bits[0] = a; bits[1] = b; bits[2] = c;
                if (run_dfa(&dfa, bits, 3)) {
                    count = count + 1;
                }
            }
        }
    }
    // EXPECT: 3-bit strings divisible by 3: 3
    printf("3-bit strings divisible by 3: %d\n", count);

    // Test all 4-bit binary numbers
    int accepted_4bit = 0;
    int bits4[4];
    int d;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                for (d = 0; d < 2; d++) {
                    bits4[0] = a; bits4[1] = b; bits4[2] = c; bits4[3] = d;
                    if (run_dfa(&dfa, bits4, 4)) {
                        accepted_4bit = accepted_4bit + 1;
                    }
                }
            }
        }
    }
    // EXPECT: 4-bit strings divisible by 3: 6
    printf("4-bit strings divisible by 3: %d\n", accepted_4bit);

    return 0;
}
