int printf(const char *fmt, ...);

// Test 078: Retry logic - retry with backoff, max attempts, transient error detection

enum RetryResult {
    RETRY_SUCCESS,
    RETRY_TRANSIENT_FAIL,
    RETRY_PERMANENT_FAIL,
    RETRY_EXHAUSTED
};

struct RetryConfig {
    int max_attempts;
    int initial_delay;
    int max_delay;
    int backoff_factor;
};

struct RetryState {
    int attempt;
    int current_delay;
    int total_delay;
    int last_error;
    int transient_count;
    int permanent_count;
};

void retry_init(struct RetryState *rs) {
    rs->attempt = 0;
    rs->current_delay = 0;
    rs->total_delay = 0;
    rs->last_error = 0;
    rs->transient_count = 0;
    rs->permanent_count = 0;
}

int compute_delay(struct RetryConfig *cfg, int attempt) {
    int delay = cfg->initial_delay;
    for (int i = 0; i < attempt; i++) {
        delay = delay * cfg->backoff_factor;
        if (delay > cfg->max_delay) {
            delay = cfg->max_delay;
            break;
        }
    }
    return delay;
}

int is_transient_error(int err_code) {
    if (err_code == 1) return 1;
    if (err_code == 2) return 1;
    if (err_code == 5) return 1;
    return 0;
}

typedef int (*OperationFn)(int attempt);

int operation_succeed_third(int attempt) {
    if (attempt < 2) return 1;
    return 0;
}

int operation_always_fail(int attempt) {
    return 3;
}

int operation_permanent_fail(int attempt) {
    if (attempt == 0) return 1;
    return 9;
}

int operation_succeed_fifth(int attempt) {
    if (attempt < 4) return 2;
    return 0;
}

int operation_succeed_first(int attempt) {
    return 0;
}

char *result_str(int r) {
    if (r == RETRY_SUCCESS) return "SUCCESS";
    if (r == RETRY_TRANSIENT_FAIL) return "TRANSIENT";
    if (r == RETRY_PERMANENT_FAIL) return "PERMANENT";
    if (r == RETRY_EXHAUSTED) return "EXHAUSTED";
    return "UNKNOWN";
}

int retry_execute(struct RetryConfig *cfg, struct RetryState *rs, OperationFn op) {
    retry_init(rs);

    while (rs->attempt < cfg->max_attempts) {
        int err = op(rs->attempt);

        if (err == 0) {
            return RETRY_SUCCESS;
        }

        rs->last_error = err;

        if (!is_transient_error(err)) {
            rs->permanent_count++;
            return RETRY_PERMANENT_FAIL;
        }

        rs->transient_count++;
        rs->current_delay = compute_delay(cfg, rs->attempt);
        rs->total_delay = rs->total_delay + rs->current_delay;
        rs->attempt++;
    }

    return RETRY_EXHAUSTED;
}

int main() {
    struct RetryConfig cfg;
    cfg.max_attempts = 5;
    cfg.initial_delay = 100;
    cfg.max_delay = 1000;
    cfg.backoff_factor = 2;

    printf("Delay schedule:\n");
    // EXPECT: Delay schedule:
    for (int i = 0; i < 6; i++) {
        printf("  attempt %d: delay=%d\n", i, compute_delay(&cfg, i));
    }
    // EXPECT:   attempt 0: delay=100
    // EXPECT:   attempt 1: delay=200
    // EXPECT:   attempt 2: delay=400
    // EXPECT:   attempt 3: delay=800
    // EXPECT:   attempt 4: delay=1000
    // EXPECT:   attempt 5: delay=1000

    struct RetryState rs;
    int result;

    result = retry_execute(&cfg, &rs, operation_succeed_third);
    printf("Test1: %s attempts=%d total_delay=%d\n",
           result_str(result), rs.attempt, rs.total_delay);
    // EXPECT: Test1: SUCCESS attempts=2 total_delay=300

    result = retry_execute(&cfg, &rs, operation_always_fail);
    printf("Test2: %s attempts=%d last_err=%d\n",
           result_str(result), rs.attempt, rs.last_error);
    // EXPECT: Test2: PERMANENT attempts=0 last_err=3

    result = retry_execute(&cfg, &rs, operation_permanent_fail);
    printf("Test3: %s attempts=%d transient=%d permanent=%d\n",
           result_str(result), rs.attempt, rs.transient_count, rs.permanent_count);
    // EXPECT: Test3: PERMANENT attempts=1 transient=1 permanent=1

    result = retry_execute(&cfg, &rs, operation_succeed_fifth);
    printf("Test4: %s attempts=%d total_delay=%d\n",
           result_str(result), rs.attempt, rs.total_delay);
    // EXPECT: Test4: SUCCESS attempts=4 total_delay=1500

    result = retry_execute(&cfg, &rs, operation_succeed_first);
    printf("Test5: %s attempts=%d total_delay=%d\n",
           result_str(result), rs.attempt, rs.total_delay);
    // EXPECT: Test5: SUCCESS attempts=0 total_delay=0

    struct RetryConfig strict;
    strict.max_attempts = 2;
    strict.initial_delay = 50;
    strict.max_delay = 200;
    strict.backoff_factor = 3;

    result = retry_execute(&strict, &rs, operation_succeed_third);
    printf("Test6 strict: %s attempts=%d transient=%d\n",
           result_str(result), rs.attempt, rs.transient_count);
    // EXPECT: Test6 strict: EXHAUSTED attempts=2 transient=2

    printf("Transient errors: 1=%d 2=%d 3=%d 5=%d 9=%d\n",
           is_transient_error(1), is_transient_error(2),
           is_transient_error(3), is_transient_error(5),
           is_transient_error(9));
    // EXPECT: Transient errors: 1=1 2=1 3=0 5=1 9=0

    printf("Retry logic done\n");
    // EXPECT: Retry logic done
    return 0;
}
