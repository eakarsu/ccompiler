int printf(const char *fmt, ...);

// Turing machine simulator
// Tape, head position, state transitions, halt state

enum Direction { DIR_LEFT, DIR_RIGHT, DIR_STAY };

typedef struct {
    int cur_state;
    int read_sym;
    int write_sym;
    int direction;
    int next_state;
} TMRule;

typedef struct {
    int tape[128];
    int tape_len;
    int head;
    int state;
    TMRule rules[64];
    int num_rules;
    int halt_state;
    int steps;
} TuringMachine;

void tm_init(TuringMachine *tm) {
    int i = 0;
    while (i < 128) { tm->tape[i] = 0; i = i + 1; }
    tm->tape_len = 128;
    tm->head = 64;  // Start in middle
    tm->state = 0;
    tm->num_rules = 0;
    tm->halt_state = -1;
    tm->steps = 0;
}

void tm_add_rule(TuringMachine *tm, int cs, int rs, int ws, int dir, int ns) {
    int idx = tm->num_rules;
    tm->rules[idx].cur_state = cs;
    tm->rules[idx].read_sym = rs;
    tm->rules[idx].write_sym = ws;
    tm->rules[idx].direction = dir;
    tm->rules[idx].next_state = ns;
    tm->num_rules = tm->num_rules + 1;
}

