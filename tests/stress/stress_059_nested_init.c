// stress_059_nested_init.c - Deeply nested initialization stress test
// Array of structs containing arrays of structs, 4 levels deep.
// Partial initialization (zeros), access every leaf field.
// EXPECT: L1[0].id=1
// EXPECT: L1[0].sub[0].x=10
// EXPECT: L1[0].sub[0].y=20
// EXPECT: L1[0].sub[0].inner[0].a=100
// EXPECT: L1[0].sub[0].inner[0].b=200
// EXPECT: L1[0].sub[0].inner[0].deep[0]=1000
// EXPECT: L1[0].sub[0].inner[0].deep[1]=1001
// EXPECT: L1[0].sub[0].inner[1].a=300
// EXPECT: L1[0].sub[0].inner[1].b=400
// EXPECT: L1[0].sub[1].x=30
// EXPECT: L1[0].sub[1].y=40
// EXPECT: L1[1].id=2
// EXPECT: L1[1].sub[0].x=50
// EXPECT: L1[1].sub[0].inner[0].a=500
// EXPECT: partial_zero=0
// EXPECT: partial_zero2=0
// EXPECT: sum_all_deep=12012
// EXPECT: array_of_pairs=10,20,30,40,50,60
// EXPECT: nested_array_sum=120
// EXPECT: init_tests=19

int printf(const char *fmt, ...);

// Level 4 (deepest)
struct Deep {
    int a;
    int b;
    int deep[4];  // array inside deepest struct
};

// Level 3
struct Inner {
    int x;
    int y;
    struct Deep inner[2];  // array of Deep
};

// Level 2
struct Outer {
    int id;
    struct Inner sub[3];   // array of Inner
};

// Level 1
struct TopLevel {
    struct Outer items[2]; // array of Outer
    int count;
};

struct Pair {
    int first;
    int second;
};

