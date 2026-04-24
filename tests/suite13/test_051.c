int printf(const char *fmt, ...);

// Singly linked list with insert/delete/search using array pool

struct SNode {
    int data;
    int next; // index into pool, -1 = null
};

struct SList {
    struct SNode pool[20];
    int head;       // index of first node, -1 = empty
    int free_head;  // index of first free node
    int size;
};

void slist_init(struct SList *list) {
    int i;
    list->head = -1;
    list->size = 0;
    list->free_head = 0;
    for (i = 0; i < 19; i++) {
        list->pool[i].next = i + 1;
        list->pool[i].data = 0;
    }
    list->pool[19].next = -1;
    list->pool[19].data = 0;
}

int slist_alloc(struct SList *list) {
    int idx;
    if (list->free_head == -1) return -1;
    idx = list->free_head;
    list->free_head = list->pool[idx].next;
    list->pool[idx].next = -1;
    return idx;
}

void slist_free_node(struct SList *list, int idx) {
    list->pool[idx].next = list->free_head;
    list->free_head = idx;
}

void slist_push_front(struct SList *list, int val) {
    int idx = slist_alloc(list);
    if (idx == -1) return;
    list->pool[idx].data = val;
    list->pool[idx].next = list->head;
    list->head = idx;
    list->size++;
}

void slist_push_back(struct SList *list, int val) {
    int idx = slist_alloc(list);
    int cur;
    if (idx == -1) return;
    list->pool[idx].data = val;
    list->pool[idx].next = -1;
    if (list->head == -1) {
        list->head = idx;
    } else {
        cur = list->head;
        while (list->pool[cur].next != -1) {
            cur = list->pool[cur].next;
        }
        list->pool[cur].next = idx;
    }
    list->size++;
}

int slist_delete(struct SList *list, int val) {
    int cur;
    int prev;
    if (list->head == -1) return 0;
    if (list->pool[list->head].data == val) {
        cur = list->head;
        list->head = list->pool[cur].next;
        slist_free_node(list, cur);
        list->size--;
        return 1;
    }
    prev = list->head;
    cur = list->pool[prev].next;
    while (cur != -1) {
        if (list->pool[cur].data == val) {
            list->pool[prev].next = list->pool[cur].next;
            slist_free_node(list, cur);
            list->size--;
            return 1;
        }
        prev = cur;
        cur = list->pool[cur].next;
    }
    return 0;
}

int slist_search(struct SList *list, int val) {
    int cur = list->head;
    int pos = 0;
    while (cur != -1) {
        if (list->pool[cur].data == val) return pos;
        cur = list->pool[cur].next;
        pos++;
    }
    return -1;
}

void slist_print(struct SList *list) {
    int cur = list->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" -> ");
        printf("%d", list->pool[cur].data);
        first = 0;
        cur = list->pool[cur].next;
    }
    printf("\n");
}

int slist_get(struct SList *list, int index) {
    int cur = list->head;
    int i;
    for (i = 0; i < index; i++) {
        if (cur == -1) return -999;
        cur = list->pool[cur].next;
    }
    if (cur == -1) return -999;
    return list->pool[cur].data;
}

int main() {
    struct SList list;
    int found;
    int deleted;

    slist_init(&list);

    // Push front: 3, 2, 1 => list is 1 -> 2 -> 3
    slist_push_front(&list, 3);
    slist_push_front(&list, 2);
    slist_push_front(&list, 1);

    printf("Size after 3 push_front: %d\n", list.size);
    // EXPECT: Size after 3 push_front: 3

    printf("List: %d %d %d\n", slist_get(&list, 0), slist_get(&list, 1), slist_get(&list, 2));
    // EXPECT: List: 1 2 3

    // Push back: 4, 5 => list is 1 -> 2 -> 3 -> 4 -> 5
    slist_push_back(&list, 4);
    slist_push_back(&list, 5);

    printf("Size after 2 push_back: %d\n", list.size);
    // EXPECT: Size after 2 push_back: 5

    slist_print(&list);
    // EXPECT: 1 -> 2 -> 3 -> 4 -> 5

    // Search
    found = slist_search(&list, 3);
    printf("Search 3: pos %d\n", found);
    // EXPECT: Search 3: pos 2

    found = slist_search(&list, 5);
    printf("Search 5: pos %d\n", found);
    // EXPECT: Search 5: pos 4

    found = slist_search(&list, 99);
    printf("Search 99: pos %d\n", found);
    // EXPECT: Search 99: pos -1

    // Delete middle element (3)
    deleted = slist_delete(&list, 3);
    printf("Delete 3: %d\n", deleted);
    // EXPECT: Delete 3: 1

    slist_print(&list);
    // EXPECT: 1 -> 2 -> 4 -> 5

    // Delete head (1)
    deleted = slist_delete(&list, 1);
    printf("Delete 1: %d\n", deleted);
    // EXPECT: Delete 1: 1

    slist_print(&list);
    // EXPECT: 2 -> 4 -> 5

    // Delete tail (5)
    deleted = slist_delete(&list, 5);
    printf("Delete 5: %d\n", deleted);
    // EXPECT: Delete 5: 1

    slist_print(&list);
    // EXPECT: 2 -> 4

    // Delete non-existent
    deleted = slist_delete(&list, 99);
    printf("Delete 99: %d\n", deleted);
    // EXPECT: Delete 99: 0

    printf("Final size: %d\n", list.size);
    // EXPECT: Final size: 2

    // Rebuild list for more testing
    slist_push_front(&list, 10);
    slist_push_back(&list, 20);
    slist_print(&list);
    // EXPECT: 10 -> 2 -> 4 -> 20

    printf("Final size: %d\n", list.size);
    // EXPECT: Final size: 4

    return 0;
}
