int printf(const char *fmt, ...);

/* test_010: Transaction log with rollback simulation */

typedef struct {
    int op;       /* 0=insert, 1=update, 2=delete */
    int row_id;
    int old_val;
    int new_val;
    int txn_id;
} LogEntry;

typedef struct {
    LogEntry entries[40];
    int count;
} TxnLog;

typedef struct {
    int id;
    int value;
    int exists;
} Record;

typedef struct {
    Record rows[16];
    int count;
} SimpleTable;

void log_init(TxnLog *log) {
    log->count = 0;
}

void tbl_init(SimpleTable *t) {
    t->count = 0;
}

int tbl_find(SimpleTable *t, int id) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].id == id && t->rows[i].exists) return i;
    }
    return -1;
}

void log_append(TxnLog *log, int op, int row_id, int old_val, int new_val, int txn_id) {
    if (log->count >= 40) return;
    LogEntry *e = &log->entries[log->count];
    e->op = op;
    e->row_id = row_id;
    e->old_val = old_val;
    e->new_val = new_val;
    e->txn_id = txn_id;
    log->count = log->count + 1;
}

void tbl_insert(SimpleTable *t, TxnLog *log, int id, int value, int txn_id) {
    if (t->count >= 16) return;
    t->rows[t->count].id = id;
    t->rows[t->count].value = value;
    t->rows[t->count].exists = 1;
    log_append(log, 0, id, 0, value, txn_id);
    t->count = t->count + 1;
}

int tbl_update(SimpleTable *t, TxnLog *log, int id, int new_val, int txn_id) {
    int idx = tbl_find(t, id);
    if (idx < 0) return 0;
    int old = t->rows[idx].value;
    t->rows[idx].value = new_val;
    log_append(log, 1, id, old, new_val, txn_id);
    return 1;
}

int tbl_delete(SimpleTable *t, TxnLog *log, int id, int txn_id) {
    int idx = tbl_find(t, id);
    if (idx < 0) return 0;
    int old = t->rows[idx].value;
    t->rows[idx].exists = 0;
    log_append(log, 2, id, old, 0, txn_id);
    return 1;
}

/* Rollback all operations for a given txn_id (in reverse order) */
int rollback(SimpleTable *t, TxnLog *log, int txn_id) {
    int undone = 0;
    int i;
    for (i = log->count - 1; i >= 0; i = i - 1) {
        if (log->entries[i].txn_id != txn_id) continue;
        int op = log->entries[i].op;
        int rid = log->entries[i].row_id;

        if (op == 0) {
            /* Undo insert: mark row as not existing */
            int idx = tbl_find(t, rid);
            if (idx >= 0) {
                t->rows[idx].exists = 0;
            }
            undone = undone + 1;
        } else if (op == 1) {
            /* Undo update: restore old value */
            int j;
            for (j = 0; j < t->count; j = j + 1) {
                if (t->rows[j].id == rid) {
                    t->rows[j].value = log->entries[i].old_val;
                    t->rows[j].exists = 1;
                    break;
                }
            }
            undone = undone + 1;
        } else if (op == 2) {
            /* Undo delete: restore row */
            int j;
            for (j = 0; j < t->count; j = j + 1) {
                if (t->rows[j].id == rid) {
                    t->rows[j].exists = 1;
                    t->rows[j].value = log->entries[i].old_val;
                    break;
                }
            }
            undone = undone + 1;
        }
    }
    return undone;
}

void tbl_print(SimpleTable *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].exists) {
            printf("  id=%d val=%d\n", t->rows[i].id, t->rows[i].value);
        }
    }
}

int tbl_live_count(SimpleTable *t) {
    int n = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].exists) n = n + 1;
    }
    return n;
}

