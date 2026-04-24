int printf(const char *fmt, ...);

int global_int = 1000;
char global_char = 65;

int char_to_int(char c) {
    return (int)c;
}

int main(void) {
    // char + int promotion
    char c = 10;
    int i = 20;
    int result = c + i;
    // EXPECT: 30
    printf("%d\n", result);

    // char * int
    char c2 = 5;
    int prod = c2 * 6;
    // EXPECT: 30
    printf("%d\n", prod);

    // char - char yields int
    char a = 100;
    char b = 30;
    int diff = a - b;
    // EXPECT: 70
    printf("%d\n", diff);

    // char in comparison with int
    char ch = 65;
    int cmp = (ch == 65);
    // EXPECT: 1
    printf("%d\n", cmp);

    // sizeof int
    int sz_int = sizeof(int);
    // EXPECT: 4
    printf("%d\n", sz_int);

    // sizeof char
    int sz_char = sizeof(char);
    // EXPECT: 1
    printf("%d\n", sz_char);

    // sizeof pointer
    int x = 0;
    int *px = &x;
    int sz_ptr = sizeof(px);
    // On ARM64 (Apple Silicon), pointers are 8 bytes
    // EXPECT: 8
    printf("%d\n", sz_ptr);

    // sizeof in expression
    int arr[10];
    int num_elements = sizeof(arr) / sizeof(arr[0]);
    // 10 * 4 / 4 = 10
    // EXPECT: 10
    printf("%d\n", num_elements);

    // sizeof struct
    struct pair { int first; int second; };
    int sz_pair = sizeof(struct pair);
    // two ints = 8
    // EXPECT: 8
    printf("%d\n", sz_pair);

    // Pointer arithmetic: pointer + int
    int parr[5];
    parr[0] = 100;
    parr[1] = 200;
    parr[2] = 300;
    parr[3] = 400;
    parr[4] = 500;
    int *pp = parr;
    int val_at_2 = *(pp + 2);
    // EXPECT: 300
    printf("%d\n", val_at_2);

    // int + pointer (commutative)
    int val_at_3 = *(3 + pp);
    // EXPECT: 400
    printf("%d\n", val_at_3);

    // Pointer subtraction gives element count
    int *p_start = &parr[0];
    int *p_end = &parr[4];
    int pdiff = (int)(p_end - p_start);
    // EXPECT: 4
    printf("%d\n", pdiff);

    // char arithmetic with overflow
    char overflow_c = 120;
    char overflow_r = overflow_c + 20;
    // 140 as signed char = 140 - 256 = -116
    // EXPECT: -116
    printf("%d\n", (int)overflow_r);

    // Mixing char and int in array
    char chars[4];
    chars[0] = 1;
    chars[1] = 2;
    chars[2] = 3;
    chars[3] = 4;
    int isum = 0;
    for (int j = 0; j < 4; j++) {
        isum = isum + chars[j];
    }
    // EXPECT: 10
    printf("%d\n", isum);

    // sizeof expression (not type)
    int expr_sz = sizeof(1 + 2);
    // result of int + int is int = 4
    // EXPECT: 4
    printf("%d\n", expr_sz);

    // sizeof char expression
    char c3 = 5;
    int csz = sizeof(c3);
    // sizeof variable of type char = 1
    // EXPECT: 1
    printf("%d\n", csz);

    // Mixed: sizeof in arithmetic
    int bytes_for_5 = 5 * sizeof(int);
    // EXPECT: 20
    printf("%d\n", bytes_for_5);

    // Char used as boolean
    char flag = 1;
    int bval = flag ? 100 : 200;
    // EXPECT: 100
    printf("%d\n", bval);

    char zero_flag = 0;
    int bval2 = zero_flag ? 100 : 200;
    // EXPECT: 200
    printf("%d\n", bval2);

    // Global char in expression
    int gresult = global_char + global_int;
    // 65 + 1000 = 1065
    // EXPECT: 1065
    printf("%d\n", gresult);

    // Function converting char to int
    char fc = 42;
    int fi = char_to_int(fc);
    // EXPECT: 42
    printf("%d\n", fi);

    // Pointer to char, dereference in int expression
    char pch_val = 50;
    char *pch = &pch_val;
    int pch_result = *pch + 100;
    // EXPECT: 150
    printf("%d\n", pch_result);

    // sizeof array of chars
    char str[] = "hello";
    int str_sz = sizeof(str);
    // "hello" + null = 6
    // EXPECT: 6
    printf("%d\n", str_sz);

    // Enum in mixed expression
    enum color { RED, GREEN, BLUE };
    enum color clr = BLUE;
    int enum_val = clr + 10;
    // BLUE = 2, so 12
    // EXPECT: 12
    printf("%d\n", enum_val);

    // Sizeof array divided to get count
    int big_arr[20];
    int big_count = sizeof(big_arr) / sizeof(int);
    // EXPECT: 20
    printf("%d\n", big_count);

    // char negation promoted to int
    char neg_c = 5;
    int neg_result = -neg_c;
    // EXPECT: -5
    printf("%d\n", neg_result);

    // Mixed: struct member (char) + int
    struct mixed { char c; int i; };
    struct mixed ms;
    ms.c = 10;
    ms.i = 90;
    int ms_sum = ms.c + ms.i;
    // EXPECT: 100
    printf("%d\n", ms_sum);

    return 0;
}
