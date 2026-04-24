int printf(const char *fmt, ...);

/* test_007: UPDATE and DELETE operations */

typedef struct {
    int id;
    int price;
    int qty;
    int deleted;
} Product;

typedef struct {
    Product rows[20];
    int count;
} ProductTable;

void pt_init(ProductTable *t) {
    t->count = 0;
}

void pt_insert(ProductTable *t, int id, int price, int qty) {
    if (t->count >= 20) return;
    Product *p = &t->rows[t->count];
    p->id = id;
    p->price = price;
    p->qty = qty;
    p->deleted = 0;
    t->count = t->count + 1;
}

/* Count non-deleted rows */
int pt_live_count(ProductTable *t) {
    int n = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0) n = n + 1;
    }
    return n;
}

/* Print all non-deleted rows */
void pt_print(ProductTable *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0) {
            printf("  id=%d price=%d qty=%d\n", t->rows[i].id, t->rows[i].price, t->rows[i].qty);
        }
    }
}

/* UPDATE: SET price = new_price WHERE id = target_id */
int update_price_by_id(ProductTable *t, int target_id, int new_price) {
    int updated = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0 && t->rows[i].id == target_id) {
            t->rows[i].price = new_price;
            updated = updated + 1;
        }
    }
    return updated;
}

/* UPDATE: SET qty = qty + delta WHERE price > threshold */
int update_qty_where_price_gt(ProductTable *t, int threshold, int delta) {
    int updated = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0 && t->rows[i].price > threshold) {
            t->rows[i].qty = t->rows[i].qty + delta;
            updated = updated + 1;
        }
    }
    return updated;
}

/* DELETE WHERE id = target_id (soft delete) */
int delete_by_id(ProductTable *t, int target_id) {
    int deleted = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0 && t->rows[i].id == target_id) {
            t->rows[i].deleted = 1;
            deleted = deleted + 1;
        }
    }
    return deleted;
}

/* DELETE WHERE qty < threshold */
int delete_where_qty_lt(ProductTable *t, int threshold) {
    int deleted = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0 && t->rows[i].qty < threshold) {
            t->rows[i].deleted = 1;
            deleted = deleted + 1;
        }
    }
    return deleted;
}

/* Compact: physically remove deleted rows */
void pt_compact(ProductTable *t) {
    int write_idx = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].deleted == 0) {
            if (write_idx != i) {
                t->rows[write_idx] = t->rows[i];
            }
            write_idx = write_idx + 1;
        }
    }
    t->count = write_idx;
}

int main() {
    ProductTable t;
    pt_init(&t);

    pt_insert(&t, 1, 100, 10);
    pt_insert(&t, 2, 250, 5);
    pt_insert(&t, 3, 180, 8);
    pt_insert(&t, 4, 300, 3);
    pt_insert(&t, 5, 120, 15);
    pt_insert(&t, 6, 200, 7);

    printf("Initial (live=%d):\n", pt_live_count(&t));
    // EXPECT: Initial (live=6):
    pt_print(&t);
    // EXPECT:   id=1 price=100 qty=10
    // EXPECT:   id=2 price=250 qty=5
    // EXPECT:   id=3 price=180 qty=8
    // EXPECT:   id=4 price=300 qty=3
    // EXPECT:   id=5 price=120 qty=15
    // EXPECT:   id=6 price=200 qty=7

    printf("UPDATE id=3 SET price=190:\n");
    // EXPECT: UPDATE id=3 SET price=190:
    int n = update_price_by_id(&t, 3, 190);
    printf("updated=%d\n", n);
    // EXPECT: updated=1

    printf("UPDATE SET qty+=10 WHERE price>180:\n");
    // EXPECT: UPDATE SET qty+=10 WHERE price>180:
    n = update_qty_where_price_gt(&t, 180, 10);
    printf("updated=%d\n", n);
    // EXPECT: updated=4

    printf("After updates:\n");
    // EXPECT: After updates:
    pt_print(&t);
    // EXPECT:   id=1 price=100 qty=10
    // EXPECT:   id=2 price=250 qty=15
    // EXPECT:   id=3 price=190 qty=18
    // EXPECT:   id=4 price=300 qty=13
    // EXPECT:   id=5 price=120 qty=15
    // EXPECT:   id=6 price=200 qty=17

    printf("DELETE id=2:\n");
    // EXPECT: DELETE id=2:
    n = delete_by_id(&t, 2);
    printf("deleted=%d\n", n);
    // EXPECT: deleted=1

    printf("After delete (live=%d):\n", pt_live_count(&t));
    // EXPECT: After delete (live=5):
    pt_print(&t);
    // EXPECT:   id=1 price=100 qty=10
    // EXPECT:   id=3 price=190 qty=18
    // EXPECT:   id=4 price=300 qty=13
    // EXPECT:   id=5 price=120 qty=15
    // EXPECT:   id=6 price=200 qty=17

    printf("DELETE WHERE qty<15:\n");
    // EXPECT: DELETE WHERE qty<15:
    n = delete_where_qty_lt(&t, 15);
    printf("deleted=%d\n", n);
    // EXPECT: deleted=2

    printf("After delete (live=%d):\n", pt_live_count(&t));
    // EXPECT: After delete (live=3):
    pt_print(&t);
    // EXPECT:   id=3 price=190 qty=18
    // EXPECT:   id=5 price=120 qty=15
    // EXPECT:   id=6 price=200 qty=17

    printf("Compacting...\n");
    // EXPECT: Compacting...
    pt_compact(&t);
    printf("physical_count=%d\n", t.count);
    // EXPECT: physical_count=3

    pt_print(&t);
    // EXPECT:   id=3 price=190 qty=18
    // EXPECT:   id=5 price=120 qty=15
    // EXPECT:   id=6 price=200 qty=17

    printf("done\n");
    // EXPECT: done

    return 0;
}
