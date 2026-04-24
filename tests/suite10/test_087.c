int printf(const char *fmt, ...);

// Decorator Pattern: function wrapping to add behavior

typedef int (*IntFunc)(int);

int base_compute(int x) {
    return x * 2;
}

int logging_state;

int logging_compute(int x) {
    printf("Input: %d\n", x);
    int result;
    result = base_compute(x);
    printf("Output: %d\n", result);
    return result;
}

int caching_values[32];
int caching_results[32];
int cache_count;

void cache_init() {
    cache_count = 0;
}

int cache_lookup(int x) {
    int i;
    for (i = 0; i < cache_count; i = i + 1) {
        if (caching_values[i] == x) {
            return i;
        }
    }
    return -1;
}

int caching_compute(int x) {
    int idx;
    idx = cache_lookup(x);
    if (idx >= 0) {
        printf("Cache hit for %d\n", x);
        return caching_results[idx];
    }
    int result;
    result = base_compute(x);
    printf("Cache miss for %d\n", x);
    caching_values[cache_count] = x;
    caching_results[cache_count] = result;
    cache_count = cache_count + 1;
    return result;
}

int validating_compute(int x) {
    if (x < 0) {
        printf("Invalid input: %d\n", x);
        return -1;
    }
    if (x > 100) {
        printf("Input too large: %d\n", x);
        return -1;
    }
    return base_compute(x);
}

struct Decorator {
    IntFunc func;
    char *name;
};

int call_decorated(struct Decorator *d, int x) {
    return d->func(x);
}

struct DecoratorChain {
    struct Decorator decorators[4];
    int count;
};

void chain_init(struct DecoratorChain *ch) {
    ch->count = 0;
}

void chain_add(struct DecoratorChain *ch, IntFunc f, char *name) {
    ch->decorators[ch->count].func = f;
    ch->decorators[ch->count].name = name;
    ch->count = ch->count + 1;
}

int timing_counter;

int timing_compute(int x) {
    timing_counter = timing_counter + 1;
    int result;
    result = base_compute(x);
    return result;
}

int main() {
    printf("--- Base ---\n"); // EXPECT: --- Base ---
    int r;
    r = base_compute(5);
    printf("base(5): %d\n", r); // EXPECT: base(5): 10
    r = base_compute(12);
    printf("base(12): %d\n", r); // EXPECT: base(12): 24

    printf("--- Logging Decorator ---\n"); // EXPECT: --- Logging Decorator ---
    r = logging_compute(7); // EXPECT: Input: 7
    // EXPECT: Output: 14
    printf("Result: %d\n", r); // EXPECT: Result: 14

    printf("--- Caching Decorator ---\n"); // EXPECT: --- Caching Decorator ---
    cache_init();
    r = caching_compute(5); // EXPECT: Cache miss for 5
    printf("Result: %d\n", r); // EXPECT: Result: 10
    r = caching_compute(5); // EXPECT: Cache hit for 5
    printf("Result: %d\n", r); // EXPECT: Result: 10
    r = caching_compute(8); // EXPECT: Cache miss for 8
    printf("Result: %d\n", r); // EXPECT: Result: 16
    r = caching_compute(5); // EXPECT: Cache hit for 5
    printf("Result: %d\n", r); // EXPECT: Result: 10
    printf("Cache size: %d\n", cache_count); // EXPECT: Cache size: 2

    printf("--- Validating Decorator ---\n"); // EXPECT: --- Validating Decorator ---
    r = validating_compute(10);
    printf("valid(10): %d\n", r); // EXPECT: valid(10): 20
    r = validating_compute(-5); // EXPECT: Invalid input: -5
    printf("valid(-5): %d\n", r); // EXPECT: valid(-5): -1
    r = validating_compute(200); // EXPECT: Input too large: 200
    printf("valid(200): %d\n", r); // EXPECT: valid(200): -1

    printf("--- Decorator Struct ---\n"); // EXPECT: --- Decorator Struct ---
    struct Decorator d;
    d.func = base_compute;
    d.name = "base";
    printf("Decorator: %s\n", d.name); // EXPECT: Decorator: base
    r = call_decorated(&d, 15);
    printf("Result: %d\n", r); // EXPECT: Result: 30

    printf("--- Timing Decorator ---\n"); // EXPECT: --- Timing Decorator ---
    timing_counter = 0;
    int i;
    for (i = 1; i <= 5; i = i + 1) {
        timing_compute(i);
    }
    printf("Calls tracked: %d\n", timing_counter); // EXPECT: Calls tracked: 5

    printf("Done\n"); // EXPECT: Done
    return 0;
}
