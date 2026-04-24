int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Vec2;

Vec2 make_vec(int x, int y) {
    Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

Vec2 vec_add(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

Vec2 vec_sub(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

Vec2 vec_scale(Vec2 v, int s) {
    Vec2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

int vec_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

int vec_cross(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

int vec_length_sq(Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

Vec2 vec_rotate90_ccw(Vec2 v) {
    Vec2 r;
    r.x = -v.y;
    r.y = v.x;
    return r;
}

Vec2 vec_rotate90_cw(Vec2 v) {
    Vec2 r;
    r.x = v.y;
    r.y = -v.x;
    return r;
}

Vec2 vec_rotate180(Vec2 v) {
    Vec2 r;
    r.x = -v.x;
    r.y = -v.y;
    return r;
}

Vec2 vec_negate(Vec2 v) {
    Vec2 r;
    r.x = -v.x;
    r.y = -v.y;
    return r;
}

int vec_is_perpendicular(Vec2 a, Vec2 b) {
    return vec_dot(a, b) == 0;
}

int vec_is_parallel(Vec2 a, Vec2 b) {
    return vec_cross(a, b) == 0;
}

Vec2 vec_project_scalar_num_den(Vec2 a, Vec2 b, int *num, int *den) {
    *num = vec_dot(a, b);
    *den = vec_dot(b, b);
    Vec2 dummy;
    dummy.x = 0;
    dummy.y = 0;
    return dummy;
}

int vec_angle_sign(Vec2 a, Vec2 b) {
    int c = vec_cross(a, b);
    if (c > 0) return 1;
    if (c < 0) return -1;
    return 0;
}

int main(void) {
    Vec2 v1 = make_vec(3, 4);
    Vec2 v2 = make_vec(1, 2);

    Vec2 sum = vec_add(v1, v2);
    printf("%d %d\n", sum.x, sum.y);
    // EXPECT: 4 6

    Vec2 diff = vec_sub(v1, v2);
    printf("%d %d\n", diff.x, diff.y);
    // EXPECT: 2 2

    Vec2 scaled = vec_scale(v1, 3);
    printf("%d %d\n", scaled.x, scaled.y);
    // EXPECT: 9 12

    int dot1 = vec_dot(v1, v2);
    printf("%d\n", dot1);
    // EXPECT: 11

    int cross1 = vec_cross(v1, v2);
    printf("%d\n", cross1);
    // EXPECT: 2

    int len1 = vec_length_sq(v1);
    printf("%d\n", len1);
    // EXPECT: 25

    Vec2 rot1 = vec_rotate90_ccw(v1);
    printf("%d %d\n", rot1.x, rot1.y);
    // EXPECT: -4 3

    Vec2 rot2 = vec_rotate90_cw(v1);
    printf("%d %d\n", rot2.x, rot2.y);
    // EXPECT: 4 -3

    Vec2 rot3 = vec_rotate180(v1);
    printf("%d %d\n", rot3.x, rot3.y);
    // EXPECT: -3 -4

    int perp1 = vec_is_perpendicular(v1, rot1);
    printf("%d\n", perp1);
    // EXPECT: 1

    int perp2 = vec_is_perpendicular(v1, v2);
    printf("%d\n", perp2);
    // EXPECT: 0

    Vec2 vx = make_vec(5, 0);
    Vec2 vy = make_vec(0, 7);
    int perp3 = vec_is_perpendicular(vx, vy);
    printf("%d\n", perp3);
    // EXPECT: 1

    int par1 = vec_is_parallel(v1, vec_scale(v1, 2));
    printf("%d\n", par1);
    // EXPECT: 1

    int par2 = vec_is_parallel(v1, v2);
    printf("%d\n", par2);
    // EXPECT: 0

    int par3 = vec_is_parallel(v1, vec_negate(v1));
    printf("%d\n", par3);
    // EXPECT: 1

    Vec2 chain = v1;
    chain = vec_rotate90_ccw(chain);
    chain = vec_rotate90_ccw(chain);
    chain = vec_rotate90_ccw(chain);
    chain = vec_rotate90_ccw(chain);
    printf("%d %d\n", chain.x, chain.y);
    // EXPECT: 3 4

    int as1 = vec_angle_sign(make_vec(1, 0), make_vec(0, 1));
    printf("%d\n", as1);
    // EXPECT: 1

    int as2 = vec_angle_sign(make_vec(0, 1), make_vec(1, 0));
    printf("%d\n", as2);
    // EXPECT: -1

    int as3 = vec_angle_sign(make_vec(1, 0), make_vec(2, 0));
    printf("%d\n", as3);
    // EXPECT: 0

    int num;
    int den;
    vec_project_scalar_num_den(make_vec(3, 4), make_vec(1, 0), &num, &den);
    printf("%d %d\n", num, den);
    // EXPECT: 3 1

    vec_project_scalar_num_den(make_vec(3, 4), make_vec(0, 1), &num, &den);
    printf("%d %d\n", num, den);
    // EXPECT: 4 1

    vec_project_scalar_num_den(make_vec(5, 5), make_vec(1, 1), &num, &den);
    printf("%d %d\n", num, den);
    // EXPECT: 10 2

    Vec2 vecs[4];
    vecs[0] = make_vec(1, 0);
    vecs[1] = make_vec(0, 1);
    vecs[2] = make_vec(-1, 0);
    vecs[3] = make_vec(0, -1);

    int total_cross = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        total_cross = total_cross + vec_cross(vecs[i], vecs[j]);
    }
    printf("%d\n", total_cross);
    // EXPECT: 4

    int total_dot = 0;
    for (i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        total_dot = total_dot + vec_dot(vecs[i], vecs[j]);
    }
    printf("%d\n", total_dot);
    // EXPECT: 0

    Vec2 diag = vec_add(make_vec(3, 0), make_vec(0, 4));
    printf("%d\n", vec_length_sq(diag));
    // EXPECT: 25

    printf("done\n");
    // EXPECT: done

    return 0;
}
