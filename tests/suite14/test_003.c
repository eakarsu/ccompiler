int printf(const char *fmt, ...);

/* Mealy machine: output depends on current state AND current input.
   This Mealy machine detects overlapping "101" patterns in a binary stream.
   States: S0 (initial), S1 (seen 1), S2 (seen 10)
   Output: 1 when pattern "101" is completed, 0 otherwise.

   Transitions (state, input) -> (next_state, output):
   (S0, 0) -> (S0, 0)
   (S0, 1) -> (S1, 0)
   (S1, 0) -> (S2, 0)
   (S1, 1) -> (S1, 0)
   (S2, 0) -> (S0, 0)
   (S2, 1) -> (S1, 1)  <-- pattern detected!
*/

int next_state_table[3][2];
int output_table[3][2];

void init_mealy(void) {
    /* S0 transitions */
    next_state_table[0][0] = 0;  /* S0, 0 -> S0 */
    next_state_table[0][1] = 1;  /* S0, 1 -> S1 */
    output_table[0][0] = 0;
    output_table[0][1] = 0;

    /* S1 transitions */
    next_state_table[1][0] = 2;  /* S1, 0 -> S2 */
    next_state_table[1][1] = 1;  /* S1, 1 -> S1 */
    output_table[1][0] = 0;
    output_table[1][1] = 0;

    /* S2 transitions */
    next_state_table[2][0] = 0;  /* S2, 0 -> S0 */
    next_state_table[2][1] = 1;  /* S2, 1 -> S1 */
    output_table[2][0] = 0;
    output_table[2][1] = 1;      /* pattern detected! */
}

struct mealy_result {
    int next_state;
    int output;
};

void mealy_step(int state, int input, struct mealy_result *r) {
    r->next_state = next_state_table[state][input];
    r->output = output_table[state][input];
}

int count_patterns(int *bits, int len) {
    int state = 0;
    int count = 0;
    int i;
    struct mealy_result r;

    for (i = 0; i < len; i++) {
        mealy_step(state, bits[i], &r);
        count = count + r.output;
        state = r.next_state;
    }
    return count;
}

void trace_mealy(int *bits, int len) {
    int state = 0;
    int i;
    struct mealy_result r;

    for (i = 0; i < len; i++) {
        mealy_step(state, bits[i], &r);
        printf("  S%d + %d -> S%d / %d\n", state, bits[i], r.next_state, r.output);
        state = r.next_state;
    }
}

void test_basic_patterns(void) {
    int b1[3];
    int b2[5];
    int b3[7];
    int b4[4];
    int c;

    printf("=== Mealy Basic Tests ===\n");
    // EXPECT: === Mealy Basic Tests ===

    /* "101" -> one pattern */
    b1[0] = 1; b1[1] = 0; b1[2] = 1;
    c = count_patterns(b1, 3);
    printf("101: %d\n", c);
    // EXPECT: 101: 1

    /* "10101" -> two overlapping "101" patterns */
    b2[0] = 1; b2[1] = 0; b2[2] = 1; b2[3] = 0; b2[4] = 1;
    c = count_patterns(b2, 5);
    printf("10101: %d\n", c);
    // EXPECT: 10101: 2

    /* "1010101" -> three overlapping patterns */
    b3[0] = 1; b3[1] = 0; b3[2] = 1; b3[3] = 0;
    b3[4] = 1; b3[5] = 0; b3[6] = 1;
    c = count_patterns(b3, 7);
    printf("1010101: %d\n", c);
    // EXPECT: 1010101: 3

    /* "1100" -> no pattern */
    b4[0] = 1; b4[1] = 1; b4[2] = 0; b4[3] = 0;
    c = count_patterns(b4, 4);
    printf("1100: %d\n", c);
    // EXPECT: 1100: 0
}

void test_trace(void) {
    int b[5];

    printf("=== Mealy Trace ===\n");
    // EXPECT: === Mealy Trace ===

    /* Trace "10101" */
    b[0] = 1; b[1] = 0; b[2] = 1; b[3] = 0; b[4] = 1;
    printf("Tracing 10101:\n");
    // EXPECT: Tracing 10101:
    trace_mealy(b, 5);
    // EXPECT:   S0 + 1 -> S1 / 0
    // EXPECT:   S1 + 0 -> S2 / 0
    // EXPECT:   S2 + 1 -> S1 / 1
    // EXPECT:   S1 + 0 -> S2 / 0
    // EXPECT:   S2 + 1 -> S1 / 1
}

void test_no_match(void) {
    int b1[4];
    int b2[3];
    int b3[1];
    int c;

    printf("=== Mealy No Match ===\n");
    // EXPECT: === Mealy No Match ===

    /* "0000" -> no match */
    b1[0] = 0; b1[1] = 0; b1[2] = 0; b1[3] = 0;
    c = count_patterns(b1, 4);
    printf("0000: %d\n", c);
    // EXPECT: 0000: 0

    /* "111" -> no match */
    b2[0] = 1; b2[1] = 1; b2[2] = 1;
    c = count_patterns(b2, 3);
    printf("111: %d\n", c);
    // EXPECT: 111: 0

    /* "0" -> no match */
    b3[0] = 0;
    c = count_patterns(b3, 1);
    printf("0: %d\n", c);
    // EXPECT: 0: 0

    /* empty -> no match */
    c = count_patterns(b3, 0);
    printf("empty: %d\n", c);
    // EXPECT: empty: 0
}

void test_complex_stream(void) {
    int b[12];
    int c;
    int state;
    int i;
    struct mealy_result r;

    printf("=== Complex Stream ===\n");
    // EXPECT: === Complex Stream ===

    /* "110100101011" */
    b[0]=1; b[1]=1; b[2]=0; b[3]=1; b[4]=0; b[5]=0;
    b[6]=1; b[7]=0; b[8]=1; b[9]=0; b[10]=1; b[11]=1;
    c = count_patterns(b, 12);
    printf("110100101011: %d\n", c);
    // EXPECT: 110100101011: 3

    /* Verify by tracing: let's find final state */
    state = 0;
    for (i = 0; i < 12; i++) {
        mealy_step(state, b[i], &r);
        state = r.next_state;
    }
    printf("final state: S%d\n", state);
    // EXPECT: final state: S1
}

int main(void) {
    init_mealy();
    test_basic_patterns();
    test_trace();
    test_no_match();
    test_complex_stream();

    printf("Mealy machine done.\n");
    // EXPECT: Mealy machine done.
    return 0;
}
