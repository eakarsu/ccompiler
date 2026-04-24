int printf(const char *fmt, ...);

void set_via_pp(int **pp, int val) {
    **pp = val;
}

void redirect_ptr(int **pp, int *new_target) {
    *pp = new_target;
}

void swap_ptrs(int **pa, int **pb) {
    int *tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

int read_pp(int **pp) {
    return **pp;
}

void increment_via_pp(int **pp) {
    **pp = **pp + 1;
}

void zero_array_via_pp(int **pp, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        *(*pp + i) = 0;
    }
}

int sum_via_pp(int **pp, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + *(*pp + i);
    }
    return s;
}

int main(void) {
    int x = 10;
    int *p = &x;
    int **pp = &p;

    // EXPECT: 10
    printf("%d\n", **pp);

    **pp = 20;
    // EXPECT: 20
    printf("%d\n", x);
    // EXPECT: 20
    printf("%d\n", *p);
    // EXPECT: 20
    printf("%d\n", **pp);

    int y = 30;
    *pp = &y;
    // EXPECT: 30
    printf("%d\n", *p);
    // EXPECT: 30
    printf("%d\n", **pp);

    set_via_pp(&p, 55);
    // EXPECT: 55
    printf("%d\n", y);

    int z = 99;
    redirect_ptr(&p, &z);
    // EXPECT: 99
    printf("%d\n", *p);

    int a = 1;
    int b = 2;
    int *pa = &a;
    int *pb = &b;
    swap_ptrs(&pa, &pb);
    // EXPECT: 2
    printf("%d\n", *pa);
    // EXPECT: 1
    printf("%d\n", *pb);

    int val = 42;
    int *vp = &val;
    // EXPECT: 42
    printf("%d\n", read_pp(&vp));

    increment_via_pp(&vp);
    // EXPECT: 43
    printf("%d\n", val);

    increment_via_pp(&vp);
    increment_via_pp(&vp);
    // EXPECT: 45
    printf("%d\n", val);

    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int *ap = arr;

    // EXPECT: 150
    printf("%d\n", sum_via_pp(&ap, 5));

    zero_array_via_pp(&ap, 5);
    // EXPECT: 0
    printf("%d\n", arr[0]);
    // EXPECT: 0
    printf("%d\n", arr[4]);
    // EXPECT: 0
    printf("%d\n", sum_via_pp(&ap, 5));

    /* Triple pointer */
    int w = 77;
    int *wp = &w;
    int **wpp = &wp;
    int ***wppp = &wpp;
    // EXPECT: 77
    printf("%d\n", ***wppp);

    ***wppp = 88;
    // EXPECT: 88
    printf("%d\n", w);

    /* Array of pointers */
    int v1 = 100;
    int v2 = 200;
    int v3 = 300;
    int *ptrs[3];
    ptrs[0] = &v1;
    ptrs[1] = &v2;
    ptrs[2] = &v3;

    // EXPECT: 100
    printf("%d\n", *ptrs[0]);
    // EXPECT: 200
    printf("%d\n", *ptrs[1]);
    // EXPECT: 300
    printf("%d\n", *ptrs[2]);

    *ptrs[1] = 250;
    // EXPECT: 250
    printf("%d\n", v2);

    int sum = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        sum = sum + *ptrs[i];
    }
    // EXPECT: 650
    printf("%d\n", sum);

    return 0;
}
