int printf(const char *fmt, ...);

/* Compound literals: (Type){...} used as expressions, args, in loops, etc. */

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point tl;
    struct Point br;
};

/* Accept a Point by value */
int point_sum(struct Point p) {
    return p.x + p.y;
}

/* Accept a Rect by value */
int rect_area(struct Rect r) {
    int w = r.br.x - r.tl.x;
    int h = r.br.y - r.tl.y;
    return w * h;
}

/* Accept int array pointer and size */
int array_sum(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += arr[i];
    return s;
}

/* Return a Point compound literal from function */
struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

int main(void) {
    /* --- Basic struct compound literal --- */
    struct Point p1 = (struct Point){3, 7};
    printf("%d\n", p1.x);          // EXPECT: 3
    printf("%d\n", p1.y);          // EXPECT: 7

    /* --- Compound literal as function argument --- */
    int s1 = point_sum((struct Point){10, 20});
    printf("%d\n", s1);            // EXPECT: 30

    /* --- Nested struct compound literal --- */
    struct Rect r1 = (struct Rect){(struct Point){1, 2}, (struct Point){5, 6}};
    printf("%d\n", r1.tl.x);      // EXPECT: 1
    printf("%d\n", r1.tl.y);      // EXPECT: 2
    printf("%d\n", r1.br.x);      // EXPECT: 5
    printf("%d\n", r1.br.y);      // EXPECT: 6

    /* --- Rect area via compound literal arg --- */
    int area = rect_area((struct Rect){(struct Point){0, 0}, (struct Point){4, 3}});
    printf("%d\n", area);          // EXPECT: 12

    /* --- Compound literal assigned, then mutated --- */
    struct Point p2 = (struct Point){100, 200};
    p2.x = p2.x + 5;
    p2.y = p2.y - 10;
    printf("%d\n", p2.x);         // EXPECT: 105
    printf("%d\n", p2.y);         // EXPECT: 190

    /* --- Compound literal in expression context --- */
    int total = point_sum((struct Point){7, 3}) + point_sum((struct Point){2, 8});
    printf("%d\n", total);         // EXPECT: 20

    /* --- Multiple compound literals in same statement --- */
    struct Point pa = (struct Point){1, 1};
    struct Point pb = (struct Point){9, 9};
    printf("%d\n", pa.x + pb.x);  // EXPECT: 10
    printf("%d\n", pa.y + pb.y);  // EXPECT: 10

    /* --- Compound literal in loop body --- */
    int loop_sum = 0;
    int i;
    for (i = 1; i <= 5; i++) {
        struct Point tmp = (struct Point){i, i * 2};
        loop_sum += tmp.x + tmp.y;
        /* i=1: 1+2=3; i=2: 2+4=6; i=3: 3+6=9; i=4: 4+8=12; i=5: 5+10=15 => 45 */
    }
    printf("%d\n", loop_sum);      // EXPECT: 45

    /* --- Compound literal assigned to pointer (address-of) --- */
    struct Point *pp = &(struct Point){55, 66};
    printf("%d\n", pp->x);        // EXPECT: 55
    printf("%d\n", pp->y);        // EXPECT: 66

    /* --- Compound literal address-of, modify through pointer --- */
    struct Point *pm = &(struct Point){10, 20};
    pm->x *= 3;
    pm->y += 5;
    printf("%d\n", pm->x);        // EXPECT: 30
    printf("%d\n", pm->y);        // EXPECT: 25

    /* --- Compound literal member access directly --- */
    int cx = (struct Point){42, 58}.x;
    int cy = (struct Point){42, 58}.y;
    printf("%d\n", cx);           // EXPECT: 42
    printf("%d\n", cy);           // EXPECT: 58

    /* --- Compound literal in conditional --- */
    struct Point pc = (struct Point){0, 0};
    int cond = (pc.x == 0) ? point_sum((struct Point){3, 4}) : 0;
    printf("%d\n", cond);         // EXPECT: 7

    /* --- Compound literal updated in if-else --- */
    struct Point pd;
    int flag = 1;
    if (flag) {
        pd = (struct Point){11, 22};
    } else {
        pd = (struct Point){0, 0};
    }
    printf("%d\n", pd.x);         // EXPECT: 11
    printf("%d\n", pd.y);         // EXPECT: 22

    /* --- Compound literal used in return from make_point, compared --- */
    struct Point mp = make_point(7, 8);
    struct Point cl = (struct Point){7, 8};
    printf("%d\n", (mp.x == cl.x) ? 1 : 0); // EXPECT: 1
    printf("%d\n", (mp.y == cl.y) ? 1 : 0); // EXPECT: 1

    /* --- Compound literal sum across array of structs --- */
    struct Point pts[4];
    pts[0] = (struct Point){1, 2};
    pts[1] = (struct Point){3, 4};
    pts[2] = (struct Point){5, 6};
    pts[3] = (struct Point){7, 8};
    int xsum = 0, ysum = 0;
    for (i = 0; i < 4; i++) {
        xsum += pts[i].x;
        ysum += pts[i].y;
    }
    printf("%d\n", xsum);         // EXPECT: 16
    printf("%d\n", ysum);         // EXPECT: 20

    /* --- Compound literal overwrite in loop --- */
    struct Point last = (struct Point){0, 0};
    for (i = 0; i < 3; i++) {
        last = (struct Point){i * 10, i * 10 + 1};
    }
    /* After loop: i=2, last = {20, 21} */
    printf("%d\n", last.x);       // EXPECT: 20
    printf("%d\n", last.y);       // EXPECT: 21

    /* --- Compound literal as argument in while loop --- */
    int wsum = 0;
    int j = 0;
    while (j < 4) {
        wsum += point_sum((struct Point){j, j + 1});
        /* j=0:1; j=1:3; j=2:5; j=3:7 => 16 */
        j++;
    }
    printf("%d\n", wsum);         // EXPECT: 16

    return 0;
}
