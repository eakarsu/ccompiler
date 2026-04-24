int printf(const char *fmt, ...);

// Test: Compound literals for structs, passing compound literal to function

struct Point {
    int x;
    int y;
};

struct RGB {
    int r;
    int g;
    int b;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Pair {
    int first;
    int second;
};

int point_sum(struct Point *p) {
    return p->x + p->y;
}

void print_rgb(struct RGB *c) {
    printf("%d %d %d\n", c->r, c->g, c->b);
}

int rect_area(struct Rect *r) {
    return r->w * r->h;
}

int pair_product(struct Pair *p) {
    return p->first * p->second;
}

struct Point add_points(struct Point *a, struct Point *b) {
    struct Point result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    return result;
}

int main(void) {
    // Basic compound literal assignment
    struct Point p = (struct Point){10, 20};
    // EXPECT: 10 20
    printf("%d %d\n", p.x, p.y);

    // Compound literal with different values
    struct Point p2 = (struct Point){-5, 15};
    // EXPECT: -5 15
    printf("%d %d\n", p2.x, p2.y);

    // RGB compound literal
    struct RGB red = (struct RGB){255, 0, 0};
    // EXPECT: 255 0 0
    printf("%d %d %d\n", red.r, red.g, red.b);

    struct RGB green = (struct RGB){0, 255, 0};
    // EXPECT: 0 255 0
    printf("%d %d %d\n", green.r, green.g, green.b);

    struct RGB blue = (struct RGB){0, 0, 255};
    // EXPECT: 0 0 255
    printf("%d %d %d\n", blue.r, blue.g, blue.b);

    // Rect compound literal
    struct Rect r = (struct Rect){10, 20, 100, 50};
    // EXPECT: 10 20 100 50
    printf("%d %d %d %d\n", r.x, r.y, r.w, r.h);

    // Area of compound-literal-initialized rect
    int area = r.w * r.h;
    // EXPECT: 5000
    printf("%d\n", area);

    // Compound literal in function call (pass address)
    struct Point tmp1 = (struct Point){30, 40};
    int s = point_sum(&tmp1);
    // EXPECT: 70
    printf("%d\n", s);

    // Another function call with compound literal
    struct RGB tmp2 = (struct RGB){128, 64, 32};
    // EXPECT: 128 64 32
    print_rgb(&tmp2);

    // Rect area via function
    struct Rect tmp3 = (struct Rect){0, 0, 25, 12};
    int a2 = rect_area(&tmp3);
    // EXPECT: 300
    printf("%d\n", a2);

    // Pair compound literal
    struct Pair pr = (struct Pair){7, 8};
    int prod = pair_product(&pr);
    // EXPECT: 56
    printf("%d\n", prod);

    // Overwrite with compound literal
    p = (struct Point){111, 222};
    // EXPECT: 111 222
    printf("%d %d\n", p.x, p.y);

    // Compound literal in expression
    struct Point tmp4 = (struct Point){3, 4};
    int dist_sq = tmp4.x * tmp4.x + tmp4.y * tmp4.y;
    // EXPECT: 25
    printf("%d\n", dist_sq);

    // Add two compound-literal points
    struct Point tmpa = (struct Point){10, 20};
    struct Point tmpb = (struct Point){30, 40};
    struct Point sum_pt = add_points(&tmpa, &tmpb);
    // EXPECT: 40 60
    printf("%d %d\n", sum_pt.x, sum_pt.y);

    // Multiple compound literals in sequence
    struct Pair pairs[4];
    pairs[0] = (struct Pair){1, 2};
    pairs[1] = (struct Pair){3, 4};
    pairs[2] = (struct Pair){5, 6};
    pairs[3] = (struct Pair){7, 8};
    int pair_sum = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        pair_sum = pair_sum + pairs[i].first + pairs[i].second;
    }
    // 1+2+3+4+5+6+7+8 = 36
    // EXPECT: 36
    printf("%d\n", pair_sum);

    // Verify individual compound literal array entries
    // EXPECT: 1 2
    printf("%d %d\n", pairs[0].first, pairs[0].second);
    // EXPECT: 7 8
    printf("%d %d\n", pairs[3].first, pairs[3].second);

    return 0;
}
