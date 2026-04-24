int printf(const char *fmt, ...);

int main(void) {
    // Array of int pointers
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int e = 50;

    int *ptrs[5];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    ptrs[3] = &d;
    ptrs[4] = &e;

    // Read through pointer array
    // EXPECT: ptrs[0]: 10
    printf("ptrs[0]: %d\n", *ptrs[0]);
    // EXPECT: ptrs[2]: 30
    printf("ptrs[2]: %d\n", *ptrs[2]);
    // EXPECT: ptrs[4]: 50
    printf("ptrs[4]: %d\n", *ptrs[4]);

    // Sum through pointer array
    int sum = 0;
    int i = 0;
    while (i < 5) {
        sum = sum + *ptrs[i];
        i = i + 1;
    }
    // EXPECT: sum: 150
    printf("sum: %d\n", sum);

    // Write through pointer array
    *ptrs[0] = 100;
    *ptrs[4] = 500;
    // EXPECT: a modified: 100
    printf("a modified: %d\n", a);
    // EXPECT: e modified: 500
    printf("e modified: %d\n", e);

    // Sort pointers by pointed-to values (bubble sort)
    int vals[6];
    vals[0] = 50;
    vals[1] = 10;
    vals[2] = 40;
    vals[3] = 20;
    vals[4] = 30;
    vals[5] = 5;

    int *sorted[6];
    i = 0;
    while (i < 6) {
        sorted[i] = &vals[i];
        i = i + 1;
    }

    // Bubble sort the pointer array
    i = 0;
    while (i < 5) {
        int j = 0;
        while (j < 5 - i) {
            if (*sorted[j] > *sorted[j + 1]) {
                int *tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    // Read sorted order through pointers
    // EXPECT: sorted[0]: 5
    printf("sorted[0]: %d\n", *sorted[0]);
    // EXPECT: sorted[1]: 10
    printf("sorted[1]: %d\n", *sorted[1]);
    // EXPECT: sorted[2]: 20
    printf("sorted[2]: %d\n", *sorted[2]);
    // EXPECT: sorted[5]: 50
    printf("sorted[5]: %d\n", *sorted[5]);

    // Verify original array is unchanged
    // EXPECT: vals[0] orig: 50
    printf("vals[0] orig: %d\n", vals[0]);
    // EXPECT: vals[5] orig: 5
    printf("vals[5] orig: %d\n", vals[5]);

    // Array of pointers to array elements
    int data[8];
    data[0] = 2;
    data[1] = 4;
    data[2] = 6;
    data[3] = 8;
    data[4] = 10;
    data[5] = 12;
    data[6] = 14;
    data[7] = 16;

    // Even-indexed pointers
    int *evens[4];
    evens[0] = &data[0];
    evens[1] = &data[2];
    evens[2] = &data[4];
    evens[3] = &data[6];

    int esum = 0;
    i = 0;
    while (i < 4) {
        esum = esum + *evens[i];
        i = i + 1;
    }
    // EXPECT: even sum: 32
    printf("even sum: %d\n", esum);

    // Indirect modification through pointer array
    *evens[0] = 99;
    // EXPECT: data[0] changed: 99
    printf("data[0] changed: %d\n", data[0]);

    // Reverse traversal through pointer array
    int rev[4];
    rev[0] = 7;
    rev[1] = 8;
    rev[2] = 9;
    rev[3] = 10;
    int *rptrs[4];
    rptrs[0] = &rev[0];
    rptrs[1] = &rev[1];
    rptrs[2] = &rev[2];
    rptrs[3] = &rev[3];

    int rsum = 0;
    i = 3;
    while (i >= 0) {
        rsum = rsum + *rptrs[i];
        i = i - 1;
    }
    // EXPECT: reverse sum: 34
    printf("reverse sum: %d\n", rsum);

    // Find max via pointer array
    int maxval = *rptrs[0];
    i = 1;
    while (i < 4) {
        if (*rptrs[i] > maxval) {
            maxval = *rptrs[i];
        }
        i = i + 1;
    }
    // EXPECT: max: 10
    printf("max: %d\n", maxval);

    return 0;
}
