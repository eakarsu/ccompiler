// stress_065_designated_sparse.c - Sparse designated initializer stress test
// EXPECT: sparse_arr[0]=0
// EXPECT: sparse_arr[10]=100
// EXPECT: sparse_arr[25]=250
// EXPECT: sparse_arr[50]=500
// EXPECT: sparse_arr[75]=750
// EXPECT: sparse_arr[99]=990
// EXPECT: sparse_arr_zeros=95
// EXPECT: sparse_struct_a=0
// EXPECT: sparse_struct_d=42
// EXPECT: sparse_struct_g=99
// EXPECT: sparse_struct_j=777
// EXPECT: sparse_struct_zeros=7
// EXPECT: arr_of_struct[0].x=0
// EXPECT: arr_of_struct[0].y=0
// EXPECT: arr_of_struct[2].x=10
// EXPECT: arr_of_struct[2].y=20
// EXPECT: arr_of_struct[5].x=50
// EXPECT: arr_of_struct[5].y=60
// EXPECT: arr_of_struct[9].x=0
// EXPECT: arr_of_struct[9].y=0
// EXPECT: nested_arr[0][0]=0
// EXPECT: nested_arr[1][2]=99
// EXPECT: nested_arr[3][0]=33
// EXPECT: nested_arr[4][4]=0
// EXPECT: mixed_partial_a=1
// EXPECT: mixed_partial_b=0
// EXPECT: mixed_partial_c=3
// EXPECT: mixed_partial_d=0
// EXPECT: mixed_partial_e=5
// EXPECT: overwrite_val=20
// EXPECT: tail_designated=0 0 0 0 0 0 0 88
// EXPECT: enum_indexed[0]=0
// EXPECT: enum_indexed[1]=10
// EXPECT: enum_indexed[2]=0
// EXPECT: enum_indexed[3]=30
// EXPECT: enum_indexed[4]=0
// EXPECT: struct_arr_mix[0]=100
// EXPECT: struct_arr_mix[1]=0
// EXPECT: struct_arr_mix[2]=200
// EXPECT: struct_in_struct_a=0
// EXPECT: struct_in_struct_inner_x=42
// EXPECT: struct_in_struct_inner_y=0
// EXPECT: struct_in_struct_c=0
// EXPECT: char_arr_sparse=..X..Y..Z.
// EXPECT: zero_gaps_verified=1

int printf(const char *fmt, ...);

// --- Sparse array: 100 elements, only 6 designated ---
void test_sparse_array(void) {
    int arr[100] = {
        [10] = 100,
        [25] = 250,
        [50] = 500,
        [75] = 750,
        [99] = 990
    };

    printf("sparse_arr[0]=%d\n", arr[0]);
    printf("sparse_arr[10]=%d\n", arr[10]);
    printf("sparse_arr[25]=%d\n", arr[25]);
    printf("sparse_arr[50]=%d\n", arr[50]);
    printf("sparse_arr[75]=%d\n", arr[75]);
    printf("sparse_arr[99]=%d\n", arr[99]);

    // Count zeros: 100 elements - 5 designated non-zero = 95 zeros
    int zeros = 0;
    int i;
    for (i = 0; i < 100; i++) {
        if (arr[i] == 0) zeros++;
    }
    printf("sparse_arr_zeros=%d\n", zeros);
}

// --- Struct with 10 fields, only 3 designated ---
struct BigStruct {
    int a, b, c, d, e, f, g, h, i, j;
};

void test_sparse_struct(void) {
    struct BigStruct s = {
        .d = 42,
        .g = 99,
        .j = 777
    };

    printf("sparse_struct_a=%d\n", s.a);
    printf("sparse_struct_d=%d\n", s.d);
    printf("sparse_struct_g=%d\n", s.g);
    printf("sparse_struct_j=%d\n", s.j);

    // Count zero fields
    int zeros = 0;
    if (s.a == 0) zeros++;
    if (s.b == 0) zeros++;
    if (s.c == 0) zeros++;
    if (s.d == 0) zeros++;
    if (s.e == 0) zeros++;
    if (s.f == 0) zeros++;
    if (s.g == 0) zeros++;
    if (s.h == 0) zeros++;
    if (s.i == 0) zeros++;
    if (s.j == 0) zeros++;
    printf("sparse_struct_zeros=%d\n", zeros); // 10 - 3 = 7
}

// --- Array of structs with mixed designation ---
struct Point {
    int x, y;
};

