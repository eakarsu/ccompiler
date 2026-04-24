int printf(const char *fmt, ...);

struct VTable {
    int (*area)(int w, int h);
    int (*perimeter)(int w, int h);
    int (*diagonal_sq)(int w, int h);
    int (*is_square)(int w, int h);
};

int rect_area(int w, int h) { return w * h; }
int rect_perim(int w, int h) { return 2 * (w + h); }
int rect_diag_sq(int w, int h) { return w * w + h * h; }
int rect_is_square(int w, int h) { return w == h; }

int tri_area(int b, int h) { return (b * h) / 2; }
int tri_perim(int a, int b) { return a + b + a; }
int tri_diag_sq(int a, int b) { return a * a + b * b; }
int tri_is_square(int a, int b) { return 0; }

int para_area(int b, int h) { return b * h; }
int para_perim(int a, int b) { return 2 * (a + b); }
int para_diag_sq(int a, int b) { return a * a + b * b + a * b; }
int para_is_square(int a, int b) { return 0; }

struct Shape {
    struct VTable *vt;
    int w;
    int h;
    int kind;
};

void shape_init(struct Shape *s, struct VTable *vt, int w, int h, int kind) {
    s->vt = vt;
    s->w = w;
    s->h = h;
    s->kind = kind;
}

int shape_area(struct Shape *s) {
    return s->vt->area(s->w, s->h);
}

int shape_perimeter(struct Shape *s) {
    return s->vt->perimeter(s->w, s->h);
}

int shape_diag_sq(struct Shape *s) {
    return s->vt->diagonal_sq(s->w, s->h);
}

int shape_is_square(struct Shape *s) {
    return s->vt->is_square(s->w, s->h);
}

int total_area(struct Shape shapes[], int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + shape_area(&shapes[i]);
    }
    return sum;
}

int total_perimeter(struct Shape shapes[], int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + shape_perimeter(&shapes[i]);
    }
    return sum;
}

int count_squares(struct Shape shapes[], int n) {
    int cnt;
    int i;
    cnt = 0;
    for (i = 0; i < n; i++) {
        if (shape_is_square(&shapes[i])) cnt = cnt + 1;
    }
    return cnt;
}

int largest_area_index(struct Shape shapes[], int n) {
    int best;
    int best_area;
    int a;
    int i;
    best = 0;
    best_area = shape_area(&shapes[0]);
    for (i = 1; i < n; i++) {
        a = shape_area(&shapes[i]);
        if (a > best_area) {
            best_area = a;
            best = i;
        }
    }
    return best;
}

int main(void) {
    struct VTable rect_vt;
    struct VTable tri_vt;
    struct VTable para_vt;
    struct Shape shapes[5];
    int idx;

    rect_vt.area = rect_area;
    rect_vt.perimeter = rect_perim;
    rect_vt.diagonal_sq = rect_diag_sq;
    rect_vt.is_square = rect_is_square;

    tri_vt.area = tri_area;
    tri_vt.perimeter = tri_perim;
    tri_vt.diagonal_sq = tri_diag_sq;
    tri_vt.is_square = tri_is_square;

    para_vt.area = para_area;
    para_vt.perimeter = para_perim;
    para_vt.diagonal_sq = para_diag_sq;
    para_vt.is_square = para_is_square;

    shape_init(&shapes[0], &rect_vt, 5, 3, 0);
    shape_init(&shapes[1], &rect_vt, 4, 4, 0);
    shape_init(&shapes[2], &tri_vt, 6, 4, 1);
    shape_init(&shapes[3], &tri_vt, 3, 8, 1);
    shape_init(&shapes[4], &para_vt, 5, 6, 2);

    // EXPECT: rect 5x3 area: 15
    printf("rect 5x3 area: %d\n", shape_area(&shapes[0]));
    // EXPECT: rect 5x3 perim: 16
    printf("rect 5x3 perim: %d\n", shape_perimeter(&shapes[0]));
    // EXPECT: rect 5x3 diag_sq: 34
    printf("rect 5x3 diag_sq: %d\n", shape_diag_sq(&shapes[0]));

    // EXPECT: rect 4x4 area: 16
    printf("rect 4x4 area: %d\n", shape_area(&shapes[1]));
    // EXPECT: rect 4x4 is_square: 1
    printf("rect 4x4 is_square: %d\n", shape_is_square(&shapes[1]));
    // EXPECT: rect 5x3 is_square: 0
    printf("rect 5x3 is_square: %d\n", shape_is_square(&shapes[0]));

    // EXPECT: tri 6x4 area: 12
    printf("tri 6x4 area: %d\n", shape_area(&shapes[2]));
    // EXPECT: tri 6x4 perim: 16
    printf("tri 6x4 perim: %d\n", shape_perimeter(&shapes[2]));

    // EXPECT: tri 3x8 area: 12
    printf("tri 3x8 area: %d\n", shape_area(&shapes[3]));
    // EXPECT: para 5x6 area: 30
    printf("para 5x6 area: %d\n", shape_area(&shapes[4]));
    // EXPECT: para 5x6 perim: 22
    printf("para 5x6 perim: %d\n", shape_perimeter(&shapes[4]));

    // EXPECT: total area: 85
    printf("total area: %d\n", total_area(shapes, 5));
    // EXPECT: total perim: 84
    printf("total perim: %d\n", total_perimeter(shapes, 5));
    // EXPECT: squares: 1
    printf("squares: %d\n", count_squares(shapes, 5));

    idx = largest_area_index(shapes, 5);
    // EXPECT: largest: 4
    printf("largest: %d\n", idx);

    return 0;
}
