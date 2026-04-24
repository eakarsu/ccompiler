int printf(const char *fmt, ...);
// EXPECT: 1:42\n2:100\n3:20\n4:10\n5:55\n6:10\n7:1\n8:40\n9:5\n10:4\n11:1\n12:99\n13:99\n14:3\n15:1\n16:0\n17:10\n18:20\n19:30\n20:77\n21:200\n22:100\n23:10\n24:20\n25:30\n26:20\n27:10\n28:15\n29:7\n30:21\n31:5\n32:0\n33:5\n34:200\n35:300\n36:999\n37:32\n38:2\n39:-1\n40:11\n41:12\n42:13\n43:30\n44:40\n45:20\n46:4
// =============================================================================
// TEST 028: Pointer Arithmetic
// =============================================================================
// Tests pointer increment/decrement, pointer difference, array traversal via
// pointers, pointer to struct members, double pointers, and various pointer
// manipulation patterns.
// =============================================================================

// -----------------------------------------------------------------------------
// Global arrays for pointer tests
// -----------------------------------------------------------------------------
int g_arr[20];
int g_matrix[5][5];
char g_str[50];

// -----------------------------------------------------------------------------
// Struct for pointer-to-struct tests
// -----------------------------------------------------------------------------
struct Pair {
    int first;
    int second;
};

struct Triple {
    int a;
    int b;
    int c;
};

