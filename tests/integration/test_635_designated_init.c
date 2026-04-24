int printf(const char *fmt, ...);
// EXPECT: p.x=10 p.y=20
// arr[0]=0 arr[1]=0 arr[2]=42 arr[3]=0 arr[4]=99
// r.x=1 r.y=0 r.z=3
// mixed a=5 b=10 c=15

struct Point { int x; int y; };

struct Triple { int x; int y; int z; };

struct Mixed { int a; int b; int c; };

int main(void) {
    /* Struct designated initializers */
    struct Point p = { .y = 20, .x = 10 };
    printf("p.x=%d p.y=%d\n", p.x, p.y);

    /* Array designated initializers */
    int arr[5] = { [2] = 42, [4] = 99 };
    printf("arr[0]=%d arr[1]=%d arr[2]=%d arr[3]=%d arr[4]=%d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4]);

    /* Partial designator: first and third set, second defaults to zero */
    struct Triple r = { .x = 1, .z = 3 };
    printf("r.x=%d r.y=%d r.z=%d\n", r.x, r.y, r.z);

    /* Mixed: designated then positional */
    struct Mixed m = { .a = 5, 10, 15 };
    printf("mixed a=%d b=%d c=%d\n", m.a, m.b, m.c);

    return 0;
}
