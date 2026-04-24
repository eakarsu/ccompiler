// stress_058_enum_state.c - Enum-driven state machine stress test
// 10+ states, transition table, 50+ steps, parallel machines, history tracking.
// EXPECT: machine1_final=STATE_HALT
// EXPECT: machine1_steps=52
// EXPECT: machine1_output=AABCBCBCBCBCBCBCBCBCBCBCDDDDDDDDDDDDDDDDDDDDDDDDDEHH
// EXPECT: machine2_final=STATE_S4
// EXPECT: machine2_steps=52
// EXPECT: history_len=52
// EXPECT: state_count_S0=1
// EXPECT: state_count_S1=1
// EXPECT: interleave=ok
// EXPECT: enum_val_check=ok
// EXPECT: state_tests=10

int printf(const char *fmt, ...);

enum State {
    STATE_S0 = 0,
    STATE_S1 = 1,
    STATE_S2 = 2,
    STATE_S3 = 3,
    STATE_S4 = 4,
    STATE_S5 = 5,
    STATE_S6 = 6,
    STATE_S7 = 7,
    STATE_S8 = 8,
    STATE_S9 = 9,
    STATE_HALT = 10,
    NUM_STATES = 11
};

// Transition table: next_state[current_state][input]
// Inputs: 0, 1, 2
int transition[11][3];
char output_char[11]; // output character per state

void init_machine1(void) {
    // State S0: start -> S1 on any input
    transition[STATE_S0][0] = STATE_S1;
    transition[STATE_S0][1] = STATE_S1;
    transition[STATE_S0][2] = STATE_S1;
    output_char[STATE_S0] = 'A';

    // S1 -> S2 on input 0, S3 on input 1, S1 on input 2
    transition[STATE_S1][0] = STATE_S2;
    transition[STATE_S1][1] = STATE_S3;
    transition[STATE_S1][2] = STATE_S1;
    output_char[STATE_S1] = 'A';

    // S2 -> S3 on 0, S2 on 1, S4 on 2
    transition[STATE_S2][0] = STATE_S3;
    transition[STATE_S2][1] = STATE_S2;
    transition[STATE_S2][2] = STATE_S4;
    output_char[STATE_S2] = 'B';

    // S3 -> S2 on 0, S4 on 1, S3 on 2
    transition[STATE_S3][0] = STATE_S2;
    transition[STATE_S3][1] = STATE_S4;
    transition[STATE_S3][2] = STATE_S3;
    output_char[STATE_S3] = 'C';

    // S4 -> S5 on 0, S4 on 1, S3 on 2
    transition[STATE_S4][0] = STATE_S5;
    transition[STATE_S4][1] = STATE_S4;
    transition[STATE_S4][2] = STATE_S3;
    output_char[STATE_S4] = 'D';

    // S5 -> S6 on 0, S4 on 1, S4 on 2
    transition[STATE_S5][0] = STATE_S6;
    transition[STATE_S5][1] = STATE_S4;
    transition[STATE_S5][2] = STATE_S4;
    output_char[STATE_S5] = 'D';

    // S6 -> S7 on 0, S4 on 1, S5 on 2
    transition[STATE_S6][0] = STATE_S7;
    transition[STATE_S6][1] = STATE_S4;
    transition[STATE_S6][2] = STATE_S5;
    output_char[STATE_S6] = 'D';

    // S7 -> S8 on 0, S4 on 1, S6 on 2
    transition[STATE_S7][0] = STATE_S8;
    transition[STATE_S7][1] = STATE_S4;
    transition[STATE_S7][2] = STATE_S6;
    output_char[STATE_S7] = 'D';

    // S8 -> S9 on 0, S4 on 1, S7 on 2
    transition[STATE_S8][0] = STATE_S9;
    transition[STATE_S8][1] = STATE_S4;
    transition[STATE_S8][2] = STATE_S7;
    output_char[STATE_S8] = 'D';

    // S9 -> HALT on 0, S4 on 1, S8 on 2
    transition[STATE_S9][0] = STATE_HALT;
    transition[STATE_S9][1] = STATE_S4;
    transition[STATE_S9][2] = STATE_S8;
    output_char[STATE_S9] = 'E';

    // HALT is absorbing
    transition[STATE_HALT][0] = STATE_HALT;
    transition[STATE_HALT][1] = STATE_HALT;
    transition[STATE_HALT][2] = STATE_HALT;
    output_char[STATE_HALT] = 'H';
}

char *state_name(int s) {
    if (s == STATE_S0) return "STATE_S0";
    if (s == STATE_S1) return "STATE_S1";
    if (s == STATE_S2) return "STATE_S2";
    if (s == STATE_S3) return "STATE_S3";
    if (s == STATE_S4) return "STATE_S4";
    if (s == STATE_S5) return "STATE_S5";
    if (s == STATE_S6) return "STATE_S6";
    if (s == STATE_S7) return "STATE_S7";
    if (s == STATE_S8) return "STATE_S8";
    if (s == STATE_S9) return "STATE_S9";
    if (s == STATE_HALT) return "STATE_HALT";
    return "UNKNOWN";
}

