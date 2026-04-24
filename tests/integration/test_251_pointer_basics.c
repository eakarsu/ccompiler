int printf(const char *fmt, ...);
// EXPECT: a = 10\n*p = 10\nafter *p=42: a = 42\n*p (now b) = 20\nq == r (both point to a)\nsum via ptrs = 92\na=100 b=200 c=300\nx after +=10: 15\nnull pointer detected\nswapped: m=22 n=11
// Test: pointer basics - declaration, dereference, address-of

int main(void) {
    int a = 10;
    int b = 20;
    int c = 30;

    // Basic pointer declaration and dereference
    int *p = &a;
    printf("a = %d\n", a);
    printf("*p = %d\n", *p);

    // Modify through pointer
    *p = 42;
    printf("after *p=42: a = %d\n", a);

    // Reassign pointer
    p = &b;
    printf("*p (now b) = %d\n", *p);

    // Address-of and pointer comparison
    int *q = &a;
    int *r = &a;
    if (q == r) {
        printf("q == r (both point to a)\n");
    }

    // Pointer to different variables
    int *ptrs[3];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;

    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + *ptrs[i];
    }
    printf("sum via ptrs = %d\n", sum);

    // Modify array elements via pointers
    *ptrs[0] = 100;
    *ptrs[1] = 200;
    *ptrs[2] = 300;
    printf("a=%d b=%d c=%d\n", a, b, c);

    // Chain: pointer to value, modify, read back
    int x = 5;
    int *px = &x;
    *px = *px + 10;
    printf("x after +=10: %d\n", x);

    // Null-like check (use 0)
    int *np = 0;
    if (np == 0) {
        printf("null pointer detected\n");
    }

    // Swap via pointers
    int m = 11;
    int n = 22;
    int *pm = &m;
    int *pn = &n;
    int tmp = *pm;
    *pm = *pn;
    *pn = tmp;
    printf("swapped: m=%d n=%d\n", m, n);

    return 0;
}
