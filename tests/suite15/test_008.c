int printf(const char *fmt, ...);

/* test_008: DISTINCT and LIMIT operations */

typedef struct {
    int id;
    int color;
    int size;
    int weight;
} Item;

typedef struct {
    Item rows[24];
    int count;
} ItemTable;

void it_init(ItemTable *t) {
    t->count = 0;
}

void it_insert(ItemTable *t, int id, int color, int size, int weight) {
    if (t->count >= 24) return;
    Item *item = &t->rows[t->count];
    item->id = id;
    item->color = color;
    item->size = size;
    item->weight = weight;
    t->count = t->count + 1;
}

/* SELECT DISTINCT color FROM items */
int select_distinct_color(ItemTable *t) {
    int seen[100];
    int seen_count = 0;
    int i;
    int j;

    for (i = 0; i < t->count; i = i + 1) {
        int val = t->rows[i].color;
        int dup = 0;
        for (j = 0; j < seen_count; j = j + 1) {
            if (seen[j] == val) {
                dup = 1;
                break;
            }
        }
        if (!dup) {
            seen[seen_count] = val;
            seen_count = seen_count + 1;
        }
    }
    /* Sort distinct values */
    for (i = 0; i < seen_count - 1; i = i + 1) {
        for (j = 0; j < seen_count - 1 - i; j = j + 1) {
            if (seen[j] > seen[j + 1]) {
                int tmp = seen[j];
                seen[j] = seen[j + 1];
                seen[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < seen_count; i = i + 1) {
        printf("  color=%d\n", seen[i]);
    }
    return seen_count;
}

/* SELECT DISTINCT size FROM items */
int select_distinct_size(ItemTable *t) {
    int seen[100];
    int seen_count = 0;
    int i;
    int j;

    for (i = 0; i < t->count; i = i + 1) {
        int val = t->rows[i].size;
        int dup = 0;
        for (j = 0; j < seen_count; j = j + 1) {
            if (seen[j] == val) {
                dup = 1;
                break;
            }
        }
        if (!dup) {
            seen[seen_count] = val;
            seen_count = seen_count + 1;
        }
    }
    /* Sort */
    for (i = 0; i < seen_count - 1; i = i + 1) {
        for (j = 0; j < seen_count - 1 - i; j = j + 1) {
            if (seen[j] > seen[j + 1]) {
                int tmp = seen[j];
                seen[j] = seen[j + 1];
                seen[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < seen_count; i = i + 1) {
        printf("  size=%d\n", seen[i]);
    }
    return seen_count;
}

/* SELECT * FROM items ORDER BY weight DESC LIMIT n */
void select_with_limit(ItemTable *t, int limit) {
    /* First sort by weight descending (copy to avoid mutating) */
    Item sorted[24];
    int n = t->count;
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        sorted[i] = t->rows[i];
    }
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (sorted[j].weight < sorted[j + 1].weight) {
                Item tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }
    int rows_to_show = limit;
    if (rows_to_show > n) rows_to_show = n;
    for (i = 0; i < rows_to_show; i = i + 1) {
        printf("  id=%d color=%d size=%d weight=%d\n",
               sorted[i].id, sorted[i].color, sorted[i].size, sorted[i].weight);
    }
}

/* SELECT * FROM items LIMIT n OFFSET off */
void select_limit_offset(ItemTable *t, int limit, int offset) {
    int i;
    int printed = 0;
    for (i = offset; i < t->count && printed < limit; i = i + 1) {
        printf("  id=%d color=%d weight=%d\n",
               t->rows[i].id, t->rows[i].color, t->rows[i].weight);
        printed = printed + 1;
    }
}

int main() {
    ItemTable t;
    it_init(&t);

    it_insert(&t, 1,  1, 10, 500);
    it_insert(&t, 2,  2, 20, 300);
    it_insert(&t, 3,  1, 30, 700);
    it_insert(&t, 4,  3, 10, 200);
    it_insert(&t, 5,  2, 20, 800);
    it_insert(&t, 6,  1, 30, 150);
    it_insert(&t, 7,  3, 10, 600);
    it_insert(&t, 8,  2, 20, 400);
    it_insert(&t, 9,  3, 30, 350);
    it_insert(&t, 10, 1, 10, 900);

    printf("total=%d\n", t.count);
    // EXPECT: total=10

    printf("DISTINCT color:\n");
    // EXPECT: DISTINCT color:
    int n = select_distinct_color(&t);
    // EXPECT:   color=1
    // EXPECT:   color=2
    // EXPECT:   color=3
    printf("distinct_count=%d\n", n);
    // EXPECT: distinct_count=3

    printf("DISTINCT size:\n");
    // EXPECT: DISTINCT size:
    n = select_distinct_size(&t);
    // EXPECT:   size=10
    // EXPECT:   size=20
    // EXPECT:   size=30
    printf("distinct_count=%d\n", n);
    // EXPECT: distinct_count=3

    printf("TOP 3 by weight DESC:\n");
    // EXPECT: TOP 3 by weight DESC:
    select_with_limit(&t, 3);
    // EXPECT:   id=10 color=1 size=10 weight=900
    // EXPECT:   id=5 color=2 size=20 weight=800
    // EXPECT:   id=3 color=1 size=30 weight=700

    printf("TOP 5 by weight DESC:\n");
    // EXPECT: TOP 5 by weight DESC:
    select_with_limit(&t, 5);
    // EXPECT:   id=10 color=1 size=10 weight=900
    // EXPECT:   id=5 color=2 size=20 weight=800
    // EXPECT:   id=3 color=1 size=30 weight=700
    // EXPECT:   id=7 color=3 size=10 weight=600
    // EXPECT:   id=1 color=1 size=10 weight=500

    printf("LIMIT 3 OFFSET 2:\n");
    // EXPECT: LIMIT 3 OFFSET 2:
    select_limit_offset(&t, 3, 2);
    // EXPECT:   id=3 color=1 weight=700
    // EXPECT:   id=4 color=3 weight=200
    // EXPECT:   id=5 color=2 weight=800

    printf("LIMIT 2 OFFSET 8:\n");
    // EXPECT: LIMIT 2 OFFSET 8:
    select_limit_offset(&t, 2, 8);
    // EXPECT:   id=9 color=3 weight=350
    // EXPECT:   id=10 color=1 weight=900

    printf("done\n");
    // EXPECT: done

    return 0;
}
