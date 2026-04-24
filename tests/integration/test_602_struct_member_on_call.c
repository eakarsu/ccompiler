int printf(const char *fmt, ...);
// EXPECT: === Test 1: Simple member access on call ===\nmake_point(10,20).x = 10\nmake_point(10,20).y = 20\nmake_point(-3,7).x = -3\nmake_point(-3,7).y = 7\nmake_point(0,0).x = 0\nmake_point(0,0).y = 0\n=== Test 2: Nested struct member access on call ===\nmake_rect().origin.x = 1\nmake_rect().origin.y = 2\nmake_rect().size.x = 3\nmake_rect().size.y = 4\nmake_nested(10,20,30,40).origin.x = 10\nmake_nested(10,20,30,40).size.y = 40\n=== Test 3: Array member access on call ===\nmake_data().arr[0] = 10\nmake_data().arr[1] = 20\nmake_data().arr[2] = 30\nmake_data().arr[3] = 40\nmake_data().len = 4\n=== Test 4: Chained function calls ===\ntransform(make_point(5,3)).x = 10\ntransform(make_point(5,3)).y = 9\ndouble_rect(make_rect()).origin.x = 2\ndouble_rect(make_rect()).size.y = 8\n=== Test 5: Used in expressions ===\nmake_point(3,7).x + make_point(4,6).y = 9\nmake_point(10,20).x * 2 + make_point(1,2).y * 3 = 26\nmake_point(100,200).x - make_point(30,40).y = 60\n=== Test 6: Used in conditions ===\nmake_point(5,0).x > 0: yes\nmake_point(0,5).x > 0: no\nmake_point(-1,10).x < 0: yes\nmake_point(3,3).x == make_point(3,7).x: yes\n=== Test 7: Used as function argument ===\nprintf direct: 42\nprintf direct: 99\nsum_args: 30\n=== Test 8: Multiple accesses of same call ===\nfirst call .x = 100\nsecond call .y = 200\ncall a .x=100, call b .y=200\n=== Test 9: 16-byte struct (register return) ===\nmake_pair(11,22).first = 11\nmake_pair(11,22).second = 22\nmake_pair(0,999).second = 999\nswap_pair(11,22).first = 22\nswap_pair(11,22).second = 11\n=== Test 10: >16-byte struct (x8 pointer return) ===\nmake_big(1,2,3,4,5).a = 1\nmake_big(1,2,3,4,5).b = 2\nmake_big(1,2,3,4,5).c = 3\nmake_big(1,2,3,4,5).d = 4\nmake_big(1,2,3,4,5).e = 5\nmake_big(10,20,30,40,50).c + make_big(1,2,3,4,5).e = 35\n=== Test 11: Mixed scenarios ===\ncond big: 10\nnested big field in expr: 25\nall tests passed

// --- Struct definitions ---

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point origin;
    struct Point size;
};

struct Data {
    int arr[4];
    int len;
};

struct Pair {
    long first;
    long second;
};

struct Big {
    int a;
    int b;
    int c;
    int d;
    int e;
};

// --- Helper functions ---

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

struct Rect make_rect(void) {
    struct Rect r;
    r.origin.x = 1;
    r.origin.y = 2;
    r.size.x = 3;
    r.size.y = 4;
    return r;
}

struct Rect make_nested(int ox, int oy, int sx, int sy) {
    struct Rect r;
    r.origin.x = ox;
    r.origin.y = oy;
    r.size.x = sx;
    r.size.y = sy;
    return r;
}

struct Data make_data(void) {
    struct Data d;
    d.arr[0] = 10;
    d.arr[1] = 20;
    d.arr[2] = 30;
    d.arr[3] = 40;
    d.len = 4;
    return d;
}

struct Point transform(struct Point p) {
    struct Point result;
    result.x = p.x * 2;
    result.y = p.y * 3;
    return result;
}

struct Rect double_rect(struct Rect r) {
    struct Rect result;
    result.origin.x = r.origin.x * 2;
    result.origin.y = r.origin.y * 2;
    result.size.x = r.size.x * 2;
    result.size.y = r.size.y * 2;
    return result;
}

int sum_two(int a, int b) {
    return a + b;
}

struct Point make_hundred(void) {
    struct Point p;
    p.x = 100;
    p.y = 200;
    return p;
}

struct Pair make_pair(long a, long b) {
    struct Pair p;
    p.first = a;
    p.second = b;
    return p;
}

struct Pair swap_pair(long a, long b) {
    struct Pair p;
    p.first = b;
    p.second = a;
    return p;
}

struct Big make_big(int a, int b, int c, int d, int e) {
    struct Big bg;
    bg.a = a;
    bg.b = b;
    bg.c = c;
    bg.d = d;
    bg.e = e;
    return bg;
}

// --- Test functions ---

void test1_simple(void) {
    printf("=== Test 1: Simple member access on call ===\n");
    printf("make_point(10,20).x = %d\n", make_point(10, 20).x);
    printf("make_point(10,20).y = %d\n", make_point(10, 20).y);
    printf("make_point(-3,7).x = %d\n", make_point(-3, 7).x);
    printf("make_point(-3,7).y = %d\n", make_point(-3, 7).y);
    printf("make_point(0,0).x = %d\n", make_point(0, 0).x);
    printf("make_point(0,0).y = %d\n", make_point(0, 0).y);
}

void test2_nested(void) {
    printf("=== Test 2: Nested struct member access on call ===\n");
    printf("make_rect().origin.x = %d\n", make_rect().origin.x);
    printf("make_rect().origin.y = %d\n", make_rect().origin.y);
    printf("make_rect().size.x = %d\n", make_rect().size.x);
    printf("make_rect().size.y = %d\n", make_rect().size.y);
    printf("make_nested(10,20,30,40).origin.x = %d\n", make_nested(10, 20, 30, 40).origin.x);
    printf("make_nested(10,20,30,40).size.y = %d\n", make_nested(10, 20, 30, 40).size.y);
}

