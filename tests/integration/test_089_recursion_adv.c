int printf(const char *fmt, ...);
// EXPECT: 1 7 31 1 3 7 6 6 0 2 8 0 7 1 7 0 0 0 3 7 28 4 7 1 1 4 4 1 0 3 2 1024 81 2 0 -1 24 9 8 16 6 24 2 10 4 1 0 15 63 125

// Advanced recursion: Tower of Hanoi, flood fill, recursive maze solving,
// recursive tree operations

// Tower of Hanoi - count moves and track moves
int hanoi_count;

void hanoi(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    hanoi_count = hanoi_count + 1;
    hanoi(n - 1, aux, to, from);
}

int solve_hanoi(int n) {
    hanoi_count = 0;
    hanoi(n, 1, 3, 2);
    return hanoi_count;
}

// Track first few moves of hanoi
int hanoi_moves_from[20];
int hanoi_moves_to[20];
int hanoi_move_idx;

void hanoi_track(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi_track(n - 1, from, aux, to);
    if (hanoi_move_idx < 20) {
        hanoi_moves_from[hanoi_move_idx] = from;
        hanoi_moves_to[hanoi_move_idx] = to;
        hanoi_move_idx = hanoi_move_idx + 1;
    }
    hanoi_track(n - 1, aux, to, from);
}

// Flood fill on a small grid
int grid[10][10];
int grid_rows;
int grid_cols;

void flood_fill(int r, int c, int old_color, int new_color) {
    if (r < 0 || r >= grid_rows || c < 0 || c >= grid_cols) return;
    if (grid[r][c] != old_color) return;
    grid[r][c] = new_color;
    flood_fill(r - 1, c, old_color, new_color);
    flood_fill(r + 1, c, old_color, new_color);
    flood_fill(r, c - 1, old_color, new_color);
    flood_fill(r, c + 1, old_color, new_color);
}

// Count cells of a given color
int count_color(int color) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    for (i = 0; i < grid_rows; i = i + 1) {
        for (j = 0; j < grid_cols; j = j + 1) {
            if (grid[i][j] == color) cnt = cnt + 1;
        }
    }
    return cnt;
}

// Maze: 0 = open, 1 = wall, find path from (0,0) to (rows-1,cols-1)
int maze[10][10];
int maze_visited[10][10];
int maze_rows;
int maze_cols;
int maze_path[100];
int maze_path_len;

int solve_maze(int r, int c) {
    if (r < 0 || r >= maze_rows || c < 0 || c >= maze_cols) return 0;
    if (maze[r][c] == 1) return 0;
    if (maze_visited[r][c]) return 0;
    maze_visited[r][c] = 1;
    maze_path[maze_path_len] = r * maze_cols + c;
    maze_path_len = maze_path_len + 1;
    if (r == maze_rows - 1 && c == maze_cols - 1) return 1;
    if (solve_maze(r + 1, c)) return 1;
    if (solve_maze(r, c + 1)) return 1;
    if (solve_maze(r - 1, c)) return 1;
    if (solve_maze(r, c - 1)) return 1;
    maze_path_len = maze_path_len - 1;
    return 0;
}

void init_maze_visited(void) {
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            maze_visited[i][j] = 0;
        }
    }
    maze_path_len = 0;
}

// Recursive tree using arrays
int tree_val[50];
int tree_left[50];
int tree_right[50];
int tree_count;

void tree_init(void) {
    int i;
    tree_count = 0;
    for (i = 0; i < 50; i = i + 1) {
        tree_left[i] = -1;
        tree_right[i] = -1;
    }
}

int tree_new(int v) {
    int idx;
    idx = tree_count;
    tree_val[idx] = v;
    tree_left[idx] = -1;
    tree_right[idx] = -1;
    tree_count = tree_count + 1;
    return idx;
}

