int printf(const char *fmt, ...);

/* Singly linked list using a node pool (stack-allocated) */

#define POOL_SIZE 20

typedef struct Node {
    int val;
    int next; /* index into pool, -1 = NULL */
} Node;

typedef struct {
    Node pool[POOL_SIZE];
    int  free_head; /* head of free list */
    int  pool_used;
} NodePool;

void pool_init(NodePool *p) {
    int i;
    for (i = 0; i < POOL_SIZE - 1; i++) {
        p->pool[i].next = i + 1;
    }
    p->pool[POOL_SIZE - 1].next = -1;
    p->free_head = 0;
    p->pool_used = 0;
}

int pool_alloc(NodePool *p, int val) {
    if (p->free_head == -1) return -1;
    int idx = p->free_head;
    p->free_head = p->pool[idx].next;
    p->pool[idx].val  = val;
    p->pool[idx].next = -1;
    p->pool_used++;
    return idx;
}

void pool_free(NodePool *p, int idx) {
    p->pool[idx].next = p->free_head;
    p->free_head = idx;
    p->pool_used--;
}

/* Linked list operations; head is an index (-1 = empty) */

/* Insert at front */
int list_prepend(NodePool *p, int head, int val) {
    int n = pool_alloc(p, val);
    if (n == -1) return head;
    p->pool[n].next = head;
    return n;
}

/* Insert at back */
int list_append(NodePool *p, int head, int val) {
    int n = pool_alloc(p, val);
    if (n == -1) return head;
    if (head == -1) return n;
    int cur = head;
    while (p->pool[cur].next != -1) cur = p->pool[cur].next;
    p->pool[cur].next = n;
    return head;
}

/* Delete first occurrence of val; returns new head */
int list_delete(NodePool *p, int head, int val) {
    if (head == -1) return -1;
    if (p->pool[head].val == val) {
        int new_head = p->pool[head].next;
        pool_free(p, head);
        return new_head;
    }
    int cur = head;
    while (p->pool[cur].next != -1) {
        int nxt = p->pool[cur].next;
        if (p->pool[nxt].val == val) {
            p->pool[cur].next = p->pool[nxt].next;
            pool_free(p, nxt);
            return head;
        }
        cur = nxt;
    }
    return head;
}

/* Search: return 1 if found */
int list_search(NodePool *p, int head, int val) {
    int cur = head;
    while (cur != -1) {
        if (p->pool[cur].val == val) return 1;
        cur = p->pool[cur].next;
    }
    return 0;
}

/* Length */
int list_length(NodePool *p, int head) {
    int len = 0, cur = head;
    while (cur != -1) { len++; cur = p->pool[cur].next; }
    return len;
}

/* Print */
void list_print(NodePool *p, int head) {
    int cur = head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", p->pool[cur].val);
        first = 0;
        cur = p->pool[cur].next;
    }
    printf("\n");
}

/* Reverse in-place; returns new head */
int list_reverse(NodePool *p, int head) {
    int prev = -1, cur = head;
    while (cur != -1) {
        int nxt = p->pool[cur].next;
        p->pool[cur].next = prev;
        prev = cur;
        cur  = nxt;
    }
    return prev;
}

int main(void) {
    NodePool pool;
    pool_init(&pool);
    int head = -1;

    /* Append 1..5 */
    int i;
    for (i = 1; i <= 5; i++) {
        head = list_append(&pool, head, i * 10);
    }
    /* list: 10 20 30 40 50 */
    list_print(&pool, head);
    // EXPECT: 10 20 30 40 50

    printf("%d\n", list_length(&pool, head));  /* 5 */
    // EXPECT: 5

    /* Prepend 0 */
    head = list_prepend(&pool, head, 0);
    /* list: 0 10 20 30 40 50 */
    list_print(&pool, head);
    // EXPECT: 0 10 20 30 40 50

    /* Search */
    printf("%d\n", list_search(&pool, head, 30)); /* 1 */
    // EXPECT: 1
    printf("%d\n", list_search(&pool, head, 99)); /* 0 */
    // EXPECT: 0

    /* Delete middle element 30 */
    head = list_delete(&pool, head, 30);
    list_print(&pool, head);
    // EXPECT: 0 10 20 40 50

    /* Delete head (0) */
    head = list_delete(&pool, head, 0);
    list_print(&pool, head);
    // EXPECT: 10 20 40 50

    /* Delete tail (50) */
    head = list_delete(&pool, head, 50);
    list_print(&pool, head);
    // EXPECT: 10 20 40

    printf("%d\n", list_length(&pool, head));  /* 3 */
    // EXPECT: 3

    /* Reverse */
    head = list_reverse(&pool, head);
    list_print(&pool, head);
    // EXPECT: 40 20 10

    /* Reverse again */
    head = list_reverse(&pool, head);
    list_print(&pool, head);
    // EXPECT: 10 20 40

    /* Delete non-existent is no-op */
    head = list_delete(&pool, head, 99);
    list_print(&pool, head);
    // EXPECT: 10 20 40

    /* Build a longer list and reverse */
    int head2 = -1;
    for (i = 1; i <= 7; i++) {
        head2 = list_append(&pool, head2, i);
    }
    /* 1 2 3 4 5 6 7 */
    list_print(&pool, head2);
    // EXPECT: 1 2 3 4 5 6 7

    head2 = list_reverse(&pool, head2);
    list_print(&pool, head2);
    // EXPECT: 7 6 5 4 3 2 1

    printf("%d\n", list_length(&pool, head2));  /* 7 */
    // EXPECT: 7

    /* pool_used should reflect live nodes: 3 (from head) + 7 (from head2) = 10 */
    printf("%d\n", pool.pool_used);             /* 10 */
    // EXPECT: 10

    /* Delete all from head2 */
    while (head2 != -1) {
        int v = pool.pool[head2].val;
        head2 = list_delete(&pool, head2, v);
    }
    printf("%d\n", list_length(&pool, head2));  /* 0 */
    // EXPECT: 0

    /* pool_used back to 3 */
    printf("%d\n", pool.pool_used);             /* 3 */
    // EXPECT: 3

    return 0;
}
