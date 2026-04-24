int printf(const char *fmt, ...);

/* test_005: JOIN two tables (nested loop join) */

typedef struct {
    int order_id;
    int customer_id;
    int amount;
} OrderRow;

typedef struct {
    int customer_id;
    char name[12];
    int city_code;
} CustomerRow;

typedef struct {
    OrderRow rows[16];
    int count;
} OrderTable;

typedef struct {
    CustomerRow rows[10];
    int count;
} CustomerTable;

void copy_name(char *dst, char *src) {
    int i = 0;
    while (src[i] && i < 11) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void ot_init(OrderTable *t) { t->count = 0; }
void ct_init(CustomerTable *t) { t->count = 0; }

void ot_insert(OrderTable *t, int oid, int cid, int amt) {
    if (t->count >= 16) return;
    t->rows[t->count].order_id = oid;
    t->rows[t->count].customer_id = cid;
    t->rows[t->count].amount = amt;
    t->count = t->count + 1;
}

void ct_insert(CustomerTable *t, int cid, char *name, int city) {
    if (t->count >= 10) return;
    t->rows[t->count].customer_id = cid;
    copy_name(t->rows[t->count].name, name);
    t->rows[t->count].city_code = city;
    t->count = t->count + 1;
}

/* INNER JOIN: orders JOIN customers ON order.customer_id = customer.customer_id */
int inner_join(OrderTable *ot, CustomerTable *ct) {
    int matched = 0;
    int i;
    int j;
    for (i = 0; i < ot->count; i = i + 1) {
        for (j = 0; j < ct->count; j = j + 1) {
            if (ot->rows[i].customer_id == ct->rows[j].customer_id) {
                printf("  oid=%d cust=%s amt=%d city=%d\n",
                       ot->rows[i].order_id,
                       ct->rows[j].name,
                       ot->rows[i].amount,
                       ct->rows[j].city_code);
                matched = matched + 1;
            }
        }
    }
    return matched;
}

/* LEFT JOIN: all orders, with customer name if exists */
int left_join(OrderTable *ot, CustomerTable *ct) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < ot->count; i = i + 1) {
        int found = 0;
        for (j = 0; j < ct->count; j = j + 1) {
            if (ot->rows[i].customer_id == ct->rows[j].customer_id) {
                printf("  oid=%d cust=%s amt=%d\n",
                       ot->rows[i].order_id,
                       ct->rows[j].name,
                       ot->rows[i].amount);
                found = 1;
            }
        }
        if (!found) {
            printf("  oid=%d cust=NULL amt=%d\n",
                   ot->rows[i].order_id,
                   ot->rows[i].amount);
        }
        total = total + 1;
    }
    return total;
}

/* JOIN with aggregate: total amount per customer */
void join_sum_by_customer(OrderTable *ot, CustomerTable *ct) {
    int j;
    for (j = 0; j < ct->count; j = j + 1) {
        int total = 0;
        int cnt = 0;
        int i;
        for (i = 0; i < ot->count; i = i + 1) {
            if (ot->rows[i].customer_id == ct->rows[j].customer_id) {
                total = total + ot->rows[i].amount;
                cnt = cnt + 1;
            }
        }
        if (cnt > 0) {
            printf("  cust=%s orders=%d total=%d\n", ct->rows[j].name, cnt, total);
        }
    }
}

int main() {
    CustomerTable ct;
    ct_init(&ct);
    ct_insert(&ct, 100, "Alice", 1);
    ct_insert(&ct, 101, "Bob", 2);
    ct_insert(&ct, 102, "Carol", 1);
    ct_insert(&ct, 103, "Dave", 3);

    OrderTable ot;
    ot_init(&ot);
    ot_insert(&ot, 1, 100, 250);
    ot_insert(&ot, 2, 101, 150);
    ot_insert(&ot, 3, 100, 300);
    ot_insert(&ot, 4, 102, 200);
    ot_insert(&ot, 5, 999, 50);
    ot_insert(&ot, 6, 101, 175);
    ot_insert(&ot, 7, 103, 400);

    printf("INNER JOIN:\n");
    // EXPECT: INNER JOIN:
    int n = inner_join(&ot, &ct);
    // EXPECT:   oid=1 cust=Alice amt=250 city=1
    // EXPECT:   oid=2 cust=Bob amt=150 city=2
    // EXPECT:   oid=3 cust=Alice amt=300 city=1
    // EXPECT:   oid=4 cust=Carol amt=200 city=1
    // EXPECT:   oid=6 cust=Bob amt=175 city=2
    // EXPECT:   oid=7 cust=Dave amt=400 city=3
    printf("matched=%d\n", n);
    // EXPECT: matched=6

    printf("LEFT JOIN:\n");
    // EXPECT: LEFT JOIN:
    n = left_join(&ot, &ct);
    // EXPECT:   oid=1 cust=Alice amt=250
    // EXPECT:   oid=2 cust=Bob amt=150
    // EXPECT:   oid=3 cust=Alice amt=300
    // EXPECT:   oid=4 cust=Carol amt=200
    // EXPECT:   oid=5 cust=NULL amt=50
    // EXPECT:   oid=6 cust=Bob amt=175
    // EXPECT:   oid=7 cust=Dave amt=400
    printf("total=%d\n", n);
    // EXPECT: total=7

    printf("SUM BY CUSTOMER:\n");
    // EXPECT: SUM BY CUSTOMER:
    join_sum_by_customer(&ot, &ct);
    // EXPECT:   cust=Alice orders=2 total=550
    // EXPECT:   cust=Bob orders=2 total=325
    // EXPECT:   cust=Carol orders=1 total=200
    // EXPECT:   cust=Dave orders=1 total=400

    printf("done\n");
    // EXPECT: done

    return 0;
}
