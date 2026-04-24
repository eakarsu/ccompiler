int printf(const char *fmt, ...);

typedef int MyInt;
typedef char MyChar;
typedef int *IntPtr;
typedef char *String;
typedef int IntArray[5];

MyInt global_val = 42;

MyInt add(MyInt a, MyInt b) {
    return a + b;
}

MyInt multiply(MyInt a, MyInt b) {
    return a * b;
}

MyInt test_basic_typedef(void) {
    MyInt x = 10;
    MyInt y = 20;
    return x + y; // 30
}

MyInt test_char_typedef(void) {
    MyChar c = 65; // 'A'
    MyInt result = c;
    return result; // 65
}

MyInt test_pointer_typedef(void) {
    MyInt x = 99;
    IntPtr p = &x;
    return *p; // 99
}

MyInt test_pointer_modify(void) {
    MyInt x = 10;
    IntPtr p = &x;
    *p = 50;
    return x; // 50
}

MyInt test_string_typedef(void) {
    String s = "hello";
    MyInt count = 0;
    while (s[count] != 0) {
        count = count + 1;
    }
    return count; // 5
}

MyInt test_array_typedef(void) {
    IntArray arr;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    MyInt sum = 0;
    MyInt i;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + arr[i];
    }
    return sum; // 150
}

MyInt test_typedef_in_func(void) {
    return add(3, 7); // 10
}

MyInt test_typedef_multiply(void) {
    return multiply(6, 7); // 42
}

MyInt test_global_typedef(void) {
    return global_val; // 42
}

MyInt test_ptr_arithmetic(void) {
    IntArray arr;
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    IntPtr p = arr;
    MyInt first = *p;
    p = p + 1;
    MyInt second = *p;
    p = p + 1;
    MyInt third = *p;
    return first + second + third; // 600
}

MyInt test_typedef_assignment(void) {
    MyInt a = 5;
    MyInt b = a;
    MyInt c = b;
    return a + b + c; // 15
}

MyInt test_typedef_conditional(void) {
    MyInt x = 10;
    MyInt y = 20;
    MyInt result = (x < y) ? x : y;
    return result; // 10
}

MyInt test_nested_ptr(void) {
    MyInt val = 77;
    IntPtr p1 = &val;
    IntPtr p2 = p1;
    return *p2; // 77
}

MyInt test_sizeof_typedef(void) {
    return sizeof(MyInt); // 4
}

MyInt test_sizeof_intptr(void) {
    return sizeof(IntPtr); // 8
}

int main(void) {
    // EXPECT: basic typedef: 30
    printf("basic typedef: %d\n", test_basic_typedef());

    // EXPECT: char typedef: 65
    printf("char typedef: %d\n", test_char_typedef());

    // EXPECT: pointer typedef: 99
    printf("pointer typedef: %d\n", test_pointer_typedef());

    // EXPECT: pointer modify: 50
    printf("pointer modify: %d\n", test_pointer_modify());

    // EXPECT: string length: 5
    printf("string length: %d\n", test_string_typedef());

    // EXPECT: array typedef sum: 150
    printf("array typedef sum: %d\n", test_array_typedef());

    // EXPECT: typedef in func: 10
    printf("typedef in func: %d\n", test_typedef_in_func());

    // EXPECT: typedef multiply: 42
    printf("typedef multiply: %d\n", test_typedef_multiply());

    // EXPECT: global typedef: 42
    printf("global typedef: %d\n", test_global_typedef());

    // EXPECT: ptr arithmetic: 600
    printf("ptr arithmetic: %d\n", test_ptr_arithmetic());

    // EXPECT: typedef assignment: 15
    printf("typedef assignment: %d\n", test_typedef_assignment());

    // EXPECT: typedef conditional: 10
    printf("typedef conditional: %d\n", test_typedef_conditional());

    // EXPECT: nested ptr: 77
    printf("nested ptr: %d\n", test_nested_ptr());

    // EXPECT: sizeof MyInt: 4
    printf("sizeof MyInt: %d\n", test_sizeof_typedef());

    // EXPECT: sizeof IntPtr: 8
    printf("sizeof IntPtr: %d\n", test_sizeof_intptr());

    MyInt local = 123;
    // EXPECT: local typedef: 123
    printf("local typedef: %d\n", local);

    return 0;
}
