int printf(const char *fmt, ...);
// EXPECT: === Integer Arithmetic Tests ===
// EXPECT: add_basic: 3+5=8
// EXPECT: add_neg: -3+5=2
// EXPECT: add_neg2: -3+-5=-8
// EXPECT: sub_basic: 10-3=7
// EXPECT: sub_neg: -10-3=-13
// EXPECT: sub_to_neg: 3-10=-7
// EXPECT: mul_basic: 6*7=42
// EXPECT: mul_neg: -6*7=-42
// EXPECT: mul_neg2: -6*-7=42
// EXPECT: mul_zero: 99*0=0
// EXPECT: div_basic: 100/4=25
// EXPECT: div_trunc: 7/2=3
// EXPECT: div_neg: -7/2=-3
// EXPECT: div_neg2: -7/-2=3
// EXPECT: mod_basic: 17%5=2
// EXPECT: mod_neg: -17%5=-2
// EXPECT: mod_one: 99%1=0
// EXPECT: chain_add: 1+2+3+4+5+6+7+8+9+10=55
// EXPECT: chain_mul: 1*2*3*4*5=120
// EXPECT: chain_sub: 100-10-20-30-5=35
// EXPECT: chain_div: 10000/10/10/10=10
// EXPECT: mixed1: 2+3*4=14
// EXPECT: mixed2: (2+3)*4=20
// EXPECT: mixed3: 10-2*3+1=5
// EXPECT: mixed4: (10-2)*(3+1)=32
// EXPECT: mixed5: 100/5+3*2-1=25
// EXPECT: char_add: 65+1=66
// EXPECT: char_arith: a+3=d
// EXPECT: short_mul: 200*3=600
// EXPECT: long_add: 100000+200000=300000
// EXPECT: overflow_wrap: 2147483647+1=-2147483648
// EXPECT: neg_zero: 0-0=0
// EXPECT: multi_parens: ((2+3)*(4+1))=25
// EXPECT: complex1: (100-50)/(5+5)*3=15
// EXPECT: complex2: 1+2*3+4*5+6*7=69
// add_basic: 3+5=8
// add_neg: -3+5=2
// add_neg2: -3+-5=-8
// sub_basic: 10-3=7
// sub_neg: -10-3=-13
// sub_to_neg: 3-10=-7
// mul_basic: 6*7=42
// mul_neg: -6*7=-42
// mul_neg2: -6*-7=42
// mul_zero: 99*0=0
// div_basic: 100/4=25
// div_trunc: 7/2=3
// div_neg: -7/2=-3
// div_neg2: -7/-2=3
// mod_basic: 17%5=2
// mod_neg: -17%5=-2
// mod_one: 99%1=0
// chain_add: 1+2+3+4+5+6+7+8+9+10=55
// chain_mul: 1*2*3*4*5=120
// chain_sub: 100-10-20-30-5=35
// chain_div: 10000/10/10/10=10
// mixed1: 2+3*4=14
// mixed2: (2+3)*4=20
// mixed3: 10-2*3+1=5
// mixed4: (10-2)*(3+1)=32
// mixed5: 100/5+3*2-1=25
// char_add: 65+1=66
// char_arith: a+3=d
// short_mul: 200*3=600
// long_add: 100000+200000=300000
// overflow_wrap: 2147483647+1=-2147483648
// neg_zero: 0-0=0
// multi_parens: ((2+3)*(4+1))=25
// complex1: (100-50)/(5+5)*3=15
// complex2: 1+2*3+4*5+6*7=69

int main(void) {
    int a, b, c, r;
    char ch;
    short s1, s2;
    long l1, l2;

    printf("=== Integer Arithmetic Tests ===\n");

    // Basic addition
    a = 3; b = 5;
    printf("add_basic: 3+5=%d\n", a + b);

    // Addition with negatives
    a = -3; b = 5;
    printf("add_neg: -3+5=%d\n", a + b);
    a = -3; b = -5;
    printf("add_neg2: -3+-5=%d\n", a + b);

    // Subtraction
    a = 10; b = 3;
    printf("sub_basic: 10-3=%d\n", a - b);
    a = -10; b = 3;
    printf("sub_neg: -10-3=%d\n", a - b);
    a = 3; b = 10;
    printf("sub_to_neg: 3-10=%d\n", a - b);

    // Multiplication
    a = 6; b = 7;
    printf("mul_basic: 6*7=%d\n", a * b);
    a = -6; b = 7;
    printf("mul_neg: -6*7=%d\n", a * b);
    a = -6; b = -7;
    printf("mul_neg2: -6*-7=%d\n", a * b);
    a = 99; b = 0;
    printf("mul_zero: 99*0=%d\n", a * b);

    // Division
    a = 100; b = 4;
    printf("div_basic: 100/4=%d\n", a / b);
    a = 7; b = 2;
    printf("div_trunc: 7/2=%d\n", a / b);
    a = -7; b = 2;
    printf("div_neg: -7/2=%d\n", a / b);
    a = -7; b = -2;
    printf("div_neg2: -7/-2=%d\n", a / b);

    // Modulo
    a = 17; b = 5;
    printf("mod_basic: 17%%5=%d\n", a % b);
    a = -17; b = 5;
    printf("mod_neg: -17%%5=%d\n", a % b);
    a = 99; b = 1;
    printf("mod_one: 99%%1=%d\n", a % b);

    // Chained operations
    r = 1+2+3+4+5+6+7+8+9+10;
    printf("chain_add: 1+2+3+4+5+6+7+8+9+10=%d\n", r);
    r = 1*2*3*4*5;
    printf("chain_mul: 1*2*3*4*5=%d\n", r);
    r = 100-10-20-30-5;
    printf("chain_sub: 100-10-20-30-5=%d\n", r);
    r = 10000/10/10/10;
    printf("chain_div: 10000/10/10/10=%d\n", r);

    // Mixed precedence
    r = 2+3*4;
    printf("mixed1: 2+3*4=%d\n", r);
    r = (2+3)*4;
    printf("mixed2: (2+3)*4=%d\n", r);
    r = 10-2*3+1;
    printf("mixed3: 10-2*3+1=%d\n", r);
    r = (10-2)*(3+1);
    printf("mixed4: (10-2)*(3+1)=%d\n", r);
    r = 100/5+3*2-1;
    printf("mixed5: 100/5+3*2-1=%d\n", r);

    // Char arithmetic
    ch = 65;
    printf("char_add: 65+1=%d\n", ch + 1);
    ch = 'a';
    printf("char_arith: a+3=%c\n", ch + 3);

    // Short arithmetic
    s1 = 200; s2 = 3;
    printf("short_mul: 200*3=%d\n", s1 * s2);

    // Long arithmetic
    l1 = 100000; l2 = 200000;
    printf("long_add: %ld+%ld=%ld\n", l1, l2, l1 + l2);

    // Overflow (signed int wraps in two's complement)
    a = 2147483647;
    r = a + 1;
    printf("overflow_wrap: 2147483647+1=%d\n", r);

    // Zero subtraction
    printf("neg_zero: 0-0=%d\n", 0 - 0);

    // Nested parentheses
    r = ((2+3)*(4+1));
    printf("multi_parens: ((2+3)*(4+1))=%d\n", r);

    // Complex expressions
    r = (100-50)/(5+5)*3;
    printf("complex1: (100-50)/(5+5)*3=%d\n", r);
    r = 1+2*3+4*5+6*7;
    printf("complex2: 1+2*3+4*5+6*7=%d\n", r);

    return 0;
}