int tm_find_rule(TuringMachine *tm) {
    int i = 0;
    int sym = tm->tape[tm->head];
    while (i < tm->num_rules) {
        if (tm->rules[i].cur_state == tm->state && tm->rules[i].read_sym == sym) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int tm_step(TuringMachine *tm) {
    if (tm->state == tm->halt_state) return 0;

    int rule_idx = tm_find_rule(tm);
    if (rule_idx < 0) return 0;  // No matching rule, halt

    TMRule *r = &tm->rules[rule_idx];
    tm->tape[tm->head] = r->write_sym;

    if (r->direction == DIR_LEFT) {
        tm->head = tm->head - 1;
    } else if (r->direction == DIR_RIGHT) {
        tm->head = tm->head + 1;
    }

    tm->state = r->next_state;
    tm->steps = tm->steps + 1;
    return 1;
}

void tm_run(TuringMachine *tm, int max_steps) {
    int count = 0;
    while (count < max_steps) {
        if (tm_step(tm) == 0) break;
        count = count + 1;
    }
}

int tm_count_ones(TuringMachine *tm) {
    int count = 0;
    int i = 0;
    while (i < tm->tape_len) {
        if (tm->tape[i] == 1) count = count + 1;
        i = i + 1;
    }
    return count;
}

void tm_print_tape(TuringMachine *tm, int from, int to) {
    int i = from;
    while (i <= to) {
        printf("%d", tm->tape[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    TuringMachine tm;

    // Test 1: Simple unary increment
    // Input: ...0 1 1 1 0... (3 in unary)
    // Add one 1 to the right end => ...0 1 1 1 1 0... (4 in unary)
    // State 0: read 0 -> write 0, right, state 1 (skip leading 0s, start scanning)
    // Actually let's start head on leftmost 1
    // State 0: read 1 -> write 1, right, state 0 (move right over 1s)
    // State 0: read 0 -> write 1, stay, halt (add a 1 at the end)
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 1; tm.tape[66] = 1; // 111
    tm.head = 64;
    tm_add_rule(&tm, 0, 1, 1, DIR_RIGHT, 0);
    tm_add_rule(&tm, 0, 0, 1, DIR_STAY, 99);
    tm_run(&tm, 100);
    int ones = tm_count_ones(&tm);
    printf("t1: ones=%d steps=%d\n", ones, tm.steps);
    // EXPECT: t1: ones=4 steps=4

    // Test 2: Binary inversion on 3 cells
    // Flip 0->1 and 1->0 for 3 cells, then halt
    // State 0: read 0 -> write 1, right, state 0
    // State 0: read 1 -> write 0, right, state 0
    // We need a counter... use a sentinel value 2 at end
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 0; tm.tape[66] = 1; tm.tape[67] = 2; // 101 with sentinel
    tm.head = 64;
    tm_add_rule(&tm, 0, 0, 1, DIR_RIGHT, 0);
    tm_add_rule(&tm, 0, 1, 0, DIR_RIGHT, 0);
    tm_add_rule(&tm, 0, 2, 2, DIR_STAY, 99); // halt at sentinel
    tm_run(&tm, 100);
    printf("t2: %d%d%d\n", tm.tape[64], tm.tape[65], tm.tape[66]);
    // EXPECT: t2: 010

    // Test 3: Busy Beaver 2-state (simplified)
    // 2-state BB: writes 4 ones and halts in 6 steps
    // State A (0): read 0 -> write 1, right, B
    // State A (0): read 1 -> write 1, left, B
    // State B (1): read 0 -> write 1, left, A
    // State B (1): read 1 -> write 1, right, HALT
    tm_init(&tm);
    tm.halt_state = 99;
    tm_add_rule(&tm, 0, 0, 1, DIR_RIGHT, 1); // A,0 -> 1,R,B
    tm_add_rule(&tm, 0, 1, 1, DIR_LEFT, 1);  // A,1 -> 1,L,B
    tm_add_rule(&tm, 1, 0, 1, DIR_LEFT, 0);  // B,0 -> 1,L,A
    tm_add_rule(&tm, 1, 1, 1, DIR_RIGHT, 99); // B,1 -> 1,R,HALT
    tm_run(&tm, 100);
    ones = tm_count_ones(&tm);
    printf("t3: ones=%d steps=%d\n", ones, tm.steps);
    // EXPECT: t3: ones=4 steps=6

    // Test 4: Move a block of 1s one position right
    // Input: 0 1 1 0 0 -> 0 0 1 1 0
    // Strategy: find leftmost 1, erase it, go right past all 1s, write 1
    // State 0: read 0 -> 0, right, 0 (skip leading 0)
    // State 0: read 1 -> 0, right, 1 (erase leftmost 1, go state 1)
    // State 1: read 1 -> 1, right, 1 (skip remaining 1s)
    // State 1: read 0 -> 1, stay, 99 (write 1 at end, halt)
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 1; // 1 1 at positions 64,65
    tm.head = 63; // start before the block
    tm_add_rule(&tm, 0, 0, 0, DIR_RIGHT, 0);
    tm_add_rule(&tm, 0, 1, 0, DIR_RIGHT, 1);
    tm_add_rule(&tm, 1, 1, 1, DIR_RIGHT, 1);
    tm_add_rule(&tm, 1, 0, 1, DIR_STAY, 99);
    tm_run(&tm, 100);
    printf("t4: %d%d%d%d\n", tm.tape[64], tm.tape[65], tm.tape[66], tm.tape[67]);
    // EXPECT: t4: 0110

    // Test 5: Count from 0 to 3 in binary (unary input of 3)
    // Input: 1 1 1 (3 in unary at positions 64,65,66)
    // Just count how many 1s: scan right, count
    // We'll use the TM to just read and verify state
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 1; tm.tape[66] = 1;
    tm.head = 64;
    // Just scan right counting 1s (by tracking state transitions)
    tm_add_rule(&tm, 0, 1, 1, DIR_RIGHT, 1); // saw first 1
    tm_add_rule(&tm, 1, 1, 1, DIR_RIGHT, 2); // saw second 1
    tm_add_rule(&tm, 2, 1, 1, DIR_RIGHT, 3); // saw third 1
    tm_add_rule(&tm, 3, 0, 0, DIR_STAY, 99); // done
    tm_run(&tm, 100);
    printf("t5: final_state=%d steps=%d\n", tm.state, tm.steps);
    // EXPECT: t5: final_state=99 steps=4

    // Test 6: Unary addition: 11 0 111 => 11111 (2 + 3 = 5)
    // Input: 1 1 0 1 1 1 at positions 64-69
    // Replace the 0 separator with 1, erase the last 1
    // State 0: scan right over 1s
    // State 0, read 1: stay 1, right, state 0
    // State 0, read 0: write 1, right, state 1 (found separator)
    // State 1: scan right over 1s to find end
    // State 1, read 1: stay 1, right, state 1
    // State 1, read 0: left, state 2 (found end, go back)
    // State 2: erase last 1
    // State 2, read 1: write 0, stay, halt
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 1; tm.tape[66] = 0;
    tm.tape[67] = 1; tm.tape[68] = 1; tm.tape[69] = 1;
    tm.head = 64;
    tm_add_rule(&tm, 0, 1, 1, DIR_RIGHT, 0);
    tm_add_rule(&tm, 0, 0, 1, DIR_RIGHT, 1);
    tm_add_rule(&tm, 1, 1, 1, DIR_RIGHT, 1);
    tm_add_rule(&tm, 1, 0, 0, DIR_LEFT, 2);
    tm_add_rule(&tm, 2, 1, 0, DIR_STAY, 99);
    tm_run(&tm, 100);
    ones = tm_count_ones(&tm);
    printf("t6: ones=%d\n", ones);
    // EXPECT: t6: ones=5

    // Test 7: Binary increment: 1 0 1 (5 in binary) => 1 1 0 (6 in binary)
    // Start at rightmost bit (LSB), add 1. If 1, set to 0, carry left.
    // State 0: at current bit. read 1 -> write 0, left, state 0 (carry)
    // State 0: read 0 -> write 1, stay, halt (no more carry)
    tm_init(&tm);
    tm.halt_state = 99;
    tm.tape[64] = 1; tm.tape[65] = 0; tm.tape[66] = 1; // 101 binary = 5
    tm.head = 66; // start at LSB (rightmost)
    tm_add_rule(&tm, 0, 1, 0, DIR_LEFT, 0);  // carry
    tm_add_rule(&tm, 0, 0, 1, DIR_STAY, 99); // done
    tm_run(&tm, 100);
    printf("t7: %d%d%d\n", tm.tape[64], tm.tape[65], tm.tape[66]);
    // EXPECT: t7: 110

    printf("turing machine done\n");
    // EXPECT: turing machine done

    return 0;
}
