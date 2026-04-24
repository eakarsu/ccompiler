int printf(const char *fmt, ...);

int global_counter;
int global_sum;
int global_log[20];
int global_log_idx;

void reset_globals(void) {
    global_counter = 0;
    global_sum = 0;
    global_log_idx = 0;
    int i = 0;
    while (i < 20) {
        global_log[i] = 0;
        i = i + 1;
    }
}

void increment(void) {
    global_counter = global_counter + 1;
}

void add_to_sum(int val) {
    global_sum = global_sum + val;
}

void log_value(int val) {
    if (global_log_idx < 20) {
        global_log[global_log_idx] = val;
        global_log_idx = global_log_idx + 1;
    }
}

int get_and_increment(void) {
    int old = global_counter;
    global_counter = global_counter + 1;
    return old;
}

void multiply_counter(int factor) {
    global_counter = global_counter * factor;
}

void conditional_add(int val) {
    if (val > 0) {
        global_sum = global_sum + val;
    }
}

int accumulate_and_return(int val) {
    global_sum = global_sum + val;
    return global_sum;
}

void chain_effect_a(int n) {
    log_value(n);
    global_counter = global_counter + 1;
    global_sum = global_sum + n;
}

void chain_effect_b(int n) {
    chain_effect_a(n);
    chain_effect_a(n * 2);
}

void chain_effect_c(int n) {
    chain_effect_b(n);
    chain_effect_a(n * 3);
}

int global_state;

int modify_and_check(int threshold) {
    global_state = global_state + 10;
    if (global_state > threshold) return 1;
    return 0;
}

void apply_n_times(int n, int val) {
    int i = 0;
    while (i < n) {
        add_to_sum(val);
        increment();
        i = i + 1;
    }
}

int main(void) {
    reset_globals();
    increment();
    increment();
    increment();
    // EXPECT: counter3: 3
    printf("counter3: %d\n", global_counter);

    reset_globals();
    add_to_sum(10);
    add_to_sum(20);
    add_to_sum(30);
    // EXPECT: sum60: 60
    printf("sum60: %d\n", global_sum);

    reset_globals();
    log_value(5);
    log_value(10);
    log_value(15);
    // EXPECT: log: 5 10 15
    printf("log: %d %d %d\n", global_log[0], global_log[1], global_log[2]);
    // EXPECT: log_idx: 3
    printf("log_idx: %d\n", global_log_idx);

    reset_globals();
    int a = get_and_increment();
    int b = get_and_increment();
    int c = get_and_increment();
    // EXPECT: get_inc: 0 1 2
    printf("get_inc: %d %d %d\n", a, b, c);
    // EXPECT: counter_after: 3
    printf("counter_after: %d\n", global_counter);

    reset_globals();
    global_counter = 5;
    multiply_counter(3);
    // EXPECT: mul_counter: 15
    printf("mul_counter: %d\n", global_counter);

    reset_globals();
    conditional_add(10);
    conditional_add(-5);
    conditional_add(20);
    conditional_add(-3);
    // EXPECT: cond_sum: 30
    printf("cond_sum: %d\n", global_sum);

    reset_globals();
    int r1 = accumulate_and_return(10);
    int r2 = accumulate_and_return(20);
    int r3 = accumulate_and_return(30);
    // EXPECT: accum_ret: 10 30 60
    printf("accum_ret: %d %d %d\n", r1, r2, r3);

    reset_globals();
    chain_effect_c(5);
    // EXPECT: chain_counter: 3
    printf("chain_counter: %d\n", global_counter);
    // EXPECT: chain_sum: 30
    printf("chain_sum: %d\n", global_sum);
    // EXPECT: chain_log: 5 10 15
    printf("chain_log: %d %d %d\n", global_log[0], global_log[1], global_log[2]);

    /* Verify order matters: add then multiply vs multiply then add */
    reset_globals();
    global_counter = 2;
    add_to_sum(global_counter);
    multiply_counter(3);
    add_to_sum(global_counter);
    // EXPECT: order1_sum: 8
    printf("order1_sum: %d\n", global_sum);

    reset_globals();
    global_counter = 2;
    multiply_counter(3);
    add_to_sum(global_counter);
    // EXPECT: order2_sum: 6
    printf("order2_sum: %d\n", global_sum);

    global_state = 0;
    int hit1 = modify_and_check(25);
    int hit2 = modify_and_check(25);
    int hit3 = modify_and_check(25);
    // EXPECT: state_check: 0 0 1
    printf("state_check: %d %d %d\n", hit1, hit2, hit3);
    // EXPECT: final_state: 30
    printf("final_state: %d\n", global_state);

    reset_globals();
    apply_n_times(5, 7);
    // EXPECT: apply5: counter=5 sum=35
    printf("apply5: counter=%d sum=%d\n", global_counter, global_sum);

    return 0;
}
