int printf(const char *fmt, ...);

struct BFMachine {
    char tape[256];
    int ptr;
    char output[128];
    int out_pos;
    int steps;
};

void bf_init(struct BFMachine *m) {
    int i;
    for (i = 0; i < 256; i++) {
        m->tape[i] = 0;
    }
    for (i = 0; i < 128; i++) {
        m->output[i] = 0;
    }
    m->ptr = 128;
    m->out_pos = 0;
    m->steps = 0;
}

int find_matching_bracket(char *code, int pos, int dir) {
    int depth = 1;
    pos = pos + dir;
    while (depth > 0) {
        if (code[pos] == '[') {
            if (dir == 1) depth = depth + 1;
            else depth = depth - 1;
        } else if (code[pos] == ']') {
            if (dir == 1) depth = depth - 1;
            else depth = depth + 1;
        }
        if (depth > 0) pos = pos + dir;
    }
    return pos;
}

int bf_run(struct BFMachine *m, char *code) {
    int ip = 0;
    int codelen = 0;
    while (code[codelen] != 0) codelen = codelen + 1;

    while (ip < codelen && m->steps < 10000) {
        char c = code[ip];
        m->steps = m->steps + 1;

        if (c == '>') {
            m->ptr = m->ptr + 1;
            if (m->ptr >= 256) m->ptr = 0;
        } else if (c == '<') {
            m->ptr = m->ptr - 1;
            if (m->ptr < 0) m->ptr = 255;
        } else if (c == '+') {
            m->tape[m->ptr] = m->tape[m->ptr] + 1;
        } else if (c == '-') {
            m->tape[m->ptr] = m->tape[m->ptr] - 1;
        } else if (c == '.') {
            if (m->out_pos < 127) {
                m->output[m->out_pos] = m->tape[m->ptr];
                m->out_pos = m->out_pos + 1;
                m->output[m->out_pos] = 0;
            }
        } else if (c == '[') {
            if (m->tape[m->ptr] == 0) {
                ip = find_matching_bracket(code, ip, 1);
            }
        } else if (c == ']') {
            if (m->tape[m->ptr] != 0) {
                ip = find_matching_bracket(code, ip, -1);
            }
        }
        ip = ip + 1;
    }
    return m->steps;
}

void print_tape(struct BFMachine *m, int start, int count) {
    int i;
    printf("Tape[%d..%d]:", start, start + count - 1);
    for (i = 0; i < count; i++) {
        printf(" %d", (int)(m->tape[start + i]));
    }
    printf("\n");
}

int main() {
    struct BFMachine m;

    /* Test 1: Simple increment */
    bf_init(&m);
    bf_run(&m, "++++++");
    // EXPECT: Test 1: Simple increment
    printf("Test 1: Simple increment\n");
    // EXPECT: Cell value: 6
    printf("Cell value: %d\n", (int)(m.tape[m.ptr]));

    /* Test 2: Increment and move */
    bf_init(&m);
    bf_run(&m, "+++>+++++>+++++++");
    // EXPECT: Test 2: Increment and move
    printf("Test 2: Increment and move\n");
    // EXPECT: Tape[128..130]: 3 5 7
    print_tape(&m, 128, 3);

    /* Test 3: Decrement */
    bf_init(&m);
    bf_run(&m, "++++++++++---");
    // EXPECT: Test 3: Decrement
    printf("Test 3: Decrement\n");
    // EXPECT: Cell value: 7
    printf("Cell value: %d\n", (int)(m.tape[m.ptr]));

    /* Test 4: Simple loop (multiply 3*4=12) */
    /* Set cell0=3, then loop: dec cell0, go right, add 4, go left */
    bf_init(&m);
    bf_run(&m, "+++[>++++<-]");
    // EXPECT: Test 4: Loop multiply 3*4
    printf("Test 4: Loop multiply 3*4\n");
    // EXPECT: Cell 0: 0
    printf("Cell 0: %d\n", (int)(m.tape[128]));
    // EXPECT: Cell 1: 12
    printf("Cell 1: %d\n", (int)(m.tape[129]));

    /* Test 5: Copy cell (cell0=5, copy to cell1 via cell2) */
    bf_init(&m);
    bf_run(&m, "+++++[>+>+<<-]>>[-<<+>>]");
    // EXPECT: Test 5: Copy cell
    printf("Test 5: Copy cell\n");
    // EXPECT: Cell 0: 5
    printf("Cell 0: %d\n", (int)(m.tape[128]));
    // EXPECT: Cell 1: 5
    printf("Cell 1: %d\n", (int)(m.tape[129]));

    /* Test 6: Output ASCII 'A' (65) = 8*8+1 */
    bf_init(&m);
    bf_run(&m, "++++++++[>++++++++<-]>+.");
    // EXPECT: Test 6: Output char
    printf("Test 6: Output char\n");
    // EXPECT: Output: A
    printf("Output: %s\n", m.output);

    /* Test 7: Step counting */
    bf_init(&m);
    int steps = bf_run(&m, "+++++[>+++<-]");
    // EXPECT: Test 7: Steps
    printf("Test 7: Steps\n");
    // EXPECT: Steps taken: 41
    printf("Steps taken: %d\n", steps);

    /* Test 8: Nested loops */
    bf_init(&m);
    bf_run(&m, "++[>++[>+++<-]<-]");
    // EXPECT: Test 8: Nested loops
    printf("Test 8: Nested loops\n");
    // EXPECT: Cell 2: 12
    printf("Cell 2: %d\n", (int)(m.tape[130]));

    // EXPECT: All brainfuck tests done
    printf("All brainfuck tests done\n");
    return 0;
}
