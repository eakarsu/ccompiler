int printf(const char *fmt, ...);

// Knight's tour on a 5x5 board using Warnsdorff's heuristic
// The knight visits every square exactly once

int board[25]; // 5x5
int dx[8];
int dy[8];
int solution_found;

void init_moves(void) {
    dx[0] = 2;  dy[0] = 1;
    dx[1] = 1;  dy[1] = 2;
    dx[2] = -1; dy[2] = 2;
    dx[3] = -2; dy[3] = 1;
    dx[4] = -2; dy[4] = -1;
    dx[5] = -1; dy[5] = -2;
    dx[6] = 1;  dy[6] = -2;
    dx[7] = 2;  dy[7] = -1;
}

int is_valid(int x, int y) {
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return 0;
    if (board[y * 5 + x] != 0) return 0;
    return 1;
}

int count_moves(int x, int y) {
    int count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_valid(nx, ny)) count++;
    }
    return count;
}

// Get the move with minimum onward moves (Warnsdorff)
int best_move(int x, int y) {
    int best = -1;
    int best_count = 9;
    int i;
    for (i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_valid(nx, ny)) {
            int c = count_moves(nx, ny);
            if (c < best_count) {
                best_count = c;
                best = i;
            }
        }
    }
    return best;
}

int solve_tour(int x, int y, int move_num) {
    board[y * 5 + x] = move_num;
    if (move_num == 25) {
        solution_found = 1;
        return 1;
    }

    // Try Warnsdorff's heuristic first
    // We'll try all moves sorted by accessibility
    int tried[8];
    int ti;
    for (ti = 0; ti < 8; ti++) tried[ti] = 0;

    int attempt;
    for (attempt = 0; attempt < 8; attempt++) {
        int best_idx = -1;
        int best_cnt = 9;
        int i;
        for (i = 0; i < 8; i++) {
            if (tried[i]) continue;
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (is_valid(nx, ny)) {
                int c = count_moves(nx, ny);
                if (c < best_cnt) {
                    best_cnt = c;
                    best_idx = i;
                }
            }
        }
        if (best_idx == -1) break;
        tried[best_idx] = 1;
        int nx = x + dx[best_idx];
        int ny = y + dy[best_idx];
        if (solve_tour(nx, ny, move_num + 1)) return 1;
    }

    board[y * 5 + x] = 0;
    return 0;
}

void clear_board(void) {
    int i;
    for (i = 0; i < 25; i++) board[i] = 0;
    solution_found = 0;
}

int verify_tour(void) {
    // Check all squares visited (1..25)
    int seen[26];
    int i;
    for (i = 0; i <= 25; i++) seen[i] = 0;
    for (i = 0; i < 25; i++) {
        int v = board[i];
        if (v < 1 || v > 25) return 0;
        if (seen[v]) return 0;
        seen[v] = 1;
    }
    return 1;
}

int verify_knight_moves(void) {
    // For each move n -> n+1, verify it's a valid knight move
    int pos[26]; // pos[move_num] = index in board
    int i;
    for (i = 0; i < 25; i++) {
        pos[board[i]] = i;
    }
    int n;
    for (n = 1; n < 25; n++) {
        int from = pos[n];
        int to = pos[n + 1];
        int fx = from % 5;
        int fy = from / 5;
        int tx = to % 5;
        int ty = to / 5;
        int ddx = fx - tx;
        int ddy = fy - ty;
        if (ddx < 0) ddx = -ddx;
        if (ddy < 0) ddy = -ddy;
        if (!((ddx == 1 && ddy == 2) || (ddx == 2 && ddy == 1))) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    init_moves();

    // Test 1: Knight move validity
    printf("Valid (0,0)->(%d,%d): %d\n", dx[0], dy[0], is_valid(dx[0], dy[0]));
    // EXPECT: Valid (0,0)->(2,1): 1

    // Test 2: Invalid position
    printf("Valid (-1,0): %d\n", is_valid(-1, 0));
    // EXPECT: Valid (-1,0): 0

    // Test 3: Count moves from corner
    clear_board();
    int c = count_moves(0, 0);
    printf("Moves from (0,0): %d\n", c);
    // EXPECT: Moves from (0,0): 2

    // Test 4: Count moves from center
    c = count_moves(2, 2);
    printf("Moves from (2,2): %d\n", c);
    // EXPECT: Moves from (2,2): 8

    // Test 5: Solve tour from (0,0)
    clear_board();
    int result = solve_tour(0, 0, 1);
    printf("Tour found from (0,0): %d\n", result);
    // EXPECT: Tour found from (0,0): 1

    // Test 6: Verify all squares visited
    int valid = verify_tour();
    printf("All squares visited: %d\n", valid);
    // EXPECT: All squares visited: 1

    // Test 7: Verify all moves are knight moves
    valid = verify_knight_moves();
    printf("All knight moves valid: %d\n", valid);
    // EXPECT: All knight moves valid: 1

    // Test 8: Starting square has move 1
    printf("Start square value: %d\n", board[0]);
    // EXPECT: Start square value: 1

    // Test 9: Solve from center (2,2)
    clear_board();
    result = solve_tour(2, 2, 1);
    printf("Tour from (2,2): %d\n", result);
    // EXPECT: Tour from (2,2): 1
    valid = verify_tour();
    printf("Center tour valid: %d\n", valid);
    // EXPECT: Center tour valid: 1

    // Test 10: Solve from (1,0)
    clear_board();
    result = solve_tour(1, 0, 1);
    printf("Tour from (1,0): %d\n", result);
    // EXPECT: Tour from (1,0): 0
    // Tour not found from (1,0) with Warnsdorff heuristic on 5x5
    printf("(1,0) tour attempted: %d\n", solution_found);
    // EXPECT: (1,0) tour attempted: 0

    printf("Knight tour tests passed\n");
    // EXPECT: Knight tour tests passed
    return 0;
}
