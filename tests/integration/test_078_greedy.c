int printf(const char *fmt, ...);
// EXPECT: Activity Selection\nSelected=4\nSelNoOvlp=4\nSelAllOvlp=1\nFraction Knapsack\nFKnapsack5024000\nFKnapsack6028000\nFKnapsack106000\nKnap01Cap50220\nHuffman Merge\nMergeCost=224\nMergeCost=9\nMergeCost=80\nMergeCost=20\nMinimum Coins\nGreedy 41=4\nGreedy 30=2\nGreedy 99=9\nDPCoins  41=4\nDPCoins  99=9\nGreedy 6=3\nDPCoins  6=2\nJob Scheduling\nJobProfit= 40\nJobsSched= 3\nMST Kruskal\nMSTWeight=15\nPlatforms\nMinPlatfm=3\nAll  passed=1

// Greedy algorithms: activity selection, fractional knapsack (integer),
// Huffman-like merge costs, minimum coins

// ============ ACTIVITY SELECTION ============
int act_start[30];
int act_end[30];
int act_selected[30];
int act_sel_count;

void swap_int_arr(int a[30], int i, int j) {
    int t;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void sort_by_end_time(int s[30], int e[30], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (e[j] > e[j + 1] || (e[j] == e[j + 1] && s[j] > s[j + 1])) {
                swap_int_arr(s, j, j + 1);
                swap_int_arr(e, j, j + 1);
            }
        }
    }
}

int activity_selection(int s[30], int e[30], int n) {
    int count;
    int last_end;
    int i;

    sort_by_end_time(s, e, n);

    count = 1;
    act_selected[0] = 0;
    last_end = e[0];

    for (i = 1; i < n; i = i + 1) {
        if (s[i] >= last_end) {
            act_selected[count] = i;
            count = count + 1;
            last_end = e[i];
        }
    }
    act_sel_count = count;
    return count;
}

// ============ FRACTIONAL KNAPSACK (INTEGER VERSION) ============
// Use value-per-weight ratio (integer approximation: value*100/weight)
int kn_weight[20];
int kn_value[20];
int kn_ratio[20]; // value * 1000 / weight

void compute_ratios(int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        kn_ratio[i] = (kn_value[i] * 1000) / kn_weight[i];
    }
}

void sort_by_ratio(int n) {
    int i;
    int j;
    int t;
    // Sort descending by ratio
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (kn_ratio[j] < kn_ratio[j + 1]) {
                t = kn_ratio[j]; kn_ratio[j] = kn_ratio[j+1]; kn_ratio[j+1] = t;
                t = kn_weight[j]; kn_weight[j] = kn_weight[j+1]; kn_weight[j+1] = t;
                t = kn_value[j]; kn_value[j] = kn_value[j+1]; kn_value[j+1] = t;
            }
        }
    }
}

// Fractional knapsack: returns value * 100 (to handle fractions)
int fractional_knapsack(int n, int capacity) {
    int total_value_x100;
    int remaining;
    int i;

    compute_ratios(n);
    sort_by_ratio(n);

    total_value_x100 = 0;
    remaining = capacity;

    for (i = 0; i < n; i = i + 1) {
        if (remaining <= 0) {
            break;
        }
        if (kn_weight[i] <= remaining) {
            total_value_x100 = total_value_x100 + kn_value[i] * 100;
            remaining = remaining - kn_weight[i];
        } else {
            // Take fraction: value * remaining / weight, scaled by 100
            total_value_x100 = total_value_x100 + (kn_value[i] * remaining * 100) / kn_weight[i];
            remaining = 0;
        }
    }
    return total_value_x100;
}

// 0-1 Knapsack (DP for comparison)
int dp_knapsack[30][200]; // items x capacity

int knapsack_01(int weights[20], int values[20], int n, int capacity) {
    int i;
    int w;
    // Initialize
    for (w = 0; w <= capacity; w = w + 1) {
        dp_knapsack[0][w] = 0;
    }
    for (i = 1; i <= n; i = i + 1) {
        for (w = 0; w <= capacity; w = w + 1) {
            dp_knapsack[i][w] = dp_knapsack[i - 1][w];
            if (weights[i - 1] <= w) {
                int with_item;
                with_item = dp_knapsack[i - 1][w - weights[i - 1]] + values[i - 1];
                if (with_item > dp_knapsack[i][w]) {
                    dp_knapsack[i][w] = with_item;
                }
            }
        }
    }
    return dp_knapsack[n][capacity];
}

