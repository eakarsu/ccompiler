// Compound literal abuse: function args, ternary, arrays, nesting, pointers, loops
int printf(const char *fmt, ...);

struct point {
    int x;
    int y;
};

struct rect {
    struct point tl;
    struct point br;
};

struct with_arr {
    int id;
    int vals[4];
};

int sum_point(struct point p) {
    return p.x + p.y;
}

int sum_rect(struct rect r) {
    return r.tl.x + r.tl.y + r.br.x + r.br.y;
}

void print_point(struct point *p) {
    printf("(%d,%d)\n", p->x, p->y);
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

void test_compound_as_arg(void) {
    int s = sum_point((struct point){10, 20});
    printf("sum=%d\n", s);
    // EXPECT: sum=30
}

void test_compound_in_ternary(void) {
    int cond = 1;
    struct point p = cond ? (struct point){100, 200} : (struct point){-1, -1};
    printf("%d %d\n", p.x, p.y);
    // EXPECT: 100 200

    cond = 0;
    struct point q = cond ? (struct point){100, 200} : (struct point){-1, -1};
    printf("%d %d\n", q.x, q.y);
    // EXPECT: -1 -1
}

void test_compound_nested(void) {
    struct rect r = (struct rect){{1, 2}, {3, 4}};
    int s = sum_rect(r);
    printf("rect sum=%d\n", s);
    // EXPECT: rect sum=10
}

void test_compound_as_nested_arg(void) {
    int s = sum_rect((struct rect){{5, 6}, {7, 8}});
    printf("nested arg sum=%d\n", s);
    // EXPECT: nested arg sum=26
}

void test_compound_in_loop(void) {
    int i;
    int total = 0;
    for (i = 0; i < 5; i++) {
        struct point p = (struct point){i, i * 2};
        total = total + p.x + p.y;
    }
    printf("loop total=%d\n", total);
    // EXPECT: loop total=30
}

void test_compound_struct_with_array(void) {
    struct with_arr w = (struct with_arr){42, {10, 20, 30, 40}};
    printf("id=%d sum=%d\n", w.id, w.vals[0] + w.vals[1] + w.vals[2] + w.vals[3]);
    // EXPECT: id=42 sum=100
}

void test_compound_assign_and_read(void) {
    struct point p;
    p = (struct point){77, 88};
    printf("%d %d\n", p.x, p.y);
    // EXPECT: 77 88
}

void test_compound_multiple_in_expr(void) {
    int a = (struct point){3, 4}.x + (struct point){5, 6}.y;
    printf("expr=%d\n", a);
    // EXPECT: expr=9
}

void test_compound_chain(void) {
    struct point p1 = (struct point){1, 2};
    struct point p2 = (struct point){p1.x + 10, p1.y + 20};
    struct point p3 = (struct point){p2.x + 100, p2.y + 200};
    printf("%d %d\n", p3.x, p3.y);
    // EXPECT: 111 222
}

void test_compound_in_return_expr(void) {
    struct point p = (struct point){999, 1000};
    int v = sum_point(p);
    printf("ret=%d\n", v);
    // EXPECT: ret=1999
}

void test_compound_many_args(void) {
    int s1 = sum_point((struct point){1, 1});
    int s2 = sum_point((struct point){2, 2});
    int s3 = sum_point((struct point){3, 3});
    int s4 = sum_point((struct point){4, 4});
    int s5 = sum_point((struct point){5, 5});
    printf("%d %d %d %d %d\n", s1, s2, s3, s4, s5);
    // EXPECT: 2 4 6 8 10
}

void test_compound_overwrite(void) {
    struct point p = (struct point){1, 2};
    printf("%d %d\n", p.x, p.y);
    // EXPECT: 1 2
    p = (struct point){3, 4};
    printf("%d %d\n", p.x, p.y);
    // EXPECT: 3 4
    p = (struct point){5, 6};
    printf("%d %d\n", p.x, p.y);
    // EXPECT: 5 6
}

void test_compound_deep_nest(void) {
    struct rect r1 = (struct rect){{10, 20}, {30, 40}};
    struct rect r2 = (struct rect){{r1.tl.x + 1, r1.tl.y + 1}, {r1.br.x + 1, r1.br.y + 1}};
    printf("%d %d %d %d\n", r2.tl.x, r2.tl.y, r2.br.x, r2.br.y);
    // EXPECT: 11 21 31 41
}

int main(void) {
    test_compound_as_arg();
    test_compound_in_ternary();
    test_compound_nested();
    test_compound_as_nested_arg();
    test_compound_in_loop();
    test_compound_struct_with_array();
    test_compound_assign_and_read();
    test_compound_multiple_in_expr();
    test_compound_chain();
    test_compound_in_return_expr();
    test_compound_many_args();
    test_compound_overwrite();
    test_compound_deep_nest();
    return 0;
}
