int printf(const char *fmt, ...);

// Test char arithmetic, signed char behavior, char-to-int promotion,
// and casting between char and int.

int char_to_int(char c) {
    return c;
}

char int_to_char(int i) {
    return (char)i;
}

int char_add(char a, char b) {
    return a + b;
}

int char_sub(char a, char b) {
    return a - b;
}

int char_mul(char a, char b) {
    return a * b;
}

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int is_upper(char c) {
    return c >= 65 && c <= 90;
}

int is_lower(char c) {
    return c >= 97 && c <= 122;
}

char to_upper(char c) {
    if (is_lower(c)) return (char)(c - 32);
    return c;
}

char to_lower(char c) {
    if (is_upper(c)) return (char)(c + 32);
    return c;
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int str_sum(char *s) {
    int sum = 0;
    int i = 0;
    while (s[i] != 0) {
        sum = sum + s[i];
        i = i + 1;
    }
    return sum;
}

int count_char(char *s, char target) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == target) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    // Char literal values (ASCII)
    char a = 65;   // 'A'
    char z = 122;  // 'z'
    char zero = 48; // '0'
    char nine = 57; // '9'

    // Char to int promotion
    // EXPECT: A as int = 65
    printf("A as int = %d\n", char_to_int(a));

    // EXPECT: z as int = 122
    printf("z as int = %d\n", char_to_int(z));

    // Char arithmetic: A + 1 = B (66)
    // EXPECT: A + 1 = 66
    printf("A + 1 = %d\n", char_add(a, 1));

    // Char subtraction
    // EXPECT: z - a = 57
    printf("z - a = %d\n", char_sub(z, a));

    // Difference between '9' and '0'
    // EXPECT: 9 - 0 = 9
    printf("9 - 0 = %d\n", char_sub(nine, zero));

    // Char multiplication
    // EXPECT: 10 * 10 = 100
    printf("10 * 10 = %d\n", char_mul(10, 10));

    // Negative char (signed): -1 stored in char then promoted to int
    char neg = (char)(-1);
    // EXPECT: neg char = -1
    printf("neg char = %d\n", char_to_int(neg));

    // Char -128 (minimum signed char)
    char cmin = (char)(-128);
    // EXPECT: char min = -128
    printf("char min = %d\n", char_to_int(cmin));

    // Char 127 (maximum signed char)
    char cmax = 127;
    // EXPECT: char max = 127
    printf("char max = %d\n", char_to_int(cmax));

    // Truncation: 256 -> 0 when cast to char
    char trunc = (char)256;
    // EXPECT: 256 as char = 0
    printf("256 as char = %d\n", char_to_int(trunc));

    // Truncation: 255 -> -1 when cast to signed char
    char trunc2 = (char)255;
    // EXPECT: 255 as char = -1
    printf("255 as char = %d\n", char_to_int(trunc2));

    // Truncation: 130 -> -126 when cast to signed char
    char trunc3 = (char)130;
    // EXPECT: 130 as char = -126
    printf("130 as char = %d\n", char_to_int(trunc3));

    // is_digit tests
    // EXPECT: is_digit(48) = 1
    printf("is_digit(%d) = %d\n", 48, is_digit(48));

    // EXPECT: is_digit(65) = 0
    printf("is_digit(%d) = %d\n", 65, is_digit(65));

    // to_upper / to_lower
    // 97 is 'a', to_upper gives 65 which is 'A'
    // EXPECT: to_upper(97) = 65
    printf("to_upper(%d) = %d\n", 97, (int)to_upper(97));

    // 65 is 'A', to_lower gives 97 which is 'a'
    // EXPECT: to_lower(65) = 97
    printf("to_lower(%d) = %d\n", 65, (int)to_lower(65));

    // String length
    char *hello = "hello";
    // EXPECT: len(hello) = 5
    printf("len(hello) = %d\n", str_len(hello));

    // Sum of ASCII values in "abc": 97+98+99 = 294
    char *abc = "abc";
    // EXPECT: sum(abc) = 294
    printf("sum(abc) = %d\n", str_sum(abc));

    // Count occurrences of 'l' (108) in "hello world"
    char *hw = "hello world";
    // EXPECT: count l = 3
    printf("count l = %d\n", count_char(hw, 108));

    // Char array manipulation
    char buf[6];
    buf[0] = 72;  // H
    buf[1] = 101; // e
    buf[2] = 108; // l
    buf[3] = 108; // l
    buf[4] = 111; // o
    buf[5] = 0;
    // EXPECT: buf = Hello
    printf("buf = %s\n", buf);

    // EXPECT: buf_len = 5
    printf("buf_len = %d\n", str_len(buf));

    // Char comparison
    char x = 100;
    char y = 50;
    // EXPECT: 100 > 50 = 1
    printf("%d > %d = %d\n", (int)x, (int)y, x > y);

    // Negative char comparison
    char p = (char)(-10);
    char q = (char)(-20);
    // EXPECT: -10 > -20 = 1
    printf("%d > %d = %d\n", (int)p, (int)q, p > q);

    // Char in expressions: promoted to int
    char c1 = 100;
    char c2 = 50;
    int result = c1 * c2 + c1 - c2;
    // 100*50 + 100 - 50 = 5000 + 100 - 50 = 5050
    // EXPECT: expr = 5050
    printf("expr = %d\n", result);

    return 0;
}
