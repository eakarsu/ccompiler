int printf(const char *fmt, ...);
// EXPECT: 80 21 0 3 8 3 8 16 4 24 22 9 2 9 110 15 2 2 24 2 1 0 4 3 8 1 0 7 7 5 1 3 3 1 14 100 100 4 2 5 6 16 10 4 4 3 120 1 8 5

// Bitmask DP: traveling salesman, subset iteration, bit manipulation DP

// TSP: given distance matrix, find minimum cost Hamiltonian cycle
// dp[mask][i] = min cost to visit cities in mask, ending at city i
int tsp_dp[64][6]; // 2^6 = 64 max, up to 6 cities
int tsp_dist[6][6];

int tsp_solve(int n) {
    int full_mask;
    int mask;
    int last;
    int prev;
    int i;
    int j;
    int cost;
    int mn;
    int INF;
    INF = 99999;
    full_mask = (1 << n) - 1;

    // Initialize
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        for (i = 0; i < n; i = i + 1) {
            tsp_dp[mask][i] = INF;
        }
    }
    tsp_dp[1][0] = 0; // Start at city 0

    for (mask = 1; mask <= full_mask; mask = mask + 1) {
        for (last = 0; last < n; last = last + 1) {
            if (tsp_dp[mask][last] == INF) continue;
            if ((mask & (1 << last)) == 0) continue;
            for (j = 0; j < n; j = j + 1) {
                if (mask & (1 << j)) continue; // already visited
                cost = tsp_dp[mask][last] + tsp_dist[last][j];
                if (cost < tsp_dp[mask | (1 << j)][j]) {
                    tsp_dp[mask | (1 << j)][j] = cost;
                }
            }
        }
    }

    // Find min cost to return to city 0
    mn = INF;
    for (last = 1; last < n; last = last + 1) {
        cost = tsp_dp[full_mask][last] + tsp_dist[last][0];
        if (cost < mn) mn = cost;
    }
    return mn;
}

// Count number of set bits (popcount)
int popcount(int x) {
    int count;
    count = 0;
    while (x > 0) {
        count = count + (x & 1);
        x = x >> 1;
    }
    return count;
}

// Iterate over all subsets of a given mask
// Return count of subsets
int count_subsets(int mask) {
    int sub;
    int count;
    count = 0;
    sub = mask;
    while (1) {
        count = count + 1;
        if (sub == 0) break;
        sub = (sub - 1) & mask;
    }
    return count;
}

// Sum of all subsets of mask (sum of values for set bits)
int subset_sum_all(int values[], int n, int mask) {
    int sum;
    int sub;
    int i;
    int sub_sum;
    sum = 0;
    sub = mask;
    while (1) {
        sub_sum = 0;
        for (i = 0; i < n; i = i + 1) {
            if (sub & (1 << i)) {
                sub_sum = sub_sum + values[i];
            }
        }
        sum = sum + sub_sum;
        if (sub == 0) break;
        sub = (sub - 1) & mask;
    }
    return sum;
}

// Bitmask DP: assignment problem
// n workers, n jobs, cost[i][j] = cost of assigning worker i to job j
// Find minimum total cost
int assign_dp[64];
int assign_cost[6][6];

int assignment_problem(int n) {
    int full_mask;
    int mask;
    int i;
    int j;
    int cost;
    int INF;
    INF = 99999;
    full_mask = (1 << n) - 1;

    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        assign_dp[mask] = INF;
    }
    assign_dp[0] = 0;

    for (mask = 0; mask < full_mask; mask = mask + 1) {
        if (assign_dp[mask] == INF) continue;
        i = popcount(mask); // which worker to assign next
        for (j = 0; j < n; j = j + 1) {
            if (mask & (1 << j)) continue; // job j already taken
            cost = assign_dp[mask] + assign_cost[i][j];
            if (cost < assign_dp[mask | (1 << j)]) {
                assign_dp[mask | (1 << j)] = cost;
            }
        }
    }
    return assign_dp[full_mask];
}

