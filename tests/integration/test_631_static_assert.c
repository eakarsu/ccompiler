int printf(const char *fmt, ...);
// EXPECT: static_assert works
// sizeof int is 4

int main(void) {
    _Static_assert(sizeof(int) == 4, "int must be 4 bytes");
    _Static_assert(1, "always true");
    _Static_assert(sizeof(char) == 1, "char must be 1 byte");
    _Static_assert(sizeof(long) >= 4, "long must be at least 4 bytes");
    printf("static_assert works\n");
    printf("sizeof int is %d\n", (int)sizeof(int));
    return 0;
}
