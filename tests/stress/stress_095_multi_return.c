// stress_095_multi_return.c - Multiple return paths torture test
// Exercises: functions with 10+ return statements, computed return values,
// struct-based error/value returns, deeply nested return paths.
// EXPECT: classify(-100)=-3
// EXPECT: classify(-1)=-1
// EXPECT: classify(0)=0
// EXPECT: classify(1)=1
// EXPECT: classify(5)=2
// EXPECT: classify(13)=3
// EXPECT: classify(50)=4
// EXPECT: classify(100)=5
// EXPECT: classify(500)=6
// EXPECT: classify(1000)=7
// EXPECT: classify(5000)=8
// EXPECT: classify(99999)=9
// EXPECT: maze(1,1)=11
// EXPECT: maze(2,3)=23
// EXPECT: maze(3,2)=32
// EXPECT: maze(4,4)=404
// EXPECT: maze(5,3)=502
// EXPECT: err_ok val=126
// EXPECT: err_fail code=-1
// EXPECT: err_ok val=200
// EXPECT: err_fail code=-2
// EXPECT: chain(0)=157
// EXPECT: chain(3)=157
// EXPECT: chain(7)=150
// EXPECT: chain(15)=1
// EXPECT: multiret=55
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

// Function with 12 return paths based on value ranges
int classify(int x) {
    if (x < -10) return -3;
    if (x < -5) return -2;
    if (x < 0) return -1;
    if (x == 0) return 0;
    if (x < 5) return 1;
    if (x < 10) return 2;
    if (x < 50) return 3;
    if (x < 100) return 4;
    if (x < 500) return 5;
    if (x < 1000) return 6;
    if (x < 5000) return 7;
    if (x < 10000) return 8;
    return 9;
}

// Nested return maze: multiple return paths from nested conditions
int maze(int x, int y) {
    if (x == 1) {
        if (y == 1) return 11;
        if (y == 2) return 12;
        if (y == 3) return 13;
        return 10 + y;
    }
    if (x == 2) {
        if (y < 3) return 20 + y;
        if (y == 3) return 23;
        if (y > 5) return 200 + y;
        return 20 + y;
    }
    if (x == 3) {
        switch (y) {
            case 1: return 31;
            case 2: return 32;
            case 3: return 33;
            default: return 30 + y;
        }
    }
    if (x == 4) {
        if (y % 2 == 0) {
            if (y < 5) return 404;
            return 400 + y;
        }
        return 40 + y;
    }
    if (x == 5) {
        int i;
        for (i = 0; i < y; i++) {
            if (i == 0) continue;
            if (i == y - 1) return 500 + i;
        }
        return 500;
    }
    return -1;
}

// Error/value struct pattern
struct Result {
    int ok;
    int value;
    int error_code;
};

struct Result make_ok(int val) {
    struct Result r;
    r.ok = 1;
    r.value = val;
    r.error_code = 0;
    return r;
}

struct Result make_err(int code) {
    struct Result r;
    r.ok = 0;
    r.value = 0;
    r.error_code = code;
    return r;
}

struct Result try_divide(int a, int b) {
    if (b == 0) return make_err(-1);
    if (a < 0 && b < 0) return make_ok((-a) / (-b));
    if (a < 0) return make_err(-2);
    if (b < 0) return make_err(-3);
    return make_ok(a / b);
}

struct Result try_compute(int x) {
    if (x < 0) return make_err(-1);
    if (x == 0) return make_ok(0);
    if (x > 1000) return make_err(-2);
    int r = x * 2;
    if (r > 500) return make_ok(r / 5);
    r = r + x;
    if (r > 300) return make_ok(r / 3);
    return make_ok(r);
}

// Return from deeply nested chain
int chain(int depth) {
    if (depth >= 15) return 1;
    if (depth >= 12) {
        int v = chain(depth + 1);
        return v * 2 + 1;
    }
    if (depth >= 9) {
        int v = chain(depth + 1);
        if (v > 100) return v;
        return v * 3;
    }
    if (depth >= 6) {
        int v = chain(depth + 1);
        return v + depth;
    }
    if (depth >= 3) {
        int v = chain(depth + 1);
        if (v % 2 == 0) return v + 1;
        return v;
    }
    // depth 0-2
    int v = chain(depth + 1);
    return v;
}