// ============ HUFFMAN-LIKE MERGE COSTS ============
// Given array of frequencies, merge two smallest at a time
// Total cost = sum of all merge costs
int merge_freqs[30];
int merge_n;

int min_merge_cost(int freqs[30], int n) {
    int costs[30];
    int cn;
    int total;
    int i;
    int min1_idx;
    int min2_idx;
    int min1;
    int min2;
    int merged;

    cn = n;
    for (i = 0; i < n; i = i + 1) {
        costs[i] = freqs[i];
    }

    total = 0;

    while (cn > 1) {
        // Find two minimums
        min1_idx = 0;
        min1 = costs[0];
        for (i = 1; i < cn; i = i + 1) {
            if (costs[i] < min1) {
                min1 = costs[i];
                min1_idx = i;
            }
        }
        // Remove min1 by swapping with last
        costs[min1_idx] = costs[cn - 1];
        cn = cn - 1;

        min2_idx = 0;
        min2 = costs[0];
        for (i = 1; i < cn; i = i + 1) {
            if (costs[i] < min2) {
                min2 = costs[i];
                min2_idx = i;
            }
        }
        // Replace min2 with merged value
        merged = min1 + min2;
        costs[min2_idx] = merged;
        total = total + merged;
    }

    return total;
}

// ============ MINIMUM COINS ============
int coins[10];
int coin_count_arr;

// Greedy coin change (works for standard denominations)
int greedy_coins(int denominations[10], int num_denom, int amount) {
    int count;
    int remaining;
    int i;

    // Sort denominations descending
    {
        int j;
        int t;
        for (i = 0; i < num_denom - 1; i = i + 1) {
            for (j = 0; j < num_denom - 1 - i; j = j + 1) {
                if (denominations[j] < denominations[j + 1]) {
                    t = denominations[j];
                    denominations[j] = denominations[j + 1];
                    denominations[j + 1] = t;
                }
            }
        }
    }

    count = 0;
    remaining = amount;
    for (i = 0; i < num_denom; i = i + 1) {
        while (remaining >= denominations[i]) {
            count = count + 1;
            remaining = remaining - denominations[i];
        }
    }
    if (remaining > 0) {
        return -1; // cannot make change
    }
    return count;
}

// DP coin change for comparison
int dp_coins(int denominations[10], int num_denom, int amount) {
    int dp[200];
    int i;
    int j;

    dp[0] = 0;
    for (i = 1; i <= amount; i = i + 1) {
        dp[i] = 9999;
    }

    for (i = 1; i <= amount; i = i + 1) {
        for (j = 0; j < num_denom; j = j + 1) {
            if (denominations[j] <= i) {
                if (dp[i - denominations[j]] + 1 < dp[i]) {
                    dp[i] = dp[i - denominations[j]] + 1;
                }
            }
        }
    }

    if (dp[amount] >= 9999) {
        return -1;
    }
    return dp[amount];
}

// ============ JOB SCHEDULING WITH DEADLINES ============
int job_profit[20];
int job_deadline[20];
int job_order[20];

void sort_jobs_by_profit(int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n; i = i + 1) {
        job_order[i] = i;
    }
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (job_profit[job_order[j]] < job_profit[job_order[j + 1]]) {
                t = job_order[j];
                job_order[j] = job_order[j + 1];
                job_order[j + 1] = t;
            }
        }
    }
}

int job_schedule(int n) {
    int slots[20];
    int total_profit;
    int i;
    int j;
    int placed;

    for (i = 0; i < 20; i = i + 1) {
        slots[i] = -1;
    }

    sort_jobs_by_profit(n);
    total_profit = 0;

    for (i = 0; i < n; i = i + 1) {
        int job;
        job = job_order[i];
        // Try to place job in latest available slot before deadline
        placed = 0;
        for (j = job_deadline[job] - 1; j >= 0; j = j - 1) {
            if (slots[j] == -1) {
                slots[j] = job;
                total_profit = total_profit + job_profit[job];
                placed = 1;
                break;
            }
        }
    }
    return total_profit;
}

