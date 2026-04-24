int printf(const char *fmt, ...);

// Sliding puzzle (15-puzzle) move simulation and validation

typedef struct {
    int tiles[4][4]; // 0 = empty space
    int empty_r;
    int empty_c;
    int moves;
} Puzzle;

void puzzle_init(Puzzle *p) {
    int val = 1;
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            p->tiles[r][c] = val;
            val++;
        }
    }
    p->tiles[3][3] = 0; // empty space
    p->empty_r = 3;
    p->empty_c = 3;
    p->moves = 0;
}

// Check if puzzle is in solved state
int is_solved(Puzzle *p) {
    int val = 1;
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (r == 3 && c == 3) {
                if (p->tiles[r][c] != 0) return 0;
            } else {
                if (p->tiles[r][c] != val) return 0;
            }
            val++;
        }
    }
    return 1;
}

// Move: 0=up, 1=right, 2=down, 3=left
// These refer to the direction the TILE moves into the empty space
// Returns 1 on success, 0 on invalid move
int puzzle_move(Puzzle *p, int dir) {
    int tile_r = p->empty_r;
    int tile_c = p->empty_c;

    // Direction the tile comes from (opposite of where it moves to)
    if (dir == 0) tile_r++;       // tile from below moves up
    else if (dir == 1) tile_c--;  // tile from left moves right
    else if (dir == 2) tile_r--;  // tile from above moves down
    else if (dir == 3) tile_c++;  // tile from right moves left
    else return 0;

    if (tile_r < 0 || tile_r >= 4 || tile_c < 0 || tile_c >= 4)
        return 0;

    // Swap
    p->tiles[p->empty_r][p->empty_c] = p->tiles[tile_r][tile_c];
    p->tiles[tile_r][tile_c] = 0;
    p->empty_r = tile_r;
    p->empty_c = tile_c;
    p->moves++;
    return 1;
}

// Count misplaced tiles (Manhattan-like heuristic simplified)
int count_misplaced(Puzzle *p) {
    int count = 0;
    int val = 1;
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (r == 3 && c == 3) {
                if (p->tiles[r][c] != 0) count++;
            } else {
                if (p->tiles[r][c] != val) count++;
            }
            val++;
        }
    }
    return count;
}

// Compute Manhattan distance for all tiles
int manhattan_distance(Puzzle *p) {
    int dist = 0;
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            int val = p->tiles[r][c];
            if (val == 0) continue;
            int goal_r = (val - 1) / 4;
            int goal_c = (val - 1) % 4;
            int dr = r - goal_r;
            int dc = c - goal_c;
            if (dr < 0) dr = -dr;
            if (dc < 0) dc = -dc;
            dist += dr + dc;
        }
    }
    return dist;
}

// Count inversions (for solvability check)
int count_inversions(Puzzle *p) {
    int arr[16];
    int idx = 0;
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 4; c++)
            arr[idx++] = p->tiles[r][c];

    int inv = 0;
    int i, j;
    for (i = 0; i < 16; i++) {
        for (j = i + 1; j < 16; j++) {
            if (arr[i] != 0 && arr[j] != 0 && arr[i] > arr[j])
                inv++;
        }
    }
    return inv;
}

// Check solvability: for 4x4, solvable if:
// (inversions even AND blank on odd row from bottom) OR
// (inversions odd AND blank on even row from bottom)
int is_solvable(Puzzle *p) {
    int inv = count_inversions(p);
    int blank_row_from_bottom = 4 - p->empty_r; // 1-indexed from bottom
    if (blank_row_from_bottom % 2 == 1) {
        return inv % 2 == 0;
    } else {
        return inv % 2 == 1;
    }
}

void copy_puzzle(Puzzle *dst, Puzzle *src) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 4; c++)
            dst->tiles[r][c] = src->tiles[r][c];
    dst->empty_r = src->empty_r;
    dst->empty_c = src->empty_c;
    dst->moves = src->moves;
}

