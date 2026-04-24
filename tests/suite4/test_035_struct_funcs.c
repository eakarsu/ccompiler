int printf(const char *fmt, ...);

// Test: Passing struct to function (by value and by pointer), returning struct from function

struct Point {
    int x;
    int y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Result {
    int value;
    int error;
};

// Pass struct by pointer and read
int point_sum(struct Point *p) {
    return p->x + p->y;
}

// Modify struct through pointer
void point_scale(struct Point *p, int factor) {
    p->x = p->x * factor;
    p->y = p->y * factor;
}

// Return struct from function
struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

// Return struct with computation
struct Result compute(int a, int b) {
    struct Result r;
    if (b == 0) {
        r.value = 0;
        r.error = 1;
    } else {
        r.value = a / b;
        r.error = 0;
    }
    return r;
}

// Vec3 operations through pointer
int vec3_dot(struct Vec3 *a, struct Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_add(struct Vec3 *result, struct Vec3 *a, struct Vec3 *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

int vec3_length_sq(struct Vec3 *v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

// Return struct based on condition
struct Point choose_point(int flag, struct Point *a, struct Point *b) {
    struct Point result;
    if (flag) {
        result.x = a->x;
        result.y = a->y;
    } else {
        result.x = b->x;
        result.y = b->y;
    }
    return result;
}

int main(void) {
    // Pass struct by pointer
    struct Point p;
    p.x = 3;
    p.y = 7;
    int s = point_sum(&p);
    // EXPECT: 10
    printf("%d\n", s);

    // Modify through pointer
    point_scale(&p, 4);
    // EXPECT: 12 28
    printf("%d %d\n", p.x, p.y);

    // Return struct from function
    struct Point p2 = make_point(100, 200);
    // EXPECT: 100 200
    printf("%d %d\n", p2.x, p2.y);

    // Return struct with logic
    struct Result r1 = compute(20, 4);
    // EXPECT: 5 0
    printf("%d %d\n", r1.value, r1.error);

    // Division by zero case
    struct Result r2 = compute(10, 0);
    // EXPECT: 0 1
    printf("%d %d\n", r2.value, r2.error);

    // Vec3 dot product
    struct Vec3 v1;
    v1.x = 1; v1.y = 2; v1.z = 3;
    struct Vec3 v2;
    v2.x = 4; v2.y = 5; v2.z = 6;
    int dot = vec3_dot(&v1, &v2);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    // EXPECT: 32
    printf("%d\n", dot);

    // Vec3 addition
    struct Vec3 v3;
    vec3_add(&v3, &v1, &v2);
    // EXPECT: 5 7 9
    printf("%d %d %d\n", v3.x, v3.y, v3.z);

    // Vec3 length squared
    int len_sq = vec3_length_sq(&v1);
    // 1 + 4 + 9 = 14
    // EXPECT: 14
    printf("%d\n", len_sq);

    // Choose point based on flag
    struct Point a;
    a.x = 11; a.y = 22;
    struct Point b;
    b.x = 33; b.y = 44;
    struct Point chosen = choose_point(1, &a, &b);
    // EXPECT: 11 22
    printf("%d %d\n", chosen.x, chosen.y);

    struct Point chosen2 = choose_point(0, &a, &b);
    // EXPECT: 33 44
    printf("%d %d\n", chosen2.x, chosen2.y);

    // Chain: make_point then sum
    struct Point p3 = make_point(50, 75);
    int s2 = point_sum(&p3);
    // EXPECT: 125
    printf("%d\n", s2);

    // Multiple calls returning struct
    struct Point pts[3];
    int i;
    for (i = 0; i < 3; i = i + 1) {
        pts[i] = make_point(i * 10, i * 20);
    }
    // EXPECT: 0 0
    printf("%d %d\n", pts[0].x, pts[0].y);
    // EXPECT: 10 20
    printf("%d %d\n", pts[1].x, pts[1].y);
    // EXPECT: 20 40
    printf("%d %d\n", pts[2].x, pts[2].y);

    // Use returned struct immediately with pointer
    struct Point p4 = make_point(7, 8);
    point_scale(&p4, 3);
    // EXPECT: 21 24
    printf("%d %d\n", p4.x, p4.y);

    return 0;
}
