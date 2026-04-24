int printf(const char *fmt, ...);
// EXPECT: === Test 1: Basic compound literal ===\np1: x=10 y=20\n=== Test 2: Compound literal as function argument ===\nprint_point: x=30 y=40\nprint_point: x=100 y=200\n=== Test 3: Compound literal in ternary ===\nternary(1): x=1 y=2\nternary(0): x=3 y=4\n=== Test 4: Address-of compound literal ===\naddr: x=50 y=60\nmodified: x=55 y=65\n=== Test 5: Nested struct compound literal ===\nrect origin: x=1 y=2\nrect size: x=3 y=4\nrect2 origin: x=10 y=20\nrect2 size: x=30 y=40\n=== Test 6: Array compound literal ===\narr[0]=10 arr[1]=20 arr[2]=30\nsum=60\n=== Test 7: Compound literal in loop ===\nloop i=0: x=0 y=1\nloop i=1: x=10 y=11\nloop i=2: x=20 y=21\nloop i=3: x=30 y=31\nloop i=4: x=40 y=41\nloop sum_x=100 sum_y=105\n=== Test 8: Partial initialization ===\ndata values: 1 2 0 0\ndata count: 0\ndata2 values: 99 0 0 0\ndata2 count: 0\n=== Test 9: Multiple compound literals in same expression ===\nsum: x=25 y=70\ndiff: x=15 y=10\n=== Test 10: Compound literal assigned to struct ===\nassigned: x=77 y=88\noverwritten: x=111 y=222\n=== Test 11: Compound literal with struct containing array ===\nfull_data values: 5 10 15 20\nfull_data count: 4\n=== Test 12: Nested compound literal in function arg ===\nprint_rect: origin=(100,200) size=(300,400)\n=== Test 13: Compound literal pointer arithmetic ===\nelem[0]=100 elem[1]=200 elem[2]=300 elem[3]=400 elem[4]=500\n=== Test 14: Compound literal in chained assignment ===\na: x=42 y=84\nb: x=42 y=84\n=== Test 15: Compound literal array of structs ===\npoints[0]: x=1 y=2\npoints[1]: x=3 y=4\npoints[2]: x=5 y=6\nsum_all: x=9 y=12\n=== Test 16: Compound literal in deeply nested ternary ===\ndeep(0): x=10 y=11\ndeep(1): x=20 y=21\ndeep(2): x=30 y=31\ndeep(3): x=30 y=31\n=== Test 17: Compound literal re-assigned in loop ===\nfinal: x=90 y=91\naccum: x=450 y=460\nDone.

// Test 610: Compound literal stress test
// Tests compound literals in various contexts.
// The compiler had a bug where compound literals returned const 0.

struct Point { int x; int y; };
struct Rect { struct Point origin; struct Point size; };
struct Data { int values[4]; int count; };

// --- Helper functions ---

void print_point(struct Point p) {
    printf("print_point: x=%d y=%d\n", p.x, p.y);
}

void print_rect(struct Rect r) {
    printf("print_rect: origin=(%d,%d) size=(%d,%d)\n",
           r.origin.x, r.origin.y, r.size.x, r.size.y);
}

