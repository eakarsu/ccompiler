int printf(const char *fmt, ...);

// Finite State Transducer (FST)
// An FST reads input and produces output on each transition
// This FST converts binary to Gray code

struct FSTRule {
    int from_state;
    int input_sym;
    int to_state;
    int output_sym;
};

struct FST {
    int num_states;
    int num_rules;
    struct FSTRule rules[20];
    int state;
    int start_state;
};

void fst_init(struct FST *fst) {
    fst->num_states = 2;
    fst->num_rules = 0;
    fst->start_state = 0;
    fst->state = 0;
}

void fst_add_rule(struct FST *fst, int fs, int is, int ts, int os) {
    int n = fst->num_rules;
    fst->rules[n].from_state = fs;
    fst->rules[n].input_sym = is;
    fst->rules[n].to_state = ts;
    fst->rules[n].output_sym = os;
    fst->num_rules = n + 1;
}

int fst_step(struct FST *fst, int input, int *output) {
    int i;
    for (i = 0; i < fst->num_rules; i++) {
        if (fst->rules[i].from_state == fst->state && fst->rules[i].input_sym == input) {
            *output = fst->rules[i].output_sym;
            fst->state = fst->rules[i].to_state;
            return 1;
        }
    }
    return 0;
}

void fst_reset(struct FST *fst) {
    fst->state = fst->start_state;
}

// Setup Binary to Gray code FST
// Gray code: g[0] = b[0], g[i] = b[i-1] XOR b[i]
// State 0: last bit was 0 (or start)
// State 1: last bit was 1
void setup_binary_to_gray(struct FST *fst) {
    fst_init(fst);
    // State 0 (last=0), input 0 -> output 0, stay 0
    fst_add_rule(fst, 0, 0, 0, 0);
    // State 0 (last=0), input 1 -> output 1, go to 1
    fst_add_rule(fst, 0, 1, 1, 1);
    // State 1 (last=1), input 0 -> output 1, go to 0
    fst_add_rule(fst, 1, 0, 0, 1);
    // State 1 (last=1), input 1 -> output 0, stay 1
    fst_add_rule(fst, 1, 1, 1, 0);
}

void run_fst(struct FST *fst, int *input, int len, int *output) {
    fst_reset(fst);
    int i;
    for (i = 0; i < len; i++) {
        fst_step(fst, input[i], &output[i]);
    }
}

void print_bits(int *bits, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%d", bits[i]);
    }
}

int bits_to_int(int *bits, int len) {
    int val = 0;
    int i;
    for (i = 0; i < len; i++) {
        val = val * 2 + bits[i];
    }
    return val;
}

// Verify Gray code property: adjacent values differ by exactly 1 bit
int count_diff_bits(int *a, int *b, int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i]) count = count + 1;
    }
    return count;
}

int main() {
    struct FST fst;
    setup_binary_to_gray(&fst);

    int input[4];
    int output[4];

    // Test: binary 0000 -> Gray 0000
    input[0] = 0; input[1] = 0; input[2] = 0; input[3] = 0;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 0000 -> Gray 0000

    // Test: binary 0001 -> Gray 0001
    input[0] = 0; input[1] = 0; input[2] = 0; input[3] = 1;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 0001 -> Gray 0001

    // Test: binary 0010 -> Gray 0011
    input[0] = 0; input[1] = 0; input[2] = 1; input[3] = 0;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 0010 -> Gray 0011

    // Test: binary 0111 -> Gray 0100
    input[0] = 0; input[1] = 1; input[2] = 1; input[3] = 1;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 0111 -> Gray 0100

    // Test: binary 1010 -> Gray 1111
    input[0] = 1; input[1] = 0; input[2] = 1; input[3] = 0;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 1010 -> Gray 1111

    // Test: binary 1111 -> Gray 1000
    input[0] = 1; input[1] = 1; input[2] = 1; input[3] = 1;
    run_fst(&fst, input, 4, output);
    printf("Bin ");
    print_bits(input, 4);
    printf(" -> Gray ");
    print_bits(output, 4);
    printf("\n");
    // EXPECT: Bin 1111 -> Gray 1000

    // Verify Gray code property: generate all 3-bit Gray codes and check
    // adjacent codes differ by exactly 1 bit
    int prev_gray[3];
    int cur_gray[3];
    int cur_bin[3];
    int a; int b; int c;
    int first = 1;
    int count = 0;
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                cur_bin[0] = a; cur_bin[1] = b; cur_bin[2] = c;
                run_fst(&fst, cur_bin, 3, cur_gray);
                if (first == 0) {
                    int diff = count_diff_bits(prev_gray, cur_gray, 3);
                    if (diff == 1) count = count + 1;
                }
                prev_gray[0] = cur_gray[0];
                prev_gray[1] = cur_gray[1];
                prev_gray[2] = cur_gray[2];
                first = 0;
            }
        }
    }
    // Adjacent binary numbers: consecutive Gray codes often differ by 1 bit
    // EXPECT: Adjacent Gray codes with 1-bit diff: 7
    printf("Adjacent Gray codes with 1-bit diff: %d\n", count);

    // EXPECT: FST tests complete
    printf("FST tests complete\n");

    return 0;
}
