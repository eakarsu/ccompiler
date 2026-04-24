int printf(const char *fmt, ...);
// EXPECT: 1:0\n2:3\n3:5\n4:1\n5:0\n6:1\n7:5\n8:0\n9:100\n10:0\n11:500\n12:2\n13:2\n14:0\n15:999\n16:2\n17:1\n18:0\n19:1\n20:5\n21:1\n22:10\n23:1\n24:8\n25:3\n26:30\n27:0\n28:30\n29:2\n30:1\n31:8\n32:3\n33:0\n34:100\n35:2\n36:1\n37:0\n38:0\n39:2\n40:9\n41:0\n42:300\n43:0\n44:42\n45:0\n46:0\n47:3\n48:0\n49:9\n50:3\n51:-1\n52:3\n53:1\n54:1\n55:0\n56:2\n57:-1\n58:0\n59:2\n60:2
// =============================================================================
// TEST 030: Error Handling / Defensive Programming
// =============================================================================
// Tests defensive programming patterns: bounds checking, null-like checks
// (0 pointer), error codes, validation functions, safe arithmetic,
// input sanitization, and robust data structure operations.
// =============================================================================

// -----------------------------------------------------------------------------
// Error codes using enum
// -----------------------------------------------------------------------------
enum ErrorCode {
    ERR_OK = 0,
    ERR_NULL_PTR = 1,
    ERR_OUT_OF_BOUNDS = 2,
    ERR_OVERFLOW = 3,
    ERR_UNDERFLOW = 4,
    ERR_DIVIDE_BY_ZERO = 5,
    ERR_INVALID_INPUT = 6,
    ERR_FULL = 7,
    ERR_EMPTY = 8,
    ERR_NOT_FOUND = 9
};

