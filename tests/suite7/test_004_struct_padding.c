int printf(const char *fmt, ...);

struct A_cici {
    char a;
    int b;
    char c;
    int d;
};

struct B_ccii {
    char a;
    char b;
    int c;
    int d;
};

struct C_iicc {
    int a;
    int b;
    char c;
    char d;
};

struct D_icci {
    int a;
    char b;
    char c;
    int d;
};

struct E_single_char {
    char a;
};

struct F_single_int {
    int a;
};

struct G_two_int {
    int a;
    int b;
};

struct H_three_char {
    char a;
    char b;
    char c;
};

struct Nested_pad {
    char tag;
    struct G_two_int pair;
    char end;
};

struct Inner_ci {
    char c;
    int i;
};

struct Outer_ici {
    int x;
    struct Inner_ci inner;
    int y;
};

void fill_A(struct A_cici *s, char a, int b, char c, int d) {
    s->a = a;
    s->b = b;
    s->c = c;
    s->d = d;
}

void fill_B(struct B_ccii *s, char a, char b, int c, int d) {
    s->a = a;
    s->b = b;
    s->c = c;
    s->d = d;
}

int sum_A(struct A_cici *s) {
    return s->a + s->b + s->c + s->d;
}

int sum_B(struct B_ccii *s) {
    return s->a + s->b + s->c + s->d;
}

int main(void) {
    /* sizeof checks */
    // EXPECT: sizeof A_cici=16
    printf("sizeof A_cici=%d\n", (int)sizeof(struct A_cici));

    // EXPECT: sizeof B_ccii=12
    printf("sizeof B_ccii=%d\n", (int)sizeof(struct B_ccii));

    // EXPECT: sizeof C_iicc=12
    printf("sizeof C_iicc=%d\n", (int)sizeof(struct C_iicc));

    // EXPECT: sizeof D_icci=12
    printf("sizeof D_icci=%d\n", (int)sizeof(struct D_icci));

    // EXPECT: sizeof E_single_char=1
    printf("sizeof E_single_char=%d\n", (int)sizeof(struct E_single_char));

    // EXPECT: sizeof F_single_int=4
    printf("sizeof F_single_int=%d\n", (int)sizeof(struct F_single_int));

    // EXPECT: sizeof G_two_int=8
    printf("sizeof G_two_int=%d\n", (int)sizeof(struct G_two_int));

    // EXPECT: sizeof H_three_char=3
    printf("sizeof H_three_char=%d\n", (int)sizeof(struct H_three_char));

    // EXPECT: sizeof Nested_pad=16
    printf("sizeof Nested_pad=%d\n", (int)sizeof(struct Nested_pad));

    // EXPECT: sizeof Inner_ci=8
    printf("sizeof Inner_ci=%d\n", (int)sizeof(struct Inner_ci));

    // EXPECT: sizeof Outer_ici=16
    printf("sizeof Outer_ici=%d\n", (int)sizeof(struct Outer_ici));

    /* field access correctness with padding */
    struct A_cici sa;
    fill_A(&sa, 1, 200, 3, 400);
    // EXPECT: A sum=604
    printf("A sum=%d\n", sum_A(&sa));
    // EXPECT: A fields=1 200 3 400
    printf("A fields=%d %d %d %d\n", sa.a, sa.b, sa.c, sa.d);

    struct B_ccii sb;
    fill_B(&sb, 5, 6, 700, 800);
    // EXPECT: B sum=1511
    printf("B sum=%d\n", sum_B(&sb));
    // EXPECT: B fields=5 6 700 800
    printf("B fields=%d %d %d %d\n", sb.a, sb.b, sb.c, sb.d);

    struct C_iicc sc;
    sc.a = 10;
    sc.b = 20;
    sc.c = 30;
    sc.d = 40;
    // EXPECT: C fields=10 20 30 40
    printf("C fields=%d %d %d %d\n", sc.a, sc.b, (int)sc.c, (int)sc.d);

    struct Nested_pad np;
    np.tag = 77;
    np.pair.a = 1000;
    np.pair.b = 2000;
    np.end = 88;
    // EXPECT: nested_pad tag=77 a=1000 b=2000 end=88
    printf("nested_pad tag=%d a=%d b=%d end=%d\n",
           (int)np.tag, np.pair.a, np.pair.b, (int)np.end);

    struct Outer_ici oi;
    oi.x = 111;
    oi.inner.c = 9;
    oi.inner.i = 222;
    oi.y = 333;
    // EXPECT: outer_ici x=111 c=9 i=222 y=333
    printf("outer_ici x=%d c=%d i=%d y=%d\n", oi.x, (int)oi.inner.c, oi.inner.i, oi.y);

    /* array of padded structs */
    struct A_cici arr[3];
    int idx;
    for (idx = 0; idx < 3; idx = idx + 1) {
        fill_A(&arr[idx], idx + 1, (idx + 1) * 100, idx + 10, (idx + 1) * 50);
    }
    // EXPECT: arr[0]=1 100 10 50
    printf("arr[0]=%d %d %d %d\n", arr[0].a, arr[0].b, arr[0].c, arr[0].d);
    // EXPECT: arr[2]=3 300 12 150
    printf("arr[2]=%d %d %d %d\n", arr[2].a, arr[2].b, arr[2].c, arr[2].d);

    return 0;
}
