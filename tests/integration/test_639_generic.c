int printf(const char *fmt, ...);
int strcmp(const char *a, const char *b);
// EXPECT: int type: int
// double type: double
// char type: default
// ok

int main(void) {
    /* Integer matches int */
    const char *s1 = _Generic(42, int: "int", double: "double", default: "other");
    printf("int type: %s\n", s1);

    /* Double matches double */
    const char *s2 = _Generic(3.14, int: "int", double: "double", default: "other");
    printf("double type: %s\n", s2);

    /* Char falls through to default since there's no char association */
    char c = 'x';
    const char *s3 = _Generic(c, int: "int", double: "double", default: "default");
    printf("char type: %s\n", s3);

    if (strcmp(s1, "int") == 0 && strcmp(s2, "double") == 0)
        printf("ok\n");
    else
        printf("FAIL\n");

    return 0;
}
