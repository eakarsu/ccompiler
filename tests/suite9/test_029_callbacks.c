int printf(const char *fmt, ...);

// Test: Callback patterns - higher-order functions via function pointers

typedef int (*Predicate)(int);
typedef int (*Transform)(int);
typedef void (*Action)(int);

int is_even(int n) {
    return n % 2 == 0;
}

int is_positive(int n) {
    return n > 0;
}

int is_greater_than_5(int n) {
    return n > 5;
}

int double_it(int n) {
    return n * 2;
}

int square(int n) {
    return n * n;
}

int increment(int n) {
    return n + 1;
}

int negate_val(int n) {
    return -n;
}

// Filter: count elements matching predicate
int count_if(int *arr, int n, Predicate pred) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) {
            count = count + 1;
        }
    }
    return count;
}

// Map: apply transform to each element, store in output
void map(int *src, int *dst, int n, Transform fn) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = fn(src[i]);
    }
}

// Reduce/fold: combine all elements with a binary operation
typedef int (*Combiner)(int, int);

int add_combine(int acc, int val) {
    return acc + val;
}

int mul_combine(int acc, int val) {
    return acc * val;
}

int max_combine(int acc, int val) {
    if (val > acc) return val;
    return acc;
}

int reduce(int *arr, int n, int init, Combiner fn) {
    int acc = init;
    int i;
    for (i = 0; i < n; i++) {
        acc = fn(acc, arr[i]);
    }
    return acc;
}

// ForEach: apply action to each element
int foreach_sum;

void accumulate(int n) {
    foreach_sum = foreach_sum + n;
}

void for_each(int *arr, int n, Action act) {
    int i;
    for (i = 0; i < n; i++) {
        act(arr[i]);
    }
}

// Find first matching element, return index or -1
int find_if(int *arr, int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) {
            return i;
        }
    }
    return -1;
}

// Apply transform chain
int apply_chain(int val, Transform *fns, int n) {
    int result = val;
    int i;
    for (i = 0; i < n; i++) {
        result = fns[i](result);
    }
    return result;
}

int main() {
    int data[8];
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[4] = 5;
    data[5] = 6;
    data[6] = 7;
    data[7] = 8;

    // count_if with different predicates
    int even_count = count_if(data, 8, is_even);
    // EXPECT: even count: 4
    printf("even count: %d\n", even_count);

    int pos_count = count_if(data, 8, is_positive);
    // EXPECT: positive count: 8
    printf("positive count: %d\n", pos_count);

    int gt5_count = count_if(data, 8, is_greater_than_5);
    // EXPECT: greater than 5: 3
    printf("greater than 5: %d\n", gt5_count);

    // map with double
    int doubled[8];
    map(data, doubled, 8, double_it);
    // EXPECT: doubled[0]: 2
    printf("doubled[0]: %d\n", doubled[0]);
    // EXPECT: doubled[7]: 16
    printf("doubled[7]: %d\n", doubled[7]);

    // map with square
    int squared[8];
    map(data, squared, 8, square);
    // EXPECT: squared[0]: 1
    printf("squared[0]: %d\n", squared[0]);
    // EXPECT: squared[4]: 25
    printf("squared[4]: %d\n", squared[4]);
    // EXPECT: squared[7]: 64
    printf("squared[7]: %d\n", squared[7]);

    // reduce with sum
    int total = reduce(data, 8, 0, add_combine);
    // EXPECT: sum: 36
    printf("sum: %d\n", total);

    // reduce with product (1*2*3*4*5*6*7*8 = 40320)
    int product = reduce(data, 8, 1, mul_combine);
    // EXPECT: product: 40320
    printf("product: %d\n", product);

    // reduce with max
    int mx = reduce(data, 8, 0, max_combine);
    // EXPECT: max: 8
    printf("max: %d\n", mx);

    // for_each with accumulate
    foreach_sum = 0;
    for_each(data, 8, accumulate);
    // EXPECT: foreach sum: 36
    printf("foreach sum: %d\n", foreach_sum);

    // find_if
    int idx_even = find_if(data, 8, is_even);
    // EXPECT: first even at: 1
    printf("first even at: %d\n", idx_even);

    int idx_gt5 = find_if(data, 8, is_greater_than_5);
    // EXPECT: first >5 at: 5
    printf("first >5 at: %d\n", idx_gt5);

    // Transform chain: increment then double then square
    // (((3 + 1) * 2) ^ 2) = ((4*2)^2) = (8^2) = 64
    Transform chain[3];
    chain[0] = increment;
    chain[1] = double_it;
    chain[2] = square;
    int chained = apply_chain(3, chain, 3);
    // EXPECT: chain result: 64
    printf("chain result: %d\n", chained);

    // Reverse chain: square then double then increment
    // (((3^2)*2)+1) = ((9*2)+1) = 19
    Transform rchain[3];
    rchain[0] = square;
    rchain[1] = double_it;
    rchain[2] = increment;
    int rchained = apply_chain(3, rchain, 3);
    // EXPECT: reverse chain: 19
    printf("reverse chain: %d\n", rchained);

    // EXPECT: callbacks done
    printf("callbacks done\n");

    return 0;
}
