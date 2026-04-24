int printf(const char *fmt, ...);

int counter;

int inc_and_test(int val, int threshold) {
    counter = counter + 1;
    return val > threshold;
}

int test_and_short(int a, int b) {
    counter = 0;
    int result = inc_and_test(a, 5) && inc_and_test(b, 5);
    return result * 100 + counter;
}

int test_or_short(int a, int b) {
    counter = 0;
    int result = inc_and_test(a, 5) || inc_and_test(b, 5);
    return result * 100 + counter;
}

int chain_and(int a, int b, int c) {
    counter = 0;
    int result = inc_and_test(a, 0) && inc_and_test(b, 0) && inc_and_test(c, 0);
    return result * 100 + counter;
}

int chain_or(int a, int b, int c) {
    counter = 0;
    int result = inc_and_test(a, 0) || inc_and_test(b, 0) || inc_and_test(c, 0);
    return result * 100 + counter;
}

int mixed_and_or(int a, int b, int c) {
    counter = 0;
    int result = (inc_and_test(a, 0) && inc_and_test(b, 0)) || inc_and_test(c, 0);
    return result * 100 + counter;
}

int safe_div(int a, int b) {
    if (b != 0 && a / b > 2) return 1;
    return 0;
}

int safe_array(int arr[], int len, int idx) {
    if (idx >= 0 && idx < len && arr[idx] > 0) return 1;
    return 0;
}

int eval_order_and(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (1 && (a = 1) && (b = 2) && (c = 3)) {
        return a * 100 + b * 10 + c;
    }
    return -1;
}

int eval_order_and_fail(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (1 && (a = 1) && 0 && (c = 3)) {
        return -1;
    }
    return a * 100 + b * 10 + c;
}

int eval_order_or(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (0 || (a = 1, 0) || (b = 2, 0) || (c = 3)) {
        return a * 100 + b * 10 + c;
    }
    return -1;
}

int eval_order_or_succeed(void) {
    int a = 0;
    int b = 0;
    int c = 0;
    if (0 || (a = 1) || (b = 2) || (c = 3)) {
        return a * 100 + b * 10 + c;
    }
    return -1;
}

int nested_short_circuit(int x, int y) {
    counter = 0;
    int result = (x > 0 && y > 0) || (x < 0 && y < 0);
    return result;
}

int complex_condition(int a, int b, int c, int d) {
    return (a > 0 && b > 0) || (c > 0 && d > 0);
}

int not_with_short(int x, int y) {
    if (!(x > 0 && y > 0)) return 1;
    return 0;
}

int while_short_circuit(int arr[], int len) {
    int i = 0;
    int sum = 0;
    while (i < len && arr[i] > 0) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

int main(void) {
    // EXPECT: and_short 10,10: 102
    printf("and_short 10,10: %d\n", test_and_short(10, 10));
    // EXPECT: and_short 3,10: 1
    printf("and_short 3,10: %d\n", test_and_short(3, 10));
    // EXPECT: or_short 10,3: 101
    printf("or_short 10,3: %d\n", test_or_short(10, 3));
    // EXPECT: or_short 3,10: 102
    printf("or_short 3,10: %d\n", test_or_short(3, 10));
    // EXPECT: or_short 3,3: 2
    printf("or_short 3,3: %d\n", test_or_short(3, 3));

    // EXPECT: chain_and 1,2,3: 103
    printf("chain_and 1,2,3: %d\n", chain_and(1, 2, 3));
    // EXPECT: chain_and 1,0,3: 2
    printf("chain_and 1,0,3: %d\n", chain_and(1, 0, 3));
    // EXPECT: chain_and 0,2,3: 1
    printf("chain_and 0,2,3: %d\n", chain_and(0, 2, 3));

    // EXPECT: chain_or 0,0,1: 103
    printf("chain_or 0,0,1: %d\n", chain_or(0, 0, 1));
    // EXPECT: chain_or 1,0,0: 101
    printf("chain_or 1,0,0: %d\n", chain_or(1, 0, 0));
    // EXPECT: chain_or 0,0,0: 3
    printf("chain_or 0,0,0: %d\n", chain_or(0, 0, 0));

    // EXPECT: mixed 1,2,0: 102
    printf("mixed 1,2,0: %d\n", mixed_and_or(1, 2, 0));
    // EXPECT: mixed 1,0,5: 103
    printf("mixed 1,0,5: %d\n", mixed_and_or(1, 0, 5));
    // EXPECT: mixed 0,2,0: 2
    printf("mixed 0,2,0: %d\n", mixed_and_or(0, 2, 0));

    // EXPECT: safediv 10,3: 1
    printf("safediv 10,3: %d\n", safe_div(10, 3));
    // EXPECT: safediv 10,0: 0
    printf("safediv 10,0: %d\n", safe_div(10, 0));

    int arr[] = {5, 3, 8, -1, 7};
    // EXPECT: safearr 2: 1
    printf("safearr 2: %d\n", safe_array(arr, 5, 2));
    // EXPECT: safearr 3: 0
    printf("safearr 3: %d\n", safe_array(arr, 5, 3));
    // EXPECT: safearr 10: 0
    printf("safearr 10: %d\n", safe_array(arr, 5, 10));

    // EXPECT: evaland: 123
    printf("evaland: %d\n", eval_order_and());
    // EXPECT: evalandfail: 100
    printf("evalandfail: %d\n", eval_order_and_fail());
    // EXPECT: evalor: 123
    printf("evalor: %d\n", eval_order_or());
    // EXPECT: evalorsucceed: 100
    printf("evalorsucceed: %d\n", eval_order_or_succeed());

    // EXPECT: complex 1,1,0,0: 1
    printf("complex 1,1,0,0: %d\n", complex_condition(1, 1, 0, 0));
    // EXPECT: complex 0,0,1,1: 1
    printf("complex 0,0,1,1: %d\n", complex_condition(0, 0, 1, 1));
    // EXPECT: complex 0,1,1,0: 0
    printf("complex 0,1,1,0: %d\n", complex_condition(0, 1, 1, 0));

    // EXPECT: not 1,1: 0
    printf("not 1,1: %d\n", not_with_short(1, 1));
    // EXPECT: not 1,-1: 1
    printf("not 1,-1: %d\n", not_with_short(1, -1));

    int arr2[] = {3, 5, 7, -1, 2};
    // EXPECT: whilesc: 15
    printf("whilesc: %d\n", while_short_circuit(arr2, 5));

    return 0;
}
