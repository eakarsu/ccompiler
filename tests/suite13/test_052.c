int printf(const char *fmt, ...);

// Doubly linked list operations using array pool

struct DNode {
    int data;
    int prev; // index into pool, -1 = null
    int next; // index into pool, -1 = null
};

struct DList {
    struct DNode pool[20];
    int head;
    int tail;
    int free_head;
    int size;
};

void dlist_init(struct DList *dl) {
    int i;
    dl->head = -1;
    dl->tail = -1;
    dl->size = 0;
    dl->free_head = 0;
    for (i = 0; i < 19; i++) {
        dl->pool[i].next = i + 1;
        dl->pool[i].prev = -1;
        dl->pool[i].data = 0;
    }
    dl->pool[19].next = -1;
    dl->pool[19].prev = -1;
    dl->pool[19].data = 0;
}

int dlist_alloc(struct DList *dl) {
    int idx;
    if (dl->free_head == -1) return -1;
    idx = dl->free_head;
    dl->free_head = dl->pool[idx].next;
    dl->pool[idx].next = -1;
    dl->pool[idx].prev = -1;
    return idx;
}

void dlist_free_node(struct DList *dl, int idx) {
    dl->pool[idx].next = dl->free_head;
    dl->pool[idx].prev = -1;
    dl->free_head = idx;
}

void dlist_push_front(struct DList *dl, int val) {
    int idx = dlist_alloc(dl);
    if (idx == -1) return;
    dl->pool[idx].data = val;
    dl->pool[idx].prev = -1;
    dl->pool[idx].next = dl->head;
    if (dl->head != -1) {
        dl->pool[dl->head].prev = idx;
    } else {
        dl->tail = idx;
    }
    dl->head = idx;
    dl->size++;
}

void dlist_push_back(struct DList *dl, int val) {
    int idx = dlist_alloc(dl);
    if (idx == -1) return;
    dl->pool[idx].data = val;
    dl->pool[idx].next = -1;
    dl->pool[idx].prev = dl->tail;
    if (dl->tail != -1) {
        dl->pool[dl->tail].next = idx;
    } else {
        dl->head = idx;
    }
    dl->tail = idx;
    dl->size++;
}

int dlist_pop_front(struct DList *dl) {
    int idx;
    int val;
    if (dl->head == -1) return -999;
    idx = dl->head;
    val = dl->pool[idx].data;
    dl->head = dl->pool[idx].next;
    if (dl->head != -1) {
        dl->pool[dl->head].prev = -1;
    } else {
        dl->tail = -1;
    }
    dlist_free_node(dl, idx);
    dl->size--;
    return val;
}

int dlist_pop_back(struct DList *dl) {
    int idx;
    int val;
    if (dl->tail == -1) return -999;
    idx = dl->tail;
    val = dl->pool[idx].data;
    dl->tail = dl->pool[idx].prev;
    if (dl->tail != -1) {
        dl->pool[dl->tail].next = -1;
    } else {
        dl->head = -1;
    }
    dlist_free_node(dl, idx);
    dl->size--;
    return val;
}

void dlist_delete_node(struct DList *dl, int idx) {
    int p = dl->pool[idx].prev;
    int n = dl->pool[idx].next;
    if (p != -1) {
        dl->pool[p].next = n;
    } else {
        dl->head = n;
    }
    if (n != -1) {
        dl->pool[n].prev = p;
    } else {
        dl->tail = p;
    }
    dlist_free_node(dl, idx);
    dl->size--;
}

void dlist_print_forward(struct DList *dl) {
    int cur = dl->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", dl->pool[cur].data);
        first = 0;
        cur = dl->pool[cur].next;
    }
    printf("\n");
}

void dlist_print_backward(struct DList *dl) {
    int cur = dl->tail;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", dl->pool[cur].data);
        first = 0;
        cur = dl->pool[cur].prev;
    }
    printf("\n");
}

int main() {
    struct DList dl;
    int val;
    int cur;

    dlist_init(&dl);

    // Build list: push_back 10, 20, 30
    dlist_push_back(&dl, 10);
    dlist_push_back(&dl, 20);
    dlist_push_back(&dl, 30);

    printf("Forward: ");
    dlist_print_forward(&dl);
    // EXPECT: Forward: 10 20 30

    printf("Backward: ");
    dlist_print_backward(&dl);
    // EXPECT: Backward: 30 20 10

    printf("Size: %d\n", dl.size);
    // EXPECT: Size: 3

    // Push front: 5
    dlist_push_front(&dl, 5);
    printf("After push_front 5: ");
    dlist_print_forward(&dl);
    // EXPECT: After push_front 5: 5 10 20 30

    // Pop front
    val = dlist_pop_front(&dl);
    printf("Pop front: %d\n", val);
    // EXPECT: Pop front: 5

    printf("After pop front: ");
    dlist_print_forward(&dl);
    // EXPECT: After pop front: 10 20 30

    // Pop back
    val = dlist_pop_back(&dl);
    printf("Pop back: %d\n", val);
    // EXPECT: Pop back: 30

    printf("After pop back: ");
    dlist_print_forward(&dl);
    // EXPECT: After pop back: 10 20

    // Add more and delete middle
    dlist_push_back(&dl, 30);
    dlist_push_back(&dl, 40);
    dlist_push_back(&dl, 50);
    printf("List: ");
    dlist_print_forward(&dl);
    // EXPECT: List: 10 20 30 40 50

    // Find and delete node with value 30
    cur = dl.head;
    while (cur != -1) {
        if (dl.pool[cur].data == 30) {
            dlist_delete_node(&dl, cur);
            break;
        }
        cur = dl.pool[cur].next;
    }
    printf("After delete 30: ");
    dlist_print_forward(&dl);
    // EXPECT: After delete 30: 10 20 40 50

    printf("Backward check: ");
    dlist_print_backward(&dl);
    // EXPECT: Backward check: 50 40 20 10

    printf("Size: %d\n", dl.size);
    // EXPECT: Size: 4

    // Pop all
    val = dlist_pop_front(&dl);
    printf("Pop: %d\n", val);
    // EXPECT: Pop: 10

    val = dlist_pop_back(&dl);
    printf("Pop: %d\n", val);
    // EXPECT: Pop: 50

    val = dlist_pop_front(&dl);
    printf("Pop: %d\n", val);
    // EXPECT: Pop: 20

    val = dlist_pop_back(&dl);
    printf("Pop: %d\n", val);
    // EXPECT: Pop: 40

    printf("Size: %d\n", dl.size);
    // EXPECT: Size: 0

    printf("Empty pop: %d\n", dlist_pop_front(&dl));
    // EXPECT: Empty pop: -999

    return 0;
}