void print_log(TxnLog *log) {
    int i;
    for (i = 0; i < log->count; i = i + 1) {
        char *opname;
        if (log->entries[i].op == 0) opname = "INSERT";
        else if (log->entries[i].op == 1) opname = "UPDATE";
        else opname = "DELETE";
        printf("  log[%d]: txn=%d op=%s row=%d old=%d new=%d\n",
               i, log->entries[i].txn_id, opname,
               log->entries[i].row_id, log->entries[i].old_val, log->entries[i].new_val);
    }
}

int main() {
    SimpleTable t;
    TxnLog log;
    tbl_init(&t);
    log_init(&log);

    /* Transaction 1: insert some rows */
    tbl_insert(&t, &log, 1, 100, 1);
    tbl_insert(&t, &log, 2, 200, 1);
    tbl_insert(&t, &log, 3, 300, 1);

    printf("After txn 1 (live=%d):\n", tbl_live_count(&t));
    // EXPECT: After txn 1 (live=3):
    tbl_print(&t);
    // EXPECT:   id=1 val=100
    // EXPECT:   id=2 val=200
    // EXPECT:   id=3 val=300

    /* Transaction 2: update and insert */
    tbl_update(&t, &log, 1, 150, 2);
    tbl_update(&t, &log, 3, 350, 2);
    tbl_insert(&t, &log, 4, 400, 2);

    printf("After txn 2 (live=%d):\n", tbl_live_count(&t));
    // EXPECT: After txn 2 (live=4):
    tbl_print(&t);
    // EXPECT:   id=1 val=150
    // EXPECT:   id=2 val=200
    // EXPECT:   id=3 val=350
    // EXPECT:   id=4 val=400

    /* Transaction 3: delete and update */
    tbl_delete(&t, &log, 2, 3);
    tbl_update(&t, &log, 4, 450, 3);

    printf("After txn 3 (live=%d):\n", tbl_live_count(&t));
    // EXPECT: After txn 3 (live=3):
    tbl_print(&t);
    // EXPECT:   id=1 val=150
    // EXPECT:   id=3 val=350
    // EXPECT:   id=4 val=450

    printf("Transaction log:\n");
    // EXPECT: Transaction log:
    print_log(&log);
    // EXPECT:   log[0]: txn=1 op=INSERT row=1 old=0 new=100
    // EXPECT:   log[1]: txn=1 op=INSERT row=2 old=0 new=200
    // EXPECT:   log[2]: txn=1 op=INSERT row=3 old=0 new=300
    // EXPECT:   log[3]: txn=2 op=UPDATE row=1 old=100 new=150
    // EXPECT:   log[4]: txn=2 op=UPDATE row=3 old=300 new=350
    // EXPECT:   log[5]: txn=2 op=INSERT row=4 old=0 new=400
    // EXPECT:   log[6]: txn=3 op=DELETE row=2 old=200 new=0
    // EXPECT:   log[7]: txn=3 op=UPDATE row=4 old=400 new=450

    /* Rollback transaction 3 */
    printf("Rolling back txn 3...\n");
    // EXPECT: Rolling back txn 3...
    int undone = rollback(&t, &log, 3);
    printf("undone=%d\n", undone);
    // EXPECT: undone=2

    printf("After rollback txn 3 (live=%d):\n", tbl_live_count(&t));
    // EXPECT: After rollback txn 3 (live=4):
    tbl_print(&t);
    // EXPECT:   id=1 val=150
    // EXPECT:   id=2 val=200
    // EXPECT:   id=3 val=350
    // EXPECT:   id=4 val=400

    /* Rollback transaction 2 */
    printf("Rolling back txn 2...\n");
    // EXPECT: Rolling back txn 2...
    undone = rollback(&t, &log, 2);
    printf("undone=%d\n", undone);
    // EXPECT: undone=3

    printf("After rollback txn 2 (live=%d):\n", tbl_live_count(&t));
    // EXPECT: After rollback txn 2 (live=3):
    tbl_print(&t);
    // EXPECT:   id=1 val=100
    // EXPECT:   id=2 val=200
    // EXPECT:   id=3 val=300

    printf("done\n");
    // EXPECT: done

    return 0;
}
