int printf(const char *fmt, ...);

/* Error codes as enum */
enum ErrorCode {
    ERR_OK = 0,
    ERR_NOT_FOUND = 1,
    ERR_PERMISSION = 2,
    ERR_OVERFLOW = 3,
    ERR_INVALID_ARG = 4,
    ERR_TIMEOUT = 5,
    ERR_IO = 6,
    ERR_MEMORY = 7,
    ERR_INTERNAL = 8,
    ERR_COUNT = 9
};

/* Error severity levels */
enum Severity {
    SEV_INFO = 0,
    SEV_WARNING = 1,
    SEV_ERROR = 2,
    SEV_FATAL = 3
};

/* Error message lookup */
char *error_message(int code) {
    switch (code) {
        case ERR_OK:          return "Success";
        case ERR_NOT_FOUND:   return "Not found";
        case ERR_PERMISSION:  return "Permission denied";
        case ERR_OVERFLOW:    return "Overflow";
        case ERR_INVALID_ARG: return "Invalid argument";
        case ERR_TIMEOUT:     return "Timeout";
        case ERR_IO:          return "I/O error";
        case ERR_MEMORY:      return "Out of memory";
        case ERR_INTERNAL:    return "Internal error";
        default:              return "Unknown error";
    }
}

/* Get severity for an error code */
int get_severity(int code) {
    if (code == ERR_OK) return SEV_INFO;
    if (code == ERR_NOT_FOUND || code == ERR_TIMEOUT) return SEV_WARNING;
    if (code == ERR_PERMISSION || code == ERR_INVALID_ARG || code == ERR_IO) return SEV_ERROR;
    return SEV_FATAL;  /* OVERFLOW, MEMORY, INTERNAL */
}

char *severity_name(int sev) {
    if (sev == SEV_INFO) return "INFO";
    if (sev == SEV_WARNING) return "WARNING";
    if (sev == SEV_ERROR) return "ERROR";
    if (sev == SEV_FATAL) return "FATAL";
    return "UNKNOWN";
}

/* Error result struct */
struct Result {
    int error;
    int value;
};

struct Result make_ok(int val) {
    struct Result r;
    r.error = ERR_OK;
    r.value = val;
    return r;
}

struct Result make_error(int code) {
    struct Result r;
    r.error = code;
    r.value = 0;
    return r;
}

int is_ok(struct Result r) {
    return r.error == ERR_OK;
}

/* Simulated operations that can fail */
struct Result safe_divide(int a, int b) {
    if (b == 0) return make_error(ERR_INVALID_ARG);
    return make_ok(a / b);
}

struct Result safe_lookup(int table[], int size, int index) {
    if (index < 0 || index >= size) return make_error(ERR_NOT_FOUND);
    return make_ok(table[index]);
}

struct Result safe_add(int a, int b) {
    int result = a + b;
    /* Simplified overflow check */
    if (a > 0 && b > 0 && result < 0) return make_error(ERR_OVERFLOW);
    if (a < 0 && b < 0 && result > 0) return make_error(ERR_OVERFLOW);
    return make_ok(result);
}

/* Error log: record errors */
int error_log[20];
int log_count;

void log_error(int code) {
    if (log_count < 20) {
        error_log[log_count] = code;
        log_count++;
    }
}

int count_errors_by_severity(int sev) {
    int count = 0;
    int i;
    for (i = 0; i < log_count; i++) {
        if (get_severity(error_log[i]) == sev) count++;
    }
    return count;
}

int main(void) {
    log_count = 0;

    /* Test error messages */
    // EXPECT: ERR_OK: Success
    printf("ERR_OK: %s\n", error_message(ERR_OK));
    // EXPECT: ERR_NOT_FOUND: Not found
    printf("ERR_NOT_FOUND: %s\n", error_message(ERR_NOT_FOUND));
    // EXPECT: ERR_PERMISSION: Permission denied
    printf("ERR_PERMISSION: %s\n", error_message(ERR_PERMISSION));
    // EXPECT: ERR_MEMORY: Out of memory
    printf("ERR_MEMORY: %s\n", error_message(ERR_MEMORY));

    /* Test severity */
    // EXPECT: OK severity: INFO
    printf("OK severity: %s\n", severity_name(get_severity(ERR_OK)));
    // EXPECT: NOT_FOUND severity: WARNING
    printf("NOT_FOUND severity: %s\n", severity_name(get_severity(ERR_NOT_FOUND)));
    // EXPECT: PERMISSION severity: ERROR
    printf("PERMISSION severity: %s\n", severity_name(get_severity(ERR_PERMISSION)));
    // EXPECT: OVERFLOW severity: FATAL
    printf("OVERFLOW severity: %s\n", severity_name(get_severity(ERR_OVERFLOW)));

    /* Test safe_divide */
    struct Result r = safe_divide(10, 3);
    // EXPECT: 10/3 ok: 1 val: 3
    printf("10/3 ok: %d val: %d\n", is_ok(r), r.value);
    r = safe_divide(10, 0);
    // EXPECT: 10/0 ok: 0 err: Invalid argument
    printf("10/0 ok: %d err: %s\n", is_ok(r), error_message(r.error));

    /* Test safe_lookup */
    int table[5];
    table[0] = 100;
    table[1] = 200;
    table[2] = 300;
    table[3] = 400;
    table[4] = 500;

    r = safe_lookup(table, 5, 2);
    // EXPECT: Lookup[2] ok: 1 val: 300
    printf("Lookup[2] ok: %d val: %d\n", is_ok(r), r.value);
    r = safe_lookup(table, 5, 10);
    // EXPECT: Lookup[10] ok: 0 err: Not found
    printf("Lookup[10] ok: %d err: %s\n", is_ok(r), error_message(r.error));

    /* Test safe_add */
    r = safe_add(100, 200);
    // EXPECT: 100+200 ok: 1 val: 300
    printf("100+200 ok: %d val: %d\n", is_ok(r), r.value);

    /* Log some errors */
    log_error(ERR_NOT_FOUND);
    log_error(ERR_PERMISSION);
    log_error(ERR_NOT_FOUND);
    log_error(ERR_OVERFLOW);
    log_error(ERR_TIMEOUT);
    log_error(ERR_INTERNAL);
    log_error(ERR_IO);

    // EXPECT: Log count: 7
    printf("Log count: %d\n", log_count);
    // EXPECT: Warnings: 3
    printf("Warnings: %d\n", count_errors_by_severity(SEV_WARNING));
    // EXPECT: Errors: 2
    printf("Errors: %d\n", count_errors_by_severity(SEV_ERROR));
    // EXPECT: Fatals: 2
    printf("Fatals: %d\n", count_errors_by_severity(SEV_FATAL));

    /* Iterate all error codes and print severity */
    int code;
    for (code = 0; code < ERR_COUNT; code++) {
        printf("Code %d: %s\n", code, severity_name(get_severity(code)));
    }
    // EXPECT: Code 0: INFO
    // EXPECT: Code 1: WARNING
    // EXPECT: Code 2: ERROR
    // EXPECT: Code 3: FATAL
    // EXPECT: Code 4: ERROR
    // EXPECT: Code 5: WARNING
    // EXPECT: Code 6: ERROR
    // EXPECT: Code 7: FATAL
    // EXPECT: Code 8: FATAL

    return 0;
}
