int printf(const char *fmt, ...);

typedef int (*Predicate)(int);
typedef int (*Transform)(int);
typedef int (*BinOp)(int, int);
typedef void (*Action)(int *);

// Predicates
int is_positive(int x) { return x > 0 ? 1 : 0; }
int is_negative(int x) { return x < 0 ? 1 : 0; }
int is_even(int x) { return (x % 2 == 0) ? 1 : 0; }
int is_odd(int x) { return (x % 2 != 0) ? 1 : 0; }
int is_zero(int x) { return x == 0 ? 1 : 0; }
int greater_than_five(int x) { return x > 5 ? 1 : 0; }

// Transforms
int double_it(int x) { return x * 2; }
int square_it(int x) { return x * x; }
int negate_it(int x) { return -x; }
int increment(int x) { return x + 1; }
int decrement(int x) { return x - 1; }

// Higher-order: count elements satisfying predicate
int count_if(int *arr, int n, Predicate pred) {
    int count = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (pred(arr[i])) count = count + 1;
    }
    return count;
}

// Higher-order: find first element satisfying predicate, return index or -1
int find_if(int *arr, int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (pred(arr[i])) return i;
    }
    return -1;
}

// Higher-order: check if all elements satisfy predicate
int all_of(int *arr, int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (!pred(arr[i])) return 0;
    }
    return 1;
}

// Higher-order: check if any element satisfies predicate
int any_of(int *arr, int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (pred(arr[i])) return 1;
    }
    return 0;
}

// Higher-order: map (transform each element)
void map_array(int *arr, int n, Transform fn) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        arr[i] = fn(arr[i]);
    }
}

// Higher-order: fold/reduce
int reduce(int *arr, int n, int init, BinOp op) {
    int acc = init;
    int i;
    for (i = 0; i < n; i = i + 1) {
        acc = op(acc, arr[i]);
    }
    return acc;
}

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

// Higher-order: apply transform only to elements matching predicate
void map_if(int *arr, int n, Predicate pred, Transform fn) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (pred(arr[i])) {
            arr[i] = fn(arr[i]);
        }
    }
}

// Compose two transforms: return fn2(fn1(x))
int compose(Transform fn1, Transform fn2, int x) {
    return fn2(fn1(x));
}

// Apply a function n times
int apply_n(Transform fn, int x, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        x = fn(x);
    }
    return x;
}

// Callback: for_each element call an action
void for_each(int *arr, int n, Action act) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        act(&arr[i]);
    }
}

void double_action(int *p) {
    *p = *p * 2;
}

void zero_action(int *p) {
    *p = 0;
}

int main(void) {
    int data[] = {-3, -1, 0, 2, 4, 7, 8, 10};

    // count_if
    // EXPECT: 5
    printf("%d\n", count_if(data, 8, is_positive));
    // EXPECT: 2
    printf("%d\n", count_if(data, 8, is_negative));
    // EXPECT: 5
    printf("%d\n", count_if(data, 8, is_even));
    // EXPECT: 1
    printf("%d\n", count_if(data, 8, is_zero));

    // find_if
    // EXPECT: 3
    printf("%d\n", find_if(data, 8, is_positive));
    // EXPECT: 0
    printf("%d\n", find_if(data, 8, is_negative));
    // EXPECT: 5
    printf("%d\n", find_if(data, 8, greater_than_five));

    // all_of / any_of
    int positives[] = {1, 2, 3, 4, 5};
    // EXPECT: 1
    printf("%d\n", all_of(positives, 5, is_positive));
    // EXPECT: 0
    printf("%d\n", all_of(data, 8, is_positive));
    // EXPECT: 1
    printf("%d\n", any_of(data, 8, is_negative));
    // EXPECT: 0
    printf("%d\n", any_of(positives, 5, is_negative));

    // map_array: double each element of positives
    map_array(positives, 5, double_it);
    // EXPECT: 2 4 6 8 10
    printf("%d %d %d %d %d\n", positives[0], positives[1], positives[2], positives[3], positives[4]);

    // reduce: sum and product
    int nums[] = {1, 2, 3, 4, 5};
    // EXPECT: 15
    printf("%d\n", reduce(nums, 5, 0, add));
    // EXPECT: 120
    printf("%d\n", reduce(nums, 5, 1, mul));

    // reduce: max and min
    int mixed[] = {5, 2, 8, 1, 9, 3};
    // EXPECT: 9
    printf("%d\n", reduce(mixed, 6, mixed[0], max));
    // EXPECT: 1
    printf("%d\n", reduce(mixed, 6, mixed[0], min));

    // map_if: negate only negative values
    int data2[] = {-3, 5, -1, 7, -4};
    map_if(data2, 5, is_negative, negate_it);
    // EXPECT: 3 5 1 7 4
    printf("%d %d %d %d %d\n", data2[0], data2[1], data2[2], data2[3], data2[4]);

    // compose: double then square: square(double(3)) = square(6) = 36
    // EXPECT: 36
    printf("%d\n", compose(double_it, square_it, 3));
    // compose: square then double: double(square(3)) = double(9) = 18
    // EXPECT: 18
    printf("%d\n", compose(square_it, double_it, 3));

    // apply_n: double 3 times starting from 1: 1->2->4->8
    // EXPECT: 8
    printf("%d\n", apply_n(double_it, 1, 3));
    // apply_n: increment 10 times starting from 0
    // EXPECT: 10
    printf("%d\n", apply_n(increment, 0, 10));

    // for_each with action callback
    int arr[] = {1, 2, 3, 4};
    for_each(arr, 4, double_action);
    // EXPECT: 2 4 6 8
    printf("%d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);

    for_each(arr, 4, zero_action);
    // EXPECT: 0 0 0 0
    printf("%d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);

    return 0;
}
