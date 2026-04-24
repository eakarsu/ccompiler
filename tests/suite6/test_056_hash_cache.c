int printf(const char *fmt, ...);

int fib_cache_key[64];
int fib_cache_val[64];
int fib_cache_used[64];
int fib_hits;
int fib_misses;

int fact_cache_key[64];
int fact_cache_val[64];
int fact_cache_used[64];
int fact_hits;
int fact_misses;

void cache_init(int *keys, int *vals, int *used, int *hits, int *misses) {
    *hits = 0;
    *misses = 0;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        keys[i] = 0;
        vals[i] = 0;
        used[i] = 0;
    }
}

int cache_get(int *keys, int *vals, int *used, int key, int *found) {
    int h = key;
    if (h < 0) h = -h;
    h = h % 64;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (used[idx] == 0) {
            *found = 0;
            return 0;
        }
        if (keys[idx] == key) {
            *found = 1;
            return vals[idx];
        }
    }
    *found = 0;
    return 0;
}

void cache_put(int *keys, int *vals, int *used, int key, int val) {
    int h = key;
    if (h < 0) h = -h;
    h = h % 64;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (used[idx] == 0 || keys[idx] == key) {
            keys[idx] = key;
            vals[idx] = val;
            used[idx] = 1;
            return;
        }
    }
}

int fib_cached(int n) {
    if (n <= 1) return n;
    int found;
    int val = cache_get(fib_cache_key, fib_cache_val, fib_cache_used, n, &found);
    if (found) {
        fib_hits = fib_hits + 1;
        return val;
    }
    fib_misses = fib_misses + 1;
    int result = fib_cached(n - 1) + fib_cached(n - 2);
    cache_put(fib_cache_key, fib_cache_val, fib_cache_used, n, result);
    return result;
}

int fib_calls;

int fib_naive(int n) {
    fib_calls = fib_calls + 1;
    if (n <= 1) return n;
    return fib_naive(n - 1) + fib_naive(n - 2);
}

int fact_cached(int n) {
    if (n <= 1) return 1;
    int found;
    int val = cache_get(fact_cache_key, fact_cache_val, fact_cache_used, n, &found);
    if (found) {
        fact_hits = fact_hits + 1;
        return val;
    }
    fact_misses = fact_misses + 1;
    int result = n * fact_cached(n - 1);
    cache_put(fact_cache_key, fact_cache_val, fact_cache_used, n, result);
    return result;
}

int main(void) {
    cache_init(fib_cache_key, fib_cache_val, fib_cache_used, &fib_hits, &fib_misses);
    cache_init(fact_cache_key, fact_cache_val, fact_cache_used, &fact_hits, &fact_misses);

    // EXPECT: fib 0: 0
    printf("fib 0: %d\n", fib_cached(0));

    // EXPECT: fib 1: 1
    printf("fib 1: %d\n", fib_cached(1));

    // EXPECT: fib 10: 55
    printf("fib 10: %d\n", fib_cached(10));

    // EXPECT: fib 15: 610
    printf("fib 15: %d\n", fib_cached(15));

    // EXPECT: fib 20: 6765
    printf("fib 20: %d\n", fib_cached(20));

    // EXPECT: fib hits positive: 1
    printf("fib hits positive: %d\n", fib_hits > 0);

    // EXPECT: fib misses positive: 1
    printf("fib misses positive: %d\n", fib_misses > 0);

    fib_calls = 0;
    fib_naive(20);
    // EXPECT: naive fib20 calls high: 1
    printf("naive fib20 calls high: %d\n", fib_calls > 10000);

    // EXPECT: cached much fewer: 1
    int total_cached = fib_hits + fib_misses;
    printf("cached much fewer: %d\n", total_cached < fib_calls);

    // EXPECT: fact 1: 1
    printf("fact 1: %d\n", fact_cached(1));

    // EXPECT: fact 5: 120
    printf("fact 5: %d\n", fact_cached(5));

    // EXPECT: fact 10: 3628800
    printf("fact 10: %d\n", fact_cached(10));

    // EXPECT: fact hits from recompute: 1
    int old_hits = fact_hits;
    fact_cached(10);
    printf("fact hits from recompute: %d\n", fact_hits > old_hits);

    // EXPECT: fact 7: 5040
    printf("fact 7: %d\n", fact_cached(7));

    // EXPECT: fib cached again: 6765
    int old_hits2 = fib_hits;
    int r = fib_cached(20);
    printf("fib cached again: %d\n", r);

    // EXPECT: reuse hit: 1
    printf("reuse hit: %d\n", fib_hits > old_hits2);

    // EXPECT: done
    printf("done\n");

    return 0;
}
