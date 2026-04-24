// EXPECT: struct_cl: x=10 y=20
// EXPECT: pass_struct: x=3 y=4
// EXPECT: modified: x=99 y=4
// EXPECT: sum_pts=12
// EXPECT: arr_cl: 10 20 30
// EXPECT: arr_sum=60
// EXPECT: ret_field=42
// EXPECT: multi_pt: 100 200
// EXPECT: ptr_val=77
// EXPECT: nested_cl: x=1 y=2 v=9
// EXPECT: reassign: x=50 y=60
// EXPECT: ternary_cl: x=1 y=0
// EXPECT: loop_sum=100
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Pair {
    int a;
    int b;
};

struct Nested {
    struct Point p;
    int val;
};

int point_sum(struct Point p) {
    return p.x + p.y;
}

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

int pair_product(struct Pair pr) {
    return pr.a * pr.b;
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

int get_x(struct Point *p) {
    return p->x;
}

int main(void) {
    int passed = 1;

    // Basic struct compound literal
    struct Point p1 = (struct Point){10, 20};
    printf("struct_cl: x=%d y=%d\n", p1.x, p1.y);
    if (p1.x != 10 || p1.y != 20) passed = 0;

    // Compound literal as function argument
    int s1 = point_sum((struct Point){3, 4});
    printf("pass_struct: x=3 y=4\n");
    if (s1 != 7) passed = 0;

    // Compound literal is modifiable (it's an lvalue in C11)
    struct Point *pp = &(struct Point){55, 4};
    pp->x = 99;
    printf("modified: x=%d y=%d\n", pp->x, pp->y);
    if (pp->x != 99 || pp->y != 4) passed = 0;

    // Multiple compound literals in one expression
    int sum_pts = point_sum((struct Point){5, 7});
    printf("sum_pts=%d\n", sum_pts);
    if (sum_pts != 12) passed = 0;

    // Array compound literal
    int *arr = (int[]){10, 20, 30};
    printf("arr_cl: %d %d %d\n", arr[0], arr[1], arr[2]);
    if (arr[0] != 10 || arr[1] != 20 || arr[2] != 30) passed = 0;

    // Array compound literal passed to function
    int asum = sum_array((int[]){10, 20, 30}, 3);
    printf("arr_sum=%d\n", asum);
    if (asum != 60) passed = 0;

    // Access field from compound literal directly
    int rf = ((struct Point){42, 88}).x;
    printf("ret_field=%d\n", rf);
    if (rf != 42) passed = 0;

    // Multiple struct compound literals
    struct Point mp = (struct Point){100, 200};
    printf("multi_pt: %d %d\n", mp.x, mp.y);
    if (mp.x != 100 || mp.y != 200) passed = 0;

    // Pointer to compound literal
    int *pv = &(int){77};
    printf("ptr_val=%d\n", *pv);
    if (*pv != 77) passed = 0;

    // Nested struct compound literal
    struct Nested ns = (struct Nested){{1, 2}, 9};
    printf("nested_cl: x=%d y=%d v=%d\n", ns.p.x, ns.p.y, ns.val);
    if (ns.p.x != 1 || ns.p.y != 2 || ns.val != 9) passed = 0;

    // Reassign with compound literal
    struct Point reassigned;
    reassigned = (struct Point){50, 60};
    printf("reassign: x=%d y=%d\n", reassigned.x, reassigned.y);
    if (reassigned.x != 50 || reassigned.y != 60) passed = 0;

    // Compound literal in ternary
    int cond = 1;
    struct Point tcp = cond ? (struct Point){1, 0} : (struct Point){0, 1};
    printf("ternary_cl: x=%d y=%d\n", tcp.x, tcp.y);
    if (tcp.x != 1 || tcp.y != 0) passed = 0;

    // Compound literals in loop
    int loop_sum = 0;
    int i;
    for (i = 0; i < 5; i++) {
        struct Point lp = (struct Point){i * 10, 0};
        loop_sum = loop_sum + lp.x;
    }
    // 0 + 10 + 20 + 30 + 40 = 100
    printf("loop_sum=%d\n", loop_sum);
    if (loop_sum != 100) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
