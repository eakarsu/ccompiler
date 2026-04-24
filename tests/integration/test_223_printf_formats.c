int printf(const char *fmt, ...);
// EXPECT: === integer formats ===\ndecimal: 42\nnegative: -17\nzero: 0\nlarge: 1000000\n=== multiple args ===\n3 + 4 = 7\n6 * 7 = 42\n10 - 3 = 7\n=== char format ===\nchar: A\nchars: Hi!\n=== string format ===\nhello world\none two three\n=== mixed formats ===\nval=42 str=test chr=X\n=== int_to_str ===\n12345 -> 12345\n-99 -> -99\n0 -> 0\n=== table ===\n   1   1     2       1\n   1   2     3       2\n   1   3     4       3\n   2   1     3       2\n   2   2     4       4\n   2   3     5       6\n   3   1     4       3\n   3   2     5       6\n   3   3     6       9\n   4   1     5       4\n   4   2     6       8\n   4   3     7      12\n   5   1     6       5\n   5   2     7      10\n   5   3     8      15
// Test 223: Variadic-style (printf format testing)

int int_to_str(int val, char *buf) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return 1;
    }
    int neg = 0;
    int temp = val;
    if (val < 0) {
        neg = 1;
        temp = -val;
    }
    int digits[12];
    int count = 0;
    while (temp > 0) {
        digits[count] = temp - (temp / 10) * 10;
        temp = temp / 10;
        count = count + 1;
    }
    int pos = 0;
    if (neg) {
        buf[pos] = '-';
        pos = pos + 1;
    }
    int i = count - 1;
    while (i >= 0) {
        buf[pos] = (char)(digits[i] + 48);
        pos = pos + 1;
        i = i - 1;
    }
    buf[pos] = 0;
    return pos;
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

void print_padded(int val, int width) {
    char buf[20];
    int_to_str(val, buf);
    int len = str_len(buf);
    int pad = width - len;
    while (pad > 0) {
        printf(" ");
        pad = pad - 1;
    }
    printf("%s", buf);
}

void print_table_row(int a, int b, int sum, int prod) {
    print_padded(a, 4);
    print_padded(b, 4);
    print_padded(sum, 6);
    print_padded(prod, 8);
    printf("\n");
}

int main(void) {
    printf("=== integer formats ===\n");
    printf("decimal: %d\n", 42);
    printf("negative: %d\n", -17);
    printf("zero: %d\n", 0);
    printf("large: %d\n", 1000000);

    printf("=== multiple args ===\n");
    printf("%d + %d = %d\n", 3, 4, 7);
    printf("%d * %d = %d\n", 6, 7, 42);
    printf("%d - %d = %d\n", 10, 3, 7);

    printf("=== char format ===\n");
    printf("char: %c\n", 65);
    printf("chars: %c%c%c\n", 72, 105, 33);

    printf("=== string format ===\n");
    printf("hello %s\n", "world");
    printf("%s %s %s\n", "one", "two", "three");

    printf("=== mixed formats ===\n");
    printf("val=%d str=%s chr=%c\n", 42, "test", 88);

    printf("=== int_to_str ===\n");
    char buf[20];
    int_to_str(12345, buf);
    printf("12345 -> %s\n", buf);
    int_to_str(-99, buf);
    printf("-99 -> %s\n", buf);
    int_to_str(0, buf);
    printf("0 -> %s\n", buf);

    printf("=== table ===\n");
    int i = 1;
    while (i <= 5) {
        int j = 1;
        while (j <= 3) {
            print_table_row(i, j, i + j, i * j);
            j = j + 1;
        }
        i = i + 1;
    }

    return 0;
}
