int printf(const char *fmt, ...);

/* Skip list simulation using arrays (deterministic level assignment) */

struct SkipNode {
    int data;
    int next[4];  /* next pointers for levels 0..3 */
    int level;    /* highest level this node participates in */
};

struct SkipList {
    struct SkipNode nodes[32];
    int head;       /* index of sentinel head node */
    int max_level;  /* current max level in use */
    int count;      /* number of elements (excluding head) */
    int next_free;  /* next free index */
};

void skip_init(struct SkipList *sl) {
    int i;
    int j;
    sl->next_free = 1;  /* index 0 is the sentinel head */
    sl->count = 0;
    sl->max_level = 0;
    sl->head = 0;

    for (i = 0; i < 32; i++) {
        sl->nodes[i].data = 0;
        sl->nodes[i].level = 0;
        for (j = 0; j < 4; j++) {
            sl->nodes[i].next[j] = -1;
        }
    }
    /* Sentinel head has data = -9999 (minimum) */
    sl->nodes[0].data = -9999;
    sl->nodes[0].level = 3;
}

/* Deterministic level based on value (simulating random levels) */
int compute_level(int val) {
    /* Simple rule: level depends on divisibility */
    if (val % 8 == 0) return 3;
    if (val % 4 == 0) return 2;
    if (val % 2 == 0) return 1;
    return 0;
}

int skip_alloc(struct SkipList *sl) {
    int idx;
    if (sl->next_free >= 32) return -1;
    idx = sl->next_free;
    sl->next_free = sl->next_free + 1;
    return idx;
}

void skip_insert(struct SkipList *sl, int val) {
    int update[4];
    int cur;
    int lvl;
    int new_level;
    int idx;
    int i;

    /* Initialize update array */
    for (i = 0; i < 4; i++) {
        update[i] = sl->head;
    }

    cur = sl->head;

    /* Find position to insert at each level (top-down) */
    for (lvl = sl->max_level; lvl >= 0; lvl = lvl - 1) {
        while (sl->nodes[cur].next[lvl] != -1 &&
               sl->nodes[sl->nodes[cur].next[lvl]].data < val) {
            cur = sl->nodes[cur].next[lvl];
        }
        update[lvl] = cur;
    }

    /* Compute level for new node */
    new_level = compute_level(val);
    if (new_level > 3) new_level = 3;

    /* Allocate new node */
    idx = skip_alloc(sl);
    if (idx == -1) return;

    sl->nodes[idx].data = val;
    sl->nodes[idx].level = new_level;
    for (i = 0; i < 4; i++) {
        sl->nodes[idx].next[i] = -1;
    }

    /* Update max level if needed */
    if (new_level > sl->max_level) {
        sl->max_level = new_level;
    }

    /* Link new node at each level */
    for (lvl = 0; lvl <= new_level; lvl = lvl + 1) {
        sl->nodes[idx].next[lvl] = sl->nodes[update[lvl]].next[lvl];
        sl->nodes[update[lvl]].next[lvl] = idx;
    }

    sl->count = sl->count + 1;
}

/* Search for a value; returns node index or -1 */
int skip_search(struct SkipList *sl, int val) {
    int cur = sl->head;
    int lvl;

    for (lvl = sl->max_level; lvl >= 0; lvl = lvl - 1) {
        while (sl->nodes[cur].next[lvl] != -1 &&
               sl->nodes[sl->nodes[cur].next[lvl]].data < val) {
            cur = sl->nodes[cur].next[lvl];
        }
    }

    cur = sl->nodes[cur].next[0];
    if (cur != -1 && sl->nodes[cur].data == val) {
        return cur;
    }
    return -1;
}

/* Print level 0 (all elements) */
void skip_print_level(struct SkipList *sl, int lvl) {
    int cur;
    int first = 1;
    printf("L%d:", lvl);
    cur = sl->nodes[sl->head].next[lvl];
    while (cur != -1) {
        printf(" %d", sl->nodes[cur].data);
        cur = sl->nodes[cur].next[lvl];
    }
    printf("\n");
}

