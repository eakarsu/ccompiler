// EXPECT: === Vector Math Tests ===
// EXPECT: 2D Vector operations:
// EXPECT:   v1 = (3, 4)
// EXPECT:   v2 = (1, 2)
// EXPECT:   v1 + v2 = (4, 6)
// EXPECT:   v1 - v2 = (2, 2)
// EXPECT:   v1 dot v2 = 11
// EXPECT:   v1 * 3 = (9, 12)
// EXPECT:   mag_sq(v1) = 25
// EXPECT:   isqrt(25) = 5
// EXPECT:   mag_sq(0,0) = 0
// EXPECT:   (-3,4) dot (3,4) = 7
// EXPECT: 3D Vector operations:
// EXPECT:   u1 = (1, 2, 3)
// EXPECT:   u2 = (4, 5, 6)
// EXPECT:   u1 + u2 = (5, 7, 9)
// EXPECT:   u1 - u2 = (-3, -3, -3)
// EXPECT:   u1 dot u2 = 32
// EXPECT:   u1 cross u2 = (-3, 6, -3)
// EXPECT:   u2 cross u1 = (3, -6, 3)
// EXPECT:   mag_sq(u1) = 14
// EXPECT:   u1 * 2 = (2, 4, 6)
// EXPECT: Cross product properties:
// EXPECT:   a cross a = (0, 0, 0)
// EXPECT:   i cross j = (0, 0, 1)
// EXPECT:   j cross i = (0, 0, -1)
// EXPECT:   j cross k = (1, 0, 0)
// EXPECT:   k cross i = (0, 1, 0)
// EXPECT: Dot product properties:
// EXPECT:   v dot v = mag_sq(v): OK
// EXPECT:   v1 dot v2 = v2 dot v1: OK
// EXPECT:   (v1+v2) dot v3 = v1.v3 + v2.v3: OK
// EXPECT: Normalize approximation (scale to ~100):
// EXPECT:   norm(3,4) ~ (60, 80)
// EXPECT:   norm(0,5) ~ (0, 100)
// EXPECT:   norm(5,0) ~ (100, 0)
// EXPECT: 3D normalize approx (scale to ~100):
// EXPECT:   norm(1,0,0) ~ (100, 0, 0)
// EXPECT:   norm(0,1,0) ~ (0, 100, 0)
// EXPECT:   norm(0,0,1) ~ (0, 0, 100)
// EXPECT: Orthogonality test:
// EXPECT:   (1,0,0) dot (0,1,0) = 0: orthogonal
// EXPECT:   (1,2,3) dot (-3,0,1) = 0: orthogonal
// EXPECT:   (1,1,0) dot (1,0,0) = 1: not orthogonal
// EXPECT: All vector math tests passed!

int printf(const char *fmt, ...);

// Integer square root (floor)
int isqrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

// === 2D Vectors ===
struct Vec2 {
    int x;
    int y;
};

