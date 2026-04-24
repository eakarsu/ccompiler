int printf(const char *fmt, ...);

// Minimax with Alpha-Beta Pruning for a simple game tree
// The game tree is stored as an array (complete binary tree)
// Leaf values are at depth 3 (8 leaves)

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

// Static game tree with 15 nodes (indices 1..14)
// Leaves are at indices 7..14
int tree[16];

// Minimax without pruning
int minimax(int node, int depth, int is_max) {
    if (depth == 0) {
        return tree[node];
    }
    int left_child = node * 2;
    int right_child = node * 2 + 1;
    if (is_max) {
        int left_val = minimax(left_child, depth - 1, 0);
        int right_val = minimax(right_child, depth - 1, 0);
        int result = max(left_val, right_val);
        return result;
    } else {
        int left_val = minimax(left_child, depth - 1, 1);
        int right_val = minimax(right_child, depth - 1, 1);
        int result = min(left_val, right_val);
        return result;
    }
}

// Alpha-beta pruning version
int nodes_visited;

int alphabeta(int node, int depth, int alpha, int beta, int is_max) {
    nodes_visited = nodes_visited + 1;
    if (depth == 0) {
        return tree[node];
    }
    int left_child = node * 2;
    int right_child = node * 2 + 1;
    if (is_max) {
        int val = -9999;
        int left_val = alphabeta(left_child, depth - 1, alpha, beta, 0);
        if (left_val > val) val = left_val;
        if (val > alpha) alpha = val;
        if (beta <= alpha) {
            return val;
        }
        int right_val = alphabeta(right_child, depth - 1, alpha, beta, 0);
        if (right_val > val) val = right_val;
        return val;
    } else {
        int val = 9999;
        int left_val = alphabeta(left_child, depth - 1, alpha, beta, 1);
        if (left_val < val) val = left_val;
        if (val < beta) beta = val;
        if (beta <= alpha) {
            return val;
        }
        int right_val = alphabeta(right_child, depth - 1, alpha, beta, 1);
        if (right_val < val) val = right_val;
        return val;
    }
}

int main() {
    // Set up leaf nodes (indices 7..14)
    tree[7] = 3;
    tree[8] = 5;
    tree[9] = 6;
    tree[10] = 9;
    tree[11] = 1;
    tree[12] = 2;
    tree[13] = 0;
    tree[14] = -1;

    printf("Leaf values: %d %d %d %d %d %d %d %d\n", tree[7], tree[8], tree[9], tree[10], tree[11], tree[12], tree[13], tree[14]);
    // EXPECT: Leaf values: 3 5 6 9 1 2 0 -1

    // Run minimax from root (node 1), depth 3, maximizing
    int result = minimax(1, 3, 1);
    printf("Minimax result: %d\n", result);
    // EXPECT: Minimax result: 6

    // Run alpha-beta from root
    nodes_visited = 0;
    int ab_result = alphabeta(1, 3, -9999, 9999, 1);
    printf("Alpha-beta result: %d\n", ab_result);
    // EXPECT: Alpha-beta result: 6
    printf("Alpha-beta nodes visited: %d\n", nodes_visited);
    // EXPECT: Alpha-beta nodes visited: 11

    // Test with different leaf values
    tree[7] = 10;
    tree[8] = 2;
    tree[9] = 7;
    tree[10] = 1;
    tree[11] = 8;
    tree[12] = 9;
    tree[13] = 3;
    tree[14] = 4;

    result = minimax(1, 3, 1);
    printf("Minimax result 2: %d\n", result);
    // EXPECT: Minimax result 2: 7

    nodes_visited = 0;
    ab_result = alphabeta(1, 3, -9999, 9999, 1);
    printf("Alpha-beta result 2: %d\n", ab_result);
    // EXPECT: Alpha-beta result 2: 7
    printf("Alpha-beta nodes 2: %d\n", nodes_visited);
    // EXPECT: Alpha-beta nodes 2: 15

    // Test minimizing player at root
    result = minimax(1, 3, 0);
    printf("Minimax min-root: %d\n", result);
    // EXPECT: Minimax min-root: 2

    nodes_visited = 0;
    ab_result = alphabeta(1, 3, -9999, 9999, 0);
    printf("Alpha-beta min-root: %d\n", ab_result);
    // EXPECT: Alpha-beta min-root: 2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
