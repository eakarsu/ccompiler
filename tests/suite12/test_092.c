int printf(const char *fmt, ...);

// Long chain of function pointer calls through array

typedef int (*UnaryFn)(int);

int add_one(int x) {
    return x + 1;
}

int mul_two(int x) {
    return x * 2;
}

int sub_three(int x) {
    return x - 3;
}

int square(int x) {
    return x * x;
}

int negate(int x) {
    return -x;
}

int add_ten(int x) {
    return x + 10;
}

int div_two(int x) {
    return x / 2;
}

int mod_seven(int x) {
    return x % 7;
}

int identity(int x) {
    return x;
}

int add_hundred(int x) {
    return x + 100;
}

int apply_chain(UnaryFn *fns, int count, int start) {
    int i;
    int val;
    val = start;
    for (i = 0; i < count; i = i + 1) {
        val = fns[i](val);
    }
    return val;
}

int apply_reverse(UnaryFn *fns, int count, int start) {
    int i;
    int val;
    val = start;
    for (i = count - 1; i >= 0; i = i - 1) {
        val = fns[i](val);
    }
    return val;
}

int apply_even(UnaryFn *fns, int count, int start) {
    int i;
    int val;
    val = start;
    for (i = 0; i < count; i = i + 2) {
        val = fns[i](val);
    }
    return val;
}

int apply_odd(UnaryFn *fns, int count, int start) {
    int i;
    int val;
    val = start;
    for (i = 1; i < count; i = i + 2) {
        val = fns[i](val);
    }
    return val;
}

int apply_conditional(UnaryFn *fns, int count, int start) {
    int i;
    int val;
    val = start;
    for (i = 0; i < count; i = i + 1) {
        int tmp;
        tmp = fns[i](val);
        if (tmp > 0) {
            val = tmp;
        }
    }
    return val;
}

int compose_two(UnaryFn f, UnaryFn g, int x) {
    return f(g(x));
}

int main() {
    UnaryFn fns[10];
    int result;

    fns[0] = add_one;
    fns[1] = mul_two;
    fns[2] = sub_three;
    fns[3] = add_ten;
    fns[4] = div_two;
    fns[5] = identity;
    fns[6] = add_hundred;
    fns[7] = negate;
    fns[8] = add_one;
    fns[9] = mod_seven;

    // Chain: 5 -> 6 -> 12 -> 9 -> 19 -> 9 -> 9 -> 109 -> -109 -> -108 -> -3
    result = apply_chain(fns, 10, 5);
    printf("%d\n", result); // EXPECT: -3

    // Just first 3: 5 -> 6 -> 12 -> 9
    result = apply_chain(fns, 3, 5);
    printf("%d\n", result); // EXPECT: 9

    // Reverse of first 4: 5 -> add_ten=15 -> sub_three=12 -> mul_two=24 -> add_one=25
    result = apply_reverse(fns, 4, 5);
    printf("%d\n", result); // EXPECT: 25

    // Even indices (0,2,4,6,8): 5 -> add_one=6 -> sub_three=3 -> div_two=1 -> add_hundred=101 -> add_one=102
    result = apply_even(fns, 10, 5);
    printf("%d\n", result); // EXPECT: 102

    // Odd indices (1,3,5,7,9): 5 -> mul_two=10 -> add_ten=20 -> identity=20 -> negate=-20 -> mod_seven=-6
    result = apply_odd(fns, 10, 5);
    printf("%d\n", result); // EXPECT: -6

    // Conditional (only apply if result > 0):
    // 5 -> add_one=6(>0,take) -> mul_two=12(>0,take) -> sub_three=9(>0,take)
    // -> add_ten=19(>0,take) -> div_two=9(>0,take) -> identity=9(>0,take)
    // -> add_hundred=109(>0,take) -> negate=-109(not>0,skip,stay 109)
    // -> add_one=110(>0,take) -> mod_seven=5(>0,take)
    result = apply_conditional(fns, 10, 5);
    printf("%d\n", result); // EXPECT: 5

    // Compose tests
    result = compose_two(add_one, mul_two, 7);
    printf("%d\n", result); // EXPECT: 15

    result = compose_two(mul_two, add_one, 7);
    printf("%d\n", result); // EXPECT: 16

    result = compose_two(negate, add_hundred, 0);
    printf("%d\n", result); // EXPECT: -100

    result = compose_two(add_hundred, negate, 0);
    printf("%d\n", result); // EXPECT: 100

    // Function pointer comparison
    if (fns[0] == add_one) {
        printf("%d\n", 1); // EXPECT: 1
    }
    if (fns[1] == mul_two) {
        printf("%d\n", 1); // EXPECT: 1
    }

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
