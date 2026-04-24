int printf(const char *fmt, ...);
// EXPECT: A + 1 = B\nA + 5 = F\nA + 25 = Z\n'Z' - 'A' = 25\n'z' - 'a' = 25\n'9' - '0' = 9\nupper('h') = H\nupper('H') = H\nlower('H') = h\nlower('h') = h\nis_upper('A') = 1\nis_upper('a') = 0\nis_lower('z') = 1\nis_digit('5') = 1\nis_digit('x') = 0\nstr: abcdefghij\nsum of chars: 1015\nROT13('H') = U\nROT13('U') = H
// Test: arithmetic with char types

int is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

int is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

char to_upper(char c) {
    if (is_lower(c)) {
        return c - 32;
    }
    return c;
}

char to_lower(char c) {
    if (is_upper(c)) {
        return c + 32;
    }
    return c;
}

int main(void) {
    char c;
    int i;
    char str[20];
    int sum;

    /* Basic char arithmetic */
    c = 'A';
    printf("A + 1 = %c\n", c + 1);
    printf("A + 5 = %c\n", c + 5);
    printf("A + 25 = %c\n", c + 25);

    /* Difference between chars */
    printf("'Z' - 'A' = %d\n", 'Z' - 'A');
    printf("'z' - 'a' = %d\n", 'z' - 'a');
    printf("'9' - '0' = %d\n", '9' - '0');

    /* Case conversion */
    printf("upper('h') = %c\n", to_upper('h'));
    printf("upper('H') = %c\n", to_upper('H'));
    printf("lower('H') = %c\n", to_lower('H'));
    printf("lower('h') = %c\n", to_lower('h'));

    /* Character classification */
    printf("is_upper('A') = %d\n", is_upper('A'));
    printf("is_upper('a') = %d\n", is_upper('a'));
    printf("is_lower('z') = %d\n", is_lower('z'));
    printf("is_digit('5') = %d\n", is_digit('5'));
    printf("is_digit('x') = %d\n", is_digit('x'));

    /* Build string with char arithmetic */
    for (i = 0; i < 10; i++) {
        str[i] = (char)('a' + i);
    }
    str[10] = '\0';
    printf("str: %s\n", str);

    /* Sum of char values */
    sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + str[i];
    }
    printf("sum of chars: %d\n", sum);

    /* ROT13 on a char */
    c = 'H';
    if (is_upper(c)) {
        c = 'A' + (c - 'A' + 13) % 26;
    }
    printf("ROT13('H') = %c\n", c);

    c = 'U';
    if (is_upper(c)) {
        c = 'A' + (c - 'A' + 13) % 26;
    }
    printf("ROT13('U') = %c\n", c);

    return 0;
}
