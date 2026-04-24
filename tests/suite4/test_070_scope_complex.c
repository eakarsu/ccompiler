int printf(const char *fmt, ...);

// Test complex scope scenarios: same name in different scopes,
// accessing outer scope after inner scope ends.

int g_x = 1000;

int same_name_different_scope(int flag) {
    int result = 0;
    if (flag == 1) {
        int val = 10;
        result = val;
    }
    if (flag == 2) {
        int val = 20;
        result = val;
    }
    if (flag == 3) {
        int val = 30;
        result = val;
    }
    return result;
}

int outer_survives_inner(void) {
    int x = 100;
    {
        int y = 50;
        x = x + y;
    }
    // y is gone, x = 150
    {
        int z = 25;
        x = x + z;
    }
    // z is gone, x = 175
    return x;
}

int complex_for_scope(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        int partial = 0;
        int j;
        for (j = 0; j <= i; j = j + 1) {
            int term = (i + 1) * (j + 1);
            partial = partial + term;
        }
        sum = sum + partial;
    }
    // i=0: partial = 1*1 = 1
    // i=1: partial = 2*1 + 2*2 = 2+4 = 6
    // i=2: partial = 3*1 + 3*2 + 3*3 = 3+6+9 = 18
    // sum = 1+6+18 = 25
    return sum;
}

int switch_scope_test(int x) {
    int result = 0;
    switch (x) {
        case 1: {
            int val = 100;
            result = val;
            break;
        }
        case 2: {
            int val = 200;
            result = val;
            break;
        }
        case 3: {
            int val = 300;
            result = val;
            break;
        }
    }
    return result;
}

int redefine_after_block(void) {
    int total = 0;
    {
        int x = 5;
        total = total + x;
    }
    // x from above block is gone, define a new one
    {
        int x = 10;
        total = total + x;
    }
    {
        int x = 15;
        total = total + x;
    }
    return total;
}

int shadow_chain(int n) {
    int x = n;
    {
        int x = n * 2;
        {
            int x = n * 3;
            {
                int x = n * 4;
                // innermost, return from within
                if (n == 1) return x;
            }
            if (n == 2) return x;
        }
        if (n == 3) return x;
    }
    return x;
}

int scope_with_ternary(int a) {
    int result;
    {
        int x = a > 0 ? a : -a;
        result = x;
    }
    return result;
}

int global_vs_local(void) {
    // g_x = 1000
    int sum = g_x;
    {
        int g_x = 5;
        sum = sum + g_x;
    }
    sum = sum + g_x;
    // sum = 1000 + 5 + 1000 = 2005
    return sum;
}

int nested_loops_scope(void) {
    int total = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        int row_sum = 0;
        int j;
        for (j = 0; j < 3; j = j + 1) {
            int cell = i * 10 + j;
            row_sum = row_sum + cell;
        }
        total = total + row_sum;
    }
    // row 0: 0+1+2 = 3
    // row 1: 10+11+12 = 33
    // row 2: 20+21+22 = 63
    // total = 3+33+63 = 99
    return total;
}

int conditional_scopes(int a, int b) {
    int result = 0;
    if (a > b) {
        int diff = a - b;
        result = diff;
    } else if (a < b) {
        int diff = b - a;
        result = -diff;
    } else {
        int diff = 0;
        result = diff;
    }
    // diff is gone
    return result;
}

int scope_modify_outer(void) {
    int x = 1;
    int y = 2;
    {
        x = x + 10;
        y = y + 20;
        {
            x = x + 100;
            y = y + 200;
        }
    }
    // x = 1+10+100 = 111, y = 2+20+200 = 222
    return x + y;
}

int main(void) {
    // Same name in different branches
    // EXPECT: 10
    printf("%d\n", same_name_different_scope(1));
    // EXPECT: 20
    printf("%d\n", same_name_different_scope(2));
    // EXPECT: 30
    printf("%d\n", same_name_different_scope(3));
    // EXPECT: 0
    printf("%d\n", same_name_different_scope(4));

    // Outer survives inner: 100+50+25 = 175
    // EXPECT: 175
    printf("%d\n", outer_survives_inner());

    // Complex for scope: 1+6+18 = 25
    // EXPECT: 25
    printf("%d\n", complex_for_scope());

    // Switch with scoped blocks
    // EXPECT: 100
    printf("%d\n", switch_scope_test(1));
    // EXPECT: 200
    printf("%d\n", switch_scope_test(2));
    // EXPECT: 300
    printf("%d\n", switch_scope_test(3));

    // Redefine after block: 5+10+15 = 30
    // EXPECT: 30
    printf("%d\n", redefine_after_block());

    // shadow_chain: n=1 returns 4, n=2 returns 6, n=3 returns 6, n=4 returns 4
    // EXPECT: 4
    printf("%d\n", shadow_chain(1));
    // EXPECT: 6
    printf("%d\n", shadow_chain(2));
    // EXPECT: 6
    printf("%d\n", shadow_chain(3));
    // EXPECT: 4
    printf("%d\n", shadow_chain(4));

    // Scope with ternary
    // EXPECT: 7
    printf("%d\n", scope_with_ternary(7));
    // EXPECT: 5
    printf("%d\n", scope_with_ternary(-5));

    // Global vs local
    // EXPECT: 2005
    printf("%d\n", global_vs_local());

    // Nested loops scope: 3+33+63 = 99
    // EXPECT: 99
    printf("%d\n", nested_loops_scope());

    // Conditional scopes
    // a=10, b=3: diff=7
    // EXPECT: 7
    printf("%d\n", conditional_scopes(10, 3));
    // a=3, b=10: diff=7, result=-7
    // EXPECT: -7
    printf("%d\n", conditional_scopes(3, 10));
    // a=5, b=5: result=0
    // EXPECT: 0
    printf("%d\n", conditional_scopes(5, 5));

    // scope_modify_outer: 111+222=333
    // EXPECT: 333
    printf("%d\n", scope_modify_outer());

    // Global unchanged
    // EXPECT: 1000
    printf("%d\n", g_x);

    return 0;
}
