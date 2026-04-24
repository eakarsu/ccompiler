// String edge cases: concatenation, long strings, escape sequences, comparisons
int printf(const char *fmt, ...);

int my_strlen(const char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int my_strcmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

void test_adjacent_concat(void) {
    const char *s = "hello" " " "world" "!";
    printf("%s\n", s);
    // EXPECT: hello world!
}

void test_multiline_concat(void) {
    const char *s = "part1_"
                    "part2_"
                    "part3_"
                    "part4";
    printf("%s\n", s);
    // EXPECT: part1_part2_part3_part4
}

void test_long_string(void) {
    const char *s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789!@#$%^&*()_+-=[]"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789!@#$%^&*()_+-=[]"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789";
    int len = my_strlen(s);
    printf("long len=%d\n", len);
    // EXPECT: long len=218
}

void test_escape_newline_tab(void) {
    printf("a\tb\tc\n");
    // EXPECT: a	b	c
}

void test_escape_backslash(void) {
    printf("back\\slash\n");
    // EXPECT: back\slash
}

void test_escape_quote(void) {
    printf("say \"hello\"\n");
    // EXPECT: say "hello"
}

void test_escape_single_quote(void) {
    char c = '\'';
    printf("char=%c val=%d\n", c, c);
    // EXPECT: char=' val=39
}

void test_escape_chars_value(void) {
    // Test that \r and \n have correct numeric values
    char cr = '\r';
    char nl = '\n';
    char tab = '\t';
    printf("cr=%d nl=%d tab=%d\n", cr, nl, tab);
    // EXPECT: cr=13 nl=10 tab=9
}

void test_escape_hex_oct(void) {
    char h = '\x41';
    char o = '\101';
    printf("%c %c\n", h, o);
    // EXPECT: A A
}

void test_null_in_array(void) {
    char arr[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    printf("%s\n", arr);
    // EXPECT: hello
}

void test_empty_string(void) {
    const char *s = "";
    printf("empty len=%d\n", my_strlen(s));
    // EXPECT: empty len=0
    printf("empty is null=%d\n", s[0] == '\0');
    // EXPECT: empty is null=1
}

void test_string_as_array_init(void) {
    char buf[] = "Hello, World!";
    printf("%s len=%d\n", buf, my_strlen(buf));
    // EXPECT: Hello, World! len=13
    buf[0] = 'h';
    buf[7] = 'w';
    printf("%s\n", buf);
    // EXPECT: hello, world!
}

void test_pointer_to_literal(void) {
    const char *p = "pointer test";
    printf("%c%c%c\n", p[0], p[5], p[8]);
    // EXPECT: pet
}

void test_byte_comparison(void) {
    const char *a = "abc";
    const char *b = "abd";
    const char *c = "abc";
    printf("ab<ad=%d\n", my_strcmp(a, b) < 0);
    printf("ab==ab=%d\n", my_strcmp(a, c) == 0);
    printf("ad>ab=%d\n", my_strcmp(b, a) > 0);
    // EXPECT: ab<ad=1
    // EXPECT: ab==ab=1
    // EXPECT: ad>ab=1
}

void test_string_indexing(void) {
    const char *s = "0123456789";
    int sum = 0;
    int i;
    for (i = 0; i < 10; i++) {
        sum = sum + (s[i] - '0');
    }
    printf("digit sum=%d\n", sum);
    // EXPECT: digit sum=45
}

void test_char_array_ops(void) {
    char buf[50];
    const char *src = "copy this";
    int i = 0;
    while (src[i]) {
        buf[i] = src[i];
        i++;
    }
    buf[i] = 0;
    printf("copied: %s\n", buf);
    // EXPECT: copied: copy this
}

void test_string_in_struct(void) {
    struct {
        char name[20];
        int value;
    } item;
    const char *src = "test_item";
    int i = 0;
    while (src[i]) {
        item.name[i] = src[i];
        i++;
    }
    item.name[i] = 0;
    item.value = 42;
    printf("%s=%d\n", item.name, item.value);
    // EXPECT: test_item=42
}

void test_many_concats(void) {
    const char *s = "a" "b" "c" "d" "e" "f" "g" "h" "i" "j"
                    "k" "l" "m" "n" "o" "p" "q" "r" "s" "t"
                    "u" "v" "w" "x" "y" "z";
    printf("%s\n", s);
    // EXPECT: abcdefghijklmnopqrstuvwxyz
    printf("len=%d\n", my_strlen(s));
    // EXPECT: len=26
}

void test_escape_zero_mid(void) {
    char buf[10];
    buf[0] = 'A';
    buf[1] = '\0';
    buf[2] = 'B';
    buf[3] = '\0';
    printf("before null=%c\n", buf[0]);
    printf("after null=%c\n", buf[2]);
    printf("strlen=%d\n", my_strlen(buf));
    // EXPECT: before null=A
    // EXPECT: after null=B
    // EXPECT: strlen=1
}

int main(void) {
    test_adjacent_concat();
    test_multiline_concat();
    test_long_string();
    test_escape_newline_tab();
    test_escape_backslash();
    test_escape_quote();
    test_escape_single_quote();
    test_escape_chars_value();
    test_escape_hex_oct();
    test_null_in_array();
    test_empty_string();
    test_string_as_array_init();
    test_pointer_to_literal();
    test_byte_comparison();
    test_string_indexing();
    test_char_array_ops();
    test_string_in_struct();
    test_many_concats();
    test_escape_zero_mid();
    return 0;
}
