int printf(const char *fmt, ...);
// EXPECT: Size: 8\nLevel 0: 3 6 7 9 12 16 19 24\nLevel 1: 6 12 16 24\nLevel 2: 12 16 24\nLevel 3: 16 24\nSearch 7: 70\nSearch 16: 160\nSearch 24: 240\nSearch 5: -1\nDone

// Skip list simulation using arrays with deterministic "random" levels

struct SkipNode {
    int key;
    int value;
    int next[4];  // next pointers for each level
    int valid;
};

struct SkipList {
    struct SkipNode nodes[32];
    int head;
    int max_level;
    int size;
    int next_id;
};

void sl_init(struct SkipList *sl) {
    sl->max_level = 4;
    sl->size = 0;
    sl->next_id = 1;
    // Node 0 is sentinel head
    sl->head = 0;
    sl->nodes[0].key = -999999;
    sl->nodes[0].value = 0;
    sl->nodes[0].valid = 1;
    int i = 0;
    while (i < 4) {
        sl->nodes[0].next[i] = -1;
        i = i + 1;
    }
    i = 1;
    while (i < 32) {
        sl->nodes[i].valid = 0;
        i = i + 1;
    }
}

// Deterministic level based on key
int sl_level_for_key(int key) {
    if (key % 8 == 0) return 3;
    if (key % 4 == 0) return 2;
    if (key % 2 == 0) return 1;
    return 0;
}

void sl_insert(struct SkipList *sl, int key, int value) {
    int update[4];
    int i = 3;
    while (i >= 0) {
        update[i] = -1;
        i = i - 1;
    }

    int current = sl->head;
    int level = sl->max_level - 1;
    while (level >= 0) {
        while (sl->nodes[current].next[level] >= 0 &&
               sl->nodes[sl->nodes[current].next[level]].key < key) {
            current = sl->nodes[current].next[level];
        }
        update[level] = current;
        level = level - 1;
    }

    int new_level = sl_level_for_key(key);
    int new_id = sl->next_id;
    sl->next_id = sl->next_id + 1;
    sl->nodes[new_id].key = key;
    sl->nodes[new_id].value = value;
    sl->nodes[new_id].valid = 1;
    i = 0;
    while (i < 4) {
        sl->nodes[new_id].next[i] = -1;
        i = i + 1;
    }

    i = 0;
    while (i <= new_level) {
        sl->nodes[new_id].next[i] = sl->nodes[update[i]].next[i];
        sl->nodes[update[i]].next[i] = new_id;
        i = i + 1;
    }
    sl->size = sl->size + 1;
}

int sl_search(struct SkipList *sl, int key) {
    int current = sl->head;
    int level = sl->max_level - 1;
    while (level >= 0) {
        while (sl->nodes[current].next[level] >= 0 &&
               sl->nodes[sl->nodes[current].next[level]].key < key) {
            current = sl->nodes[current].next[level];
        }
        level = level - 1;
    }
    int next = sl->nodes[current].next[0];
    if (next >= 0 && sl->nodes[next].key == key) {
        return sl->nodes[next].value;
    }
    return -1;
}

void sl_print_level(struct SkipList *sl, int level) {
    int current = sl->nodes[sl->head].next[level];
    printf("Level %d:", level);
    while (current >= 0) {
        printf(" %d", sl->nodes[current].key);
        current = sl->nodes[current].next[level];
    }
    printf("\n");
}

int main(void) {
    struct SkipList sl;
    sl_init(&sl);

    sl_insert(&sl, 3, 30);
    sl_insert(&sl, 6, 60);
    sl_insert(&sl, 7, 70);
    sl_insert(&sl, 9, 90);
    sl_insert(&sl, 12, 120);
    sl_insert(&sl, 16, 160);
    sl_insert(&sl, 19, 190);
    sl_insert(&sl, 24, 240);

    printf("Size: %d\n", sl.size);

    // Print each level
    int i = 0;
    while (i < 4) {
        sl_print_level(&sl, i);
        i = i + 1;
    }

    // Search
    printf("Search 7: %d\n", sl_search(&sl, 7));
    printf("Search 16: %d\n", sl_search(&sl, 16));
    printf("Search 24: %d\n", sl_search(&sl, 24));
    printf("Search 5: %d\n", sl_search(&sl, 5));

    printf("Done\n");
    return 0;
}
