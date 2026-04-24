int printf(const char *fmt, ...);

/* Static local variable: persists across calls */
int counter(void) {
    static int count = 0;
    count++;
    return count;
}

int accumulator(int delta) {
    static int total = 0;
    total += delta;
    return total;
}

/* Static local in a function called multiple times */
int next_id(void) {
    static int id = 100;
    return id++;
}

/* Demonstrate variable shadowing at function level vs static */
int shadow_test(void) {
    static int x = 10;
    x += 5;
    return x;
}

/* Returns count of calls */
int call_count(void) {
    static int n = 0;
    return ++n;
}

int main(void) {
    /* Local variable scoping: inner block variable doesn't affect outer */
    int x = 1;
    printf("%d\n", x);          // EXPECT: 1
    {
        int x = 2;
        printf("%d\n", x);      // EXPECT: 2
        {
            int x = 3;
            printf("%d\n", x);  // EXPECT: 3
        }
        printf("%d\n", x);      // EXPECT: 2
    }
    printf("%d\n", x);          // EXPECT: 1

    /* Shadowing: inner variable hides outer without changing it */
    int a = 10;
    {
        int a = 20;
        a += 5;
        printf("%d\n", a);      // EXPECT: 25
    }
    printf("%d\n", a);          // EXPECT: 10

    /* Multiple layers of shadowing */
    int val = 100;
    {
        int val = 200;
        {
            int val = 300;
            val += 1;
            printf("%d\n", val); // EXPECT: 301
        }
        val += 1;
        printf("%d\n", val);     // EXPECT: 201
    }
    val += 1;
    printf("%d\n", val);         // EXPECT: 101

    /* Variable declared in if-block doesn't leak */
    int result = 0;
    if (1) {
        int tmp = 42;
        result = tmp;
    }
    printf("%d\n", result);      // EXPECT: 42

    /* Variable in else block */
    int r2 = 0;
    if (0) {
        int tmp = 1;
        r2 = tmp;
    } else {
        int tmp = 99;
        r2 = tmp;
    }
    printf("%d\n", r2);          // EXPECT: 99

    /* Variable in for-loop body */
    int sum = 0;
    int i;
    for (i = 0; i < 5; i++) {
        int step = i * i;
        sum += step;
    }
    printf("%d\n", sum);         // EXPECT: 30
    /* 0+1+4+9+16 = 30 */

    /* Nested for-loops with inner-block vars */
    int total = 0;
    for (i = 1; i <= 3; i++) {
        int inner;
        for (inner = 1; inner <= 3; inner++) {
            int product = i * inner;
            total += product;
        }
    }
    printf("%d\n", total);       // EXPECT: 36
    /* (1+2+3)+(2+4+6)+(3+6+9) = 6+12+18 = 36 */

    /* While loop body variable */
    int count = 0;
    int j = 0;
    while (j < 4) {
        int doubled = j * 2;
        count += doubled;
        j++;
    }
    printf("%d\n", count);       // EXPECT: 12
    /* 0+2+4+6 = 12 */

    /* Static local: persists across calls */
    printf("%d\n", counter());   // EXPECT: 1
    printf("%d\n", counter());   // EXPECT: 2
    printf("%d\n", counter());   // EXPECT: 3
    printf("%d\n", counter());   // EXPECT: 4
    printf("%d\n", counter());   // EXPECT: 5

    /* Static accumulator */
    printf("%d\n", accumulator(10)); // EXPECT: 10
    printf("%d\n", accumulator(5));  // EXPECT: 15
    printf("%d\n", accumulator(20)); // EXPECT: 35
    printf("%d\n", accumulator(-5)); // EXPECT: 30

    /* Static IDs */
    printf("%d\n", next_id());   // EXPECT: 100
    printf("%d\n", next_id());   // EXPECT: 101
    printf("%d\n", next_id());   // EXPECT: 102

    /* Static shadow_test */
    printf("%d\n", shadow_test()); // EXPECT: 15
    printf("%d\n", shadow_test()); // EXPECT: 20
    printf("%d\n", shadow_test()); // EXPECT: 25

    /* call_count */
    printf("%d\n", call_count()); // EXPECT: 1
    printf("%d\n", call_count()); // EXPECT: 2
    printf("%d\n", call_count()); // EXPECT: 3

    /* Scope in for-loop init (C99 style) */
    int fsum = 0;
    for (int k = 0; k < 5; k++) {
        fsum += k;
    }
    printf("%d\n", fsum);         // EXPECT: 10

    /* Two separate for loops with same init var name */
    int s1 = 0, s2 = 0;
    for (int k = 0; k < 4; k++) s1 += k;
    for (int k = 10; k < 14; k++) s2 += k;
    printf("%d\n", s1);           // EXPECT: 6
    printf("%d\n", s2);           // EXPECT: 46
    /* s1: 0+1+2+3=6, s2: 10+11+12+13=46 */

    return 0;
}
