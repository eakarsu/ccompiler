int printf(const char *fmt, ...);
// EXPECT: foobar=42
// var3=100

#define CONCAT(a, b) a##b
#define VAR(n) var##n

int main(void) {
    int foobar = 42;
    printf("foobar=%d\n", CONCAT(foo, bar));

    int var3 = 100;
    printf("var3=%d\n", VAR(3));

    return 0;
}
