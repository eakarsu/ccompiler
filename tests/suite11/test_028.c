int printf(const char *fmt, ...);

// Circular doubly linked list using array-based nodes

struct CDLNode {
    int data;
    int prev;
    int next;
    int active;
};

struct CDList {
    struct CDLNode nodes[32];
    int head;
    int size;
    int pool_next;
};

void cdl_init(struct CDList *l) {
    int i;
    l->head = -1;
    l->size = 0;
    l->pool_next = 0;
    for (i = 0; i < 32; i++) {
        l->nodes[i].active = 0;
    }
}

int cdl_alloc(struct CDList *l) {
    int idx = l->pool_next;
    l->pool_next = l->pool_next + 1;
    l->nodes[idx].active = 1;
    return idx;
}

void cdl_insert_front(struct CDList *l, int data) {
    int n = cdl_alloc(l);
    l->nodes[n].data = data;
    if (l->head == -1) {
        l->nodes[n].prev = n;
        l->nodes[n].next = n;
        l->head = n;
    } else {
        int tail = l->nodes[l->head].prev;
        l->nodes[n].next = l->head;
        l->nodes[n].prev = tail;
        l->nodes[tail].next = n;
        l->nodes[l->head].prev = n;
        l->head = n;
    }
    l->size = l->size + 1;
}

void cdl_insert_back(struct CDList *l, int data) {
    int n = cdl_alloc(l);
    l->nodes[n].data = data;
    if (l->head == -1) {
        l->nodes[n].prev = n;
        l->nodes[n].next = n;
        l->head = n;
    } else {
        int tail = l->nodes[l->head].prev;
        l->nodes[n].next = l->head;
        l->nodes[n].prev = tail;
        l->nodes[tail].next = n;
        l->nodes[l->head].prev = n;
    }
    l->size = l->size + 1;
}

int cdl_remove_front(struct CDList *l) {
    int old_head;
    int val;
    int tail;
    int new_head;
    if (l->head == -1) return -1;
    old_head = l->head;
    val = l->nodes[old_head].data;
    if (l->nodes[old_head].next == old_head) {
        l->head = -1;
    } else {
        tail = l->nodes[old_head].prev;
        new_head = l->nodes[old_head].next;
        l->nodes[new_head].prev = tail;
        l->nodes[tail].next = new_head;
        l->head = new_head;
    }
    l->nodes[old_head].active = 0;
    l->size = l->size - 1;
    return val;
}

int cdl_remove_back(struct CDList *l) {
    int tail;
    int val;
    int new_tail;
    if (l->head == -1) return -1;
    tail = l->nodes[l->head].prev;
    val = l->nodes[tail].data;
    if (tail == l->head) {
        l->head = -1;
    } else {
        new_tail = l->nodes[tail].prev;
        l->nodes[new_tail].next = l->head;
        l->nodes[l->head].prev = new_tail;
    }
    l->nodes[tail].active = 0;
    l->size = l->size - 1;
    return val;
}

void cdl_print_forward(struct CDList *l) {
    int cur;
    int start;
    if (l->head == -1) {
        printf("empty");
        return;
    }
    start = l->head;
    cur = start;
    printf("%d", l->nodes[cur].data);
    cur = l->nodes[cur].next;
    while (cur != start) {
        printf(" %d", l->nodes[cur].data);
        cur = l->nodes[cur].next;
    }
}

void cdl_print_backward(struct CDList *l) {
    int cur;
    int start;
    if (l->head == -1) {
        printf("empty");
        return;
    }
    start = l->nodes[l->head].prev;
    cur = start;
    printf("%d", l->nodes[cur].data);
    cur = l->nodes[cur].prev;
    while (cur != start) {
        printf(" %d", l->nodes[cur].data);
        cur = l->nodes[cur].prev;
    }
}

int cdl_rotate(struct CDList *l, int steps) {
    int i;
    if (l->head == -1) return -1;
    for (i = 0; i < steps; i++) {
        l->head = l->nodes[l->head].next;
    }
    return l->nodes[l->head].data;
}

int main() {
    struct CDList list;
    int val;

    cdl_init(&list);

    cdl_insert_back(&list, 10);
    cdl_insert_back(&list, 20);
    cdl_insert_back(&list, 30);
    printf("forward: ");
    cdl_print_forward(&list);
    printf("\n");                                    // EXPECT: forward: 10 20 30

    printf("backward: ");
    cdl_print_backward(&list);
    printf("\n");                                    // EXPECT: backward: 30 20 10

    cdl_insert_front(&list, 5);
    printf("after front 5: ");
    cdl_print_forward(&list);
    printf("\n");                                    // EXPECT: after front 5: 5 10 20 30

    printf("size: %d\n", list.size);                // EXPECT: size: 4

    val = cdl_remove_front(&list);
    printf("removed front: %d\n", val);             // EXPECT: removed front: 5

    val = cdl_remove_back(&list);
    printf("removed back: %d\n", val);              // EXPECT: removed back: 30

    printf("after removes: ");
    cdl_print_forward(&list);
    printf("\n");                                    // EXPECT: after removes: 10 20

    cdl_insert_back(&list, 30);
    cdl_insert_back(&list, 40);
    cdl_insert_back(&list, 50);
    val = cdl_rotate(&list, 2);
    printf("after rotate 2: %d\n", val);            // EXPECT: after rotate 2: 30

    printf("from rotated: ");
    cdl_print_forward(&list);
    printf("\n");                                    // EXPECT: from rotated: 30 40 50 10 20

    printf("size: %d\n", list.size);                // EXPECT: size: 5

    return 0;
}
