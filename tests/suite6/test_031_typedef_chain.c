int printf(const char *fmt, ...);

typedef int MyInt;
typedef MyInt MyInt2;
typedef MyInt2 MyInt3;
typedef MyInt* MyIntPtr;
typedef MyInt3* MyInt3Ptr;
typedef MyInt** MyIntPtrPtr;

MyInt add_myint(MyInt a, MyInt b) {
    return a + b;
}

MyInt2 multiply_myint2(MyInt2 a, MyInt2 b) {
    return a * b;
}

MyInt3 negate_myint3(MyInt3 x) {
    return -x;
}

void swap_via_ptr(MyIntPtr a, MyIntPtr b) {
    MyInt tmp = *a;
    *a = *b;
    *b = tmp;
}

MyInt deref_and_add(MyInt3Ptr p, MyInt offset) {
    return *p + offset;
}

MyInt sum_array(MyIntPtr arr, MyInt len) {
    MyInt total = 0;
    MyInt i;
    for (i = 0; i < len; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

MyInt chain_ops(MyInt3 x) {
    MyInt2 doubled = x * 2;
    MyInt3 tripled = x * 3;
    MyInt result = doubled + tripled;
    return result;
}

void fill_array(MyIntPtr arr, MyInt len, MyInt val) {
    MyInt i;
    for (i = 0; i < len; i = i + 1) {
        arr[i] = val + i;
    }
}

MyInt max_myint(MyInt a, MyInt b) {
    if (a > b) return a;
    return b;
}

MyInt min_myint(MyInt a, MyInt b) {
    if (a < b) return a;
    return b;
}

MyInt abs_myint(MyInt3 x) {
    if (x < 0) return -x;
    return x;
}

MyInt factorial(MyInt2 n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main(void) {
    MyInt a = 10;
    MyInt2 b = 20;
    MyInt3 c = 30;

    // EXPECT: add: 30
    printf("add: %d\n", add_myint(a, b));

    // EXPECT: multiply: 200
    printf("multiply: %d\n", multiply_myint2(a, b));

    // EXPECT: negate: -30
    printf("negate: %d\n", negate_myint3(c));

    MyInt x = 5;
    MyInt y = 15;
    swap_via_ptr(&x, &y);
    // EXPECT: swap x: 15
    printf("swap x: %d\n", x);
    // EXPECT: swap y: 5
    printf("swap y: %d\n", y);

    MyInt3 val = 100;
    // EXPECT: deref_add: 142
    printf("deref_add: %d\n", deref_and_add(&val, 42));

    MyInt arr[5];
    fill_array(arr, 5, 10);
    // EXPECT: sum: 60
    printf("sum: %d\n", sum_array(arr, 5));

    // EXPECT: chain 7: 35
    printf("chain 7: %d\n", chain_ops(7));

    // EXPECT: chain 10: 50
    printf("chain 10: %d\n", chain_ops(10));

    // EXPECT: max: 20
    printf("max: %d\n", max_myint(a, b));

    // EXPECT: min: 10
    printf("min: %d\n", min_myint(a, b));

    // EXPECT: abs -7: 7
    printf("abs -7: %d\n", abs_myint(-7));

    // EXPECT: abs 3: 3
    printf("abs 3: %d\n", abs_myint(3));

    // EXPECT: fact 5: 120
    printf("fact 5: %d\n", factorial(5));

    // EXPECT: fact 6: 720
    printf("fact 6: %d\n", factorial(6));

    MyInt pp_val = 99;
    MyIntPtr pp_ptr = &pp_val;
    MyIntPtrPtr pp = &pp_ptr;
    // EXPECT: ptrptr: 99
    printf("ptrptr: %d\n", **pp);

    **pp = 77;
    // EXPECT: modified: 77
    printf("modified: %d\n", pp_val);

    MyInt3 chain_val = 0;
    chain_val = add_myint(1, 2);
    chain_val = multiply_myint2(chain_val, 4);
    chain_val = negate_myint3(chain_val);
    // EXPECT: chained result: -12
    printf("chained result: %d\n", chain_val);

    return 0;
}
