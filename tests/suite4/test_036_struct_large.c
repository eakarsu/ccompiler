int printf(const char *fmt, ...);

// Test: Large structs (>16 bytes), passing and returning them, verifying all fields

struct Big4 {
    int a;
    int b;
    int c;
    int d;
};

struct Big5 {
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
};

struct Big8 {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
};

struct Big10 {
    int vals[10];
};

// Return large struct (>16 bytes, uses hidden x8 pointer on ARM64)
struct Big4 make_big4(int a, int b, int c, int d) {
    struct Big4 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    return s;
}

struct Big5 make_big5(int start) {
    struct Big5 s;
    s.v0 = start;
    s.v1 = start + 1;
    s.v2 = start + 2;
    s.v3 = start + 3;
    s.v4 = start + 4;
    return s;
}

struct Big8 make_big8(int base) {
    struct Big8 s;
    s.f0 = base;
    s.f1 = base + 10;
    s.f2 = base + 20;
    s.f3 = base + 30;
    s.f4 = base + 40;
    s.f5 = base + 50;
    s.f6 = base + 60;
    s.f7 = base + 70;
    return s;
}

void modify_big4(struct Big4 *s) {
    s->a = s->a * 2;
    s->b = s->b * 2;
    s->c = s->c * 2;
    s->d = s->d * 2;
}

int sum_big5(struct Big5 *s) {
    return s->v0 + s->v1 + s->v2 + s->v3 + s->v4;
}

int sum_big8(struct Big8 *s) {
    return s->f0 + s->f1 + s->f2 + s->f3 + s->f4 + s->f5 + s->f6 + s->f7;
}

int main(void) {
    // Big4: 4 ints = 16 bytes (borderline)
    struct Big4 b4 = make_big4(10, 20, 30, 40);
    // EXPECT: 10 20 30 40
    printf("%d %d %d %d\n", b4.a, b4.b, b4.c, b4.d);

    // Sum of Big4 fields
    int s4 = b4.a + b4.b + b4.c + b4.d;
    // EXPECT: 100
    printf("%d\n", s4);

    // Big5: 5 ints = 20 bytes (>16)
    struct Big5 b5 = make_big5(100);
    // EXPECT: 100 101 102 103 104
    printf("%d %d %d %d %d\n", b5.v0, b5.v1, b5.v2, b5.v3, b5.v4);

    // Sum of Big5
    int s5 = sum_big5(&b5);
    // 100+101+102+103+104 = 510
    // EXPECT: 510
    printf("%d\n", s5);

    // Big8: 8 ints = 32 bytes (large)
    struct Big8 b8 = make_big8(5);
    // EXPECT: 5 15 25 35 45 55 65 75
    printf("%d %d %d %d %d %d %d %d\n", b8.f0, b8.f1, b8.f2, b8.f3, b8.f4, b8.f5, b8.f6, b8.f7);

    // Sum of Big8
    int s8 = sum_big8(&b8);
    // 5+15+25+35+45+55+65+75 = 320
    // EXPECT: 320
    printf("%d\n", s8);

    // Modify large struct through pointer
    modify_big4(&b4);
    // EXPECT: 20 40 60 80
    printf("%d %d %d %d\n", b4.a, b4.b, b4.c, b4.d);

    // Big10: struct with int array (40 bytes)
    struct Big10 b10;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        b10.vals[i] = (i + 1) * 5;
    }
    // EXPECT: 5 10 15 20 25
    printf("%d %d %d %d %d\n", b10.vals[0], b10.vals[1], b10.vals[2], b10.vals[3], b10.vals[4]);
    // EXPECT: 30 35 40 45 50
    printf("%d %d %d %d %d\n", b10.vals[5], b10.vals[6], b10.vals[7], b10.vals[8], b10.vals[9]);

    // Sum of Big10
    int s10 = 0;
    for (i = 0; i < 10; i = i + 1) {
        s10 = s10 + b10.vals[i];
    }
    // 5+10+15+20+25+30+35+40+45+50 = 275
    // EXPECT: 275
    printf("%d\n", s10);

    // Multiple large struct returns
    struct Big5 arr5[3];
    for (i = 0; i < 3; i = i + 1) {
        arr5[i] = make_big5(i * 50);
    }
    // EXPECT: 0 1 2 3 4
    printf("%d %d %d %d %d\n", arr5[0].v0, arr5[0].v1, arr5[0].v2, arr5[0].v3, arr5[0].v4);
    // EXPECT: 50 51 52 53 54
    printf("%d %d %d %d %d\n", arr5[1].v0, arr5[1].v1, arr5[1].v2, arr5[1].v3, arr5[1].v4);
    // EXPECT: 100 101 102 103 104
    printf("%d %d %d %d %d\n", arr5[2].v0, arr5[2].v1, arr5[2].v2, arr5[2].v3, arr5[2].v4);

    // Verify field independence: changing one doesn't affect another
    struct Big8 b8_copy = make_big8(1);
    b8_copy.f3 = 999;
    // EXPECT: 1 11 21 999 41 51 61 71
    printf("%d %d %d %d %d %d %d %d\n", b8_copy.f0, b8_copy.f1, b8_copy.f2, b8_copy.f3, b8_copy.f4, b8_copy.f5, b8_copy.f6, b8_copy.f7);

    // Another Big5 with negative start
    struct Big5 neg5 = make_big5(-10);
    // EXPECT: -10 -9 -8 -7 -6
    printf("%d %d %d %d %d\n", neg5.v0, neg5.v1, neg5.v2, neg5.v3, neg5.v4);

    // Sum of negative Big5: -10+-9+-8+-7+-6 = -40
    int neg_sum = sum_big5(&neg5);
    // EXPECT: -40
    printf("%d\n", neg_sum);

    return 0;
}
