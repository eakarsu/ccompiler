int printf(const char *fmt, ...);
// EXPECT: pair: (10, 20)\nsum: 30\nproduct: 200\nmax: 20\ndoubled: (20, 40)\noriginal: (10, 20)\ntriple: (3, 5, 7)\ntriple sum: 15\ntriple product: 105\ntriple min: 3\nequal: 1\nequal: 0
// Test: struct passed to function by value

struct Pair {
    int first;
    int second;
};

struct Triple {
    int a;
    int b;
    int c;
};

int pair_sum(struct Pair p) {
    return p.first + p.second;
}

int pair_product(struct Pair p) {
    return p.first * p.second;
}

int pair_max(struct Pair p) {
    if (p.first > p.second) return p.first;
    return p.second;
}

struct Pair pair_modify(struct Pair p) {
    p.first = p.first * 2;
    p.second = p.second * 2;
    return p;
}

int triple_sum(struct Triple t) {
    return t.a + t.b + t.c;
}

int triple_product(struct Triple t) {
    return t.a * t.b * t.c;
}

int triple_min(struct Triple t) {
    int min = t.a;
    if (t.b < min) min = t.b;
    if (t.c < min) min = t.c;
    return min;
}

int are_equal_pairs(struct Pair p1, struct Pair p2) {
    return (p1.first == p2.first) && (p1.second == p2.second);
}

int main(void) {
    struct Pair p;
    p.first = 10;
    p.second = 20;

    printf("pair: (%d, %d)\n", p.first, p.second);
    printf("sum: %d\n", pair_sum(p));
    printf("product: %d\n", pair_product(p));
    printf("max: %d\n", pair_max(p));

    struct Pair doubled = pair_modify(p);
    printf("doubled: (%d, %d)\n", doubled.first, doubled.second);
    printf("original: (%d, %d)\n", p.first, p.second);

    struct Triple t;
    t.a = 3;
    t.b = 5;
    t.c = 7;
    printf("triple: (%d, %d, %d)\n", t.a, t.b, t.c);
    printf("triple sum: %d\n", triple_sum(t));
    printf("triple product: %d\n", triple_product(t));
    printf("triple min: %d\n", triple_min(t));

    struct Pair p2;
    p2.first = 10;
    p2.second = 20;
    printf("equal: %d\n", are_equal_pairs(p, p2));
    p2.second = 30;
    printf("equal: %d\n", are_equal_pairs(p, p2));

    return 0;
}
