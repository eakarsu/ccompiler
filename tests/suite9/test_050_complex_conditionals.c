int printf(const char *fmt, ...);

typedef int (*Comparator)(int, int);

int ascending(int a, int b) { return a - b; }
int descending(int a, int b) { return b - a; }

void sort3(int *a, int *b, int *c, Comparator cmp) {
    int tmp;
    if (cmp(*a, *b) > 0) { tmp = *a; *a = *b; *b = tmp; }
    if (cmp(*b, *c) > 0) { tmp = *b; *b = *c; *c = tmp; }
    if (cmp(*a, *b) > 0) { tmp = *a; *a = *b; *b = tmp; }
}

int eval_rpn(int *tokens, int len) {
    int stack[20];
    int sp = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        int t = tokens[i];
        if (t >= 0 && t <= 999) {
            stack[sp] = t;
            sp = sp + 1;
        } else {
            int b = stack[sp - 1]; sp = sp - 1;
            int a = stack[sp - 1]; sp = sp - 1;
            int r;
            if (t == -1) r = a + b;
            else if (t == -2) r = a - b;
            else if (t == -3) r = a * b;
            else if (t == -4) {
                if (b == 0) r = 0;
                else r = a / b;
            } else {
                r = 0;
            }
            stack[sp] = r;
            sp = sp + 1;
        }
    }
    return stack[0];
}

int count_paths(int grid[4][4], int rows, int cols) {
    int dp[4][4];
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = (grid[0][0] == 0) ? 1 : 0;
    for (i = 1; i < rows; i = i + 1) {
        dp[i][0] = (grid[i][0] == 0 && dp[i-1][0] > 0) ? 1 : 0;
    }
    for (j = 1; j < cols; j = j + 1) {
        dp[0][j] = (grid[0][j] == 0 && dp[0][j-1] > 0) ? 1 : 0;
    }
    for (i = 1; i < rows; i = i + 1) {
        for (j = 1; j < cols; j = j + 1) {
            if (grid[i][j] == 0) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            } else {
                dp[i][j] = 0;
            }
        }
    }
    return dp[rows-1][cols-1];
}

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
        steps = steps + 1;
        if (steps > 200) break;
    }
    return steps;
}

void test_conditional_assignment() {
    int arr[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = (i % 2 == 0)
            ? ((i % 4 == 0) ? i * 10 : i * 5)
            : ((i % 3 == 0) ? i * 3 : i * 2);
    }
    int sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + arr[i];
    }
    printf("conditional arr sum: %d\n", sum);
}

void test_loop_condition_complexity() {
    int i = 0;
    int j = 100;
    int steps = 0;
    while ((i < j) && (i * i < 500 || j > 50)) {
        i = i + 3;
        j = j - 2;
        steps = steps + 1;
    }
    printf("converge i: %d, j: %d, steps: %d\n", i, j, steps);
}

void test_switch_conditional_combo() {
    int results[12];
    int i;
    for (i = 0; i < 12; i = i + 1) {
        int category;
        switch (i % 4) {
            case 0: category = (i < 6) ? 1 : 2; break;
            case 1: category = (i > 4) ? 3 : 4; break;
            case 2: category = 5; break;
            default: category = (i % 3 == 0) ? 6 : 7; break;
        }
        results[i] = category;
    }
    int cat_sum = 0;
    for (i = 0; i < 12; i = i + 1) {
        cat_sum = cat_sum + results[i];
    }
    printf("category sum: %d\n", cat_sum);
}

int main() {
    int a = 7;
    int b = 2;
    int c = 9;
    sort3(&a, &b, &c, ascending);
    // EXPECT: sorted asc: 2 7 9
    printf("sorted asc: %d %d %d\n", a, b, c);

    a = 7; b = 2; c = 9;
    sort3(&a, &b, &c, descending);
    // EXPECT: sorted desc: 9 7 2
    printf("sorted desc: %d %d %d\n", a, b, c);

    int rpn1[] = {3, 4, -1, 2, -3};
    // EXPECT: rpn (3+4)*2: 14
    printf("rpn (3+4)*2: %d\n", eval_rpn(rpn1, 5));

    int rpn2[] = {5, 1, 2, -1, 4, -3, -1, 3, -2};
    // EXPECT: rpn 5+((1+2)*4)-3: 14
    printf("rpn 5+((1+2)*4)-3: %d\n", eval_rpn(rpn2, 9));

    int grid[4][4];
    int i;
    int j;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            grid[i][j] = 0;
        }
    }
    grid[1][1] = 1;
    grid[2][2] = 1;
    // EXPECT: grid paths: 4
    printf("grid paths: %d\n", count_paths(grid, 4, 4));

    // EXPECT: collatz 27: 111
    printf("collatz 27: %d\n", collatz_steps(27));
    // EXPECT: collatz 1: 0
    printf("collatz 1: %d\n", collatz_steps(1));

    int max_steps = 0;
    int max_n = 0;
    for (i = 1; i <= 50; i = i + 1) {
        int s = collatz_steps(i);
        if (s > max_steps) {
            max_steps = s;
            max_n = i;
        }
    }
    // EXPECT: longest collatz under 50: n=27 steps=111
    printf("longest collatz under 50: n=%d steps=%d\n", max_n, max_steps);

    // EXPECT: conditional arr sum: 222
    test_conditional_assignment();
    // EXPECT: converge i: 60, j: 60, steps: 20
    test_loop_condition_complexity();
    // EXPECT: category sum: 49
    test_switch_conditional_combo();

    return 0;
}
