int printf(const char *fmt, ...);

// Query plan cost estimation
// Models: sequential scan, index scan, nested loop join, sort, filter
// Cost model: I/O cost + CPU cost, all in integer units

enum PlanNodeType {
    PLAN_SEQ_SCAN,
    PLAN_INDEX_SCAN,
    PLAN_FILTER,
    PLAN_SORT,
    PLAN_NESTED_LOOP_JOIN,
    PLAN_HASH_JOIN,
    PLAN_PROJECT
};

struct PlanNode {
    int type;
    int table_rows;
    int result_rows;
    int selectivity_pct;
    int index_depth;
    int left_child;
    int right_child;
    int io_cost;
    int cpu_cost;
    int total_cost;
};

struct PlanNode g_nodes[20];
int g_nnodes;

void plan_init(void) {
    g_nnodes = 0;
}

int add_seq_scan(int table_rows) {
    int idx = g_nnodes;
    g_nodes[idx].type = PLAN_SEQ_SCAN;
    g_nodes[idx].table_rows = table_rows;
    g_nodes[idx].result_rows = table_rows;
    g_nodes[idx].selectivity_pct = 100;
    g_nodes[idx].index_depth = 0;
    g_nodes[idx].left_child = -1;
    g_nodes[idx].right_child = -1;
    g_nodes[idx].io_cost = (table_rows + 9) / 10;
    g_nodes[idx].cpu_cost = table_rows;
    g_nodes[idx].total_cost = g_nodes[idx].io_cost + g_nodes[idx].cpu_cost;
    g_nnodes++;
    return idx;
}

int add_index_scan(int table_rows, int result_rows, int depth) {
    int idx = g_nnodes;
    g_nodes[idx].type = PLAN_INDEX_SCAN;
    g_nodes[idx].table_rows = table_rows;
    g_nodes[idx].result_rows = result_rows;
    g_nodes[idx].selectivity_pct = (result_rows * 100) / table_rows;
    g_nodes[idx].index_depth = depth;
    g_nodes[idx].left_child = -1;
    g_nodes[idx].right_child = -1;
    g_nodes[idx].io_cost = depth + result_rows;
    g_nodes[idx].cpu_cost = depth + result_rows;
    g_nodes[idx].total_cost = g_nodes[idx].io_cost + g_nodes[idx].cpu_cost;
    g_nnodes++;
    return idx;
}

int add_filter(int child_idx, int selectivity_pct) {
    int idx = g_nnodes;
    int child_rows = g_nodes[child_idx].result_rows;
    g_nodes[idx].type = PLAN_FILTER;
    g_nodes[idx].table_rows = child_rows;
    g_nodes[idx].result_rows = (child_rows * selectivity_pct) / 100;
    if (g_nodes[idx].result_rows < 1 && child_rows > 0)
        g_nodes[idx].result_rows = 1;
    g_nodes[idx].selectivity_pct = selectivity_pct;
    g_nodes[idx].index_depth = 0;
    g_nodes[idx].left_child = child_idx;
    g_nodes[idx].right_child = -1;
    g_nodes[idx].io_cost = g_nodes[child_idx].io_cost;
    g_nodes[idx].cpu_cost = g_nodes[child_idx].cpu_cost + child_rows;
    g_nodes[idx].total_cost = g_nodes[idx].io_cost + g_nodes[idx].cpu_cost;
    g_nnodes++;
    return idx;
}

int add_sort(int child_idx) {
    int idx = g_nnodes;
    int n = g_nodes[child_idx].result_rows;
    g_nodes[idx].type = PLAN_SORT;
    g_nodes[idx].table_rows = n;
    g_nodes[idx].result_rows = n;
    g_nodes[idx].selectivity_pct = 100;
    g_nodes[idx].index_depth = 0;
    g_nodes[idx].left_child = child_idx;
    g_nodes[idx].right_child = -1;
    int log2n = 0;
    int tmp = n;
    while (tmp > 1) { log2n++; tmp = tmp / 2; }
    g_nodes[idx].io_cost = g_nodes[child_idx].io_cost;
    g_nodes[idx].cpu_cost = g_nodes[child_idx].cpu_cost + n * log2n;
    g_nodes[idx].total_cost = g_nodes[idx].io_cost + g_nodes[idx].cpu_cost;
    g_nnodes++;
    return idx;
}

int add_nested_loop_join(int left_idx, int right_idx) {
    int idx = g_nnodes;
    int left_rows = g_nodes[left_idx].result_rows;
    int right_rows = g_nodes[right_idx].result_rows;
    g_nodes[idx].type = PLAN_NESTED_LOOP_JOIN;
    g_nodes[idx].table_rows = left_rows * right_rows;
    g_nodes[idx].result_rows = (left_rows * right_rows) / 10;
    if (g_nodes[idx].result_rows < 1) g_nodes[idx].result_rows = 1;
    g_nodes[idx].selectivity_pct = 10;
    g_nodes[idx].index_depth = 0;
    g_nodes[idx].left_child = left_idx;
    g_nodes[idx].right_child = right_idx;
    g_nodes[idx].io_cost = g_nodes[left_idx].io_cost +
                            left_rows * g_nodes[right_idx].io_cost;
    g_nodes[idx].cpu_cost = g_nodes[left_idx].cpu_cost +
                             left_rows * g_nodes[right_idx].cpu_cost;
    g_nodes[idx].total_cost = g_nodes[idx].io_cost + g_nodes[idx].cpu_cost;
    g_nnodes++;
    return idx;
}

