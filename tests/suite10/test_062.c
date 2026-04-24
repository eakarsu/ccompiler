int printf(const char *fmt, ...);

// Index scan simulation: sorted index with binary search for range queries

struct Row {
    int id;
    int value;
    int category;
};

struct Table {
    struct Row rows[32];
    int nrows;
};

struct Index {
    int sorted_ids[32]; // indices into table, sorted by value
    int nentries;
};

void table_init(struct Table *t) {
    t->nrows = 0;
}

void table_insert(struct Table *t, int id, int value, int category) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].value = value;
    t->rows[t->nrows].category = category;
    t->nrows++;
}

void index_build(struct Index *idx, struct Table *t) {
    int i;
    idx->nentries = t->nrows;
    for (i = 0; i < t->nrows; i++) {
        idx->sorted_ids[i] = i;
    }
    // Insertion sort by value
    int j;
    for (i = 1; i < idx->nentries; i++) {
        int key = idx->sorted_ids[i];
        int keyval = t->rows[key].value;
        j = i - 1;
        while (j >= 0 && t->rows[idx->sorted_ids[j]].value > keyval) {
            idx->sorted_ids[j + 1] = idx->sorted_ids[j];
            j--;
        }
        idx->sorted_ids[j + 1] = key;
    }
}

// Binary search: find first index >= low_val
int index_lower_bound(struct Index *idx, struct Table *t, int low_val) {
    int lo = 0;
    int hi = idx->nentries;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (t->rows[idx->sorted_ids[mid]].value < low_val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

// Binary search: find first index > high_val
int index_upper_bound(struct Index *idx, struct Table *t, int high_val) {
    int lo = 0;
    int hi = idx->nentries;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (t->rows[idx->sorted_ids[mid]].value <= high_val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

struct ResultSet {
    int ids[32];
    int count;
};

void index_range_scan(struct Index *idx, struct Table *t, int low, int high, struct ResultSet *rs) {
    rs->count = 0;
    int start = index_lower_bound(idx, t, low);
    int end = index_upper_bound(idx, t, high);
    int i;
    for (i = start; i < end; i++) {
        rs->ids[rs->count] = idx->sorted_ids[i];
        rs->count++;
    }
}

void index_point_lookup(struct Index *idx, struct Table *t, int val, struct ResultSet *rs) {
    index_range_scan(idx, t, val, val, rs);
}

int main() {
    struct Table tbl;
    table_init(&tbl);

    // Insert 12 rows with various values
    table_insert(&tbl, 1, 50, 1);
    table_insert(&tbl, 2, 20, 2);
    table_insert(&tbl, 3, 80, 1);
    table_insert(&tbl, 4, 30, 3);
    table_insert(&tbl, 5, 60, 2);
    table_insert(&tbl, 6, 10, 1);
    table_insert(&tbl, 7, 40, 3);
    table_insert(&tbl, 8, 70, 2);
    table_insert(&tbl, 9, 90, 1);
    table_insert(&tbl, 10, 25, 3);
    table_insert(&tbl, 11, 55, 2);
    table_insert(&tbl, 12, 45, 1);

    printf("Table has %d rows\n", tbl.nrows); // EXPECT: Table has 12 rows

    struct Index idx;
    index_build(&idx, &tbl);
    printf("Index built with %d entries\n", idx.nentries); // EXPECT: Index built with 12 entries

    // Verify index is sorted
    int sorted_ok = 1;
    int i;
    for (i = 1; i < idx.nentries; i++) {
        if (tbl.rows[idx.sorted_ids[i]].value < tbl.rows[idx.sorted_ids[i - 1]].value) {
            sorted_ok = 0;
        }
    }
    printf("Index sorted: %d\n", sorted_ok); // EXPECT: Index sorted: 1

    // Range scan: values 25..60
    struct ResultSet rs;
    index_range_scan(&idx, &tbl, 25, 60, &rs);
    printf("Range [25,60] count: %d\n", rs.count); // EXPECT: Range [25,60] count: 7

    // Print matched values in sorted order
    for (i = 0; i < rs.count; i++) {
        printf("  val=%d\n", tbl.rows[rs.ids[i]].value);
    }
    // EXPECT:   val=25
    // EXPECT:   val=30
    // EXPECT:   val=40
    // EXPECT:   val=45
    // EXPECT:   val=50
    // EXPECT:   val=55
    // EXPECT:   val=60

    // Point lookup: value 70
    struct ResultSet rs2;
    index_point_lookup(&idx, &tbl, 70, &rs2);
    printf("Point lookup val=70 count: %d\n", rs2.count); // EXPECT: Point lookup val=70 count: 1
    printf("  id=%d\n", tbl.rows[rs2.ids[0]].id); // EXPECT:   id=8

    // Point lookup: value 99 (not found)
    struct ResultSet rs3;
    index_point_lookup(&idx, &tbl, 99, &rs3);
    printf("Point lookup val=99 count: %d\n", rs3.count); // EXPECT: Point lookup val=99 count: 0

    // Range scan: values 80..100
    struct ResultSet rs4;
    index_range_scan(&idx, &tbl, 80, 100, &rs4);
    printf("Range [80,100] count: %d\n", rs4.count); // EXPECT: Range [80,100] count: 2

    printf("Index scan test passed\n"); // EXPECT: Index scan test passed
    return 0;
}
