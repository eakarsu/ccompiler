int printf(const char *fmt, ...);

// Linked list reversal (iterative and recursive simulation)

struct Node {
    int data;
    int next;
};

struct List {
    struct Node pool[20];
    int head;
    int free_idx;
    int size;
};

void list_init(struct List *l) {
    int i;
    l->head = -1;
    l->size = 0;
    l->free_idx = 0;
    for (i = 0; i < 19; i++) {
        l->pool[i].next = i + 1;
        l->pool[i].data = 0;
    }
    l->pool[19].next = -1;
    l->pool[19].data = 0;
}

int list_alloc(struct List *l) {
    int idx;
    if (l->free_idx == -1) return -1;
    idx = l->free_idx;
    l->free_idx = l->pool[idx].next;
    l->pool[idx].next = -1;
    return idx;
}

void list_push_back(struct List *l, int val) {
    int idx = list_alloc(l);
    int cur;
    if (idx == -1) return;
    l->pool[idx].data = val;
    l->pool[idx].next = -1;
    if (l->head == -1) {
        l->head = idx;
    } else {
        cur = l->head;
        while (l->pool[cur].next != -1) {
            cur = l->pool[cur].next;
        }
        l->pool[cur].next = idx;
    }
    l->size++;
}

void list_print(struct List *l) {
    int cur = l->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" -> ");
        printf("%d", l->pool[cur].data);
        first = 0;
        cur = l->pool[cur].next;
    }
    printf("\n");
}

// Iterative reversal: reverse all next pointers
void list_reverse_iterative(struct List *l) {
    int prev = -1;
    int cur = l->head;
    int next_node;
    while (cur != -1) {
        next_node = l->pool[cur].next;
        l->pool[cur].next = prev;
        prev = cur;
        cur = next_node;
    }
    l->head = prev;
}

// Simulated recursive reversal using an explicit stack
void list_reverse_recursive_sim(struct List *l) {
    int stack[20];
    int sp = 0;
    int cur;
    int i;

    // Push all node indices onto stack
    cur = l->head;
    while (cur != -1) {
        stack[sp] = cur;
        sp++;
        cur = l->pool[cur].next;
    }

    if (sp == 0) return;

    // Rebuild links in reverse order
    l->head = stack[sp - 1];
    for (i = sp - 1; i > 0; i--) {
        l->pool[stack[i]].next = stack[i - 1];
    }
    l->pool[stack[0]].next = -1;
}

// Reverse only a sublist from position start to position end (0-indexed)
void list_reverse_sublist(struct List *l, int start, int end) {
    int values[20];
    int indices[20];
    int count = 0;
    int cur;
    int i;
    int j;
    int temp;

    // Collect node indices
    cur = l->head;
    while (cur != -1) {
        indices[count] = cur;
        values[count] = l->pool[cur].data;
        count++;
        cur = l->pool[cur].next;
    }

    // Reverse data values in range [start, end]
    i = start;
    j = end;
    while (i < j) {
        temp = values[i];
        values[i] = values[j];
        values[j] = temp;
        i++;
        j--;
    }

    // Write back values
    for (i = 0; i < count; i++) {
        l->pool[indices[i]].data = values[i];
    }
}

int list_length(struct List *l) {
    int count = 0;
    int cur = l->head;
    while (cur != -1) {
        count++;
        cur = l->pool[cur].next;
    }
    return count;
}

int list_get(struct List *l, int index) {
    int cur = l->head;
    int i;
    for (i = 0; i < index; i++) {
        if (cur == -1) return -999;
        cur = l->pool[cur].next;
    }
    if (cur == -1) return -999;
    return l->pool[cur].data;
}

int main() {
    struct List l;
    int i;

    list_init(&l);

    // Build list: 1 -> 2 -> 3 -> 4 -> 5
    for (i = 1; i <= 5; i++) {
        list_push_back(&l, i);
    }

    printf("Original: ");
    list_print(&l);
    // EXPECT: Original: 1 -> 2 -> 3 -> 4 -> 5

    printf("Length: %d\n", list_length(&l));
    // EXPECT: Length: 5

    // Iterative reversal
    list_reverse_iterative(&l);
    printf("After iterative reverse: ");
    list_print(&l);
    // EXPECT: After iterative reverse: 5 -> 4 -> 3 -> 2 -> 1

    // Reverse again using simulated recursion
    list_reverse_recursive_sim(&l);
    printf("After recursive-sim reverse: ");
    list_print(&l);
    // EXPECT: After recursive-sim reverse: 1 -> 2 -> 3 -> 4 -> 5

    // Reverse sublist [1, 3] => positions 1,2,3 reversed
    // Before: 1 -> 2 -> 3 -> 4 -> 5
    // After:  1 -> 4 -> 3 -> 2 -> 5
    list_reverse_sublist(&l, 1, 3);
    printf("After sublist reverse [1,3]: ");
    list_print(&l);
    // EXPECT: After sublist reverse [1,3]: 1 -> 4 -> 3 -> 2 -> 5

    // Double reverse should restore
    list_reverse_iterative(&l);
    printf("Reversed: ");
    list_print(&l);
    // EXPECT: Reversed: 5 -> 2 -> 3 -> 4 -> 1

    list_reverse_iterative(&l);
    printf("Double reversed: ");
    list_print(&l);
    // EXPECT: Double reversed: 1 -> 4 -> 3 -> 2 -> 5

    // Reverse entire thing
    list_reverse_sublist(&l, 0, 4);
    printf("Full sublist reverse: ");
    list_print(&l);
    // EXPECT: Full sublist reverse: 5 -> 2 -> 3 -> 4 -> 1

    // Test element access
    printf("Element 0: %d\n", list_get(&l, 0));
    // EXPECT: Element 0: 5

    printf("Element 4: %d\n", list_get(&l, 4));
    // EXPECT: Element 4: 1

    printf("Element 2: %d\n", list_get(&l, 2));
    // EXPECT: Element 2: 3

    // Reverse empty list
    {
        struct List empty;
        list_init(&empty);
        list_reverse_iterative(&empty);
        printf("Empty reversed length: %d\n", list_length(&empty));
        // EXPECT: Empty reversed length: 0
    }

    // Reverse single element
    {
        struct List single;
        list_init(&single);
        list_push_back(&single, 42);
        list_reverse_iterative(&single);
        printf("Single reversed: ");
        list_print(&single);
        // EXPECT: Single reversed: 42
    }

    return 0;
}
