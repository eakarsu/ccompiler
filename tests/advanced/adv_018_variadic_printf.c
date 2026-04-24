int printf(const char *fmt, ...);
// EXPECT: === Printf Format Tests ===
// EXPECT: int_pos: 42
// EXPECT: int_neg: -42
// EXPECT: int_zero: 0
// EXPECT: int_max: 2147483647
// EXPECT: int_min: -2147483648
// EXPECT: int_multi: 1 2 3 4 5
// EXPECT: hex_lower: 0xff
// EXPECT: hex_upper: 0xFF
// EXPECT: hex_zero: 0x0
// EXPECT: hex_num: 0x2a
// EXPECT: hex_large: 0xdeadbeef
// EXPECT: octal: 52
// EXPECT: octal_zero: 0
// EXPECT: octal_max: 17777777777
// EXPECT: char_A: A
// EXPECT: char_z: z
// EXPECT: char_zero: 0
// EXPECT: char_space: [ ]
// EXPECT: char_newline_after: before
// EXPECT: after
// EXPECT: string_hello: hello
// EXPECT: string_empty: []
// EXPECT: string_spaces: [  hi  ]
// EXPECT: percent: 100%
// EXPECT: percent_double: %%
// EXPECT: mixed_int_str: val=42 name=test
// EXPECT: mixed_char_int: ch=X num=99
// EXPECT: mixed_hex_dec: hex=0xff dec=255
// EXPECT: long_val: 1000000000
// EXPECT: negative_hex: 0xffffffd6
// EXPECT: multi_string: hello world foo
// EXPECT: digit_chars: 0123456789
// EXPECT: alpha_lower: abcdefghij
// EXPECT: alpha_upper: ABCDEFGHIJ
// EXPECT: int_width: [42]
// EXPECT: repeated: aaa bbb ccc
// EXPECT: big_expression: 1000000
// EXPECT: computed_hex: 0x64
// EXPECT: many_args: 1 2 3 4 5 6
// EXPECT: === All Printf Format Tests Passed ===

int main() {
    printf("=== Printf Format Tests ===\n");

    // basic integer formatting
    printf("int_pos: %d\n", 42);
    printf("int_neg: %d\n", -42);
    printf("int_zero: %d\n", 0);
    printf("int_max: %d\n", 2147483647);
    // -2147483648 expressed as computation to avoid literal issues
    int imin = -2147483647 - 1;
    printf("int_min: %d\n", imin);

    // multiple integers
    printf("int_multi: %d %d %d %d %d\n", 1, 2, 3, 4, 5);

    // hexadecimal formatting
    printf("hex_lower: 0x%x\n", 255);
    printf("hex_upper: 0x%X\n", 255);
    printf("hex_zero: 0x%x\n", 0);
    printf("hex_num: 0x%x\n", 42);
    // 0xdeadbeef as unsigned
    unsigned int deadbeef = 3735928559u;
    printf("hex_large: 0x%x\n", deadbeef);

    // octal formatting
    printf("octal: %o\n", 42);
    printf("octal_zero: %o\n", 0);
    printf("octal_max: %o\n", 2147483647);

    // character formatting
    printf("char_A: %c\n", 'A');
    printf("char_z: %c\n", 'z');
    printf("char_zero: %c\n", '0');
    printf("char_space: [%c]\n", ' ');

    // newline within output
    printf("char_newline_after: before\n");
    printf("after\n");

    // string formatting
    printf("string_hello: %s\n", "hello");
    printf("string_empty: [%s]\n", "");
    printf("string_spaces: [%s]\n", "  hi  ");

    // percent literal
    printf("percent: 100%%\n");
    printf("percent_double: %%%%\n");

    // mixed format specifiers
    printf("mixed_int_str: val=%d name=%s\n", 42, "test");
    printf("mixed_char_int: ch=%c num=%d\n", 'X', 99);
    printf("mixed_hex_dec: hex=0x%x dec=%d\n", 255, 255);

    // long value
    long lv = 1000000000;
    printf("long_val: %ld\n", lv);

    // negative number in hex (two's complement)
    printf("negative_hex: 0x%x\n", -42);

    // multiple strings
    printf("multi_string: %s %s %s\n", "hello", "world", "foo");

    // digit characters via loop
    printf("digit_chars: ");
    int i;
    for (i = 0; i < 10; i++) {
        printf("%c", '0' + i);
    }
    printf("\n");

    // lowercase alphabet subset
    printf("alpha_lower: ");
    for (i = 0; i < 10; i++) {
        printf("%c", 'a' + i);
    }
    printf("\n");

    // uppercase alphabet subset
    printf("alpha_upper: ");
    for (i = 0; i < 10; i++) {
        printf("%c", 'A' + i);
    }
    printf("\n");

    // integer in brackets (simulated width)
    printf("int_width: [%d]\n", 42);

    // repeated values
    printf("repeated: %s %s %s\n", "aaa", "bbb", "ccc");

    // expression as argument
    printf("big_expression: %d\n", 1000 * 1000);

    // computed hex value
    printf("computed_hex: 0x%x\n", 100);

    // many arguments
    printf("many_args: %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);

    printf("=== All Printf Format Tests Passed ===\n");
    return 0;
}
