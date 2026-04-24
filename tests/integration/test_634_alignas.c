int printf(const char *fmt, ...);
// EXPECT: alignment ok

int main(void) {
    _Alignas(16) int x = 42;
    /* Check that the address is 16-byte aligned */
    long addr = (long)&x;
    if ((addr % 16) == 0)
        printf("alignment ok\n");
    else
        printf("FAIL: addr=%ld mod16=%ld\n", addr, addr % 16);
    return 0;
}
