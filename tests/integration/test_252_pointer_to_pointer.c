int printf(const char *fmt, ...);
// EXPECT: val = 42\n*p = 42\n**pp = 42\nafter **pp=100: val = 100\n*p now = 999\n**pp now = 999\n***pppa = 1\na after ***pppa=77: 77\ndpp[0] = 10\ndpp[1] = 20\ndpp[2] = 30\ndpp[3] = 40\ndpp[4] = 50\narr[2] after mod = 555\npx->22 py->11
// Test: pointer to pointer (double indirection)

int main(void) {
    int val = 42;
    int *p = &val;
    int **pp = &p;

    printf("val = %d\n", val);
    printf("*p = %d\n", *p);
    printf("**pp = %d\n", **pp);

    // Modify through double pointer
    **pp = 100;
    printf("after **pp=100: val = %d\n", val);

    // Change what p points to through pp
    int other = 999;
    *pp = &other;
    printf("*p now = %d\n", *p);
    printf("**pp now = %d\n", **pp);

    // Triple pointer
    int a = 1;
    int *pa = &a;
    int **ppa = &pa;
    int ***pppa = &ppa;
    printf("***pppa = %d\n", ***pppa);
    ***pppa = 77;
    printf("a after ***pppa=77: %d\n", a);

    // Array of pointers with double pointer
    int arr[5];
    int i;
    for (i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
    }

    int *ptrs[5];
    for (i = 0; i < 5; i++) {
        ptrs[i] = &arr[i];
    }

    int **dpp = ptrs;
    for (i = 0; i < 5; i++) {
        printf("dpp[%d] = %d\n", i, *dpp[i]);
    }

    // Modify array through double pointer
    *dpp[2] = 555;
    printf("arr[2] after mod = %d\n", arr[2]);

    // Swap pointers using pointer-to-pointer
    int x = 11;
    int y = 22;
    int *px = &x;
    int *py = &y;
    int *tmp = px;
    px = py;
    py = tmp;
    printf("px->%d py->%d\n", *px, *py);

    return 0;
}
