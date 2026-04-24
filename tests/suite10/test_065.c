int printf(const char *fmt, ...);

// Aggregate functions: SUM, COUNT, MIN, MAX, AVG (integer)

struct Row {
    int id;
    int category;
    int value;
    int quantity;
};

struct Table {
    struct Row rows[20];
    int nrows;
};

void add_row(struct Table *t, int id, int cat, int val, int qty) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].category = cat;
    t->rows[t->nrows].value = val;
    t->rows[t->nrows].quantity = qty;
    t->nrows++;
}

int agg_count(struct Table *t) {
    return t->nrows;
}

int agg_sum(struct Table *t, int col) {
    int s = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (col == 0) s = s + t->rows[i].value;
        else s = s + t->rows[i].quantity;
    }
    return s;
}

int agg_min(struct Table *t, int col) {
    int m;
    if (col == 0) m = t->rows[0].value;
    else m = t->rows[0].quantity;
    int i;
    for (i = 1; i < t->nrows; i++) {
        int v;
        if (col == 0) v = t->rows[i].value;
        else v = t->rows[i].quantity;
        if (v < m) m = v;
    }
    return m;
}

int agg_max(struct Table *t, int col) {
    int m;
    if (col == 0) m = t->rows[0].value;
    else m = t->rows[0].quantity;
    int i;
    for (i = 1; i < t->nrows; i++) {
        int v;
        if (col == 0) v = t->rows[i].value;
        else v = t->rows[i].quantity;
        if (v > m) m = v;
    }
    return m;
}

int agg_avg(struct Table *t, int col) {
    return agg_sum(t, col) / agg_count(t);
}

// Filtered aggregates (WHERE category == cat_filter)
int agg_count_where(struct Table *t, int cat_filter) {
    int c = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].category == cat_filter) c++;
    }
    return c;
}

int agg_sum_where(struct Table *t, int col, int cat_filter) {
    int s = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].category == cat_filter) {
            if (col == 0) s = s + t->rows[i].value;
            else s = s + t->rows[i].quantity;
        }
    }
    return s;
}

int agg_min_where(struct Table *t, int col, int cat_filter) {
    int m = 999999;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].category == cat_filter) {
            int v;
            if (col == 0) v = t->rows[i].value;
            else v = t->rows[i].quantity;
            if (v < m) m = v;
        }
    }
    return m;
}

int agg_max_where(struct Table *t, int col, int cat_filter) {
    int m = -999999;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (t->rows[i].category == cat_filter) {
            int v;
            if (col == 0) v = t->rows[i].value;
            else v = t->rows[i].quantity;
            if (v > m) m = v;
        }
    }
    return m;
}

int main() {
    struct Table t;
    t.nrows = 0;

    // Category 1: Electronics
    add_row(&t, 1, 1, 500, 10);
    add_row(&t, 2, 1, 300, 25);
    add_row(&t, 3, 1, 800, 5);
    add_row(&t, 4, 1, 150, 40);

    // Category 2: Books
    add_row(&t, 5, 2, 20, 100);
    add_row(&t, 6, 2, 35, 80);
    add_row(&t, 7, 2, 15, 200);

    // Category 3: Clothing
    add_row(&t, 8, 3, 50, 60);
    add_row(&t, 9, 3, 75, 45);
    add_row(&t, 10, 3, 120, 30);

    printf("Total rows: %d\n", agg_count(&t)); // EXPECT: Total rows: 10

    // Global aggregates on value (col 0)
    printf("SUM(value): %d\n", agg_sum(&t, 0)); // EXPECT: SUM(value): 2065
    printf("MIN(value): %d\n", agg_min(&t, 0)); // EXPECT: MIN(value): 15
    printf("MAX(value): %d\n", agg_max(&t, 0)); // EXPECT: MAX(value): 800
    printf("AVG(value): %d\n", agg_avg(&t, 0)); // EXPECT: AVG(value): 206

    // Global aggregates on quantity (col 1)
    printf("SUM(qty): %d\n", agg_sum(&t, 1)); // EXPECT: SUM(qty): 595
    printf("MIN(qty): %d\n", agg_min(&t, 1)); // EXPECT: MIN(qty): 5
    printf("MAX(qty): %d\n", agg_max(&t, 1)); // EXPECT: MAX(qty): 200
    printf("AVG(qty): %d\n", agg_avg(&t, 1)); // EXPECT: AVG(qty): 59

    // WHERE category = 1 (Electronics)
    printf("Electronics COUNT: %d\n", agg_count_where(&t, 1)); // EXPECT: Electronics COUNT: 4
    printf("Electronics SUM(value): %d\n", agg_sum_where(&t, 0, 1)); // EXPECT: Electronics SUM(value): 1750
    printf("Electronics MIN(value): %d\n", agg_min_where(&t, 0, 1)); // EXPECT: Electronics MIN(value): 150
    printf("Electronics MAX(value): %d\n", agg_max_where(&t, 0, 1)); // EXPECT: Electronics MAX(value): 800

    // WHERE category = 2 (Books)
    printf("Books COUNT: %d\n", agg_count_where(&t, 2)); // EXPECT: Books COUNT: 3
    printf("Books SUM(qty): %d\n", agg_sum_where(&t, 1, 2)); // EXPECT: Books SUM(qty): 380
    printf("Books MIN(value): %d\n", agg_min_where(&t, 0, 2)); // EXPECT: Books MIN(value): 15
    printf("Books MAX(qty): %d\n", agg_max_where(&t, 1, 2)); // EXPECT: Books MAX(qty): 200

    // WHERE category = 3 (Clothing)
    printf("Clothing COUNT: %d\n", agg_count_where(&t, 3)); // EXPECT: Clothing COUNT: 3
    printf("Clothing SUM(value): %d\n", agg_sum_where(&t, 0, 3)); // EXPECT: Clothing SUM(value): 245

    printf("Aggregate test passed\n"); // EXPECT: Aggregate test passed
    return 0;
}