char *type_name(int t) {
    if (t == PLAN_SEQ_SCAN) return "SeqScan";
    if (t == PLAN_INDEX_SCAN) return "IdxScan";
    if (t == PLAN_FILTER) return "Filter";
    if (t == PLAN_SORT) return "Sort";
    if (t == PLAN_NESTED_LOOP_JOIN) return "NLJoin";
    if (t == PLAN_HASH_JOIN) return "HashJoin";
    if (t == PLAN_PROJECT) return "Project";
    return "Unknown";
}

int choose_cheaper(int a, int b) {
    if (g_nodes[a].total_cost <= g_nodes[b].total_cost) return a;
    return b;
}

int main() {
    plan_init();

    // Plan 1: SeqScan on 1000-row table
    int ss1 = add_seq_scan(1000);
    printf("SeqScan(1000): io=%d cpu=%d total=%d rows=%d\n",
           g_nodes[ss1].io_cost, g_nodes[ss1].cpu_cost,
           g_nodes[ss1].total_cost, g_nodes[ss1].result_rows);
    // EXPECT: SeqScan(1000): io=100 cpu=1000 total=1100 rows=1000

    // Plan 2: IndexScan on same table, 50 matching rows, depth 3
    int is1 = add_index_scan(1000, 50, 3);
    printf("IdxScan(1000,50): io=%d cpu=%d total=%d rows=%d\n",
           g_nodes[is1].io_cost, g_nodes[is1].cpu_cost,
           g_nodes[is1].total_cost, g_nodes[is1].result_rows);
    // EXPECT: IdxScan(1000,50): io=53 cpu=53 total=106 rows=50

    int cheaper = choose_cheaper(ss1, is1);
    printf("Cheaper scan: %s\n", type_name(g_nodes[cheaper].type));
    // EXPECT: Cheaper scan: IdxScan

    // Plan 3: SeqScan + Filter(20%)
    int ss2 = add_seq_scan(500);
    int f1 = add_filter(ss2, 20);
    printf("SeqScan+Filter(500,20): total=%d rows=%d\n",
           g_nodes[f1].total_cost, g_nodes[f1].result_rows);
    // EXPECT: SeqScan+Filter(500,20): total=1050 rows=100

    // Plan 4: SeqScan + Sort
    int ss3 = add_seq_scan(200);
    int s1 = add_sort(ss3);
    printf("SeqScan+Sort(200): total=%d rows=%d\n",
           g_nodes[s1].total_cost, g_nodes[s1].result_rows);
    // EXPECT: SeqScan+Sort(200): total=1620 rows=200

    // Plan 5: Nested Loop Join
    int ss_left = add_seq_scan(100);
    int ss_right = add_seq_scan(200);
    int nlj = add_nested_loop_join(ss_left, ss_right);
    printf("NLJoin(100x200): io=%d cpu=%d total=%d rows=%d\n",
           g_nodes[nlj].io_cost, g_nodes[nlj].cpu_cost,
           g_nodes[nlj].total_cost, g_nodes[nlj].result_rows);
    // EXPECT: NLJoin(100x200): io=2010 cpu=20100 total=22110 rows=2000

    // Plan 6: Compare join orders (A join B vs B join A)
    int ss_a = add_seq_scan(50);
    int ss_b = add_seq_scan(500);
    int join_ab = add_nested_loop_join(ss_a, ss_b);
    printf("Join(50x500): total=%d\n", g_nodes[join_ab].total_cost);
    // EXPECT: Join(50x500): total=27555

    int ss_b2 = add_seq_scan(500);
    int ss_a2 = add_seq_scan(50);
    int join_ba = add_nested_loop_join(ss_b2, ss_a2);
    printf("Join(500x50): total=%d\n", g_nodes[join_ba].total_cost);
    // EXPECT: Join(500x50): total=28050

    int better = choose_cheaper(join_ab, join_ba);
    printf("Better join order: small-first=%d\n",
           g_nodes[better].total_cost == g_nodes[join_ab].total_cost);
    // EXPECT: Better join order: small-first=1

    // Plan 7: Complex query plan with filter + sort
    int scan_emp = add_seq_scan(1000);
    int filt_emp = add_filter(scan_emp, 30);
    int sort_emp = add_sort(filt_emp);
    printf("Filtered+Sorted emps: rows=%d cost=%d\n",
           g_nodes[sort_emp].result_rows, g_nodes[sort_emp].total_cost);
    // EXPECT: Filtered+Sorted emps: rows=300 cost=4500

    printf("Total plan nodes: %d\n", g_nnodes); // EXPECT: Total plan nodes: 18

    printf("Query plan test passed\n"); // EXPECT: Query plan test passed
    return 0;
}
