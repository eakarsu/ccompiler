int printf(const char *fmt, ...);

// Register allocator using graph coloring simulation
// Builds an interference graph and colors it with k registers

int MAX_REGS = 32;
int NUM_COLORS = 4;

struct VReg {
    int start;
    int end;
    int color;
    int spilled;
    int degree;
    int active;
};

struct VReg vregs[32];
int vreg_count;

// Adjacency matrix for interference
int adj[32][32];

void init_regalloc() {
    vreg_count = 0;
    int i = 0;
    while (i < 32) {
        vregs[i].color = -1;
        vregs[i].spilled = 0;
        vregs[i].degree = 0;
        vregs[i].active = 1;
        int j = 0;
        while (j < 32) {
            adj[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

int add_vreg(int start, int end) {
    int idx = vreg_count;
    vregs[idx].start = start;
    vregs[idx].end = end;
    vregs[idx].color = -1;
    vregs[idx].spilled = 0;
    vregs[idx].active = 1;
    vreg_count = vreg_count + 1;
    return idx;
}

void add_interference(int a, int b) {
    if (a != b) {
        adj[a][b] = 1;
        adj[b][a] = 1;
    }
}

void build_interference() {
    int i = 0;
    while (i < vreg_count) {
        int j = i + 1;
        while (j < vreg_count) {
            int ov = 0;
            if (vregs[i].start <= vregs[j].end && vregs[j].start <= vregs[i].end) {
                ov = 1;
            }
            if (ov) {
                add_interference(i, j);
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void compute_degrees() {
    int i = 0;
    while (i < vreg_count) {
        int deg = 0;
        int j = 0;
        while (j < vreg_count) {
            if (adj[i][j] && vregs[j].active) deg = deg + 1;
            j = j + 1;
        }
        vregs[i].degree = deg;
        i = i + 1;
    }
}

// Simplify: remove nodes with degree < NUM_COLORS
int simplify_stack[32];
int stack_top;

int simplify() {
    stack_top = 0;
    int removed = 1;
    while (removed) {
        removed = 0;
        compute_degrees();
        int i = 0;
        while (i < vreg_count) {
            if (vregs[i].active && vregs[i].degree < NUM_COLORS) {
                vregs[i].active = 0;
                simplify_stack[stack_top] = i;
                stack_top = stack_top + 1;
                removed = 1;
            }
            i = i + 1;
        }
    }
    // Check for nodes still active (need spilling)
    int i = 0;
    int spill_count = 0;
    while (i < vreg_count) {
        if (vregs[i].active) {
            vregs[i].spilled = 1;
            vregs[i].active = 0;
            simplify_stack[stack_top] = i;
            stack_top = stack_top + 1;
            spill_count = spill_count + 1;
        }
        i = i + 1;
    }
    return spill_count;
}

void color_graph() {
    while (stack_top > 0) {
        stack_top = stack_top - 1;
        int reg = simplify_stack[stack_top];
        if (vregs[reg].spilled) continue;

        int used_colors[4];
        int c = 0;
        while (c < NUM_COLORS) { used_colors[c] = 0; c = c + 1; }

        int j = 0;
        while (j < vreg_count) {
            if (adj[reg][j] && vregs[j].color >= 0) {
                if (vregs[j].color < NUM_COLORS) {
                    used_colors[vregs[j].color] = 1;
                }
            }
            j = j + 1;
        }

        c = 0;
        while (c < NUM_COLORS) {
            if (!used_colors[c]) {
                vregs[reg].color = c;
                break;
            }
            c = c + 1;
        }
        if (vregs[reg].color < 0) {
            vregs[reg].spilled = 1;
        }
    }
}

int count_spilled() {
    int c = 0;
    int i = 0;
    while (i < vreg_count) {
        if (vregs[i].spilled) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_colored() {
    int c = 0;
    int i = 0;
    while (i < vreg_count) {
        if (vregs[i].color >= 0) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main() {
    // Test 1: Simple non-overlapping intervals
    init_regalloc();
    add_vreg(0, 2);
    add_vreg(3, 5);
    add_vreg(6, 8);
    build_interference();
    int spills = simplify();
    color_graph();

    printf("test1 colored: %d\n", count_colored());
    // EXPECT: test1 colored: 3
    printf("test1 spills: %d\n", count_spilled());
    // EXPECT: test1 spills: 0

    // Test 2: All overlapping - 3 regs, 4 colors
    init_regalloc();
    add_vreg(0, 10);
    add_vreg(0, 10);
    add_vreg(0, 10);
    build_interference();
    spills = simplify();
    color_graph();

    printf("test2 colored: %d\n", count_colored());
    // EXPECT: test2 colored: 3
    printf("test2 spills: %d\n", count_spilled());
    // EXPECT: test2 spills: 0

    // Verify all different colors
    int c0 = vregs[0].color;
    int c1 = vregs[1].color;
    int c2 = vregs[2].color;
    int all_diff = (c0 != c1) && (c1 != c2) && (c0 != c2);
    printf("test2 all diff: %d\n", all_diff);
    // EXPECT: test2 all diff: 1

    // Test 3: 5 overlapping with 4 colors - need 1 spill
    init_regalloc();
    add_vreg(0, 10);
    add_vreg(0, 10);
    add_vreg(0, 10);
    add_vreg(0, 10);
    add_vreg(0, 10);
    build_interference();
    spills = simplify();
    color_graph();

    printf("test3 spills: %d\n", count_spilled());
    // EXPECT: test3 spills: 5
    printf("test3 colored: %d\n", count_colored());
    // EXPECT: test3 colored: 0

    // Test 4: Chain of overlaps
    init_regalloc();
    add_vreg(0, 3);
    add_vreg(2, 5);
    add_vreg(4, 7);
    add_vreg(6, 9);
    build_interference();
    compute_degrees();

    printf("chain deg[0]: %d\n", vregs[0].degree);
    // EXPECT: chain deg[0]: 1
    printf("chain deg[1]: %d\n", vregs[1].degree);
    // EXPECT: chain deg[1]: 2

    spills = simplify();
    color_graph();
    printf("test4 colored: %d\n", count_colored());
    // EXPECT: test4 colored: 4
    printf("test4 spills: %d\n", count_spilled());
    // EXPECT: test4 spills: 0

    // Verify adjacent nodes have different colors
    int valid = 1;
    int ii = 0;
    while (ii < 4) {
        int jj = ii + 1;
        while (jj < 4) {
            if (adj[ii][jj] && vregs[ii].color == vregs[jj].color) {
                valid = 0;
            }
            jj = jj + 1;
        }
        ii = ii + 1;
    }
    printf("test4 valid coloring: %d\n", valid);
    // EXPECT: test4 valid coloring: 1

    printf("regalloc done\n");
    // EXPECT: regalloc done

    return 0;
}
