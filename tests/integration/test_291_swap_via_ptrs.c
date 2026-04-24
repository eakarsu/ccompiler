int printf(const char *fmt, ...);
// EXPECT: before: x=10 y=20\nafter swap: x=20 y=10\nno temp: a=200 b=100\nxor swap: p=99 q=42\nchar swap: Z A\nbefore rotate: 1 2 3\nafter rotate: 2 3 1\nsort2: 3 5\nbefore sort3: 30 10 20\nafter sort3: 10 20 30\nbefore: 5 4 3 2 1 \nreversed: 1 2 3 4 5 
// Test: swap via pointers (various types and patterns)

void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_char(char *a, char *b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

// Swap without temp (using arithmetic)
void swap_no_temp(int *a, int *b) {
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

// Swap using XOR
void swap_xor(int *a, int *b) {
    if (a == b) return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// Rotate three values: a->b->c->a
void rotate3(int *a, int *b, int *c) {
    int tmp = *a;
    *a = *b;
    *b = *c;
    *c = tmp;
}

// Conditional swap (sort two values)
void sort2(int *a, int *b) {
    if (*a > *b) swap_int(a, b);
}

// Sort three values using swaps
void sort3(int *a, int *b, int *c) {
    sort2(a, b);
    sort2(b, c);
    sort2(a, b);
}

int main(void) {
    // Basic swap
    int x = 10; int y = 20;
    printf("before: x=%d y=%d\n", x, y);
    swap_int(&x, &y);
    printf("after swap: x=%d y=%d\n", x, y);

    // Swap without temp
    int a = 100; int b = 200;
    swap_no_temp(&a, &b);
    printf("no temp: a=%d b=%d\n", a, b);

    // XOR swap
    int p = 42; int q = 99;
    swap_xor(&p, &q);
    printf("xor swap: p=%d q=%d\n", p, q);

    // Char swap
    char c1 = 'A'; char c2 = 'Z';
    swap_char(&c1, &c2);
    printf("char swap: %c %c\n", c1, c2);

    // Rotate 3
    int r1 = 1; int r2 = 2; int r3 = 3;
    printf("before rotate: %d %d %d\n", r1, r2, r3);
    rotate3(&r1, &r2, &r3);
    printf("after rotate: %d %d %d\n", r1, r2, r3);

    // Sort 2
    int s1 = 5; int s2 = 3;
    sort2(&s1, &s2);
    printf("sort2: %d %d\n", s1, s2);

    // Sort 3
    int t1 = 30; int t2 = 10; int t3 = 20;
    printf("before sort3: %d %d %d\n", t1, t2, t3);
    sort3(&t1, &t2, &t3);
    printf("after sort3: %d %d %d\n", t1, t2, t3);

    // Swap array elements
    int arr[5];
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    printf("before: ");
    int i;
    for (i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    // Reverse via swap
    int lo = 0; int hi = 4;
    while (lo < hi) {
        swap_int(&arr[lo], &arr[hi]);
        lo = lo + 1;
        hi = hi - 1;
    }
    printf("reversed: ");
    for (i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
