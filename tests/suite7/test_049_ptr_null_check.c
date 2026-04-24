int printf(const char *fmt, ...);

struct Node {
    int value;
    struct Node *next;
};

int safe_deref(int *p) {
    if (p != 0) {
        return *p;
    }
    return -1;
}

int count_chain(struct Node *head) {
    int count = 0;
    struct Node *cur = head;
    while (cur != 0) {
        count = count + 1;
        cur = cur->next;
    }
    return count;
}

int sum_chain(struct Node *head) {
    int sum = 0;
    struct Node *cur = head;
    while (cur != 0) {
        sum = sum + cur->value;
        cur = cur->next;
    }
    return sum;
}

int main(void) {
    // Basic null check
    int *np = 0;
    int is_null = (np == 0);
    // EXPECT: is_null = 1
    printf("is_null = %d\n", is_null);

    int val = 42;
    int *vp = &val;
    int is_not_null = (vp != 0);
    // EXPECT: is_not_null = 1
    printf("is_not_null = %d\n", is_not_null);

    // Null as false in boolean context
    int null_as_bool = 0;
    if (np) {
        null_as_bool = 1;
    }
    // EXPECT: null_as_bool = 0
    printf("null_as_bool = %d\n", null_as_bool);

    int nonnull_as_bool = 0;
    if (vp) {
        nonnull_as_bool = 1;
    }
    // EXPECT: nonnull_as_bool = 1
    printf("nonnull_as_bool = %d\n", nonnull_as_bool);

    // Safe dereference function
    int x = 100;
    // EXPECT: safe_val = 100
    printf("safe_val = %d\n", safe_deref(&x));

    // EXPECT: safe_null = -1
    printf("safe_null = %d\n", safe_deref(0));

    // Null as sentinel in array of pointers
    int a = 10;
    int b = 20;
    int c = 30;
    int *ptrs[4];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    ptrs[3] = 0;

    int sum = 0;
    int i = 0;
    while (ptrs[i] != 0) {
        sum = sum + *ptrs[i];
        i = i + 1;
    }
    // EXPECT: sentinel_sum = 60
    printf("sentinel_sum = %d\n", sum);

    // EXPECT: sentinel_count = 3
    printf("sentinel_count = %d\n", i);

    // Null pointer in struct
    struct Node n3;
    n3.value = 30;
    n3.next = 0;

    struct Node n2;
    n2.value = 20;
    n2.next = &n3;

    struct Node n1;
    n1.value = 10;
    n1.next = &n2;

    // EXPECT: chain_count = 3
    printf("chain_count = %d\n", count_chain(&n1));

    // EXPECT: chain_sum = 60
    printf("chain_sum = %d\n", sum_chain(&n1));

    // Single node chain
    // EXPECT: single_count = 1
    printf("single_count = %d\n", count_chain(&n3));

    // EXPECT: single_sum = 30
    printf("single_sum = %d\n", sum_chain(&n3));

    // Null chain
    // EXPECT: null_count = 0
    printf("null_count = %d\n", count_chain(0));

    // EXPECT: null_sum = 0
    printf("null_sum = %d\n", sum_chain(0));

    // Ternary with null check
    int *maybe = 0;
    int result = (maybe != 0) ? *maybe : -999;
    // EXPECT: ternary_null = -999
    printf("ternary_null = %d\n", result);

    maybe = &val;
    result = (maybe != 0) ? *maybe : -999;
    // EXPECT: ternary_valid = 42
    printf("ternary_valid = %d\n", result);

    // Assign null then reassign
    int *evolve = 0;
    // EXPECT: evolve_null = 1
    printf("evolve_null = %d\n", evolve == 0);

    int ev_val = 777;
    evolve = &ev_val;
    // EXPECT: evolve_val = 777
    printf("evolve_val = %d\n", *evolve);

    evolve = 0;
    // EXPECT: evolve_null_again = 1
    printf("evolve_null_again = %d\n", evolve == 0);

    // Find first non-null
    int *candidates[5];
    candidates[0] = 0;
    candidates[1] = 0;
    candidates[2] = &a;
    candidates[3] = &b;
    candidates[4] = 0;

    int *found = 0;
    for (i = 0; i < 5; i++) {
        if (candidates[i] != 0) {
            found = candidates[i];
            break;
        }
    }
    // EXPECT: first_nonnull = 10
    printf("first_nonnull = %d\n", (found != 0) ? *found : -1);

    return 0;
}
