int printf(const char *fmt, ...);

// Test: Pointer to pointer (double/triple indirection)

void swap_ptrs(int **a, int **b) {
    int *tmp = *a;
    *a = *b;
    *b = tmp;
}

void set_via_pp(int **pp, int val) {
    printf("setting via pp\n");
    **pp = val;
}

int main() {
    // Basic pointer to pointer
    int x = 42;
    int *px = &x;
    int **ppx = &px;
    // EXPECT: x = 42
    printf("x = %d\n", x);
    // EXPECT: *px = 42
    printf("*px = %d\n", *px);
    // EXPECT: **ppx = 42
    printf("**ppx = %d\n", **ppx);

    // Modify through double pointer
    **ppx = 99;
    // EXPECT: x after pp mod: 99
    printf("x after pp mod: %d\n", x);

    // Redirect pointer through pp
    int y = 77;
    *ppx = &y;
    // EXPECT: **ppx after redirect: 77
    printf("**ppx after redirect: %d\n", **ppx);
    // EXPECT: *px now: 77
    printf("*px now: %d\n", *px);

    // Swap two pointers using pointer-to-pointer
    int a = 10;
    int b = 20;
    int *pa = &a;
    int *pb = &b;
    // EXPECT: before swap: 10 20
    printf("before swap: %d %d\n", *pa, *pb);
    swap_ptrs(&pa, &pb);
    // EXPECT: after swap: 20 10
    printf("after swap: %d %d\n", *pa, *pb);

    // Array of pointers accessed via pointer to pointer
    int v1 = 111;
    int v2 = 222;
    int v3 = 333;
    int *ptrs[3];
    ptrs[0] = &v1;
    ptrs[1] = &v2;
    ptrs[2] = &v3;

    int **pp = ptrs;
    // EXPECT: pp[0] = 111
    printf("pp[0] = %d\n", *pp[0]);
    // EXPECT: pp[1] = 222
    printf("pp[1] = %d\n", *pp[1]);
    // EXPECT: pp[2] = 333
    printf("pp[2] = %d\n", *pp[2]);

    // Walk array of pointers via double pointer
    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + **(pp + i);
    }
    // EXPECT: sum via pp: 666
    printf("sum via pp: %d\n", sum);

    // Set value via function taking **
    int z = 0;
    int *pz = &z;
    // EXPECT: setting via pp
    set_via_pp(&pz, 55);
    // EXPECT: z after set_via_pp: 55
    printf("z after set_via_pp: %d\n", z);

    // Triple pointer
    int val = 7;
    int *p1 = &val;
    int **p2 = &p1;
    int ***p3 = &p2;
    // EXPECT: ***p3 = 7
    printf("***p3 = %d\n", ***p3);

    ***p3 = 88;
    // EXPECT: val after ***p3 = 88
    printf("val after ***p3 = %d\n", val);

    // Double pointer to modify which array element a pointer targets
    int arr[4];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    int *cursor = &arr[0];
    int **pcursor = &cursor;
    // EXPECT: cursor at: 10
    printf("cursor at: %d\n", **pcursor);
    *pcursor = &arr[2];
    // EXPECT: cursor moved to: 30
    printf("cursor moved to: %d\n", **pcursor);

    // EXPECT: ptr_to_ptr done
    printf("ptr_to_ptr done\n");

    return 0;
}
