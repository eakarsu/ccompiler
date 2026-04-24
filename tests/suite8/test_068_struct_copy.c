int printf(const char *fmt, ...);

struct Small {
    int x;
    int y;
};

struct Medium {
    int a;
    int b;
    int c;
    int d;
};

struct Large {
    int data[8];
};

struct Nested {
    struct Small pos;
    struct Small vel;
    int id;
};

void copy_small(struct Small *dst, struct Small *src) {
    dst->x = src->x;
    dst->y = src->y;
}

void copy_medium(struct Medium *dst, struct Medium *src) {
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    dst->d = src->d;
}

void copy_large(struct Large *dst, struct Large *src) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        dst->data[i] = src->data[i];
    }
}

void copy_nested(struct Nested *dst, struct Nested *src) {
    copy_small(&dst->pos, &src->pos);
    copy_small(&dst->vel, &src->vel);
    dst->id = src->id;
}

int small_equal(struct Small *a, struct Small *b) {
    return (a->x == b->x) && (a->y == b->y);
}

int medium_equal(struct Medium *a, struct Medium *b) {
    return (a->a == b->a) && (a->b == b->b) && (a->c == b->c) && (a->d == b->d);
}

int large_equal(struct Large *a, struct Large *b) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

int nested_equal(struct Nested *a, struct Nested *b) {
    if (!small_equal(&a->pos, &b->pos)) return 0;
    if (!small_equal(&a->vel, &b->vel)) return 0;
    if (a->id != b->id) return 0;
    return 1;
}

void swap_small(struct Small *a, struct Small *b) {
    struct Small tmp;
    copy_small(&tmp, a);
    copy_small(a, b);
    copy_small(b, &tmp);
}

int sum_large(struct Large *l) {
    int sum = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        sum = sum + l->data[i];
    }
    return sum;
}

void scale_large(struct Large *dst, struct Large *src, int factor) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        dst->data[i] = src->data[i] * factor;
    }
}

void add_large(struct Large *dst, struct Large *a, struct Large *b) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        dst->data[i] = a->data[i] + b->data[i];
    }
}

int main() {
    struct Small s1;
    s1.x = 10;
    s1.y = 20;
    struct Small s2;
    copy_small(&s2, &s1);
    // EXPECT: Copied small: (10, 20)
    printf("Copied small: (%d, %d)\n", s2.x, s2.y);
    // EXPECT: Small equal: 1
    printf("Small equal: %d\n", small_equal(&s1, &s2));

    s2.x = 99;
    // EXPECT: After modify, equal: 0
    printf("After modify, equal: %d\n", small_equal(&s1, &s2));
    // EXPECT: Original unchanged: (10, 20)
    printf("Original unchanged: (%d, %d)\n", s1.x, s1.y);

    struct Medium m1;
    m1.a = 1; m1.b = 2; m1.c = 3; m1.d = 4;
    struct Medium m2;
    copy_medium(&m2, &m1);
    // EXPECT: Medium copy: (1, 2, 3, 4)
    printf("Medium copy: (%d, %d, %d, %d)\n", m2.a, m2.b, m2.c, m2.d);
    // EXPECT: Medium equal: 1
    printf("Medium equal: %d\n", medium_equal(&m1, &m2));

    struct Large l1;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        l1.data[i] = (i + 1) * 10;
    }
    struct Large l2;
    copy_large(&l2, &l1);
    // EXPECT: Large equal: 1
    printf("Large equal: %d\n", large_equal(&l1, &l2));
    // EXPECT: Large sum: 360
    printf("Large sum: %d\n", sum_large(&l2));

    struct Large l3;
    scale_large(&l3, &l1, 2);
    // EXPECT: Scaled sum: 720
    printf("Scaled sum: %d\n", sum_large(&l3));

    struct Large l4;
    add_large(&l4, &l1, &l3);
    // EXPECT: Added sum: 1080
    printf("Added sum: %d\n", sum_large(&l4));

    struct Nested n1;
    n1.pos.x = 5; n1.pos.y = 10;
    n1.vel.x = 1; n1.vel.y = -1;
    n1.id = 42;
    struct Nested n2;
    copy_nested(&n2, &n1);
    // EXPECT: Nested equal: 1
    printf("Nested equal: %d\n", nested_equal(&n1, &n2));
    // EXPECT: Nested copy pos: (5, 10)
    printf("Nested copy pos: (%d, %d)\n", n2.pos.x, n2.pos.y);
    // EXPECT: Nested copy vel: (1, -1)
    printf("Nested copy vel: (%d, %d)\n", n2.vel.x, n2.vel.y);
    // EXPECT: Nested copy id: 42
    printf("Nested copy id: %d\n", n2.id);

    struct Small sa;
    sa.x = 1; sa.y = 2;
    struct Small sb;
    sb.x = 3; sb.y = 4;
    swap_small(&sa, &sb);
    // EXPECT: After swap: a=(3, 4) b=(1, 2)
    printf("After swap: a=(%d, %d) b=(%d, %d)\n", sa.x, sa.y, sb.x, sb.y);

    // EXPECT: Large element 3: 40
    printf("Large element 3: %d\n", l1.data[3]);
    // EXPECT: Scaled element 3: 80
    printf("Scaled element 3: %d\n", l3.data[3]);

    return 0;
}