struct Point add_points(struct Point a, struct Point b) {
    struct Point result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

struct Point sub_points(struct Point a, struct Point b) {
    struct Point result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

struct Point make_point(int x, int y) {
    return (struct Point){x, y};
}

struct Rect make_rect(int ox, int oy, int sx, int sy) {
    return (struct Rect){(struct Point){ox, oy}, (struct Point){sx, sy}};
}

// --- Test 1: Basic compound literal ---
void test_basic(void) {
    printf("=== Test 1: Basic compound literal ===\n");
    struct Point p1 = (struct Point){10, 20};
    printf("p1: x=%d y=%d\n", p1.x, p1.y);
}

// --- Test 2: Compound literal as function argument ---
void test_func_arg(void) {
    printf("=== Test 2: Compound literal as function argument ===\n");
    print_point((struct Point){30, 40});
    print_point((struct Point){100, 200});
}

// --- Test 3: Compound literal in ternary ---
void test_ternary(void) {
    printf("=== Test 3: Compound literal in ternary ===\n");

    int cond1 = 1;
    struct Point p1 = cond1 ? (struct Point){1, 2} : (struct Point){3, 4};
    printf("ternary(1): x=%d y=%d\n", p1.x, p1.y);

    int cond2 = 0;
    struct Point p2 = cond2 ? (struct Point){1, 2} : (struct Point){3, 4};
    printf("ternary(0): x=%d y=%d\n", p2.x, p2.y);
}

// --- Test 4: Address-of compound literal ---
void test_address_of(void) {
    printf("=== Test 4: Address-of compound literal ===\n");
    struct Point tmp_pp = (struct Point){50, 60};
    struct Point *pp = &tmp_pp;
    printf("addr: x=%d y=%d\n", pp->x, pp->y);

    // Modify through the pointer
    pp->x = 55;
    pp->y = 65;
    printf("modified: x=%d y=%d\n", pp->x, pp->y);
}

// --- Test 5: Nested struct compound literal ---
void test_nested(void) {
    printf("=== Test 5: Nested struct compound literal ===\n");
    struct Rect r = (struct Rect){(struct Point){1, 2}, (struct Point){3, 4}};
    printf("rect origin: x=%d y=%d\n", r.origin.x, r.origin.y);
    printf("rect size: x=%d y=%d\n", r.size.x, r.size.y);

    struct Rect r2 = (struct Rect){(struct Point){10, 20}, (struct Point){30, 40}};
    printf("rect2 origin: x=%d y=%d\n", r2.origin.x, r2.origin.y);
    printf("rect2 size: x=%d y=%d\n", r2.size.x, r2.size.y);
}

// --- Test 6: Array compound literal ---
void test_array_literal(void) {
    printf("=== Test 6: Array compound literal ===\n");
    int tmp_arr[] = {10, 20, 30};
    int *p = tmp_arr;
    printf("arr[0]=%d arr[1]=%d arr[2]=%d\n", p[0], p[1], p[2]);

    int sum = p[0] + p[1] + p[2];
    printf("sum=%d\n", sum);
}

// --- Test 7: Compound literal in loop ---
void test_loop(void) {
    printf("=== Test 7: Compound literal in loop ===\n");
    int sum_x = 0;
    int sum_y = 0;
    int i;
    for (i = 0; i < 5; i++) {
        struct Point p = (struct Point){i * 10, i * 10 + 1};
        printf("loop i=%d: x=%d y=%d\n", i, p.x, p.y);
        sum_x = sum_x + p.x;
        sum_y = sum_y + p.y;
    }
    printf("loop sum_x=%d sum_y=%d\n", sum_x, sum_y);
}

// --- Test 8: Partial initialization ---
void test_partial_init(void) {
    printf("=== Test 8: Partial initialization ===\n");

    // Only first two values initialized, rest should be 0
    struct Data d = (struct Data){{1, 2}};
    printf("data values: %d %d %d %d\n",
           d.values[0], d.values[1], d.values[2], d.values[3]);
    printf("data count: %d\n", d.count);

    // Only first value initialized
    struct Data d2 = (struct Data){{99}};
    printf("data2 values: %d %d %d %d\n",
           d2.values[0], d2.values[1], d2.values[2], d2.values[3]);
    printf("data2 count: %d\n", d2.count);
}

// --- Test 9: Multiple compound literals in same expression ---
void test_multi_expr(void) {
    printf("=== Test 9: Multiple compound literals in same expression ===\n");

    struct Point sum = add_points(
        (struct Point){20, 30},
        (struct Point){5, 40}
    );
    printf("sum: x=%d y=%d\n", sum.x, sum.y);

    struct Point diff = sub_points(
        (struct Point){20, 30},
        (struct Point){5, 20}
    );
    printf("diff: x=%d y=%d\n", diff.x, diff.y);
}

// --- Test 10: Compound literal assigned to struct ---
void test_assign(void) {
    printf("=== Test 10: Compound literal assigned to struct ===\n");
    struct Point p;
    p = (struct Point){77, 88};
    printf("assigned: x=%d y=%d\n", p.x, p.y);

    p = (struct Point){111, 222};
    printf("overwritten: x=%d y=%d\n", p.x, p.y);
}

// --- Test 11: Compound literal with struct containing array ---
void test_struct_array(void) {
    printf("=== Test 11: Compound literal with struct containing array ===\n");

    struct Data d = (struct Data){{5, 10, 15, 20}, 4};
    printf("full_data values: %d %d %d %d\n",
           d.values[0], d.values[1], d.values[2], d.values[3]);
    printf("full_data count: %d\n", d.count);
}

// --- Test 12: Nested compound literal in function arg ---
void test_nested_func_arg(void) {
    printf("=== Test 12: Nested compound literal in function arg ===\n");
    print_rect((struct Rect){(struct Point){100, 200}, (struct Point){300, 400}});
}

// --- Test 13: Compound literal pointer arithmetic ---
void test_ptr_arith(void) {
    printf("=== Test 13: Compound literal pointer arithmetic ===\n");
    int tmp_arr2[] = {100, 200, 300, 400, 500};
    int *arr = tmp_arr2;
    int i;
    for (i = 0; i < 5; i++) {
        printf("elem[%d]=%d", i, *(arr + i));
        if (i < 4) {
            printf(" ");
        }
    }
    printf("\n");
}

// --- Test 14: Compound literal in chained assignment ---
void test_chain_assign(void) {
    printf("=== Test 14: Compound literal in chained assignment ===\n");
    struct Point a;
    struct Point b;
    a = (struct Point){42, 84};
    b = a;
    printf("a: x=%d y=%d\n", a.x, a.y);
    printf("b: x=%d y=%d\n", b.x, b.y);
}

// --- Test 15: Compound literal array of structs ---
void test_struct_array_literal(void) {
    printf("=== Test 15: Compound literal array of structs ===\n");

    struct Point points[3];
    points[0] = (struct Point){1, 2};
    points[1] = (struct Point){3, 4};
    points[2] = (struct Point){5, 6};

    int sum_x = 0;
    int sum_y = 0;
    int i;
    for (i = 0; i < 3; i++) {
        printf("points[%d]: x=%d y=%d\n", i, points[i].x, points[i].y);
        sum_x = sum_x + points[i].x;
        sum_y = sum_y + points[i].y;
    }
    printf("sum_all: x=%d y=%d\n", sum_x, sum_y);
}

// --- Test 16: Compound literal in deeply nested ternary ---
void test_deep_ternary(void) {
    printf("=== Test 16: Compound literal in deeply nested ternary ===\n");

    int i;
    for (i = 0; i < 4; i++) {
        struct Point p = (i == 0) ? (struct Point){10, 11}
                       : (i == 1) ? (struct Point){20, 21}
                       :            (struct Point){30, 31};
        printf("deep(%d): x=%d y=%d\n", i, p.x, p.y);
    }
}

// --- Test 17: Compound literal re-assigned in loop ---
void test_reassign_loop(void) {
    printf("=== Test 17: Compound literal re-assigned in loop ===\n");
    struct Point p;
    int accum_x = 0;
    int accum_y = 0;
    int i;
    for (i = 0; i < 10; i++) {
        p = (struct Point){i * 10, i * 10 + 1};
        accum_x = accum_x + p.x;
        accum_y = accum_y + p.y;
    }
    printf("final: x=%d y=%d\n", p.x, p.y);
    printf("accum: x=%d y=%d\n", accum_x, accum_y);
}

int main(void) {
    test_basic();
    test_func_arg();
    test_ternary();
    test_address_of();
    test_nested();
    test_array_literal();
    test_loop();
    test_partial_init();
    test_multi_expr();
    test_assign();
    test_struct_array();
    test_nested_func_arg();
    test_ptr_arith();
    test_chain_assign();
    test_struct_array_literal();
    test_deep_ternary();
    test_reassign_loop();

    printf("Done.\n");
    return 0;
}
