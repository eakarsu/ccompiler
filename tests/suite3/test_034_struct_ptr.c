int printf(const char *fmt, ...);

/* Node for a singly-linked stack (stack-allocated nodes) */
struct Node {
    int        value;
    struct Node *next;
};

struct Counter {
    int value;
    int step;
};

struct Pair {
    int first;
    int second;
};

/* --- Functions operating on struct pointers --- */

/* Increment counter by its step */
void tick(struct Counter *c) {
    c->value += c->step;
}

/* Reset counter */
void reset(struct Counter *c) {
    c->value = 0;
}

/* Swap the two fields of a Pair through pointer */
void swap_pair(struct Pair *p) {
    int tmp   = p->first;
    p->first  = p->second;
    p->second = tmp;
}

/* Sum pair fields */
int pair_sum(struct Pair *p) {
    return p->first + p->second;
}

/* Push value onto stack-allocated linked list;
   caller passes the address of the head pointer */
void push(struct Node **head, struct Node *node, int val) {
    node->value = val;
    node->next  = *head;
    *head       = node;
}

/* Pop value from stack (returns -1 if empty) */
int pop(struct Node **head) {
    if (*head == 0) return -1;
    int v  = (*head)->value;
    *head  = (*head)->next;
    return v;
}

/* Count nodes in list */
int list_len(struct Node *head) {
    int n = 0;
    while (head) {
        n++;
        head = head->next;
    }
    return n;
}

/* Sum all values in list */
int list_sum(struct Node *head) {
    int s = 0;
    while (head) {
        s   += head->value;
        head = head->next;
    }
    return s;
}

int main(void) {
    /* --- Basic pointer to struct --- */
    struct Counter c;
    c.value = 0;
    c.step  = 5;

    struct Counter *cp = &c;
    printf("%d\n", cp->value);              // EXPECT: 0
    printf("%d\n", cp->step);               // EXPECT: 5

    tick(cp);
    printf("%d\n", c.value);               // EXPECT: 5
    tick(cp);
    printf("%d\n", c.value);               // EXPECT: 10
    tick(cp);
    printf("%d\n", c.value);               // EXPECT: 15

    reset(cp);
    printf("%d\n", c.value);               // EXPECT: 0

    /* Change step through pointer */
    cp->step = 3;
    tick(cp);
    tick(cp);
    printf("%d\n", c.value);               // EXPECT: 6

    /* --- Pair via pointer --- */
    struct Pair p = {11, 22};
    printf("%d\n", pair_sum(&p));          // EXPECT: 33
    swap_pair(&p);
    printf("%d\n", p.first);              // EXPECT: 22
    printf("%d\n", p.second);             // EXPECT: 11

    /* --- Stack-based linked list --- */
    struct Node n1, n2, n3, n4, n5;
    struct Node *head = 0;   /* empty stack */

    push(&head, &n1, 10);
    push(&head, &n2, 20);
    push(&head, &n3, 30);
    push(&head, &n4, 40);
    push(&head, &n5, 50);

    printf("%d\n", list_len(head));        // EXPECT: 5
    /* 10+20+30+40+50 = 150 */
    printf("%d\n", list_sum(head));        // EXPECT: 150

    /* Head should be last pushed (50) */
    printf("%d\n", head->value);           // EXPECT: 50
    printf("%d\n", head->next->value);     // EXPECT: 40

    /* Pop all values (LIFO: 50 40 30 20 10) */
    printf("%d\n", pop(&head));            // EXPECT: 50
    printf("%d\n", pop(&head));            // EXPECT: 40
    printf("%d\n", pop(&head));            // EXPECT: 30
    printf("%d\n", list_len(head));        // EXPECT: 2
    printf("%d\n", pop(&head));            // EXPECT: 20
    printf("%d\n", pop(&head));            // EXPECT: 10
    printf("%d\n", pop(&head));            // EXPECT: -1

    /* --- Array of struct pointers --- */
    struct Pair pairs[4];
    struct Pair *ptrs[4];
    int i;
    for (i = 0; i < 4; i++) {
        pairs[i].first  = i * 10;
        pairs[i].second = i * 10 + 1;
        ptrs[i] = &pairs[i];
    }
    for (i = 0; i < 4; i++) {
        printf("%d %d\n", ptrs[i]->first, ptrs[i]->second);
    }
    // EXPECT: 0 1
    // EXPECT: 10 11
    // EXPECT: 20 21
    // EXPECT: 30 31

    /* Modify through pointer array */
    ptrs[2]->first = 999;
    printf("%d\n", pairs[2].first);       // EXPECT: 999

    /* --- Pointer arithmetic on struct pointer --- */
    struct Counter arr[3];
    arr[0].value = 1; arr[0].step = 1;
    arr[1].value = 2; arr[1].step = 2;
    arr[2].value = 3; arr[2].step = 3;

    struct Counter *cp2 = arr;
    tick(cp2);        /* arr[0].value = 2 */
    cp2++;
    tick(cp2);        /* arr[1].value = 4 */
    cp2++;
    tick(cp2);        /* arr[2].value = 6 */

    printf("%d\n", arr[0].value);         // EXPECT: 2
    printf("%d\n", arr[1].value);         // EXPECT: 4
    printf("%d\n", arr[2].value);         // EXPECT: 6

    return 0;
}
