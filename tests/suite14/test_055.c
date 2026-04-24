int printf(const char *fmt, ...);

// Knight's tour using Warnsdorff's heuristic on a 5x5 board

int board[5][5];

int dx[8];
int dy[8];

void init_moves() {
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
    return x >= 0 && x < 5 && y >= 0 && y < 5 && board[x][y] == 0;
}

// Count how many onward moves from (x,y)
int degree(int x, int y) {
    int count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_valid(nx, ny)) count++;
    }
    return count;
}

// Select next move using Warnsdorff: pick the move with minimum onward degree
// Returns 1 if a move was found, 0 otherwise
int next_move(int *cx, int *cy) {
    int min_deg = 9;
    int best_i = -1;
    int i;
    for (i = 0; i < 8; i++) {
        int nx = *cx + dx[i];
        int ny = *cy + dy[i];
        if (is_valid(nx, ny)) {
            int d = degree(nx, ny);
            if (d < min_deg) {
                min_deg = d;
                best_i = i;
            }
        }
    }
    if (best_i == -1) return 0;
    *cx = *cx + dx[best_i];
    *cy = *cy + dy[best_i];
    return 1;
}

int knights_tour(int start_x, int start_y) {
    int r, c;
    for (r = 0; r < 5; r++)
        for (c = 0; c < 5; c++)
            board[r][c] = 0;

    int cx = start_x;
    int cy = start_y;
    board[cx][cy] = 1;
    int move_num;
    for (move_num = 2; move_num <= 25; move_num++) {
        if (!next_move(&cx, &cy)) return move_num - 1;
        board[cx][cy] = move_num;
    }
    return 25;
}

int count_visited() {
    int count = 0;
    int r, c;
    for (r = 0; r < 5; r++)
        for (c = 0; c < 5; c++)
            if (board[r][c] > 0) count++;
    return count;
}

// Check that all move numbers from 1..n are present
int verify_tour(int n) {
    int found[26];
    int i;
    for (i = 0; i < 26; i++) found[i] = 0;
    int r, c;
    for (r = 0; r < 5; r++) {
        for (c = 0; c < 5; c++) {
            int v = board[r][c];
            if (v >= 1 && v <= 25) found[v] = 1;
        }
    }
    for (i = 1; i <= n; i++) {
        if (!found[i]) return 0;
    }
    return 1;
}

// Check consecutive moves are valid knight moves
int verify_knight_moves(int n) {
    // Find position of each move number
    int px[26];
    int py[26];
    int i;
    for (i = 0; i < 26; i++) { px[i] = -1; py[i] = -1; }
    int r, c;
    for (r = 0; r < 5; r++) {
        for (c = 0; c < 5; c++) {
            int v = board[r][c];
            if (v >= 1 && v <= 25) {
                px[v] = r;
                py[v] = c;
            }
        }
    }
    for (i = 1; i < n; i++) {
        int dxx = px[i+1] - px[i];
        int dyy = py[i+1] - py[i];
        if (dxx < 0) dxx = -dxx;
        if (dyy < 0) dyy = -dyy;
        // Valid knight move: (1,2) or (2,1)
        if (!((dxx == 1 && dyy == 2) || (dxx == 2 && dyy == 1)))
            return 0;
    }
    return 1;
}

int main() {
    init_moves();

    // Test from corner (0,0)
    int result1 = knights_tour(0, 0);
    printf("tour_00: %d\n", result1);
    // EXPECT: tour_00: 25
    printf("visited_00: %d\n", count_visited());
    // EXPECT: visited_00: 25
    printf("verify_00: %d\n", verify_tour(result1));
    // EXPECT: verify_00: 1
    printf("moves_ok_00: %d\n", verify_knight_moves(result1));
    // EXPECT: moves_ok_00: 1
    printf("start_00: %d\n", board[0][0]);
    // EXPECT: start_00: 1

    // Test from center (2,2)
    int result2 = knights_tour(2, 2);
    printf("tour_22: %d\n", result2);
    // EXPECT: tour_22: 25
    printf("visited_22: %d\n", count_visited());
    // EXPECT: visited_22: 25
    printf("verify_22: %d\n", verify_tour(result2));
    // EXPECT: verify_22: 1
    printf("moves_ok_22: %d\n", verify_knight_moves(result2));
    // EXPECT: moves_ok_22: 1
    printf("start_22: %d\n", board[2][2]);
    // EXPECT: start_22: 1

    // Test from (1,0)
    int result3 = knights_tour(1, 0);
    printf("tour_10: %d\n", result3);
    // EXPECT: tour_10: 22
    printf("verify_10: %d\n", verify_tour(result3));
    // EXPECT: verify_10: 1
    printf("moves_ok_10: %d\n", verify_knight_moves(result3));
    // EXPECT: moves_ok_10: 1

    // Test degree from corner of empty board
    int r, c;
    for (r = 0; r < 5; r++)
        for (c = 0; c < 5; c++)
            board[r][c] = 0;
    printf("deg_corner: %d\n", degree(0, 0));
    // EXPECT: deg_corner: 2
    printf("deg_center: %d\n", degree(2, 2));
    // EXPECT: deg_center: 8
    printf("deg_edge: %d\n", degree(0, 2));
    // EXPECT: deg_edge: 4

    printf("done\n");
    // EXPECT: done
    return 0;
}
