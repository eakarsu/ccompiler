int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point a;
    Point b;
    Point c;
} Triangle;

int triangle_area2(Triangle t) {
    int area = (t.b.x - t.a.x) * (t.c.y - t.a.y) -
               (t.c.x - t.a.x) * (t.b.y - t.a.y);
    if (area < 0) area = -area;
    return area;
}

int dist_sq(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}

int is_degenerate(Triangle t) {
    return triangle_area2(t) == 0;
}

void classify_triangle(Triangle t, int *type_angle, int *type_side) {
    int ab = dist_sq(t.a, t.b);
    int bc = dist_sq(t.b, t.c);
    int ca = dist_sq(t.c, t.a);

    int sides[3];
    sides[0] = ab;
    sides[1] = bc;
    sides[2] = ca;

    int i;
    int j;
    for (i = 0; i < 2; i++) {
        for (j = i + 1; j < 3; j++) {
            if (sides[j] > sides[i]) {
                int tmp = sides[i];
                sides[i] = sides[j];
                sides[j] = tmp;
            }
        }
    }

    if (sides[0] > sides[1] + sides[2]) {
        *type_angle = 2;
    } else if (sides[0] == sides[1] + sides[2]) {
        *type_angle = 1;
    } else {
        *type_angle = 0;
    }

    if (ab == bc && bc == ca) {
        *type_side = 2;
    } else if (ab == bc || bc == ca || ca == ab) {
        *type_side = 1;
    } else {
        *type_side = 0;
    }
}

int is_right_triangle(Triangle t) {
    int ta;
    int ts;
    classify_triangle(t, &ta, &ts);
    return ta == 1;
}

int point_in_triangle(Point p, Triangle t) {
    int d1 = (p.x - t.b.x) * (t.a.y - t.b.y) - (t.a.x - t.b.x) * (p.y - t.b.y);
    int d2 = (p.x - t.c.x) * (t.b.y - t.c.y) - (t.b.x - t.c.x) * (p.y - t.c.y);
    int d3 = (p.x - t.a.x) * (t.c.y - t.a.y) - (t.c.x - t.a.x) * (p.y - t.a.y);

    int has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

Point make_pt(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Triangle make_tri(int x1, int y1, int x2, int y2, int x3, int y3) {
    Triangle t;
    t.a.x = x1; t.a.y = y1;
    t.b.x = x2; t.b.y = y2;
    t.c.x = x3; t.c.y = y3;
    return t;
}

int main(void) {
    Triangle t1 = make_tri(0, 0, 10, 0, 0, 10);
    int a1 = triangle_area2(t1);
    printf("%d\n", a1);
    // EXPECT: 100

    int r1 = is_right_triangle(t1);
    printf("%d\n", r1);
    // EXPECT: 1

    int ta;
    int ts;
    classify_triangle(t1, &ta, &ts);
    printf("%d %d\n", ta, ts);
    // EXPECT: 1 1

    Triangle t2 = make_tri(0, 0, 4, 0, 2, 4);
    classify_triangle(t2, &ta, &ts);
    printf("%d %d\n", ta, ts);
    // EXPECT: 0 1

    Triangle t3 = make_tri(0, 0, 10, 0, 5, 1);
    classify_triangle(t3, &ta, &ts);
    printf("%d %d\n", ta, ts);
    // EXPECT: 2 1

    Triangle t4 = make_tri(0, 0, 3, 0, 6, 0);
    int dg = is_degenerate(t4);
    printf("%d\n", dg);
    // EXPECT: 1

    int dg2 = is_degenerate(t1);
    printf("%d\n", dg2);
    // EXPECT: 0

    Triangle t5 = make_tri(0, 0, 6, 0, 3, 4);
    int a5 = triangle_area2(t5);
    printf("%d\n", a5);
    // EXPECT: 24

    Triangle t345 = make_tri(0, 0, 3, 0, 0, 4);
    int rt = is_right_triangle(t345);
    printf("%d\n", rt);
    // EXPECT: 1

    int a345 = triangle_area2(t345);
    printf("%d\n", a345);
    // EXPECT: 12

    Point inside_pt = make_pt(3, 3);
    int in1 = point_in_triangle(inside_pt, t1);
    printf("%d\n", in1);
    // EXPECT: 1

    Point outside_pt = make_pt(8, 8);
    int in2 = point_in_triangle(outside_pt, t1);
    printf("%d\n", in2);
    // EXPECT: 0

    Point edge_pt = make_pt(5, 0);
    int in3 = point_in_triangle(edge_pt, t1);
    printf("%d\n", in3);
    // EXPECT: 1

    Point vertex_pt = make_pt(0, 0);
    int in4 = point_in_triangle(vertex_pt, t1);
    printf("%d\n", in4);
    // EXPECT: 1

    Triangle equi = make_tri(0, 0, 10, 0, 5, 9);
    classify_triangle(equi, &ta, &ts);
    printf("%d\n", ts);
    // EXPECT: 1

    Triangle big_right = make_tri(0, 0, 5, 0, 0, 12);
    classify_triangle(big_right, &ta, &ts);
    printf("%d\n", ta);
    // EXPECT: 1

    int a_big = triangle_area2(big_right);
    printf("%d\n", a_big);
    // EXPECT: 60

    Triangle scalene = make_tri(0, 0, 7, 0, 3, 5);
    classify_triangle(scalene, &ta, &ts);
    printf("%d %d\n", ta, ts);
    // EXPECT: 0 0

    int a_sc = triangle_area2(scalene);
    printf("%d\n", a_sc);
    // EXPECT: 35

    printf("done\n");
    // EXPECT: done

    return 0;
}
