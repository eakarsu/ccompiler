int printf(const char *fmt, ...);

/* DFA: Accepts binary strings whose decimal value is divisible by 3.
   States: 0 (remainder 0), 1 (remainder 1), 2 (remainder 2).
   Transition on bit b: next_state = (current_state * 2 + b) % 3
   Accept state: 0 */

int dfa_transition(int state, int bit) {
    return (state * 2 + bit) % 3;
}

int dfa_run(int *bits, int len) {
    int state = 0;
    int i;
    for (i = 0; i < len; i++) {
        state = dfa_transition(state, bits[i]);
    }
    return state;
}

int dfa_accepts(int *bits, int len) {
    return dfa_run(bits, len) == 0;
}

void trace_dfa(int *bits, int len) {
    int state = 0;
    int i;
    printf("Trace start: S0\n");
    // EXPECT: === Basic DFA Tests ===
    for (i = 0; i < len; i++) {
        state = dfa_transition(state, bits[i]);
        printf("  bit %d -> S%d\n", bits[i], state);
    }
}

void test_basic_strings(void) {
    int b0[1];
    int b3[2];
    int b6[3];
    int b7[3];
    int b9[4];
    int b5[3];
    int result;

    printf("=== Basic DFA Tests ===\n");
    // EXPECT: 0 div3: 1

    /* "0" = 0, divisible by 3 */
    b0[0] = 0;
    result = dfa_accepts(b0, 1);
    printf("0 div3: %d\n", result);
    // EXPECT: 3 div3: 1

    /* "11" = 3, divisible by 3 */
    b3[0] = 1; b3[1] = 1;
    result = dfa_accepts(b3, 2);
    printf("3 div3: %d\n", result);
    // EXPECT: 6 div3: 1

    /* "110" = 6, divisible by 3 */
    b6[0] = 1; b6[1] = 1; b6[2] = 0;
    result = dfa_accepts(b6, 3);
    printf("6 div3: %d\n", result);
    // EXPECT: 7 div3: 0

    /* "111" = 7, NOT divisible by 3 */
    b7[0] = 1; b7[1] = 1; b7[2] = 1;
    result = dfa_accepts(b7, 3);
    printf("7 div3: %d\n", result);
    // EXPECT: 9 div3: 1

    /* "1001" = 9, divisible by 3 */
    b9[0] = 1; b9[1] = 0; b9[2] = 0; b9[3] = 1;
    result = dfa_accepts(b9, 4);
    printf("9 div3: %d\n", result);
    // EXPECT: 5 div3: 0

    /* "101" = 5, NOT divisible by 3 */
    b5[0] = 1; b5[1] = 0; b5[2] = 1;
    result = dfa_accepts(b5, 3);
    printf("5 div3: %d\n", result);
    // EXPECT: === Longer Strings ===
}

void test_longer_strings(void) {
    int b12[4];
    int b15[4];
    int b21[5];
    int b10[4];
    int result;

    printf("=== Longer Strings ===\n");
    // EXPECT: 12 div3: 1

    /* "1100" = 12, divisible by 3 */
    b12[0] = 1; b12[1] = 1; b12[2] = 0; b12[3] = 0;
    result = dfa_accepts(b12, 4);
    printf("12 div3: %d\n", result);
    // EXPECT: 15 div3: 1

    /* "1111" = 15, divisible by 3 */
    b15[0] = 1; b15[1] = 1; b15[2] = 1; b15[3] = 1;
    result = dfa_accepts(b15, 4);
    printf("15 div3: %d\n", result);
    // EXPECT: 21 div3: 1

    /* "10101" = 21, divisible by 3 */
    b21[0] = 1; b21[1] = 0; b21[2] = 1; b21[3] = 0; b21[4] = 1;
    result = dfa_accepts(b21, 5);
    printf("21 div3: %d\n", result);
    // EXPECT: 10 div3: 0

    /* "1010" = 10, NOT divisible by 3 */
    b10[0] = 1; b10[1] = 0; b10[2] = 1; b10[3] = 0;
    result = dfa_accepts(b10, 4);
    printf("10 div3: %d\n", result);
    // EXPECT: === Final State Values ===
}

void test_final_states(void) {
    int bits[6];
    int final_state;

    printf("=== Final State Values ===\n");
    // EXPECT: 4 mod3: 1

    /* "100" = 4, 4 % 3 = 1 */
    bits[0] = 1; bits[1] = 0; bits[2] = 0;
    final_state = dfa_run(bits, 3);
    printf("4 mod3: %d\n", final_state);
    // EXPECT: 5 mod3: 2

    /* "101" = 5, 5 % 3 = 2 */
    bits[0] = 1; bits[1] = 0; bits[2] = 1;
    final_state = dfa_run(bits, 3);
    printf("5 mod3: %d\n", final_state);
    // EXPECT: 24 mod3: 0

    /* "11000" = 24, 24 % 3 = 0 */
    bits[0] = 1; bits[1] = 1; bits[2] = 0; bits[3] = 0; bits[4] = 0;
    final_state = dfa_run(bits, 5);
    printf("24 mod3: %d\n", final_state);
    // EXPECT: 25 mod3: 1

    /* "11001" = 25, 25 % 3 = 1 */
    bits[0] = 1; bits[1] = 1; bits[2] = 0; bits[3] = 0; bits[4] = 1;
    final_state = dfa_run(bits, 5);
    printf("25 mod3: %d\n", final_state);
    // EXPECT: 26 mod3: 2

    /* "11010" = 26, 26 % 3 = 2 */
    bits[0] = 1; bits[1] = 1; bits[2] = 0; bits[3] = 1; bits[4] = 0;
    final_state = dfa_run(bits, 5);
    printf("26 mod3: %d\n", final_state);
    // EXPECT: === Empty String ===
}

void test_empty_string(void) {
    int bits[1];
    int result;

    printf("=== Empty String ===\n");
    // EXPECT: empty div3: 1

    /* Empty string: stay in state 0, accept */
    result = dfa_accepts(bits, 0);
    printf("empty div3: %d\n", result);
    // EXPECT: === Trace Example ===
}

int main(void) {
    int trace_bits[3];

    test_basic_strings();
    test_longer_strings();
    test_final_states();
    test_empty_string();

    printf("=== Trace Example ===\n");
    // EXPECT: Trace start: S0

    /* Trace "011" = 3: state 0 -0-> 0 -1-> 1 -1-> 0 */
    trace_bits[0] = 0; trace_bits[1] = 1; trace_bits[2] = 1;
    trace_dfa(trace_bits, 3);
    // EXPECT:   bit 0 -> S0
    // EXPECT:   bit 1 -> S1
    // EXPECT:   bit 1 -> S0

    printf("All DFA tests done.\n");
    // EXPECT: All DFA tests done.

    return 0;
}
