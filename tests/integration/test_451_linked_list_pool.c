int printf(const char *fmt, ...);
// EXPECT: After front inserts: [10,20,30]\nSearch 20: pos=1\nSearch 99: pos=-1\nAfter delete 20: [10,30]\nAfter delete 10: [30]\nSorted insert: [10,20,30,40,50]\nSize: 5
// Linked list with array pool: insert, delete, search, print

struct Node {
    int data;
    int next; // index into pool, -1 = null
};

struct LinkedList {
    struct Node pool[20];
    int free_list;
    int head;
    int size;
};

void ll_init(struct LinkedList *ll) {
    int i;
    for (i = 0; i < 20; i++) {
        ll->pool[i].data = 0;
        ll->pool[i].next = i + 1;
    }
    ll->pool[19].next = -1;
    ll->free_list = 0;
    ll->head = -1;
    ll->size = 0;
}

int ll_alloc(struct LinkedList *ll) {
    int idx;
    if (ll->free_list == -1) return -1;
    idx = ll->free_list;
    ll->free_list = ll->pool[idx].next;
    return idx;
}

void ll_free_node(struct LinkedList *ll, int idx) {
    ll->pool[idx].next = ll->free_list;
    ll->free_list = idx;
}

void ll_insert_front(struct LinkedList *ll, int val) {
    int idx = ll_alloc(ll);
    if (idx == -1) return;
    ll->pool[idx].data = val;
    ll->pool[idx].next = ll->head;
    ll->head = idx;
    ll->size++;
}

void ll_insert_sorted(struct LinkedList *ll, int val) {
    int idx = ll_alloc(ll);
    int cur;
    int prev;
    if (idx == -1) return;
    ll->pool[idx].data = val;
    if (ll->head == -1 || ll->pool[ll->head].data >= val) {
        ll->pool[idx].next = ll->head;
        ll->head = idx;
    } else {
        prev = ll->head;
        cur = ll->pool[ll->head].next;
        while (cur != -1 && ll->pool[cur].data < val) {
            prev = cur;
            cur = ll->pool[cur].next;
        }
        ll->pool[idx].next = cur;
        ll->pool[prev].next = idx;
    }
    ll->size++;
}

int ll_search(struct LinkedList *ll, int val) {
    int cur = ll->head;
    int pos = 0;
    while (cur != -1) {
        if (ll->pool[cur].data == val) return pos;
        cur = ll->pool[cur].next;
        pos++;
    }
    return -1;
}

int ll_delete(struct LinkedList *ll, int val) {
    int cur = ll->head;
    int prev = -1;
    while (cur != -1) {
        if (ll->pool[cur].data == val) {
            if (prev == -1) {
                ll->head = ll->pool[cur].next;
            } else {
                ll->pool[prev].next = ll->pool[cur].next;
            }
            ll_free_node(ll, cur);
            ll->size--;
            return 1;
        }
        prev = cur;
        cur = ll->pool[cur].next;
    }
    return 0;
}

void ll_print(struct LinkedList *ll) {
    int cur = ll->head;
    printf("[");
    while (cur != -1) {
        printf("%d", ll->pool[cur].data);
        cur = ll->pool[cur].next;
        if (cur != -1) printf(",");
    }
    printf("]\n");
}

int main(void) {
    struct LinkedList ll;
    ll_init(&ll);

    ll_insert_front(&ll, 30);
    ll_insert_front(&ll, 20);
    ll_insert_front(&ll, 10);
    printf("After front inserts: ");
    ll_print(&ll);

    printf("Search 20: pos=%d\n", ll_search(&ll, 20));
    printf("Search 99: pos=%d\n", ll_search(&ll, 99));

    ll_delete(&ll, 20);
    printf("After delete 20: ");
    ll_print(&ll);

    ll_delete(&ll, 10);
    printf("After delete 10: ");
    ll_print(&ll);

    ll_init(&ll);
    ll_insert_sorted(&ll, 50);
    ll_insert_sorted(&ll, 20);
    ll_insert_sorted(&ll, 40);
    ll_insert_sorted(&ll, 10);
    ll_insert_sorted(&ll, 30);
    printf("Sorted insert: ");
    ll_print(&ll);
    printf("Size: %d\n", ll.size);

    return 0;
}
