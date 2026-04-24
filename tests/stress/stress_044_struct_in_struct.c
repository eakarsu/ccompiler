// stress_044_struct_in_struct.c - Deeply nested struct composition torture test
// EXPECT: deep_access=42
// EXPECT: deep_sum=150
// EXPECT: array_of_outer=540
// EXPECT: ptr_inner=77
// EXPECT: copy_nested=42
// EXPECT: compare_ok=1
// EXPECT: modify_deep=999
// EXPECT: mixed_access=210
// EXPECT: chain_assign=55
// EXPECT: nested_array_struct=600
// EXPECT: struct_ptr_chain=123
// EXPECT: offset_trick=88

int printf(const char *fmt, ...);

struct D {
    int d1;
    int d2;
    int d3;
};

struct C {
    int c1;
    struct D d;
    int c2;
};

struct B {
    struct C c;
    int b1;
    int b2;
};

struct A {
    int a1;
    struct B b;
    int a2;
};

// --- Struct with array inside ---
struct WithArray {
    int tag;
    int vals[5];
    int count;
};

// --- Struct containing two structs ---
struct Pair {
    struct D left;
    struct D right;
};

// Sum all fields of struct A
int sum_a(struct A *p) {
    return p->a1 + p->a2
         + p->b.b1 + p->b.b2
         + p->b.c.c1 + p->b.c.c2
         + p->b.c.d.d1 + p->b.c.d.d2 + p->b.c.d.d3;
}

// Access inner struct D through pointer
int get_d1(struct D *dp) {
    return dp->d1;
}

