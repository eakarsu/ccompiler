// stress_063_bool_convert.c - _Bool conversion stress test
// EXPECT: bool_from_zero=0
// EXPECT: bool_from_one=1
// EXPECT: bool_from_neg=1
// EXPECT: bool_from_large=1
// EXPECT: bool_from_char0=0
// EXPECT: bool_from_charA=1
// EXPECT: bool_from_short0=0
// EXPECT: bool_from_short1=1
// EXPECT: bool_from_long0=0
// EXPECT: bool_from_long1=1
// EXPECT: bool_from_uint0=0
// EXPECT: bool_from_uint1=1
// EXPECT: bool_from_ulong=1
// EXPECT: bool_from_ptr_null=0
// EXPECT: bool_from_ptr_valid=1
// EXPECT: bool_arith_add=2
// EXPECT: bool_arith_mul=0
// EXPECT: bool_arith_sub=1
// EXPECT: bool_in_cond=yes
// EXPECT: bool_assign_chain=1
// EXPECT: bool_double_neg=1
// EXPECT: bool_bitwise_and=0
// EXPECT: bool_bitwise_or=1
// EXPECT: bool_array=1 0 1 0 1
// EXPECT: bool_struct_fields=1 0 1
// EXPECT: bool_struct_sum=2
// EXPECT: bool_func_ret_true=1
// EXPECT: bool_func_ret_false=0
// EXPECT: bool_from_expr=1
// EXPECT: bool_from_compare=1
// EXPECT: bool_normalize=1
// EXPECT: bool_ternary=10
// EXPECT: bool_loop_count=5
// EXPECT: bool_switch=one
// EXPECT: bool_sizeof=1
// EXPECT: bool_cast_back=1
// EXPECT: bool_logical_and=1
// EXPECT: bool_logical_or=1
// EXPECT: bool_logical_not=0

int printf(const char *fmt, ...);

// _Bool from int values
void test_bool_from_int(void) {
    _Bool b0 = 0;
    _Bool b1 = 1;
    _Bool bn = -42;
    _Bool bl = 999999;
    printf("bool_from_zero=%d\n", b0);
    printf("bool_from_one=%d\n", b1);
    printf("bool_from_neg=%d\n", bn);
    printf("bool_from_large=%d\n", bl);
}

// _Bool from char
void test_bool_from_char(void) {
    char c0 = 0;
    char cA = 'A';
    _Bool b0 = c0;
    _Bool bA = cA;
    printf("bool_from_char0=%d\n", b0);
    printf("bool_from_charA=%d\n", bA);
}

// _Bool from short
void test_bool_from_short(void) {
    short s0 = 0;
    short s1 = 32767;
    _Bool b0 = s0;
    _Bool b1 = s1;
    printf("bool_from_short0=%d\n", b0);
    printf("bool_from_short1=%d\n", b1);
}

// _Bool from long
void test_bool_from_long(void) {
    long l0 = 0;
    long l1 = 9999999999L;
    _Bool b0 = l0;
    _Bool b1 = l1;
    printf("bool_from_long0=%d\n", b0);
    printf("bool_from_long1=%d\n", b1);
}

// _Bool from unsigned types
void test_bool_from_unsigned(void) {
    unsigned int u0 = 0;
    unsigned int u1 = 4294967295U;
    unsigned long ul = 1UL;
    _Bool b0 = u0;
    _Bool b1 = u1;
    _Bool bul = ul;
    printf("bool_from_uint0=%d\n", b0);
    printf("bool_from_uint1=%d\n", b1);
    printf("bool_from_ulong=%d\n", bul);
}

// _Bool from pointer
void test_bool_from_ptr(void) {
    int x = 42;
    int *null_ptr = 0;
    int *valid_ptr = &x;
    _Bool b_null = (_Bool)(unsigned long)null_ptr;
    _Bool b_valid = (_Bool)(unsigned long)valid_ptr;
    printf("bool_from_ptr_null=%d\n", b_null);
    printf("bool_from_ptr_valid=%d\n", b_valid);
}

// _Bool arithmetic (promotes to int)
void test_bool_arithmetic(void) {
    _Bool a = 1;
    _Bool b = 1;
    _Bool c = 0;
    int sum = a + b;        // 1 + 1 = 2
    int product = a * c;    // 1 * 0 = 0
    int diff = a - c;       // 1 - 0 = 1
    printf("bool_arith_add=%d\n", sum);
    printf("bool_arith_mul=%d\n", product);
    printf("bool_arith_sub=%d\n", diff);
}

// _Bool in conditional
void test_bool_in_cond(void) {
    _Bool flag = 42; // becomes 1
    if (flag) {
        printf("bool_in_cond=yes\n");
    } else {
        printf("bool_in_cond=no\n");
    }
}