void test_arr_of_struct(void) {
    struct Point pts[10] = {
        [2] = {10, 20},
        [5] = {50, 60}
    };

    printf("arr_of_struct[0].x=%d\n", pts[0].x);
    printf("arr_of_struct[0].y=%d\n", pts[0].y);
    printf("arr_of_struct[2].x=%d\n", pts[2].x);
    printf("arr_of_struct[2].y=%d\n", pts[2].y);
    printf("arr_of_struct[5].x=%d\n", pts[5].x);
    printf("arr_of_struct[5].y=%d\n", pts[5].y);
    printf("arr_of_struct[9].x=%d\n", pts[9].x);
    printf("arr_of_struct[9].y=%d\n", pts[9].y);
}

// --- Nested arrays with designation ---
void test_nested_arrays(void) {
    int arr[5][5] = {
        [1][2] = 99,
        [3][0] = 33
    };

    printf("nested_arr[0][0]=%d\n", arr[0][0]);
    printf("nested_arr[1][2]=%d\n", arr[1][2]);
    printf("nested_arr[3][0]=%d\n", arr[3][0]);
    printf("nested_arr[4][4]=%d\n", arr[4][4]);
}

// --- Mixed positional and designated ---
void test_mixed_partial(void) {
    // Designated inits can mix with positional (not standard-guaranteed order
    // but C11 says after a designator, subsequent inits continue from that position)
    int arr[5] = {
        [0] = 1,
        [2] = 3,
        [4] = 5
    };
    printf("mixed_partial_a=%d\n", arr[0]);
    printf("mixed_partial_b=%d\n", arr[1]);
    printf("mixed_partial_c=%d\n", arr[2]);
    printf("mixed_partial_d=%d\n", arr[3]);
    printf("mixed_partial_e=%d\n", arr[4]);
}

// --- Overwrite: later designator overwrites earlier ---
void test_overwrite(void) {
    int arr[5] = {
        [2] = 10,
        [2] = 20  // overwrites
    };
    printf("overwrite_val=%d\n", arr[2]);
}

// --- Tail designation ---
void test_tail_designated(void) {
    int arr[8] = {
        [7] = 88
    };
    printf("tail_designated=");
    int i;
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// --- Enum as designator index ---
enum Color { RED, GREEN, BLUE, YELLOW, WHITE };

void test_enum_indexed(void) {
    int palette[5] = {
        [GREEN] = 10,
        [YELLOW] = 30
    };
    int i;
    for (i = 0; i < 5; i++) {
        printf("enum_indexed[%d]=%d\n", i, palette[i]);
    }
}

// --- Struct with array member, partially designated ---
struct WithArr {
    int id;
    int vals[3];
};

void test_struct_arr_mix(void) {
    struct WithArr w = {
        .vals = {100, [2] = 200}
    };
    // vals[0] = 100, vals[1] = 0, vals[2] = 200
    printf("struct_arr_mix[0]=%d\n", w.vals[0]);
    printf("struct_arr_mix[1]=%d\n", w.vals[1]);
    printf("struct_arr_mix[2]=%d\n", w.vals[2]);
}

// --- Struct in struct, sparse ---
struct Inner {
    int x, y, z;
};

struct Outer {
    int a;
    struct Inner inner;
    int c;
};

void test_struct_in_struct(void) {
    struct Outer o = {
        .inner.x = 42
    };
    printf("struct_in_struct_a=%d\n", o.a);
    printf("struct_in_struct_inner_x=%d\n", o.inner.x);
    printf("struct_in_struct_inner_y=%d\n", o.inner.y);
    printf("struct_in_struct_c=%d\n", o.c);
}

// --- Char array with sparse designation ---
void test_char_sparse(void) {
    char buf[10] = {
        [2] = 'X',
        [5] = 'Y',
        [8] = 'Z'
    };
    printf("char_arr_sparse=");
    int i;
    for (i = 0; i < 10; i++) {
        if (buf[i] == 0)
            printf(".");
        else
            printf("%c", buf[i]);
    }
    printf("\n");
}

// --- Verify all zero gaps thoroughly ---
void test_zero_gaps(void) {
    int arr[20] = {
        [3] = 30,
        [7] = 70,
        [15] = 150
    };

    int ok = 1;
    int i;
    for (i = 0; i < 20; i++) {
        if (i == 3) {
            if (arr[i] != 30) ok = 0;
        } else if (i == 7) {
            if (arr[i] != 70) ok = 0;
        } else if (i == 15) {
            if (arr[i] != 150) ok = 0;
        } else {
            if (arr[i] != 0) ok = 0;
        }
    }
    printf("zero_gaps_verified=%d\n", ok);
}

int main(void) {
    test_sparse_array();
    test_sparse_struct();
    test_arr_of_struct();
    test_nested_arrays();
    test_mixed_partial();
    test_overwrite();
    test_tail_designated();
    test_enum_indexed();
    test_struct_arr_mix();
    test_struct_in_struct();
    test_char_sparse();
    test_zero_gaps();
    return 0;
}