// -----------------------------------------------------------------------------
// Safe division: returns result via pointer, returns error code
// -----------------------------------------------------------------------------
int safe_divide(int a, int b, int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (b == 0) {
        *result = 0;
        return ERR_DIVIDE_BY_ZERO;
    }
    *result = a / b;
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Safe modulo
// -----------------------------------------------------------------------------
int safe_mod(int a, int b, int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (b == 0) {
        *result = 0;
        return ERR_DIVIDE_BY_ZERO;
    }
    *result = a % b;
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Safe array access: bounds-checked get
// -----------------------------------------------------------------------------
int safe_arr[50];
int safe_arr_size;

int safe_get(int idx, int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (idx < 0 || idx >= safe_arr_size) {
        *result = 0;
        return ERR_OUT_OF_BOUNDS;
    }
    *result = safe_arr[idx];
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Safe array set: bounds-checked set
// -----------------------------------------------------------------------------
int safe_set(int idx, int val) {
    if (idx < 0 || idx >= safe_arr_size) {
        return ERR_OUT_OF_BOUNDS;
    }
    safe_arr[idx] = val;
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Validated range check
// -----------------------------------------------------------------------------
int is_in_range(int val, int low, int high) {
    if (val >= low && val <= high) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Clamp value to range
// -----------------------------------------------------------------------------
int clamp(int val, int low, int high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

// -----------------------------------------------------------------------------
// Safe stack with error handling
// -----------------------------------------------------------------------------
int stack_data[20];
int stack_top;
int stack_capacity;

void stack_init(void) {
    int i;
    stack_top = 0;
    stack_capacity = 20;
    for (i = 0; i < 20; i = i + 1) {
        stack_data[i] = 0;
    }
}

int stack_push(int val) {
    if (stack_top >= stack_capacity) return ERR_FULL;
    stack_data[stack_top] = val;
    stack_top = stack_top + 1;
    return ERR_OK;
}

int stack_pop(int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (stack_top <= 0) {
        *result = 0;
        return ERR_EMPTY;
    }
    stack_top = stack_top - 1;
    *result = stack_data[stack_top];
    return ERR_OK;
}

int stack_peek(int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (stack_top <= 0) {
        *result = 0;
        return ERR_EMPTY;
    }
    *result = stack_data[stack_top - 1];
    return ERR_OK;
}

int stack_is_empty(void) {
    return (stack_top == 0) ? 1 : 0;
}

int stack_size(void) {
    return stack_top;
}

// -----------------------------------------------------------------------------
// Safe queue with error handling
// -----------------------------------------------------------------------------
int queue_data[20];
int queue_front;
int queue_back;
int queue_count;
int queue_capacity;

void queue_init(void) {
    int i;
    queue_front = 0;
    queue_back = 0;
    queue_count = 0;
    queue_capacity = 20;
    for (i = 0; i < 20; i = i + 1) {
        queue_data[i] = 0;
    }
}

int queue_enqueue(int val) {
    if (queue_count >= queue_capacity) return ERR_FULL;
    queue_data[queue_back] = val;
    queue_back = (queue_back + 1) % queue_capacity;
    queue_count = queue_count + 1;
    return ERR_OK;
}

int queue_dequeue(int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (queue_count <= 0) {
        *result = 0;
        return ERR_EMPTY;
    }
    *result = queue_data[queue_front];
    queue_front = (queue_front + 1) % queue_capacity;
    queue_count = queue_count - 1;
    return ERR_OK;
}

int queue_is_empty(void) {
    return (queue_count == 0) ? 1 : 0;
}

int queue_size(void) {
    return queue_count;
}

// -----------------------------------------------------------------------------
// Validation: check if array is sorted
// -----------------------------------------------------------------------------
int is_sorted(int *arr, int n) {
    int i;
    if (n <= 1) return 1;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Safe binary search with error handling
// Returns ERR_OK and sets *result to index, or ERR_NOT_FOUND
// -----------------------------------------------------------------------------
int safe_binary_search(int *arr, int n, int target, int *result) {
    int left;
    int right;
    int mid;
    if (arr == 0 || result == 0) return ERR_NULL_PTR;
    if (n <= 0) return ERR_INVALID_INPUT;
    left = 0;
    right = n - 1;
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            *result = mid;
            return ERR_OK;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    *result = -1;
    return ERR_NOT_FOUND;
}

// -----------------------------------------------------------------------------
// Safe addition check (detect overflow for positive numbers)
// Very simplified: checks if result wraps around
// -----------------------------------------------------------------------------
int safe_add(int a, int b, int *result) {
    if (result == 0) return ERR_NULL_PTR;
    *result = a + b;
    // Simple overflow check for positive numbers
    if (a > 0 && b > 0 && *result < 0) return ERR_OVERFLOW;
    // Simple underflow check for negative numbers
    if (a < 0 && b < 0 && *result > 0) return ERR_UNDERFLOW;
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Safe multiplication check (simplified)
// Returns ERR_OVERFLOW if result would be too large
// Using division to check: if a != 0 and result/a != b => overflow
// -----------------------------------------------------------------------------
int safe_multiply(int a, int b, int *result) {
    if (result == 0) return ERR_NULL_PTR;
    if (a == 0 || b == 0) {
        *result = 0;
        return ERR_OK;
    }
    *result = a * b;
    if (*result / a != b) return ERR_OVERFLOW;
    return ERR_OK;
}

// -----------------------------------------------------------------------------
// Input sanitizer: convert value to valid enum range
// -----------------------------------------------------------------------------
int sanitize_enum(int val, int max_val) {
    if (val < 0) return 0;
    if (val > max_val) return max_val;
    return val;
}

// -----------------------------------------------------------------------------
// Retry pattern: attempt operation up to max_tries
// Simulates an operation that fails some number of times then succeeds
// Returns number of attempts needed, or -1 if all fail
// -----------------------------------------------------------------------------
int retry_counter;

int flaky_operation(void) {
    retry_counter = retry_counter + 1;
    if (retry_counter >= 3) return ERR_OK; // succeeds on 3rd try
    return ERR_INVALID_INPUT;
}

int retry_operation(int max_tries) {
    int i;
    int err;
    retry_counter = 0;
    for (i = 0; i < max_tries; i = i + 1) {
        err = flaky_operation();
        if (err == ERR_OK) return i + 1;
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Error accumulator: track multiple errors
// -----------------------------------------------------------------------------
int error_log[20];
int error_count;

void error_log_init(void) {
    int i;
    error_count = 0;
    for (i = 0; i < 20; i = i + 1) error_log[i] = 0;
}

void log_error(int code) {
    if (error_count < 20) {
        error_log[error_count] = code;
        error_count = error_count + 1;
    }
}

int get_error_count(void) {
    return error_count;
}

int get_last_error(void) {
    if (error_count == 0) return ERR_OK;
    return error_log[error_count - 1];
}

int has_error(int code) {
    int i;
    for (i = 0; i < error_count; i = i + 1) {
        if (error_log[i] == code) return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Safe string length (with max limit)
// -----------------------------------------------------------------------------
int safe_strlen(char *s, int max_len) {
    int i;
    if (s == 0) return -1;
    i = 0;
    while (i < max_len && s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// -----------------------------------------------------------------------------
// Validate index helper
// -----------------------------------------------------------------------------
int validate_index(int idx, int size) {
    if (idx < 0) return ERR_OUT_OF_BOUNDS;
    if (idx >= size) return ERR_OUT_OF_BOUNDS;
    return ERR_OK;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int result;
    int err;
    int v;
    int i;
    int arr[10];
    char str[10];

    test_num = 0;

    // ---- Safe division ----
    err = safe_divide(10, 3, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0 (OK)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 3

    err = safe_divide(10, 0, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 5 (DIVIDE_BY_ZERO)

    err = safe_divide(10, 3, 0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 1 (NULL_PTR)

    // ---- Safe modulo ----
    err = safe_mod(10, 3, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 1

    err = safe_mod(10, 0, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 5

    // ---- Safe array access ----
    safe_arr_size = 5;
    safe_arr[0] = 100; safe_arr[1] = 200; safe_arr[2] = 300;
    safe_arr[3] = 400; safe_arr[4] = 500;

    err = safe_get(0, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 100

    err = safe_get(4, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 500

    err = safe_get(5, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 2 (OUT_OF_BOUNDS)

    err = safe_get(-1, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 2

    err = safe_set(2, 999);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    safe_get(2, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 999

    err = safe_set(10, 0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 2

    // ---- Range checks ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_in_range(5, 1, 10)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_in_range(0, 1, 10)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_in_range(10, 1, 10)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, clamp(5, 1, 10)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, clamp(-5, 1, 10)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, clamp(15, 1, 10)); // 10

    // ---- Stack tests ----
    stack_init();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stack_is_empty()); // 1

    // Pop from empty
    err = stack_pop(&result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 8 (EMPTY)

    // Push and pop
    stack_push(10);
    stack_push(20);
    stack_push(30);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stack_size()); // 3

    err = stack_peek(&result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 30

    err = stack_pop(&result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 30

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stack_size()); // 2

    // ---- Queue tests ----
    queue_init();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, queue_is_empty()); // 1

    // Dequeue from empty
    err = queue_dequeue(&result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 8 (EMPTY)

    queue_enqueue(100);
    queue_enqueue(200);
    queue_enqueue(300);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, queue_size()); // 3

    err = queue_dequeue(&result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 100 (FIFO)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, queue_size()); // 2

    // ---- Sorted check ----
    arr[0] = 1; arr[1] = 3; arr[2] = 5; arr[3] = 7;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_sorted(arr, 4)); // 1

    arr[2] = 2;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_sorted(arr, 4)); // 0

    // ---- Safe binary search ----
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    err = safe_binary_search(arr, 5, 30, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 2

    err = safe_binary_search(arr, 5, 99, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 9 (NOT_FOUND)

    // ---- Safe add ----
    err = safe_add(100, 200, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 300

    // ---- Safe multiply ----
    err = safe_multiply(6, 7, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 42

    err = safe_multiply(0, 999, &result);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, err); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 0

    // ---- Sanitize enum ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sanitize_enum(3, 9)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sanitize_enum(-1, 9)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sanitize_enum(15, 9)); // 9

    // ---- Retry pattern ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, retry_operation(5)); // 3 (succeeds on 3rd try)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, retry_operation(2)); // -1 (not enough tries)

    // ---- Error log ----
    error_log_init();
    log_error(ERR_DIVIDE_BY_ZERO);
    log_error(ERR_OUT_OF_BOUNDS);
    log_error(ERR_NULL_PTR);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, get_error_count()); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, get_last_error()); // 1 (NULL_PTR)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_error(ERR_DIVIDE_BY_ZERO)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_error(ERR_OVERFLOW)); // 0

    // ---- Safe strlen ----
    str[0] = 104; str[1] = 105; str[2] = 0; // "hi"
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, safe_strlen(str, 10)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, safe_strlen(0, 10)); // -1

    // ---- Validate index ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, validate_index(3, 10)); // 0 (OK)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, validate_index(-1, 10)); // 2 (OUT_OF_BOUNDS)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, validate_index(10, 10)); // 2

    return 0;
}
