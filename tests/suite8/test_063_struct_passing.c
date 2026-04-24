int printf(const char *fmt, ...);

struct Vec2 {
    int x;
    int y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Transform {
    struct Vec3 position;
    struct Vec3 scale;
    int rotation;
};

int vec2_dot(struct Vec2 *a, struct Vec2 *b) {
    return a->x * b->x + a->y * b->y;
}

int vec2_length_sq(struct Vec2 *v) {
    return v->x * v->x + v->y * v->y;
}

void vec2_add(struct Vec2 *result, struct Vec2 *a, struct Vec2 *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void vec2_scale(struct Vec2 *result, struct Vec2 *v, int factor) {
    result->x = v->x * factor;
    result->y = v->y * factor;
}

int vec3_dot(struct Vec3 *a, struct Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_cross(struct Vec3 *result, struct Vec3 *a, struct Vec3 *b) {
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

void vec3_add(struct Vec3 *result, struct Vec3 *a, struct Vec3 *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void init_transform(struct Transform *t, int px, int py, int pz,
                    int sx, int sy, int sz, int rot) {
    t->position.x = px;
    t->position.y = py;
    t->position.z = pz;
    t->scale.x = sx;
    t->scale.y = sy;
    t->scale.z = sz;
    t->rotation = rot;
}

void translate(struct Transform *t, struct Vec3 *offset) {
    t->position.x = t->position.x + offset->x;
    t->position.y = t->position.y + offset->y;
    t->position.z = t->position.z + offset->z;
}

void apply_scale(struct Transform *t, int factor) {
    t->scale.x = t->scale.x * factor;
    t->scale.y = t->scale.y * factor;
    t->scale.z = t->scale.z * factor;
}

void print_vec3(struct Vec3 *v, char *label) {
    printf("%s: (%d, %d, %d)\n", label, v->x, v->y, v->z);
}

void print_transform(struct Transform *t) {
    printf("Pos: (%d, %d, %d)\n", t->position.x, t->position.y, t->position.z);
    printf("Scale: (%d, %d, %d)\n", t->scale.x, t->scale.y, t->scale.z);
    printf("Rotation: %d\n", t->rotation);
}

int vec3_length_sq(struct Vec3 *v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

int main() {
    struct Vec2 a;
    a.x = 3;
    a.y = 4;
    struct Vec2 b;
    b.x = 1;
    b.y = 2;

    // EXPECT: Vec2 dot: 11
    printf("Vec2 dot: %d\n", vec2_dot(&a, &b));

    // EXPECT: Vec2 length_sq: 25
    printf("Vec2 length_sq: %d\n", vec2_length_sq(&a));

    struct Vec2 sum2;
    vec2_add(&sum2, &a, &b);
    // EXPECT: Vec2 sum: (4, 6)
    printf("Vec2 sum: (%d, %d)\n", sum2.x, sum2.y);

    struct Vec2 scaled2;
    vec2_scale(&scaled2, &a, 3);
    // EXPECT: Vec2 scaled: (9, 12)
    printf("Vec2 scaled: (%d, %d)\n", scaled2.x, scaled2.y);

    struct Vec3 v1;
    v1.x = 1; v1.y = 2; v1.z = 3;
    struct Vec3 v2;
    v2.x = 4; v2.y = 5; v2.z = 6;

    // EXPECT: Vec3 dot: 32
    printf("Vec3 dot: %d\n", vec3_dot(&v1, &v2));

    struct Vec3 cross;
    vec3_cross(&cross, &v1, &v2);
    // EXPECT: Cross: (-3, 6, -3)
    printf("Cross: (%d, %d, %d)\n", cross.x, cross.y, cross.z);

    struct Vec3 sum3;
    vec3_add(&sum3, &v1, &v2);
    // EXPECT: Vec3 sum: (5, 7, 9)
    printf("Vec3 sum: (%d, %d, %d)\n", sum3.x, sum3.y, sum3.z);

    struct Transform t;
    init_transform(&t, 10, 20, 30, 1, 1, 1, 45);
    // EXPECT: Initial transform:
    printf("Initial transform:\n");
    // EXPECT: Pos: (10, 20, 30)
    // EXPECT: Scale: (1, 1, 1)
    // EXPECT: Rotation: 45
    print_transform(&t);

    struct Vec3 offset;
    offset.x = 5; offset.y = -10; offset.z = 15;
    translate(&t, &offset);
    // EXPECT: After translate:
    printf("After translate:\n");
    // EXPECT: Pos: (15, 10, 45)
    // EXPECT: Scale: (1, 1, 1)
    // EXPECT: Rotation: 45
    print_transform(&t);

    apply_scale(&t, 3);
    // EXPECT: After scale:
    printf("After scale:\n");
    // EXPECT: Pos: (15, 10, 45)
    // EXPECT: Scale: (3, 3, 3)
    // EXPECT: Rotation: 45
    print_transform(&t);

    // EXPECT: Position length_sq: 2350
    printf("Position length_sq: %d\n", vec3_length_sq(&t.position));

    return 0;
}
