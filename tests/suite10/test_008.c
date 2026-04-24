int printf(const char *fmt, ...);

// Moore Machine Simulator
// A Moore machine's output depends ONLY on the current state (not input)
// This Moore machine detects if the number of 1s seen so far is divisible by 3
// States: 0 (count%3==0, output "Y"), 1 (count%3==1, output "N"), 2 (count%3==2, output "N")

struct MooreState {
    int transitions[2]; // next state for input 0 and 1
    char output;        // output symbol for this state
};

struct MooreMachine {
    int num_states;
    struct MooreState states[10];
    int current_state;
    int start_state;
};

void moore_init(struct MooreMachine *m) {
    m->num_states = 3;
    m->start_state = 0;
    m->current_state = 0;

    // State 0: count%3==0, output 'Y' (yes, divisible)
    m->states[0].transitions[0] = 0; // input 0: stay
    m->states[0].transitions[1] = 1; // input 1: count++
    m->states[0].output = 'Y';

    // State 1: count%3==1, output 'N'
    m->states[1].transitions[0] = 1; // input 0: stay
    m->states[1].transitions[1] = 2; // input 1: count++
    m->states[1].output = 'N';

    // State 2: count%3==2, output 'N'
    m->states[2].transitions[0] = 2; // input 0: stay
    m->states[2].transitions[1] = 0; // input 1: wraps to 0
    m->states[2].output = 'N';
}

void moore_reset(struct MooreMachine *m) {
    m->current_state = m->start_state;
}

char moore_output(struct MooreMachine *m) {
    return m->states[m->current_state].output;
}

void moore_step(struct MooreMachine *m, int input) {
    m->current_state = m->states[m->current_state].transitions[input];
}

void run_moore(struct MooreMachine *m, int *input, int len, char *outputs) {
    moore_reset(m);
    // First output is from initial state (before any input)
    outputs[0] = moore_output(m);
    int i;
    for (i = 0; i < len; i++) {
        moore_step(m, input[i]);
        outputs[i + 1] = moore_output(m);
    }
    outputs[len + 1] = 0; // null terminate
}

void print_str(char *s) {
    int i = 0;
    while (s[i] != 0) {
        printf("%s", (s[i] == 'Y') ? "Y" : "N");
        i = i + 1;
    }
}

int main() {
    struct MooreMachine m;
    moore_init(&m);

    // Test 1: input "111" - 0 ones(Y), 1 one(N), 2 ones(N), 3 ones(Y)
    int input1[3];
    input1[0] = 1; input1[1] = 1; input1[2] = 1;
    char out1[5];
    run_moore(&m, input1, 3, out1);
    printf("Input: 111, Outputs: ");
    print_str(out1);
    printf("\n");
    // EXPECT: Input: 111, Outputs: YNNY

    // Test 2: input "101" - 0(Y), 1 one(N), still 1(N), 2 ones(N)
    int input2[3];
    input2[0] = 1; input2[1] = 0; input2[2] = 1;
    char out2[5];
    run_moore(&m, input2, 3, out2);
    printf("Input: 101, Outputs: ");
    print_str(out2);
    printf("\n");
    // EXPECT: Input: 101, Outputs: YNNN

    // Test 3: input "000" - always 0 ones, always Y
    int input3[3];
    input3[0] = 0; input3[1] = 0; input3[2] = 0;
    char out3[5];
    run_moore(&m, input3, 3, out3);
    printf("Input: 000, Outputs: ");
    print_str(out3);
    printf("\n");
    // EXPECT: Input: 000, Outputs: YYYY

    // Test 4: input "110110" - 0(Y),1(N),2(N),2(N),3(Y),3(Y),4(N)
    int input4[6];
    input4[0] = 1; input4[1] = 1; input4[2] = 0;
    input4[3] = 1; input4[4] = 1; input4[5] = 0;
    char out4[8];
    run_moore(&m, input4, 6, out4);
    printf("Input: 110110, Outputs: ");
    print_str(out4);
    printf("\n");
    // EXPECT: Input: 110110, Outputs: YNNNYNN

    // Test 5: single input "1"
    int input5[1];
    input5[0] = 1;
    char out5[3];
    run_moore(&m, input5, 1, out5);
    printf("Input: 1, Outputs: ");
    print_str(out5);
    printf("\n");
    // EXPECT: Input: 1, Outputs: YN

    // Test 6: single input "0"
    int input6[1];
    input6[0] = 0;
    char out6[3];
    run_moore(&m, input6, 1, out6);
    printf("Input: 0, Outputs: ");
    print_str(out6);
    printf("\n");
    // EXPECT: Input: 0, Outputs: YY

    // Verify state transitions manually
    moore_reset(&m);
    // EXPECT: State: 0 Output: Y
    printf("State: %d Output: %s\n", m.current_state, (moore_output(&m) == 'Y') ? "Y" : "N");
    moore_step(&m, 1);
    // EXPECT: State: 1 Output: N
    printf("State: %d Output: %s\n", m.current_state, (moore_output(&m) == 'Y') ? "Y" : "N");
    moore_step(&m, 1);
    // EXPECT: State: 2 Output: N
    printf("State: %d Output: %s\n", m.current_state, (moore_output(&m) == 'Y') ? "Y" : "N");
    moore_step(&m, 1);
    // EXPECT: State: 0 Output: Y
    printf("State: %d Output: %s\n", m.current_state, (moore_output(&m) == 'Y') ? "Y" : "N");
    moore_step(&m, 0);
    // EXPECT: State: 0 Output: Y
    printf("State: %d Output: %s\n", m.current_state, (moore_output(&m) == 'Y') ? "Y" : "N");

    // Count how many of the 8 3-bit inputs end in state 0
    int count_accept = 0;
    int bits[3];
    int a; int b; int c;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                bits[0] = a; bits[1] = b; bits[2] = c;
                moore_reset(&m);
                int k;
                for (k = 0; k < 3; k++) moore_step(&m, bits[k]);
                if (m.current_state == 0) count_accept = count_accept + 1;
            }
        }
    }
    // EXPECT: 3-bit inputs ending in state 0: 2
    printf("3-bit inputs ending in state 0: %d\n", count_accept);

    // EXPECT: Moore machine tests complete
    printf("Moore machine tests complete\n");

    return 0;
}