int main() {
    // Test 1: Solved puzzle
    Puzzle p;
    puzzle_init(&p);
    printf("solved: %d\n", is_solved(&p));
    // EXPECT: solved: 1
    printf("misplaced: %d\n", count_misplaced(&p));
    // EXPECT: misplaced: 0
    printf("manhattan: %d\n", manhattan_distance(&p));
    // EXPECT: manhattan: 0
    printf("inversions: %d\n", count_inversions(&p));
    // EXPECT: inversions: 0

    // Test 2: Make one move (tile from below moves up into empty at 3,3)
    // That means tile at (3+1,3) which is out of bounds. dir=0 means tile from below.
    // Empty is at (3,3). dir=0: tile_r = 3+1=4, invalid.
    // dir=2: tile from above: tile_r = 3-1=2, tile_c=3. So tile 12 moves down.
    int ok = puzzle_move(&p, 2);
    printf("move_down: %d\n", ok);
    // EXPECT: move_down: 1
    printf("solved_after1: %d\n", is_solved(&p));
    // EXPECT: solved_after1: 0
    printf("misplaced_after1: %d\n", count_misplaced(&p));
    // EXPECT: misplaced_after1: 2
    // Tile 12 moved from (2,3) to (3,3), empty now at (2,3)
    printf("empty_pos: %d %d\n", p.empty_r, p.empty_c);
    // EXPECT: empty_pos: 2 3
    printf("moved_tile: %d\n", p.tiles[3][3]);
    // EXPECT: moved_tile: 12

    // Test 3: Undo the move (move tile up: dir=0, tile from (3,3) moves up to (2,3))
    ok = puzzle_move(&p, 0);
    printf("undo: %d\n", ok);
    // EXPECT: undo: 1
    printf("solved_again: %d\n", is_solved(&p));
    // EXPECT: solved_again: 1

    // Test 4: Invalid move (try to move up when empty is at bottom-right)
    // Empty is at (3,3) again. dir=0 means tile from (4,3)=invalid
    ok = puzzle_move(&p, 0);
    printf("invalid_up: %d\n", ok);
    // EXPECT: invalid_up: 0
    // dir=1 means tile from (3,2) moves right
    ok = puzzle_move(&p, 1);
    printf("move_right: %d\n", ok);
    // EXPECT: move_right: 1
    printf("tile_at_33: %d\n", p.tiles[3][3]);
    // EXPECT: tile_at_33: 15
    printf("empty_after: %d %d\n", p.empty_r, p.empty_c);
    // EXPECT: empty_after: 3 2

    // Test 5: Sequence of moves and undo
    Puzzle p2;
    puzzle_init(&p2);
    // Scramble: down, left (right-tile-moves-left), down, left
    puzzle_move(&p2, 2); // tile from (2,3) goes to (3,3), empty at (2,3)
    puzzle_move(&p2, 3); // tile from (2,4)=invalid? No: empty at (2,3), dir=3 means tile from (2,4)=invalid
    // dir=3: tile_c = 3+1=4, invalid. Let's use dir=1 instead.
    // Reset and redo
    puzzle_init(&p2);
    puzzle_move(&p2, 2); // empty: (3,3)->(2,3), tile 12 moves to (3,3)
    puzzle_move(&p2, 1); // empty: (2,3), tile from (2,2) moves right to (2,3), empty at (2,2)
    puzzle_move(&p2, 2); // empty: (2,2), tile from (1,2) moves down to (2,2), empty at (1,2)
    puzzle_move(&p2, 1); // empty: (1,2), tile from (1,1) moves right to (1,2), empty at (1,1)
    printf("moves_p2: %d\n", p2.moves);
    // EXPECT: moves_p2: 4
    printf("misplaced_p2: %d\n", count_misplaced(&p2));
    // EXPECT: misplaced_p2: 5
    // After moves: tiles 12,11,7,6 are displaced
    // (3,3)=12, (2,3)=11, (2,2)=7, (1,2)=6, empty at (1,1)
    printf("tile_11: %d\n", p2.tiles[1][1]);
    // EXPECT: tile_11: 0
    printf("tile_12: %d\n", p2.tiles[1][2]);
    // EXPECT: tile_12: 6

    // Test 6: Manhattan distance after scramble
    int md = manhattan_distance(&p2);
    // Displaced tiles: tile 12 at (3,3) should be at (2,3): dist=1
    // tile 11 at (2,3) should be at (2,2): dist=1
    // tile 7 at (2,2) should be at (1,2): dist=1
    // tile 6 at (1,2) should be at (1,1): dist=1
    // All others in place. Total = 4
    printf("manhattan_p2: %d\n", md);
    // EXPECT: manhattan_p2: 4

    // Test 7: Solvability
    Puzzle p3;
    puzzle_init(&p3);
    printf("solvable_init: %d\n", is_solvable(&p3));
    // EXPECT: solvable_init: 1

    // Swap two tiles to make unsolvable (single swap changes parity)
    int tmp = p3.tiles[0][0];
    p3.tiles[0][0] = p3.tiles[0][1];
    p3.tiles[0][1] = tmp;
    printf("solvable_swapped: %d\n", is_solvable(&p3));
    // EXPECT: solvable_swapped: 0

    // Test 8: Copy puzzle
    Puzzle p4;
    copy_puzzle(&p4, &p2);
    printf("copy_moves: %d\n", p4.moves);
    // EXPECT: copy_moves: 4
    printf("copy_empty: %d %d\n", p4.empty_r, p4.empty_c);
    // EXPECT: copy_empty: 1 1
    printf("copy_eq: %d\n", p4.tiles[1][2] == p2.tiles[1][2]);
    // EXPECT: copy_eq: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
