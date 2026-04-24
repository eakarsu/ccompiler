int printf(const char *fmt, ...);

int main(void) {
    int a = 10;
    int *p1 = &a;
    int **p2 = &p1;
    int ***p3 = &p2;
    int ****p4 = &p3;

    // Single level indirection
    // EXPECT: p1 val: 10
    printf("p1 val: %d\n", *p1);

    // Double level indirection
    // EXPECT: p2 val: 10
    printf("p2 val: %d\n", **p2);

    // Triple level indirection
    // EXPECT: p3 val: 10
    printf("p3 val: %d\n", ***p3);

    // Quad level indirection
    // EXPECT: p4 val: 10
    printf("p4 val: %d\n", ****p4);

    // Write through triple pointer
    ***p3 = 20;
    // EXPECT: after p3 write: 20
    printf("after p3 write: %d\n", a);

    // Write through quad pointer
    ****p4 = 30;
    // EXPECT: after p4 write: 30
    printf("after p4 write: %d\n", a);

    // Read back through each level
    // EXPECT: p1 after: 30
    printf("p1 after: %d\n", *p1);
    // EXPECT: p2 after: 30
    printf("p2 after: %d\n", **p2);

    // Redirect chain: make p1 point to a different var
    int b = 100;
    *p2 = &b;
    // EXPECT: redirected p1: 100
    printf("redirected p1: %d\n", *p1);
    // EXPECT: a still: 30
    printf("a still: %d\n", a);

    // Write through redirected chain
    **p2 = 200;
    // EXPECT: b after write: 200
    printf("b after write: %d\n", b);
    // EXPECT: a unchanged: 30
    printf("a unchanged: %d\n", a);

    // Array of values with multi-level pointer access
    int arr[5];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;

    int *ap = arr;
    int **app = &ap;
    int ***appp = &app;

    // EXPECT: arr[0] via triple: 1
    printf("arr[0] via triple: %d\n", ***appp);

    // Pointer arithmetic through double pointer
    *app = arr + 2;
    // EXPECT: arr[2] via triple: 3
    printf("arr[2] via triple: %d\n", ***appp);

    // Write through triple pointer into array
    ***appp = 99;
    // EXPECT: arr[2] modified: 99
    printf("arr[2] modified: %d\n", arr[2]);

    // Chain with multiple variables
    int v1 = 11;
    int v2 = 22;
    int v3 = 33;
    int *pv = &v1;
    int **ppv = &pv;
    int ***pppv = &ppv;

    int sum = 0;
    sum = sum + ***pppv;
    *ppv = &v2;
    sum = sum + ***pppv;
    *ppv = &v3;
    sum = sum + ***pppv;
    // EXPECT: sum of chain: 66
    printf("sum of chain: %d\n", sum);

    // Nested pointer chain in a loop
    int vals[4];
    vals[0] = 10;
    vals[1] = 20;
    vals[2] = 30;
    vals[3] = 40;
    int *vp = &vals[0];
    int **vpp = &vp;

    int total = 0;
    int i = 0;
    while (i < 4) {
        *vpp = &vals[i];
        total = total + **vpp;
        i = i + 1;
    }
    // EXPECT: loop total: 100
    printf("loop total: %d\n", total);

    // Double pointer swap simulation
    int x = 5;
    int y = 10;
    int *px = &x;
    int *py = &y;
    int **ppx = &px;
    int **ppy = &py;

    // Swap pointers via double pointers
    int *tmp = *ppx;
    *ppx = *ppy;
    *ppy = tmp;
    // EXPECT: px now points to: 10
    printf("px now points to: %d\n", *px);
    // EXPECT: py now points to: 5
    printf("py now points to: %d\n", *py);

    // Verify original values unchanged
    // EXPECT: x original: 5
    printf("x original: %d\n", x);
    // EXPECT: y original: 10
    printf("y original: %d\n", y);

    return 0;
}