// Count scheduled jobs
int count_scheduled_jobs(int n) {
    int slots[20];
    int count;
    int i;
    int j;

    for (i = 0; i < 20; i = i + 1) {
        slots[i] = -1;
    }

    sort_jobs_by_profit(n);
    count = 0;

    for (i = 0; i < n; i = i + 1) {
        int job;
        job = job_order[i];
        for (j = job_deadline[job] - 1; j >= 0; j = j - 1) {
            if (slots[j] == -1) {
                slots[j] = job;
                count = count + 1;
                break;
            }
        }
    }
    return count;
}

// ============ MINIMUM SPANNING TREE (Kruskal-like on small graph) ============
int edge_u[30];
int edge_v[30];
int edge_w[30];
int parent[10];
int rank_arr[10];

void init_uf(int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        parent[i] = i;
        rank_arr[i] = 0;
    }
}

int find_uf(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int union_uf(int x, int y) {
    int px;
    int py;
    px = find_uf(x);
    py = find_uf(y);
    if (px == py) {
        return 0;
    }
    if (rank_arr[px] < rank_arr[py]) {
        parent[px] = py;
    } else if (rank_arr[px] > rank_arr[py]) {
        parent[py] = px;
    } else {
        parent[py] = px;
        rank_arr[px] = rank_arr[px] + 1;
    }
    return 1;
}

int kruskal_mst(int num_edges, int num_nodes) {
    int i;
    int j;
    int t;
    int mst_weight;
    int edges_added;

    // Sort edges by weight
    for (i = 0; i < num_edges - 1; i = i + 1) {
        for (j = 0; j < num_edges - 1 - i; j = j + 1) {
            if (edge_w[j] > edge_w[j + 1]) {
                t = edge_u[j]; edge_u[j] = edge_u[j+1]; edge_u[j+1] = t;
                t = edge_v[j]; edge_v[j] = edge_v[j+1]; edge_v[j+1] = t;
                t = edge_w[j]; edge_w[j] = edge_w[j+1]; edge_w[j+1] = t;
            }
        }
    }

    init_uf(num_nodes);
    mst_weight = 0;
    edges_added = 0;

    for (i = 0; i < num_edges; i = i + 1) {
        if (union_uf(edge_u[i], edge_v[i]) == 1) {
            mst_weight = mst_weight + edge_w[i];
            edges_added = edges_added + 1;
        }
        if (edges_added == num_nodes - 1) {
            break;
        }
    }
    return mst_weight;
}

// ============ PLATFORM ASSIGNMENT ============
// Minimum platforms needed at a station (similar to interval max overlap)
int train_arrive[20];
int train_depart[20];

int min_platforms(int n) {
    int arr_sorted[20];
    int dep_sorted[20];
    int i;
    int j;
    int t;
    int plat;
    int max_plat;
    int ai;
    int di;

    for (i = 0; i < n; i = i + 1) {
        arr_sorted[i] = train_arrive[i];
        dep_sorted[i] = train_depart[i];
    }

    // Sort arrivals
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr_sorted[j] > arr_sorted[j + 1]) {
                t = arr_sorted[j]; arr_sorted[j] = arr_sorted[j+1]; arr_sorted[j+1] = t;
            }
        }
    }

    // Sort departures
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (dep_sorted[j] > dep_sorted[j + 1]) {
                t = dep_sorted[j]; dep_sorted[j] = dep_sorted[j+1]; dep_sorted[j+1] = t;
            }
        }
    }

    plat = 0;
    max_plat = 0;
    ai = 0;
    di = 0;

    while (ai < n && di < n) {
        if (arr_sorted[ai] <= dep_sorted[di]) {
            plat = plat + 1;
            if (plat > max_plat) {
                max_plat = plat;
            }
            ai = ai + 1;
        } else {
            plat = plat - 1;
            di = di + 1;
        }
    }
    return max_plat;
}

// Helper functions
int min_of(int a, int b) {
    if (a < b) return a;
    return b;
}

int max_of(int a, int b) {
    if (a > b) return a;
    return b;
}