// Bitmask DP: max weight independent set on a path graph
// dp[mask] represents selecting nodes in mask, check adjacency
int is_independent(int mask, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if ((mask & (1 << i)) && (mask & (1 << (i + 1)))) {
            return 0;
        }
    }
    return 1;
}

int max_independent_set(int weights[], int n) {
    int mask;
    int full_mask;
    int sum;
    int mx;
    int i;
    full_mask = (1 << n) - 1;
    mx = 0;
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        if (is_independent(mask, n)) {
            sum = 0;
            for (i = 0; i < n; i = i + 1) {
                if (mask & (1 << i)) {
                    sum = sum + weights[i];
                }
            }
            if (sum > mx) mx = sum;
        }
    }
    return mx;
}

// Bitmask: minimum vertex cover (complement of maximum independent set)
int min_vertex_cover(int adj[6][6], int n) {
    int mask;
    int full_mask;
    int mn;
    int is_cover;
    int i;
    int j;
    int cnt;
    full_mask = (1 << n) - 1;
    mn = n;
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        // Check if mask is a valid vertex cover
        is_cover = 1;
        for (i = 0; i < n && is_cover; i = i + 1) {
            for (j = i + 1; j < n && is_cover; j = j + 1) {
                if (adj[i][j]) {
                    if ((mask & (1 << i)) == 0 && (mask & (1 << j)) == 0) {
                        is_cover = 0;
                    }
                }
            }
        }
        if (is_cover) {
            cnt = popcount(mask);
            if (cnt < mn) mn = cnt;
        }
    }
    return mn;
}

// Bitmask: count Hamiltonian paths
int count_hamiltonian_paths(int adj2[6][6], int n) {
    int ham_dp[64][6];
    int mask;
    int i;
    int j;
    int full_mask;
    int count;
    full_mask = (1 << n) - 1;

    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        for (i = 0; i < n; i = i + 1) {
            ham_dp[mask][i] = 0;
        }
    }
    for (i = 0; i < n; i = i + 1) {
        ham_dp[1 << i][i] = 1;
    }

    for (mask = 1; mask <= full_mask; mask = mask + 1) {
        for (i = 0; i < n; i = i + 1) {
            if (ham_dp[mask][i] == 0) continue;
            if ((mask & (1 << i)) == 0) continue;
            for (j = 0; j < n; j = j + 1) {
                if (mask & (1 << j)) continue;
                if (adj2[i][j]) {
                    ham_dp[mask | (1 << j)][j] = ham_dp[mask | (1 << j)][j] + ham_dp[mask][i];
                }
            }
        }
    }
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        count = count + ham_dp[full_mask][i];
    }
    return count;
}

// Bitmask: partition into two equal-sum subsets
int can_equal_partition(int values[], int n) {
    int total;
    int half;
    int mask;
    int full_mask;
    int sum;
    int i;
    total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + values[i];
    }
    if (total % 2 != 0) return 0;
    half = total / 2;
    full_mask = (1 << n) - 1;
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        sum = 0;
        for (i = 0; i < n; i = i + 1) {
            if (mask & (1 << i)) {
                sum = sum + values[i];
            }
        }
        if (sum == half) return 1;
    }
    return 0;
}

// Bit manipulation utilities
int lowest_set_bit(int x) {
    return x & (-x);
}

int highest_bit_pos(int x) {
    int pos;
    pos = 0;
    while (x > 1) {
        x = x >> 1;
        pos = pos + 1;
    }
    return pos;
}

int clear_lowest_bit(int x) {
    return x & (x - 1);
}

int is_power_of_two(int x) {
    if (x <= 0) return 0;
    if ((x & (x - 1)) == 0) return 1;
    return 0;
}

// Toggle bit at position
int toggle_bit(int x, int pos) {
    return x ^ (1 << pos);
}

// Set bit at position
int set_bit(int x, int pos) {
    return x | (1 << pos);
}

