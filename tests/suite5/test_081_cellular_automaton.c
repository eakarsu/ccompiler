int printf(const char *fmt, ...);

int get_rule_bit(int rule, int pattern) {
    int i;
    int val = rule;
    for (i = 0; i < pattern; i = i + 1) {
        val = val / 2;
    }
    return val % 2;
}

void evolve(int *cur, int *nxt, int size, int rule) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        int left;
        int center;
        int right;
        if (i == 0) {
            left = 0;
        } else {
            left = cur[i - 1];
        }
        center = cur[i];
        if (i == size - 1) {
            right = 0;
        } else {
            right = cur[i + 1];
        }
        int pattern = left * 4 + center * 2 + right;
        nxt[i] = get_rule_bit(rule, pattern);
    }
}

int count_live(int *cells, int size) {
    int count = 0;
    int i;
    for (i = 0; i < size; i = i + 1) {
        count = count + cells[i];
    }
    return count;
}

void copy_cells(int *dst, int *src, int size) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        dst[i] = src[i];
    }
}

int main(void) {
    int cells[21];
    int next[21];
    int size = 21;
    int i;
    int gen;

    for (i = 0; i < size; i = i + 1) {
        cells[i] = 0;
    }
    cells[10] = 1;

    /* Rule 30 test */
    // EXPECT: Rule30 gen0 live: 1
    printf("Rule30 gen0 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 30);
    copy_cells(cells, next, size);
    // EXPECT: Rule30 gen1 live: 3
    printf("Rule30 gen1 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 30);
    copy_cells(cells, next, size);
    // EXPECT: Rule30 gen2 live: 3
    printf("Rule30 gen2 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 30);
    copy_cells(cells, next, size);
    // EXPECT: Rule30 gen3 live: 6
    printf("Rule30 gen3 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 30);
    copy_cells(cells, next, size);
    // EXPECT: Rule30 gen4 live: 4
    printf("Rule30 gen4 live: %d\n", count_live(cells, size));

    /* Rule 110 test */
    for (i = 0; i < size; i = i + 1) {
        cells[i] = 0;
    }
    cells[10] = 1;

    // EXPECT: Rule110 gen0 live: 1
    printf("Rule110 gen0 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 110);
    copy_cells(cells, next, size);
    // EXPECT: Rule110 gen1 live: 2
    printf("Rule110 gen1 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 110);
    copy_cells(cells, next, size);
    // EXPECT: Rule110 gen2 live: 3
    printf("Rule110 gen2 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 110);
    copy_cells(cells, next, size);
    // EXPECT: Rule110 gen3 live: 3
    printf("Rule110 gen3 live: %d\n", count_live(cells, size));

    evolve(cells, next, size, 110);
    copy_cells(cells, next, size);
    // EXPECT: Rule110 gen4 live: 5
    printf("Rule110 gen4 live: %d\n", count_live(cells, size));

    /* Rule bit extraction tests */
    // EXPECT: RuleBit 30 pat0: 0
    printf("RuleBit 30 pat0: %d\n", get_rule_bit(30, 0));
    // EXPECT: RuleBit 30 pat1: 1
    printf("RuleBit 30 pat1: %d\n", get_rule_bit(30, 1));
    // EXPECT: RuleBit 30 pat2: 1
    printf("RuleBit 30 pat2: %d\n", get_rule_bit(30, 2));
    // EXPECT: RuleBit 30 pat3: 1
    printf("RuleBit 30 pat3: %d\n", get_rule_bit(30, 3));
    // EXPECT: RuleBit 30 pat4: 1
    printf("RuleBit 30 pat4: %d\n", get_rule_bit(30, 4));

    /* Multi-generation rule 30 accumulation */
    for (i = 0; i < size; i = i + 1) {
        cells[i] = 0;
    }
    cells[10] = 1;
    int total = 0;
    for (gen = 0; gen < 6; gen = gen + 1) {
        total = total + count_live(cells, size);
        evolve(cells, next, size, 30);
        copy_cells(cells, next, size);
    }
    // EXPECT: Rule30 6gen total: 26
    printf("Rule30 6gen total: %d\n", total);

    /* Center cell tracking rule 30 */
    for (i = 0; i < size; i = i + 1) {
        cells[i] = 0;
    }
    cells[10] = 1;
    int center_sum = 0;
    for (gen = 0; gen < 8; gen = gen + 1) {
        center_sum = center_sum + cells[10];
        evolve(cells, next, size, 30);
        copy_cells(cells, next, size);
    }
    // EXPECT: Rule30 center8: 5
    printf("Rule30 center8: %d\n", center_sum);

    return 0;
}
