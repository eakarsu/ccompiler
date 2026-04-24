int printf(const char *fmt, ...);

// Test 071: Error codes with enums - comprehensive error code system

enum ErrorCode {
    ERR_NONE,
    ERR_NULL_PTR,
    ERR_OUT_OF_RANGE,
    ERR_INVALID_ARG,
    ERR_OVERFLOW,
    ERR_UNDERFLOW,
    ERR_DIVIDE_ZERO,
    ERR_NOT_FOUND,
    ERR_ALREADY_EXISTS,
    ERR_PERMISSION,
    ERR_TIMEOUT,
    ERR_UNKNOWN
};

struct ErrorInfo {
    int code;
    int line;
    int severity;
};

int error_to_severity(int code) {
    if (code == ERR_NONE) return 0;
    if (code == ERR_NULL_PTR) return 3;
    if (code == ERR_OUT_OF_RANGE) return 2;
    if (code == ERR_INVALID_ARG) return 2;
    if (code == ERR_OVERFLOW) return 3;
    if (code == ERR_UNDERFLOW) return 3;
    if (code == ERR_DIVIDE_ZERO) return 3;
    if (code == ERR_NOT_FOUND) return 1;
    if (code == ERR_ALREADY_EXISTS) return 1;
    if (code == ERR_PERMISSION) return 2;
    if (code == ERR_TIMEOUT) return 2;
    return 3;
}

char *error_name(int code) {
    if (code == ERR_NONE) return "NONE";
    if (code == ERR_NULL_PTR) return "NULL_PTR";
    if (code == ERR_OUT_OF_RANGE) return "OUT_OF_RANGE";
    if (code == ERR_INVALID_ARG) return "INVALID_ARG";
    if (code == ERR_OVERFLOW) return "OVERFLOW";
    if (code == ERR_UNDERFLOW) return "UNDERFLOW";
    if (code == ERR_DIVIDE_ZERO) return "DIVIDE_ZERO";
    if (code == ERR_NOT_FOUND) return "NOT_FOUND";
    if (code == ERR_ALREADY_EXISTS) return "ALREADY_EXISTS";
    if (code == ERR_PERMISSION) return "PERMISSION";
    if (code == ERR_TIMEOUT) return "TIMEOUT";
    return "UNKNOWN";
}

char *severity_label(int sev) {
    if (sev == 0) return "OK";
    if (sev == 1) return "WARN";
    if (sev == 2) return "ERROR";
    if (sev == 3) return "FATAL";
    return "UNKNOWN";
}

int is_recoverable(int code) {
    if (code == ERR_NONE) return 1;
    if (code == ERR_NOT_FOUND) return 1;
    if (code == ERR_ALREADY_EXISTS) return 1;
    if (code == ERR_TIMEOUT) return 1;
    return 0;
}

struct ErrorInfo make_error(int code, int line) {
    struct ErrorInfo ei;
    ei.code = code;
    ei.line = line;
    ei.severity = error_to_severity(code);
    return ei;
}

void print_error(struct ErrorInfo ei) {
    printf("Error[%s] at line %d severity=%s\n",
           error_name(ei.code), ei.line, severity_label(ei.severity));
}

int safe_divide(int a, int b, int *result, int *err) {
    if (b == 0) {
        *err = ERR_DIVIDE_ZERO;
        return -1;
    }
    *result = a / b;
    *err = ERR_NONE;
    return 0;
}

int safe_array_access(int *arr, int size, int idx, int *result, int *err) {
    if (arr == 0) {
        *err = ERR_NULL_PTR;
        return -1;
    }
    if (idx < 0 || idx >= size) {
        *err = ERR_OUT_OF_RANGE;
        return -1;
    }
    *result = arr[idx];
    *err = ERR_NONE;
    return 0;
}

int main() {
    int errors_tested = 0;
    int recoverable_count = 0;
    int fatal_count = 0;

    int codes[12];
    codes[0] = ERR_NONE;
    codes[1] = ERR_NULL_PTR;
    codes[2] = ERR_OUT_OF_RANGE;
    codes[3] = ERR_INVALID_ARG;
    codes[4] = ERR_OVERFLOW;
    codes[5] = ERR_UNDERFLOW;
    codes[6] = ERR_DIVIDE_ZERO;
    codes[7] = ERR_NOT_FOUND;
    codes[8] = ERR_ALREADY_EXISTS;
    codes[9] = ERR_PERMISSION;
    codes[10] = ERR_TIMEOUT;
    codes[11] = ERR_UNKNOWN;

    for (int i = 0; i < 12; i++) {
        if (is_recoverable(codes[i])) recoverable_count++;
        if (error_to_severity(codes[i]) == 3) fatal_count++;
        errors_tested++;
    }

    printf("Tested %d error codes\n", errors_tested);               // EXPECT: Tested 12 error codes
    printf("Recoverable: %d\n", recoverable_count);                 // EXPECT: Recoverable: 4
    printf("Fatal: %d\n", fatal_count);                             // EXPECT: Fatal: 5

    struct ErrorInfo e1 = make_error(ERR_DIVIDE_ZERO, 42);
    print_error(e1);  // EXPECT: Error[DIVIDE_ZERO] at line 42 severity=FATAL

    struct ErrorInfo e2 = make_error(ERR_NOT_FOUND, 99);
    print_error(e2);  // EXPECT: Error[NOT_FOUND] at line 99 severity=WARN

    int result = 0;
    int err = 0;

    safe_divide(10, 3, &result, &err);
    printf("10/3 = %d err=%s\n", result, error_name(err));          // EXPECT: 10/3 = 3 err=NONE

    safe_divide(10, 0, &result, &err);
    printf("10/0 err=%s\n", error_name(err));                       // EXPECT: 10/0 err=DIVIDE_ZERO

    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;

    safe_array_access(arr, 5, 2, &result, &err);
    printf("arr[2] = %d err=%s\n", result, error_name(err));        // EXPECT: arr[2] = 30 err=NONE

    safe_array_access(arr, 5, 10, &result, &err);
    printf("arr[10] err=%s\n", error_name(err));                    // EXPECT: arr[10] err=OUT_OF_RANGE

    safe_array_access(0, 5, 0, &result, &err);
    printf("null arr err=%s\n", error_name(err));                   // EXPECT: null arr err=NULL_PTR

    printf("Done\n");                                                // EXPECT: Done
    return 0;
}
