int printf(const char *fmt, ...);

// Sparse Table for Range Minimum Queries (RMQ)
// Preprocessing: O(n log n), Query: O(1)

struct SparseTable {
    int table[8][4]; // table[i][j] = min in range starting at i, length 2^j
    int log_table[9]; // precomputed floor(log2(i))
    int n;
};

int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

void sparse_build(struct SparseTable *st, int *arr, int n) {
    int i;
    int j;
    int len;
    st->n = n;

    // Compute log table
    st->log_table[0] = -1;
    st->log_table[1] = 0;
    for (i = 2; i <= n; i++) {
        st->log_table[i] = st->log_table[i / 2] + 1;
    }

    // Initialize level 0 (ranges of length 1)
    for (i = 0; i < n; i++) {
        st->table[i][0] = arr[i];
    }

    // Build higher levels
    for (j = 1; (1 << j) <= n; j++) {
        len = 1 << j;
        for (i = 0; i + len - 1 < n; i++) {
            st->table[i][j] = min_val(st->table[i][j - 1],
                                       st->table[i + (1 << (j - 1))][j - 1]);
        }
    }
}

int sparse_query(struct SparseTable *st, int l, int r) {
    int k;
    int len;
    len = r - l + 1;
    k = st->log_table[len];
    return min_val(st->table[l][k], st->table[r - (1 << k) + 1][k]);
}

// Build a sparse table for maximum queries
struct SparseTableMax {
    int table[8][4];
    int log_table[9];
    int n;
};

void sparse_build_max(struct SparseTableMax *st, int *arr, int n) {
    int i;
    int j;
    int len;
    st->n = n;

    st->log_table[0] = -1;
    st->log_table[1] = 0;
    for (i = 2; i <= n; i++) {
        st->log_table[i] = st->log_table[i / 2] + 1;
    }

    for (i = 0; i < n; i++) {
        st->table[i][0] = arr[i];
    }

    for (j = 1; (1 << j) <= n; j++) {
        len = 1 << j;
        for (i = 0; i + len - 1 < n; i++) {
            st->table[i][j] = max_val(st->table[i][j - 1],
                                       st->table[i + (1 << (j - 1))][j - 1]);
        }
    }
}

int sparse_query_max(struct SparseTableMax *st, int l, int r) {
    int k;
    int len;
    len = r - l + 1;
    k = st->log_table[len];
    return max_val(st->table[l][k], st->table[r - (1 << k) + 1][k]);
}

int main() {
    struct SparseTable st;
    struct SparseTableMax stm;
    int arr[8];
    int i;

    arr[0] = 7;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 0;
    arr[4] = 5;
    arr[5] = 10;
    arr[6] = 3;
    arr[7] = 12;

    sparse_build(&st, arr, 8);

    printf("Min [0..7]: %d\n", sparse_query(&st, 0, 7));
    // EXPECT: Min [0..7]: 0

    printf("Min [0..3]: %d\n", sparse_query(&st, 0, 3));
    // EXPECT: Min [0..3]: 0

    printf("Min [4..7]: %d\n", sparse_query(&st, 4, 7));
    // EXPECT: Min [4..7]: 3

    printf("Min [1..2]: %d\n", sparse_query(&st, 1, 2));
    // EXPECT: Min [1..2]: 2

    printf("Min [0..0]: %d\n", sparse_query(&st, 0, 0));
    // EXPECT: Min [0..0]: 7

    printf("Min [5..7]: %d\n", sparse_query(&st, 5, 7));
    // EXPECT: Min [5..7]: 3

    printf("Min [2..5]: %d\n", sparse_query(&st, 2, 5));
    // EXPECT: Min [2..5]: 0

    printf("Min [6..7]: %d\n", sparse_query(&st, 6, 7));
    // EXPECT: Min [6..7]: 3

    // Test with max sparse table
    sparse_build_max(&stm, arr, 8);

    printf("Max [0..7]: %d\n", sparse_query_max(&stm, 0, 7));
    // EXPECT: Max [0..7]: 12

    printf("Max [0..3]: %d\n", sparse_query_max(&stm, 0, 3));
    // EXPECT: Max [0..3]: 7

    printf("Max [4..7]: %d\n", sparse_query_max(&stm, 4, 7));
    // EXPECT: Max [4..7]: 12

    printf("Max [1..2]: %d\n", sparse_query_max(&stm, 1, 2));
    // EXPECT: Max [1..2]: 3

    printf("Max [2..5]: %d\n", sparse_query_max(&stm, 2, 5));
    // EXPECT: Max [2..5]: 10

    // Log table verification
    printf("log[1]: %d\n", st.log_table[1]);
    // EXPECT: log[1]: 0
    printf("log[4]: %d\n", st.log_table[4]);
    // EXPECT: log[4]: 2
    printf("log[7]: %d\n", st.log_table[7]);
    // EXPECT: log[7]: 2
    printf("log[8]: %d\n", st.log_table[8]);
    // EXPECT: log[8]: 3

    return 0;
}
