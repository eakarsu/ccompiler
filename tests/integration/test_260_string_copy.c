int printf(const char *fmt, ...);
// EXPECT: copied: Hello\nlen: 5\nempty copy len: 0\nlong copy: abcdefghij\npartial (5): abcde\nchain: XYZ -> XYZ -> XYZ\nbefore: old\nafter: XYZ\noffset copy: .....XYZ
// Test: string copy (manual strcpy, strncpy)

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

void my_strncpy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (src[i] == '\0') {
            // Pad with nulls
            while (i < n) {
                dst[i] = '\0';
                i = i + 1;
            }
            return;
        }
        dst[i] = src[i];
    }
}

int main(void) {
    // Basic strcpy
    char src[10];
    src[0]='H'; src[1]='e'; src[2]='l'; src[3]='l'; src[4]='o'; src[5]='\0';
    char dst[20];
    my_strcpy(dst, src);
    printf("copied: %s\n", dst);
    printf("len: %d\n", my_strlen(dst));

    // Copy empty string
    char empty[1];
    empty[0] = '\0';
    char buf[10];
    my_strcpy(buf, empty);
    printf("empty copy len: %d\n", my_strlen(buf));

    // Copy longer string
    char long_str[20];
    long_str[0]='a'; long_str[1]='b'; long_str[2]='c'; long_str[3]='d';
    long_str[4]='e'; long_str[5]='f'; long_str[6]='g'; long_str[7]='h';
    long_str[8]='i'; long_str[9]='j'; long_str[10]='\0';
    char long_dst[20];
    my_strcpy(long_dst, long_str);
    printf("long copy: %s\n", long_dst);

    // strncpy - copy partial
    char partial[10];
    my_strncpy(partial, long_str, 5);
    partial[5] = '\0';
    printf("partial (5): %s\n", partial);

    // Chain copies
    char a[10]; char b[10]; char c[10];
    a[0]='X'; a[1]='Y'; a[2]='Z'; a[3]='\0';
    my_strcpy(b, a);
    my_strcpy(c, b);
    printf("chain: %s -> %s -> %s\n", a, b, c);

    // Overwrite destination
    char overwrite[10];
    overwrite[0]='o'; overwrite[1]='l'; overwrite[2]='d'; overwrite[3]='\0';
    printf("before: %s\n", overwrite);
    my_strcpy(overwrite, a);
    printf("after: %s\n", overwrite);

    // Copy into middle of buffer
    char big[30];
    int i;
    for (i = 0; i < 29; i++) big[i] = '.';
    big[29] = '\0';
    my_strcpy(big + 5, a);
    printf("offset copy: ");
    for (i = 0; i < 8; i++) {
        printf("%c", big[i]);
    }
    printf("\n");

    return 0;
}
