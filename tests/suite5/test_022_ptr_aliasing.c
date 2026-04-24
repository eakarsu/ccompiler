int printf(const char *fmt, ...);

int main(void) {
    // Two pointers to the same memory
    int x = 42;
    int *p1 = &x;
    int *p2 = &x;

    // EXPECT: p1 reads: 42
    printf("p1 reads: %d\n", *p1);
    // EXPECT: p2 reads: 42
    printf("p2 reads: %d\n", *p2);

    // Modify through p1, read through p2
    *p1 = 100;
    // EXPECT: p2 after p1 write: 100
    printf("p2 after p1 write: %d\n", *p2);

    // Modify through p2, read through p1
    *p2 = 200;
    // EXPECT: p1 after p2 write: 200
    printf("p1 after p2 write: %d\n", *p1);
    // EXPECT: x is: 200
    printf("x is: %d\n", x);

    // Aliasing within an array
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    int *a = &arr[2];
    int *b = &arr[2];

    *a = 99;
    // EXPECT: b sees alias: 99
    printf("b sees alias: %d\n", *b);

    // Adjacent aliasing: a points to arr[1], b points to arr[2]
    a = &arr[1];
    b = &arr[2];
    *a = 111;
    *b = 222;
    // EXPECT: arr[1]: 111
    printf("arr[1]: %d\n", arr[1]);
    // EXPECT: arr[2]: 222
    printf("arr[2]: %d\n", arr[2]);

    // Swap via pointer aliasing
    int s1 = 5;
    int s2 = 10;
    int *ps1 = &s1;
    int *ps2 = &s2;
    int tmp = *ps1;
    *ps1 = *ps2;
    *ps2 = tmp;
    // EXPECT: s1 after swap: 10
    printf("s1 after swap: %d\n", s1);
    // EXPECT: s2 after swap: 5
    printf("s2 after swap: %d\n", s2);

    // Swap array elements via pointers
    int darr[4];
    darr[0] = 1;
    darr[1] = 2;
    darr[2] = 3;
    darr[3] = 4;

    int *dp1 = &darr[0];
    int *dp2 = &darr[3];
    tmp = *dp1;
    *dp1 = *dp2;
    *dp2 = tmp;
    // EXPECT: darr[0] swapped: 4
    printf("darr[0] swapped: %d\n", darr[0]);
    // EXPECT: darr[3] swapped: 1
    printf("darr[3] swapped: %d\n", darr[3]);

    // Chain of aliased writes
    int val = 0;
    int *w1 = &val;
    int *w2 = &val;
    int *w3 = &val;
    *w1 = *w1 + 10;
    *w2 = *w2 + 20;
    *w3 = *w3 + 30;
    // EXPECT: chain writes: 60
    printf("chain writes: %d\n", val);

    // Aliasing through struct member
    struct Pair {
        int a;
        int b;
    };
    struct Pair pair;
    pair.a = 7;
    pair.b = 8;
    int *pa = &pair.a;
    int *pb = &pair.b;
    *pa = *pa + *pb;
    // EXPECT: pair.a aliased: 15
    printf("pair.a aliased: %d\n", pair.a);
    // EXPECT: pair.b unchanged: 8
    printf("pair.b unchanged: %d\n", pair.b);

    // Bubble sort using pointer aliasing
    int sortarr[5];
    sortarr[0] = 5;
    sortarr[1] = 3;
    sortarr[2] = 1;
    sortarr[3] = 4;
    sortarr[4] = 2;

    int i = 0;
    while (i < 4) {
        int j = 0;
        while (j < 4 - i) {
            int *left = &sortarr[j];
            int *right = &sortarr[j + 1];
            if (*left > *right) {
                int t = *left;
                *left = *right;
                *right = t;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    // EXPECT: sorted[0]: 1
    printf("sorted[0]: %d\n", sortarr[0]);
    // EXPECT: sorted[4]: 5
    printf("sorted[4]: %d\n", sortarr[4]);

    // Aliased accumulation
    int acc = 0;
    int *pacc = &acc;
    i = 0;
    while (i < 10) {
        *pacc = *pacc + i;
        i = i + 1;
    }
    // EXPECT: accumulated: 45
    printf("accumulated: %d\n", acc);

    return 0;
}
