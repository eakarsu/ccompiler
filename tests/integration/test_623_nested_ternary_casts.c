int printf(const char *fmt, ...);
// EXPECT: max(5,3)=5\nmax3(5,8,3)=8\nmax3(1,2,9)=9\nmax3(7,4,2)=7\nclamp(15,0,10)=10\nclamp(-5,0,10)=0\nclamp(5,0,10)=5\nabs(5)=5\nabs(-3)=3\nabs(0)=0\nside1: a=10\nside0: a=20\nsizeof(int)=4\nsizeof(char)=1\nsizeof(S)=8\nsizeof(int)+sizeof(char)=5\nsizeof(int)*2=8\nsizeof(S)-sizeof(int)=4\ncast200=-56\ncast65=65\ncastm1=-1\ncast128=-128\ncasttern1=65\ncasttern0=0\narrsum=28\nexpr1=7\nexpr0=12\nchain1=10\nchain2=20\nchain3=30\nchain4=0\nptern1: a=100 b=5\nptern0: a=10 b=100\narridx1=30\narridx0=50\nlooptern=2\nsztern1=4\nsztern0=1\ncastmath1=44\ncastmath2=1\ngrade95=65\ngrade85=66\ngrade75=67\ngrade65=68\ngrade55=70\nsfield=42\nmulti=10\nmulti2=1\nszcomp=1

// Test 623: Deeply nested ternary operators, cast chains, sizeof exprs

struct Padded {
    int x;
    char c;
};

// Test 1: Simple ternary (max of two)
void test_simple_ternary(void) {
    int a = 5;
    int b = 3;
    int mx = (a > b) ? a : b;
    printf("max(%d,%d)=%d\n", a, b, mx);
}

// Test 2: Nested ternary (max of three, 3 levels)
void test_max3(int a, int b, int c) {
    int mx = a > b ? (a > c ? a : c) : (b > c ? b : c);
    printf("max3(%d,%d,%d)=%d\n", a, b, c, mx);
}

// Test 3: Deeply nested ternary (4 levels) - clamp
void test_clamp(int val, int lo, int hi) {
    int clamped = val < lo ? lo : (val > hi ? hi : val);
    printf("clamp(%d,%d,%d)=%d\n", val, lo, hi, clamped);
}

// Test 4: Ternary in function argument
void test_ternary_in_arg(void) {
    int a;
    a = 5;
    printf("abs(%d)=%d\n", a, a > 0 ? a : -a);
    a = -3;
    printf("abs(%d)=%d\n", a, a > 0 ? a : -a);
    a = 0;
    printf("abs(%d)=%d\n", a, a > 0 ? a : -a);
}

// Test 5: Ternary with side effects (simplified - no comma operator)
void test_ternary_side_effects(void) {
    int flag;
    int a;
    flag = 1;
    a = flag ? 10 : 20;
    printf("side1: a=%d\n", a);
    flag = 0;
    a = flag ? 10 : 20;
    printf("side0: a=%d\n", a);
}

// Test 6: sizeof in arithmetic expressions
void test_sizeof_exprs(void) {
    printf("sizeof(int)=%d\n", (int)sizeof(int));
    printf("sizeof(char)=%d\n", (int)sizeof(char));
    struct Padded s;
    printf("sizeof(S)=%d\n", (int)sizeof(struct Padded));
    int sum = (int)sizeof(int) + (int)sizeof(char);
    printf("sizeof(int)+sizeof(char)=%d\n", sum);
    int prod = (int)sizeof(int) * 2;
    printf("sizeof(int)*2=%d\n", prod);
    int diff = (int)sizeof(struct Padded) - (int)sizeof(int);
    printf("sizeof(S)-sizeof(int)=%d\n", diff);
}

// Test 7: Cast chains - (int)(char)(int)value
void test_cast_chains(void) {
    // 200 = 0xC8 -> signed char = -56 -> sign-extend to int = -56
    int v1 = 200;
    int r1 = (int)(char)(int)v1;
    printf("cast200=%d\n", r1);
    // 65 fits in signed char range, no truncation
    int v2 = 65;
    int r2 = (int)(char)(int)v2;
    printf("cast65=%d\n", r2);
    // -1 -> char 0xFF = -1 signed -> int -1
    int v3 = -1;
    int r3 = (int)(char)(int)v3;
    printf("castm1=%d\n", r3);
    // 128 = 0x80 -> signed char = -128 -> int -128
    int v4 = 128;
    int r4 = (int)(char)(int)v4;
    printf("cast128=%d\n", r4);
}

// Test 8: Cast in ternary
void test_cast_in_ternary(void) {
    char c = 65;
    int flag;
    flag = 1;
    int x1 = flag ? (int)c : 0;
    printf("casttern1=%d\n", x1);
    flag = 0;
    int x2 = flag ? (int)c : 0;
    printf("casttern0=%d\n", x2);
}

// Test 9: sizeof used as array size
void test_sizeof_array_size(void) {
    int arr[sizeof(int) * 2];  // 8 elements
    int i = 0;
    while (i < 8) {
        arr[i] = i;
        i = i + 1;
    }
    int sum = 0;
    i = 0;
    while (i < 8) {
        sum = sum + arr[i];
        i = i + 1;
    }
    // 0+1+2+3+4+5+6+7 = 28
    printf("arrsum=%d\n", sum);
}

