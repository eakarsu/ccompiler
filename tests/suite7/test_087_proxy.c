int printf(const char *fmt, ...);

typedef int (*MathFunc)(int, int);

struct ProxyStats {
    int call_count;
    int total_args_sum;
    int last_result;
    int error_count;
};

int real_add(int a, int b) { return a + b; }
int real_sub(int a, int b) { return a - b; }
int real_mul(int a, int b) { return a * b; }
int real_div(int a, int b) {
    if (b == 0) return -9999;
    return a / b;
}

struct Proxy {
    MathFunc real_func;
    struct ProxyStats stats;
    int logging_enabled;
    int cache_key_a;
    int cache_key_b;
    int cache_value;
    int cache_valid;
};

void proxy_init(struct Proxy *p, MathFunc func) {
    p->real_func = func;
    p->stats.call_count = 0;
    p->stats.total_args_sum = 0;
    p->stats.last_result = 0;
    p->stats.error_count = 0;
    p->logging_enabled = 0;
    p->cache_valid = 0;
    p->cache_key_a = 0;
    p->cache_key_b = 0;
    p->cache_value = 0;
}

int proxy_call(struct Proxy *p, int a, int b) {
    int result;

    if (p->cache_valid && p->cache_key_a == a && p->cache_key_b == b) {
        p->stats.call_count = p->stats.call_count + 1;
        return p->cache_value;
    }

    result = p->real_func(a, b);

    p->stats.call_count = p->stats.call_count + 1;
    p->stats.total_args_sum = p->stats.total_args_sum + a + b;
    p->stats.last_result = result;

    if (result == -9999) {
        p->stats.error_count = p->stats.error_count + 1;
    }

    p->cache_key_a = a;
    p->cache_key_b = b;
    p->cache_value = result;
    p->cache_valid = 1;

    return result;
}

void proxy_enable_logging(struct Proxy *p) {
    p->logging_enabled = 1;
}

void proxy_reset_stats(struct Proxy *p) {
    p->stats.call_count = 0;
    p->stats.total_args_sum = 0;
    p->stats.last_result = 0;
    p->stats.error_count = 0;
}

void proxy_invalidate_cache(struct Proxy *p) {
    p->cache_valid = 0;
}

int proxy_avg_args(struct Proxy *p) {
    if (p->stats.call_count == 0) return 0;
    return p->stats.total_args_sum / p->stats.call_count;
}

int main(void) {
    struct Proxy add_proxy;
    struct Proxy div_proxy;
    struct Proxy mul_proxy;
    int result;
    int i;

    proxy_init(&add_proxy, real_add);
    proxy_init(&div_proxy, real_div);
    proxy_init(&mul_proxy, real_mul);

    result = proxy_call(&add_proxy, 10, 20);
    // EXPECT: add(10,20) = 30
    printf("add(10,20) = %d\n", result);
    // EXPECT: calls: 1
    printf("calls: %d\n", add_proxy.stats.call_count);

    result = proxy_call(&add_proxy, 5, 3);
    // EXPECT: add(5,3) = 8
    printf("add(5,3) = %d\n", result);
    // EXPECT: calls: 2
    printf("calls: %d\n", add_proxy.stats.call_count);

    result = proxy_call(&add_proxy, 5, 3);
    // EXPECT: cached add(5,3) = 8
    printf("cached add(5,3) = %d\n", result);
    // EXPECT: calls after cache: 3
    printf("calls after cache: %d\n", add_proxy.stats.call_count);

    // EXPECT: args sum: 38
    printf("args sum: %d\n", add_proxy.stats.total_args_sum);
    // EXPECT: last result: 8
    printf("last result: %d\n", add_proxy.stats.last_result);

    result = proxy_call(&div_proxy, 100, 5);
    // EXPECT: div(100,5) = 20
    printf("div(100,5) = %d\n", result);

    result = proxy_call(&div_proxy, 10, 0);
    // EXPECT: div(10,0) = -9999
    printf("div(10,0) = %d\n", result);
    // EXPECT: errors: 1
    printf("errors: %d\n", div_proxy.stats.error_count);

    for (i = 1; i <= 5; i++) {
        proxy_call(&mul_proxy, i, i);
    }
    // EXPECT: mul calls: 5
    printf("mul calls: %d\n", mul_proxy.stats.call_count);
    // EXPECT: mul last: 25
    printf("mul last: %d\n", mul_proxy.stats.last_result);
    // EXPECT: mul args sum: 30
    printf("mul args sum: %d\n", mul_proxy.stats.total_args_sum);
    // EXPECT: mul avg args: 6
    printf("mul avg args: %d\n", proxy_avg_args(&mul_proxy));

    proxy_reset_stats(&mul_proxy);
    // EXPECT: after reset: 0
    printf("after reset: %d\n", mul_proxy.stats.call_count);

    proxy_call(&mul_proxy, 7, 8);
    // EXPECT: mul(7,8) = 56
    printf("mul(7,8) = %d\n", mul_proxy.stats.last_result);

    proxy_invalidate_cache(&mul_proxy);
    proxy_call(&mul_proxy, 7, 8);
    // EXPECT: after invalidate calls: 2
    printf("after invalidate calls: %d\n", mul_proxy.stats.call_count);

    return 0;
}
