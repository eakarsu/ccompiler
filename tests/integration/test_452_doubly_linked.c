int printf(const char *fmt, ...);
// EXPECT: Forward: [10,20,30]\nBackward: [30,20,10]\nAfter front pushes: [1,5,10,20,30]\nPop front: 1\nPop back: 30\nRemaining: [5,10,20]\nSize: 3
// Doubly linked list operations

struct DNode {
    int data;
    int prev;
    int next;
};

struct DList {
    struct DNode pool[20];
    int head;
    int tail;
    int free_head;
    int size;
};

void dl_init(struct DList *dl) {
    int i;
    for (i = 0; i < 20; i++) {
        dl->pool[i].data = 0;
        dl->pool[i].prev = -1;
        dl->pool[i].next = i + 1;
    }
    dl->pool[19].next = -1;
    dl->head = -1;
    dl->tail = -1;
    dl->free_head = 0;
    dl->size = 0;
}

int dl_alloc(struct DList *dl) {
    int idx;
    if (dl->free_head == -1) return -1;
    idx = dl->free_head;
    dl->free_head = dl->pool[idx].next;
    dl->pool[idx].prev = -1;
    dl->pool[idx].next = -1;
    return idx;
}

void dl_push_back(struct DList *dl, int val) {
    int idx = dl_alloc(dl);
    if (idx == -1) return;
    dl->pool[idx].data = val;
    dl->pool[idx].prev = dl->tail;
    dl->pool[idx].next = -1;
    if (dl->tail != -1) {
        dl->pool[dl->tail].next = idx;
    }
    dl->tail = idx;
    if (dl->head == -1) dl->head = idx;
    dl->size++;
}

void dl_push_front(struct DList *dl, int val) {
    int idx = dl_alloc(dl);
    if (idx == -1) return;
    dl->pool[idx].data = val;
    dl->pool[idx].next = dl->head;
    dl->pool[idx].prev = -1;
    if (dl->head != -1) {
        dl->pool[dl->head].prev = idx;
    }
    dl->head = idx;
    if (dl->tail == -1) dl->tail = idx;
    dl->size++;
}

int dl_pop_front(struct DList *dl) {
    int idx;
    int val;
    if (dl->head == -1) return -1;
    idx = dl->head;
    val = dl->pool[idx].data;
    dl->head = dl->pool[idx].next;
    if (dl->head != -1) {
        dl->pool[dl->head].prev = -1;
    } else {
        dl->tail = -1;
    }
    dl->pool[idx].next = dl->free_head;
    dl->free_head = idx;
    dl->size--;
    return val;
}

int dl_pop_back(struct DList *dl) {
    int idx;
    int val;
    if (dl->tail == -1) return -1;
    idx = dl->tail;
    val = dl->pool[idx].data;
    dl->tail = dl->pool[idx].prev;
    if (dl->tail != -1) {
        dl->pool[dl->tail].next = -1;
    } else {
        dl->head = -1;
    }
    dl->pool[idx].next = dl->free_head;
    dl->free_head = idx;
    dl->size--;
    return val;
}

void dl_print_forward(struct DList *dl) {
    int cur = dl->head;
    printf("[");
    while (cur != -1) {
        printf("%d", dl->pool[cur].data);
        cur = dl->pool[cur].next;
        if (cur != -1) printf(",");
    }
    printf("]\n");
}

void dl_print_backward(struct DList *dl) {
    int cur = dl->tail;
    printf("[");
    while (cur != -1) {
        printf("%d", dl->pool[cur].data);
        cur = dl->pool[cur].prev;
        if (cur != -1) printf(",");
    }
    printf("]\n");
}

int main(void) {
    struct DList dl;
    dl_init(&dl);

    dl_push_back(&dl, 10);
    dl_push_back(&dl, 20);
    dl_push_back(&dl, 30);
    printf("Forward: ");
    dl_print_forward(&dl);
    printf("Backward: ");
    dl_print_backward(&dl);

    dl_push_front(&dl, 5);
    dl_push_front(&dl, 1);
    printf("After front pushes: ");
    dl_print_forward(&dl);

    printf("Pop front: %d\n", dl_pop_front(&dl));
    printf("Pop back: %d\n", dl_pop_back(&dl));
    printf("Remaining: ");
    dl_print_forward(&dl);
    printf("Size: %d\n", dl.size);

    return 0;
}
