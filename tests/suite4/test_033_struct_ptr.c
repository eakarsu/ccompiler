int printf(const char *fmt, ...);

// Test: Pointer to struct, arrow operator (->), modifying struct through pointer

struct Point {
    int x;
    int y;
};

struct Data {
    int value;
    int count;
    int total;
};

struct Pair {
    int first;
    int second;
};

void set_point(struct Point *p, int x, int y) {
    p->x = x;
    p->y = y;
}

void double_data(struct Data *d) {
    d->value = d->value * 2;
    d->count = d->count * 2;
    d->total = d->total * 2;
}

void swap_pair(struct Pair *p) {
    int tmp = p->first;
    p->first = p->second;
    p->second = tmp;
}

int sum_data(struct Data *d) {
    return d->value + d->count + d->total;
}

void increment_all(struct Data *d, int amount) {
    d->value = d->value + amount;
    d->count = d->count + amount;
    d->total = d->total + amount;
}

int main(void) {
    // Basic pointer to struct with arrow operator
    struct Point p;
    struct Point *pp = &p;
    pp->x = 10;
    pp->y = 20;
    // EXPECT: 10 20
    printf("%d %d\n", p.x, p.y);

    // Read through pointer
    // EXPECT: 10 20
    printf("%d %d\n", pp->x, pp->y);

    // Modify through pointer
    pp->x = pp->x + 5;
    pp->y = pp->y - 3;
    // EXPECT: 15 17
    printf("%d %d\n", pp->x, pp->y);

    // Function modifying struct through pointer
    set_point(&p, 100, 200);
    // EXPECT: 100 200
    printf("%d %d\n", p.x, p.y);

    // Data struct pointer operations
    struct Data d;
    d.value = 5;
    d.count = 10;
    d.total = 15;
    struct Data *dp = &d;
    // EXPECT: 5 10 15
    printf("%d %d %d\n", dp->value, dp->count, dp->total);

    // Double data through pointer
    double_data(&d);
    // EXPECT: 10 20 30
    printf("%d %d %d\n", d.value, d.count, d.total);

    // Sum through pointer function
    int s = sum_data(&d);
    // EXPECT: 60
    printf("%d\n", s);

    // Increment all fields
    increment_all(&d, 7);
    // EXPECT: 17 27 37
    printf("%d %d %d\n", d.value, d.count, d.total);

    // Pair swap through pointer
    struct Pair pair;
    pair.first = 42;
    pair.second = 99;
    // EXPECT: 42 99
    printf("%d %d\n", pair.first, pair.second);

    swap_pair(&pair);
    // EXPECT: 99 42
    printf("%d %d\n", pair.first, pair.second);

    // Multiple pointers to same struct
    struct Point q;
    q.x = 1;
    q.y = 2;
    struct Point *q1 = &q;
    struct Point *q2 = &q;
    q1->x = 50;
    // EXPECT: 50
    printf("%d\n", q2->x);

    // Array of structs with pointer iteration
    struct Point pts[4];
    int i;
    for (i = 0; i < 4; i = i + 1) {
        struct Point *cur = &pts[i];
        cur->x = i * 3;
        cur->y = i * 7;
    }
    // EXPECT: 0 0
    printf("%d %d\n", pts[0].x, pts[0].y);
    // EXPECT: 3 7
    printf("%d %d\n", pts[1].x, pts[1].y);
    // EXPECT: 6 14
    printf("%d %d\n", pts[2].x, pts[2].y);
    // EXPECT: 9 21
    printf("%d %d\n", pts[3].x, pts[3].y);

    // Pointer arithmetic on struct member
    struct Data d2;
    struct Data *d2p = &d2;
    d2p->value = 100;
    d2p->count = d2p->value / 4;
    d2p->total = d2p->value + d2p->count;
    // EXPECT: 100 25 125
    printf("%d %d %d\n", d2p->value, d2p->count, d2p->total);

    return 0;
}
