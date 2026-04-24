int printf(const char *fmt, ...);
// EXPECT: === PHI Node Stress Test ===\nT1: 100 200 300 400 500 600\nT1b: 1 2 3 4 5 6\nT2: 10 200 300\nT2b: 1 20 30\nT3: 10 120 5\nT4: 20 22 24\nT4b: 0 0 0\nT5: 50 10 150 20\nT6: 42\nT6b: 99\nT7: 0 4\nT7b: 8 7 3\nT8: 100 2 300 4 500 6 700 8 900 10\nT8b: 1 20 3 40 5 60 7 80 9 100\nT9a: 15 25\nT9b: 15 20\nT9c: 15 30\nT9d: 20 30\nT10: 1 2 4 8 16 32 64 128 256 512\nT11: 55 3628800\nT12: 210 190 100\nT13: 1 0 1 0 1 0 1 0\n=== DONE ===

// T1: 6 variables updated in both if/else branches => 6 PHI nodes at merge
int test1(int cond) {
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
    if (cond) {
        a = 100; b = 200; c = 300; d = 400; e = 500; f = 600;
    } else {
        a = -1; b = -2; c = -3; d = -4; e = -5; f = -6;
    }
    printf("T1: %d %d %d %d %d %d\n", a, b, c, d, e, f);
    // Second diamond: else branch with no updates => PHI merges originals
    a = 1; b = 2; c = 3; d = 4; e = 5; f = 6;
    if (!cond) {
        a = 100; b = 200; c = 300; d = 400; e = 500; f = 600;
    } else {
        // no changes
    }
    printf("T1b: %d %d %d %d %d %d\n", a, b, c, d, e, f);
    return 0;
}

// T2: Nested if/else => PHI at inner merge and outer merge
int test2(int outer, int inner) {
    int x = 1, y = 2, z = 3;
    if (outer) {
        x = 10;
        if (inner) { y = 100; z = 100; }
        else       { y = 200; z = 300; }
    } else {
        x = -10; y = -20; z = -30;
    }
    printf("T2: %d %d %d\n", x, y, z);
    return 0;
}

int test2b(int outer, int inner) {
    int x = 1, y = 2, z = 3;
    if (outer) {
        x = 10;
        if (inner) { y = 100; z = 100; }
        else       { y = 200; z = 300; }
    } else {
        x = 1; y = 20; z = 30;
    }
    printf("T2b: %d %d %d\n", x, y, z);
    return 0;
}

// T3: Loop PHI nodes for sum, prod, count, i
int test3(void) {
    int sum = 0, prod = 1, count = 0, i = 0;
    while (i < 5) {
        sum = sum + i;
        prod = prod * (i + 1);
        count = count + 1;
        i = i + 1;
    }
    printf("T3: %d %d %d\n", sum, prod, count);
    return 0;
}

// T4: if/else-if chain => PHI merges 4 possible paths
int test4(int selector) {
    int r = 0, s = 0, t = 0;
    if (selector == 1)      { r = 10; s = 11; t = 12; }
    else if (selector == 2) { r = 20; s = 22; t = 24; }
    else if (selector == 3) { r = 30; s = 33; t = 36; }
    else                    { r = -1; s = -1; t = -1; }
    printf("T4: %d %d %d\n", r, s, t);
    return 0;
}

int test4b(int selector) {
    int r = 0, s = 0, t = 0;
    if (selector == 1)      { r = 10; s = 11; t = 12; }
    else if (selector == 2) { r = 20; s = 22; t = 24; }
    else if (selector == 3) { r = 30; s = 33; t = 36; }
    else                    { /* no changes */ }
    printf("T4b: %d %d %d\n", r, s, t);
    return 0;
}

// T5: Mix -- p,r updated in branches (need PHI), q,s untouched (no PHI)
int test5(int flag) {
    int p = 5, q = 10, r = 15, s = 20;
    if (flag) { p = 50; r = 150; }
    else      { p = -50; r = -150; }
    printf("T5: %d %d %d %d\n", p, q, r, s);
    return 0;
}

// T6: Variable updated in only one branch => PHI with original value
int test6(int cond) {
    int val = 42;
    if (cond) { val = 99; }
    printf("T6: %d\n", val);
    return 0;
}

int test6b(int cond) {
    int val = 42;
    if (cond) { val = 99; }
    printf("T6b: %d\n", val);
    return 0;
}

// T7: Variable used in condition AND modified in branch body
// Loop + inner branch both create PHI for a and count
int test7(int start) {
    int a = start, count = 0;
    while (a > 0) {
        if (a > 3) { a = a - 2; }
        else       { a = a - 1; }
        count = count + 1;
    }
    printf("T7: %d %d\n", a, count);
    return 0;
}

// T7b: Two variables converge => loop PHI for a, b, step
int test7b(void) {
    int a = 5, b = 10, step = 0;
    while (a < b) {
        a = a + 1;
        b = b - 1;
        step = step + 1;
    }
    printf("T7b: %d %d %d\n", a, b, step);
    return 0;
}

