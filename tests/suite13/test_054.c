int printf(const char *fmt, ...);

// Merge two sorted linked lists

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

int list_length(struct List *l) {
    int count = 0;
    int cur = l->head;
    while (cur != -1) {
        count++;
        cur = l->pool[cur].next;
    }
    return count;
}

// Merge two sorted lists from separate pools into result list
// a and b are separate lists; result gets the merged data
void merge_sorted(struct List *a, struct List *b, struct List *result) {
    int ca = a->head;
    int cb = b->head;

    while (ca != -1 && cb != -1) {
        if (a->pool[ca].data <= b->pool[cb].data) {
            list_push_back(result, a->pool[ca].data);
            ca = a->pool[ca].next;
        } else {
            list_push_back(result, b->pool[cb].data);
            cb = b->pool[cb].next;
        }
    }
    while (ca != -1) {
        list_push_back(result, a->pool[ca].data);
        ca = a->pool[ca].next;
    }
    while (cb != -1) {
        list_push_back(result, b->pool[cb].data);
        cb = b->pool[cb].next;
    }
}

// Check if a list is sorted in non-decreasing order
int is_sorted(struct List *l) {
    int cur;
    int next_idx;
    if (l->head == -1) return 1;
    cur = l->head;
    next_idx = l->pool[cur].next;
    while (next_idx != -1) {
        if (l->pool[cur].data > l->pool[next_idx].data) return 0;
        cur = next_idx;
        next_idx = l->pool[cur].next;
    }
    return 1;
}

// Merge sort on a single list (using data array, not pointer manipulation)
void list_to_array(struct List *l, int *arr, int *n) {
    int cur = l->head;
    *n = 0;
    while (cur != -1) {
        arr[*n] = l->pool[cur].data;
        (*n)++;
        cur = l->pool[cur].next;
    }
}

void merge_arrays(int *arr, int left, int mid, int right) {
    int temp[30];
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort_arr(int *arr, int left, int right) {
    int mid;
    if (left >= right) return;
    mid = left + (right - left) / 2;
    merge_sort_arr(arr, left, mid);
    merge_sort_arr(arr, mid + 1, right);
    merge_arrays(arr, left, mid, right);
}

void array_to_list(struct List *l, int *arr, int n) {
    int i;
    list_init(l);
    for (i = 0; i < n; i++) {
        list_push_back(l, arr[i]);
    }
}

int main() {
    struct List a;
    struct List b;
    struct List merged;
    int arr[30];
    int n;

    // Test 1: Merge two sorted lists
    list_init(&a);
    list_init(&b);
    list_init(&merged);

    list_push_back(&a, 1);
    list_push_back(&a, 3);
    list_push_back(&a, 5);
    list_push_back(&a, 7);

    list_push_back(&b, 2);
    list_push_back(&b, 4);
    list_push_back(&b, 6);
    list_push_back(&b, 8);

    printf("List A: ");
    list_print(&a);
    // EXPECT: List A: 1 3 5 7

    printf("List B: ");
    list_print(&b);
    // EXPECT: List B: 2 4 6 8

    merge_sorted(&a, &b, &merged);
    printf("Merged: ");
    list_print(&merged);
    // EXPECT: Merged: 1 2 3 4 5 6 7 8

    printf("Merged sorted: %d\n", is_sorted(&merged));
    // EXPECT: Merged sorted: 1

    printf("Merged length: %d\n", list_length(&merged));
    // EXPECT: Merged length: 8

    // Test 2: Merge with duplicates
    list_init(&a);
    list_init(&b);
    list_init(&merged);

    list_push_back(&a, 1);
    list_push_back(&a, 3);
    list_push_back(&a, 3);
    list_push_back(&a, 5);

    list_push_back(&b, 2);
    list_push_back(&b, 3);
    list_push_back(&b, 4);

    merge_sorted(&a, &b, &merged);
    printf("Merge with dups: ");
    list_print(&merged);
    // EXPECT: Merge with dups: 1 2 3 3 3 4 5

    printf("Sorted: %d\n", is_sorted(&merged));
    // EXPECT: Sorted: 1

    // Test 3: Merge with empty list
    list_init(&a);
    list_init(&b);
    list_init(&merged);

    list_push_back(&a, 10);
    list_push_back(&a, 20);
    // b is empty

    merge_sorted(&a, &b, &merged);
    printf("Merge with empty: ");
    list_print(&merged);
    // EXPECT: Merge with empty: 10 20

    // Test 4: Merge sort on unsorted list
    list_init(&a);
    list_push_back(&a, 5);
    list_push_back(&a, 2);
    list_push_back(&a, 8);
    list_push_back(&a, 1);
    list_push_back(&a, 9);
    list_push_back(&a, 3);

    printf("Unsorted: ");
    list_print(&a);
    // EXPECT: Unsorted: 5 2 8 1 9 3

    printf("Is sorted: %d\n", is_sorted(&a));
    // EXPECT: Is sorted: 0

    list_to_array(&a, arr, &n);
    merge_sort_arr(arr, 0, n - 1);
    array_to_list(&a, arr, n);

    printf("After merge sort: ");
    list_print(&a);
    // EXPECT: After merge sort: 1 2 3 5 8 9

    printf("Is sorted: %d\n", is_sorted(&a));
    // EXPECT: Is sorted: 1

    return 0;
}