int main(void) {
    int n;
    int i;
    int result;
    int denom[10];

    // ===== ACTIVITY SELECTION =====
    printf("%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c", 'A','c','t','i','v','i','t','y', 'S','e','l','e','c','t','i','o','n');
    printf("%c", 10);

    act_start[0]=1; act_end[0]=4;
    act_start[1]=3; act_end[1]=5;
    act_start[2]=0; act_end[2]=6;
    act_start[3]=5; act_end[3]=7;
    act_start[4]=3; act_end[4]=9;
    act_start[5]=5; act_end[5]=9;
    act_start[6]=6; act_end[6]=10;
    act_start[7]=8; act_end[7]=11;
    act_start[8]=8; act_end[8]=12;
    act_start[9]=2; act_end[9]=14;
    act_start[10]=12; act_end[10]=16;
    n = 11;

    result = activity_selection(act_start, act_end, n);
    printf("%c%c%c%c%c%c%c%c%c%d", 'S','e','l','e','c','t','e','d','=', result);
    printf("%c", 10);

    // Second test
    act_start[0]=1; act_end[0]=2;
    act_start[1]=3; act_end[1]=4;
    act_start[2]=5; act_end[2]=6;
    act_start[3]=7; act_end[3]=8;
    n = 4;
    result = activity_selection(act_start, act_end, n);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'S','e','l','N','o','O','v','l','p','=', result);
    printf("%c", 10);

    // Third test - all overlapping
    act_start[0]=1; act_end[0]=10;
    act_start[1]=2; act_end[1]=9;
    act_start[2]=3; act_end[2]=8;
    act_start[3]=4; act_end[3]=7;
    n = 4;
    result = activity_selection(act_start, act_end, n);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'S','e','l','A','l','l','O','v','l','p','=', result);
    printf("%c", 10);

    // ===== FRACTIONAL KNAPSACK =====
    printf("%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c", 'F','r','a','c','t','i','o','n', 'K','n','a','p','s','a','c','k');
    printf("%c", 10);

    kn_weight[0]=10; kn_value[0]=60;
    kn_weight[1]=20; kn_value[1]=100;
    kn_weight[2]=30; kn_value[2]=120;
    n = 3;
    result = fractional_knapsack(n, 50);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'F','K','n','a','p','s','a','c','k','5','0', result);
    printf("%c", 10);

    // Full capacity
    kn_weight[0]=10; kn_value[0]=60;
    kn_weight[1]=20; kn_value[1]=100;
    kn_weight[2]=30; kn_value[2]=120;
    result = fractional_knapsack(3, 60);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'F','K','n','a','p','s','a','c','k','6','0', result);
    printf("%c", 10);

    // Small capacity
    kn_weight[0]=10; kn_value[0]=60;
    kn_weight[1]=20; kn_value[1]=100;
    kn_weight[2]=30; kn_value[2]=120;
    result = fractional_knapsack(3, 10);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'F','K','n','a','p','s','a','c','k','1','0', result);
    printf("%c", 10);

    // 0-1 Knapsack comparison
    {
        int w[20];
        int v[20];
        w[0]=10; v[0]=60;
        w[1]=20; v[1]=100;
        w[2]=30; v[2]=120;
        result = knapsack_01(w, v, 3, 50);
        printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'K','n','a','p','0','1','C','a','p','5','0', result);
        printf("%c", 10);
    }

    // ===== HUFFMAN MERGE COSTS =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c", 'H','u','f','f','m','a','n', 'M','e','r','g','e');
    printf("%c", 10);

    merge_freqs[0]=5; merge_freqs[1]=9; merge_freqs[2]=12;
    merge_freqs[3]=13; merge_freqs[4]=16; merge_freqs[5]=45;
    result = min_merge_cost(merge_freqs, 6);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','e','r','g','e','C','o','s','t','=', result);
    printf("%c", 10);

    // Small example
    merge_freqs[0]=1; merge_freqs[1]=2; merge_freqs[2]=3;
    result = min_merge_cost(merge_freqs, 3);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','e','r','g','e','C','o','s','t','=', result);
    printf("%c", 10);

    // Equal frequencies
    merge_freqs[0]=10; merge_freqs[1]=10; merge_freqs[2]=10; merge_freqs[3]=10;
    result = min_merge_cost(merge_freqs, 4);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','e','r','g','e','C','o','s','t','=', result);
    printf("%c", 10);

    // Two elements
    merge_freqs[0]=5; merge_freqs[1]=15;
    result = min_merge_cost(merge_freqs, 2);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','e','r','g','e','C','o','s','t','=', result);
    printf("%c", 10);

    // ===== MINIMUM COINS =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c", 'M','i','n','i','m','u','m', 'C','o','i','n','s');
    printf("%c", 10);

    denom[0]=1; denom[1]=5; denom[2]=10; denom[3]=25;
    result = greedy_coins(denom, 4, 41);
    printf("%c%c%c%c%c%c %d%c%d", 'G','r','e','e','d','y', 41, '=', result);
    printf("%c", 10);

    denom[0]=1; denom[1]=5; denom[2]=10; denom[3]=25;
    result = greedy_coins(denom, 4, 30);
    printf("%c%c%c%c%c%c %d%c%d", 'G','r','e','e','d','y', 30, '=', result);
    printf("%c", 10);

    denom[0]=1; denom[1]=5; denom[2]=10; denom[3]=25;
    result = greedy_coins(denom, 4, 99);
    printf("%c%c%c%c%c%c %d%c%d", 'G','r','e','e','d','y', 99, '=', result);
    printf("%c", 10);

    // DP coins
    denom[0]=1; denom[1]=5; denom[2]=10; denom[3]=25;
    result = dp_coins(denom, 4, 41);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'D','P','C','o','i','n','s',' ', 41, '=', result);
    printf("%c", 10);

    denom[0]=1; denom[1]=5; denom[2]=10; denom[3]=25;
    result = dp_coins(denom, 4, 99);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'D','P','C','o','i','n','s',' ', 99, '=', result);
    printf("%c", 10);

    // Non-standard denominations where greedy fails
    denom[0]=1; denom[1]=3; denom[2]=4;
    result = greedy_coins(denom, 3, 6);
    printf("%c%c%c%c%c%c %d%c%d", 'G','r','e','e','d','y', 6, '=', result);
    printf("%c", 10);

    denom[0]=1; denom[1]=3; denom[2]=4;
    result = dp_coins(denom, 3, 6);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'D','P','C','o','i','n','s',' ', 6, '=', result);
    printf("%c", 10);

    // ===== JOB SCHEDULING =====
    printf("%c%c%c %c%c%c%c%c%c%c%c%c%c", 'J','o','b', 'S','c','h','e','d','u','l','i','n','g');
    printf("%c", 10);

    job_profit[0]=20; job_deadline[0]=2;
    job_profit[1]=15; job_deadline[1]=2;
    job_profit[2]=10; job_deadline[2]=1;
    job_profit[3]=5;  job_deadline[3]=3;
    job_profit[4]=1;  job_deadline[4]=3;
    n = 5;

    result = job_schedule(n);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'J','o','b','P','r','o','f','i','t','=', ' ', result);
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'J','o','b','s','S','c','h','e','d','=', ' ', count_scheduled_jobs(n));
    printf("%c", 10);

    // ===== MST =====
    printf("%c%c%c %c%c%c%c%c%c%c", 'M','S','T', 'K','r','u','s','k','a','l');
    printf("%c", 10);

    // Graph with 5 nodes, 7 edges
    edge_u[0]=0; edge_v[0]=1; edge_w[0]=4;
    edge_u[1]=0; edge_v[1]=2; edge_w[1]=8;
    edge_u[2]=1; edge_v[2]=2; edge_w[2]=2;
    edge_u[3]=1; edge_v[3]=3; edge_w[3]=5;
    edge_u[4]=2; edge_v[4]=3; edge_w[4]=5;
    edge_u[5]=2; edge_v[5]=4; edge_w[5]=9;
    edge_u[6]=3; edge_v[6]=4; edge_w[6]=4;

    result = kruskal_mst(7, 5);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','S','T','W','e','i','g','h','t','=', result);
    printf("%c", 10);

    // ===== PLATFORMS =====
    printf("%c%c%c%c%c%c%c%c%c", 'P','l','a','t','f','o','r','m','s');
    printf("%c", 10);

    train_arrive[0]=100; train_depart[0]=130;
    train_arrive[1]=110; train_depart[1]=150;
    train_arrive[2]=120; train_depart[2]=140;
    train_arrive[3]=200; train_depart[3]=230;
    train_arrive[4]=300; train_depart[4]=340;
    train_arrive[5]=350; train_depart[5]=400;
    n = 6;
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','i','n','P','l','a','t','f','m','=', min_platforms(n));
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
