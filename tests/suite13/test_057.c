int printf(const char *fmt, ...);

// Skip list basic operations (simplified, array-based)
// A skip list with max 2 levels (level 0 = bottom, level 1 = express lane)

struct SkipNode {
    int data;
    int next0; // next at level 0 (bottom)
    int next1; // next at level 1 (top), -1 if not in level 1
    int level; // max level this node participates in (0 or 1)
    int used;
};

struct SkipList {
    struct SkipNode pool[30];
    int head;    // sentinel head node
    int free_idx;
    int size;
};

void skip_init(struct SkipList *sl) {
    int i;
    // Node 0 is the sentinel head
    sl->pool[0].data = -9999;
    sl->pool[0].next0 = -1;
    sl->pool[0].next1 = -1;
    sl->pool[0].level = 1;
    sl->pool[0].used = 1;
    sl->head = 0;
    sl->free_idx = 1;
    sl->size = 0;
    for (i = 1; i < 29; i++) {
        sl->pool[i].next0 = i + 1;
        sl->pool[i].next1 = -1;
        sl->pool[i].data = 0;
        sl->pool[i].level = 0;
        sl->pool[i].used = 0;
    }
    sl->pool[29].next0 = -1;
    sl->pool[29].next1 = -1;
    sl->pool[29].data = 0;
    sl->pool[29].level = 0;
    sl->pool[29].used = 0;
}

int skip_alloc(struct SkipList *sl) {
    int idx;
    if (sl->free_idx == -1) return -1;
    idx = sl->free_idx;
    sl->free_idx = sl->pool[idx].next0;
    sl->pool[idx].next0 = -1;
    sl->pool[idx].next1 = -1;
    sl->pool[idx].used = 1;
    return idx;
}

// Determine level for a new node based on its value
// Simple deterministic: promote every other insert (based on value)
int decide_level(int val) {
    // Promote if value is divisible by 3
    if (val % 3 == 0) return 1;
    return 0;
}

// Insert a value into the skip list (maintains sorted order)
void skip_insert(struct SkipList *sl, int val) {
    int new_idx = skip_alloc(sl);
    int new_level;
    int cur;

    if (new_idx == -1) return;
    sl->pool[new_idx].data = val;
    new_level = decide_level(val);
    sl->pool[new_idx].level = new_level;

    // Insert at level 1 if promoted
    if (new_level >= 1) {
        cur = sl->head;
        while (sl->pool[cur].next1 != -1 && sl->pool[sl->pool[cur].next1].data < val) {
            cur = sl->pool[cur].next1;
        }
        sl->pool[new_idx].next1 = sl->pool[cur].next1;
        sl->pool[cur].next1 = new_idx;
    }

    // Always insert at level 0
    cur = sl->head;
    while (sl->pool[cur].next0 != -1 && sl->pool[sl->pool[cur].next0].data < val) {
        cur = sl->pool[cur].next0;
    }
    sl->pool[new_idx].next0 = sl->pool[cur].next0;
    sl->pool[cur].next0 = new_idx;

    sl->size++;
}

// Search for a value; returns 1 if found, 0 otherwise
// Also counts steps for performance comparison
int skip_search(struct SkipList *sl, int val, int *steps) {
    int cur = sl->head;
    *steps = 0;

    // Search level 1 first
    while (sl->pool[cur].next1 != -1 && sl->pool[sl->pool[cur].next1].data <= val) {
        (*steps)++;
        if (sl->pool[sl->pool[cur].next1].data == val) return 1;
        cur = sl->pool[cur].next1;
    }

    // Drop to level 0
    while (sl->pool[cur].next0 != -1 && sl->pool[sl->pool[cur].next0].data <= val) {
        (*steps)++;
        if (sl->pool[sl->pool[cur].next0].data == val) return 1;
        cur = sl->pool[cur].next0;
    }

    return 0;
}

// Linear search at level 0 only (for comparison)
int linear_search(struct SkipList *sl, int val, int *steps) {
    int cur = sl->pool[sl->head].next0;
    *steps = 0;
    while (cur != -1) {
        (*steps)++;
        if (sl->pool[cur].data == val) return 1;
        if (sl->pool[cur].data > val) return 0;
        cur = sl->pool[cur].next0;
    }
    return 0;
}

void skip_print_level0(struct SkipList *sl) {
    int cur = sl->pool[sl->head].next0;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", sl->pool[cur].data);
        first = 0;
        cur = sl->pool[cur].next0;
    }
    printf("\n");
}