// -----------------------------------------------------------------------------
// Sum array using pointer traversal
// -----------------------------------------------------------------------------
int sum_via_ptr(int *arr, int n) {
    int *p;
    int *end;
    int sum;
    sum = 0;
    p = arr;
    end = arr + n;
    while (p < end) {
        sum = sum + *p;
        p = p + 1;
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Find max using pointer traversal
// -----------------------------------------------------------------------------
int max_via_ptr(int *arr, int n) {
    int *p;
    int mx;
    int i;
    mx = arr[0];
    p = arr + 1;
    for (i = 1; i < n; i = i + 1) {
        if (*p > mx) mx = *p;
        p = p + 1;
    }
    return mx;
}

// -----------------------------------------------------------------------------
// Find min using pointer traversal
// -----------------------------------------------------------------------------
int min_via_ptr(int *arr, int n) {
    int *p;
    int mn;
    int i;
    mn = arr[0];
    p = arr + 1;
    for (i = 1; i < n; i = i + 1) {
        if (*p < mn) mn = *p;
        p = p + 1;
    }
    return mn;
}

// -----------------------------------------------------------------------------
// Reverse array in place using pointers
// -----------------------------------------------------------------------------
void reverse_via_ptr(int *arr, int n) {
    int *left;
    int *right;
    int tmp;
    left = arr;
    right = arr + n - 1;
    while (left < right) {
        tmp = *left;
        *left = *right;
        *right = tmp;
        left = left + 1;
        right = right - 1;
    }
}

// -----------------------------------------------------------------------------
// Swap via pointers
// -----------------------------------------------------------------------------
void swap_ptr(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

// -----------------------------------------------------------------------------
// Fill array via pointer
// -----------------------------------------------------------------------------
void fill_via_ptr(int *arr, int n, int val) {
    int *p;
    int *end;
    p = arr;
    end = arr + n;
    while (p < end) {
        *p = val;
        p = p + 1;
    }
}

// -----------------------------------------------------------------------------
// Count occurrences via pointer
// -----------------------------------------------------------------------------
int count_via_ptr(int *arr, int n, int val) {
    int *p;
    int *end;
    int count;
    count = 0;
    p = arr;
    end = arr + n;
    while (p < end) {
        if (*p == val) count = count + 1;
        p = p + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Copy array via pointers
// -----------------------------------------------------------------------------
void copy_via_ptr(int *dest, int *src, int n) {
    int *dp;
    int *sp;
    int *end;
    dp = dest;
    sp = src;
    end = src + n;
    while (sp < end) {
        *dp = *sp;
        dp = dp + 1;
        sp = sp + 1;
    }
}

// -----------------------------------------------------------------------------
// Double pointer: modify what a pointer points to
// -----------------------------------------------------------------------------
void set_value(int **pp, int val) {
    **pp = val;
}

// -----------------------------------------------------------------------------
// Double pointer: swap what two pointers point to
// -----------------------------------------------------------------------------
void swap_ptrs(int **pp1, int **pp2) {
    int *tmp;
    tmp = *pp1;
    *pp1 = *pp2;
    *pp2 = tmp;
}

// -----------------------------------------------------------------------------
// Struct pointer operations
// -----------------------------------------------------------------------------
void pair_init(struct Pair *p, int f, int s) {
    p->first = f;
    p->second = s;
}

int pair_sum(struct Pair *p) {
    return p->first + p->second;
}

void pair_swap(struct Pair *p) {
    int tmp;
    tmp = p->first;
    p->first = p->second;
    p->second = tmp;
}

void triple_init(struct Triple *t, int a, int b, int c) {
    t->a = a;
    t->b = b;
    t->c = c;
}

int triple_sum(struct Triple *t) {
    return t->a + t->b + t->c;
}

int triple_max(struct Triple *t) {
    int mx;
    mx = t->a;
    if (t->b > mx) mx = t->b;
    if (t->c > mx) mx = t->c;
    return mx;
}

// -----------------------------------------------------------------------------
// Array of struct pointers (using array of structs)
// -----------------------------------------------------------------------------
struct Pair pairs[10];

int sum_all_pairs(int n) {
    int i;
    int total;
    total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + pair_sum(&pairs[i]);
    }
    return total;
}

// -----------------------------------------------------------------------------
// Pointer arithmetic on char arrays (string-like operations)
// -----------------------------------------------------------------------------
int strlen_ptr(char *s) {
    char *p;
    p = s;
    while (*p != 0) {
        p = p + 1;
    }
    return p - s;
}

void strcpy_ptr(char *dest, char *src) {
    while (*src != 0) {
        *dest = *src;
        dest = dest + 1;
        src = src + 1;
    }
    *dest = 0;
}

int strcmp_ptr(char *s1, char *s2) {
    while (*s1 != 0 && *s2 != 0) {
        if (*s1 != *s2) return *s1 - *s2;
        s1 = s1 + 1;
        s2 = s2 + 1;
    }
    return *s1 - *s2;
}

// -----------------------------------------------------------------------------
// Pointer to array element access patterns
// -----------------------------------------------------------------------------
int access_pattern_1(int *arr, int idx) {
    return *(arr + idx);
}

int access_pattern_2(int *arr, int idx) {
    int *p;
    p = arr;
    p = p + idx;
    return *p;
}

// -----------------------------------------------------------------------------
// Modify array through pointer with offset
// -----------------------------------------------------------------------------
void modify_at(int *arr, int idx, int val) {
    *(arr + idx) = val;
}

// -----------------------------------------------------------------------------
// Compute dot product using pointers
// -----------------------------------------------------------------------------
int dot_product(int *a, int *b, int n) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < n; i = i + 1) {
        result = result + (*(a + i)) * (*(b + i));
    }
    return result;
}

// -----------------------------------------------------------------------------
// Find element using pointer, return pointer offset from start
// Returns -1 if not found
// -----------------------------------------------------------------------------
int find_element(int *arr, int n, int val) {
    int *p;
    int *end;
    p = arr;
    end = arr + n;
    while (p < end) {
        if (*p == val) return p - arr;
        p = p + 1;
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Increment all elements via pointer
// -----------------------------------------------------------------------------
void increment_all(int *arr, int n, int delta) {
    int *p;
    int *end;
    p = arr;
    end = arr + n;
    while (p < end) {
        *p = *p + delta;
        p = p + 1;
    }
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int arr[10];
    int arr2[10];
    int a;
    int b;
    int *pa;
    int *pb;
    int v;
    struct Pair pr;
    struct Triple tr;
    char s1[20];
    char s2[20];
    int i;

    test_num = 0;

    // ---- Basic pointer tests ----
    a = 42;
    pa = &a;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *pa); // 42

    *pa = 100;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, a); // 100

    // ---- Swap via pointers ----
    a = 10;
    b = 20;
    swap_ptr(&a, &b);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, a); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, b); // 10

    // ---- Array traversal via pointers ----
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    arr[5] = 6; arr[6] = 7; arr[7] = 8; arr[8] = 9; arr[9] = 10;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_via_ptr(arr, 10)); // 55

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, max_via_ptr(arr, 10)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, min_via_ptr(arr, 10)); // 1

    // Sum of partial array
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_via_ptr(arr + 5, 5)); // 40

    // ---- Reverse via pointers ----
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    reverse_via_ptr(arr, 5);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[0]); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[1]); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[4]); // 1

    // ---- Fill via pointer ----
    fill_via_ptr(arr, 5, 99);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[0]); // 99

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[4]); // 99

    // ---- Count via pointer ----
    arr[0] = 1; arr[1] = 2; arr[2] = 1; arr[3] = 3; arr[4] = 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_via_ptr(arr, 5, 1)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_via_ptr(arr, 5, 2)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_via_ptr(arr, 5, 9)); // 0

    // ---- Copy via pointer ----
    arr[0] = 10; arr[1] = 20; arr[2] = 30;
    copy_via_ptr(arr2, arr, 3);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr2[0]); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr2[1]); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr2[2]); // 30

    // ---- Double pointer tests ----
    a = 50;
    pa = &a;
    set_value(&pa, 77);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, a); // 77

    // Swap pointers
    a = 100;
    b = 200;
    pa = &a;
    pb = &b;
    swap_ptrs(&pa, &pb);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *pa); // 200

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *pb); // 100

    // ---- Struct pointer tests ----
    pair_init(&pr, 10, 20);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, pr.first); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, pr.second); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, pair_sum(&pr)); // 30

    pair_swap(&pr);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, pr.first); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, pr.second); // 10

    // Triple
    triple_init(&tr, 3, 7, 5);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, triple_sum(&tr)); // 15

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, triple_max(&tr)); // 7

    // ---- Array of structs via pointers ----
    pair_init(&pairs[0], 1, 2);
    pair_init(&pairs[1], 3, 4);
    pair_init(&pairs[2], 5, 6);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_all_pairs(3)); // 21

    // ---- String pointer operations ----
    s1[0] = 104; s1[1] = 101; s1[2] = 108; s1[3] = 108; s1[4] = 111; s1[5] = 0; // "hello"
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, strlen_ptr(s1)); // 5

    strcpy_ptr(s2, s1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, strcmp_ptr(s1, s2)); // 0 (equal)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, strlen_ptr(s2)); // 5

    // ---- Access patterns ----
    arr[0] = 100; arr[1] = 200; arr[2] = 300;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, access_pattern_1(arr, 1)); // 200

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, access_pattern_2(arr, 2)); // 300

    // ---- Modify via pointer with offset ----
    modify_at(arr, 0, 999);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[0]); // 999

    // ---- Dot product ----
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    arr2[0] = 4; arr2[1] = 5; arr2[2] = 6;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dot_product(arr, arr2, 3)); // 32

    // ---- Find element ----
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_element(arr, 5, 30)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_element(arr, 5, 99)); // -1

    // ---- Increment all ----
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    increment_all(arr, 3, 10);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[0]); // 11

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[1]); // 12

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, arr[2]); // 13

    // ---- Pointer to middle of array ----
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    pa = arr + 2;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *pa); // 30

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *(pa + 1)); // 40

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, *(pa - 1)); // 20

    // Pointer difference
    pa = arr;
    pb = arr + 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, (int)(pb - pa)); // 4

    return 0;
}