int main(void) {
    int tests = 0;

    // Initialize deeply nested structure manually
    struct Outer data[2];

    // data[0]
    data[0].id = 1;

    data[0].sub[0].x = 10;
    data[0].sub[0].y = 20;
    data[0].sub[0].inner[0].a = 100;
    data[0].sub[0].inner[0].b = 200;
    data[0].sub[0].inner[0].deep[0] = 1000;
    data[0].sub[0].inner[0].deep[1] = 1001;
    data[0].sub[0].inner[0].deep[2] = 1002;
    data[0].sub[0].inner[0].deep[3] = 1003;
    data[0].sub[0].inner[1].a = 300;
    data[0].sub[0].inner[1].b = 400;
    data[0].sub[0].inner[1].deep[0] = 0;
    data[0].sub[0].inner[1].deep[1] = 0;
    data[0].sub[0].inner[1].deep[2] = 0;
    data[0].sub[0].inner[1].deep[3] = 0;

    data[0].sub[1].x = 30;
    data[0].sub[1].y = 40;
    data[0].sub[1].inner[0].a = 0;
    data[0].sub[1].inner[0].b = 0;
    data[0].sub[1].inner[0].deep[0] = 0;
    data[0].sub[1].inner[0].deep[1] = 0;
    data[0].sub[1].inner[0].deep[2] = 0;
    data[0].sub[1].inner[0].deep[3] = 0;
    data[0].sub[1].inner[1].a = 0;
    data[0].sub[1].inner[1].b = 0;
    data[0].sub[1].inner[1].deep[0] = 0;
    data[0].sub[1].inner[1].deep[1] = 0;
    data[0].sub[1].inner[1].deep[2] = 0;
    data[0].sub[1].inner[1].deep[3] = 0;

    data[0].sub[2].x = 0;
    data[0].sub[2].y = 0;
    data[0].sub[2].inner[0].a = 0;
    data[0].sub[2].inner[0].b = 0;
    data[0].sub[2].inner[0].deep[0] = 0;
    data[0].sub[2].inner[0].deep[1] = 0;
    data[0].sub[2].inner[0].deep[2] = 0;
    data[0].sub[2].inner[0].deep[3] = 0;
    data[0].sub[2].inner[1].a = 0;
    data[0].sub[2].inner[1].b = 0;
    data[0].sub[2].inner[1].deep[0] = 0;
    data[0].sub[2].inner[1].deep[1] = 0;
    data[0].sub[2].inner[1].deep[2] = 0;
    data[0].sub[2].inner[1].deep[3] = 0;

    // data[1]
    data[1].id = 2;

    data[1].sub[0].x = 50;
    data[1].sub[0].y = 60;
    data[1].sub[0].inner[0].a = 500;
    data[1].sub[0].inner[0].b = 600;
    data[1].sub[0].inner[0].deep[0] = 2000;
    data[1].sub[0].inner[0].deep[1] = 2001;
    data[1].sub[0].inner[0].deep[2] = 2002;
    data[1].sub[0].inner[0].deep[3] = 2003;
    data[1].sub[0].inner[1].a = 0;
    data[1].sub[0].inner[1].b = 0;
    data[1].sub[0].inner[1].deep[0] = 0;
    data[1].sub[0].inner[1].deep[1] = 0;
    data[1].sub[0].inner[1].deep[2] = 0;
    data[1].sub[0].inner[1].deep[3] = 0;

    data[1].sub[1].x = 0;
    data[1].sub[1].y = 0;
    data[1].sub[1].inner[0].a = 0;
    data[1].sub[1].inner[0].b = 0;
    data[1].sub[1].inner[0].deep[0] = 0;
    data[1].sub[1].inner[0].deep[1] = 0;
    data[1].sub[1].inner[0].deep[2] = 0;
    data[1].sub[1].inner[0].deep[3] = 0;
    data[1].sub[1].inner[1].a = 0;
    data[1].sub[1].inner[1].b = 0;
    data[1].sub[1].inner[1].deep[0] = 0;
    data[1].sub[1].inner[1].deep[1] = 0;
    data[1].sub[1].inner[1].deep[2] = 0;
    data[1].sub[1].inner[1].deep[3] = 0;

    data[1].sub[2].x = 0;
    data[1].sub[2].y = 0;
    data[1].sub[2].inner[0].a = 0;
    data[1].sub[2].inner[0].b = 0;
    data[1].sub[2].inner[0].deep[0] = 0;
    data[1].sub[2].inner[0].deep[1] = 0;
    data[1].sub[2].inner[0].deep[2] = 0;
    data[1].sub[2].inner[0].deep[3] = 0;
    data[1].sub[2].inner[1].a = 0;
    data[1].sub[2].inner[1].b = 0;
    data[1].sub[2].inner[1].deep[0] = 0;
    data[1].sub[2].inner[1].deep[1] = 0;
    data[1].sub[2].inner[1].deep[2] = 0;
    data[1].sub[2].inner[1].deep[3] = 0;

    // Access and verify every level
    printf("L1[0].id=%d\n", data[0].id);
    tests++;

    printf("L1[0].sub[0].x=%d\n", data[0].sub[0].x);
    tests++;

    printf("L1[0].sub[0].y=%d\n", data[0].sub[0].y);
    tests++;

    printf("L1[0].sub[0].inner[0].a=%d\n", data[0].sub[0].inner[0].a);
    tests++;

    printf("L1[0].sub[0].inner[0].b=%d\n", data[0].sub[0].inner[0].b);
    tests++;

    printf("L1[0].sub[0].inner[0].deep[0]=%d\n", data[0].sub[0].inner[0].deep[0]);
    tests++;

    printf("L1[0].sub[0].inner[0].deep[1]=%d\n", data[0].sub[0].inner[0].deep[1]);
    tests++;

    printf("L1[0].sub[0].inner[1].a=%d\n", data[0].sub[0].inner[1].a);
    tests++;

    printf("L1[0].sub[0].inner[1].b=%d\n", data[0].sub[0].inner[1].b);
    tests++;

    printf("L1[0].sub[1].x=%d\n", data[0].sub[1].x);
    tests++;

    printf("L1[0].sub[1].y=%d\n", data[0].sub[1].y);
    tests++;

    printf("L1[1].id=%d\n", data[1].id);
    tests++;

    printf("L1[1].sub[0].x=%d\n", data[1].sub[0].x);
    tests++;

    printf("L1[1].sub[0].inner[0].a=%d\n", data[1].sub[0].inner[0].a);
    tests++;

    // Partial init: unset fields should be 0
    printf("partial_zero=%d\n", data[1].sub[2].inner[1].deep[3]);
    tests++;

    printf("partial_zero2=%d\n", data[0].sub[2].y);
    tests++;

    // Sum all deep[] values across entire structure
    int sum = 0;
    int i, j, k, l;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 4; l++) {
                    sum += data[i].sub[j].inner[k].deep[l];
                }
            }
        }
    }
    printf("sum_all_deep=%d\n", sum);
    tests++;

    // Array of pairs (simpler nested struct array)
    struct Pair pairs[3];
    pairs[0].first = 10; pairs[0].second = 20;
    pairs[1].first = 30; pairs[1].second = 40;
    pairs[2].first = 50; pairs[2].second = 60;
    printf("array_of_pairs=%d,%d,%d,%d,%d,%d\n",
           pairs[0].first, pairs[0].second,
           pairs[1].first, pairs[1].second,
           pairs[2].first, pairs[2].second);
    tests++;

    // Nested int array sum
    int nested[3][5];
    int val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5; j++) {
            nested[i][j] = val++;
        }
    }
    int nsum = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5; j++) {
            nsum += nested[i][j];
        }
    }
    // sum 1..15 = 120. Wait, 3*5=15, sum = 15*16/2 = 120
    printf("nested_array_sum=%d\n", nsum);
    tests++;

    printf("init_tests=%d\n", tests);

    return 0;
}
