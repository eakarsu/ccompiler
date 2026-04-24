// EXPECT: === Basic Struct ===
// EXPECT: x=10 y=20
// EXPECT: x=3 y=4
// EXPECT: === Nested Struct ===
// EXPECT: x=1 y=2 z=100
// EXPECT: === Struct Assignment ===
// EXPECT: copy: x=10 y=20
// EXPECT: === Struct Array ===
// EXPECT: points[0]: x=0 y=0
// EXPECT: points[1]: x=1 y=10
// EXPECT: points[2]: x=2 y=20
// EXPECT: === Struct Return ===
// EXPECT: returned: x=42 y=84
// EXPECT: === Struct Modify ===
// EXPECT: after modify: a=99 b=200
// EXPECT: === Struct Compare Fields ===
// EXPECT: p1 and p2 have same x
// EXPECT: === Large Struct ===
// EXPECT: large: a=1 b=2 c=3 d=4 e=5 f=6

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Point3D {
    struct Point base;
    int z;
};

struct Rect {
    struct Point topleft;
    struct Point botright;
};

struct Large {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

void modify_point(struct Point *p, int x, int y) {
    p->x = x;
    p->y = y;
}

void print_point(struct Point p) {
    printf("x=%d y=%d\n", p.x, p.y);
}

void test_basic(void) {
    printf("=== Basic Struct ===\n");
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    print_point(p1);

    struct Point p2;
    p2.x = 3;
    p2.y = 4;
    print_point(p2);
}

void test_nested(void) {
    printf("=== Nested Struct ===\n");
    struct Point3D p3;
    p3.base.x = 1;
    p3.base.y = 2;
    p3.z = 100;
    printf("x=%d y=%d z=%d\n", p3.base.x, p3.base.y, p3.z);
}

void test_assignment(void) {
    printf("=== Struct Assignment ===\n");
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    struct Point p2;
    p2 = p1;
    printf("copy: x=%d y=%d\n", p2.x, p2.y);
}

void test_array(void) {
    printf("=== Struct Array ===\n");
    struct Point points[3];
    int i;
    for (i = 0; i < 3; i++) {
        points[i].x = i;
        points[i].y = i * 10;
    }
    for (i = 0; i < 3; i++) {
        printf("points[%d]: x=%d y=%d\n", i, points[i].x, points[i].y);
    }
}

void test_return(void) {
    printf("=== Struct Return ===\n");
    struct Point p = make_point(42, 84);
    printf("returned: x=%d y=%d\n", p.x, p.y);
}

void test_modify(void) {
    printf("=== Struct Modify ===\n");
    struct Point p;
    p.x = 10;
    p.y = 20;
    modify_point(&p, 99, 200);
    printf("after modify: a=%d b=%d\n", p.x, p.y);
}

void test_compare_fields(void) {
    printf("=== Struct Compare Fields ===\n");
    struct Point p1;
    p1.x = 5;
    p1.y = 10;
    struct Point p2;
    p2.x = 5;
    p2.y = 99;
    if (p1.x == p2.x) {
        printf("p1 and p2 have same x\n");
    } else {
        printf("p1 and p2 differ in x\n");
    }
}

void test_large_struct(void) {
    printf("=== Large Struct ===\n");
    struct Large lg;
    lg.a = 1;
    lg.b = 2;
    lg.c = 3;
    lg.d = 4;
    lg.e = 5;
    lg.f = 6;
    printf("large: a=%d b=%d c=%d d=%d e=%d f=%d\n",
           lg.a, lg.b, lg.c, lg.d, lg.e, lg.f);
}

int main(void) {
    test_basic();
    test_nested();
    test_assignment();
    test_array();
    test_return();
    test_modify();
    test_compare_fields();
    test_large_struct();
    return 0;
}
