int printf(const char *fmt, ...);

struct point {
    int x;
    int y;
};

struct rect {
    struct point top_left;
    struct point bottom_right;
};

struct small {
    int a;
    int b;
};

struct small make_small(int a, int b) {
    struct small s;
    s.a = a;
    s.b = b;
    return s;
}

int dot_product(struct point *p1, struct point *p2) {
    return p1->x * p2->x + p1->y * p2->y;
}

int rect_area(struct rect *r) {
    int width = r->bottom_right.x - r->top_left.x;
    int height = r->bottom_right.y - r->top_left.y;
    return width * height;
}

int main(void) {
    // Basic struct member in expression
    struct point p;
    p.x = 3;
    p.y = 4;
    int sum = p.x + p.y;
    // EXPECT: 7
    printf("%d\n", sum);

    // Struct members in complex expression
    int dist_sq = p.x * p.x + p.y * p.y;
    // 9 + 16 = 25
    // EXPECT: 25
    printf("%d\n", dist_sq);

    // Arrow operator with pointer
    struct point *pp = &p;
    int arrow_sum = pp->x + pp->y;
    // EXPECT: 7
    printf("%d\n", arrow_sum);

    // Arrow in multiplication
    int arrow_prod = pp->x * pp->y;
    // EXPECT: 12
    printf("%d\n", arrow_prod);

    // Nested struct member access
    struct rect r;
    r.top_left.x = 1;
    r.top_left.y = 2;
    r.bottom_right.x = 5;
    r.bottom_right.y = 8;
    int width = r.bottom_right.x - r.top_left.x;
    // EXPECT: 4
    printf("%d\n", width);
    int height = r.bottom_right.y - r.top_left.y;
    // EXPECT: 6
    printf("%d\n", height);
    int area = width * height;
    // EXPECT: 24
    printf("%d\n", area);

    // Arrow on nested struct
    struct rect *rp = &r;
    int rarea = (rp->bottom_right.x - rp->top_left.x) * (rp->bottom_right.y - rp->top_left.y);
    // EXPECT: 24
    printf("%d\n", rarea);

    // Function taking struct pointer
    struct point p1;
    p1.x = 2;
    p1.y = 3;
    struct point p2;
    p2.x = 4;
    p2.y = 5;
    int dp = dot_product(&p1, &p2);
    // 2*4 + 3*5 = 8 + 15 = 23
    // EXPECT: 23
    printf("%d\n", dp);

    // Function returning small struct
    struct small sv = make_small(10, 20);
    // EXPECT: 10
    printf("%d\n", sv.a);
    // EXPECT: 20
    printf("%d\n", sv.b);

    // Struct return value member access
    int smem = make_small(7, 8).a;
    // EXPECT: 7
    printf("%d\n", smem);

    int smem2 = make_small(7, 8).b;
    // EXPECT: 8
    printf("%d\n", smem2);

    // Struct member in ternary
    struct point cond_p;
    cond_p.x = 10;
    cond_p.y = 20;
    int bigger = (cond_p.x > cond_p.y) ? cond_p.x : cond_p.y;
    // EXPECT: 20
    printf("%d\n", bigger);

    // Struct member in loop
    struct point pts[4];
    pts[0].x = 1; pts[0].y = 1;
    pts[1].x = 2; pts[1].y = 4;
    pts[2].x = 3; pts[2].y = 9;
    pts[3].x = 4; pts[3].y = 16;
    int sum_y = 0;
    for (int i = 0; i < 4; i++) {
        sum_y = sum_y + pts[i].y;
    }
    // 1+4+9+16 = 30
    // EXPECT: 30
    printf("%d\n", sum_y);

    // Struct pointer array
    struct point *ptrs[4];
    ptrs[0] = &pts[0];
    ptrs[1] = &pts[1];
    ptrs[2] = &pts[2];
    ptrs[3] = &pts[3];
    int sum_x = 0;
    for (int i = 0; i < 4; i++) {
        sum_x = sum_x + ptrs[i]->x;
    }
    // 1+2+3+4 = 10
    // EXPECT: 10
    printf("%d\n", sum_x);

    // Rect area via function
    int rarea2 = rect_area(&r);
    // EXPECT: 24
    printf("%d\n", rarea2);

    // Compound struct expression: add two struct members
    struct small s1;
    s1.a = 100;
    s1.b = 200;
    struct small s2;
    s2.a = 300;
    s2.b = 400;
    int combined = s1.a + s2.b;
    // EXPECT: 500
    printf("%d\n", combined);

    // Struct member as array index
    struct point idx_pt;
    idx_pt.x = 2;
    int lookup[5];
    lookup[0] = 10;
    lookup[1] = 20;
    lookup[2] = 30;
    lookup[3] = 40;
    lookup[4] = 50;
    // EXPECT: 30
    printf("%d\n", lookup[idx_pt.x]);

    // Struct member comparison
    struct point cmp1;
    cmp1.x = 5;
    cmp1.y = 5;
    int is_equal = (cmp1.x == cmp1.y);
    // EXPECT: 1
    printf("%d\n", is_equal);

    // Modify struct through pointer and read result
    struct point mod;
    mod.x = 1;
    mod.y = 1;
    struct point *modp = &mod;
    modp->x = modp->x + 9;
    modp->y = modp->y + 19;
    // EXPECT: 10
    printf("%d\n", mod.x);
    // EXPECT: 20
    printf("%d\n", mod.y);

    // Struct member in bitwise expression
    struct small bw;
    bw.a = 0xFF;
    bw.b = 0x0F;
    int masked = bw.a & bw.b;
    // EXPECT: 15
    printf("%d\n", masked);

    return 0;
}