// Many return paths via computed goto-like pattern
int multiret(int n) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (i == 3 && n == 3) return 6;
        if (i == 5 && n == 5) return 15;
        if (i == 7 && n == 7) return 28;
        sum = sum + i;
    }
    if (sum > 100) return sum / 2;
    if (sum > 50) return sum;
    if (sum > 25) return sum + 1;
    return sum;
}

int main(void) {
    int pass = 1;

    // classify with many return paths
    printf("classify(-100)=%d\n", classify(-100));
    if (classify(-100) != -3) pass = 0;
    printf("classify(-1)=%d\n", classify(-1));
    if (classify(-1) != -1) pass = 0;
    printf("classify(0)=%d\n", classify(0));
    if (classify(0) != 0) pass = 0;
    printf("classify(1)=%d\n", classify(1));
    if (classify(1) != 1) pass = 0;
    printf("classify(5)=%d\n", classify(5));
    if (classify(5) != 2) pass = 0;
    printf("classify(13)=%d\n", classify(13));
    if (classify(13) != 3) pass = 0;
    printf("classify(50)=%d\n", classify(50));
    if (classify(50) != 4) pass = 0;
    printf("classify(100)=%d\n", classify(100));
    if (classify(100) != 5) pass = 0;
    printf("classify(500)=%d\n", classify(500));
    if (classify(500) != 6) pass = 0;
    printf("classify(1000)=%d\n", classify(1000));
    if (classify(1000) != 7) pass = 0;
    printf("classify(5000)=%d\n", classify(5000));
    if (classify(5000) != 8) pass = 0;
    printf("classify(99999)=%d\n", classify(99999));
    if (classify(99999) != 9) pass = 0;

    // maze returns
    printf("maze(1,1)=%d\n", maze(1, 1));
    if (maze(1, 1) != 11) pass = 0;
    printf("maze(2,3)=%d\n", maze(2, 3));
    if (maze(2, 3) != 23) pass = 0;
    printf("maze(3,2)=%d\n", maze(3, 2));
    if (maze(3, 2) != 32) pass = 0;
    printf("maze(4,4)=%d\n", maze(4, 4));
    if (maze(4, 4) != 404) pass = 0;
    printf("maze(5,3)=%d\n", maze(5, 3));
    if (maze(5, 3) != 502) pass = 0;

    // error/value pattern
    struct Result r1 = try_compute(42);
    if (r1.ok) {
        printf("err_ok val=%d\n", r1.value);
        if (r1.value != 126) pass = 0;
    } else {
        printf("err_fail code=%d\n", r1.error_code);
        pass = 0;
    }

    struct Result r2 = try_divide(10, 0);
    if (r2.ok) {
        printf("err_ok val=%d\n", r2.value);
        pass = 0;
    } else {
        printf("err_fail code=%d\n", r2.error_code);
        if (r2.error_code != -1) pass = 0;
    }

    struct Result r3 = try_compute(500);
    if (r3.ok) {
        printf("err_ok val=%d\n", r3.value);
        if (r3.value != 200) pass = 0;
    } else {
        printf("err_fail code=%d\n", r3.error_code);
        pass = 0;
    }

    struct Result r4 = try_compute(5000);
    if (r4.ok) {
        printf("err_ok val=%d\n", r4.value);
        pass = 0;
    } else {
        printf("err_fail code=%d\n", r4.error_code);
        if (r4.error_code != -2) pass = 0;
    }

    // chain returns
    printf("chain(0)=%d\n", chain(0));
    if (chain(0) != 157) pass = 0;
    printf("chain(3)=%d\n", chain(3));
    if (chain(3) != 157) pass = 0;
    printf("chain(7)=%d\n", chain(7));
    if (chain(7) != 150) pass = 0;
    printf("chain(15)=%d\n", chain(15));
    if (chain(15) != 1) pass = 0;

    // multiret - sum of 1..10 = 55
    printf("multiret=%d\n", multiret(10));
    if (multiret(10) != 55) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