// Tree height
int tree_height(int root) {
    int lh;
    int rh;
    if (root == -1) return 0;
    lh = tree_height(tree_left[root]);
    rh = tree_height(tree_right[root]);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// Tree size
int tree_size(int root) {
    if (root == -1) return 0;
    return 1 + tree_size(tree_left[root]) + tree_size(tree_right[root]);
}

// Tree sum
int tree_sum(int root) {
    if (root == -1) return 0;
    return tree_val[root] + tree_sum(tree_left[root]) + tree_sum(tree_right[root]);
}

// Count leaves
int tree_leaves(int root) {
    if (root == -1) return 0;
    if (tree_left[root] == -1 && tree_right[root] == -1) return 1;
    return tree_leaves(tree_left[root]) + tree_leaves(tree_right[root]);
}

// Tree max value
int tree_max(int root) {
    int lm;
    int rm;
    int mx;
    if (root == -1) return -99999;
    mx = tree_val[root];
    lm = tree_max(tree_left[root]);
    rm = tree_max(tree_right[root]);
    if (lm > mx) mx = lm;
    if (rm > mx) mx = rm;
    return mx;
}

// Tree min value
int tree_min(int root) {
    int lm;
    int rm;
    int mn;
    if (root == -1) return 99999;
    mn = tree_val[root];
    lm = tree_min(tree_left[root]);
    rm = tree_min(tree_right[root]);
    if (lm < mn) mn = lm;
    if (rm < mn) mn = rm;
    return mn;
}

// Check if value exists in tree
int tree_contains(int root, int v) {
    if (root == -1) return 0;
    if (tree_val[root] == v) return 1;
    if (tree_contains(tree_left[root], v)) return 1;
    return tree_contains(tree_right[root], v);
}

// Tree mirror (swap left and right recursively)
void tree_mirror(int root) {
    int t;
    if (root == -1) return;
    t = tree_left[root];
    tree_left[root] = tree_right[root];
    tree_right[root] = t;
    tree_mirror(tree_left[root]);
    tree_mirror(tree_right[root]);
}

// Tree diameter (longest path between any two nodes)
int tree_diameter_val;

int tree_diameter_helper(int root) {
    int lh;
    int rh;
    int d;
    if (root == -1) return 0;
    lh = tree_diameter_helper(tree_left[root]);
    rh = tree_diameter_helper(tree_right[root]);
    d = lh + rh;
    if (d > tree_diameter_val) tree_diameter_val = d;
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int tree_diameter(int root) {
    tree_diameter_val = 0;
    tree_diameter_helper(root);
    return tree_diameter_val;
}

// Count nodes at given level
int count_at_level(int root, int level) {
    if (root == -1) return 0;
    if (level == 0) return 1;
    return count_at_level(tree_left[root], level - 1) + count_at_level(tree_right[root], level - 1);
}

// Path sum: does a root-to-leaf path with given sum exist?
int has_path_sum(int root, int target) {
    if (root == -1) return 0;
    target = target - tree_val[root];
    if (tree_left[root] == -1 && tree_right[root] == -1) {
        if (target == 0) return 1;
        return 0;
    }
    if (has_path_sum(tree_left[root], target)) return 1;
    return has_path_sum(tree_right[root], target);
}

// Check if two trees are identical
int trees_identical(int r1, int r2) {
    if (r1 == -1 && r2 == -1) return 1;
    if (r1 == -1 || r2 == -1) return 0;
    if (tree_val[r1] != tree_val[r2]) return 0;
    return trees_identical(tree_left[r1], tree_left[r2]) &&
           trees_identical(tree_right[r1], tree_right[r2]);
}

// Recursive power
int rec_power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        int half;
        half = rec_power(base, exp / 2);
        return half * half;
    }
    return base * rec_power(base, exp - 1);
}

// Recursive binary search
int rec_binary_search(int arr[], int low, int high, int target) {
    int mid;
    if (low > high) return -1;
    mid = (low + high) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return rec_binary_search(arr, mid + 1, high, target);
    return rec_binary_search(arr, low, mid - 1, target);
}

// Recursive sum of array
int rec_sum(int arr[], int n) {
    if (n == 0) return 0;
    return arr[n - 1] + rec_sum(arr, n - 1);
}

