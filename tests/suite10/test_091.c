int printf(const char *fmt, ...);

// Large struct with 20+ fields stress test

struct BigStruct {
    int a; int b; int c; int d; int e;
    int f; int g; int h; int i; int j;
    int k; int l; int m; int n; int o;
    int p; int q; int r; int s; int t;
    int u; int v; int w; int x; int y;
};

void init_big(struct BigStruct *bs, int base) {
    bs->a = base + 1;
    bs->b = base + 2;
    bs->c = base + 3;
    bs->d = base + 4;
    bs->e = base + 5;
    bs->f = base + 6;
    bs->g = base + 7;
    bs->h = base + 8;
    bs->i = base + 9;
    bs->j = base + 10;
    bs->k = base + 11;
    bs->l = base + 12;
    bs->m = base + 13;
    bs->n = base + 14;
    bs->o = base + 15;
    bs->p = base + 16;
    bs->q = base + 17;
    bs->r = base + 18;
    bs->s = base + 19;
    bs->t = base + 20;
    bs->u = base + 21;
    bs->v = base + 22;
    bs->w = base + 23;
    bs->x = base + 24;
    bs->y = base + 25;
}

int sum_big(struct BigStruct *bs) {
    int total = 0;
    total = total + bs->a + bs->b + bs->c + bs->d + bs->e;
    total = total + bs->f + bs->g + bs->h + bs->i + bs->j;
    total = total + bs->k + bs->l + bs->m + bs->n + bs->o;
    total = total + bs->p + bs->q + bs->r + bs->s + bs->t;
    total = total + bs->u + bs->v + bs->w + bs->x + bs->y;
    return total;
}

int product_first_five(struct BigStruct *bs) {
    return bs->a * bs->b * bs->c * bs->d * bs->e;
}

void scale_big(struct BigStruct *bs, int factor) {
    bs->a = bs->a * factor;
    bs->b = bs->b * factor;
    bs->c = bs->c * factor;
    bs->d = bs->d * factor;
    bs->e = bs->e * factor;
    bs->f = bs->f * factor;
    bs->g = bs->g * factor;
    bs->h = bs->h * factor;
    bs->i = bs->i * factor;
    bs->j = bs->j * factor;
    bs->k = bs->k * factor;
    bs->l = bs->l * factor;
    bs->m = bs->m * factor;
    bs->n = bs->n * factor;
    bs->o = bs->o * factor;
    bs->p = bs->p * factor;
    bs->q = bs->q * factor;
    bs->r = bs->r * factor;
    bs->s = bs->s * factor;
    bs->t = bs->t * factor;
    bs->u = bs->u * factor;
    bs->v = bs->v * factor;
    bs->w = bs->w * factor;
    bs->x = bs->x * factor;
    bs->y = bs->y * factor;
}

void copy_big(struct BigStruct *dst, struct BigStruct *src) {
    dst->a = src->a; dst->b = src->b; dst->c = src->c;
    dst->d = src->d; dst->e = src->e; dst->f = src->f;
    dst->g = src->g; dst->h = src->h; dst->i = src->i;
    dst->j = src->j; dst->k = src->k; dst->l = src->l;
    dst->m = src->m; dst->n = src->n; dst->o = src->o;
    dst->p = src->p; dst->q = src->q; dst->r = src->r;
    dst->s = src->s; dst->t = src->t; dst->u = src->u;
    dst->v = src->v; dst->w = src->w; dst->x = src->x;
    dst->y = src->y;
}

int max_field(struct BigStruct *bs) {
    int max = bs->a;
    if (bs->b > max) max = bs->b;
    if (bs->c > max) max = bs->c;
    if (bs->d > max) max = bs->d;
    if (bs->e > max) max = bs->e;
    if (bs->f > max) max = bs->f;
    if (bs->g > max) max = bs->g;
    if (bs->h > max) max = bs->h;
    if (bs->i > max) max = bs->i;
    if (bs->j > max) max = bs->j;
    if (bs->k > max) max = bs->k;
    if (bs->l > max) max = bs->l;
    if (bs->m > max) max = bs->m;
    if (bs->n > max) max = bs->n;
    if (bs->o > max) max = bs->o;
    if (bs->p > max) max = bs->p;
    if (bs->q > max) max = bs->q;
    if (bs->r > max) max = bs->r;
    if (bs->s > max) max = bs->s;
    if (bs->t > max) max = bs->t;
    if (bs->u > max) max = bs->u;
    if (bs->v > max) max = bs->v;
    if (bs->w > max) max = bs->w;
    if (bs->x > max) max = bs->x;
    if (bs->y > max) max = bs->y;
    return max;
}

int main() {
    struct BigStruct s1;
    struct BigStruct s2;

    init_big(&s1, 0);
    int total = sum_big(&s1);
    printf("sum base 0: %d\n", total);
    // EXPECT: sum base 0: 325

    init_big(&s1, 100);
    total = sum_big(&s1);
    printf("sum base 100: %d\n", total);
    // EXPECT: sum base 100: 2825

    init_big(&s1, 0);
    int prod = product_first_five(&s1);
    printf("product first 5: %d\n", prod);
    // EXPECT: product first 5: 120

    scale_big(&s1, 3);
    total = sum_big(&s1);
    printf("scaled sum: %d\n", total);
    // EXPECT: scaled sum: 975

    copy_big(&s2, &s1);
    total = sum_big(&s2);
    printf("copied sum: %d\n", total);
    // EXPECT: copied sum: 975

    init_big(&s1, 10);
    int mx = max_field(&s1);
    printf("max field: %d\n", mx);
    // EXPECT: max field: 35

    init_big(&s1, -30);
    total = sum_big(&s1);
    printf("negative base sum: %d\n", total);
    // EXPECT: negative base sum: -425

    mx = max_field(&s1);
    printf("negative base max: %d\n", mx);
    // EXPECT: negative base max: -5

    return 0;
}
