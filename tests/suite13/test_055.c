int printf(const char *fmt, ...);

// Detect and measure cycle in linked list (Floyd's algorithm)

struct Node {
    int data;
    int next;
};

struct Pool {
    struct Node nodes[20];
    int head;
    int free_idx;
    int count;
};

void pool_init(struct Pool *p) {
    int i;
    p->head = -1;
    p->count = 0;
    p->free_idx = 0;
    for (i = 0; i < 19; i++) {
        p->nodes[i].next = i + 1;
        p->nodes[i].data = 0;
    }
    p->nodes[19].next = -1;
    p->nodes[19].data = 0;
}

int pool_alloc(struct Pool *p) {
    int idx;
    if (p->free_idx == -1) return -1;
    idx = p->free_idx;
    p->free_idx = p->nodes[idx].next;
    p->nodes[idx].next = -1;
    return idx;
}

// Append a node and return its index
int pool_append(struct Pool *p, int val) {
    int idx = pool_alloc(p);
    int cur;
    if (idx == -1) return -1;
    p->nodes[idx].data = val;
    p->nodes[idx].next = -1;
    if (p->head == -1) {
        p->head = idx;
    } else {
        cur = p->head;
        while (p->nodes[cur].next != -1) {
            cur = p->nodes[cur].next;
        }
        p->nodes[cur].next = idx;
    }
    p->count++;
    return idx;
}

// Get node index at position pos (0-indexed)
int get_node_at(struct Pool *p, int pos) {
    int cur = p->head;
    int i;
    for (i = 0; i < pos; i++) {
        if (cur == -1) return -1;
        cur = p->nodes[cur].next;
    }
    return cur;
}

// Create a cycle: connect the tail to node at position pos
void create_cycle(struct Pool *p, int tail_pos, int cycle_pos) {
    int tail_idx = get_node_at(p, tail_pos);
    int cycle_idx = get_node_at(p, cycle_pos);
    if (tail_idx != -1 && cycle_idx != -1) {
        p->nodes[tail_idx].next = cycle_idx;
    }
}

// Floyd's cycle detection: returns 1 if cycle exists, 0 otherwise
int detect_cycle(struct Pool *p) {
    int slow;
    int fast;
    if (p->head == -1) return 0;
    slow = p->head;
    fast = p->head;
    while (fast != -1 && p->nodes[fast].next != -1) {
        slow = p->nodes[slow].next;
        fast = p->nodes[p->nodes[fast].next].next;
        if (slow == fast) return 1;
    }
    return 0;
}

// Find the start of the cycle (returns node index, or -1)
int find_cycle_start(struct Pool *p) {
    int slow;
    int fast;
    int has_cycle = 0;

    if (p->head == -1) return -1;
    slow = p->head;
    fast = p->head;
    while (fast != -1 && p->nodes[fast].next != -1) {
        slow = p->nodes[slow].next;
        fast = p->nodes[p->nodes[fast].next].next;
        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }
    if (!has_cycle) return -1;

    // Move slow to head, advance both by 1
    slow = p->head;
    while (slow != fast) {
        slow = p->nodes[slow].next;
        fast = p->nodes[fast].next;
    }
    return slow;
}

// Measure cycle length
int cycle_length(struct Pool *p) {
    int slow;
    int fast;
    int has_cycle = 0;
    int len;

    if (p->head == -1) return 0;
    slow = p->head;
    fast = p->head;
    while (fast != -1 && p->nodes[fast].next != -1) {
        slow = p->nodes[slow].next;
        fast = p->nodes[p->nodes[fast].next].next;
        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }
    if (!has_cycle) return 0;

    // Count nodes in the cycle
    len = 1;
    fast = p->nodes[slow].next;
    while (fast != slow) {
        len++;
        fast = p->nodes[fast].next;
    }
    return len;
}

int main() {
    struct Pool p;
    int cycle_start_idx;
    int i;

    // Test 1: No cycle
    pool_init(&p);
    pool_append(&p, 10);
    pool_append(&p, 20);
    pool_append(&p, 30);
    pool_append(&p, 40);

    printf("Test 1 - No cycle\n");
    // EXPECT: Test 1 - No cycle

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 0

    printf("Cycle length: %d\n", cycle_length(&p));
    // EXPECT: Cycle length: 0

    printf("Cycle start: %d\n", find_cycle_start(&p));
    // EXPECT: Cycle start: -1

    // Test 2: Cycle from tail back to head
    // List: 10 -> 20 -> 30 -> 40 -> (back to 10)
    pool_init(&p);
    pool_append(&p, 10);
    pool_append(&p, 20);
    pool_append(&p, 30);
    pool_append(&p, 40);
    create_cycle(&p, 3, 0);

    printf("Test 2 - Full cycle to head\n");
    // EXPECT: Test 2 - Full cycle to head

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 1

    printf("Cycle length: %d\n", cycle_length(&p));
    // EXPECT: Cycle length: 4

    cycle_start_idx = find_cycle_start(&p);
    printf("Cycle start data: %d\n", p.nodes[cycle_start_idx].data);
    // EXPECT: Cycle start data: 10

    // Test 3: Cycle from tail to middle
    // List: 10 -> 20 -> 30 -> 40 -> 50 -> (back to 30)
    pool_init(&p);
    pool_append(&p, 10);
    pool_append(&p, 20);
    pool_append(&p, 30);
    pool_append(&p, 40);
    pool_append(&p, 50);
    create_cycle(&p, 4, 2);

    printf("Test 3 - Cycle to middle\n");
    // EXPECT: Test 3 - Cycle to middle

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 1

    printf("Cycle length: %d\n", cycle_length(&p));
    // EXPECT: Cycle length: 3

    cycle_start_idx = find_cycle_start(&p);
    printf("Cycle start data: %d\n", p.nodes[cycle_start_idx].data);
    // EXPECT: Cycle start data: 30

    // Test 4: Self-loop (single node pointing to itself)
    pool_init(&p);
    pool_append(&p, 99);
    create_cycle(&p, 0, 0);

    printf("Test 4 - Self loop\n");
    // EXPECT: Test 4 - Self loop

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 1

    printf("Cycle length: %d\n", cycle_length(&p));
    // EXPECT: Cycle length: 1

    cycle_start_idx = find_cycle_start(&p);
    printf("Cycle start data: %d\n", p.nodes[cycle_start_idx].data);
    // EXPECT: Cycle start data: 99

    // Test 5: Longer tail, short cycle
    // 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> (back to 5)
    pool_init(&p);
    for (i = 1; i <= 7; i++) {
        pool_append(&p, i);
    }
    create_cycle(&p, 6, 4);

    printf("Test 5 - Long tail, short cycle\n");
    // EXPECT: Test 5 - Long tail, short cycle

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 1

    printf("Cycle length: %d\n", cycle_length(&p));
    // EXPECT: Cycle length: 3

    cycle_start_idx = find_cycle_start(&p);
    printf("Cycle start data: %d\n", p.nodes[cycle_start_idx].data);
    // EXPECT: Cycle start data: 5

    // Test 6: Empty list
    pool_init(&p);

    printf("Test 6 - Empty list\n");
    // EXPECT: Test 6 - Empty list

    printf("Has cycle: %d\n", detect_cycle(&p));
    // EXPECT: Has cycle: 0

    return 0;
}
