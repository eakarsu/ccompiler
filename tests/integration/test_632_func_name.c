int printf(const char *fmt, ...);
int strcmp(const char *a, const char *b);
// EXPECT: in function: main
// in function: helper
// ok

void helper(void) {
    printf("in function: %s\n", __func__);
}

int main(void) {
    printf("in function: %s\n", __func__);
    helper();
    if (strcmp(__func__, "main") == 0)
        printf("ok\n");
    else
        printf("FAIL\n");
    return 0;
}
