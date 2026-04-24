int printf(const char *fmt, ...);
// EXPECT: a = (1, 2, 3)\nb = (4, 5, 6)\na + b = (5, 7, 9)\ndot(a, b) = 32\n|a|^2 = 14\na * 3 = (3, 6, 9)\nafter swap:\na = (4, 5, 6)\nb = (3, 6, 9)
// Test: struct pointer and arrow operator

struct Vec3 {
    int x;
    int y;
    int z;
};

void vec3_set(struct Vec3 *v, int x, int y, int z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void vec3_add(struct Vec3 *result, struct Vec3 *a, struct Vec3 *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void vec3_scale(struct Vec3 *v, int factor) {
    v->x = v->x * factor;
    v->y = v->y * factor;
    v->z = v->z * factor;
}

int vec3_dot(struct Vec3 *a, struct Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

int vec3_length_sq(struct Vec3 *v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

void vec3_print(struct Vec3 *v) {
    printf("(%d, %d, %d)\n", v->x, v->y, v->z);
}

void vec3_swap(struct Vec3 *a, struct Vec3 *b) {
    struct Vec3 tmp;
    tmp.x = a->x; tmp.y = a->y; tmp.z = a->z;
    a->x = b->x; a->y = b->y; a->z = b->z;
    b->x = tmp.x; b->y = tmp.y; b->z = tmp.z;
}

int main(void) {
    struct Vec3 a;
    struct Vec3 b;
    struct Vec3 c;

    vec3_set(&a, 1, 2, 3);
    vec3_set(&b, 4, 5, 6);

    printf("a = "); vec3_print(&a);
    printf("b = "); vec3_print(&b);

    vec3_add(&c, &a, &b);
    printf("a + b = "); vec3_print(&c);

    printf("dot(a, b) = %d\n", vec3_dot(&a, &b));
    printf("|a|^2 = %d\n", vec3_length_sq(&a));

    vec3_scale(&a, 3);
    printf("a * 3 = "); vec3_print(&a);

    vec3_swap(&a, &b);
    printf("after swap:\n");
    printf("a = "); vec3_print(&a);
    printf("b = "); vec3_print(&b);

    return 0;
}