void skip_print_level1(struct SkipList *sl) {
    int cur = sl->pool[sl->head].next1;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", sl->pool[cur].data);
        first = 0;
        cur = sl->pool[cur].next1;
    }
    printf("\n");
}

// Delete a value from the skip list
int skip_delete(struct SkipList *sl, int val) {
    int cur;
    int target;

    // Remove from level 1 if present
    cur = sl->head;
    while (sl->pool[cur].next1 != -1) {
        if (sl->pool[sl->pool[cur].next1].data == val) {
            target = sl->pool[cur].next1;
            sl->pool[cur].next1 = sl->pool[target].next1;
            break;
        }
        cur = sl->pool[cur].next1;
    }

    // Remove from level 0
    cur = sl->head;
    while (sl->pool[cur].next0 != -1) {
        if (sl->pool[sl->pool[cur].next0].data == val) {
            target = sl->pool[cur].next0;
            sl->pool[cur].next0 = sl->pool[target].next0;
            sl->pool[target].used = 0;
            sl->size--;
            return 1;
        }
        cur = sl->pool[cur].next0;
    }
    return 0;
}

int main() {
    struct SkipList sl;
    int found;
    int steps;
    int linear_steps;
    int deleted;

    skip_init(&sl);

    // Insert values: 5, 10, 15, 20, 25, 30, 35, 40
    // Level 1 (divisible by 3): 15, 30
    // Wait - let me trace: 5%3=2(L0), 10%3=1(L0), 15%3=0(L1), 20%3=2(L0), 25%3=1(L0), 30%3=0(L1), 35%3=2(L0), 40%3=1(L0)
    skip_insert(&sl, 5);
    skip_insert(&sl, 10);
    skip_insert(&sl, 15);
    skip_insert(&sl, 20);
    skip_insert(&sl, 25);
    skip_insert(&sl, 30);
    skip_insert(&sl, 35);
    skip_insert(&sl, 40);

    printf("Size: %d\n", sl.size);
    // EXPECT: Size: 8

    printf("Level 0: ");
    skip_print_level0(&sl);
    // EXPECT: Level 0: 5 10 15 20 25 30 35 40

    printf("Level 1: ");
    skip_print_level1(&sl);
    // EXPECT: Level 1: 15 30

    // Search tests
    found = skip_search(&sl, 5, &steps);
    printf("Find 5: %d\n", found);
    // EXPECT: Find 5: 1

    found = skip_search(&sl, 40, &steps);
    printf("Find 40: %d\n", found);
    // EXPECT: Find 40: 1

    found = skip_search(&sl, 15, &steps);
    printf("Find 15: %d\n", found);
    // EXPECT: Find 15: 1

    found = skip_search(&sl, 30, &steps);
    printf("Find 30: %d\n", found);
    // EXPECT: Find 30: 1

    found = skip_search(&sl, 99, &steps);
    printf("Find 99: %d\n", found);
    // EXPECT: Find 99: 0

    found = skip_search(&sl, 1, &steps);
    printf("Find 1: %d\n", found);
    // EXPECT: Find 1: 0

    // Delete 15 (was on level 1)
    deleted = skip_delete(&sl, 15);
    printf("Delete 15: %d\n", deleted);
    // EXPECT: Delete 15: 1

    printf("After delete level 0: ");
    skip_print_level0(&sl);
    // EXPECT: After delete level 0: 5 10 20 25 30 35 40

    printf("After delete level 1: ");
    skip_print_level1(&sl);
    // EXPECT: After delete level 1: 30

    printf("Size: %d\n", sl.size);
    // EXPECT: Size: 7

    // Delete non-existent
    deleted = skip_delete(&sl, 99);
    printf("Delete 99: %d\n", deleted);
    // EXPECT: Delete 99: 0

    // Insert 9 (9%3=0, goes to level 1)
    skip_insert(&sl, 9);
    printf("Level 1 after insert 9: ");
    skip_print_level1(&sl);
    // EXPECT: Level 1 after insert 9: 9 30

    printf("Level 0 after insert 9: ");
    skip_print_level0(&sl);
    // EXPECT: Level 0 after insert 9: 5 9 10 20 25 30 35 40

    printf("Final size: %d\n", sl.size);
    // EXPECT: Final size: 8

    return 0;
}
