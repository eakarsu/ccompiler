int printf(const char *fmt, ...);

// Struct return stress: small, medium, large, nested, chained, immediate dot access.

struct Small {
    int x;
    int y;
};

struct Medium {
    int a;
    int b;
    int c;
    int d;
};

struct Large {
    int v[8];
};

struct Nested {
    struct Small s;
    int z;
};

struct Pair {
    int first;
    int second;
};

struct Triple {
    int a;
    int b;
    int c;
};

// Return small struct (2 fields, 8 bytes)
struct Small make_small(int x, int y) {
    struct Small s;
    s.x = x;
    s.y = y;
    return s;
}

// Return medium struct (4 fields, 16 bytes)
struct Medium make_medium(int a, int b, int c, int d) {
    struct Medium m;
    m.a = a;
    m.b = b;
    m.c = c;
    m.d = d;
    return m;
}

// Return large struct (8 ints = 32 bytes, > 16, via hidden pointer)
struct Large make_large(int base) {
    struct Large l;
    int i;
    for (i = 0; i < 8; i++) {
        l.v[i] = base + i;
    }
    return l;
}

// Return nested struct
struct Nested make_nested(int x, int y, int z) {
    struct Nested n;
    n.s.x = x;
    n.s.y = y;
    n.z = z;
    return n;
}

// Chain: function returning struct calls another function returning struct
struct Small add_small(struct Small a, struct Small b) {
    struct Small result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

struct Small triple_add(int x1, int y1, int x2, int y2, int x3, int y3) {
    struct Small a = make_small(x1, y1);
    struct Small b = make_small(x2, y2);
    struct Small ab = add_small(a, b);
    struct Small c = make_small(x3, y3);
    struct Small result = add_small(ab, c);
    return result;
}

// Struct return used in expression
int sum_of_small(int x, int y) {
    struct Small s = make_small(x, y);
    return s.x + s.y;
}

// Multiple struct returns in one expression
int multi_struct_expr(void) {
    struct Small a = make_small(10, 20);
    struct Small b = make_small(30, 40);
    return a.x * b.y - a.y * b.x;
    // 10*40 - 20*30 = 400 - 600 = -200
}

// Struct return as function argument
struct Small double_small(struct Small s) {
    return make_small(s.x * 2, s.y * 2);
}

// Pass struct return directly (without intermediate variable)
int pass_return_directly(void) {
    struct Small d = double_small(make_small(5, 15));
    return d.x + d.y;  // 10 + 30 = 40
}

// Nested struct access through return
int nested_access(void) {
    struct Nested n = make_nested(100, 200, 300);
    return n.s.x + n.s.y + n.z;  // 100 + 200 + 300 = 600
}

// Large struct operations
int large_struct_sum(void) {
    struct Large l = make_large(10);
    int sum = 0;
    int i;
    for (i = 0; i < 8; i++) {
        sum += l.v[i];
    }
    // 10+11+12+13+14+15+16+17 = 108
    return sum;
}

// Modify and return struct
struct Small shift_small(struct Small s, int dx, int dy) {
    struct Small result;
    result.x = s.x + dx;
    result.y = s.y + dy;
    return result;
}

int chain_shifts(void) {
    struct Small s = make_small(0, 0);
    s = shift_small(s, 10, 20);
    s = shift_small(s, 30, 40);
    s = shift_small(s, 50, 60);
    return s.x * 1000 + s.y;
    // x = 0+10+30+50=90, y = 0+20+40+60=120 => 90120
}

// Struct in conditional
struct Small pick_small(int which) {
    if (which == 0) {
        return make_small(111, 222);
    } else {
        return make_small(333, 444);
    }
}

// Recursive struct return
struct Pair fib_pair(int n) {
    struct Pair p;
    if (n <= 0) {
        p.first = 0;
        p.second = 1;
        return p;
    }
    struct Pair prev = fib_pair(n - 1);
    p.first = prev.second;
    p.second = prev.first + prev.second;
    return p;
}

// Struct return in loop
struct Small accumulate(int n) {
    struct Small s = make_small(0, 0);
    int i;
    for (i = 1; i <= n; i++) {
        s = add_small(s, make_small(i, i * i));
    }
    return s;
}

int main(void) {
    // Small struct
    struct Small s = make_small(42, 99);
    printf("%d %d\n", s.x, s.y);
    // EXPECT: 42 99

    // Medium struct
    struct Medium m = make_medium(1, 2, 3, 4);
    printf("%d %d %d %d\n", m.a, m.b, m.c, m.d);
    // EXPECT: 1 2 3 4

    // Large struct
    struct Large l = make_large(100);
    printf("%d %d %d %d\n", l.v[0], l.v[3], l.v[5], l.v[7]);
    // EXPECT: 100 103 105 107

    // Nested struct
    struct Nested n = make_nested(10, 20, 30);
    printf("%d %d %d\n", n.s.x, n.s.y, n.z);
    // EXPECT: 10 20 30

    // Triple chain add
    struct Small t = triple_add(1, 2, 3, 4, 5, 6);
    printf("%d %d\n", t.x, t.y);
    // EXPECT: 9 12

    // Sum of small
    printf("%d\n", sum_of_small(100, 200));
    // EXPECT: 300

    // Multi struct expr
    printf("%d\n", multi_struct_expr());
    // EXPECT: -200

    // Pass return directly
    printf("%d\n", pass_return_directly());
    // EXPECT: 40

    // Nested access
    printf("%d\n", nested_access());
    // EXPECT: 600

    // Large struct sum
    printf("%d\n", large_struct_sum());
    // EXPECT: 108

    // Chain shifts
    printf("%d\n", chain_shifts());
    // EXPECT: 90120

    // Pick small
    struct Small p0 = pick_small(0);
    struct Small p1 = pick_small(1);
    printf("%d %d %d %d\n", p0.x, p0.y, p1.x, p1.y);
    // EXPECT: 111 222 333 444

    // Fibonacci pair: fib_pair(10) gives fib(10)=55, fib(11)=89
    struct Pair fp = fib_pair(10);
    printf("%d %d\n", fp.first, fp.second);
    // EXPECT: 55 89

    // Accumulate: x = 1+2+...+10 = 55, y = 1+4+9+16+25+36+49+64+81+100 = 385
    struct Small acc = accumulate(10);
    printf("%d %d\n", acc.x, acc.y);
    // EXPECT: 55 385

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
