// stress_051_cfg_stress.c - Control flow graph stress test
// Exercises: 20+ basic blocks, diamond patterns, unreachable code,
// multiple paths to same block, loop with multiple back edges,
// break from switch inside loop, nested diamonds.
// EXPECT: diamond1=10
// EXPECT: diamond2=20
// EXPECT: diamond3=29
// EXPECT: multi_path=77
// EXPECT: unreachable_test=42
// EXPECT: switch_in_loop=150
// EXPECT: nested_diamond=999
// EXPECT: back_edge=245
// EXPECT: cfg_maze=3628800
// EXPECT: mega_cfg=32
// EXPECT: all_cfg=3630404

int printf(const char *fmt, ...);

// Diamond pattern: if/else that rejoins
int diamond1(int x) {
    int r = 0;
    if (x > 5) {
        r = x;
    } else {
        r = 10 - x;
    }
    // rejoin point
    return r;
}

// Double diamond: two diamonds in sequence
int diamond2(int x) {
    int r;
    // first diamond
    if (x > 0) {
        r = x * 2;
    } else {
        r = -x * 2;
    }
    // second diamond
    if (r > 10) {
        r = r - 10;
    } else {
        r = r + 10;
    }
    return r;
}

// Triple diamond with shared merge
int diamond3(int x, int y, int z) {
    int r = 0;
    if (x > 0) { r += 10; } else { r += 1; }
    if (y > 0) { r += 20; } else { r += 2; }
    if (z > 0) { r -= 1; } else { r -= 2; }
    return r;
}

// Multiple paths converging to same block
int multi_path(int selector) {
    int result = 0;
    if (selector == 1) {
        result = 10;
    } else if (selector == 2) {
        result = 20;
    } else if (selector == 3) {
        result = 30;
    } else if (selector == 4) {
        result = 40;
    } else if (selector == 5) {
        result = 50;
    } else if (selector == 6) {
        result = 60;
    } else if (selector == 7) {
        result = 70;
    } else {
        result = 77;
    }
    // all 8 paths merge here
    return result;
}

// Unreachable code after return
int unreachable_test(int x) {
    if (x > 0) {
        return 42;
        x = 999;  // unreachable
        x = x + 1; // unreachable
    }
    return -1;
}

// Switch inside loop with break
int switch_in_loop(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        int mode = i % 5;
        switch (mode) {
            case 0:
                sum += 1;
                break;
            case 1:
                sum += 2;
                break;
            case 2:
                sum += 3;
                break;
            case 3:
                sum += 4;
                break;
            case 4:
                sum += 5;
                break;
        }
    }
    return sum;
}

// Nested diamonds - 4 levels
int nested_diamond(int a, int b, int c, int d) {
    int r;
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (d > 0) {
                    r = 999;
                } else {
                    r = 888;
                }
            } else {
                if (d > 0) {
                    r = 777;
                } else {
                    r = 666;
                }
            }
        } else {
            if (c > 0) {
                r = 555;
            } else {
                r = 444;
            }
        }
    } else {
        if (b > 0) {
            r = 333;
        } else {
            r = 222;
        }
    }
    return r;
}

// Loop with multiple back edges (continue from different points)
int back_edge(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        i++;
        if (i % 7 == 0) {
            sum += 100;
            continue; // back edge 1
        }
        if (i % 5 == 0) {
            sum += 10;
            continue; // back edge 2
        }
        if (i % 3 == 0) {
            sum += 1;
            continue; // back edge 3
        }
        // default back edge
        sum += 0;
    }
    return sum;
}

// CFG maze: factorial via goto-like control flow (using loops + breaks)
int cfg_maze(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        int j;
        int partial = 0;
        for (j = 0; j < i; j++) {
            partial += result;
        }
        result = partial;
        if (result < 0) break; // safety, never taken
    }
    return result;
}

// Mega CFG: 20+ basic blocks via chained if-else-if
int mega_cfg(int x) {
    int r = 0;
    if (x >= 20) { r = 1; }
    else if (x >= 19) { r = 2; }
    else if (x >= 18) { r = 3; }
    else if (x >= 17) { r = 4; }
    else if (x >= 16) { r = 5; }
    else if (x >= 15) { r = 6; }
    else if (x >= 14) { r = 7; }
    else if (x >= 13) { r = 8; }
    else if (x >= 12) { r = 9; }
    else if (x >= 11) { r = 10; }
    else if (x >= 10) { r = 11; }
    else if (x >= 9) { r = 12; }
    else if (x >= 8) { r = 13; }
    else if (x >= 7) { r = 14; }
    else if (x >= 6) { r = 15; }
    else if (x >= 5) { r = 16; }
    else if (x >= 4) { r = 17; }
    else if (x >= 3) { r = 18; }
    else if (x >= 2) { r = 19; }
    else if (x >= 1) { r = 20; }
    else { r = 0; }

    // Also a switch generating many BBs
    int s = 0;
    switch (r) {
        case 0: s = 0; break;
        case 1: s = 1; break;
        case 2: s = 4; break;
        case 3: s = 9; break;
        case 4: s = 16; break;
        case 5: s = 25; break;
        case 10: s = 30; break;
        case 11: s = 31; break;
        case 15: s = 35; break;
        default: s = r; break;
    }
    return r + s;
}

int main(void) {
    int d1 = diamond1(10);
    printf("diamond1=%d\n", d1);

    int d2 = diamond2(5);
    printf("diamond2=%d\n", d2);

    int d3 = diamond3(1, 1, 1);
    printf("diamond3=%d\n", d3);

    int mp = multi_path(99);
    printf("multi_path=%d\n", mp);

    int ut = unreachable_test(1);
    printf("unreachable_test=%d\n", ut);

    int sl = switch_in_loop(50);
    printf("switch_in_loop=%d\n", sl);

    int nd = nested_diamond(1, 1, 1, 1);
    printf("nested_diamond=%d\n", nd);

    int be = back_edge(20);
    printf("back_edge=%d\n", be);

    int cm = cfg_maze(10);
    printf("cfg_maze=%d\n", cm);

    int mc = mega_cfg(5);
    printf("mega_cfg=%d\n", mc);

    int total = d1 + d2 + d3 + mp + ut + sl + nd + be + cm + mc;
    printf("all_cfg=%d\n", total);

    return 0;
}
