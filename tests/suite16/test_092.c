int printf(const char *fmt, ...);

// Test 092: Filter and reduce over arrays using function pointers

typedef int (*Predicate)(int);
typedef int (*Reducer)(int, int);

int is_even(int x) {
    return x % 2 == 0;
}

int is_odd(int x) {
    return x % 2 != 0;
}

int is_positive(int x) {
    return x > 0;
}

int is_greater_than_five(int x) {
    return x > 5;
}

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int max_of(int a, int b) {
    if (a > b) return a;
    return b;
}

int min_of(int a, int b) {
    if (a < b) return a;
    return b;
}

int filter_array(int *src, int *dst, int len, Predicate pred) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < len; i++) {
        if (pred(src[i])) {
            dst[count] = src[i];
            count = count + 1;
        }
    }
    return count;
}

int reduce_array(int *arr, int len, int init, Reducer fn) {
    int i;
    int acc;
    acc = init;
    for (i = 0; i < len; i++) {
        acc = fn(acc, arr[i]);
    }
    return acc;
}

int count_matching(int *arr, int len, Predicate pred) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < len; i++) {
        if (pred(arr[i])) {
            count = count + 1;
        }
    }
    return count;
}

int any_match(int *arr, int len, Predicate pred) {
    int i;
    for (i = 0; i < len; i++) {
        if (pred(arr[i])) return 1;
    }
    return 0;
}

int all_match(int *arr, int len, Predicate pred) {
    int i;
    for (i = 0; i < len; i++) {
        if (!pred(arr[i])) return 0;
    }
    return 1;
}

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int data[8];
    int filtered[8];
    int n;

    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 4;
    data[4] = 5; data[5] = 6; data[6] = 7; data[7] = 8;

    n = filter_array(data, filtered, 8, is_even);
    print_array("evens", filtered, n); // EXPECT: evens: 2 4 6 8

    n = filter_array(data, filtered, 8, is_odd);
    print_array("odds", filtered, n); // EXPECT: odds: 1 3 5 7

    n = filter_array(data, filtered, 8, is_greater_than_five);
    print_array("gt5", filtered, n); // EXPECT: gt5: 6 7 8

    printf("sum: %d\n", reduce_array(data, 8, 0, add)); // EXPECT: sum: 36
    printf("product: %d\n", reduce_array(data, 8, 1, multiply)); // EXPECT: product: 40320
    printf("max: %d\n", reduce_array(data, 8, data[0], max_of)); // EXPECT: max: 8
    printf("min: %d\n", reduce_array(data, 8, data[0], min_of)); // EXPECT: min: 1

    printf("count_even: %d\n", count_matching(data, 8, is_even)); // EXPECT: count_even: 4
    printf("count_odd: %d\n", count_matching(data, 8, is_odd)); // EXPECT: count_odd: 4
    printf("count_gt5: %d\n", count_matching(data, 8, is_greater_than_five)); // EXPECT: count_gt5: 3

    printf("any_even: %d\n", any_match(data, 8, is_even)); // EXPECT: any_even: 1
    printf("all_pos: %d\n", all_match(data, 8, is_positive)); // EXPECT: all_pos: 1

    // filter then reduce: sum of evens
    n = filter_array(data, filtered, 8, is_even);
    printf("sum_evens: %d\n", reduce_array(filtered, n, 0, add)); // EXPECT: sum_evens: 20

    // filter then reduce: product of odds
    n = filter_array(data, filtered, 8, is_odd);
    printf("prod_odds: %d\n", reduce_array(filtered, n, 1, multiply)); // EXPECT: prod_odds: 105

    // filter then reduce: max of gt5
    n = filter_array(data, filtered, 8, is_greater_than_five);
    printf("max_gt5: %d\n", reduce_array(filtered, n, filtered[0], max_of)); // EXPECT: max_gt5: 8

    printf("done\n"); // EXPECT: done
    return 0;
}