// T8: 10 local variables, two if-blocks selectively update subsets
// mode=1 => block1 fires (even-idx *= 100), block2 skipped
int test8(int mode) {
    int v0 = 1, v1 = 2, v2 = 3, v3 = 4, v4 = 5;
    int v5 = 6, v6 = 7, v7 = 8, v8 = 9, v9 = 10;
    if (mode & 1) {
        v0 = v0 * 100; v2 = v2 * 100; v4 = v4 * 100;
        v6 = v6 * 100; v8 = v8 * 100;
    }
    if (mode & 2) {
        v1 = v1 * 10; v3 = v3 * 10; v5 = v5 * 10;
        v7 = v7 * 10; v9 = v9 * 10;
    }
    printf("T8: %d %d %d %d %d %d %d %d %d %d\n",
           v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return 0;
}

// mode=2 => block1 skipped, block2 fires (odd-idx *= 10)
int test8b(int mode) {
    int v0 = 1, v1 = 2, v2 = 3, v3 = 4, v4 = 5;
    int v5 = 6, v6 = 7, v7 = 8, v8 = 9, v9 = 10;
    if (mode & 1) {
        v0 = v0 * 100; v2 = v2 * 100; v4 = v4 * 100;
        v6 = v6 * 100; v8 = v8 * 100;
    }
    if (mode & 2) {
        v1 = v1 * 10; v3 = v3 * 10; v5 = v5 * 10;
        v7 = v7 * 10; v9 = v9 * 10;
    }
    printf("T8b: %d %d %d %d %d %d %d %d %d %d\n",
           v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return 0;
}

// T9: Sequential diamond patterns => PHI at each merge
int test9a_fn(int a_flag, int b_flag) {
    int x = 10, y = 20;
    if (a_flag) { x = x + 5; y = y + 5; }
    else        { x = x - 5; y = y - 5; }
    printf("T9a: %d %d\n", x, y);
    if (b_flag) { y = y + 5; }
    else        { y = y - 5; }
    printf("T9b: %d %d\n", x, y);
    return 0;
}

int test9c_fn(int a_flag, int b_flag) {
    int x = 10, y = 20;
    if (a_flag) { x = x + 5; y = y + 5; }
    else        { x = x - 5; y = y - 5; }
    if (b_flag) { y = y + 5; }
    else        { y = y - 5; }
    printf("T9c: %d %d\n", x, y);
    return 0;
}

int test9d_fn(int a_flag, int b_flag) {
    int x = 10, y = 20;
    if (a_flag) { x = x + 10; y = y + 10; }
    else        { x = x - 10; y = y - 10; }
    printf("T9d: %d %d\n", x, y);
    return 0;
}

// T10: Chain of powers-of-two, then conditional gauntlet forcing PHI
int test10(void) {
    int v0 = 1, v1 = 1, v2 = 1, v3 = 1, v4 = 1;
    int v5 = 1, v6 = 1, v7 = 1, v8 = 1, v9 = 1;
    v1 = v0 * 2; v2 = v1 * 2; v3 = v2 * 2; v4 = v3 * 2;
    v5 = v4 * 2; v6 = v5 * 2; v7 = v6 * 2; v8 = v7 * 2;
    v9 = v8 * 2;
    // 5 conditional blocks: true path preserves, else zeros => PHI each
    if (v0 < v1) { /* keep */ } else { v0 = 0; }
    if (v2 > v1) { /* keep */ } else { v2 = 0; }
    if (v4 > v3) { /* keep */ } else { v4 = 0; }
    if (v6 > v5) { /* keep */ } else { v6 = 0; }
    if (v8 > v7) { /* keep */ } else { v8 = 0; }
    printf("T10: %d %d %d %d %d %d %d %d %d %d\n",
           v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return 0;
}

// T11: Loop with branch inside => loop PHI + inner PHI
// sum=1+..+10=55, fact=10!=3628800
int test11(void) {
    int sum = 0, fact = 1, i = 1;
    while (i <= 10) {
        sum = sum + i;
        if (i <= 10) { fact = fact * i; }
        i = i + 1;
    }
    printf("T11: %d %d\n", sum, fact);
    return 0;
}

// T12: Nested branches with cross-variable dependencies
// a=100,b=10: x=110,y=90; x>10 => z=100, x=210, y>0 => y=190
int test12(int a, int b) {
    int x = a + b, y = a - b, z = 0;
    if (x > 10) {
        z = x - 10;
        x = x + z;
        if (y > 0) { y = y + z; }
        else       { y = y - z; }
    } else {
        z = 10 - x;
        x = x + z;
        y = y + z;
    }
    printf("T12: %d %d %d\n", x, y, z);
    return 0;
}

// T13: 8 variables in 4 pairs, swap loop => loop PHI for all 8+temps
// 3 swaps (odd) => pairs flip: (0,1) -> (1,0)
int test13(void) {
    int a = 0, b = 1, c = 0, d = 1;
    int e = 0, f = 1, g = 0, h = 1;
    int i = 0;
    while (i < 3) {
        int ta = a; a = b; b = ta;
        int tc = c; c = d; d = tc;
        int te = e; e = f; f = te;
        int tg = g; g = h; h = tg;
        i = i + 1;
    }
    printf("T13: %d %d %d %d %d %d %d %d\n", a, b, c, d, e, f, g, h);
    return 0;
}

int main(void) {
    printf("=== PHI Node Stress Test ===\n");
    test1(1);
    test2(1, 0);
    test2b(0, 1);
    test3();
    test4(2);
    test4b(5);
    test5(1);
    test6(0);
    test6b(1);
    test7(5);
    test7b();
    test8(1);
    test8b(2);
    test9a_fn(1, 0);
    test9c_fn(1, 1);
    test9d_fn(1, 0);
    test10();
    test11();
    test12(100, 10);
    test13();
    printf("=== DONE ===\n");
    return 0;
}
