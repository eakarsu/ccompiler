int printf(const char *fmt, ...);
// EXPECT: 65 as char: A\n97 as char: a\n'A' as int: 65\n'z' as int: 122\nABCDEFGHIJ\n256 as char: 0\n300 as char: 44\n'5' - '0' = 5\n7 + '0' = 7\nchar vals: 0 10 20 30 40 50 60 70 80 90 \ncast in expr: 76\nchar(-1) = -1\nchar(-128) = -128
// Test: type casting between int and char

int main(void) {
    int i;
    char c;
    int arr[10];
    char buf[10];
    int val;

    /* Int to char truncation */
    i = 65;
    c = (char)i;
    printf("65 as char: %c\n", c);

    i = 97;
    c = (char)i;
    printf("97 as char: %c\n", c);

    /* Char to int promotion */
    c = 'A';
    i = (int)c;
    printf("'A' as int: %d\n", i);

    c = 'z';
    i = (int)c;
    printf("'z' as int: %d\n", i);

    /* Char arithmetic with casting */
    c = 'A';
    for (i = 0; i < 26; i++) {
        buf[0] = (char)(c + i);
        if (i < 10) {
            printf("%c", buf[0]);
        }
    }
    printf("\n");

    /* Int to char overflow */
    i = 256;
    c = (char)i;
    printf("256 as char: %d\n", (int)c);

    i = 300;
    c = (char)i;
    printf("300 as char: %d\n", (int)c);

    /* Digit char to int */
    c = '5';
    val = c - '0';
    printf("'5' - '0' = %d\n", val);

    /* Int to digit char */
    i = 7;
    c = (char)(i + '0');
    printf("7 + '0' = %c\n", c);

    /* Array of chars as small ints */
    for (i = 0; i < 10; i++) {
        buf[i] = (char)(i * 10);
    }
    printf("char vals: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", (int)buf[i]);
    }
    printf("\n");

    /* Cast in expression */
    i = 1000;
    val = (int)((char)i) + 100;
    printf("cast in expr: %d\n", val);

    /* Negative char */
    c = (char)(-1);
    printf("char(-1) = %d\n", (int)c);

    c = (char)(-128);
    printf("char(-128) = %d\n", (int)c);

    return 0;
}
