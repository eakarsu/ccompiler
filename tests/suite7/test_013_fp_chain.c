int printf(const char *fmt, ...);

typedef int (*Transform)(int);

int double_it(int x) { return x * 2; }
int add_one(int x) { return x + 1; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int add_ten(int x) { return x + 10; }
int halve(int x) { return x / 2; }
int mod_seven(int x) { return x % 7; }

int apply_chain(Transform chain[], int count, int value) {
    int i;
    for (i = 0; i < count; i++) {
        value = chain[i](value);
    }
    return value;
}

void transform_array(int arr[], int n, Transform chain[], int chain_len) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = apply_chain(chain, chain_len, arr[i]);
    }
}

int apply_chain_reverse(Transform chain[], int count, int value) {
    int i;
    for (i = count - 1; i >= 0; i--) {
        value = chain[i](value);
    }
    return value;
}

int apply_n_times(Transform f, int n, int value) {
    int i;
    for (i = 0; i < n; i++) {
        value = f(value);
    }
    return value;
}

int chain_until(Transform f, int value, int limit) {
    int steps;
    steps = 0;
    while (value < limit) {
        value = f(value);
        steps = steps + 1;
    }
    return steps;
}

int sum_transformed(int arr[], int n, Transform f) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i++) {
        total = total + f(arr[i]);
    }
    return total;
}

int max_transformed(int arr[], int n, Transform f) {
    int best;
    int val;
    int i;
    best = f(arr[0]);
    for (i = 1; i < n; i++) {
        val = f(arr[i]);
        if (val > best) best = val;
    }
    return best;
}

int main(void) {
    Transform chain[4];
    int arr[5];
    int val;
    int i;

    chain[0] = double_it;
    chain[1] = add_one;
    val = apply_chain(chain, 2, 5);
    // EXPECT: double+1(5) = 11
    printf("double+1(5) = %d\n", val);

    chain[0] = add_one;
    chain[1] = double_it;
    val = apply_chain(chain, 2, 5);
    // EXPECT: +1 then double(5) = 12
    printf("+1 then double(5) = %d\n", val);

    chain[0] = add_one;
    chain[1] = square;
    chain[2] = negate;
    val = apply_chain(chain, 3, 4);
    // EXPECT: +1,sq,neg(4) = -25
    printf("+1,sq,neg(4) = %d\n", val);

    val = apply_chain_reverse(chain, 3, 4);
    // EXPECT: rev neg,sq,+1(4) = 17
    printf("rev neg,sq,+1(4) = %d\n", val);

    val = apply_n_times(double_it, 4, 1);
    // EXPECT: double x4(1) = 16
    printf("double x4(1) = %d\n", val);

    val = apply_n_times(add_one, 10, 0);
    // EXPECT: +1 x10(0) = 10
    printf("+1 x10(0) = %d\n", val);

    val = chain_until(double_it, 1, 100);
    // EXPECT: steps to 100: 7
    printf("steps to 100: %d\n", val);

    val = chain_until(add_ten, 0, 55);
    // EXPECT: steps +10 to 55: 6
    printf("steps +10 to 55: %d\n", val);

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    val = sum_transformed(arr, 5, square);
    // EXPECT: sum squares: 55
    printf("sum squares: %d\n", val);

    val = sum_transformed(arr, 5, double_it);
    // EXPECT: sum doubled: 30
    printf("sum doubled: %d\n", val);

    val = max_transformed(arr, 5, negate);
    // EXPECT: max negated: -1
    printf("max negated: %d\n", val);

    chain[0] = double_it;
    chain[1] = add_one;
    transform_array(arr, 5, chain, 2);
    // EXPECT: transformed: 3 5 7 9 11
    printf("transformed: %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    chain[0] = halve;
    chain[1] = mod_seven;
    transform_array(arr, 5, chain, 2);
    // EXPECT: halve mod7: 5 3 1 6 4
    printf("halve mod7: %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    chain[0] = add_one;
    chain[1] = double_it;
    chain[2] = add_ten;
    chain[3] = halve;
    val = apply_chain(chain, 4, 3);
    // EXPECT: pipeline(3) = 9
    printf("pipeline(3) = %d\n", val);

    val = apply_chain(chain, 4, 7);
    // EXPECT: pipeline(7) = 13
    printf("pipeline(7) = %d\n", val);

    return 0;
}
