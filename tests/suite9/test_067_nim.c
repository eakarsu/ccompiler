int printf(const char *fmt, ...);

// Nim Game Engine
// Classic Nim: multiple heaps, players alternate removing objects
// Winning strategy based on XOR (nim-sum)

int heaps[10];
int num_heaps;
int move_history_heap[100];
int move_history_count[100];
int total_moves;

void init_game(int n, int sizes[]) {
    num_heaps = n;
    int i;
    for (i = 0; i < n; i++) {
        heaps[i] = sizes[i];
    }
    total_moves = 0;
}

int nim_sum(void) {
    int xor_val = 0;
    int i;
    for (i = 0; i < num_heaps; i++) {
        xor_val = xor_val ^ heaps[i];
    }
    return xor_val;
}

int is_game_over(void) {
    int i;
    for (i = 0; i < num_heaps; i++) {
        if (heaps[i] > 0) return 0;
    }
    return 1;
}

int total_objects(void) {
    int sum = 0;
    int i;
    for (i = 0; i < num_heaps; i++) {
        sum += heaps[i];
    }
    return sum;
}

// Make a move: remove count objects from heap
int make_move(int heap, int count) {
    if (heap < 0 || heap >= num_heaps) return 0;
    if (count < 1 || count > heaps[heap]) return 0;
    heaps[heap] = heaps[heap] - count;
    if (total_moves < 100) {
        move_history_heap[total_moves] = heap;
        move_history_count[total_moves] = count;
    }
    total_moves++;
    return 1;
}

// Find optimal move: returns heap in *out_heap, count in *out_count
// Returns 1 if winning move found, 0 if losing position
int find_optimal_move(int *out_heap, int *out_count) {
    int ns = nim_sum();
    if (ns == 0) {
        // Losing position - take 1 from first non-empty heap
        int i;
        for (i = 0; i < num_heaps; i++) {
            if (heaps[i] > 0) {
                *out_heap = i;
                *out_count = 1;
                return 0;
            }
        }
        return 0;
    }
    // Winning position - find move to make nim-sum 0
    int i;
    for (i = 0; i < num_heaps; i++) {
        int target = heaps[i] ^ ns;
        if (target < heaps[i]) {
            *out_heap = i;
            *out_count = heaps[i] - target;
            return 1;
        }
    }
    return 0;
}

// Is current position a winning position for the player to move?
int is_winning(void) {
    return nim_sum() != 0;
}

// Count non-empty heaps
int active_heaps(void) {
    int count = 0;
    int i;
    for (i = 0; i < num_heaps; i++) {
        if (heaps[i] > 0) count++;
    }
    return count;
}

// Simulate a full game with optimal play
// Returns 1 if first player wins, 2 if second player wins
int simulate_optimal_game(void) {
    int player = 1;
    while (!is_game_over()) {
        int heap;
        int count;
        find_optimal_move(&heap, &count);
        make_move(heap, count);
        if (player == 1) player = 2;
        else player = 1;
    }
    // Last player to move wins, so the player whose turn it is now loses
    if (player == 1) return 2;
    return 1;
}

int main(void) {
    int sizes[10];

    // Test 1: Simple 1-heap game
    sizes[0] = 5;
    init_game(1, sizes);
    printf("Nim sum (1 heap of 5): %d\n", nim_sum());
    // EXPECT: Nim sum (1 heap of 5): 5

    // Test 2: Classic 3-heap game (1, 2, 3)
    sizes[0] = 1; sizes[1] = 2; sizes[2] = 3;
    init_game(3, sizes);
    printf("Nim sum (1,2,3): %d\n", nim_sum());
    // EXPECT: Nim sum (1,2,3): 0

    // Test 3: Is (1,2,3) a winning position?
    printf("(1,2,3) winning: %d\n", is_winning());
    // EXPECT: (1,2,3) winning: 0

    // Test 4: (1,2,4) should be winning
    sizes[0] = 1; sizes[1] = 2; sizes[2] = 4;
    init_game(3, sizes);
    printf("(1,2,4) winning: %d\n", is_winning());
    // EXPECT: (1,2,4) winning: 1

    // Test 5: Find optimal move for (1,2,4)
    int heap;
    int count;
    int found = find_optimal_move(&heap, &count);
    printf("Optimal: heap %d remove %d (found=%d)\n", heap, count, found);
    // EXPECT: Optimal: heap 2 remove 1 (found=1)

    // Test 6: After optimal move, nim-sum should be 0
    make_move(heap, count);
    printf("After optimal: nim sum=%d heaps=(%d,%d,%d)\n",
           nim_sum(), heaps[0], heaps[1], heaps[2]);
    // EXPECT: After optimal: nim sum=0 heaps=(1,2,3)

    // Test 7: Total objects
    int tot = total_objects();
    printf("Total objects: %d\n", tot);
    // EXPECT: Total objects: 6

    // Test 8: Invalid move
    int ok = make_move(0, 5);
    printf("Invalid move result: %d\n", ok);
    // EXPECT: Invalid move result: 0

    // Test 9: Simulate full game from winning position
    sizes[0] = 3; sizes[1] = 4; sizes[2] = 5;
    init_game(3, sizes);
    int ns = nim_sum();
    printf("(3,4,5) nim sum: %d\n", ns);
    // EXPECT: (3,4,5) nim sum: 2
    int winner = simulate_optimal_game();
    printf("Winner from (3,4,5): player %d\n", winner);
    // EXPECT: Winner from (3,4,5): player 1

    // Test 10: Simulate from losing position
    sizes[0] = 1; sizes[1] = 2; sizes[2] = 3;
    init_game(3, sizes);
    winner = simulate_optimal_game();
    printf("Winner from (1,2,3): player %d\n", winner);
    // EXPECT: Winner from (1,2,3): player 2

    // Test 11: Single object - immediate win
    sizes[0] = 1;
    init_game(1, sizes);
    printf("(1) winning: %d\n", is_winning());
    // EXPECT: (1) winning: 1

    // Test 12: Game over check
    sizes[0] = 0; sizes[1] = 0;
    init_game(2, sizes);
    printf("Empty game over: %d\n", is_game_over());
    // EXPECT: Empty game over: 1

    // Test 13: Active heaps
    sizes[0] = 3; sizes[1] = 0; sizes[2] = 5; sizes[3] = 0;
    init_game(4, sizes);
    printf("Active heaps: %d\n", active_heaps());
    // EXPECT: Active heaps: 2

    // Test 14: Larger game
    sizes[0] = 7; sizes[1] = 11; sizes[2] = 13; sizes[3] = 9;
    init_game(4, sizes);
    ns = nim_sum();
    printf("(7,11,13,9) nim sum: %d winning: %d\n", ns, is_winning());
    // EXPECT: (7,11,13,9) nim sum: 8 winning: 1

    printf("Nim game tests passed\n");
    // EXPECT: Nim game tests passed
    return 0;
}
