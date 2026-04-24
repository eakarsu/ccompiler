int printf(const char *fmt, ...);

// Test 076: Defensive programming patterns - assertions, preconditions, invariants

struct IntStack {
    int items[16];
    int top;
    int max_size;
    int push_count;
    int pop_count;
    int error_count;
};

int assert_check(int condition, char *msg, int *error_count) {
    if (!condition) {
        printf("ASSERT FAIL: %s\n", msg);
        (*error_count)++;
        return 0;
    }
    return 1;
}

void stack_init(struct IntStack *s) {
    s->top = -1;
    s->max_size = 16;
    s->push_count = 0;
    s->pop_count = 0;
    s->error_count = 0;
    for (int i = 0; i < 16; i++) {
        s->items[i] = 0;
    }
}

int stack_invariant(struct IntStack *s) {
    if (s->top < -1) return 0;
    if (s->top >= s->max_size) return 0;
    if (s->push_count < s->pop_count) return 0;
    if (s->top + 1 != s->push_count - s->pop_count) return 0;
    return 1;
}

int stack_push(struct IntStack *s, int val) {
    if (!assert_check(s->top < s->max_size - 1, "stack not full", &s->error_count)) {
        return -1;
    }
    s->top++;
    s->items[s->top] = val;
    s->push_count++;

    if (!assert_check(stack_invariant(s), "invariant after push", &s->error_count)) {
        return -2;
    }
    return 0;
}

int stack_pop(struct IntStack *s, int *val) {
    if (!assert_check(s->top >= 0, "stack not empty", &s->error_count)) {
        return -1;
    }
    *val = s->items[s->top];
    s->top--;
    s->pop_count++;

    if (!assert_check(stack_invariant(s), "invariant after pop", &s->error_count)) {
        return -2;
    }
    return 0;
}

int stack_peek(struct IntStack *s, int *val) {
    if (!assert_check(s->top >= 0, "stack not empty for peek", &s->error_count)) {
        return -1;
    }
    *val = s->items[s->top];
    return 0;
}

int stack_size(struct IntStack *s) {
    return s->top + 1;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int safe_index(int idx, int size) {
    if (size <= 0) return 0;
    if (idx < 0) return 0;
    if (idx >= size) return size - 1;
    return idx;
}

int require_positive(int val, int *err) {
    if (val <= 0) {
        *err = 1;
        return 0;
    }
    *err = 0;
    return val;
}

int require_in_range(int val, int lo, int hi, int *err) {
    if (val < lo || val > hi) {
        *err = 1;
        return clamp(val, lo, hi);
    }
    *err = 0;
    return val;
}

int main() {
    struct IntStack s;
    stack_init(&s);

    printf("Initial invariant: %d\n", stack_invariant(&s));
    // EXPECT: Initial invariant: 1

    for (int i = 0; i < 5; i++) {
        stack_push(&s, (i + 1) * 10);
    }
    printf("After 5 pushes: size=%d invariant=%d\n", stack_size(&s), stack_invariant(&s));
    // EXPECT: After 5 pushes: size=5 invariant=1

    int val = 0;
    stack_peek(&s, &val);
    printf("Peek: %d\n", val);
    // EXPECT: Peek: 50

    stack_pop(&s, &val);
    printf("Pop: %d size=%d\n", val, stack_size(&s));
    // EXPECT: Pop: 50 size=4

    stack_pop(&s, &val);
    printf("Pop: %d size=%d\n", val, stack_size(&s));
    // EXPECT: Pop: 40 size=3

    printf("Invariant: %d errors: %d\n", stack_invariant(&s), s.error_count);
    // EXPECT: Invariant: 1 errors: 0

    printf("Stats: pushes=%d pops=%d\n", s.push_count, s.pop_count);
    // EXPECT: Stats: pushes=5 pops=2

    while (s.top >= 0) {
        stack_pop(&s, &val);
    }
    printf("After drain: size=%d\n", stack_size(&s));
    // EXPECT: After drain: size=0

    stack_pop(&s, &val);
    // EXPECT: ASSERT FAIL: stack not empty
    printf("Error count: %d\n", s.error_count);
    // EXPECT: Error count: 1

    stack_peek(&s, &val);
    // EXPECT: ASSERT FAIL: stack not empty for peek
    printf("Error count: %d\n", s.error_count);
    // EXPECT: Error count: 2

    printf("Clamp tests:\n");
    // EXPECT: Clamp tests:
    printf("  clamp(-5,0,10)=%d\n", clamp(-5, 0, 10));
    // EXPECT:   clamp(-5,0,10)=0
    printf("  clamp(15,0,10)=%d\n", clamp(15, 0, 10));
    // EXPECT:   clamp(15,0,10)=10
    printf("  clamp(5,0,10)=%d\n", clamp(5, 0, 10));
    // EXPECT:   clamp(5,0,10)=5

    printf("Safe index tests:\n");
    // EXPECT: Safe index tests:
    printf("  safe(-1,5)=%d\n", safe_index(-1, 5));
    // EXPECT:   safe(-1,5)=0
    printf("  safe(10,5)=%d\n", safe_index(10, 5));
    // EXPECT:   safe(10,5)=4
    printf("  safe(3,5)=%d\n", safe_index(3, 5));
    // EXPECT:   safe(3,5)=3

    int err = 0;
    int v = require_positive(5, &err);
    printf("require_positive(5): v=%d err=%d\n", v, err);
    // EXPECT: require_positive(5): v=5 err=0
    v = require_positive(-3, &err);
    printf("require_positive(-3): v=%d err=%d\n", v, err);
    // EXPECT: require_positive(-3): v=0 err=1

    v = require_in_range(50, 0, 100, &err);
    printf("in_range(50,0,100): v=%d err=%d\n", v, err);
    // EXPECT: in_range(50,0,100): v=50 err=0
    v = require_in_range(200, 0, 100, &err);
    printf("in_range(200,0,100): v=%d err=%d\n", v, err);
    // EXPECT: in_range(200,0,100): v=100 err=1

    printf("Defensive patterns done\n");
    // EXPECT: Defensive patterns done
    return 0;
}
