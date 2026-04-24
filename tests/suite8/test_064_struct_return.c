int printf(const char *fmt, ...);

struct Pair {
    int first;
    int second;
};

struct Triple {
    int a;
    int b;
    int c;
};

struct Result {
    int value;
    int error;
};

struct Pair make_pair(int a, int b) {
    struct Pair p;
    p.first = a;
    p.second = b;
    return p;
}

struct Pair swap_pair(struct Pair *p) {
    struct Pair result;
    result.first = p->second;
    result.second = p->first;
    return result;
}

struct Pair add_pairs(struct Pair *a, struct Pair *b) {
    struct Pair result;
    result.first = a->first + b->first;
    result.second = a->second + b->second;
    return result;
}

struct Triple make_triple(int a, int b, int c) {
    struct Triple t;
    t.a = a;
    t.b = b;
    t.c = c;
    return t;
}

struct Triple rotate_triple(struct Triple *t) {
    struct Triple result;
    result.a = t->b;
    result.b = t->c;
    result.c = t->a;
    return result;
}

struct Result safe_divide(int num, int den) {
    struct Result r;
    if (den == 0) {
        r.value = 0;
        r.error = 1;
    } else {
        r.value = num / den;
        r.error = 0;
    }
    return r;
}

struct Pair min_max(int *arr, int n) {
    struct Pair result;
    result.first = arr[0];
    result.second = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < result.first) result.first = arr[i];
        if (arr[i] > result.second) result.second = arr[i];
    }
    return result;
}

struct Result factorial_safe(int n) {
    struct Result r;
    if (n < 0) {
        r.value = 0;
        r.error = 1;
        return r;
    }
    int result = 1;
    int i;
    for (i = 2; i <= n; i = i + 1) {
        result = result * i;
    }
    r.value = result;
    r.error = 0;
    return r;
}

struct Pair fibonacci_pair(int n) {
    int a = 0;
    int b = 1;
    int i;
    for (i = 0; i < n; i = i + 1) {
        int temp = b;
        b = a + b;
        a = temp;
    }
    struct Pair p;
    p.first = a;
    p.second = b;
    return p;
}

int main() {
    struct Pair p1 = make_pair(10, 20);
    // EXPECT: Pair: (10, 20)
    printf("Pair: (%d, %d)\n", p1.first, p1.second);

    struct Pair p2 = swap_pair(&p1);
    // EXPECT: Swapped: (20, 10)
    printf("Swapped: (%d, %d)\n", p2.first, p2.second);

    struct Pair p3 = add_pairs(&p1, &p2);
    // EXPECT: Added: (30, 30)
    printf("Added: (%d, %d)\n", p3.first, p3.second);

    struct Triple t1 = make_triple(1, 2, 3);
    // EXPECT: Triple: (1, 2, 3)
    printf("Triple: (%d, %d, %d)\n", t1.a, t1.b, t1.c);

    struct Triple t2 = rotate_triple(&t1);
    // EXPECT: Rotated: (2, 3, 1)
    printf("Rotated: (%d, %d, %d)\n", t2.a, t2.b, t2.c);

    struct Triple t3 = rotate_triple(&t2);
    // EXPECT: Rotated2: (3, 1, 2)
    printf("Rotated2: (%d, %d, %d)\n", t3.a, t3.b, t3.c);

    struct Result r1 = safe_divide(100, 7);
    // EXPECT: 100/7 = 14, err=0
    printf("100/7 = %d, err=%d\n", r1.value, r1.error);

    struct Result r2 = safe_divide(50, 0);
    // EXPECT: 50/0 = 0, err=1
    printf("50/0 = %d, err=%d\n", r2.value, r2.error);

    int arr[7];
    arr[0] = 34; arr[1] = 12; arr[2] = 56; arr[3] = 7;
    arr[4] = 89; arr[5] = 23; arr[6] = 45;
    struct Pair mm = min_max(arr, 7);
    // EXPECT: Min=7, Max=89
    printf("Min=%d, Max=%d\n", mm.first, mm.second);

    struct Result f5 = factorial_safe(5);
    // EXPECT: 5! = 120, err=0
    printf("5! = %d, err=%d\n", f5.value, f5.error);

    struct Result fn = factorial_safe(-1);
    // EXPECT: (-1)! = 0, err=1
    printf("(-1)! = %d, err=%d\n", fn.value, fn.error);

    struct Pair fib = fibonacci_pair(10);
    // EXPECT: Fib(10): curr=55, next=89
    printf("Fib(10): curr=%d, next=%d\n", fib.first, fib.second);

    struct Pair fib2 = fibonacci_pair(7);
    // EXPECT: Fib(7): curr=13, next=21
    printf("Fib(7): curr=%d, next=%d\n", fib2.first, fib2.second);

    return 0;
}
