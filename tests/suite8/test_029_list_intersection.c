int printf(const char *fmt, ...);

/* Find intersection point of two linked lists (using array backing store) */

struct Node {
    int data;
    int next;
};

struct Pool {
    struct Node nodes[64];
    int next_free;
};

void pool_init(struct Pool *pool) {
    int i;
    pool->next_free = 0;
    for (i = 0; i < 64; i++) {
        pool->nodes[i].data = 0;
        pool->nodes[i].next = -1;
    }
}

int pool_alloc(struct Pool *pool, int data) {
    int idx;
    if (pool->next_free >= 64) return -1;
    idx = pool->next_free;
    pool->next_free = pool->next_free + 1;
    pool->nodes[idx].data = data;
    pool->nodes[idx].next = -1;
    return idx;
}

void pool_link(struct Pool *pool, int from, int to) {
    pool->nodes[from].next = to;
}

/* Get length of list starting at head */
int get_length(struct Pool *pool, int head) {
    int cur = head;
    int len = 0;
    while (cur != -1) {
        len = len + 1;
        cur = pool->nodes[cur].next;
    }
    return len;
}

/* Find intersection node of two lists (returns node index or -1) */
int find_intersection(struct Pool *pool, int head1, int head2) {
    int len1 = get_length(pool, head1);
    int len2 = get_length(pool, head2);
    int diff;
    int cur1 = head1;
    int cur2 = head2;
    int i;

    /* Advance the longer list by the difference */
    if (len1 > len2) {
        diff = len1 - len2;
        for (i = 0; i < diff; i++) {
            cur1 = pool->nodes[cur1].next;
        }
    } else {
        diff = len2 - len1;
        for (i = 0; i < diff; i++) {
            cur2 = pool->nodes[cur2].next;
        }
    }

    /* Walk both lists together until they meet */
    while (cur1 != -1 && cur2 != -1) {
        if (cur1 == cur2) return cur1;
        cur1 = pool->nodes[cur1].next;
        cur2 = pool->nodes[cur2].next;
    }
    return -1;
}

/* Print list from head */
void print_list(struct Pool *pool, int head) {
    int cur = head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", pool->nodes[cur].data);
        first = 0;
        cur = pool->nodes[cur].next;
    }
    printf("\n");
}

/* Check if two lists are identical (same sequence of data values) */
int lists_equal(struct Pool *pool, int h1, int h2) {
    int c1 = h1;
    int c2 = h2;
    while (c1 != -1 && c2 != -1) {
        if (pool->nodes[c1].data != pool->nodes[c2].data) return 0;
        c1 = pool->nodes[c1].next;
        c2 = pool->nodes[c2].next;
    }
    return (c1 == -1 && c2 == -1);
}

/* Remove duplicates from sorted list */
void remove_duplicates(struct Pool *pool, int head) {
    int cur = head;
    int next_node;
    while (cur != -1) {
        next_node = pool->nodes[cur].next;
        while (next_node != -1 && pool->nodes[next_node].data == pool->nodes[cur].data) {
            next_node = pool->nodes[next_node].next;
        }
        pool->nodes[cur].next = next_node;
        cur = next_node;
    }
}

