int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// Heavy malloc usage: linked list, 2D array, struct array sort.

struct Node {
    int value;
    struct Node *next;
};

struct Node *create_list(int n) {
    struct Node *head = 0;
    int i;
    for (i = 0; i < n; i++) {
        struct Node *node = (struct Node *)malloc(sizeof(struct Node));
        node->value = i;
        node->next = head;
        head = node;
    }
    return head;
}

int list_length(struct Node *head) {
    int count = 0;
    struct Node *cur = head;
    while (cur) {
        count++;
        cur = cur->next;
    }
    return count;
}

int list_sum(struct Node *head) {
    int sum = 0;
    struct Node *cur = head;
    while (cur) {
        sum += cur->value;
        cur = cur->next;
    }
    return sum;
}

struct Node *reverse_list(struct Node *head) {
    struct Node *prev = 0;
    struct Node *cur = head;
    struct Node *next;
    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

int list_first(struct Node *head) {
    if (head) return head->value;
    return -1;
}

int list_last(struct Node *head) {
    struct Node *cur = head;
    while (cur && cur->next) {
        cur = cur->next;
    }
    if (cur) return cur->value;
    return -1;
}

void free_list(struct Node *head) {
    struct Node *cur = head;
    struct Node *next;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

// 2D array via malloc
int test_2d_array(void) {
    int **arr;
    int i, j;
    int sum = 0;

    arr = (int **)malloc(10 * sizeof(int *));
    for (i = 0; i < 10; i++) {
        arr[i] = (int *)malloc(10 * sizeof(int));
    }

    // Fill with i*10+j
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            arr[i][j] = i * 10 + j;
        }
    }

    // Sum diagonal
    for (i = 0; i < 10; i++) {
        sum += arr[i][i];
    }

    // Transpose in-place
    for (i = 0; i < 10; i++) {
        for (j = i + 1; j < 10; j++) {
            int tmp = arr[i][j];
            arr[i][j] = arr[j][i];
            arr[j][i] = tmp;
        }
    }

    // After transpose, arr[i][j] was arr[j][i] = j*10+i
    // Check arr[0][5] should be 50 (was arr[5][0]=50)
    int check = arr[0][5];

    for (i = 0; i < 10; i++) {
        free(arr[i]);
    }
    free(arr);

    return sum * 1000 + check;
}

// Struct array with sort
struct Item {
    int key;
    int value;
};

void sort_items(struct Item *items, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (items[j].key > items[j + 1].key) {
                struct Item tmp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = tmp;
            }
        }
    }
}

int test_struct_array(void) {
    int n = 20;
    struct Item *items = (struct Item *)malloc(n * sizeof(struct Item));
    int i;

    // Fill in reverse order
    for (i = 0; i < n; i++) {
        items[i].key = n - 1 - i;
        items[i].value = i * 100;
    }

    sort_items(items, n);

    // After sort, items[0].key=0, items[0].value should be 1900 (was at index 19)
    // items[19].key=19, items[19].value should be 0 (was at index 0)
    int result = items[0].key * 10000 + items[0].value * 10 + items[19].key;

    free(items);
    return result;
}

// Allocate chain: allocate, use, free, repeat
int test_alloc_chain(void) {
    int total = 0;
    int round;
    for (round = 0; round < 50; round++) {
        int *p = (int *)malloc(sizeof(int));
        *p = round * 3;
        total += *p;
        free(p);
    }
    // sum of round*3 for round=0..49 = 3 * (49*50/2) = 3*1225 = 3675
    return total;
}

// Multiple lists simultaneously
int test_multi_list(void) {
    struct Node *list1 = create_list(30);
    struct Node *list2 = create_list(20);
    struct Node *list3 = create_list(10);

    int s1 = list_sum(list1);  // 0+1+...+29 = 435
    int s2 = list_sum(list2);  // 0+1+...+19 = 190
    int s3 = list_sum(list3);  // 0+1+...+9 = 45

    free_list(list1);
    free_list(list2);
    free_list(list3);

    return s1 + s2 + s3;
}

int main(void) {
    // Create list of 100 nodes (values 0..99 inserted in order, head has 99)
    struct Node *list = create_list(100);

    printf("%d\n", list_length(list));
    // EXPECT: 100

    // sum 0..99 = 4950
    printf("%d\n", list_sum(list));
    // EXPECT: 4950

    // Head is 99 (last inserted), last is 0
    printf("%d\n", list_first(list));
    // EXPECT: 99
    printf("%d\n", list_last(list));
    // EXPECT: 0

    // Reverse: head becomes 0, last becomes 99
    list = reverse_list(list);
    printf("%d\n", list_first(list));
    // EXPECT: 0
    printf("%d\n", list_last(list));
    // EXPECT: 99

    // Sum unchanged
    printf("%d\n", list_sum(list));
    // EXPECT: 4950

    free_list(list);

    // 2D array test: diagonal sum = 0+11+22+33+44+55+66+77+88+99 = 495
    // result = 495*1000 + 50 = 495050
    printf("%d\n", test_2d_array());
    // EXPECT: 495050

    // Struct array: 0*10000 + 1900*10 + 19 = 19019
    printf("%d\n", test_struct_array());
    // EXPECT: 19019

    printf("%d\n", test_alloc_chain());
    // EXPECT: 3675

    // multi list: 435 + 190 + 45 = 670
    printf("%d\n", test_multi_list());
    // EXPECT: 670

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
