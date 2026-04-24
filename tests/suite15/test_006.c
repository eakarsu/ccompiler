int printf(const char *fmt, ...);

/* test_006: Index simulation (sorted array with binary search) */

typedef struct {
    int key;
    int row_idx;
} IndexEntry;

typedef struct {
    IndexEntry entries[32];
    int count;
} BTreeIndex;

typedef struct {
    int id;
    int value;
    int category;
} DataRow;

typedef struct {
    DataRow rows[32];
    int count;
} DataTable;

void dt_init(DataTable *t) { t->count = 0; }
void idx_init(BTreeIndex *idx) { idx->count = 0; }

void dt_insert(DataTable *t, int id, int value, int cat) {
    if (t->count >= 32) return;
    t->rows[t->count].id = id;
    t->rows[t->count].value = value;
    t->rows[t->count].category = cat;
    t->count = t->count + 1;
}

/* Insert into index maintaining sorted order */
void idx_insert(BTreeIndex *idx, int key, int row_idx) {
    if (idx->count >= 32) return;
    /* Find insertion point */
    int pos = 0;
    while (pos < idx->count && idx->entries[pos].key < key) {
        pos = pos + 1;
    }
    /* Shift elements right */
    int i;
    for (i = idx->count; i > pos; i = i - 1) {
        idx->entries[i] = idx->entries[i - 1];
    }
    idx->entries[pos].key = key;
    idx->entries[pos].row_idx = row_idx;
    idx->count = idx->count + 1;
}

/* Binary search: returns index or -1 */
int idx_search(BTreeIndex *idx, int key) {
    int lo = 0;
    int hi = idx->count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (idx->entries[mid].key == key) {
            return idx->entries[mid].row_idx;
        } else if (idx->entries[mid].key < key) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

/* Range scan: find all entries with lo <= key <= hi */
int idx_range_scan(BTreeIndex *idx, DataTable *t, int lo, int hi) {
    int count = 0;
    /* Find first entry >= lo using binary search */
    int left = 0;
    int right = idx->count - 1;
    int start = idx->count;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (idx->entries[mid].key >= lo) {
            start = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    /* Scan forward while key <= hi */
    int i;
    for (i = start; i < idx->count && idx->entries[i].key <= hi; i = i + 1) {
        int ri = idx->entries[i].row_idx;
        printf("  scan: id=%d val=%d cat=%d\n", t->rows[ri].id, t->rows[ri].value, t->rows[ri].category);
        count = count + 1;
    }
    return count;
}

/* Build index on the 'value' column */
void build_index_on_value(DataTable *t, BTreeIndex *idx) {
    idx_init(idx);
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        idx_insert(idx, t->rows[i].value, i);
    }
}

void print_index(BTreeIndex *idx) {
    int i;
    for (i = 0; i < idx->count; i = i + 1) {
        printf("  idx[%d]: key=%d row=%d\n", i, idx->entries[i].key, idx->entries[i].row_idx);
    }
}

int main() {
    DataTable t;
    dt_init(&t);

    dt_insert(&t, 1, 50, 1);
    dt_insert(&t, 2, 20, 2);
    dt_insert(&t, 3, 80, 1);
    dt_insert(&t, 4, 35, 3);
    dt_insert(&t, 5, 60, 2);
    dt_insert(&t, 6, 10, 1);
    dt_insert(&t, 7, 45, 3);
    dt_insert(&t, 8, 70, 2);

    printf("rows=%d\n", t.count);
    // EXPECT: rows=8

    BTreeIndex idx;
    build_index_on_value(&t, &idx);

    printf("index entries=%d\n", idx.count);
    // EXPECT: index entries=8

    printf("Sorted index:\n");
    // EXPECT: Sorted index:
    print_index(&idx);
    // EXPECT:   idx[0]: key=10 row=5
    // EXPECT:   idx[1]: key=20 row=1
    // EXPECT:   idx[2]: key=35 row=3
    // EXPECT:   idx[3]: key=45 row=6
    // EXPECT:   idx[4]: key=50 row=0
    // EXPECT:   idx[5]: key=60 row=4
    // EXPECT:   idx[6]: key=70 row=7
    // EXPECT:   idx[7]: key=80 row=2

    printf("Search val=50:\n");
    // EXPECT: Search val=50:
    int ri = idx_search(&idx, 50);
    printf("  row_idx=%d id=%d\n", ri, t.rows[ri].id);
    // EXPECT:   row_idx=0 id=1

    printf("Search val=35:\n");
    // EXPECT: Search val=35:
    ri = idx_search(&idx, 35);
    printf("  row_idx=%d id=%d\n", ri, t.rows[ri].id);
    // EXPECT:   row_idx=3 id=4

    printf("Search val=99:\n");
    // EXPECT: Search val=99:
    ri = idx_search(&idx, 99);
    printf("  row_idx=%d\n", ri);
    // EXPECT:   row_idx=-1

    printf("Range scan 30..60:\n");
    // EXPECT: Range scan 30..60:
    int n = idx_range_scan(&idx, &t, 30, 60);
    // EXPECT:   scan: id=4 val=35 cat=3
    // EXPECT:   scan: id=7 val=45 cat=3
    // EXPECT:   scan: id=1 val=50 cat=1
    // EXPECT:   scan: id=5 val=60 cat=2
    printf("range_count=%d\n", n);
    // EXPECT: range_count=4

    printf("Range scan 65..85:\n");
    // EXPECT: Range scan 65..85:
    n = idx_range_scan(&idx, &t, 65, 85);
    // EXPECT:   scan: id=8 val=70 cat=2
    // EXPECT:   scan: id=3 val=80 cat=1
    printf("range_count=%d\n", n);
    // EXPECT: range_count=2

    printf("done\n");
    // EXPECT: done

    return 0;
}