int main() {
    struct Pool pool;
    int a0, a1, a2;
    int b0, b1;
    int c0, c1, c2;
    int inter;
    int head1, head2;
    int len;

    /* Test 1: Two lists sharing a common tail
     * List1: 1 -> 2 -> 3 -> 6 -> 7 -> 8
     * List2: 4 -> 5 -> 6 -> 7 -> 8
     * Intersection at node with data=6
     */
    pool_init(&pool);

    /* Common tail: 6 -> 7 -> 8 */
    c0 = pool_alloc(&pool, 6);
    c1 = pool_alloc(&pool, 7);
    c2 = pool_alloc(&pool, 8);
    pool_link(&pool, c0, c1);
    pool_link(&pool, c1, c2);

    /* List 1: 1 -> 2 -> 3 -> [common] */
    a0 = pool_alloc(&pool, 1);
    a1 = pool_alloc(&pool, 2);
    a2 = pool_alloc(&pool, 3);
    pool_link(&pool, a0, a1);
    pool_link(&pool, a1, a2);
    pool_link(&pool, a2, c0);

    /* List 2: 4 -> 5 -> [common] */
    b0 = pool_alloc(&pool, 4);
    b1 = pool_alloc(&pool, 5);
    pool_link(&pool, b0, b1);
    pool_link(&pool, b1, c0);

    printf("list1: ");
    // EXPECT: list1: 1 2 3 6 7 8
    print_list(&pool, a0);

    printf("list2: ");
    // EXPECT: list2: 4 5 6 7 8
    print_list(&pool, b0);

    len = get_length(&pool, a0);
    printf("list1 len=%d\n", len);
    // EXPECT: list1 len=6

    len = get_length(&pool, b0);
    printf("list2 len=%d\n", len);
    // EXPECT: list2 len=5

    inter = find_intersection(&pool, a0, b0);
    printf("intersection at data=%d\n", pool.nodes[inter].data);
    // EXPECT: intersection at data=6

    /* Test 2: No intersection */
    pool_init(&pool);
    a0 = pool_alloc(&pool, 10);
    a1 = pool_alloc(&pool, 20);
    pool_link(&pool, a0, a1);

    b0 = pool_alloc(&pool, 30);
    b1 = pool_alloc(&pool, 40);
    pool_link(&pool, b0, b1);

    inter = find_intersection(&pool, a0, b0);
    printf("no intersection=%d\n", inter);
    // EXPECT: no intersection=-1

    /* Test 3: Intersection at first node of shorter list */
    pool_init(&pool);
    c0 = pool_alloc(&pool, 100);
    c1 = pool_alloc(&pool, 200);
    pool_link(&pool, c0, c1);

    a0 = pool_alloc(&pool, 50);
    pool_link(&pool, a0, c0);

    printf("list_a: ");
    // EXPECT: list_a: 50 100 200
    print_list(&pool, a0);

    printf("list_c: ");
    // EXPECT: list_c: 100 200
    print_list(&pool, c0);

    inter = find_intersection(&pool, a0, c0);
    printf("intersection at data=%d\n", pool.nodes[inter].data);
    // EXPECT: intersection at data=100

    /* Test 4: Lists equal check */
    pool_init(&pool);
    a0 = pool_alloc(&pool, 1);
    a1 = pool_alloc(&pool, 2);
    a2 = pool_alloc(&pool, 3);
    pool_link(&pool, a0, a1);
    pool_link(&pool, a1, a2);

    b0 = pool_alloc(&pool, 1);
    b1 = pool_alloc(&pool, 2);
    c0 = pool_alloc(&pool, 3);
    pool_link(&pool, b0, b1);
    pool_link(&pool, b1, c0);

    printf("lists equal=%d\n", lists_equal(&pool, a0, b0));
    // EXPECT: lists equal=1

    /* Test 5: Remove duplicates from sorted list */
    pool_init(&pool);
    a0 = pool_alloc(&pool, 1);
    a1 = pool_alloc(&pool, 1);
    a2 = pool_alloc(&pool, 2);
    b0 = pool_alloc(&pool, 3);
    b1 = pool_alloc(&pool, 3);
    c0 = pool_alloc(&pool, 3);
    c1 = pool_alloc(&pool, 4);
    pool_link(&pool, a0, a1);
    pool_link(&pool, a1, a2);
    pool_link(&pool, a2, b0);
    pool_link(&pool, b0, b1);
    pool_link(&pool, b1, c0);
    pool_link(&pool, c0, c1);

    printf("before dedup: ");
    // EXPECT: before dedup: 1 1 2 3 3 3 4
    print_list(&pool, a0);

    remove_duplicates(&pool, a0);
    printf("after dedup: ");
    // EXPECT: after dedup: 1 2 3 4
    print_list(&pool, a0);

    printf("done\n");
    // EXPECT: done

    return 0;
}
