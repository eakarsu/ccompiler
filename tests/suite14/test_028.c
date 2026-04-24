int printf(const char *fmt, ...);

// Josephus problem: n people in a circle, every k-th person eliminated
// J(n, k) = (J(n-1, k) + k) % n, J(1, k) = 0

// Returns 0-indexed position of survivor
int josephus(int n, int k) {
    int pos = 0;
    int i;
    for (i = 2; i <= n; i++) {
        pos = (pos + k) % i;
    }
    return pos;
}

// Return 1-indexed position
int josephus1(int n, int k) {
    return josephus(n, k) + 1;
}

// Simulate the Josephus problem directly (brute force)
int josephus_simulate(int n, int k) {
    int alive[64];
    int i;
    for (i = 0; i < n; i++) {
        alive[i] = 1;
    }
    int remaining = n;
    int current = 0;
    int count;
    int last_eliminated = -1;

    while (remaining > 0) {
        count = 0;
        while (count < k) {
            if (alive[current]) {
                count++;
                if (count == k) {
                    alive[current] = 0;
                    last_eliminated = current;
                    remaining--;
                }
            }
            current = (current + 1) % n;
        }
    }
    return last_eliminated;
}

// Get the full elimination order
void josephus_order(int n, int k, int *order) {
    int alive[64];
    int i;
    for (i = 0; i < n; i++) {
        alive[i] = 1;
    }
    int remaining = n;
    int current = 0;
    int count;
    int idx = 0;

    while (remaining > 0) {
        count = 0;
        while (count < k) {
            if (alive[current]) {
                count++;
                if (count == k) {
                    alive[current] = 0;
                    order[idx] = current + 1; // 1-indexed
                    idx++;
                    remaining--;
                }
            }
            current = (current + 1) % n;
        }
    }
}

// Extended: find when person p is eliminated (1-indexed position in order)
int elimination_position(int n, int k, int p) {
    int order[64];
    josephus_order(n, k, order);
    int i;
    for (i = 0; i < n; i++) {
        if (order[i] == p) return i + 1;
    }
    return -1;
}

// Classic Josephus: k=2
// J(1)=1, J(2n)=2*J(n)-1, J(2n+1)=2*J(n)+1
// Closed form: J(n) = 2*L + 1 where n = 2^m + L
int josephus_k2_closed(int n) {
    // Find highest power of 2 <= n
    int p = 1;
    while (p * 2 <= n) p = p * 2;
    int l = n - p;
    return 2 * l + 1;
}

// Find n where person in position p survives
// (for k=2, survivor is at position 2L+1 where n=2^m+L)
int find_safe_position(int n, int k) {
    return josephus1(n, k);
}

int main(void) {
    // Test 1: Classic Josephus (k=2) small cases
    printf("J(1,2): %d\n", josephus1(1, 2));
    // EXPECT: J(1,2): 1
    printf("J(2,2): %d\n", josephus1(2, 2));
    // EXPECT: J(2,2): 1
    printf("J(3,2): %d\n", josephus1(3, 2));
    // EXPECT: J(3,2): 3
    printf("J(4,2): %d\n", josephus1(4, 2));
    // EXPECT: J(4,2): 1
    printf("J(5,2): %d\n", josephus1(5, 2));
    // EXPECT: J(5,2): 3
    printf("J(6,2): %d\n", josephus1(6, 2));
    // EXPECT: J(6,2): 5
    printf("J(7,2): %d\n", josephus1(7, 2));
    // EXPECT: J(7,2): 7

    // Test 2: k=3 cases
    printf("J(3,3): %d\n", josephus1(3, 3));
    // EXPECT: J(3,3): 2
    printf("J(5,3): %d\n", josephus1(5, 3));
    // EXPECT: J(5,3): 4
    printf("J(7,3): %d\n", josephus1(7, 3));
    // EXPECT: J(7,3): 4
    printf("J(10,3): %d\n", josephus1(10, 3));
    // EXPECT: J(10,3): 4

    // Test 3: Various k
    printf("J(6,4): %d\n", josephus1(6, 4));
    // EXPECT: J(6,4): 5
    printf("J(10,4): %d\n", josephus1(10, 4));
    // EXPECT: J(10,4): 5
    printf("J(8,5): %d\n", josephus1(8, 5));
    // EXPECT: J(8,5): 3

    // Test 4: Simulation matches formula
    int i;
    int all_match = 1;
    for (i = 1; i <= 20; i++) {
        if (josephus(i, 2) != josephus_simulate(i, 2)) {
            all_match = 0;
        }
    }
    printf("sim match k=2: %d\n", all_match);
    // EXPECT: sim match k=2: 1

    all_match = 1;
    for (i = 1; i <= 15; i++) {
        if (josephus(i, 3) != josephus_simulate(i, 3)) {
            all_match = 0;
        }
    }
    printf("sim match k=3: %d\n", all_match);
    // EXPECT: sim match k=3: 1

    // Test 5: Closed form for k=2
    printf("closed(1): %d\n", josephus_k2_closed(1));
    // EXPECT: closed(1): 1
    printf("closed(5): %d\n", josephus_k2_closed(5));
    // EXPECT: closed(5): 3
    printf("closed(10): %d\n", josephus_k2_closed(10));
    // EXPECT: closed(10): 5
    printf("closed(16): %d\n", josephus_k2_closed(16));
    // EXPECT: closed(16): 1
    printf("closed(41): %d\n", josephus_k2_closed(41));
    // EXPECT: closed(41): 19

    // Test 6: Elimination order for n=7, k=3
    int order[64];
    josephus_order(7, 3, order);
    printf("order 7,3: %d %d %d %d %d %d %d\n",
        order[0], order[1], order[2], order[3],
        order[4], order[5], order[6]);
    // EXPECT: order 7,3: 3 6 2 7 5 1 4

    // Test 7: Elimination order for n=6, k=2
    josephus_order(6, 2, order);
    printf("order 6,2: %d %d %d %d %d %d\n",
        order[0], order[1], order[2], order[3],
        order[4], order[5]);
    // EXPECT: order 6,2: 2 4 6 3 1 5

    // Test 8: Elimination position
    printf("elim pos(7,3,4): %d\n", elimination_position(7, 3, 4));
    // EXPECT: elim pos(7,3,4): 7
    printf("elim pos(7,3,3): %d\n", elimination_position(7, 3, 3));
    // EXPECT: elim pos(7,3,3): 1
    printf("elim pos(7,3,6): %d\n", elimination_position(7, 3, 6));
    // EXPECT: elim pos(7,3,6): 2

    // Test 9: Larger cases
    printf("J(20,2): %d\n", josephus1(20, 2));
    // EXPECT: J(20,2): 9
    printf("J(41,3): %d\n", josephus1(41, 3));
    // EXPECT: J(41,3): 31
    printf("J(50,7): %d\n", josephus1(50, 7));
    // EXPECT: J(50,7): 1

    // Test 10: Powers of 2 for k=2 (survivor is always position 1)
    printf("J(2,2): %d\n", josephus1(2, 2));
    // EXPECT: J(2,2): 1
    printf("J(8,2): %d\n", josephus1(8, 2));
    // EXPECT: J(8,2): 1
    printf("J(32,2): %d\n", josephus1(32, 2));
    // EXPECT: J(32,2): 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