// Test 10: Ternary returning different expressions
void test_ternary_diff_expr(void) {
    int a = 3;
    int b = 4;
    int flag;
    flag = 1;
    int r1 = flag ? a + b : a * b;
    printf("expr1=%d\n", r1);
    flag = 0;
    int r2 = flag ? a + b : a * b;
    printf("expr0=%d\n", r2);
}

// Test 11: Chained ternary for multi-way branch
void test_chained_ternary(int x, int label) {
    int result = x == 1 ? 10 : x == 2 ? 20 : x == 3 ? 30 : 0;
    printf("chain%d=%d\n", label, result);
}

// Test 12: Ternary with pointer
void test_ternary_pointer(void) {
    int a;
    int b;
    int *p;
    // flag=1: p -> a
    a = 10;
    b = 5;
    int flag = 1;
    p = flag ? &a : &b;
    *p = 100;
    printf("ptern1: a=%d b=%d\n", a, b);
    // flag=0: p -> b
    a = 10;
    b = 5;
    flag = 0;
    p = flag ? &a : &b;
    *p = 100;
    printf("ptern0: a=%d b=%d\n", a, b);
}

// Test 13: Ternary in array index
void test_ternary_array_index(void) {
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int flag;
    flag = 1;
    int r1 = arr[flag ? 2 : 4];
    printf("arridx1=%d\n", r1);
    flag = 0;
    int r2 = arr[flag ? 2 : 4];
    printf("arridx0=%d\n", r2);
}

// Test 14: Ternary in loop
void test_ternary_loop(void) {
    int sum = 0;
    int i = 0;
    while (i < 5) {
        sum = sum + ((i % 2 == 0) ? i : -i);
        i = i + 1;
    }
    // 0 + (-1) + 2 + (-3) + 4 = 2
    printf("looptern=%d\n", sum);
}

// Test 15: sizeof in ternary
void test_sizeof_ternary(void) {
    int flag;
    flag = 1;
    int r1 = flag ? (int)sizeof(int) : (int)sizeof(char);
    printf("sztern1=%d\n", r1);
    flag = 0;
    int r2 = flag ? (int)sizeof(int) : (int)sizeof(char);
    printf("sztern0=%d\n", r2);
}

// Test 16: Nested cast with arithmetic
void test_nested_cast_math(void) {
    // (char)(44) = 44 (fits in signed char)
    int r1 = (int)((char)(300 - 256));
    printf("castmath1=%d\n", r1);
    // (char)(257) = (char)(257 & 0xFF) = (char)1 = 1
    int r2 = (int)((char)(256 + 1));
    printf("castmath2=%d\n", r2);
}

// Test 17: 5-level nested ternary (letter grade as ASCII)
void test_grade(int score) {
    int grade = score >= 90 ? 65
              : score >= 80 ? 66
              : score >= 70 ? 67
              : score >= 60 ? 68
              : 70;
    printf("grade%d=%d\n", score, grade);
}

// Test 18: Ternary assigning to struct member
void test_ternary_struct_member(void) {
    struct Padded s;
    int flag = 1;
    s.x = flag ? 42 : 99;
    printf("sfield=%d\n", s.x);
}

// Test 19: Multiple ternary sub-expressions combined
void test_multi_ternary(void) {
    int a;
    int b;
    a = 3;
    b = 7;
    int r1 = (a > 5 ? 1 : 0) + (b > 5 ? 10 : 0);
    printf("multi=%d\n", r1);
    a = 8;
    b = 2;
    int r2 = (a > 5 ? 1 : 0) + (b > 5 ? 10 : 0);
    printf("multi2=%d\n", r2);
}

// Test 20: sizeof comparison in ternary condition
void test_sizeof_comparison(void) {
    int r = (int)sizeof(int) > (int)sizeof(char) ? 1 : 0;
    printf("szcomp=%d\n", r);
}

int main(void) {
    test_simple_ternary();

    test_max3(5, 8, 3);
    test_max3(1, 2, 9);
    test_max3(7, 4, 2);

    test_clamp(15, 0, 10);
    test_clamp(-5, 0, 10);
    test_clamp(5, 0, 10);

    test_ternary_in_arg();
    test_ternary_side_effects();
    test_sizeof_exprs();
    test_cast_chains();
    test_cast_in_ternary();
    test_sizeof_array_size();
    test_ternary_diff_expr();

    test_chained_ternary(1, 1);
    test_chained_ternary(2, 2);
    test_chained_ternary(3, 3);
    test_chained_ternary(4, 4);

    test_ternary_pointer();
    test_ternary_array_index();
    test_ternary_loop();
    test_sizeof_ternary();
    test_nested_cast_math();

    test_grade(95);
    test_grade(85);
    test_grade(75);
    test_grade(65);
    test_grade(55);

    test_ternary_struct_member();
    test_multi_ternary();
    test_sizeof_comparison();

    return 0;
}
