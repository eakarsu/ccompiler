int printf(const char *fmt, ...);

/* test_021_pointer_basics.c
   Tests: pointer declaration, &, *, pointer arithmetic (+, -), pointer comparison,
          NULL pointer check, pointer to pointer
*/

void print_via_ptr(int *p) {
    printf("%d\n", *p);
}

void increment_via_ptr(int *p) {
    *p = *p + 1;
}

int sum_array_ptr(int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + *(p + i);
    }
    return s;
}

int main(void) {
    /* basic pointer declaration and dereference */
    int a = 42;
    int *pa = &a;
    printf("%d\n", *pa);
    // EXPECT: 42

    /* writing through pointer */
    *pa = 100;
    printf("%d\n", a);
    // EXPECT: 100

    /* address-of and back */
    int b = 7;
    int *pb = &b;
    printf("%d\n", *pb);
    // EXPECT: 7

    /* pointer passed to function, read */
    int c = 55;
    print_via_ptr(&c);
    // EXPECT: 55

    /* pointer passed to function, write */
    increment_via_ptr(&c);
    printf("%d\n", c);
    // EXPECT: 56

    /* pointer arithmetic: incrementing pointer over array */
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    int *p = arr;
    printf("%d\n", *p);
    // EXPECT: 10
    printf("%d\n", *(p + 1));
    // EXPECT: 20
    printf("%d\n", *(p + 4));
    // EXPECT: 50

    /* advance pointer with ++ */
    p = arr;
    p++;
    printf("%d\n", *p);
    // EXPECT: 20
    p = p + 2;
    printf("%d\n", *p);
    // EXPECT: 40

    /* pointer subtraction: distance between pointers */
    int *q = arr + 4;
    int *r = arr;
    int diff = (int)(q - r);
    printf("%d\n", diff);
    // EXPECT: 4

    /* pointer comparison */
    int *lo = arr;
    int *hi = arr + 3;
    if (lo < hi) {
        printf("lo<hi\n");
        // EXPECT: lo<hi
    }
    if (hi > lo) {
        printf("hi>lo\n");
        // EXPECT: hi>lo
    }
    if (lo != hi) {
        printf("ne\n");
        // EXPECT: ne
    }
    p = arr + 2;
    if (p == arr + 2) {
        printf("eq\n");
        // EXPECT: eq
    }

    /* NULL pointer check */
    int *null_ptr = 0;
    if (null_ptr == 0) {
        printf("null\n");
        // EXPECT: null
    }
    if (!null_ptr) {
        printf("null2\n");
        // EXPECT: null2
    }

    /* pointer to pointer */
    int x = 99;
    int *px = &x;
    int **ppx = &px;
    printf("%d\n", **ppx);
    // EXPECT: 99
    **ppx = 77;
    printf("%d\n", x);
    // EXPECT: 77

    /* pointer to pointer: change what pointer points to */
    int v1 = 11;
    int v2 = 22;
    int *pv = &v1;
    int **ppv = &pv;
    printf("%d\n", *pv);
    // EXPECT: 11
    *ppv = &v2;
    printf("%d\n", *pv);
    // EXPECT: 22

    /* use pointer arithmetic to sum */
    int data[6];
    data[0] = 1; data[1] = 2; data[2] = 3;
    data[3] = 4; data[4] = 5; data[5] = 6;
    int total = sum_array_ptr(data, 6);
    printf("%d\n", total);
    // EXPECT: 21

    /* step backward with pointer */
    int *end = data + 5;
    printf("%d\n", *end);
    // EXPECT: 6
    printf("%d\n", *(end - 1));
    // EXPECT: 5
    printf("%d\n", *(end - 2));
    // EXPECT: 4

    /* pointer arithmetic and subscript equivalence */
    int vals[4];
    vals[0] = 100; vals[1] = 200; vals[2] = 300; vals[3] = 400;
    int *pv2 = vals;
    printf("%d\n", pv2[2]);
    // EXPECT: 300
    printf("%d\n", *(pv2 + 3));
    // EXPECT: 400

    /* compare pointer to itself */
    int *same = arr;
    if (same == arr) {
        printf("same\n");
        // EXPECT: same
    }

    return 0;
}