void test3_array_member(void) {
    printf("=== Test 3: Array member access on call ===\n");
    printf("make_data().arr[0] = %d\n", make_data().arr[0]);
    printf("make_data().arr[1] = %d\n", make_data().arr[1]);
    printf("make_data().arr[2] = %d\n", make_data().arr[2]);
    printf("make_data().arr[3] = %d\n", make_data().arr[3]);
    printf("make_data().len = %d\n", make_data().len);
}

void test4_chained(void) {
    printf("=== Test 4: Chained function calls ===\n");
    printf("transform(make_point(5,3)).x = %d\n", transform(make_point(5, 3)).x);
    printf("transform(make_point(5,3)).y = %d\n", transform(make_point(5, 3)).y);
    printf("double_rect(make_rect()).origin.x = %d\n", double_rect(make_rect()).origin.x);
    printf("double_rect(make_rect()).size.y = %d\n", double_rect(make_rect()).size.y);
}

void test5_expressions(void) {
    printf("=== Test 5: Used in expressions ===\n");
    int sum = make_point(3, 7).x + make_point(4, 6).y;
    printf("make_point(3,7).x + make_point(4,6).y = %d\n", sum);

    int expr = make_point(10, 20).x * 2 + make_point(1, 2).y * 3;
    printf("make_point(10,20).x * 2 + make_point(1,2).y * 3 = %d\n", expr);

    int diff = make_point(100, 200).x - make_point(30, 40).y;
    printf("make_point(100,200).x - make_point(30,40).y = %d\n", diff);
}

void test6_conditions(void) {
    printf("=== Test 6: Used in conditions ===\n");

    if (make_point(5, 0).x > 0)
        printf("make_point(5,0).x > 0: yes\n");
    else
        printf("make_point(5,0).x > 0: no\n");

    if (make_point(0, 5).x > 0)
        printf("make_point(0,5).x > 0: yes\n");
    else
        printf("make_point(0,5).x > 0: no\n");

    if (make_point(-1, 10).x < 0)
        printf("make_point(-1,10).x < 0: yes\n");
    else
        printf("make_point(-1,10).x < 0: no\n");

    if (make_point(3, 3).x == make_point(3, 7).x)
        printf("make_point(3,3).x == make_point(3,7).x: yes\n");
    else
        printf("make_point(3,3).x == make_point(3,7).x: no\n");
}

void test7_as_argument(void) {
    printf("=== Test 7: Used as function argument ===\n");
    printf("printf direct: %d\n", make_point(42, 99).x);
    printf("printf direct: %d\n", make_point(42, 99).y);
    int s = sum_two(make_point(10, 20).x, make_point(10, 20).y);
    printf("sum_args: %d\n", s);
}

void test8_multiple_accesses(void) {
    printf("=== Test 8: Multiple accesses of same call ===\n");
    int a = make_hundred().x;
    int b = make_hundred().y;
    printf("first call .x = %d\n", a);
    printf("second call .y = %d\n", b);
    printf("call a .x=%d, call b .y=%d\n", make_hundred().x, make_hundred().y);
}

void test9_sixteen_bytes(void) {
    printf("=== Test 9: 16-byte struct (register return) ===\n");
    printf("make_pair(11,22).first = %ld\n", make_pair(11, 22).first);
    printf("make_pair(11,22).second = %ld\n", make_pair(11, 22).second);
    printf("make_pair(0,999).second = %ld\n", make_pair(0, 999).second);
    printf("swap_pair(11,22).first = %ld\n", swap_pair(11, 22).first);
    printf("swap_pair(11,22).second = %ld\n", swap_pair(11, 22).second);
}

void test10_big_struct(void) {
    printf("=== Test 10: >16-byte struct (x8 pointer return) ===\n");
    printf("make_big(1,2,3,4,5).a = %d\n", make_big(1, 2, 3, 4, 5).a);
    printf("make_big(1,2,3,4,5).b = %d\n", make_big(1, 2, 3, 4, 5).b);
    printf("make_big(1,2,3,4,5).c = %d\n", make_big(1, 2, 3, 4, 5).c);
    printf("make_big(1,2,3,4,5).d = %d\n", make_big(1, 2, 3, 4, 5).d);
    printf("make_big(1,2,3,4,5).e = %d\n", make_big(1, 2, 3, 4, 5).e);

    int combined = make_big(10, 20, 30, 40, 50).c + make_big(1, 2, 3, 4, 5).e;
    printf("make_big(10,20,30,40,50).c + make_big(1,2,3,4,5).e = %d\n", combined);
}

void test11_mixed(void) {
    printf("=== Test 11: Mixed scenarios ===\n");

    // Condition on big struct member
    int val;
    if (make_big(10, 20, 30, 40, 50).a > 5)
        val = make_big(10, 20, 30, 40, 50).a;
    else
        val = 0;
    printf("cond big: %d\n", val);

    // Nested big struct field in expression
    int expr = make_big(1, 2, 3, 4, 5).c * make_big(1, 2, 3, 4, 5).e + make_big(10, 20, 30, 40, 50).b / 2;
    printf("nested big field in expr: %d\n", expr);

    printf("all tests passed\n");
}

int main(void) {
    test1_simple();
    test2_nested();
    test3_array_member();
    test4_chained();
    test5_expressions();
    test6_conditions();
    test7_as_argument();
    test8_multiple_accesses();
    test9_sixteen_bytes();
    test10_big_struct();
    test11_mixed();
    return 0;
}
