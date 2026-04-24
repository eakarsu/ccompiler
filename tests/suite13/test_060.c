int printf(const char *fmt, ...);

// Linked list sorting (insertion sort on list)

struct Node {
    int data;
    int next;
};

struct List {
    struct Node pool[30];
    int head;
    int free_idx;
    int size;
};

void list_init(struct List *l) {
    int i;
    l->head = -1;
    l->size = 0;
    l->free_idx = 0;
    for (i = 0; i < 29; i++) {
        l->pool[i].next = i + 1;
        l->pool[i].data = 0;
    }
    l->pool[29].next = -1;
    l->pool[29].data = 0;
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
        if (!first) printf(" ");
        printf("%d", l->pool[cur].data);
        first = 0;
        cur = l->pool[cur].next;
    }
    printf("\n");
}

int is_sorted(struct List *l) {
    int cur;
    int nxt;
    if (l->head == -1) return 1;
    cur = l->head;
    nxt = l->pool[cur].next;
    while (nxt != -1) {
        if (l->pool[cur].data > l->pool[nxt].data) return 0;
        cur = nxt;
        nxt = l->pool[cur].next;
    }
    return 1;
}

// Insertion sort on linked list
// Removes each node and inserts it into a new sorted list
void insertion_sort(struct List *l) {
    int sorted_head = -1;
    int cur = l->head;
    int next_node;
    int prev;
    int s;

    while (cur != -1) {
        next_node = l->pool[cur].next;

        // Insert cur into sorted list
        if (sorted_head == -1 || l->pool[cur].data <= l->pool[sorted_head].data) {
            l->pool[cur].next = sorted_head;
            sorted_head = cur;
        } else {
            s = sorted_head;
            while (l->pool[s].next != -1 && l->pool[l->pool[s].next].data < l->pool[cur].data) {
                s = l->pool[s].next;
            }
            l->pool[cur].next = l->pool[s].next;
            l->pool[s].next = cur;
        }

        cur = next_node;
    }

    l->head = sorted_head;
}

// Selection sort on linked list (swap data, not nodes)
void selection_sort(struct List *l) {
    int i_node = l->head;
    int j_node;
    int min_node;
    int temp;

    while (i_node != -1) {
        min_node = i_node;
        j_node = l->pool[i_node].next;
        while (j_node != -1) {
            if (l->pool[j_node].data < l->pool[min_node].data) {
                min_node = j_node;
            }
            j_node = l->pool[j_node].next;
        }
        if (min_node != i_node) {
            temp = l->pool[i_node].data;
            l->pool[i_node].data = l->pool[min_node].data;
            l->pool[min_node].data = temp;
        }
        i_node = l->pool[i_node].next;
    }
}

// Bubble sort on linked list (swap data)
void bubble_sort(struct List *l) {
    int swapped;
    int cur;
    int nxt;
    int temp;
    int end = -1; // optimization: last sorted position

    if (l->head == -1) return;

    do {
        swapped = 0;
        cur = l->head;
        nxt = l->pool[cur].next;
        while (nxt != end && nxt != -1) {
            if (l->pool[cur].data > l->pool[nxt].data) {
                temp = l->pool[cur].data;
                l->pool[cur].data = l->pool[nxt].data;
                l->pool[nxt].data = temp;
                swapped = 1;
            }
            cur = nxt;
            nxt = l->pool[cur].next;
        }
        end = cur;
    } while (swapped);
}

// Count inversions in the list
int count_inversions(struct List *l) {
    int count = 0;
    int i_node = l->head;
    int j_node;
    while (i_node != -1) {
        j_node = l->pool[i_node].next;
        while (j_node != -1) {
            if (l->pool[i_node].data > l->pool[j_node].data) {
                count++;
            }
            j_node = l->pool[j_node].next;
        }
        i_node = l->pool[i_node].next;
    }
    return count;
}

