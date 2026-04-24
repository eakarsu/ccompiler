int printf(const char *fmt, ...);

/* Singly linked list using arrays as backing store */

struct Node {
    int data;
    int next;  /* index of next node, -1 = end */
};

struct SList {
    struct Node nodes[32];
    int head;
    int free_head;
    int size;
};

void slist_init(struct SList *list) {
    int i;
    list->head = -1;
    list->size = 0;
    list->free_head = 0;
    for (i = 0; i < 31; i++) {
        list->nodes[i].next = i + 1;
        list->nodes[i].data = 0;
    }
    list->nodes[31].next = -1;
    list->nodes[31].data = 0;
}

int slist_alloc(struct SList *list) {
    int idx;
    if (list->free_head == -1) return -1;
    idx = list->free_head;
    list->free_head = list->nodes[idx].next;
    list->nodes[idx].next = -1;
    return idx;
}

void slist_free_node(struct SList *list, int idx) {
    list->nodes[idx].next = list->free_head;
    list->free_head = idx;
}

void slist_push_front(struct SList *list, int val) {
    int idx = slist_alloc(list);
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].next = list->head;
    list->head = idx;
    list->size = list->size + 1;
}

void slist_push_back(struct SList *list, int val) {
    int idx = slist_alloc(list);
    int cur;
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].next = -1;
    if (list->head == -1) {
        list->head = idx;
    } else {
        cur = list->head;
        while (list->nodes[cur].next != -1) {
            cur = list->nodes[cur].next;
        }
        list->nodes[cur].next = idx;
    }
    list->size = list->size + 1;
}

int slist_pop_front(struct SList *list) {
    int idx;
    int val;
    if (list->head == -1) return -999;
    idx = list->head;
    val = list->nodes[idx].data;
    list->head = list->nodes[idx].next;
    slist_free_node(list, idx);
    list->size = list->size - 1;
    return val;
}

void slist_print(struct SList *list) {
    int cur = list->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", list->nodes[cur].data);
        first = 0;
        cur = list->nodes[cur].next;
    }
    printf("\n");
}

int slist_find(struct SList *list, int val) {
    int cur = list->head;
    int pos = 0;
    while (cur != -1) {
        if (list->nodes[cur].data == val) return pos;
        cur = list->nodes[cur].next;
        pos = pos + 1;
    }
    return -1;
}

int slist_get(struct SList *list, int index) {
    int cur = list->head;
    int i = 0;
    while (cur != -1 && i < index) {
        cur = list->nodes[cur].next;
        i = i + 1;
    }
    if (cur == -1) return -999;
    return list->nodes[cur].data;
}

int main() {
    struct SList list;
    int val;
    int pos;

    slist_init(&list);

    slist_push_front(&list, 10);
    slist_push_front(&list, 20);
    slist_push_front(&list, 30);
    // EXPECT: 30 20 10
    slist_print(&list);

    printf("size=%d\n", list.size);
    // EXPECT: size=3

    slist_push_back(&list, 40);
    slist_push_back(&list, 50);
    // EXPECT: 30 20 10 40 50
    slist_print(&list);

    val = slist_pop_front(&list);
    printf("popped=%d\n", val);
    // EXPECT: popped=30

    // EXPECT: 20 10 40 50
    slist_print(&list);

    pos = slist_find(&list, 40);
    printf("find 40 at pos=%d\n", pos);
    // EXPECT: find 40 at pos=2

    pos = slist_find(&list, 99);
    printf("find 99 at pos=%d\n", pos);
    // EXPECT: find 99 at pos=-1

    val = slist_get(&list, 0);
    printf("get[0]=%d\n", val);
    // EXPECT: get[0]=20

    val = slist_get(&list, 3);
    printf("get[3]=%d\n", val);
    // EXPECT: get[3]=50

    val = slist_get(&list, 10);
    printf("get[10]=%d\n", val);
    // EXPECT: get[10]=-999

    printf("final size=%d\n", list.size);
    // EXPECT: final size=4

    return 0;
}
