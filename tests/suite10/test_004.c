int printf(const char *fmt, ...);

// Turing Machine Simulator
// Simulates a simple TM that increments a binary number by 1
// Tape alphabet: '0', '1', 'B' (blank)

enum Direction { LEFT, RIGHT, STAY };

struct TMRule {
    int from_state;
    char read_sym;
    int to_state;
    char write_sym;
    int direction;
};

struct TuringMachine {
    int num_rules;
    struct TMRule rules[20];
    int state;
    char tape[30];
    int head;
    int tape_used; // how many cells contain data
    int accept_state;
};

void tm_init(struct TuringMachine *tm) {
    tm->num_rules = 0;
    tm->state = 0;
    tm->head = 0;
    tm->tape_used = 0;
    tm->accept_state = 2;
    int i;
    for (i = 0; i < 30; i++) {
        tm->tape[i] = 'B';
    }
}

void tm_add_rule(struct TuringMachine *tm, int fs, char rs, int ts, char ws, int dir) {
    int n = tm->num_rules;
    tm->rules[n].from_state = fs;
    tm->rules[n].read_sym = rs;
    tm->rules[n].to_state = ts;
    tm->rules[n].write_sym = ws;
    tm->rules[n].direction = dir;
    tm->num_rules = n + 1;
}

void tm_set_input(struct TuringMachine *tm, char *input) {
    // Write input starting at position 1 (leave position 0 as blank for overflow)
    int i = 0;
    while (input[i] != 0) {
        tm->tape[i + 1] = input[i];
        i = i + 1;
    }
    tm->tape_used = i + 1; // includes leading blank
    tm->head = i; // point to last digit (rightmost)
    tm->state = 0;
}

int tm_step(struct TuringMachine *tm) {
    if (tm->state == tm->accept_state) return 0;
    char sym = tm->tape[tm->head];
    int i;
    for (i = 0; i < tm->num_rules; i++) {
        if (tm->rules[i].from_state == tm->state && tm->rules[i].read_sym == sym) {
            tm->tape[tm->head] = tm->rules[i].write_sym;
            tm->state = tm->rules[i].to_state;
            if (tm->rules[i].direction == LEFT) {
                if (tm->head > 0) tm->head = tm->head - 1;
            } else if (tm->rules[i].direction == RIGHT) {
                tm->head = tm->head + 1;
            }
            return 1;
        }
    }
    return 0;
}

void tm_run(struct TuringMachine *tm, int max_steps) {
    int steps = 0;
    while (tm_step(tm) && steps < max_steps) {
        steps = steps + 1;
    }
}

void tm_print_result(struct TuringMachine *tm) {
    // Find first non-blank and last non-blank
    int first = -1;
    int last = -1;
    int i;
    for (i = 0; i < 30; i++) {
        if (tm->tape[i] != 'B') {
            if (first == -1) first = i;
            last = i;
        }
    }
    if (first == -1) {
        printf("0");
        return;
    }
    for (i = first; i <= last; i++) {
        if (tm->tape[i] == '0') printf("0");
        else if (tm->tape[i] == '1') printf("1");
        else printf("B");
    }
}

void setup_increment_tm(struct TuringMachine *tm) {
    // State 0: scanning right-to-left, carry propagation
    // On '1': write '0', stay in state 0, move LEFT (carry)
    tm_add_rule(tm, 0, '1', 0, '0', LEFT);
    // On '0': write '1', go to state 1 (done), stay
    tm_add_rule(tm, 0, '0', 1, '1', STAY);
    // On 'B': write '1', go to state 1 (overflow into blank)
    tm_add_rule(tm, 0, 'B', 1, '1', STAY);
    // State 1: halt -> accept
    tm_add_rule(tm, 1, '0', 2, '0', STAY);
    tm_add_rule(tm, 1, '1', 2, '1', STAY);
    tm_add_rule(tm, 1, 'B', 2, 'B', STAY);
}

int main() {
    struct TuringMachine tm;

    // Test 1: increment "101" (5) -> "110" (6)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "101");
    tm_run(&tm, 100);
    printf("Inc 101 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 101 -> 110 state=2

    // Test 2: increment "111" (7) -> "1000" (8)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "111");
    tm_run(&tm, 100);
    printf("Inc 111 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 111 -> 1000 state=2

    // Test 3: increment "0" (0) -> "1" (1)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "0");
    tm_run(&tm, 100);
    printf("Inc 0 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 0 -> 1 state=2

    // Test 4: increment "1" (1) -> "10" (2)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "1");
    tm_run(&tm, 100);
    printf("Inc 1 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 1 -> 10 state=2

    // Test 5: increment "1010" (10) -> "1011" (11)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "1010");
    tm_run(&tm, 100);
    printf("Inc 1010 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 1010 -> 1011 state=2

    // Test 6: increment "1011" (11) -> "1100" (12)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "1011");
    tm_run(&tm, 100);
    printf("Inc 1011 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 1011 -> 1100 state=2

    // Test 7: increment "1111" (15) -> "10000" (16)
    tm_init(&tm);
    setup_increment_tm(&tm);
    tm_set_input(&tm, "1111");
    tm_run(&tm, 100);
    printf("Inc 1111 -> ");
    tm_print_result(&tm);
    printf(" state=%d\n", tm.state);
    // EXPECT: Inc 1111 -> 10000 state=2

    // EXPECT: TM increment tests complete
    printf("TM increment tests complete\n");

    return 0;
}
