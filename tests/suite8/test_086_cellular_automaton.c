int printf(const char *fmt, ...);

struct CellularAutomaton {
    int cells[64];
    int size;
    int rule;
    int generation;
    int total_alive_history[20];
};

void init_ca(struct CellularAutomaton *ca, int size, int rule) {
    int i;
    ca->size = size;
    ca->rule = rule;
    ca->generation = 0;
    for (i = 0; i < 64; i = i + 1) {
        ca->cells[i] = 0;
    }
    for (i = 0; i < 20; i = i + 1) {
        ca->total_alive_history[i] = 0;
    }
}

int get_bit(int rule, int index) {
    int shift = index;
    int val = rule;
    while (shift > 0) {
        val = val / 2;
        shift = shift - 1;
    }
    return val % 2;
}

int count_alive_ca(struct CellularAutomaton *ca) {
    int total = 0;
    int i;
    for (i = 0; i < ca->size; i = i + 1) {
        total = total + ca->cells[i];
    }
    return total;
}

void step_ca(struct CellularAutomaton *ca) {
    int next[64];
    int i;
    for (i = 0; i < ca->size; i = i + 1) {
        int left;
        int center;
        int right;
        if (i == 0) left = 0;
        else left = ca->cells[i - 1];
        center = ca->cells[i];
        if (i == ca->size - 1) right = 0;
        else right = ca->cells[i + 1];

        int pattern = left * 4 + center * 2 + right;
        next[i] = get_bit(ca->rule, pattern);
    }
    for (i = 0; i < ca->size; i = i + 1) {
        ca->cells[i] = next[i];
    }
    if (ca->generation < 20) {
        ca->total_alive_history[ca->generation] = count_alive_ca(ca);
    }
    ca->generation = ca->generation + 1;
}

void print_ca_state(struct CellularAutomaton *ca) {
    int i;
    int alive = count_alive_ca(ca);
    printf("Gen %d: alive=%d\n", ca->generation, alive);
}

int find_pattern_period(struct CellularAutomaton *ca) {
    int saved[64];
    int i;
    for (i = 0; i < ca->size; i = i + 1) {
        saved[i] = ca->cells[i];
    }

    int step;
    for (step = 1; step <= 10; step = step + 1) {
        step_ca(ca);
        int match = 1;
        for (i = 0; i < ca->size; i = i + 1) {
            if (ca->cells[i] != saved[i]) {
                match = 0;
                break;
            }
        }
        if (match) return step;
    }
    return -1;
}

void run_rule(int rule, int size, int center_start, int generations) {
    struct CellularAutomaton ca;
    init_ca(&ca, size, rule);
    if (center_start) {
        ca.cells[size / 2] = 1;
    }
    printf("Rule %d, size=%d:\n", rule, size);
    print_ca_state(&ca);
    int g;
    for (g = 0; g < generations; g = g + 1) {
        step_ca(&ca);
        print_ca_state(&ca);
    }
}

int main() {
    printf("=== Rule 30 ===\n");
    // EXPECT: === Rule 30 ===
    run_rule(30, 16, 1, 5);
    // EXPECT: Rule 30, size=16:
    // EXPECT: Gen 0: alive=1
    // EXPECT: Gen 1: alive=3
    // EXPECT: Gen 2: alive=3
    // EXPECT: Gen 3: alive=6
    // EXPECT: Gen 4: alive=4
    // EXPECT: Gen 5: alive=9

    printf("=== Rule 90 ===\n");
    // EXPECT: === Rule 90 ===
    run_rule(90, 16, 1, 5);
    // EXPECT: Rule 90, size=16:
    // EXPECT: Gen 0: alive=1
    // EXPECT: Gen 1: alive=2
    // EXPECT: Gen 2: alive=2
    // EXPECT: Gen 3: alive=4
    // EXPECT: Gen 4: alive=2
    // EXPECT: Gen 5: alive=4

    printf("=== Rule 110 ===\n");
    // EXPECT: === Rule 110 ===
    run_rule(110, 16, 1, 5);
    // EXPECT: Rule 110, size=16:
    // EXPECT: Gen 0: alive=1
    // EXPECT: Gen 1: alive=2
    // EXPECT: Gen 2: alive=3
    // EXPECT: Gen 3: alive=3
    // EXPECT: Gen 4: alive=5
    // EXPECT: Gen 5: alive=3

    printf("=== get_bit tests ===\n");
    // EXPECT: === get_bit tests ===
    printf("Rule30 bit0=%d bit1=%d bit2=%d bit3=%d\n",
           get_bit(30, 0), get_bit(30, 1), get_bit(30, 2), get_bit(30, 3));
    // EXPECT: Rule30 bit0=0 bit1=1 bit2=1 bit3=1
    printf("Rule30 bit4=%d bit5=%d bit6=%d bit7=%d\n",
           get_bit(30, 4), get_bit(30, 5), get_bit(30, 6), get_bit(30, 7));
    // EXPECT: Rule30 bit4=1 bit5=0 bit6=0 bit7=0

    printf("=== Period detection ===\n");
    // EXPECT: === Period detection ===
    struct CellularAutomaton ca2;
    init_ca(&ca2, 8, 90);
    ca2.cells[3] = 1;
    ca2.cells[4] = 1;
    int period = find_pattern_period(&ca2);
    printf("Rule 90 pair period: %d\n", period);
    // EXPECT: Rule 90 pair period: 7

    return 0;
}