int main(void) {
    // --- Deep access: a.b.c.d.d1 ---
    struct A a;
    a.a1 = 10;
    a.a2 = 20;
    a.b.b1 = 30;
    a.b.b2 = 40;
    a.b.c.c1 = 50;
    a.b.c.c2 = 60;
    a.b.c.d.d1 = 42;
    a.b.c.d.d2 = 80;
    a.b.c.d.d3 = 90;
    printf("deep_access=%d\n", a.b.c.d.d1);

    // --- Sum of all fields through pointer ---
    // 10+20+30+40+50+60+42+80+90 = 422... let me adjust
    a.a1 = 10; a.a2 = 20; a.b.b1 = 5; a.b.b2 = 15;
    a.b.c.c1 = 25; a.b.c.c2 = 35;
    a.b.c.d.d1 = 10; a.b.c.d.d2 = 15; a.b.c.d.d3 = 15;
    // sum = 10+20+5+15+25+35+10+15+15 = 150
    printf("deep_sum=%d\n", sum_a(&a));

    // --- Array of outer struct ---
    struct A arr[3];
    int i;
    for (i = 0; i < 3; i++) {
        arr[i].a1 = (i + 1) * 10;
        arr[i].a2 = (i + 1) * 10;
        arr[i].b.b1 = (i + 1) * 5;
        arr[i].b.b2 = (i + 1) * 5;
        arr[i].b.c.c1 = (i + 1) * 10;
        arr[i].b.c.c2 = (i + 1) * 10;
        arr[i].b.c.d.d1 = (i + 1) * 5;
        arr[i].b.c.d.d2 = (i + 1) * 5;
        arr[i].b.c.d.d3 = (i + 1) * 10 - (i + 1) * 5;
    }
    // For i=0 (mult=1): 10+10+5+5+10+10+5+5+5 = 65... hmm
    // Actually (i+1)*10 - (i+1)*5 = (i+1)*5
    // Each struct sum = (i+1)*(10+10+5+5+10+10+5+5+5) = (i+1)*65
    // Total = 1*65 + 2*65 + 3*65 = 6*65 = 390... not 300
    // Let me simplify: each field = (i+1)*N where sum of N's = 100, then total = 6*100/2=...
    // Actually let me just set all fields to (i+1)*X:
    for (i = 0; i < 3; i++) {
        int m = (i + 1) * 10;
        arr[i].a1 = m;
        arr[i].a2 = m;
        arr[i].b.b1 = m;
        arr[i].b.b2 = m;
        arr[i].b.c.c1 = m;
        arr[i].b.c.c2 = m;
        arr[i].b.c.d.d1 = m;
        arr[i].b.c.d.d2 = m;
        arr[i].b.c.d.d3 = m;
    }
    // Each has 9 fields, all = (i+1)*10
    // arr[0] sum = 9*10=90, arr[1] sum=9*20=180, arr[2]=9*30=270
    // total = 90+180+270 = 540. Not 300.
    // For 300: fields all = (i+1)*N, 9*N*(1+2+3)=54*N=300 -> N not integer
    // Let me use 5 fields: no, struct has 9 fields.
    // Let me just compute and fix EXPECT:
    // With all = (i+1)*10: total = 540. Or just print sum of arr[0] and arr[1]:
    // 90+180=270. Or sum_a(&arr[1]) = 180.
    // Let me simplify: arr[i].all = 10*(i+1), total of arr = 540. Update EXPECT.
    // Or: set each to (i+1)*5 -> each sum = 9*5*(i+1), total = 45*(1+2+3)=270.
    // For 300: 300/6=50 per (i+1), so 50/9 per field... not clean.
    // Just use sum_a(&arr[0]) + sum_a(&arr[1]) = 9*10 + 9*20 = 90+180 = 270.
    // I'll fix EXPECT to 540.
    {
        int total = 0;
        for (i = 0; i < 3; i++)
            total = total + sum_a(&arr[i]);
        printf("array_of_outer=%d\n", total);
    }

    // --- Pointer to inner struct ---
    struct A a2;
    a2.b.c.d.d1 = 77;
    a2.b.c.d.d2 = 88;
    a2.b.c.d.d3 = 99;
    struct D *dp = &a2.b.c.d;
    printf("ptr_inner=%d\n", get_d1(dp));

    // --- Copy nested struct (full struct assignment) ---
    struct A a3;
    a3 = a2;
    a3.b.c.d.d1 = 42;
    // a2 should be unchanged
    printf("copy_nested=%d\n", a3.b.c.d.d1);

    // --- Compare nested struct fields ---
    struct A x;
    struct A y;
    x.b.c.d.d1 = 100; x.b.c.d.d2 = 200; x.b.c.d.d3 = 300;
    y.b.c.d.d1 = 100; y.b.c.d.d2 = 200; y.b.c.d.d3 = 300;
    int eq = (x.b.c.d.d1 == y.b.c.d.d1)
           && (x.b.c.d.d2 == y.b.c.d.d2)
           && (x.b.c.d.d3 == y.b.c.d.d3);
    printf("compare_ok=%d\n", eq);

    // --- Modify deeply nested and verify ---
    struct A m;
    m.a1 = 1; m.a2 = 2;
    m.b.b1 = 3; m.b.b2 = 4;
    m.b.c.c1 = 5; m.b.c.c2 = 6;
    m.b.c.d.d1 = 7; m.b.c.d.d2 = 8; m.b.c.d.d3 = 9;
    m.b.c.d.d1 = 999;
    printf("modify_deep=%d\n", m.b.c.d.d1);

    // --- Mixed access patterns ---
    // Access struct fields through various levels
    struct A t;
    t.a1 = 10; t.a2 = 20;
    t.b.b1 = 30; t.b.b2 = 40;
    t.b.c.c1 = 50; t.b.c.c2 = 60;
    t.b.c.d.d1 = 0; t.b.c.d.d2 = 0; t.b.c.d.d3 = 0;
    // sum = a1 + a2 + b1 + b2 + c1 + c2 = 10+20+30+40+50+60 = 210
    printf("mixed_access=%d\n", t.a1 + t.a2 + t.b.b1 + t.b.b2 + t.b.c.c1 + t.b.c.c2);

    // --- Chain assignment through nested fields ---
    struct A ca;
    ca.b.c.d.d1 = 55;
    ca.b.c.d.d2 = ca.b.c.d.d1;
    ca.b.c.d.d3 = ca.b.c.d.d2;
    ca.b.c.c1 = ca.b.c.d.d3;
    printf("chain_assign=%d\n", ca.b.c.c1);

    // --- Struct containing array, nested inside another struct ---
    struct WithArray wa;
    wa.tag = 100;
    wa.count = 5;
    for (i = 0; i < 5; i++)
        wa.vals[i] = (i + 1) * 20;
    // sum of vals = 20+40+60+80+100 = 300
    // total = tag + sum + count = 100 + 300 + 5 = 405
    // But we want 600. Let me adjust:
    wa.tag = 0;
    wa.count = 0;
    for (i = 0; i < 5; i++)
        wa.vals[i] = (i + 1) * 40;
    // 40+80+120+160+200 = 600
    {
        int ws = wa.tag;
        for (i = 0; i < wa.count + 5; i++)
            ws = ws + wa.vals[i];
        printf("nested_array_struct=%d\n", ws);
    }

    // --- Struct pointer chain: A* -> B -> C -> D ---
    struct A *ap;
    struct A target;
    target.b.c.d.d1 = 123;
    target.b.c.d.d2 = 456;
    target.b.c.d.d3 = 789;
    ap = &target;
    printf("struct_ptr_chain=%d\n", ap->b.c.d.d1);

    // --- Offset access: modify inner via outer ---
    struct Pair p;
    p.left.d1 = 11; p.left.d2 = 22; p.left.d3 = 33;
    p.right.d1 = 88; p.right.d2 = 77; p.right.d3 = 66;
    printf("offset_trick=%d\n", p.right.d1);

    return 0;
}
