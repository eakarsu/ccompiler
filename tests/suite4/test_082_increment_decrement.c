int printf(const char *fmt, ...);

int global_counter = 0;

int next_val(void) {
    int old = global_counter;
    global_counter = global_counter + 1;
    return old;
}

int main(void) {
    // Pre-increment basics
    int a = 5;
    int b = ++a;
    // EXPECT: 6
    printf("%d\n", a);
    // EXPECT: 6
    printf("%d\n", b);

    // Post-increment basics
    int c = 10;
    int d = c++;
    // EXPECT: 11
    printf("%d\n", c);
    // EXPECT: 10
    printf("%d\n", d);

    // Pre-decrement basics
    int e = 20;
    int f = --e;
    // EXPECT: 19
    printf("%d\n", e);
    // EXPECT: 19
    printf("%d\n", f);

    // Post-decrement basics
    int g = 30;
    int h = g--;
    // EXPECT: 29
    printf("%d\n", g);
    // EXPECT: 30
    printf("%d\n", h);

    // Increment in arithmetic expression
    int x = 5;
    int result = x++ + 10;
    // x was 5 when used, so result = 15, x is now 6
    // EXPECT: 15
    printf("%d\n", result);
    // EXPECT: 6
    printf("%d\n", x);

    // Pre-increment in arithmetic
    int y = 5;
    int result2 = ++y + 10;
    // y becomes 6 first, result2 = 16
    // EXPECT: 16
    printf("%d\n", result2);
    // EXPECT: 6
    printf("%d\n", y);

    // Increment in for loop
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum = sum + i;
    }
    // 0+1+2+3+4 = 10
    // EXPECT: 10
    printf("%d\n", sum);

    // Decrement in while loop (countdown)
    int count = 5;
    int total = 0;
    while (count > 0) {
        total = total + count;
        count--;
    }
    // 5+4+3+2+1 = 15
    // EXPECT: 15
    printf("%d\n", total);

    // Pre-decrement in while condition
    int n = 6;
    int steps = 0;
    while (--n > 0) {
        steps++;
    }
    // n goes 5,4,3,2,1,0 -> loop runs 5 times
    // EXPECT: 5
    printf("%d\n", steps);

    // Post-increment on pointer
    int arr[5];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    arr[3] = 400;
    arr[4] = 500;
    int *p = arr;
    // EXPECT: 100
    printf("%d\n", *p++);
    // EXPECT: 200
    printf("%d\n", *p++);
    // EXPECT: 300
    printf("%d\n", *p);

    // Pre-increment on pointer
    int *q = arr;
    // EXPECT: 200
    printf("%d\n", *++q);
    // EXPECT: 300
    printf("%d\n", *++q);

    // Increment value pointed to: (*p)++
    int val = 42;
    int *vp = &val;
    (*vp)++;
    // EXPECT: 43
    printf("%d\n", val);

    (*vp)--;
    (*vp)--;
    // EXPECT: 41
    printf("%d\n", val);

    // Increment in array index
    int idx = 0;
    int data[4];
    data[0] = 10;
    data[1] = 20;
    data[2] = 30;
    data[3] = 40;
    // EXPECT: 10
    printf("%d\n", data[idx++]);
    // EXPECT: 20
    printf("%d\n", data[idx++]);
    // EXPECT: 2
    printf("%d\n", idx);

    // Multiple increments in separate statements
    int m = 0;
    m++;
    m++;
    m++;
    ++m;
    ++m;
    // EXPECT: 5
    printf("%d\n", m);

    // Increment in do-while
    int dw = 0;
    int dw_sum = 0;
    do {
        dw_sum = dw_sum + dw;
        dw++;
    } while (dw < 4);
    // 0+1+2+3 = 6
    // EXPECT: 6
    printf("%d\n", dw_sum);

    // Decrement to traverse array backward
    int rev[5];
    rev[0] = 1;
    rev[1] = 2;
    rev[2] = 3;
    rev[3] = 4;
    rev[4] = 5;
    int ri = 4;
    int rev_sum = 0;
    while (ri >= 0) {
        rev_sum = rev_sum + rev[ri];
        ri--;
    }
    // 5+4+3+2+1 = 15
    // EXPECT: 15
    printf("%d\n", rev_sum);

    // Post-increment result used in comparison
    int cmp = 3;
    int was_three = (cmp++ == 3);
    // EXPECT: 1
    printf("%d\n", was_three);
    // EXPECT: 4
    printf("%d\n", cmp);

    // Pre-increment result used in comparison
    int cmp2 = 3;
    int is_four = (++cmp2 == 4);
    // EXPECT: 1
    printf("%d\n", is_four);

    return 0;
}
