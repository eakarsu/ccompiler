int printf(const char *fmt, ...);

/* NFA simulation with epsilon transitions.
   NFA recognizes strings matching pattern: (a|b)*abb
   States: 0,1,2,3 (3 is accept)
   Simulated via subset construction at runtime. */

/* Transition table: trans[state][input] gives a bitmask of next states.
   input: 0='a', 1='b', 2=epsilon */
int trans[10][3];

/* Max 10 states, using bitmask for state sets (int has 32 bits, enough) */

void init_nfa(void) {
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 3; j++) {
            trans[i][j] = 0;
        }
    }
    /* State 0: on 'a' -> {0, 1}, on 'b' -> {0} */
    trans[0][0] = (1 << 0) | (1 << 1);  /* a -> {0,1} */
    trans[0][1] = (1 << 0);             /* b -> {0} */

    /* State 1: on 'b' -> {2} */
    trans[1][1] = (1 << 2);             /* b -> {2} */

    /* State 2: on 'b' -> {3} */
    trans[2][1] = (1 << 3);             /* b -> {3} */

    /* State 3: accept state (no transitions needed for this NFA) */
}

int epsilon_closure(int state_set) {
    /* For this NFA, no epsilon transitions, so closure is identity */
    return state_set;
}

int nfa_move(int state_set, int input) {
    int result = 0;
    int s;
    for (s = 0; s < 10; s++) {
        if (state_set & (1 << s)) {
            result = result | trans[s][input];
        }
    }
    return epsilon_closure(result);
}

int count_states(int state_set) {
    int count = 0;
    int i;
    for (i = 0; i < 10; i++) {
        if (state_set & (1 << i)) {
            count++;
        }
    }
    return count;
}

int nfa_accepts(int *input, int len) {
    int current = epsilon_closure(1 << 0); /* start in state 0 */
    int i;
    for (i = 0; i < len; i++) {
        current = nfa_move(current, input[i]);
        if (current == 0) return 0;  /* dead */
    }
    return (current & (1 << 3)) != 0; /* check if state 3 in set */
}

int format_state_set(int state_set, char *buf) {
    int i;
    int pos = 0;
    int first = 1;
    buf[pos] = '{'; pos++;
    for (i = 0; i < 10; i++) {
        if (state_set & (1 << i)) {
            if (!first) { buf[pos] = ','; pos++; }
            if (i >= 10) {
                buf[pos] = '0' + (i / 10); pos++;
            }
            buf[pos] = '0' + (i % 10); pos++;
            first = 0;
        }
    }
    buf[pos] = '}'; pos++;
    buf[pos] = 0;
    return pos;
}

void trace_nfa(int *input, int len) {
    int current = epsilon_closure(1 << 0);
    int i;
    int nc;
    printf("Start: %d states\n", count_states(current));
    // EXPECT: === NFA Basic Tests ===
    for (i = 0; i < len; i++) {
        current = nfa_move(current, input[i]);
        nc = count_states(current);
        if (input[i] == 0) {
            printf("  a -> %d states\n", nc);
        } else {
            printf("  b -> %d states\n", nc);
        }
    }
}

void test_basic(void) {
    int in1[3];
    int in2[4];
    int in3[5];
    int in4[6];
    int r;

    printf("=== NFA Basic Tests ===\n");
    // EXPECT: abb: 1

    /* "abb" should accept */
    in1[0] = 0; in1[1] = 1; in1[2] = 1;
    r = nfa_accepts(in1, 3);
    printf("abb: %d\n", r);
    // EXPECT: aabb: 1

    /* "aabb" should accept */
    in2[0] = 0; in2[1] = 0; in2[2] = 1; in2[3] = 1;
    r = nfa_accepts(in2, 4);
    printf("aabb: %d\n", r);
    // EXPECT: babb: 1

    /* "babb" -> should accept */
    in3[0] = 1; in3[1] = 0; in3[2] = 1; in3[3] = 1;
    r = nfa_accepts(in3, 4);
    printf("babb: %d\n", r);
    // EXPECT: ababb: 1

    /* "ababb" should accept */
    in4[0] = 0; in4[1] = 1; in4[2] = 0; in4[3] = 1; in4[4] = 1;
    r = nfa_accepts(in4, 5);
    printf("ababb: %d\n", r);
    // EXPECT: === NFA Reject Tests ===
}

void test_rejects(void) {
    int in1[2];
    int in2[3];
    int in3[4];
    int in4[1];
    int r;

    printf("=== NFA Reject Tests ===\n");
    // EXPECT: ab: 0

    /* "ab" should reject */
    in1[0] = 0; in1[1] = 1;
    r = nfa_accepts(in1, 2);
    printf("ab: %d\n", r);
    // EXPECT: aba: 0

    /* "aba" should reject */
    in2[0] = 0; in2[1] = 1; in2[2] = 0;
    r = nfa_accepts(in2, 3);
    printf("aba: %d\n", r);
    // EXPECT: abba: 0

    /* "abba" should reject */
    in3[0] = 0; in3[1] = 1; in3[2] = 1; in3[3] = 0;
    r = nfa_accepts(in3, 4);
    printf("abba: %d\n", r);
    // EXPECT: a: 0

    /* "a" should reject */
    in4[0] = 0;
    r = nfa_accepts(in4, 1);
    printf("a: %d\n", r);
    // EXPECT: === NFA Trace ===
}

void test_trace(void) {
    int in1[3];

    printf("=== NFA Trace ===\n");
    // EXPECT: Start: 1 states

    /* trace "abb" */
    in1[0] = 0; in1[1] = 1; in1[2] = 1;
    trace_nfa(in1, 3);
    // EXPECT:   a -> 2 states
    // EXPECT:   b -> 2 states
    // EXPECT:   b -> 2 states
}

void test_state_sets(void) {
    int set1;
    int set2;
    int set3;

    printf("=== State Set Operations ===\n");
    // EXPECT: === State Set Operations ===

    set1 = (1 << 0) | (1 << 2) | (1 << 3);
    printf("count {0,2,3}: %d\n", count_states(set1));
    // EXPECT: count {0,2,3}: 3

    set2 = (1 << 1);
    printf("count {1}: %d\n", count_states(set2));
    // EXPECT: count {1}: 1

    set3 = 0;
    printf("count {}: %d\n", count_states(set3));
    // EXPECT: count {}: 0

    {
        char buf[64];
        format_state_set(set1, buf);
        printf("set {0,2,3}: %s\n", buf);
        // EXPECT: set {0,2,3}: {0,2,3}
    }
}

int main(void) {
    init_nfa();
    test_basic();
    test_rejects();
    test_trace();
    test_state_sets();

    printf("NFA simulation done.\n");
    // EXPECT: NFA simulation done.
    return 0;
}
