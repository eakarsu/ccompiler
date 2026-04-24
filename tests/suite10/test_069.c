int printf(const char *fmt, ...);

// UNION and INTERSECT of result sets

struct IntSet {
    int vals[32];
    int count;
};

void set_init(struct IntSet *s) {
    s->count = 0;
}

int set_contains(struct IntSet *s, int val) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->vals[i] == val) return 1;
    }
    return 0;
}

void set_add(struct IntSet *s, int val) {
    if (!set_contains(s, val)) {
        s->vals[s->count] = val;
        s->count++;
    }
}

// UNION: all distinct elements from both sets
void set_union(struct IntSet *a, struct IntSet *b, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < a->count; i++) {
        set_add(result, a->vals[i]);
    }
    for (i = 0; i < b->count; i++) {
        set_add(result, b->vals[i]);
    }
}

// INTERSECT: elements in both sets
void set_intersect(struct IntSet *a, struct IntSet *b, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < a->count; i++) {
        if (set_contains(b, a->vals[i])) {
            set_add(result, a->vals[i]);
        }
    }
}

// EXCEPT: elements in a but not in b
void set_except(struct IntSet *a, struct IntSet *b, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < a->count; i++) {
        if (!set_contains(b, a->vals[i])) {
            set_add(result, a->vals[i]);
        }
    }
}

// Sort a set for deterministic output
void set_sort(struct IntSet *s) {
    int i;
    int j;
    for (i = 1; i < s->count; i++) {
        int tmp = s->vals[i];
        j = i - 1;
        while (j >= 0 && s->vals[j] > tmp) {
            s->vals[j + 1] = s->vals[j];
            j--;
        }
        s->vals[j + 1] = tmp;
    }
}

void print_set(struct IntSet *s) {
    int i;
    printf("{");
    for (i = 0; i < s->count; i++) {
        if (i > 0) printf(",");
        printf("%d", s->vals[i]);
    }
    printf("}\n");
}

// Simulate query results as sets
// Query1: SELECT id FROM orders WHERE amount > 100
// Query2: SELECT id FROM orders WHERE region = 'East'

struct Order {
    int id;
    int amount;
    int region; // 1=North, 2=South, 3=East, 4=West
};

struct OrderTable {
    struct Order rows[20];
    int nrows;
};

void order_add(struct OrderTable *t, int id, int amt, int reg) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].amount = amt;
    t->rows[t->nrows].region = reg;
    t->nrows++;
}

void query_amount_gt_100(struct OrderTable *t, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].amount > 100) {
            set_add(result, t->rows[i].id);
        }
    }
}

void query_region_east(struct OrderTable *t, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].region == 3) {
            set_add(result, t->rows[i].id);
        }
    }
}

void query_amount_lt_80(struct OrderTable *t, struct IntSet *result) {
    set_init(result);
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].amount < 80) {
            set_add(result, t->rows[i].id);
        }
    }
}

int main() {
    // Basic set operations
    struct IntSet a;
    set_init(&a);
    set_add(&a, 1); set_add(&a, 2); set_add(&a, 3); set_add(&a, 4); set_add(&a, 5);

    struct IntSet b;
    set_init(&b);
    set_add(&b, 3); set_add(&b, 4); set_add(&b, 5); set_add(&b, 6); set_add(&b, 7);

    printf("Set A count: %d\n", a.count); // EXPECT: Set A count: 5
    printf("Set B count: %d\n", b.count); // EXPECT: Set B count: 5

    struct IntSet u;
    set_union(&a, &b, &u);
    set_sort(&u);
    printf("UNION count: %d\n", u.count); // EXPECT: UNION count: 7
    printf("UNION: ");
    print_set(&u); // EXPECT: UNION: {1,2,3,4,5,6,7}

    struct IntSet inter;
    set_intersect(&a, &b, &inter);
    set_sort(&inter);
    printf("INTERSECT count: %d\n", inter.count); // EXPECT: INTERSECT count: 3
    printf("INTERSECT: ");
    print_set(&inter); // EXPECT: INTERSECT: {3,4,5}

    struct IntSet exc;
    set_except(&a, &b, &exc);
    set_sort(&exc);
    printf("A EXCEPT B count: %d\n", exc.count); // EXPECT: A EXCEPT B count: 2
    printf("A EXCEPT B: ");
    print_set(&exc); // EXPECT: A EXCEPT B: {1,2}

    struct IntSet exc2;
    set_except(&b, &a, &exc2);
    set_sort(&exc2);
    printf("B EXCEPT A: ");
    print_set(&exc2); // EXPECT: B EXCEPT A: {6,7}

    // Query-based set operations
    struct OrderTable ot;
    ot.nrows = 0;
    order_add(&ot, 1, 150, 1);   // North, 150
    order_add(&ot, 2, 50, 3);    // East, 50
    order_add(&ot, 3, 200, 3);   // East, 200
    order_add(&ot, 4, 75, 2);    // South, 75
    order_add(&ot, 5, 120, 3);   // East, 120
    order_add(&ot, 6, 90, 4);    // West, 90
    order_add(&ot, 7, 300, 1);   // North, 300
    order_add(&ot, 8, 60, 3);    // East, 60

    printf("Orders: %d\n", ot.nrows); // EXPECT: Orders: 8

    struct IntSet q1;
    query_amount_gt_100(&ot, &q1);
    set_sort(&q1);
    printf("Q1 (amount>100): ");
    print_set(&q1); // EXPECT: Q1 (amount>100): {1,3,5,7}

    struct IntSet q2;
    query_region_east(&ot, &q2);
    set_sort(&q2);
    printf("Q2 (East): ");
    print_set(&q2); // EXPECT: Q2 (East): {2,3,5,8}

    // UNION of Q1 and Q2
    struct IntSet qu;
    set_union(&q1, &q2, &qu);
    set_sort(&qu);
    printf("Q1 UNION Q2: ");
    print_set(&qu); // EXPECT: Q1 UNION Q2: {1,2,3,5,7,8}

    // INTERSECT of Q1 and Q2 (East orders > 100)
    struct IntSet qi;
    set_intersect(&q1, &q2, &qi);
    set_sort(&qi);
    printf("Q1 INTERSECT Q2: ");
    print_set(&qi); // EXPECT: Q1 INTERSECT Q2: {3,5}

    // Q1 EXCEPT Q2 (>100 but not East)
    struct IntSet qe;
    set_except(&q1, &q2, &qe);
    set_sort(&qe);
    printf("Q1 EXCEPT Q2: ");
    print_set(&qe); // EXPECT: Q1 EXCEPT Q2: {1,7}

    // Triple set operation: (Q1 UNION Q2) INTERSECT Q3
    struct IntSet q3;
    query_amount_lt_80(&ot, &q3);
    set_sort(&q3);
    printf("Q3 (amount<80): ");
    print_set(&q3); // EXPECT: Q3 (amount<80): {2,4,8}

    struct IntSet triple;
    set_intersect(&qu, &q3, &triple);
    set_sort(&triple);
    printf("(Q1 UNION Q2) INTERSECT Q3: ");
    print_set(&triple); // EXPECT: (Q1 UNION Q2) INTERSECT Q3: {2,8}

    printf("Set operations test passed\n"); // EXPECT: Set operations test passed
    return 0;
}
