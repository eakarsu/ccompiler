// EXPECT: === Callback Patterns ===
// EXPECT: Square callback: 1 4 9 16 25
// EXPECT: Double callback: 2 4 6 8 10
// EXPECT: Negate callback: -1 -2 -3 -4 -5
// EXPECT: Event: button_click handled by handler_log
// EXPECT: Event: button_click handled by handler_count
// EXPECT: Event: key_press handled by handler_log
// EXPECT: Event: key_press handled by handler_count
// EXPECT: Strategy add: 15
// EXPECT: Strategy mul: 120
// EXPECT: Strategy max: 5
// EXPECT: Map [1..6] square: 1 4 9 16 25 36
// EXPECT: Filter [1..10] even: 2 4 6 8 10
// EXPECT: Reduce [1..5] sum: 15
// EXPECT: Reduce [1..5] product: 120
// EXPECT: Chained: map then reduce sum of squares = 55
// EXPECT: All callback tests passed

int printf(const char *fmt, ...);

typedef int (*transform_fn)(int);
typedef void (*event_handler)(const char *name);
typedef int (*binary_fn)(int, int);
typedef int (*predicate_fn)(int);

int square(int x) { return x * x; }
int dbl(int x) { return x * 2; }
int negate(int x) { return -x; }

void apply_and_print(int *arr, int n, transform_fn fn, const char *label) {
    printf("%s:", label);
    int i;
    for (i = 0; i < n; i++) {
        printf(" %d", fn(arr[i]));
    }
    printf("\n");
}

void test_basic_callbacks(void) {
    int arr[5];
    int i;
    for (i = 0; i < 5; i++) arr[i] = i + 1;

    apply_and_print(arr, 5, square, "Square callback");
    apply_and_print(arr, 5, dbl, "Double callback");
    apply_and_print(arr, 5, negate, "Negate callback");
}

void handler_log(const char *name) {
    printf("Event: %s handled by handler_log\n", name);
}

void handler_count(const char *name) {
    printf("Event: %s handled by handler_count\n", name);
}

void dispatch_event(const char *name, event_handler *handlers, int n) {
    int i;
    for (i = 0; i < n; i++) {
        handlers[i](name);
    }
}

void test_event_handlers(void) {
    event_handler handlers[2];
    handlers[0] = handler_log;
    handlers[1] = handler_count;

    dispatch_event("button_click", handlers, 2);
    dispatch_event("key_press", handlers, 2);
}

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int max(int a, int b) { return a > b ? a : b; }

int apply_strategy(int *arr, int n, binary_fn fn, int init) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = fn(result, arr[i]);
    }
    return result;
}

void test_strategy(void) {
    int arr[5];
    int i;
    for (i = 0; i < 5; i++) arr[i] = i + 1;

    printf("Strategy add: %d\n", apply_strategy(arr, 5, add, 0));
    printf("Strategy mul: %d\n", apply_strategy(arr, 5, mul, 1));
    printf("Strategy max: %d\n", apply_strategy(arr, 5, max, 0));
}

void map(int *src, int *dst, int n, transform_fn fn) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = fn(src[i]);
    }
}

int filter(int *src, int *dst, int n, predicate_fn pred) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(src[i])) {
            dst[count] = src[i];
            count++;
        }
    }
    return count;
}

int reduce(int *arr, int n, binary_fn fn, int init) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = fn(result, arr[i]);
    }
    return result;
}

int is_even(int x) { return (x % 2) == 0; }

void test_map_filter_reduce(void) {
    int src[6];
    int mapped[6];
    int filtered[10];
    int i;

    for (i = 0; i < 6; i++) src[i] = i + 1;

    map(src, mapped, 6, square);
    printf("Map [1..6] square:");
    for (i = 0; i < 6; i++) printf(" %d", mapped[i]);
    printf("\n");

    int src10[10];
    for (i = 0; i < 10; i++) src10[i] = i + 1;
    int fcount = filter(src10, filtered, 10, is_even);
    printf("Filter [1..10] even:");
    for (i = 0; i < fcount; i++) printf(" %d", filtered[i]);
    printf("\n");

    int arr5[5];
    for (i = 0; i < 5; i++) arr5[i] = i + 1;
    printf("Reduce [1..5] sum: %d\n", reduce(arr5, 5, add, 0));
    printf("Reduce [1..5] product: %d\n", reduce(arr5, 5, mul, 1));

    map(arr5, mapped, 5, square);
    int chain_result = reduce(mapped, 5, add, 0);
    printf("Chained: map then reduce sum of squares = %d\n", chain_result);
}

int main(void) {
    printf("=== Callback Patterns ===\n");
    test_basic_callbacks();
    test_event_handlers();
    test_strategy();
    test_map_filter_reduce();
    printf("All callback tests passed\n");
    return 0;
}
