int printf(const char *fmt, ...);
// EXPECT: a=5 b=12
// x=3 y=7 z=1
// size=4

struct Packed {
    unsigned int a : 4;
    unsigned int b : 4;
};

struct Flags {
    unsigned int x : 3;
    unsigned int y : 4;
    unsigned int z : 1;
};

int main(void) {
    struct Packed p;
    p.a = 5;
    p.b = 12;
    printf("a=%d b=%d\n", p.a, p.b);

    struct Flags f;
    f.x = 3;
    f.y = 7;
    f.z = 1;
    printf("x=%d y=%d z=%d\n", f.x, f.y, f.z);

    printf("size=%d\n", (int)sizeof(struct Packed));

    return 0;
}