int main() {
    struct List l;
    int inversions;

    // Test 1: Insertion sort on random data
    list_init(&l);
    list_push_back(&l, 5);
    list_push_back(&l, 3);
    list_push_back(&l, 8);
    list_push_back(&l, 1);
    list_push_back(&l, 9);
    list_push_back(&l, 2);
    list_push_back(&l, 7);

    printf("Before insertion sort: ");
    list_print(&l);
    // EXPECT: Before insertion sort: 5 3 8 1 9 2 7

    printf("Sorted: %d\n", is_sorted(&l));
    // EXPECT: Sorted: 0

    inversions = count_inversions(&l);
    printf("Inversions: %d\n", inversions);
    // EXPECT: Inversions: 10

    insertion_sort(&l);
    printf("After insertion sort: ");
    list_print(&l);
    // EXPECT: After insertion sort: 1 2 3 5 7 8 9

    printf("Sorted: %d\n", is_sorted(&l));
    // EXPECT: Sorted: 1

    printf("Inversions: %d\n", count_inversions(&l));
    // EXPECT: Inversions: 0

    // Test 2: Selection sort
    list_init(&l);
    list_push_back(&l, 10);
    list_push_back(&l, 4);
    list_push_back(&l, 6);
    list_push_back(&l, 2);
    list_push_back(&l, 8);

    printf("Before selection sort: ");
    list_print(&l);
    // EXPECT: Before selection sort: 10 4 6 2 8

    selection_sort(&l);
    printf("After selection sort: ");
    list_print(&l);
    // EXPECT: After selection sort: 2 4 6 8 10

    printf("Sorted: %d\n", is_sorted(&l));
    // EXPECT: Sorted: 1

    // Test 3: Bubble sort
    list_init(&l);
    list_push_back(&l, 7);
    list_push_back(&l, 3);
    list_push_back(&l, 5);
    list_push_back(&l, 1);

    printf("Before bubble sort: ");
    list_print(&l);
    // EXPECT: Before bubble sort: 7 3 5 1

    bubble_sort(&l);
    printf("After bubble sort: ");
    list_print(&l);
    // EXPECT: After bubble sort: 1 3 5 7

    printf("Sorted: %d\n", is_sorted(&l));
    // EXPECT: Sorted: 1

    // Test 4: Already sorted
    list_init(&l);
    list_push_back(&l, 1);
    list_push_back(&l, 2);
    list_push_back(&l, 3);
    list_push_back(&l, 4);

    printf("Already sorted: %d\n", is_sorted(&l));
    // EXPECT: Already sorted: 1

    insertion_sort(&l);
    printf("After sort: ");
    list_print(&l);
    // EXPECT: After sort: 1 2 3 4

    // Test 5: Reverse sorted
    list_init(&l);
    list_push_back(&l, 5);
    list_push_back(&l, 4);
    list_push_back(&l, 3);
    list_push_back(&l, 2);
    list_push_back(&l, 1);

    inversions = count_inversions(&l);
    printf("Reverse inversions: %d\n", inversions);
    // EXPECT: Reverse inversions: 10

    insertion_sort(&l);
    printf("Reverse sorted: ");
    list_print(&l);
    // EXPECT: Reverse sorted: 1 2 3 4 5

    // Test 6: Duplicates
    list_init(&l);
    list_push_back(&l, 3);
    list_push_back(&l, 1);
    list_push_back(&l, 3);
    list_push_back(&l, 2);
    list_push_back(&l, 1);

    selection_sort(&l);
    printf("With duplicates: ");
    list_print(&l);
    // EXPECT: With duplicates: 1 1 2 3 3

    // Test 7: Single element
    list_init(&l);
    list_push_back(&l, 42);
    insertion_sort(&l);
    printf("Single: ");
    list_print(&l);
    // EXPECT: Single: 42

    printf("Single sorted: %d\n", is_sorted(&l));
    // EXPECT: Single sorted: 1

    return 0;
}