// Input sequence for machine 1 (drives it through many states)
int input_seq[60];

void init_input_seq(void) {
    // Carefully chosen to traverse many states and reach HALT
    // Path: S0->S1->S2->S3->S2->S3->...->S3->S4->S4->...->S4->S5->S6->S7->S8->S9->HALT
    int idx = 0;
    int i;

    // S0 -0-> S1 (step 0)
    input_seq[idx++] = 0;
    // S1 -0-> S2 (step 1)
    input_seq[idx++] = 0;
    // Oscillate S2<->S3 with input 0 for 10 pairs (20 steps, steps 2-21)
    // S2-0->S3, S3-0->S2, S2-0->S3, ... after 20 steps we're at S2 (even count from S2)
    for (i = 0; i < 20; i++) {
        input_seq[idx++] = 0;
    }
    // Now at S2 (step 22). S2-0->S3
    input_seq[idx++] = 0;
    // Now at S3 (step 23). S3-1->S4
    input_seq[idx++] = 1;
    // Now at S4 (step 24). Loop in S4 with input 1 for 20 steps (steps 24-43)
    for (i = 0; i < 20; i++) {
        input_seq[idx++] = 1; // S4-1->S4
    }
    // Now at S4 (step 44). Chain of 0s: S4->S5->S6->S7->S8->S9->HALT (6 steps)
    for (i = 0; i < 6; i++) {
        input_seq[idx++] = 0;
    }
    // idx should be 50. Fill remaining with 0s.
    while (idx < 52) {
        input_seq[idx++] = 0;
    }
    // Total 52 steps. After step 49 we reach HALT. Steps 50,51 stay in HALT.
}

struct Machine {
    int state;
    int steps;
    int history[64];
    int hist_len;
    char output[64];
    int out_len;
};

void machine_init(struct Machine *m, int start_state) {
    m->state = start_state;
    m->steps = 0;
    m->hist_len = 0;
    m->out_len = 0;
}

void machine_step(struct Machine *m, int input) {
    m->history[m->hist_len++] = m->state;
    m->output[m->out_len++] = output_char[m->state];
    m->state = transition[m->state][input];
    m->steps++;
}

int main(void) {
    int tests = 0;

    init_machine1();
    init_input_seq();

    // Run machine 1
    struct Machine m1;
    machine_init(&m1, STATE_S0);
    int i;
    for (i = 0; i < 52; i++) {
        machine_step(&m1, input_seq[i]);
    }
    m1.output[m1.out_len] = 0;
    printf("machine1_final=%s\n", state_name(m1.state));
    tests++;

    printf("machine1_steps=%d\n", m1.steps);
    tests++;

    printf("machine1_output=%s\n", m1.output);
    tests++;

    // Run machine 2 with different input: all input 1
    struct Machine m2;
    machine_init(&m2, STATE_S0);
    for (i = 0; i < 52; i++) {
        machine_step(&m2, 1);
    }
    m2.output[m2.out_len] = 0;
    printf("machine2_final=%s\n", state_name(m2.state));
    tests++;

    printf("machine2_steps=%d\n", m2.steps);
    tests++;

    // History analysis
    printf("history_len=%d\n", m1.hist_len);
    tests++;

    // Count how many times machine1 was in S0
    int s0_count = 0;
    for (i = 0; i < m1.hist_len; i++) {
        if (m1.history[i] == STATE_S0) s0_count++;
    }
    printf("state_count_S0=%d\n", s0_count);
    tests++;

    // Count S1
    int s1_count = 0;
    for (i = 0; i < m1.hist_len; i++) {
        if (m1.history[i] == STATE_S1) s1_count++;
    }
    printf("state_count_S1=%d\n", s1_count);
    tests++;

    // Interleaved execution: step m1 and m2 alternately
    struct Machine m3;
    struct Machine m4;
    machine_init(&m3, STATE_S0);
    machine_init(&m4, STATE_S0);
    for (i = 0; i < 10; i++) {
        machine_step(&m3, 0);
        machine_step(&m4, 1);
    }
    // Both should have done 10 steps
    if (m3.steps == 10 && m4.steps == 10)
        printf("interleave=ok\n");
    else
        printf("interleave=FAIL\n");
    tests++;

    // Verify enum values
    if (STATE_S0 == 0 && STATE_S5 == 5 && STATE_HALT == 10 && NUM_STATES == 11)
        printf("enum_val_check=ok\n");
    else
        printf("enum_val_check=FAIL\n");
    tests++;

    printf("state_tests=%d\n", tests);

    return 0;
}
