int printf(const char *fmt, ...);

// Mealy Machine Simulator
// A Mealy machine outputs depend on current state AND input
// This Mealy machine detects the pattern "01" in a binary stream
// and outputs 1 when the pattern is found

struct MealyRule {
    int from_state;
    int input_sym;
    int to_state;
    int output_sym;
};

struct MealyMachine {
    int num_states;
    int num_rules;
    struct MealyRule rules[20];
    int state;
    int start_state;
};

void mealy_init(struct MealyMachine *m) {
    m->num_states = 2;
    m->num_rules = 0;
    m->start_state = 0;
    m->state = 0;
}

void mealy_add_rule(struct MealyMachine *m, int fs, int is, int ts, int os) {
    int n = m->num_rules;
    m->rules[n].from_state = fs;
    m->rules[n].input_sym = is;
    m->rules[n].to_state = ts;
    m->rules[n].output_sym = os;
    m->num_rules = n + 1;
}

int mealy_step(struct MealyMachine *m, int input) {
    int i;
    for (i = 0; i < m->num_rules; i++) {
        if (m->rules[i].from_state == m->state && m->rules[i].input_sym == input) {
            int out = m->rules[i].output_sym;
            m->state = m->rules[i].to_state;
            return out;
        }
    }
    return -1; // no rule found
}

void mealy_reset(struct MealyMachine *m) {
    m->state = m->start_state;
}

// Setup: detect "01" pattern
// State 0: haven't seen relevant prefix
// State 1: just saw '0'
void setup_pattern_detector(struct MealyMachine *m) {
    mealy_init(m);
    // State 0, input 0 -> go to state 1, output 0
    mealy_add_rule(m, 0, 0, 1, 0);
    // State 0, input 1 -> stay state 0, output 0
    mealy_add_rule(m, 0, 1, 0, 0);
    // State 1, input 1 -> go to state 0, output 1 (found "01"!)
    mealy_add_rule(m, 1, 1, 0, 1);
    // State 1, input 0 -> stay state 1, output 0
    mealy_add_rule(m, 1, 0, 1, 0);
}

void run_mealy(struct MealyMachine *m, int *input, int *output, int len) {
    mealy_reset(m);
    int i;
    for (i = 0; i < len; i++) {
        output[i] = mealy_step(m, input[i]);
    }
}

void print_array(int *arr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%d", arr[i]);
    }
}

int count_ones(int *arr, int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (arr[i] == 1) count = count + 1;
    }
    return count;
}

int main() {
    struct MealyMachine m;
    setup_pattern_detector(&m);

    int input1[6];
    int output1[6];

    // Test 1: "010101" should detect "01" at positions 1, 3, 5
    input1[0] = 0; input1[1] = 1; input1[2] = 0;
    input1[3] = 1; input1[4] = 0; input1[5] = 1;
    run_mealy(&m, input1, output1, 6);
    printf("Input:  ");
    print_array(input1, 6);
    printf("\n");
    // EXPECT: Input:  010101
    printf("Output: ");
    print_array(output1, 6);
    printf("\n");
    // EXPECT: Output: 010101
    int c1 = count_ones(output1, 6);
    // EXPECT: Pattern '01' found 3 times
    printf("Pattern '01' found %d times\n", c1);

    // Test 2: "110010" should detect "01" at position 3 and 5
    int input2[6]; int output2[6];
    input2[0] = 1; input2[1] = 1; input2[2] = 0;
    input2[3] = 0; input2[4] = 1; input2[5] = 0;
    run_mealy(&m, input2, output2, 6);
    printf("Input:  ");
    print_array(input2, 6);
    printf("\n");
    // EXPECT: Input:  110010
    printf("Output: ");
    print_array(output2, 6);
    printf("\n");
    // EXPECT: Output: 000010
    int c2 = count_ones(output2, 6);
    // EXPECT: Pattern '01' found 1 times
    printf("Pattern '01' found %d times\n", c2);

    // Test 3: "1111" should detect no "01"
    int input3[4]; int output3[4];
    input3[0] = 1; input3[1] = 1; input3[2] = 1; input3[3] = 1;
    run_mealy(&m, input3, output3, 4);
    printf("Input:  ");
    print_array(input3, 4);
    printf("\n");
    // EXPECT: Input:  1111
    printf("Output: ");
    print_array(output3, 4);
    printf("\n");
    // EXPECT: Output: 0000
    int c3 = count_ones(output3, 4);
    // EXPECT: Pattern '01' found 0 times
    printf("Pattern '01' found %d times\n", c3);

    // Test 4: "0011" should detect "01" once at position 2
    int input4[4]; int output4[4];
    input4[0] = 0; input4[1] = 0; input4[2] = 1; input4[3] = 1;
    run_mealy(&m, input4, output4, 4);
    printf("Input:  ");
    print_array(input4, 4);
    printf("\n");
    // EXPECT: Input:  0011
    printf("Output: ");
    print_array(output4, 4);
    printf("\n");
    // EXPECT: Output: 0010
    int c4 = count_ones(output4, 4);
    // EXPECT: Pattern '01' found 1 times
    printf("Pattern '01' found %d times\n", c4);

    // Test state tracking
    mealy_reset(&m);
    // EXPECT: Initial state: 0
    printf("Initial state: %d\n", m.state);
    mealy_step(&m, 0);
    // EXPECT: After input 0: state=1
    printf("After input 0: state=%d\n", m.state);
    mealy_step(&m, 1);
    // EXPECT: After input 1: state=0
    printf("After input 1: state=%d\n", m.state);

    // EXPECT: Mealy machine tests complete
    printf("Mealy machine tests complete\n");

    return 0;
}
