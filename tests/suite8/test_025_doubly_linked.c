int printf(const char *fmt, ...);

/* Doubly linked list using arrays as backing store */

struct DNode {
    int data;
    int prev;
    int next;
};

struct DList {
    struct DNode nodes[32];
    int head;
    int tail;
    int free_head;
    int size;
};

void dlist_init(struct DList *list) {
    int i;
    list->head = -1;
    list->tail = -1;
    list->size = 0;
    list->free_head = 0;
    for (i = 0; i < 31; i++) {
        list->nodes[i].next = i + 1;
        list->nodes[i].prev = -1;
        list->nodes[i].data = 0;
    }
    list->nodes[31].next = -1;
    list->nodes[31].prev = -1;
    list->nodes[31].data = 0;
}

int dlist_alloc(struct DList *list) {
    int idx;
    if (list->free_head == -1) return -1;
    idx = list->free_head;
    list->free_head = list->nodes[idx].next;
    list->nodes[idx].next = -1;
    list->nodes[idx].prev = -1;
    return idx;
}

void dlist_free_node(struct DList *list, int idx) {
    list->nodes[idx].next = list->free_head;
    list->nodes[idx].prev = -1;
    list->free_head = idx;
}

void dlist_push_front(struct DList *list, int val) {
    int idx = dlist_alloc(list);
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].prev = -1;
    list->nodes[idx].next = list->head;
    if (list->head != -1) {
        list->nodes[list->head].prev = idx;
    } else {
        list->tail = idx;
    }
    list->head = idx;
    list->size = list->size + 1;
}

void dlist_push_back(struct DList *list, int val) {
    int idx = dlist_alloc(list);
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].next = -1;
    list->nodes[idx].prev = list->tail;
    if (list->tail != -1) {
        list->nodes[list->tail].next = idx;
    } else {
        list->head = idx;
    }
    list->tail = idx;
    list->size = list->size + 1;
}

int dlist_pop_front(struct DList *list) {
    int idx;
    int val;
    if (list->head == -1) return -999;
    idx = list->head;
    val = list->nodes[idx].data;
    list->head = list->nodes[idx].next;
    if (list->head != -1) {
        list->nodes[list->head].prev = -1;
    } else {
        list->tail = -1;
    }
    dlist_free_node(list, idx);
    list->size = list->size - 1;
    return val;
}

int dlist_pop_back(struct DList *list) {
    int idx;
    int val;
    if (list->tail == -1) return -999;
    idx = list->tail;
    val = list->nodes[idx].data;
    list->tail = list->nodes[idx].prev;
    if (list->tail != -1) {
        list->nodes[list->tail].next = -1;
    } else {
        list->head = -1;
    }
    dlist_free_node(list, idx);
    list->size = list->size - 1;
    return val;
}

void dlist_print_forward(struct DList *list) {
    int cur = list->head;
    printf("fwd:");
    while (cur != -1) {
        printf(" %d", list->nodes[cur].data);
        cur = list->nodes[cur].next;
    }
    printf("\n");
}

void dlist_print_backward(struct DList *list) {
    int cur = list->tail;
    printf("bwd:");
    while (cur != -1) {
        printf(" %d", list->nodes[cur].data);
        cur = list->nodes[cur].prev;
    }
    printf("\n");
}

int dlist_remove_val(struct DList *list, int val) {
    int cur = list->head;
    while (cur != -1) {
        if (list->nodes[cur].data == val) {
            int p = list->nodes[cur].prev;
            int n = list->nodes[cur].next;
            if (p != -1) list->nodes[p].next = n;
            else list->head = n;
            if (n != -1) list->nodes[n].prev = p;
            else list->tail = p;
            dlist_free_node(list, cur);
            list->size = list->size - 1;
            return 1;
        }
        cur = list->nodes[cur].next;
    }
    return 0;
}

int main() {
    struct DList list;
    int val;

    dlist_init(&list);

    dlist_push_back(&list, 10);
    dlist_push_back(&list, 20);
    dlist_push_back(&list, 30);
    dlist_push_front(&list, 5);
    dlist_push_front(&list, 1);

    // EXPECT: fwd: 1 5 10 20 30
    dlist_print_forward(&list);

    // EXPECT: bwd: 30 20 10 5 1
    dlist_print_backward(&list);

    printf("size=%d\n", list.size);
    // EXPECT: size=5

    val = dlist_pop_front(&list);
    printf("pop_front=%d\n", val);
    // EXPECT: pop_front=1

    val = dlist_pop_back(&list);
    printf("pop_back=%d\n", val);
    // EXPECT: pop_back=30

    // EXPECT: fwd: 5 10 20
    dlist_print_forward(&list);

    val = dlist_remove_val(&list, 10);
    printf("removed 10=%d\n", val);
    // EXPECT: removed 10=1

    val = dlist_remove_val(&list, 99);
    printf("removed 99=%d\n", val);
    // EXPECT: removed 99=0

    // EXPECT: fwd: 5 20
    dlist_print_forward(&list);

    // EXPECT: bwd: 20 5
    dlist_print_backward(&list);

    printf("final size=%d\n", list.size);
    // EXPECT: final size=2

    return 0;
}
