int printf(const char *fmt, ...);
// EXPECT: line is valid
// file is valid

int main(void) {
    int line = __LINE__;
    if (line > 0)
        printf("line is valid\n");
    else
        printf("FAIL: line=%d\n", line);

    const char *file = __FILE__;
    if (file[0] != '\0')
        printf("file is valid\n");
    else
        printf("FAIL: empty file\n");
    return 0;
}
