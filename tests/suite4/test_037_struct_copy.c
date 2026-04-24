int printf(const char *fmt, ...);

// Test: Struct assignment (copy), comparing members after copy, modifying copy

struct Point {
    int x;
    int y;
};

struct Triple {
    int a;
    int b;
    int c;
};

struct WithArray {
    int data[4];
    int len;
};

struct Nested {
    struct Point pos;
    int id;
};

struct Big {
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
};

int main(void) {
    // Basic struct copy via assignment
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    struct Point p2 = p1;
    // EXPECT: 10 20
    printf("%d %d\n", p2.x, p2.y);

    // Verify both have same values
    int same = (p1.x == p2.x && p1.y == p2.y) ? 1 : 0;
    // EXPECT: 1
    printf("%d\n", same);

    // Modify copy, original unchanged
    p2.x = 999;
    p2.y = 888;
    // EXPECT: 10 20
    printf("%d %d\n", p1.x, p1.y);
    // EXPECT: 999 888
    printf("%d %d\n", p2.x, p2.y);

    // Triple struct copy
    struct Triple t1;
    t1.a = 100;
    t1.b = 200;
    t1.c = 300;
    struct Triple t2 = t1;
    // EXPECT: 100 200 300
    printf("%d %d %d\n", t2.a, t2.b, t2.c);

    // Modify original, copy unchanged
    t1.a = 0;
    t1.b = 0;
    t1.c = 0;
    // EXPECT: 100 200 300
    printf("%d %d %d\n", t2.a, t2.b, t2.c);
    // EXPECT: 0 0 0
    printf("%d %d %d\n", t1.a, t1.b, t1.c);

    // Struct with array: copy
    struct WithArray w1;
    w1.data[0] = 11;
    w1.data[1] = 22;
    w1.data[2] = 33;
    w1.data[3] = 44;
    w1.len = 4;
    struct WithArray w2 = w1;
    // EXPECT: 11 22 33 44
    printf("%d %d %d %d\n", w2.data[0], w2.data[1], w2.data[2], w2.data[3]);
    // EXPECT: 4
    printf("%d\n", w2.len);

    // Modify copy's array, original unchanged
    w2.data[0] = 99;
    w2.data[1] = 88;
    // EXPECT: 11 22
    printf("%d %d\n", w1.data[0], w1.data[1]);
    // EXPECT: 99 88
    printf("%d %d\n", w2.data[0], w2.data[1]);

    // Nested struct copy
    struct Nested n1;
    n1.pos.x = 5;
    n1.pos.y = 15;
    n1.id = 42;
    struct Nested n2 = n1;
    // EXPECT: 5 15 42
    printf("%d %d %d\n", n2.pos.x, n2.pos.y, n2.id);

    // Modify nested copy
    n2.pos.x = 50;
    n2.pos.y = 150;
    n2.id = 420;
    // EXPECT: 5 15 42
    printf("%d %d %d\n", n1.pos.x, n1.pos.y, n1.id);
    // EXPECT: 50 150 420
    printf("%d %d %d\n", n2.pos.x, n2.pos.y, n2.id);

    // Re-assign struct (overwrite)
    struct Point p3;
    p3.x = 1;
    p3.y = 2;
    struct Point p4;
    p4.x = 3;
    p4.y = 4;
    p3 = p4;
    // EXPECT: 3 4
    printf("%d %d\n", p3.x, p3.y);

    // Chain of copies
    struct Point chain_a;
    chain_a.x = 7;
    chain_a.y = 14;
    struct Point chain_b = chain_a;
    struct Point chain_c = chain_b;
    struct Point chain_d = chain_c;
    // EXPECT: 7 14
    printf("%d %d\n", chain_d.x, chain_d.y);

    // Modify end of chain, start unchanged
    chain_d.x = 0;
    // EXPECT: 7
    printf("%d\n", chain_a.x);
    // EXPECT: 0
    printf("%d\n", chain_d.x);

    // Big struct copy
    struct Big big1;
    big1.v0 = 10;
    big1.v1 = 20;
    big1.v2 = 30;
    big1.v3 = 40;
    big1.v4 = 50;
    struct Big big2 = big1;
    // EXPECT: 10 20 30 40 50
    printf("%d %d %d %d %d\n", big2.v0, big2.v1, big2.v2, big2.v3, big2.v4);

    // Sum of copied big struct
    int bigsum = big2.v0 + big2.v1 + big2.v2 + big2.v3 + big2.v4;
    // EXPECT: 150
    printf("%d\n", bigsum);

    // Copy in loop
    struct Point arr[4];
    struct Point src;
    src.x = 77;
    src.y = 88;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        arr[i] = src;
    }
    int all_same = 1;
    for (i = 0; i < 4; i = i + 1) {
        if (arr[i].x != 77 || arr[i].y != 88) {
            all_same = 0;
        }
    }
    // EXPECT: 1
    printf("%d\n", all_same);

    return 0;
}
