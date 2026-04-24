int printf(const char *fmt, ...);

int main(void) {
    int val = 42;
    int *p = &val;
    int **pp = &p;

    // EXPECT: val = 42
    printf("val = %d\n", val);

    // EXPECT: *p = 42
    printf("*p = %d\n", *p);

    // EXPECT: **pp = 42
    printf("**pp = %d\n", **pp);

    // Modify through double pointer
    **pp = 100;
    // EXPECT: val_after = 100
    printf("val_after = %d\n", val);

    // Triple indirection
    int x = 7;
    int *px = &x;
    int **ppx = &px;
    int ***pppx = &ppx;

    // EXPECT: ***pppx = 7
    printf("***pppx = %d\n", ***pppx);

    ***pppx = 99;
    // EXPECT: x_after = 99
    printf("x_after = %d\n", x);

    // Redirect pointer through double pointer
    int a = 10;
    int b = 20;
    int *ptr = &a;
    int **pptr = &ptr;
    // EXPECT: before_redirect = 10
    printf("before_redirect = %d\n", **pptr);

    *pptr = &b;
    // EXPECT: after_redirect = 20
    printf("after_redirect = %d\n", **pptr);

    // EXPECT: ptr_now_b = 20
    printf("ptr_now_b = %d\n", *ptr);

    // Array of pointers with double indirection
    int v0 = 100;
    int v1 = 200;
    int v2 = 300;
    int *arr[3];
    arr[0] = &v0;
    arr[1] = &v1;
    arr[2] = &v2;

    int **ap = arr;
    // EXPECT: ap[0] = 100
    printf("ap[0] = %d\n", *ap[0]);

    // EXPECT: ap[1] = 200
    printf("ap[1] = %d\n", *ap[1]);

    // EXPECT: ap[2] = 300
    printf("ap[2] = %d\n", *ap[2]);

    // Modify via array of pointers
    *arr[1] = 999;
    // EXPECT: v1_modified = 999
    printf("v1_modified = %d\n", v1);

    // Swap via double pointers
    int sa = 11;
    int sb = 22;
    int *spa = &sa;
    int *spb = &sb;
    int **ppa = &spa;
    int **ppb = &spb;

    int *tmp = *ppa;
    *ppa = *ppb;
    *ppb = tmp;

    // EXPECT: spa_now = 22
    printf("spa_now = %d\n", *spa);

    // EXPECT: spb_now = 11
    printf("spb_now = %d\n", *spb);

    // sa and sb unchanged, just pointers swapped
    // EXPECT: sa_unchanged = 11
    printf("sa_unchanged = %d\n", sa);

    // EXPECT: sb_unchanged = 22
    printf("sb_unchanged = %d\n", sb);

    // Chain: modify a value through 3 levels
    int deep = 0;
    int *dp = &deep;
    int **dpp = &dp;
    int ***dppp = &dpp;

    ***dppp = 555;
    // EXPECT: deep = 555
    printf("deep = %d\n", deep);

    // EXPECT: *dp = 555
    printf("*dp = %d\n", *dp);

    // EXPECT: **dpp = 555
    printf("**dpp = %d\n", **dpp);

    // Sum through double pointers
    int nums[4];
    nums[0] = 1; nums[1] = 2; nums[2] = 3; nums[3] = 4;
    int *ptrs[4];
    int i;
    for (i = 0; i < 4; i++) {
        ptrs[i] = &nums[i];
    }
    int **pp2 = ptrs;
    int sum = 0;
    for (i = 0; i < 4; i++) {
        sum = sum + *pp2[i];
    }
    // EXPECT: sum = 10
    printf("sum = %d\n", sum);

    return 0;
}
