int printf(const char *fmt, ...);

// HAVING filter on GROUP BY results

struct SalesRow {
    int id;
    int region;    // 1=North, 2=South, 3=East, 4=West
    int product;   // 1=A, 2=B, 3=C
    int amount;
    int quantity;
};

struct SalesTable {
    struct SalesRow rows[30];
    int nrows;
};

struct GroupAgg {
    int key;
    int count;
    int sum_amount;
    int sum_qty;
    int min_amount;
    int max_amount;
};

struct GroupResult {
    struct GroupAgg groups[10];
    int ngroups;
};

void sales_add(struct SalesTable *t, int id, int reg, int prod, int amt, int qty) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].region = reg;
    t->rows[t->nrows].product = prod;
    t->rows[t->nrows].amount = amt;
    t->rows[t->nrows].quantity = qty;
    t->nrows++;
}

void group_by_key(struct SalesTable *t, int use_region, struct GroupResult *gr) {
    gr->ngroups = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        int key;
        if (use_region) key = t->rows[i].region;
        else key = t->rows[i].product;

        // Find or create group
        int g = -1;
        int j;
        for (j = 0; j < gr->ngroups; j++) {
            if (gr->groups[j].key == key) { g = j; break; }
        }
        if (g == -1) {
            g = gr->ngroups;
            gr->groups[g].key = key;
            gr->groups[g].count = 0;
            gr->groups[g].sum_amount = 0;
            gr->groups[g].sum_qty = 0;
            gr->groups[g].min_amount = 999999;
            gr->groups[g].max_amount = -999999;
            gr->ngroups++;
        }
        gr->groups[g].count++;
        gr->groups[g].sum_amount = gr->groups[g].sum_amount + t->rows[i].amount;
        gr->groups[g].sum_qty = gr->groups[g].sum_qty + t->rows[i].quantity;
        if (t->rows[i].amount < gr->groups[g].min_amount)
            gr->groups[g].min_amount = t->rows[i].amount;
        if (t->rows[i].amount > gr->groups[g].max_amount)
            gr->groups[g].max_amount = t->rows[i].amount;
    }
    // Sort by key
    for (i = 1; i < gr->ngroups; i++) {
        struct GroupAgg tmp = gr->groups[i];
        int j = i - 1;
        while (j >= 0 && gr->groups[j].key > tmp.key) {
            gr->groups[j + 1] = gr->groups[j];
            j--;
        }
        gr->groups[j + 1] = tmp;
    }
}

// HAVING: filter groups by predicate
typedef int (*HavingPred)(struct GroupAgg *g);

int having_count_gt_3(struct GroupAgg *g) {
    return g->count > 3;
}

int having_sum_gt_500(struct GroupAgg *g) {
    return g->sum_amount > 500;
}

int having_avg_gt_100(struct GroupAgg *g) {
    return (g->sum_amount / g->count) > 100;
}

int having_max_gt_200(struct GroupAgg *g) {
    return g->max_amount > 200;
}

void apply_having(struct GroupResult *src, struct GroupResult *dst, HavingPred pred) {
    dst->ngroups = 0;
    int i;
    for (i = 0; i < src->ngroups; i++) {
        if (pred(&src->groups[i])) {
            dst->groups[dst->ngroups] = src->groups[i];
            dst->ngroups++;
        }
    }
}

char *region_name(int r) {
    if (r == 1) return "North";
    if (r == 2) return "South";
    if (r == 3) return "East";
    if (r == 4) return "West";
    return "?";
}

char *product_name(int p) {
    if (p == 1) return "ProductA";
    if (p == 2) return "ProductB";
    if (p == 3) return "ProductC";
    return "?";
}

int main() {
    struct SalesTable st;
    st.nrows = 0;

    // North region sales
    sales_add(&st, 1, 1, 1, 100, 5);
    sales_add(&st, 2, 1, 2, 200, 3);
    sales_add(&st, 3, 1, 3, 150, 4);
    sales_add(&st, 4, 1, 1, 250, 2);
    sales_add(&st, 5, 1, 2, 300, 1);

    // South region sales
    sales_add(&st, 6, 2, 1, 80, 8);
    sales_add(&st, 7, 2, 3, 60, 10);
    sales_add(&st, 8, 2, 2, 90, 6);

    // East region sales
    sales_add(&st, 9, 3, 1, 120, 7);
    sales_add(&st, 10, 3, 2, 180, 4);
    sales_add(&st, 11, 3, 3, 220, 3);
    sales_add(&st, 12, 3, 1, 140, 5);

    // West region sales
    sales_add(&st, 13, 4, 2, 50, 12);
    sales_add(&st, 14, 4, 3, 70, 9);

    printf("Sales rows: %d\n", st.nrows); // EXPECT: Sales rows: 14

    // GROUP BY region
    struct GroupResult by_region;
    group_by_key(&st, 1, &by_region);
    printf("Groups by region: %d\n", by_region.ngroups); // EXPECT: Groups by region: 4

    int i;
    for (i = 0; i < by_region.ngroups; i++) {
        printf("  %s: count=%d sum=%d\n", region_name(by_region.groups[i].key),
               by_region.groups[i].count, by_region.groups[i].sum_amount);
    }
    // EXPECT:   North: count=5 sum=1000
    // EXPECT:   South: count=3 sum=230
    // EXPECT:   East: count=4 sum=660
    // EXPECT:   West: count=2 sum=120

    // HAVING count > 3
    struct GroupResult h1;
    apply_having(&by_region, &h1, having_count_gt_3);
    printf("HAVING count>3: %d groups\n", h1.ngroups); // EXPECT: HAVING count>3: 2 groups
    for (i = 0; i < h1.ngroups; i++) {
        printf("  %s\n", region_name(h1.groups[i].key));
    }
    // EXPECT:   North
    // EXPECT:   East

    // HAVING sum > 500
    struct GroupResult h2;
    apply_having(&by_region, &h2, having_sum_gt_500);
    printf("HAVING sum>500: %d groups\n", h2.ngroups); // EXPECT: HAVING sum>500: 2 groups
    for (i = 0; i < h2.ngroups; i++) {
        printf("  %s (sum=%d)\n", region_name(h2.groups[i].key), h2.groups[i].sum_amount);
    }
    // EXPECT:   North (sum=1000)
    // EXPECT:   East (sum=660)

    // GROUP BY product
    struct GroupResult by_product;
    group_by_key(&st, 0, &by_product);
    printf("Groups by product: %d\n", by_product.ngroups); // EXPECT: Groups by product: 3

    // HAVING avg > 100
    struct GroupResult h3;
    apply_having(&by_product, &h3, having_avg_gt_100);
    printf("HAVING avg>100: %d groups\n", h3.ngroups); // EXPECT: HAVING avg>100: 3 groups
    for (i = 0; i < h3.ngroups; i++) {
        int avg = h3.groups[i].sum_amount / h3.groups[i].count;
        printf("  %s avg=%d\n", product_name(h3.groups[i].key), avg);
    }
    // EXPECT:   ProductA avg=138
    // EXPECT:   ProductB avg=164
    // EXPECT:   ProductC avg=125

    printf("HAVING test passed\n"); // EXPECT: HAVING test passed
    return 0;
}
