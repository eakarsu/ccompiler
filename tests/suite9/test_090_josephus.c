int printf(const char *fmt, ...);

// Josephus problem - find the survivor position

int josephus(int n, int k) {
    int pos;
    int i;
    pos = 0;
    for (i = 2; i <= n; i++) {
        pos = (pos + k) % i;
    }
    return pos + 1;
}

int josephus_recursive(int n, int k) {
    if (n == 1) return 1;
    return (josephus_recursive(n - 1, k) + k - 1) % n + 1;
}

void josephus_elimination_order(int n, int k) {
    int circle[64];
    int alive;
    int i;
    int current;
    int count;
    int first;

    for (i = 0; i < n; i++) {
        circle[i] = 1;
    }
    alive = n;
    current = 0;
    first = 1;
    printf("Elimination order: ");
    while (alive > 0) {
        count = 0;
        while (count < k) {
            if (circle[current]) {
                count = count + 1;
            }
            if (count < k) {
                current = (current + 1) % n;
            }
        }
        circle[current] = 0;
        alive = alive - 1;
        if (first) {
            printf("%d", current + 1);
            first = 0;
        } else {
            printf(" %d", current + 1);
        }
        current = (current + 1) % n;
    }
    printf("\n");
}

int josephus_special_k2(int n) {
    int p;
    int l;
    p = 1;
    while (p * 2 <= n) {
        p = p * 2;
    }
    l = n - p;
    return 2 * l + 1;
}

void print_josephus_table(int max_n, int k) {
    int i;
    int first;
    first = 1;
    for (i = 1; i <= max_n; i++) {
        if (first) {
            printf("%d", josephus(i, k));
            first = 0;
        } else {
            printf(" %d", josephus(i, k));
        }
    }
    printf("\n");
}

int last_two_survivors(int n, int k) {
    int circle[64];
    int alive;
    int i;
    int current;
    int count;
    int second_last;

    for (i = 0; i < n; i++) {
        circle[i] = 1;
    }
    alive = n;
    current = 0;
    second_last = -1;

    while (alive > 1) {
        count = 0;
        while (count < k) {
            if (circle[current]) {
                count = count + 1;
            }
            if (count < k) {
                current = (current + 1) % n;
            }
        }
        if (alive == 2) {
            second_last = current + 1;
        }
        circle[current] = 0;
        alive = alive - 1;
        current = (current + 1) % n;
    }
    return second_last;
}

int josephus_survivor_position(int n, int k) {
    return josephus(n, k);
}

int count_safe_position(int max_n, int k, int pos) {
    int count;
    int i;
    count = 0;
    for (i = 1; i <= max_n; i++) {
        if (josephus(i, k) == pos) {
            count = count + 1;
        }
    }
    return count;
}

void josephus_multiple_rounds(int n, int k, int rounds) {
    int circle[64];
    int alive;
    int i;
    int current;
    int count;
    int eliminated;

    for (i = 0; i < n; i++) {
        circle[i] = 1;
    }
    alive = n;
    current = 0;
    eliminated = 0;

    while (eliminated < rounds && alive > 0) {
        count = 0;
        while (count < k) {
            if (circle[current]) {
                count = count + 1;
            }
            if (count < k) {
                current = (current + 1) % n;
            }
        }
        circle[current] = 0;
        alive = alive - 1;
        eliminated = eliminated + 1;
        printf("Round %d: eliminate %d\n", eliminated, current + 1);
        current = (current + 1) % n;
    }
}

int main() {
    int j;
    int j2;
    int sl;
    int csp;

    j = josephus(7, 3);
    // EXPECT: J(7,3) = 4
    printf("J(7,3) = %d\n", j);

    j = josephus(10, 2);
    // EXPECT: J(10,2) = 5
    printf("J(10,2) = %d\n", j);

    j = josephus(41, 3);
    // EXPECT: J(41,3) = 31
    printf("J(41,3) = %d\n", j);

    j2 = josephus_recursive(7, 3);
    // EXPECT: J_rec(7,3) = 4
    printf("J_rec(7,3) = %d\n", j2);

    j2 = josephus_recursive(10, 2);
    // EXPECT: J_rec(10,2) = 5
    printf("J_rec(10,2) = %d\n", j2);

    // EXPECT: J(10,2) formula: 5
    printf("J(10,2) formula: %d\n", josephus_special_k2(10));
    // EXPECT: J(16,2) formula: 1
    printf("J(16,2) formula: %d\n", josephus_special_k2(16));
    // EXPECT: J(41,2) formula: 19
    printf("J(41,2) formula: %d\n", josephus_special_k2(41));

    // EXPECT: Elimination order (7 people, k=3):
    printf("Elimination order (7 people, k=3):\n");
    // EXPECT: Elimination order: 3 6 2 7 5 1 4
    josephus_elimination_order(7, 3);

    // EXPECT: Elimination order (10 people, k=2):
    printf("Elimination order (10 people, k=2):\n");
    // EXPECT: Elimination order: 2 4 6 8 10 3 7 1 9 5
    josephus_elimination_order(10, 2);

    // EXPECT: J(n,2) for n=1..15:
    printf("J(n,2) for n=1..15:\n");
    // EXPECT: 1 1 3 1 3 5 7 1 3 5 7 9 11 13 15
    print_josephus_table(15, 2);

    // EXPECT: J(n,3) for n=1..15:
    printf("J(n,3) for n=1..15:\n");
    // EXPECT: 1 2 2 1 4 1 4 7 1 4 7 10 13 2 5
    print_josephus_table(15, 3);

    sl = last_two_survivors(10, 2);
    // EXPECT: Second-to-last eliminated (10,2): 9
    printf("Second-to-last eliminated (10,2): %d\n", sl);

    csp = count_safe_position(20, 2, 1);
    // EXPECT: Times position 1 survives (n=1..20, k=2): 5
    printf("Times position 1 survives (n=1..20, k=2): %d\n", csp);

    // EXPECT: First 5 eliminations (10 people, k=3):
    printf("First 5 eliminations (10 people, k=3):\n");
    // EXPECT: Round 1: eliminate 3
    // EXPECT: Round 2: eliminate 6
    // EXPECT: Round 3: eliminate 9
    // EXPECT: Round 4: eliminate 2
    // EXPECT: Round 5: eliminate 7
    josephus_multiple_rounds(10, 3, 5);

    return 0;
}
