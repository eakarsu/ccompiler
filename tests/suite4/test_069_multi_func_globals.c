int printf(const char *fmt, ...);

// Test multiple functions sharing global state and accumulating results.

int g_total = 0;
int g_count = 0;
int g_min = 2147483647;
int g_max = -2147483647;
int g_product = 1;
int g_history[20];
int g_hist_len = 0;

void record(int val) {
    g_total = g_total + val;
    g_count = g_count + 1;
    if (val < g_min) g_min = val;
    if (val > g_max) g_max = val;
    if (g_hist_len < 20) {
        g_history[g_hist_len] = val;
        g_hist_len = g_hist_len + 1;
    }
}

int get_total(void) {
    return g_total;
}

int get_count(void) {
    return g_count;
}

int get_min(void) {
    return g_min;
}

int get_max(void) {
    return g_max;
}

int get_range(void) {
    return g_max - g_min;
}

int get_average(void) {
    if (g_count == 0) return 0;
    return g_total / g_count;
}

void reset_stats(void) {
    g_total = 0;
    g_count = 0;
    g_min = 2147483647;
    g_max = -2147483647;
    g_hist_len = 0;
}

// Separate accumulator system using different globals
int g_acc_a = 0;
int g_acc_b = 0;
int g_acc_c = 0;

void add_to_a(int val) {
    g_acc_a = g_acc_a + val;
}

void add_to_b(int val) {
    g_acc_b = g_acc_b + val;
}

void add_to_c(int val) {
    g_acc_c = g_acc_c + val;
}

int sum_accumulators(void) {
    return g_acc_a + g_acc_b + g_acc_c;
}

void transfer_a_to_b(void) {
    g_acc_b = g_acc_b + g_acc_a;
    g_acc_a = 0;
}

// Producer-consumer pattern with globals
int g_buffer[10];
int g_buf_write = 0;
int g_buf_read = 0;

void produce(int val) {
    if (g_buf_write < 10) {
        g_buffer[g_buf_write] = val;
        g_buf_write = g_buf_write + 1;
    }
}

int consume(void) {
    int val;
    if (g_buf_read < g_buf_write) {
        val = g_buffer[g_buf_read];
        g_buf_read = g_buf_read + 1;
        return val;
    }
    return -1;
}

int items_available(void) {
    return g_buf_write - g_buf_read;
}

// Chain of functions that build up a result
int g_chain = 0;

void chain_add(int x) { g_chain = g_chain + x; }
void chain_mul(int x) { g_chain = g_chain * x; }
void chain_sub(int x) { g_chain = g_chain - x; }

int main(void) {
    // Record some values
    record(10);
    record(30);
    record(20);
    record(5);
    record(25);

    // total = 10+30+20+5+25 = 90
    // EXPECT: 90
    printf("%d\n", get_total());
    // EXPECT: 5
    printf("%d\n", get_count());
    // EXPECT: 5
    printf("%d\n", get_min());
    // EXPECT: 30
    printf("%d\n", get_max());
    // EXPECT: 25
    printf("%d\n", get_range());
    // average = 90/5 = 18
    // EXPECT: 18
    printf("%d\n", get_average());

    // Check history
    // EXPECT: 10
    printf("%d\n", g_history[0]);
    // EXPECT: 25
    printf("%d\n", g_history[4]);

    // Reset and re-record
    reset_stats();
    record(100);
    record(-50);
    // total = 100 + (-50) = 50
    // EXPECT: 50
    printf("%d\n", get_total());
    // EXPECT: 2
    printf("%d\n", get_count());
    // EXPECT: -50
    printf("%d\n", get_min());
    // EXPECT: 100
    printf("%d\n", get_max());

    // Test multiple accumulators
    add_to_a(10);
    add_to_a(20);
    add_to_b(30);
    add_to_c(40);
    add_to_c(50);
    // a=30, b=30, c=90
    // EXPECT: 30
    printf("%d\n", g_acc_a);
    // EXPECT: 30
    printf("%d\n", g_acc_b);
    // EXPECT: 90
    printf("%d\n", g_acc_c);
    // sum = 30+30+90 = 150
    // EXPECT: 150
    printf("%d\n", sum_accumulators());

    // Transfer a to b
    transfer_a_to_b();
    // EXPECT: 0
    printf("%d\n", g_acc_a);
    // b = 30+30 = 60
    // EXPECT: 60
    printf("%d\n", g_acc_b);

    // Producer-consumer
    produce(11);
    produce(22);
    produce(33);
    // EXPECT: 3
    printf("%d\n", items_available());

    // EXPECT: 11
    printf("%d\n", consume());
    // EXPECT: 22
    printf("%d\n", consume());
    // EXPECT: 1
    printf("%d\n", items_available());
    // EXPECT: 33
    printf("%d\n", consume());
    // EXPECT: 0
    printf("%d\n", items_available());

    // No more items
    // EXPECT: -1
    printf("%d\n", consume());

    // Chain operations: start=0, +5=5, *3=15, -7=8, +2=10, *4=40
    chain_add(5);
    chain_mul(3);
    chain_sub(7);
    chain_add(2);
    chain_mul(4);
    // EXPECT: 40
    printf("%d\n", g_chain);

    return 0;
}
