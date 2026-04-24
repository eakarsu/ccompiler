int printf(const char *fmt, ...);

// Test 091: Complex pointer arithmetic (multi-level, array decay)

int global_arr[5] = {10, 20, 30, 40, 50};

void test_array_decay(void) {
    int a[4] = {100, 200, 300, 400};
    int *p = a;

    printf("%d\n", *p);
    // EXPECT: 100
    printf("%d\n", *(p + 2));
    // EXPECT: 300
    printf("%d\n", p[3]);
    // EXPECT: 400

    // pointer subtraction
    int *q = &a[3];
    int diff = (int)(q - p);
    printf("%d\n", diff);
    // EXPECT: 3

    // array name as pointer
    printf("%d\n", *(a + 1));
    // EXPECT: 200
}

void test_pointer_to_pointer(void) {
    int x = 42;
    int y = 99;
    int *px = &x;
    int *py = &y;
    int **pp = &px;

    printf("%d\n", **pp);
    // EXPECT: 42

    *pp = py;  // now pp points to py
    printf("%d\n", **pp);
    // EXPECT: 99

    **pp = 77;  // modify y through pp
    printf("%d\n", y);
    // EXPECT: 77
}

void test_array_of_pointers(void) {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int *ptrs[4];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    ptrs[3] = &d;

    int sum = 0;
    int i;
    for (i = 0; i < 4; i++) {
        sum = sum + *ptrs[i];
    }
    printf("%d\n", sum);
    // EXPECT: 10

    // modify through pointer array
    *ptrs[2] = 30;
    printf("%d\n", c);
    // EXPECT: 30
}

void test_pointer_stepping(void) {
    int arr[6] = {5, 10, 15, 20, 25, 30};
    int *p = arr;
    int *end = arr + 6;
    int sum = 0;

    while (p < end) {
        sum = sum + *p;
        p = p + 1;
    }
    printf("%d\n", sum);
    // EXPECT: 105

    // step backwards
    p = arr + 5;
    printf("%d\n", *p);
    // EXPECT: 30
    p = p - 3;
    printf("%d\n", *p);
    // EXPECT: 15
}

void test_global_pointer_arith(void) {
    int *p = global_arr;
    printf("%d\n", *(p + 4));
    // EXPECT: 50

    int *mid = global_arr + 2;
    printf("%d\n", *mid);
    // EXPECT: 30

    int offset = (int)(mid - global_arr);
    printf("%d\n", offset);
    // EXPECT: 2
}

void test_2d_via_pointers(void) {
    int matrix[3][3];
    int i;
    int j;
    int val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            matrix[i][j] = val;
            val = val + 1;
        }
    }
    // matrix = {1,2,3,4,5,6,7,8,9}
    int *flat = &matrix[0][0];
    printf("%d\n", flat[4]);
    // EXPECT: 5
    printf("%d\n", flat[8]);
    // EXPECT: 9

    // pointer to row
    int *row1 = matrix[1];  // decay of matrix[1]
    printf("%d\n", row1[0]);
    // EXPECT: 4
    printf("%d\n", row1[2]);
    // EXPECT: 6
}

int main(void) {
    test_array_decay();
    test_pointer_to_pointer();
    test_array_of_pointers();
    test_pointer_stepping();
    test_global_pointer_arith();
    test_2d_via_pointers();
    return 0;
}