// Assignment chain
void test_bool_assign_chain(void) {
    _Bool a = 255; // becomes 1
    _Bool b = a;
    _Bool c = b;
    printf("bool_assign_chain=%d\n", c);
}

// Double negation
void test_bool_double_neg(void) {
    _Bool val = 42; // becomes 1
    _Bool neg1 = !val; // 0
    _Bool neg2 = !neg1; // 1
    printf("bool_double_neg=%d\n", neg2);
}

// Bitwise operations on _Bool (promoted to int)
void test_bool_bitwise(void) {
    _Bool a = 1;
    _Bool b = 0;
    int and_result = a & b;
    int or_result = a | b;
    printf("bool_bitwise_and=%d\n", and_result);
    printf("bool_bitwise_or=%d\n", or_result);
}

// Array of _Bool
void test_bool_array(void) {
    _Bool arr[5];
    arr[0] = 100;   // 1
    arr[1] = 0;     // 0
    arr[2] = -1;    // 1
    arr[3] = 0;     // 0
    arr[4] = 1;     // 1
    printf("bool_array=");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Struct with _Bool fields
struct BoolStruct {
    _Bool flag1;
    _Bool flag2;
    _Bool flag3;
    int value;
};

void test_bool_struct(void) {
    struct BoolStruct bs;
    bs.flag1 = 999;   // 1
    bs.flag2 = 0;     // 0
    bs.flag3 = -100;  // 1
    bs.value = 42;
    printf("bool_struct_fields=%d %d %d\n", bs.flag1, bs.flag2, bs.flag3);
    int sum = bs.flag1 + bs.flag2 + bs.flag3;
    printf("bool_struct_sum=%d\n", sum);
}

// _Bool as function return
_Bool is_positive(int x) {
    return x > 0;
}

_Bool is_zero(int x) {
    return x == 0;
}

void test_bool_func_ret(void) {
    printf("bool_func_ret_true=%d\n", is_positive(42));
    printf("bool_func_ret_false=%d\n", is_positive(-5));
}

// _Bool from complex expression
void test_bool_from_expr(void) {
    int a = 10, b = 20, c = 30;
    _Bool result = (a + b) * c - 100; // 900-100=800, nonzero -> 1
    printf("bool_from_expr=%d\n", result);
}

// _Bool from comparison
void test_bool_from_compare(void) {
    int x = 5, y = 10;
    _Bool less = x < y;
    printf("bool_from_compare=%d\n", less);
}

// Normalizing any integer to 0/1 via _Bool
void test_bool_normalize(void) {
    int big = 1000000;
    _Bool normalized = big;
    printf("bool_normalize=%d\n", normalized);
}

// _Bool in ternary
void test_bool_ternary(void) {
    _Bool cond = 42; // becomes 1
    int result = cond ? 10 : 20;
    printf("bool_ternary=%d\n", result);
}

// _Bool in loop
void test_bool_loop(void) {
    _Bool flags[10];
    int i;
    for (i = 0; i < 10; i++) {
        flags[i] = (i % 2 == 0); // alternating 1,0,1,0,...
    }
    int count = 0;
    for (i = 0; i < 10; i++) {
        count += flags[i];
    }
    printf("bool_loop_count=%d\n", count);
}

// _Bool in switch
void test_bool_switch(void) {
    _Bool val = 1;
    switch (val) {
        case 0: printf("bool_switch=zero\n"); break;
        case 1: printf("bool_switch=one\n"); break;
    }
}

// sizeof _Bool
void test_bool_sizeof(void) {
    printf("bool_sizeof=%d\n", (int)sizeof(_Bool));
}

// Cast _Bool back to int
void test_bool_cast_back(void) {
    _Bool b = 42; // becomes 1
    int i = (int)b;
    printf("bool_cast_back=%d\n", i);
}

// _Bool with logical operators
void test_bool_logical(void) {
    _Bool a = 1;
    _Bool b = 1;
    _Bool c = 0;
    _Bool and_result = a && b;
    _Bool or_result = b || c;
    _Bool not_result = !a;
    printf("bool_logical_and=%d\n", and_result);
    printf("bool_logical_or=%d\n", or_result);
    printf("bool_logical_not=%d\n", not_result);
}

int main(void) {
    test_bool_from_int();
    test_bool_from_char();
    test_bool_from_short();
    test_bool_from_long();
    test_bool_from_unsigned();
    test_bool_from_ptr();
    test_bool_arithmetic();
    test_bool_in_cond();
    test_bool_assign_chain();
    test_bool_double_neg();
    test_bool_bitwise();
    test_bool_array();
    test_bool_struct();
    test_bool_func_ret();
    test_bool_from_expr();
    test_bool_from_compare();
    test_bool_normalize();
    test_bool_ternary();
    test_bool_loop();
    test_bool_switch();
    test_bool_sizeof();
    test_bool_cast_back();
    test_bool_logical();
    return 0;
}
