int printf(const char *fmt, ...);

// Brainfuck interpreter (simplified, bounded tape)
// Commands: > < + - [ ] .
// We use '.' to output as integer for testing

typedef struct {
    char tape[256];
    int dp;
    char prog[256];
    int pc;
    int proglen;
    int output[64];
    int outpos;
} BFMachine;

void bf_init(BFMachine *m) {
    int i = 0;
    while (i < 256) {
        m->tape[i] = 0;
        m->prog[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 64) {
        m->output[i] = 0;
        i = i + 1;
    }
    m->dp = 0;
    m->pc = 0;
    m->proglen = 0;
    m->outpos = 0;
}

void bf_load(BFMachine *m, char *code) {
    int i = 0;
    while (code[i] != 0) {
        m->prog[i] = code[i];
        i = i + 1;
    }
    m->proglen = i;
}

int bf_find_matching_close(BFMachine *m, int pos) {
    int depth = 1;
    int p = pos + 1;
    while (p < m->proglen && depth > 0) {
        if (m->prog[p] == '[') depth = depth + 1;
        if (m->prog[p] == ']') depth = depth - 1;
        if (depth > 0) p = p + 1;
    }
    return p;
}

int bf_find_matching_open(BFMachine *m, int pos) {
    int depth = 1;
    int p = pos - 1;
    while (p >= 0 && depth > 0) {
        if (m->prog[p] == ']') depth = depth + 1;
        if (m->prog[p] == '[') depth = depth - 1;
        if (depth > 0) p = p - 1;
    }
    return p;
}

void bf_run(BFMachine *m) {
    int limit = 10000;
    int steps = 0;
    while (m->pc < m->proglen && steps < limit) {
        char c = m->prog[m->pc];
        if (c == '>') {
            m->dp = m->dp + 1;
        } else if (c == '<') {
            m->dp = m->dp - 1;
        } else if (c == '+') {
            m->tape[m->dp] = m->tape[m->dp] + 1;
        } else if (c == '-') {
            m->tape[m->dp] = m->tape[m->dp] - 1;
        } else if (c == '.') {
            m->output[m->outpos] = m->tape[m->dp];
            m->outpos = m->outpos + 1;
        } else if (c == '[') {
            if (m->tape[m->dp] == 0) {
                m->pc = bf_find_matching_close(m, m->pc);
            }
        } else if (c == ']') {
            if (m->tape[m->dp] != 0) {
                m->pc = bf_find_matching_open(m, m->pc);
            }
        }
        m->pc = m->pc + 1;
        steps = steps + 1;
    }
}

void print_outputs(BFMachine *m) {
    int i = 0;
    while (i < m->outpos) {
        printf("%d ", m->output[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    BFMachine m;

    // Test 1: Simple increment and output: "+++++." => tape[0]=5
    bf_init(&m);
    bf_load(&m, "+++++.");
    bf_run(&m);
    printf("t1: %d\n", m.output[0]);
    // EXPECT: t1: 5

    // Test 2: Increment, move, increment: "+++>++." => tape[1]=2
    bf_init(&m);
    bf_load(&m, "+++>++.");
    bf_run(&m);
    printf("t2: %d\n", m.output[0]);
    // EXPECT: t2: 2

    // Test 3: Go back and read first cell: "+++>++<." => tape[0]=3
    bf_init(&m);
    bf_load(&m, "+++>++<.");
    bf_run(&m);
    printf("t3: %d\n", m.output[0]);
    // EXPECT: t3: 3

    // Test 4: Decrement: "+++++--." => tape[0]=3
    bf_init(&m);
    bf_load(&m, "+++++--.");
    bf_run(&m);
    printf("t4: %d\n", m.output[0]);
    // EXPECT: t4: 3

    // Test 5: Simple loop: set cell[0]=3, loop: move to cell[1], inc, back, dec
    // This copies 3 into cell[1]: "+++[>+<-]>."
    bf_init(&m);
    bf_load(&m, "+++[>+<-]>.");
    bf_run(&m);
    printf("t5: %d\n", m.output[0]);
    // EXPECT: t5: 3

    // Test 6: Multiply 3 * 4 = 12
    // cell[0]=3, loop: cell[1]+=4, cell[0]--
    // "+++ [> ++++  < -] >."
    bf_init(&m);
    bf_load(&m, "+++[>++++<-]>.");
    bf_run(&m);
    printf("t6: %d\n", m.output[0]);
    // EXPECT: t6: 12

    // Test 7: Nested loops - multiply then copy
    // cell[0]=2, cell[1]=3*cell[0]=6, then copy cell[1] to cell[2]
    // "++[>+++<-]>[>+<-]>>." => but we output cell[2]
    // Step: cell[0]=2, inner: cell[1]+=3, cell[0]--, so cell[1]=6
    // Then cell[1]=6, inner: cell[2]+=1, cell[1]--, cell[2]=6
    // "> [>+<-]" we are at cell[1] at start of second part
    // Actually: "++[>+++<-]" => cell0=0, cell1=6
    // Then ">" => at cell1=6, "[>+<-]" => cell1=0, cell2=6
    // Then ">." => at cell2=6, output
    bf_init(&m);
    bf_load(&m, "++[>+++<-]>[>+<-]>>.");
    bf_run(&m);
    // After "++[>+++<-]": cell0=0, cell1=6
    // ">": dp=1 (cell1=6)
    // "[>+<-]": cell1 to cell2: cell1=0, cell2=6
    // ">>": dp=3 (cell3=0)
    // ".": output cell3=0
    // Hmm, that's wrong. Let me retrace.
    // After "++[>+++<-]": dp=0, cell0=0, cell1=6
    // ">": dp=1
    // "[>+<-]": dp=1,cell1=6. loop: >: dp=2, +: cell2=1, <: dp=1, -: cell1=5
    //   repeat until cell1=0 => cell2=6
    //   After loop: dp=1, cell1=0
    // ">": dp=2
    // ">": dp=3
    // ".": output cell3=0
    // So output is 0. Fix: use ">." instead of ">>."
    printf("t7: %d\n", m.output[0]);
    // EXPECT: t7: 0

    // Test 7b: redo properly
    bf_init(&m);
    bf_load(&m, "++[>+++<-]>[>+<-]>.");
    bf_run(&m);
    // After "++[>+++<-]": cell0=0, cell1=6, dp=0
    // ">": dp=1, cell1=6
    // "[>+<-]": copies cell1 to cell2. cell1=0, cell2=6, dp=1
    // ">": dp=2, cell2=6
    // ".": output 6
    printf("t7b: %d\n", m.output[0]);
    // EXPECT: t7b: 6

    // Test 8: Multiple outputs
    bf_init(&m);
    bf_load(&m, "++.>+++.>+.");
    bf_run(&m);
    printf("t8: %d %d %d\n", m.output[0], m.output[1], m.output[2]);
    // EXPECT: t8: 2 3 1

    // Test 9: Skip loop when cell is zero
    bf_init(&m);
    bf_load(&m, "[+++]++.");
    bf_run(&m);
    // cell[0] starts at 0, so [+++] is skipped, then ++ makes it 2
    printf("t9: %d\n", m.output[0]);
    // EXPECT: t9: 2

    // Test 10: Fibonacci-like: compute cell[0]=1, cell[1]=1
    // then sum into cell[2] = cell[0]+cell[1]
    bf_init(&m);
    bf_load(&m, "+>+<[>>[>+<-]<[>+<-]<-]>>.");
    bf_run(&m);
    // cell0=1, cell1=1, dp=0
    // Loop: cell0=1 != 0
    //   >>: dp=2
    //   [>+<-]: cell2=0, skipped
    //   <: dp=1
    //   [>+<-]: cell1=1, loop: >dp=2,+cell2=1,<dp=1,-cell1=0 => cell1=0,cell2=1
    //   <: dp=0
    //   -: cell0=0
    // Loop ends, cell0=0
    // >>: dp=2, cell2=1
    // .: output 1
    printf("t10: %d\n", m.output[0]);
    // EXPECT: t10: 1

    printf("brainfuck tests done\n");
    // EXPECT: brainfuck tests done

    return 0;
}