struct Vec2 vec2_new(int x, int y) {
    struct Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

struct Vec2 vec2_add(struct Vec2 a, struct Vec2 b) {
    struct Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

struct Vec2 vec2_sub(struct Vec2 a, struct Vec2 b) {
    struct Vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

int vec2_dot(struct Vec2 a, struct Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

struct Vec2 vec2_scale(struct Vec2 v, int s) {
    struct Vec2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

int vec2_mag_sq(struct Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

// Normalize to scale ~100 (integer approximation)
struct Vec2 vec2_norm100(struct Vec2 v) {
    struct Vec2 r;
    int mag = isqrt(vec2_mag_sq(v));
    if (mag == 0) { r.x = 0; r.y = 0; return r; }
    r.x = (v.x * 100) / mag;
    r.y = (v.y * 100) / mag;
    return r;
}

// === 3D Vectors ===
struct Vec3 {
    int x;
    int y;
    int z;
};

struct Vec3 vec3_new(int x, int y, int z) {
    struct Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Vec3 vec3_add(struct Vec3 a, struct Vec3 b) {
    struct Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

struct Vec3 vec3_sub(struct Vec3 a, struct Vec3 b) {
    struct Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

int vec3_dot(struct Vec3 a, struct Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vec3 vec3_cross(struct Vec3 a, struct Vec3 b) {
    struct Vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

struct Vec3 vec3_scale(struct Vec3 v, int s) {
    struct Vec3 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

int vec3_mag_sq(struct Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

struct Vec3 vec3_norm100(struct Vec3 v) {
    struct Vec3 r;
    int mag = isqrt(vec3_mag_sq(v));
    if (mag == 0) { r.x = 0; r.y = 0; r.z = 0; return r; }
    r.x = (v.x * 100) / mag;
    r.y = (v.y * 100) / mag;
    r.z = (v.z * 100) / mag;
    return r;
}

int main() {
    printf("=== Vector Math Tests ===\n");

    // 2D tests
    printf("2D Vector operations:\n");
    struct Vec2 v1 = vec2_new(3, 4);
    struct Vec2 v2 = vec2_new(1, 2);
    printf("  v1 = (%d, %d)\n", v1.x, v1.y);
    printf("  v2 = (%d, %d)\n", v2.x, v2.y);

    struct Vec2 r2;
    r2 = vec2_add(v1, v2);
    printf("  v1 + v2 = (%d, %d)\n", r2.x, r2.y);

    r2 = vec2_sub(v1, v2);
    printf("  v1 - v2 = (%d, %d)\n", r2.x, r2.y);

    printf("  v1 dot v2 = %d\n", vec2_dot(v1, v2));

    r2 = vec2_scale(v1, 3);
    printf("  v1 * 3 = (%d, %d)\n", r2.x, r2.y);

    printf("  mag_sq(v1) = %d\n", vec2_mag_sq(v1));
    printf("  isqrt(25) = %d\n", isqrt(vec2_mag_sq(v1)));

    struct Vec2 zero2 = vec2_new(0, 0);
    printf("  mag_sq(0,0) = %d\n", vec2_mag_sq(zero2));

    struct Vec2 v3 = vec2_new(-3, 4);
    printf("  (-3,4) dot (3,4) = %d\n", vec2_dot(v3, v1));

    // 3D tests
    printf("3D Vector operations:\n");
    struct Vec3 u1 = vec3_new(1, 2, 3);
    struct Vec3 u2 = vec3_new(4, 5, 6);
    printf("  u1 = (%d, %d, %d)\n", u1.x, u1.y, u1.z);
    printf("  u2 = (%d, %d, %d)\n", u2.x, u2.y, u2.z);

    struct Vec3 r3;
    r3 = vec3_add(u1, u2);
    printf("  u1 + u2 = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_sub(u1, u2);
    printf("  u1 - u2 = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    printf("  u1 dot u2 = %d\n", vec3_dot(u1, u2));

    r3 = vec3_cross(u1, u2);
    printf("  u1 cross u2 = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_cross(u2, u1);
    printf("  u2 cross u1 = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    printf("  mag_sq(u1) = %d\n", vec3_mag_sq(u1));

    r3 = vec3_scale(u1, 2);
    printf("  u1 * 2 = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    // Cross product properties
    printf("Cross product properties:\n");
    r3 = vec3_cross(u1, u1);
    printf("  a cross a = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    struct Vec3 vi = vec3_new(1, 0, 0);
    struct Vec3 vj = vec3_new(0, 1, 0);
    struct Vec3 vk = vec3_new(0, 0, 1);

    r3 = vec3_cross(vi, vj);
    printf("  i cross j = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_cross(vj, vi);
    printf("  j cross i = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_cross(vj, vk);
    printf("  j cross k = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_cross(vk, vi);
    printf("  k cross i = (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    // Dot product properties
    printf("Dot product properties:\n");
    int d1 = vec3_dot(u1, u1);
    int m1 = vec3_mag_sq(u1);
    printf("  v dot v = mag_sq(v): %s\n", d1 == m1 ? "OK" : "FAIL");

    int d12 = vec3_dot(u1, u2);
    int d21 = vec3_dot(u2, u1);
    printf("  v1 dot v2 = v2 dot v1: %s\n", d12 == d21 ? "OK" : "FAIL");

    // Distributive: (u1+u2) . u3 = u1.u3 + u2.u3
    struct Vec3 u3 = vec3_new(7, 8, 9);
    struct Vec3 sum3 = vec3_add(u1, u2);
    int lhs = vec3_dot(sum3, u3);
    int rhs = vec3_dot(u1, u3) + vec3_dot(u2, u3);
    printf("  (v1+v2) dot v3 = v1.v3 + v2.v3: %s\n", lhs == rhs ? "OK" : "FAIL");

    // Normalize approximation (2D)
    printf("Normalize approximation (scale to ~100):\n");
    r2 = vec2_norm100(v1);
    printf("  norm(3,4) ~ (%d, %d)\n", r2.x, r2.y);

    struct Vec2 v05 = vec2_new(0, 5);
    r2 = vec2_norm100(v05);
    printf("  norm(0,5) ~ (%d, %d)\n", r2.x, r2.y);

    struct Vec2 v50 = vec2_new(5, 0);
    r2 = vec2_norm100(v50);
    printf("  norm(5,0) ~ (%d, %d)\n", r2.x, r2.y);

    // Normalize approximation (3D)
    printf("3D normalize approx (scale to ~100):\n");
    r3 = vec3_norm100(vi);
    printf("  norm(1,0,0) ~ (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_norm100(vj);
    printf("  norm(0,1,0) ~ (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    r3 = vec3_norm100(vk);
    printf("  norm(0,0,1) ~ (%d, %d, %d)\n", r3.x, r3.y, r3.z);

    // Orthogonality
    printf("Orthogonality test:\n");
    int d_ij = vec3_dot(vi, vj);
    printf("  (1,0,0) dot (0,1,0) = %d: %s\n", d_ij,
           d_ij == 0 ? "orthogonal" : "not orthogonal");

    struct Vec3 oa = vec3_new(1, 2, 3);
    struct Vec3 ob = vec3_new(-3, 0, 1);
    int d_ab = vec3_dot(oa, ob);
    printf("  (1,2,3) dot (-3,0,1) = %d: %s\n", d_ab,
           d_ab == 0 ? "orthogonal" : "not orthogonal");

    struct Vec3 oc = vec3_new(1, 1, 0);
    int d_ci = vec3_dot(oc, vi);
    printf("  (1,1,0) dot (1,0,0) = %d: %s\n", d_ci,
           d_ci == 0 ? "orthogonal" : "not orthogonal");

    printf("All vector math tests passed!\n");
    return 0;
}
