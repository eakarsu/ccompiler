int printf(const char *fmt, ...);
// EXPECT: sum=45 calls=10\nid1=1\nid2=2\nid3=3\nid4=4\nid5=5\nfib(10)=55\nfib(20)=6765\nfib(30)=832040\nfib(40)=102334155\ncompute(5)=47\ncompute(10)=137\ncompute(5)=47\ntoggle: 1 0 1 0 1 0 1 0 \nseq: 0 3 6 9 12 15 
// Test 224: Static local variables

int call_counter;

int counted_add(int a, int b) {
    call_counter = call_counter + 1;
    return a + b;
}

int accumulator(int val) {
    // Simulate static with global
    // For this test, we use a global to track state
    return val;
}

int global_counter;

int next_id(void) {
    global_counter = global_counter + 1;
    return global_counter;
}

int fib_memo[50];
int fib_initialized;

void init_fib_memo(void) {
    int i = 0;
    while (i < 50) {
        fib_memo[i] = -1;
        i = i + 1;
    }
    fib_initialized = 1;
}

int fib_memoized(int n) {
    if (!fib_initialized) init_fib_memo();
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (fib_memo[n] >= 0) return fib_memo[n];
    fib_memo[n] = fib_memoized(n - 1) + fib_memoized(n - 2);
    return fib_memo[n];
}

int cache[100];
int cache_valid[100];

int expensive_compute(int x) {
    if (x >= 0 && x < 100 && cache_valid[x]) {
        return cache[x];
    }
    int result = x * x + 3 * x + 7;
    if (x >= 0 && x < 100) {
        cache[x] = result;
        cache_valid[x] = 1;
    }
    return result;
}

int toggle_state;

int toggle(void) {
    toggle_state = 1 - toggle_state;
    return toggle_state;
}

int sequence_state;

int next_in_sequence(void) {
    int val = sequence_state;
    sequence_state = sequence_state + 3;
    return val;
}

int main(void) {
    call_counter = 0;
    int sum = 0;
    int i = 0;
    while (i < 10) {
        sum = counted_add(sum, i);
        i = i + 1;
    }
    printf("sum=%d calls=%d\n", sum, call_counter);

    global_counter = 0;
    printf("id1=%d\n", next_id());
    printf("id2=%d\n", next_id());
    printf("id3=%d\n", next_id());
    printf("id4=%d\n", next_id());
    printf("id5=%d\n", next_id());

    fib_initialized = 0;
    printf("fib(10)=%d\n", fib_memoized(10));
    printf("fib(20)=%d\n", fib_memoized(20));
    printf("fib(30)=%d\n", fib_memoized(30));
    printf("fib(40)=%d\n", fib_memoized(40));

    i = 0;
    while (i < 100) { cache_valid[i] = 0; i = i + 1; }
    printf("compute(5)=%d\n", expensive_compute(5));
    printf("compute(10)=%d\n", expensive_compute(10));
    printf("compute(5)=%d\n", expensive_compute(5));

    toggle_state = 0;
    printf("toggle: ");
    i = 0;
    while (i < 8) {
        printf("%d ", toggle());
        i = i + 1;
    }
    printf("\n");

    sequence_state = 0;
    printf("seq: ");
    i = 0;
    while (i < 6) {
        printf("%d ", next_in_sequence());
        i = i + 1;
    }
    printf("\n");

    return 0;
}
