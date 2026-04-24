int printf(const char *fmt, ...);
// EXPECT: 10 20 30
// 42 99
// 100
// 7

struct Point { int x; int y; };

int main(void) {
    /* Array compound literal */
    int *p = (int[]){10, 20, 30};
    printf("%d %d %d\n", p[0], p[1], p[2]);

    /* Address-of compound literal */
    struct Point *q = &(struct Point){42, 99};
    printf("%d %d\n", q->x, q->y);

    /* Compound literal as function arg */
    struct Point r = (struct Point){100, 200};
    printf("%d\n", r.x);

    /* Scalar compound literal */
    int v = (int){7};
    printf("%d\n", v);

    return 0;
}
