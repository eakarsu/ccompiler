// stress_038_static_local.c - Static local variable stress
// EXPECT: counter_20=20
// EXPECT: multi_a=5 50
// EXPECT: multi_b=10 100
// EXPECT: recursive_static=10
// EXPECT: static_arr_sum=55
// EXPECT: static_struct=10 20
// EXPECT: static_struct2=20 40
// EXPECT: func_a_count=4
// EXPECT: func_b_count=6
// EXPECT: func_a_after=5
// EXPECT: toggle=1
// EXPECT: toggle=0
// EXPECT: toggle=1
// EXPECT: toggle=0
// EXPECT: accumulate=55
// EXPECT: init_once=42
// EXPECT: init_once=42
// EXPECT: static_ptr=100
// EXPECT: interleave=a1 b1 a2 b2 a3 b3
// EXPECT: countdown=5 4 3 2 1 0
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// --- Simple counter called 20 times ---
int counter(void) {
    static int c = 0;
    c++;
    return c;
}

// --- Multiple static locals in one function ---
void multi_static(int *out_a, int *out_b) {
    static int a = 0;
    static int b = 0;
    a++;
    b = b + 10;
    *out_a = a;
    *out_b = b;
}

// --- Static local in recursive function ---
int recursive_count(int n) {
    static int total = 0;
    if (n <= 0) return total;
    total++;
    return recursive_count(n - 1);
}

// --- Static local array ---
int static_arr_access(int idx, int val, int do_set) {
    static int arr[10];
    if (do_set) {
        arr[idx] = val;
        return 0;
    }
    return arr[idx];
}

// --- Static local struct ---
struct Point { int x; int y; };

struct Point static_struct_func(int dx, int dy) {
    static struct Point p;
    p.x = p.x + dx;
    p.y = p.y + dy;
    struct Point ret;
    ret.x = p.x;
    ret.y = p.y;
    return ret;
}

// --- Multiple functions each with static locals ---
int func_a(void) {
    static int count = 0;
    count++;
    return count;
}

int func_b(void) {
    static int count = 0;
    count++;
    return count;
}

// --- Toggle function ---
int toggle(void) {
    static int state = 0;
    state = !state;
    return state;
}

// --- Accumulator ---
int accumulate(int val) {
    static int sum = 0;
    sum = sum + val;
    return sum;
}

// --- Init once pattern ---
int init_once(void) {
    static int initialized = 0;
    static int value = 0;
    if (!initialized) {
        value = 42;
        initialized = 1;
    }
    return value;
}

// --- Static pointer ---
int static_ptr_func(int *new_ptr) {
    static int *saved = 0;
    if (new_ptr) saved = new_ptr;
    if (saved) return *saved;
    return -1;
}

// --- Interleaved calls ---
int interleave_a(void) {
    static int n = 0;
    n++;
    return n;
}

int interleave_b(void) {
    static int n = 0;
    n++;
    return n;
}

// --- Countdown ---
int countdown(void) {
    static int n = 6;
    n--;
    return n;
}

int main(void) {
    // Counter called 20 times
    int i;
    int val;
    for (i = 0; i < 20; i++) val = counter();
    printf("counter_20=%d\n", val);

    // Multiple static locals
    int a_val;
    int b_val;
    for (i = 0; i < 5; i++) multi_static(&a_val, &b_val);
    printf("multi_a=%d %d\n", a_val, b_val);
    for (i = 0; i < 5; i++) multi_static(&a_val, &b_val);
    // Now a=10, b=100... wait, after first 5 calls a=5,b=50. After 5 more a=10,b=100
    // But EXPECT says 5 500. Fix: should be 10 100.
    printf("multi_b=%d %d\n", a_val, b_val);

    // Recursive static
    int rc = recursive_count(10);
    printf("recursive_static=%d\n", rc);

    // Static array: set values then read sum
    for (i = 0; i < 10; i++) static_arr_access(i, i + 1, 1);
    int arr_sum = 0;
    for (i = 0; i < 10; i++) arr_sum = arr_sum + static_arr_access(i, 0, 0);
    printf("static_arr_sum=%d\n", arr_sum); // 1+2+...+10=55

    // Static struct
    struct Point p1 = static_struct_func(10, 20);
    printf("static_struct=%d %d\n", p1.x, p1.y);
    struct Point p2 = static_struct_func(10, 20);
    printf("static_struct2=%d %d\n", p2.x, p2.y);

    // Multiple functions with independent static locals
    for (i = 0; i < 3; i++) func_a();
    for (i = 0; i < 5; i++) func_b();
    printf("func_a_count=%d\n", func_a());  // 4th call but we want 3
    // Actually func_a() was called 3 times, then this printf calls it a 4th time
    // So it prints 4. Let me restructure.
    // Actually let me fix: call func_a 3 times and store result, then print.
    // But we already called it 3 times and now call it again. Let me just adjust EXPECT.

    printf("func_b_count=%d\n", func_b()); // 6th call
    printf("func_a_after=%d\n", func_a()); // 5th call of func_a

    // Toggle
    for (i = 0; i < 4; i++) {
        printf("toggle=%d\n", toggle());
    }

    // Accumulator
    for (i = 1; i <= 10; i++) val = accumulate(i);
    printf("accumulate=%d\n", val);

    // Init once
    printf("init_once=%d\n", init_once());
    printf("init_once=%d\n", init_once());

    // Static pointer
    int sv = 100;
    static_ptr_func(&sv);
    printf("static_ptr=%d\n", static_ptr_func(0));

    // Interleaved calls
    printf("interleave=");
    for (i = 0; i < 3; i++) {
        if (i > 0) printf(" ");
        printf("a%d b%d", interleave_a(), interleave_b());
    }
    printf("\n");

    // Countdown
    printf("countdown=");
    for (i = 0; i < 6; i++) {
        if (i > 0) printf(" ");
        printf("%d", countdown());
    }
    printf("\n");

    printf("pass_done=1\n");
    return 0;
}
