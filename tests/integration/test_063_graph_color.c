int printf(const char *fmt, ...);
// EXPECT: 1: 1 6 4 4 1\n2: 3 3 1\n3: 4 2 2 1\n4: 5 3 3 1 3 1\n5: 4 4 2 2 1 0 1 1 1 1 \n6: 0 1 1 1\n7: 4 2 2 1 0 1 0 1 0 \n8: 9 3 3 4 1 4 1\n9: 6 2 2 1\n10: 8 4 3 3 1 0 1 2 1 2 \n11: 1 1 1\n12: 2 2 1\n13: 1 1 1\n14: 1 10 5 5 1\n15: 2 3 4 3 2 2 \n16: 2 3 3 \n17: 3 3 1 3\n18: 6 3 3 1\n19: 9 4 1 3 1\n20: 5 3 3 1\n21: 1 0 1 1
// ============================================================================
// Graph Coloring - Greedy Coloring, Chromatic Number Approximation
// ============================================================================
// This file implements graph coloring algorithms on small graphs.
// Graphs are represented using adjacency matrices.
// We implement:
// 1. Greedy coloring with natural ordering
// 2. Greedy coloring with largest-degree-first ordering
// 3. Chromatic number computation via brute force (small graphs)
// 4. Coloring verification
// ============================================================================

#define MAX_NODES 20
#define MAX_COLORS 20

// Adjacency matrix: adj[i][j] = 1 if edge between i and j
int adj[MAX_NODES][MAX_NODES];

// Color assigned to each node
int color[MAX_NODES];

// Degree of each node
int degree[MAX_NODES];

// Ordering of nodes (for ordered greedy)
int order[MAX_NODES];

// ============================================================================
// Initialize adjacency matrix to all zeros
// ============================================================================
void init_graph(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            adj[i][j] = 0;
        }
        color[i] = -1;
        degree[i] = 0;
    }
}

// ============================================================================
// Add an undirected edge between u and v
// ============================================================================
void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

// ============================================================================
// Compute degree of each node
// ============================================================================
void compute_degrees(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        degree[i] = 0;
        for (j = 0; j < n; j = j + 1) {
            if (adj[i][j] == 1) {
                degree[i] = degree[i] + 1;
            }
        }
    }
}

// ============================================================================
// Count the number of edges in the graph
// ============================================================================
int count_edges(int n) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (adj[i][j] == 1) {
                count = count + 1;
            }
        }
    }
    return count;
}

// ============================================================================
// Check if a graph is complete (all pairs connected)
// ============================================================================
int is_complete(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (adj[i][j] == 0) return 0;
        }
    }
    return 1;
}

// ============================================================================
// Greedy coloring with natural ordering (0, 1, 2, ...)
// Returns the number of colors used
// ============================================================================
int greedy_color_natural(int n) {
    int i;
    int j;
    int c;
    int used[MAX_COLORS];
    int num_colors;

    // Reset colors
    for (i = 0; i < n; i = i + 1) {
        color[i] = -1;
    }

    num_colors = 0;

    for (i = 0; i < n; i = i + 1) {
        // Mark colors used by neighbors
        for (c = 0; c < MAX_COLORS; c = c + 1) {
            used[c] = 0;
        }
        for (j = 0; j < n; j = j + 1) {
            if (adj[i][j] == 1 && color[j] != -1) {
                used[color[j]] = 1;
            }
        }
        // Find smallest available color
        for (c = 0; c < MAX_COLORS; c = c + 1) {
            if (used[c] == 0) {
                color[i] = c;
                if (c + 1 > num_colors) {
                    num_colors = c + 1;
                }
                c = MAX_COLORS; // break
            }
        }
    }

    return num_colors;
}