// Recursive max of array
int rec_max(int arr[], int n) {
    int sub_max;
    if (n == 1) return arr[0];
    sub_max = rec_max(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

// Generate all subsets of first n elements, count them
int subset_count;

void gen_subsets(int arr[], int n, int idx, int current[], int cur_len) {
    int i;
    subset_count = subset_count + 1;
    for (i = idx; i < n; i = i + 1) {
        current[cur_len] = arr[i];
        gen_subsets(arr, n, i + 1, current, cur_len + 1);
    }
}

// Permutation count
int perm_count;

void gen_perms(int arr[], int n, int start) {
    int i;
    int t;
    if (start == n) {
        perm_count = perm_count + 1;
        return;
    }
    for (i = start; i < n; i = i + 1) {
        t = arr[start]; arr[start] = arr[i]; arr[i] = t;
        gen_perms(arr, n, start + 1);
        t = arr[start]; arr[start] = arr[i]; arr[i] = t;
    }
}

// N-Queens count for small n (using recursion + backtracking)
int nq_cols[10];
int nq_count;

int nq_safe(int row, int col, int n) {
    int i;
    for (i = 0; i < row; i = i + 1) {
        if (nq_cols[i] == col) return 0;
        if (nq_cols[i] - i == col - row) return 0;
        if (nq_cols[i] + i == col + row) return 0;
    }
    return 1;
}

void nq_solve(int row, int n) {
    int col;
    if (row == n) {
        nq_count = nq_count + 1;
        return;
    }
    for (col = 0; col < n; col = col + 1) {
        if (nq_safe(row, col, n)) {
            nq_cols[row] = col;
            nq_solve(row + 1, n);
        }
    }
}

int n_queens(int n) {
    nq_count = 0;
    nq_solve(0, n);
    return nq_count;
}

int main(void) {
    int arr[20];
    int current[20];
    int root;
    int r1;
    int r2;
    int i;
    int j;

    // Test 1-3: Tower of Hanoi move counts
    printf("%d", solve_hanoi(1)); // 1
    printf(" %d", solve_hanoi(3)); // 7
    printf(" %d", solve_hanoi(5)); // 31

    // Test 4-6: Track Hanoi moves (3 disks)
    hanoi_move_idx = 0;
    hanoi_track(3, 1, 3, 2);
    printf(" %d", hanoi_moves_from[0]); // 1 (first move: 1->3)
    printf(" %d", hanoi_moves_to[0]);   // 3
    printf(" %d", hanoi_move_idx);      // 7

    // Test 7-10: Flood fill
    grid_rows = 4;
    grid_cols = 4;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            grid[i][j] = 1;
        }
    }
    grid[0][0] = 0; grid[0][1] = 0;
    grid[1][0] = 0; grid[1][1] = 0; grid[1][2] = 0;
    grid[2][0] = 0;
    // Count of 0s: 6
    printf(" %d", count_color(0)); // 6
    flood_fill(0, 0, 0, 2);
    printf(" %d", count_color(2)); // 6 (all 0s became 2)
    printf(" %d", count_color(0)); // 0
    printf(" %d", grid[1][2]);     // 2

    // Test 11-13: Another flood fill
    grid_rows = 3;
    grid_cols = 3;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            grid[i][j] = 5;
        }
    }
    grid[1][1] = 0; // center is different
    flood_fill(0, 0, 5, 7);
    printf(" %d", count_color(7)); // 8 (all except center)
    printf(" %d", grid[1][1]);     // 0 (unchanged)
    printf(" %d", grid[0][0]);     // 7

    // Test 14-16: Maze solving
    maze_rows = 4;
    maze_cols = 4;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            maze[i][j] = 0;
        }
    }
    maze[0][1] = 1; maze[1][1] = 1; // wall column
    maze[2][3] = 1;
    // Path: (0,0)->(1,0)->(2,0)->(2,1)->(2,2)->(3,2)->(3,3)
    init_maze_visited();
    printf(" %d", solve_maze(0, 0)); // 1 (path exists)
    printf(" %d", maze_path_len);    // 7
    printf(" %d", maze_path[0]);     // 0 (cell 0,0 = 0*4+0 = 0)

    // Test 17-18: Blocked maze
    maze_rows = 3;
    maze_cols = 3;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            maze[i][j] = 0;
        }
    }
    maze[0][1] = 1; maze[1][0] = 1; // block both paths from start
    init_maze_visited();
    printf(" %d", solve_maze(0, 0)); // 0 (no path)
    printf(" %d", maze_path_len);    // 1 (just start cell)

    // Test 19-23: Tree operations
    tree_init();
    root = tree_new(1);
    tree_left[0] = tree_new(2);
    tree_right[0] = tree_new(3);
    tree_left[1] = tree_new(4);
    tree_right[1] = tree_new(5);
    tree_left[2] = tree_new(6);
    tree_right[2] = tree_new(7);
    //        1
    //       / \
    //      2   3
    //     / \ / \
    //    4  5 6  7
    printf(" %d", tree_height(root));  // 3
    printf(" %d", tree_size(root));    // 7
    printf(" %d", tree_sum(root));     // 28
    printf(" %d", tree_leaves(root));  // 4
    printf(" %d", tree_max(root));     // 7

    // Test 24-25: Tree min, contains
    printf(" %d", tree_min(root));          // 1
    printf(" %d", tree_contains(root, 5));  // 1

    // Test 26-27: Tree diameter, level count
    printf(" %d", tree_diameter(root)); // 4 (e.g., 4->2->1->3->7)
    printf(" %d", count_at_level(root, 2)); // 4

    // Test 28-29: Path sum
    // root-to-leaf: 1+2+4=7, 1+2+5=8, 1+3+6=10, 1+3+7=11
    printf(" %d", has_path_sum(root, 7));  // 1
    printf(" %d", has_path_sum(root, 9));  // 0

    // Test 30-31: Mirror tree then check
    tree_mirror(root);
    // After mirror: left of root = 3 (was right), right of root = 2
    printf(" %d", tree_val[tree_left[root]]);  // 3
    printf(" %d", tree_val[tree_right[root]]); // 2

    // Test 32-33: Recursive power
    printf(" %d", rec_power(2, 10)); // 1024
    printf(" %d", rec_power(3, 4));  // 81

    // Test 34-36: Recursive binary search
    arr[0]=10; arr[1]=20; arr[2]=30; arr[3]=40; arr[4]=50;
    printf(" %d", rec_binary_search(arr, 0, 4, 30)); // 2
    printf(" %d", rec_binary_search(arr, 0, 4, 10)); // 0
    printf(" %d", rec_binary_search(arr, 0, 4, 99)); // -1

    // Test 37-38: Recursive sum and max
    arr[0]=3; arr[1]=7; arr[2]=1; arr[3]=9; arr[4]=4;
    printf(" %d", rec_sum(arr, 5)); // 24
    printf(" %d", rec_max(arr, 5)); // 9

    // Test 39-40: Subsets count
    arr[0]=1; arr[1]=2; arr[2]=3;
    subset_count = 0;
    gen_subsets(arr, 3, 0, current, 0);
    printf(" %d", subset_count); // 8 (2^3)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    subset_count = 0;
    gen_subsets(arr, 4, 0, current, 0);
    printf(" %d", subset_count); // 16 (2^4)

    // Test 41-42: Permutation count
    arr[0]=1; arr[1]=2; arr[2]=3;
    perm_count = 0;
    gen_perms(arr, 3, 0);
    printf(" %d", perm_count); // 6 (3!)

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    perm_count = 0;
    gen_perms(arr, 4, 0);
    printf(" %d", perm_count); // 24 (4!)

    // Test 43-45: N-Queens
    printf(" %d", n_queens(4)); // 2
    printf(" %d", n_queens(5)); // 10
    printf(" %d", n_queens(6)); // 4

    // Test 46-47: Trees identical
    tree_init();
    r1 = tree_new(1);
    tree_left[r1] = tree_new(2);
    tree_right[r1] = tree_new(3);
    r2 = tree_new(1);
    tree_left[r2] = tree_new(2);
    tree_right[r2] = tree_new(3);
    printf(" %d", trees_identical(r1, r2)); // 1

    tree_val[tree_right[r2]] = 4;
    printf(" %d", trees_identical(r1, r2)); // 0

    // Test 48-50: More hanoi and recursion
    printf(" %d", solve_hanoi(4));   // 15
    printf(" %d", solve_hanoi(6));   // 63
    printf(" %d", rec_power(5, 3));  // 125

    printf("\n");
    return 0;
}
