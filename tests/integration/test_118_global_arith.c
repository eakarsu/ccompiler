int printf(const char *fmt, ...);
// EXPECT: ga=10 gb=20 gc=30\nga+gb: ga=30\nga*2: ga=60\nga*gb+gc = 230\nbefore swap: ga=10 gb=20\nafter swap: ga=20 gb=10\ncounter: 0 1 2 3 4 \ncounter now = 5\nsum of squares = 385\ng_arr: 30 10 200\nga after loop = 5
// Test: arithmetic with global variables

int ga = 10;
int gb = 20;
int gc = 30;
int g_arr[10];
int g_counter = 0;

int get_and_increment(void) {
    int val;
    val = g_counter;
    g_counter = g_counter + 1;
    return val;
}

void swap_globals(void) {
    int tmp;
    tmp = ga;
    ga = gb;
    gb = tmp;
}

int sum_global_array(int n) {
    int i, s;
    s = 0;
    for (i = 0; i < n; i++) {
        s += g_arr[i];
    }
    return s;
}

int main(void) {
    int i;
    int r;

    /* Read globals */
    printf("ga=%d gb=%d gc=%d\n", ga, gb, gc);

    /* Arithmetic on globals */
    ga = ga + gb;
    printf("ga+gb: ga=%d\n", ga);

    ga = ga * 2;
    printf("ga*2: ga=%d\n", ga);

    /* Reset and use in expression */
    ga = 10;
    gb = 20;
    r = ga * gb + gc;
    printf("ga*gb+gc = %d\n", r);

    /* Swap globals */
    printf("before swap: ga=%d gb=%d\n", ga, gb);
    swap_globals();
    printf("after swap: ga=%d gb=%d\n", ga, gb);

    /* Global counter */
    printf("counter: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", get_and_increment());
    }
    printf("\n");
    printf("counter now = %d\n", g_counter);

    /* Fill and sum global array */
    for (i = 0; i < 10; i++) {
        g_arr[i] = (i + 1) * (i + 1);
    }
    r = sum_global_array(10);
    printf("sum of squares = %d\n", r);

    /* Modify global array elements */
    g_arr[0] = ga + gb;
    g_arr[1] = ga - gb;
    g_arr[2] = ga * gb;
    printf("g_arr: %d %d %d\n", g_arr[0], g_arr[1], g_arr[2]);

    /* Global in loop condition */
    ga = 0;
    while (ga < 5) {
        ga++;
    }
    printf("ga after loop = %d\n", ga);

    return 0;
}