// ============================================================================
// Greedy coloring with largest-degree-first ordering
// Returns the number of colors used
// ============================================================================
int greedy_color_ldf(int n) {
    int i;
    int j;
    int c;
    int used[MAX_COLORS];
    int num_colors;
    int temp;
    int node;

    // Compute degrees
    compute_degrees(n);

    // Initialize order
    for (i = 0; i < n; i = i + 1) {
        order[i] = i;
    }

    // Sort by degree (descending) using bubble sort
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (degree[order[j]] < degree[order[j + 1]]) {
                temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    // Reset colors
    for (i = 0; i < n; i = i + 1) {
        color[i] = -1;
    }

    num_colors = 0;

    for (i = 0; i < n; i = i + 1) {
        node = order[i];
        // Mark colors used by neighbors
        for (c = 0; c < MAX_COLORS; c = c + 1) {
            used[c] = 0;
        }
        for (j = 0; j < n; j = j + 1) {
            if (adj[node][j] == 1 && color[j] != -1) {
                used[color[j]] = 1;
            }
        }
        // Find smallest available color
        for (c = 0; c < MAX_COLORS; c = c + 1) {
            if (used[c] == 0) {
                color[node] = c;
                if (c + 1 > num_colors) {
                    num_colors = c + 1;
                }
                c = MAX_COLORS; // break
            }
        }
    }

    return num_colors;
}

// ============================================================================
// Verify that a coloring is valid (no two adjacent nodes share color)
// Returns 1 if valid, 0 otherwise
// ============================================================================
int verify_coloring(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        if (color[i] == -1) return 0; // uncolored
        for (j = i + 1; j < n; j = j + 1) {
            if (adj[i][j] == 1 && color[i] == color[j]) {
                return 0;
            }
        }
    }
    return 1;
}

// ============================================================================
// Check if a graph can be colored with k colors using backtracking
// This is used for small graphs to find the exact chromatic number
// ============================================================================
int can_color_bt(int n, int k, int node) {
    int c;
    int j;
    int valid;

    if (node == n) return 1; // all nodes colored

    for (c = 0; c < k; c = c + 1) {
        // Check if color c is valid for node
        valid = 1;
        for (j = 0; j < node; j = j + 1) {
            if (adj[node][j] == 1 && color[j] == c) {
                valid = 0;
                j = node; // break
            }
        }
        if (valid) {
            color[node] = c;
            if (can_color_bt(n, k, node + 1)) {
                return 1;
            }
            color[node] = -1;
        }
    }
    return 0;
}

// ============================================================================
// Find the exact chromatic number (brute force, small graphs only)
// ============================================================================
int chromatic_number(int n) {
    int k;
    int i;

    for (k = 1; k <= n; k = k + 1) {
        // Reset colors
        for (i = 0; i < n; i = i + 1) {
            color[i] = -1;
        }
        if (can_color_bt(n, k, 0)) {
            return k;
        }
    }
    return n; // worst case
}

// ============================================================================
// Count nodes with a specific color
// ============================================================================
int count_color(int n, int c) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (color[i] == c) count = count + 1;
    }
    return count;
}

// ============================================================================
// Find the maximum degree of any node
// ============================================================================
int max_degree(int n) {
    int i;
    int max;
    compute_degrees(n);
    max = 0;
    for (i = 0; i < n; i = i + 1) {
        if (degree[i] > max) {
            max = degree[i];
        }
    }
    return max;
}

// ============================================================================
// Test Case 1: Complete graph K4
// Chromatic number of K4 = 4
// ============================================================================
void test_case_1(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 1);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 2); add_edge(1, 3);
    add_edge(2, 3);

    printf("%d", is_complete(n));
    printf("%c", ' ');
    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 2: Complete graph K3
// ============================================================================
void test_case_2(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 2);
    printf("%c", ':');
    printf("%c", ' ');

    n = 3;
    init_graph(n);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 2);

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 3: Bipartite graph (even cycle C4)
// Chromatic number = 2
// ============================================================================
void test_case_3(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 3);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    init_graph(n);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 4: Odd cycle C5
// Chromatic number = 3
// ============================================================================
void test_case_4(void) {
    int n;
    int chi;
    int greedy;
    int ldf;
    int valid;

    printf("%d", 4);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 0);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');

    ldf = greedy_color_ldf(n);
    valid = verify_coloring(n);
    printf("%d", ldf);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 5: Star graph S4 (center connected to 4 leaves)