// Clear bit at position
int clear_bit(int x, int pos) {
    return x & (~(1 << pos));
}

// Check bit at position
int check_bit(int x, int pos) {
    if (x & (1 << pos)) return 1;
    return 0;
}

// Count trailing zeros
int trailing_zeros(int x) {
    int count;
    if (x == 0) return 32;
    count = 0;
    while ((x & 1) == 0) {
        count = count + 1;
        x = x >> 1;
    }
    return count;
}

// Bitmask: maximum clique (brute force for small n)
int max_clique_size(int adj3[6][6], int n) {
    int mask;
    int full_mask;
    int mx;
    int is_cliq;
    int i;
    int j;
    int cnt;
    full_mask = (1 << n) - 1;
    mx = 0;
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        is_cliq = 1;
        cnt = 0;
        for (i = 0; i < n && is_cliq; i = i + 1) {
            if ((mask & (1 << i)) == 0) continue;
            cnt = cnt + 1;
            for (j = i + 1; j < n && is_cliq; j = j + 1) {
                if ((mask & (1 << j)) == 0) continue;
                if (adj3[i][j] == 0) is_cliq = 0;
            }
        }
        if (is_cliq && cnt > mx) mx = cnt;
    }
    return mx;
}

// Bitmask DP: weighted job scheduling (small n)
// Each job i has: start, end, profit. Select non-overlapping jobs for max profit.
int job_start[10];
int job_end[10];
int job_profit[10];

int max_job_profit(int n) {
    int mask;
    int full_mask;
    int mx;
    int i;
    int j;
    int ok;
    int sum;
    full_mask = (1 << n) - 1;
    mx = 0;
    for (mask = 0; mask <= full_mask; mask = mask + 1) {
        ok = 1;
        sum = 0;
        for (i = 0; i < n && ok; i = i + 1) {
            if ((mask & (1 << i)) == 0) continue;
            sum = sum + job_profit[i];
            for (j = i + 1; j < n && ok; j = j + 1) {
                if ((mask & (1 << j)) == 0) continue;
                // Check overlap: i and j overlap if not (end[i]<=start[j] or end[j]<=start[i])
                if (job_end[i] > job_start[j] && job_end[j] > job_start[i]) {
                    ok = 0;
                }
            }
        }
        if (ok && sum > mx) mx = sum;
    }
    return mx;
}

// Subset XOR: XOR of all subset sums
int subset_xor(int values[], int n) {
    int mask;
    int full_mask;
    int xor_val;
    int sum;
    int i;
    full_mask = (1 << n) - 1;
    xor_val = 0;
    for (mask = 1; mask <= full_mask; mask = mask + 1) {
        sum = 0;
        for (i = 0; i < n; i = i + 1) {
            if (mask & (1 << i)) {
                sum = sum + values[i];
            }
        }
        xor_val = xor_val ^ sum;
    }
    return xor_val;
}

