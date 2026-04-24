int printf(const char *fmt, ...);
// EXPECT: 1 0 1 1
// 1
// 0 1

int main(void) {
    _Bool a = 5;
    _Bool b = 0;
    _Bool c = -1;
    _Bool d = 256;
    printf("%d %d %d %d\n", a, b, c, d);

    int x = 42;
    _Bool e = x;
    printf("%d\n", e);

    /* Bool assignment from expressions */
    _Bool f = (0 == 1);
    _Bool g = (1 == 1);
    printf("%d %d\n", f, g);

    return 0;
}
