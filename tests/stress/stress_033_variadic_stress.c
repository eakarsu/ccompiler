// stress_033_variadic_stress.c - Variadic function stress (printf)
// EXPECT: eight_args=1 2 3 4 5 6 7 8
// EXPECT: ten_args=10 20 30 40 50 60 70 80 90 100
// EXPECT: mixed=42 A hello 99
// EXPECT: hex_ff=ff
// EXPECT: hex_dead=dead
// EXPECT: oct_8=10
// EXPECT: oct_255=377
// EXPECT: 12345
// EXPECT: nested_len=5
// EXPECT: abc
// EXPECT: nested_chain=3
// EXPECT: loop_0=item0
// EXPECT: loop_1=item1
// EXPECT: loop_2=item2
// EXPECT: loop_3=item3
// EXPECT: loop_4=item4
// EXPECT: computed_0=val=0
// EXPECT: computed_1=val=10
// EXPECT: computed_2=val=20
// EXPECT: computed_3=val=30
// EXPECT: ptr_print=ptr_ok
// EXPECT: percent_literal=100%
// EXPECT: multi_string=abc def ghi
// EXPECT: zero_pad=0042
// EXPECT: big_num=1000000
// EXPECT: negative=-42
// EXPECT: char_print=X
// EXPECT: string_null=(null_safe)
// EXPECT: expr_in_arg=150
// EXPECT: ternary_fmt=yes
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

int compute(int a, int b) {
    return a * 10 + b;
}

int identity(int x) {
    return x;
}

int main(void) {
    // 8 arguments
    printf("eight_args=%d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);

    // 10 arguments
    printf("ten_args=%d %d %d %d %d %d %d %d %d %d\n",
           10, 20, 30, 40, 50, 60, 70, 80, 90, 100);

    // Mixed types: int, char, string, int
    printf("mixed=%d %c %s %d\n", 42, 65, "hello", 99);

    // Hex format
    printf("hex_ff=%x\n", 255);
    printf("hex_dead=%x\n", 0xdead);

    // Octal format
    printf("oct_8=%o\n", 8);
    printf("oct_255=%o\n", 255);

    // Nested printf: printf returns number of chars written
    int n = printf("12345");
    printf("\n");
    printf("nested_len=%d\n", n);

    // Chain: use printf return in expression
    int r = printf("abc");
    printf("\n");
    printf("nested_chain=%d\n", r);

    // Printf in loop
    int i;
    for (i = 0; i < 5; i++) {
        printf("loop_%d=item%d\n", i, i);
    }

    // Computed format strings via conditional selection
    for (i = 0; i < 4; i++) {
        printf("computed_%d=val=%d\n", i, i * 10);
    }

    // Printing pointer-related (just a string that says ptr_ok)
    int dummy = 42;
    int *p = &dummy;
    // We don't print actual address, just verify pointer is non-null
    if (p) printf("ptr_print=ptr_ok\n");

    // Percent literal
    printf("percent_literal=100%%\n");

    // Multiple string arguments
    printf("multi_string=%s %s %s\n", "abc", "def", "ghi");

    // Zero-padded number
    printf("zero_pad=%04d\n", 42);

    // Large number
    printf("big_num=%d\n", 1000000);

    // Negative number
    printf("negative=%d\n", -42);

    // Char print
    printf("char_print=%c\n", 88);

    // Null-safe string (don't actually pass NULL, just show safe behavior)
    printf("string_null=(null_safe)\n");

    // Expression in argument
    printf("expr_in_arg=%d\n", compute(14, 10) + identity(0));

    // Ternary in format selection
    int cond = 1;
    printf("ternary_fmt=%s\n", cond ? "yes" : "no");

    printf("pass_done=1\n");
    return 0;
}
