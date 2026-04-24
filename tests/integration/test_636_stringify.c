int printf(const char *fmt, ...);
int strcmp(const char *a, const char *b);
// EXPECT: stringified: hello
// stringified: 42
// ok

#define STR(x) #x

int main(void) {
    printf("stringified: %s\n", STR(hello));
    printf("stringified: %s\n", STR(42));
    if (strcmp(STR(hello), "hello") == 0)
        printf("ok\n");
    else
        printf("FAIL\n");
    return 0;
}
