int printf(const char *fmt, ...);

/* Small structs (<=16 bytes) returned by value */
struct Vec2 {
    int x;
    int y;
};

struct Triple {
    int a;
    int b;
    int c;
};

/* Large struct (>16 bytes) returned via hidden pointer (x8 ABI) */
struct BigData {
    int v[6];   /* 6 ints = 24 bytes */
};

struct TwoLong {
    int p;
    int q;
    int r;
    int s;
    int t;   /* 5 ints = 20 bytes */
};

/* --- Functions returning small structs --- */

struct Vec2 make_vec2(int x, int y) {
    struct Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

struct Vec2 add_vec2(struct Vec2 a, struct Vec2 b) {
    struct Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

struct Vec2 scale_vec2(struct Vec2 v, int factor) {
    struct Vec2 r;
    r.x = v.x * factor;
    r.y = v.y * factor;
    return r;
}

struct Triple make_triple(int a, int b, int c) {
    struct Triple t;
    t.a = a;
    t.b = b;
    t.c = c;
    return t;
}

struct Triple sum_triples(struct Triple x, struct Triple y) {
    struct Triple r;
    r.a = x.a + y.a;
    r.b = x.b + y.b;
    r.c = x.c + y.c;
    return r;
}

/* --- Functions returning large structs --- */

struct BigData make_big(int base) {
    struct BigData d;
    int i;
    for (i = 0; i < 6; i++) {
        d.v[i] = base + i;
    }
    return d;
}

struct TwoLong make_twolong(int p, int q, int r, int s, int t) {
    struct TwoLong tl;
    tl.p = p;
    tl.q = q;
    tl.r = r;
    tl.s = s;
    tl.t = t;
    return tl;
}

/* Sum all fields of BigData */
int sum_big(struct BigData d) {
    int i, s = 0;
    for (i = 0; i < 6; i++) s += d.v[i];
    return s;
}

int main(void) {
    /* --- Small struct return and member access --- */
    struct Vec2 v = make_vec2(3, 4);
    printf("%d\n", v.x);                    // EXPECT: 3
    printf("%d\n", v.y);                    // EXPECT: 4

    /* Direct member access on returned struct */
    printf("%d\n", make_vec2(7, 8).x);      // EXPECT: 7
    printf("%d\n", make_vec2(7, 8).y);      // EXPECT: 8

    /* add_vec2: (1,2)+(3,4)=(4,6) */
    struct Vec2 sum = add_vec2(make_vec2(1, 2), make_vec2(3, 4));
    printf("%d\n", sum.x);                  // EXPECT: 4
    printf("%d\n", sum.y);                  // EXPECT: 6

    /* scale_vec2: (5,6)*3=(15,18) */
    struct Vec2 scaled = scale_vec2(make_vec2(5, 6), 3);
    printf("%d\n", scaled.x);               // EXPECT: 15
    printf("%d\n", scaled.y);               // EXPECT: 18

    /* Chained: scale(add((1,2),(2,3)),2) = scale((3,5),2) = (6,10) */
    struct Vec2 chained = scale_vec2(add_vec2(make_vec2(1, 2), make_vec2(2, 3)), 2);
    printf("%d\n", chained.x);              // EXPECT: 6
    printf("%d\n", chained.y);              // EXPECT: 10

    /* Triple */
    struct Triple t1 = make_triple(1, 2, 3);
    printf("%d\n", t1.a);                   // EXPECT: 1
    printf("%d\n", t1.b);                   // EXPECT: 2
    printf("%d\n", t1.c);                   // EXPECT: 3

    struct Triple t2 = make_triple(10, 20, 30);
    struct Triple t3 = sum_triples(t1, t2);
    printf("%d\n", t3.a);                   // EXPECT: 11
    printf("%d\n", t3.b);                   // EXPECT: 22
    printf("%d\n", t3.c);                   // EXPECT: 33

    /* Direct member on returned triple */
    printf("%d\n", make_triple(99, 88, 77).b); // EXPECT: 88

    /* --- Large struct return (>16 bytes) --- */
    struct BigData bd = make_big(10);
    /* base=10: v=[10,11,12,13,14,15] */
    printf("%d\n", bd.v[0]);                // EXPECT: 10
    printf("%d\n", bd.v[1]);                // EXPECT: 11
    printf("%d\n", bd.v[5]);                // EXPECT: 15
    /* sum = 10+11+12+13+14+15 = 75 */
    printf("%d\n", sum_big(bd));            // EXPECT: 75

    /* Large struct returned and passed again */
    struct BigData bd2 = make_big(0);
    /* v=[0,1,2,3,4,5] sum=15 */
    printf("%d\n", sum_big(bd2));           // EXPECT: 15

    /* TwoLong (20 bytes) */
    struct TwoLong tl = make_twolong(2, 4, 6, 8, 10);
    printf("%d\n", tl.p);                  // EXPECT: 2
    printf("%d\n", tl.q);                  // EXPECT: 4
    printf("%d\n", tl.r);                  // EXPECT: 6
    printf("%d\n", tl.s);                  // EXPECT: 8
    printf("%d\n", tl.t);                  // EXPECT: 10

    /* Use returned struct in arithmetic */
    struct Vec2 va = make_vec2(3, 4);
    struct Vec2 vb = make_vec2(6, 8);
    /* dot product: 3*6 + 4*8 = 18+32 = 50 */
    printf("%d\n", va.x * vb.x + va.y * vb.y); // EXPECT: 50

    /* Reassign from return */
    va = make_vec2(0, 0);
    printf("%d\n", va.x);                   // EXPECT: 0
    printf("%d\n", va.y);                   // EXPECT: 0

    /* Multiple returns stored in array */
    struct Vec2 vecs[3];
    vecs[0] = make_vec2(1, 1);
    vecs[1] = make_vec2(2, 2);
    vecs[2] = make_vec2(3, 3);
    int i;
    int dot_sum = 0;
    for (i = 0; i < 3; i++) {
        dot_sum += vecs[i].x + vecs[i].y;
    }
    /* (1+1)+(2+2)+(3+3) = 12 */
    printf("%d\n", dot_sum);                // EXPECT: 12

    return 0;
}