/* Delete a value from skip list */
int skip_delete(struct SkipList *sl, int val) {
    int update[4];
    int cur;
    int lvl;
    int target;
    int i;

    for (i = 0; i < 4; i++) update[i] = sl->head;

    cur = sl->head;
    for (lvl = sl->max_level; lvl >= 0; lvl = lvl - 1) {
        while (sl->nodes[cur].next[lvl] != -1 &&
               sl->nodes[sl->nodes[cur].next[lvl]].data < val) {
            cur = sl->nodes[cur].next[lvl];
        }
        update[lvl] = cur;
    }

    target = sl->nodes[cur].next[0];
    if (target == -1 || sl->nodes[target].data != val) return 0;

    for (lvl = 0; lvl <= sl->nodes[target].level; lvl = lvl + 1) {
        if (sl->nodes[update[lvl]].next[lvl] == target) {
            sl->nodes[update[lvl]].next[lvl] = sl->nodes[target].next[lvl];
        }
    }

    /* Update max level if needed */
    while (sl->max_level > 0 && sl->nodes[sl->head].next[sl->max_level] == -1) {
        sl->max_level = sl->max_level - 1;
    }

    sl->count = sl->count - 1;
    return 1;
}

int main() {
    struct SkipList sl;
    int found;
    int result;

    skip_init(&sl);

    /* Insert values: 5, 10, 15, 20, 25, 30, 3, 8, 16, 24 */
    skip_insert(&sl, 5);
    skip_insert(&sl, 10);
    skip_insert(&sl, 15);
    skip_insert(&sl, 20);
    skip_insert(&sl, 25);
    skip_insert(&sl, 30);
    skip_insert(&sl, 3);
    skip_insert(&sl, 8);
    skip_insert(&sl, 16);
    skip_insert(&sl, 24);

    printf("count=%d\n", sl.count);
    // EXPECT: count=10

    /* Level 0 has all elements */
    // EXPECT: L0: 3 5 8 10 15 16 20 24 25 30
    skip_print_level(&sl, 0);

    /* Level 1 has even numbers (divisible by 2) */
    // EXPECT: L1: 8 10 16 20 24 30
    skip_print_level(&sl, 1);

    /* Level 2 has numbers divisible by 4 */
    // EXPECT: L2: 8 16 20 24
    skip_print_level(&sl, 2);

    /* Level 3 has numbers divisible by 8 */
    // EXPECT: L3: 8 16 24
    skip_print_level(&sl, 3);

    /* Search tests */
    found = skip_search(&sl, 15);
    printf("search 15: %s\n", found != -1 ? "found" : "not found");
    // EXPECT: search 15: found

    found = skip_search(&sl, 7);
    printf("search 7: %s\n", found != -1 ? "found" : "not found");
    // EXPECT: search 7: not found

    found = skip_search(&sl, 24);
    printf("search 24: %s\n", found != -1 ? "found" : "not found");
    // EXPECT: search 24: found

    found = skip_search(&sl, 3);
    printf("search 3: %s\n", found != -1 ? "found" : "not found");
    // EXPECT: search 3: found

    /* Delete test */
    result = skip_delete(&sl, 10);
    printf("delete 10=%d\n", result);
    // EXPECT: delete 10=1

    result = skip_delete(&sl, 99);
    printf("delete 99=%d\n", result);
    // EXPECT: delete 99=0

    // EXPECT: L0: 3 5 8 15 16 20 24 25 30
    skip_print_level(&sl, 0);

    printf("count after delete=%d\n", sl.count);
    // EXPECT: count after delete=9

    /* Delete more */
    skip_delete(&sl, 8);
    skip_delete(&sl, 24);

    // EXPECT: L0: 3 5 15 16 20 25 30
    skip_print_level(&sl, 0);

    // EXPECT: L2: 16 20
    skip_print_level(&sl, 2);

    printf("final count=%d\n", sl.count);
    // EXPECT: final count=7

    return 0;
}