int main(void) {
    int values[10];
    int weights[10];
    int adj[6][6];
    int adj2[6][6];
    int adj3[6][6];
    int i;
    int j;

    // Test 1-2: TSP with 4 cities
    tsp_dist[0][0]=0;  tsp_dist[0][1]=10; tsp_dist[0][2]=15; tsp_dist[0][3]=20;
    tsp_dist[1][0]=10; tsp_dist[1][1]=0;  tsp_dist[1][2]=35; tsp_dist[1][3]=25;
    tsp_dist[2][0]=15; tsp_dist[2][1]=35; tsp_dist[2][2]=0;  tsp_dist[2][3]=30;
    tsp_dist[3][0]=20; tsp_dist[3][1]=25; tsp_dist[3][2]=30; tsp_dist[3][3]=0;
    printf("%d", tsp_solve(4)); // 80 (0->1->3->2->0: 10+25+30+15=80)

    // TSP with 4 cities, different
    tsp_dist[0][0]=0;  tsp_dist[0][1]=1;  tsp_dist[0][2]=15; tsp_dist[0][3]=6;
    tsp_dist[1][0]=2;  tsp_dist[1][1]=0;  tsp_dist[1][2]=7;  tsp_dist[1][3]=3;
    tsp_dist[2][0]=9;  tsp_dist[2][1]=6;  tsp_dist[2][2]=0;  tsp_dist[2][3]=12;
    tsp_dist[3][0]=10; tsp_dist[3][1]=4;  tsp_dist[3][2]=8;  tsp_dist[3][3]=0;
    printf(" %d", tsp_solve(4)); // 0->1->3->2->0: 1+3+8+9=21

    // Test 3-6: Popcount
    printf(" %d", popcount(0));   // 0
    printf(" %d", popcount(7));   // 3
    printf(" %d", popcount(255)); // 8
    printf(" %d", popcount(100)); // 3 (1100100)

    // Test 7-9: Count subsets
    printf(" %d", count_subsets(7));  // 2^3 = 8 (including empty)
    printf(" %d", count_subsets(15)); // 2^4 = 16
    printf(" %d", count_subsets(5));  // 4 (5=101: subsets are 101,100,001,000)

    // Test 10-11: Subset sum of all subsets
    values[0]=1; values[1]=2; values[2]=3;
    printf(" %d", subset_sum_all(values, 3, 7)); // subsets of {1,2,3}: {},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3} sums: 0+1+2+3+3+4+5+6 = 24
    values[0]=1; values[1]=10;
    printf(" %d", subset_sum_all(values, 2, 3)); // subsets of {1,10}: {},{1},{10},{11} sums: 0+1+10+11 = 22

    // Test 12-13: Assignment problem
    assign_cost[0][0]=9; assign_cost[0][1]=2; assign_cost[0][2]=7;
    assign_cost[1][0]=6; assign_cost[1][1]=4; assign_cost[1][2]=3;
    assign_cost[2][0]=5; assign_cost[2][1]=8; assign_cost[2][2]=1;
    printf(" %d", assignment_problem(3)); // w0->j1(2), w1->j0(6), w2->j2(1) = 9? or w0->j1(2),w1->j2(3),w2->j0(5)=10
    // Let me check all: 9+4+1=14, 9+3+8=20, 2+6+1=9, 2+3+5=10, 7+6+8=21, 7+4+5=16 => min=9

    // Another assignment
    assign_cost[0][0]=1; assign_cost[0][1]=2;
    assign_cost[1][0]=2; assign_cost[1][1]=1;
    printf(" %d", assignment_problem(2)); // min(1+1, 2+2) = 2

    // Test 14-16: Max independent set on path
    weights[0]=1; weights[1]=2; weights[2]=3; weights[3]=4; weights[4]=5;
    printf(" %d", max_independent_set(weights, 5)); // 1+3+5=9

    weights[0]=5; weights[1]=5; weights[2]=10; weights[3]=100; weights[4]=10; weights[5]=5;
    printf(" %d", max_independent_set(weights, 6)); // 5+100+5=110

    weights[0]=3; weights[1]=2; weights[2]=5; weights[3]=10; weights[4]=7;
    printf(" %d", max_independent_set(weights, 5)); // 3+5+7=15 or 3+10=13 or 2+10=12 => 15

    // Test 17-18: Vertex cover
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            adj[i][j] = 0;
        }
    }
    // Triangle: 0-1, 1-2, 0-2
    adj[0][1]=1; adj[1][0]=1;
    adj[1][2]=1; adj[2][1]=1;
    adj[0][2]=1; adj[2][0]=1;
    printf(" %d", min_vertex_cover(adj, 3)); // 2

    // Path: 0-1-2-3
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj[i][j] = 0;
    adj[0][1]=1; adj[1][0]=1;
    adj[1][2]=1; adj[2][1]=1;
    adj[2][3]=1; adj[3][2]=1;
    printf(" %d", min_vertex_cover(adj, 4)); // 2 (nodes 1,2)

    // Test 19-20: Hamiltonian paths
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj2[i][j] = 0;
    // Complete graph K4
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            if (i != j) adj2[i][j] = 1;
        }
    }
    printf(" %d", count_hamiltonian_paths(adj2, 4)); // 4! = 24

    // Path graph: 0-1-2-3
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj2[i][j] = 0;
    adj2[0][1]=1; adj2[1][0]=1;
    adj2[1][2]=1; adj2[2][1]=1;
    adj2[2][3]=1; adj2[3][2]=1;
    printf(" %d", count_hamiltonian_paths(adj2, 4)); // 2 (0-1-2-3 and 3-2-1-0)

    // Test 21-22: Equal partition
    values[0]=1; values[1]=5; values[2]=11; values[3]=5;
    printf(" %d", can_equal_partition(values, 4)); // 1

    values[0]=1; values[1]=2; values[2]=3; values[3]=5;
    printf(" %d", can_equal_partition(values, 4)); // 0 (sum=11, odd)

    // Test 23-26: Bit utilities
    printf(" %d", lowest_set_bit(12));    // 4 (1100 -> lowest = 100 = 4)
    printf(" %d", highest_bit_pos(8));    // 3
    printf(" %d", clear_lowest_bit(10));  // 8 (1010 -> 1000)
    printf(" %d", is_power_of_two(16));   // 1

    // Test 27-29: More bit utilities
    printf(" %d", is_power_of_two(15));  // 0
    printf(" %d", toggle_bit(5, 1));     // 7 (101 ^ 010 = 111)
    printf(" %d", set_bit(5, 1));        // 7 (101 | 010 = 111)

    // Test 30-32: More bit ops
    printf(" %d", clear_bit(7, 1));   // 5 (111 & ~010 = 101)
    printf(" %d", check_bit(5, 2));   // 1 (101, bit 2 = 1)
    printf(" %d", trailing_zeros(8)); // 3

    // Test 33-34: Max clique
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj3[i][j] = 0;
    // K3 embedded in 4 vertices: 0-1, 0-2, 1-2
    adj3[0][1]=1; adj3[1][0]=1;
    adj3[0][2]=1; adj3[2][0]=1;
    adj3[1][2]=1; adj3[2][1]=1;
    printf(" %d", max_clique_size(adj3, 4)); // 3

    // No edges
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj3[i][j] = 0;
    printf(" %d", max_clique_size(adj3, 4)); // 1

    // Test 35-37: Job scheduling
    job_start[0]=1; job_end[0]=3; job_profit[0]=5;
    job_start[1]=2; job_end[1]=5; job_profit[1]=6;
    job_start[2]=4; job_end[2]=6; job_profit[2]=5;
    job_start[3]=6; job_end[3]=8; job_profit[3]=4;
    printf(" %d", max_job_profit(4)); // job0+job2+job3: 5+5+4=14? check overlaps: job0(1-3),job2(4-6): no overlap. job2(4-6),job3(6-8): end[2]=6>start[3]=6: overlap! So job0+job3=9, job1+job3: end[1]=5>start[3]=6? 5>6 false, no overlap. 6+4=10. job0+job2=10 (no overlap). job0+job2: end[0]=3>start[2]=4? 3>4=no. 5+5=10. job0+job3: end[0]=3>start[3]=6? no. 5+4=9. best single: 6. best pair: job0+job2=10 or job1+job3=10. Can we do job0+job2+job3? job2 end=6, job3 start=6: 6>6=false, no overlap! So 5+5+4=14.

    job_start[0]=1; job_end[0]=2; job_profit[0]=50;
    job_start[1]=3; job_end[1]=5; job_profit[1]=20;
    job_start[2]=6; job_end[2]=8; job_profit[2]=30;
    printf(" %d", max_job_profit(3)); // all non-overlapping: 50+20+30=100

    job_start[0]=1; job_end[0]=10; job_profit[0]=100;
    job_start[1]=2; job_end[1]=3; job_profit[1]=10;
    job_start[2]=4; job_end[2]=5; job_profit[2]=10;
    printf(" %d", max_job_profit(3)); // job0 alone=100, job1+job2=20. Best=100

    // Test 38-40: Subset XOR
    values[0]=1; values[1]=2; values[2]=3;
    printf(" %d", subset_xor(values, 3)); // sums: 1,2,3,3,4,5,6 -> XOR: 1^2=3, 3^3=0, 0^3=3, 3^4=7, 7^5=2, 2^6=4

    values[0]=1; values[1]=1;
    printf(" %d", subset_xor(values, 2)); // sums: 1,1,2 -> 1^1^2=2

    values[0]=5;
    printf(" %d", subset_xor(values, 1)); // just 5

    // Test 41-43: More TSP
    // 3 cities
    tsp_dist[0][0]=0; tsp_dist[0][1]=1; tsp_dist[0][2]=2;
    tsp_dist[1][0]=1; tsp_dist[1][1]=0; tsp_dist[1][2]=3;
    tsp_dist[2][0]=2; tsp_dist[2][1]=3; tsp_dist[2][2]=0;
    printf(" %d", tsp_solve(3)); // 0->1->2->0: 1+3+2=6 or 0->2->1->0: 2+3+1=6

    // 5 cities
    tsp_dist[0][0]=0;  tsp_dist[0][1]=3;  tsp_dist[0][2]=1;  tsp_dist[0][3]=5;  tsp_dist[0][4]=8;
    tsp_dist[1][0]=3;  tsp_dist[1][1]=0;  tsp_dist[1][2]=6;  tsp_dist[1][3]=7;  tsp_dist[1][4]=9;
    tsp_dist[2][0]=1;  tsp_dist[2][1]=6;  tsp_dist[2][2]=0;  tsp_dist[2][3]=4;  tsp_dist[2][4]=2;
    tsp_dist[3][0]=5;  tsp_dist[3][1]=7;  tsp_dist[3][2]=4;  tsp_dist[3][3]=0;  tsp_dist[3][4]=3;
    tsp_dist[4][0]=8;  tsp_dist[4][1]=9;  tsp_dist[4][2]=2;  tsp_dist[4][3]=3;  tsp_dist[4][4]=0;
    printf(" %d", tsp_solve(5));

    // Test popcount of various
    printf(" %d", popcount(1023)); // 10 bits

    // Test 44-46: More bit ops
    printf(" %d", trailing_zeros(16));  // 4
    printf(" %d", highest_bit_pos(31)); // 4
    printf(" %d", popcount(42));        // 3 (101010)

    // Test 47: Complete graph K5 hamiltonian paths
    for (i = 0; i < 6; i = i + 1) for (j = 0; j < 6; j = j + 1) adj2[i][j] = 0;
    for (i = 0; i < 5; i = i + 1) for (j = 0; j < 5; j = j + 1) if (i != j) adj2[i][j] = 1;
    printf(" %d", count_hamiltonian_paths(adj2, 5)); // 5! = 120

    // Test 48-50: More partition and assignment
    values[0]=2; values[1]=2; values[2]=2; values[3]=2;
    printf(" %d", can_equal_partition(values, 4)); // 1 (4=4)

    assign_cost[0][0]=5; assign_cost[0][1]=9; assign_cost[0][2]=3;
    assign_cost[1][0]=8; assign_cost[1][1]=2; assign_cost[1][2]=6;
    assign_cost[2][0]=7; assign_cost[2][1]=4; assign_cost[2][2]=1;
    printf(" %d", assignment_problem(3)); // min: w0->j2(3)+w1->j1(2)+w2->j0(7)=12? or w0->j0(5)+w1->j1(2)+w2->j2(1)=8

    printf(" %d", popcount(31)); // 5

    printf("\n");
    return 0;
}