// Chromatic number = 2
// ============================================================================
void test_case_5(void) {
    int n;
    int chi;
    int greedy;
    int valid;
    int i;

    printf("%d", 5);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    add_edge(0, 4);

    printf("%d", count_edges(n));
    printf("%c", ' ');
    printf("%d", max_degree(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');

    // Print colors assigned
    for (i = 0; i < n; i = i + 1) {
        printf("%d", color[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 6: Empty graph (no edges)
// Chromatic number = 1
// ============================================================================
void test_case_6(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 6);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 7: Path graph P5 (0-1-2-3-4)
// Chromatic number = 2
// ============================================================================
void test_case_7(void) {
    int n;
    int chi;
    int greedy;
    int valid;
    int i;

    printf("%d", 7);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');

    // Print colors
    for (i = 0; i < n; i = i + 1) {
        printf("%d", color[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 8: Petersen-like graph (partial)
// 6 nodes with specific edges
// ============================================================================
void test_case_8(void) {
    int n;
    int chi;
    int greedy;
    int ldf;
    int valid;

    printf("%d", 8);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 3); add_edge(1, 4);
    add_edge(2, 4); add_edge(2, 5);
    add_edge(3, 5);
    add_edge(4, 5);

    printf("%d", count_edges(n));
    printf("%c", ' ');
    printf("%d", max_degree(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');

    ldf = greedy_color_ldf(n);
    valid = verify_coloring(n);
    printf("%d", ldf);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 9: Complete bipartite K(2,3)
// Chromatic number = 2
// ============================================================================
void test_case_9(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 9);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    // Set A = {0, 1}, Set B = {2, 3, 4}
    add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);
    add_edge(1, 2); add_edge(1, 3); add_edge(1, 4);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 10: Wheel graph W5 (C4 + center)
// Chromatic number = 4 (odd spokes from center makes it need 4)
// Actually W5 has center + C4. C4 is bipartite, so chi=3 if center can reuse.
// But wait: center is connected to all 4 cycle nodes, needs different color.
// C4 needs 2 colors, center needs a 3rd. So chi=3.
// ============================================================================
void test_case_10(void) {
    int n;
    int chi;
    int greedy;
    int ldf;
    int valid;
    int i;

    printf("%d", 10);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    // Cycle: 1-2-3-4-1
    add_edge(1, 2); add_edge(2, 3); add_edge(3, 4); add_edge(4, 1);
    // Center (0) connected to all
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);

    printf("%d", count_edges(n));
    printf("%c", ' ');
    printf("%d", max_degree(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');

    // Print color assignments
    for (i = 0; i < n; i = i + 1) {
        printf("%d", color[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 11: Single node graph
// ============================================================================
void test_case_11(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 11);
    printf("%c", ':');
    printf("%c", ' ');

    n = 1;
    init_graph(n);

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 12: Two nodes with edge
// ============================================================================
void test_case_12(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 12);
    printf("%c", ':');
    printf("%c", ' ');

    n = 2;
    init_graph(n);
    add_edge(0, 1);

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 13: Two nodes without edge
// ============================================================================
void test_case_13(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 13);
    printf("%c", ':');
    printf("%c", ' ');

    n = 2;
    init_graph(n);

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 14: K5 (complete on 5 nodes)
// ============================================================================
void test_case_14(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 14);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);
    add_edge(1, 2); add_edge(1, 3); add_edge(1, 4);
    add_edge(2, 3); add_edge(2, 4);
    add_edge(3, 4);

    printf("%d", is_complete(n));
    printf("%c", ' ');
    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 15: Degree sequence printing
// ============================================================================
void test_case_15(void) {
    int n;
    int i;

    printf("%d", 15);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2);
    add_edge(1, 2); add_edge(1, 3);
    add_edge(2, 3); add_edge(2, 4);
    add_edge(3, 5);
    add_edge(4, 5);

    compute_degrees(n);
    for (i = 0; i < n; i = i + 1) {
        printf("%d", degree[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 16: Color count per color
// ============================================================================
void test_case_16(void) {
    int n;
    int num_colors;
    int i;

    printf("%d", 16);
    printf("%c", ':');
    printf("%c", ' ');

    // C6 cycle
    n = 6;
    init_graph(n);
    add_edge(0, 1); add_edge(1, 2); add_edge(2, 3);
    add_edge(3, 4); add_edge(4, 5); add_edge(5, 0);

    num_colors = greedy_color_natural(n);
    printf("%d", num_colors);
    printf("%c", ' ');

    for (i = 0; i < num_colors; i = i + 1) {
        printf("%d", count_color(n, i));
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 17: Graph with isolated nodes mixed with connected
// ============================================================================
void test_case_17(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 17);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    init_graph(n);
    // Nodes 0,1,2 form K3, nodes 3,4,5 are isolated
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 2);

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", ' ');
    printf("%d", count_edges(n));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 18: Multiple component graph
// Two triangles: {0,1,2} and {3,4,5}
// ============================================================================
void test_case_18(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 18);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    init_graph(n);
    add_edge(0, 1); add_edge(1, 2); add_edge(0, 2);
    add_edge(3, 4); add_edge(4, 5); add_edge(3, 5);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 19: LDF vs natural ordering comparison on specific graph
// ============================================================================
void test_case_19(void) {
    int n;
    int natural;
    int ldf;
    int valid_n;
    int valid_l;

    printf("%d", 19);
    printf("%c", ':');
    printf("%c", ' ');

    n = 7;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 4); add_edge(2, 5); add_edge(3, 6);
    add_edge(4, 5); add_edge(5, 6); add_edge(4, 6);

    printf("%d", count_edges(n));
    printf("%c", ' ');

    natural = greedy_color_natural(n);
    valid_n = verify_coloring(n);
    printf("%d", natural);
    printf("%c", ' ');
    printf("%d", valid_n);
    printf("%c", ' ');

    ldf = greedy_color_ldf(n);
    valid_l = verify_coloring(n);
    printf("%d", ldf);
    printf("%c", ' ');
    printf("%d", valid_l);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 20: Diamond graph (K4 minus one edge)
// ============================================================================
void test_case_20(void) {
    int n;
    int chi;
    int greedy;
    int valid;

    printf("%d", 20);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    init_graph(n);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 2);
    add_edge(2, 3);
    // Missing: 1-3

    printf("%d", count_edges(n));
    printf("%c", ' ');

    chi = chromatic_number(n);
    printf("%d", chi);
    printf("%c", ' ');

    greedy = greedy_color_natural(n);
    valid = verify_coloring(n);
    printf("%d", greedy);
    printf("%c", ' ');
    printf("%d", valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 21: Verify completeness check on various graphs
// ============================================================================
void test_case_21(void) {
    int n;

    printf("%d", 21);
    printf("%c", ':');
    printf("%c", ' ');

    // K3
    n = 3;
    init_graph(n);
    add_edge(0, 1); add_edge(1, 2); add_edge(0, 2);
    printf("%d", is_complete(n));
    printf("%c", ' ');

    // Not complete (missing one edge)
    n = 3;
    init_graph(n);
    add_edge(0, 1); add_edge(1, 2);
    printf("%d", is_complete(n));
    printf("%c", ' ');

    // Single node
    n = 1;
    init_graph(n);
    printf("%d", is_complete(n));
    printf("%c", ' ');

    // K2
    n = 2;
    init_graph(n);
    add_edge(0, 1);
    printf("%d", is_complete(n));
    printf("%c", '\n');
}

// ============================================================================
// Main function
// ============================================================================

// 2: 3 3 1
// 3: 4 2 2 1
// 4: 5 3 3 1 3 1
// 5: 4 4 2 2 1 0 1 1 1 1
// 6: 0 1 1 1
// 7: 4 2 2 1 0 1 0 1 0
// 8: 9 3 3 3 1 3 1
// 9: 6 2 2 1
// 10: 8 4 3 3 1 0 1 2 1 2
// 11: 1 1 1
// 12: 2 2 1
// 13: 1 1 1
// 14: 1 10 5 5 1
// 15: 2 3 3 3 2 2
// 16: 2 3 3
// 17: 3 3 1 3
// 18: 6 3 3 1
// 19: 9 3 1 3 1
// 20: 5 3 3 1
// 21: 1 0 1 1

int main(void) {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    test_case_12();
    test_case_13();
    test_case_14();
    test_case_15();
    test_case_16();
    test_case_17();
    test_case_18();
    test_case_19();
    test_case_20();
    test_case_21();
    return 0;
}
