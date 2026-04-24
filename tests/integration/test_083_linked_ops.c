int printf(const char *fmt, ...);
// EXPECT: 5 1 5 5 3 1 3 1 2 4 6 8 0 1 20 2 1 4 3 3 1 6 4 1 4 1 0 5 4 0 1 4 6 1 2 5 4 5 1 1 2 5 150 50 0 5 9 4 1 5

// Array-based linked list operations: merge sort, cycle detection, reverse k-groups

// Array-based linked list: val[i] is value, nxt[i] is next index, -1 = end
int val[200];
int nxt[200];
int node_count;

// Allocate a new node, return its index
int new_node(int v) {
    int idx;
    idx = node_count;
    val[idx] = v;
    nxt[idx] = -1;
    node_count = node_count + 1;
    return idx;
}

// Initialize linked list system
void list_init(void) {
    node_count = 0;
}

// Create a list from array, return head index
int list_from_array(int arr[], int n) {
    int head;
    int prev;
    int cur;
    int i;
    if (n == 0) return -1;
    head = new_node(arr[0]);
    prev = head;
    for (i = 1; i < n; i = i + 1) {
        cur = new_node(arr[i]);
        nxt[prev] = cur;
        prev = cur;
    }
    return head;
}

// Print list values
void list_print(int head) {
    int cur;
    int first;
    cur = head;
    first = 1;
    while (cur != -1) {
        if (first == 0) printf(" ");
        printf("%d", val[cur]);
        first = 0;
        cur = nxt[cur];
    }
}

// Count list length
int list_length(int head) {
    int count;
    int cur;
    count = 0;
    cur = head;
    while (cur != -1) {
        count = count + 1;
        cur = nxt[cur];
    }
    return count;
}

// Get nth value (0-indexed)
int list_get(int head, int n) {
    int cur;
    int i;
    cur = head;
    for (i = 0; i < n; i = i + 1) {
        cur = nxt[cur];
    }
    return val[cur];
}

// Reverse a linked list, return new head
int list_reverse(int head) {
    int prev;
    int cur;
    int next;
    prev = -1;
    cur = head;
    while (cur != -1) {
        next = nxt[cur];
        nxt[cur] = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

// Find middle node (slow-fast pointer)
int list_middle(int head) {
    int slow;
    int fast;
    slow = head;
    fast = head;
    while (fast != -1 && nxt[fast] != -1) {
        slow = nxt[slow];
        fast = nxt[nxt[fast]];
    }
    return slow;
}

// Merge two sorted lists, return head of merged
int merge_sorted(int h1, int h2) {
    int dummy;
    int tail;
    dummy = new_node(0);
    tail = dummy;
    while (h1 != -1 && h2 != -1) {
        if (val[h1] <= val[h2]) {
            nxt[tail] = h1;
            h1 = nxt[h1];
        } else {
            nxt[tail] = h2;
            h2 = nxt[h2];
        }
        tail = nxt[tail];
    }
    if (h1 != -1) nxt[tail] = h1;
    if (h2 != -1) nxt[tail] = h2;
    return nxt[dummy];
}

// Merge sort for linked list
int merge_sort(int head) {
    int slow;
    int fast;
    int mid;
    int left;
    int right;
    if (head == -1 || nxt[head] == -1) return head;
    slow = head;
    fast = nxt[head];
    while (fast != -1 && nxt[fast] != -1) {
        slow = nxt[slow];
        fast = nxt[nxt[fast]];
    }
    mid = nxt[slow];
    nxt[slow] = -1;
    left = merge_sort(head);
    right = merge_sort(mid);
    return merge_sorted(left, right);
}

// Floyd's cycle detection on array-based list
// Create cycle by connecting tail to a node
int detect_cycle(int head) {
    int slow;
    int fast;
    slow = head;
    fast = head;
    while (fast != -1 && nxt[fast] != -1) {
        slow = nxt[slow];
        fast = nxt[nxt[fast]];
        if (slow == fast) return 1;
    }
    return 0;
}

// Find cycle start node value (Floyd's)
int find_cycle_start(int head) {
    int slow;
    int fast;
    int meet;
    slow = head;
    fast = head;
    meet = -1;
    while (fast != -1 && nxt[fast] != -1) {
        slow = nxt[slow];
        fast = nxt[nxt[fast]];
        if (slow == fast) {
            meet = slow;
            break;
        }
    }
    if (meet == -1) return -1;
    slow = head;
    while (slow != meet) {
        slow = nxt[slow];
        meet = nxt[meet];
    }
    return val[slow];
}

// Reverse k-group: reverse every k nodes
int reverse_k_group(int head, int k) {
    int cur;
    int count;
    int prev;
    int next;
    int group_head;
    int group_tail;
    int new_head;
    int last_tail;
    int i;

    new_head = -1;
    last_tail = -1;
    cur = head;

    while (cur != -1) {
        // Check if k nodes remain
        count = 0;
        group_head = cur;
        while (cur != -1 && count < k) {
            cur = nxt[cur];
            count = count + 1;
        }
        if (count < k) {
            // Less than k nodes, don't reverse
            if (last_tail != -1) nxt[last_tail] = group_head;
            if (new_head == -1) new_head = group_head;
            break;
        }
        // Reverse k nodes starting from group_head
        prev = -1;
        cur = group_head;
        for (i = 0; i < k; i = i + 1) {
            next = nxt[cur];
            nxt[cur] = prev;
            prev = cur;
            cur = next;
        }
        // prev is new head of reversed group
        // group_head is now the tail of reversed group
        if (new_head == -1) new_head = prev;
        if (last_tail != -1) nxt[last_tail] = prev;
        last_tail = group_head;
        nxt[group_head] = -1; // Will be set in next iteration or remain -1
    }
    if (new_head == -1) new_head = head;
    return new_head;
}

// Remove duplicates from sorted list
int remove_duplicates(int head) {
    int cur;
    cur = head;
    while (cur != -1 && nxt[cur] != -1) {
        if (val[cur] == val[nxt[cur]]) {
            nxt[cur] = nxt[nxt[cur]];
        } else {
            cur = nxt[cur];
        }
    }
    return head;
}

// Check if list is palindrome (using array copy)
int is_palindrome(int head) {
    int arr2[50];
    int n;
    int i;
    int cur;
    n = 0;
    cur = head;
    while (cur != -1) {
        arr2[n] = val[cur];
        n = n + 1;
        cur = nxt[cur];
    }
    for (i = 0; i < n / 2; i = i + 1) {
        if (arr2[i] != arr2[n - 1 - i]) return 0;
    }
    return 1;
}

// Insert into sorted list
int insert_sorted(int head, int v) {
    int nd;
    int cur;
    int prev;
    nd = new_node(v);
    if (head == -1 || v <= val[head]) {
        nxt[nd] = head;
        return nd;
    }
    prev = head;
    cur = nxt[head];
    while (cur != -1 && val[cur] < v) {
        prev = cur;
        cur = nxt[cur];
    }
    nxt[prev] = nd;
    nxt[nd] = cur;
    return head;
}

// Interleave two lists: a1,b1,a2,b2,...
int interleave(int h1, int h2) {
    int c1;
    int c2;
    int n1;
    int n2;
    c1 = h1;
    c2 = h2;
    while (c1 != -1 && c2 != -1) {
        n1 = nxt[c1];
        n2 = nxt[c2];
        nxt[c1] = c2;
        if (n1 != -1) {
            nxt[c2] = n1;
        }
        c1 = n1;
        c2 = n2;
    }
    return h1;
}

// Rotate list by k positions
int rotate_list(int head, int k) {
    int len;
    int cur;
    int i;
    int new_head;
    len = list_length(head);
    if (len == 0 || k == 0) return head;
    k = k % len;
    if (k == 0) return head;
    cur = head;
    for (i = 0; i < len - k - 1; i = i + 1) {
        cur = nxt[cur];
    }
    new_head = nxt[cur];
    nxt[cur] = -1;
    cur = new_head;
    while (nxt[cur] != -1) {
        cur = nxt[cur];
    }
    nxt[cur] = head;
    return new_head;
}

// Partition list around value x
int partition_list(int head, int x) {
    int less_head;
    int less_tail;
    int great_head;
    int great_tail;
    int cur;
    int nd;
    less_head = -1;
    less_tail = -1;
    great_head = -1;
    great_tail = -1;
    cur = head;
    while (cur != -1) {
        nd = new_node(val[cur]);
        if (val[cur] < x) {
            if (less_head == -1) {
                less_head = nd;
                less_tail = nd;
            } else {
                nxt[less_tail] = nd;
                less_tail = nd;
            }
        } else {
            if (great_head == -1) {
                great_head = nd;
                great_tail = nd;
            } else {
                nxt[great_tail] = nd;
                great_tail = nd;
            }
        }
        cur = nxt[cur];
    }
    if (less_head == -1) return great_head;
    nxt[less_tail] = great_head;
    return less_head;
}

// Sum all values in list
int list_sum(int head) {
    int sum;
    int cur;
    sum = 0;
    cur = head;
    while (cur != -1) {
        sum = sum + val[cur];
        cur = nxt[cur];
    }
    return sum;
}

// Find max in list
int list_max(int head) {
    int mx;
    int cur;
    mx = val[head];
    cur = nxt[head];
    while (cur != -1) {
        if (val[cur] > mx) mx = val[cur];
        cur = nxt[cur];
    }
    return mx;
}

int main(void) {
    int arr[20];
    int head;
    int head2;
    int h;
    int i;
    int sorted_head;
    int tail_idx;

    // Test 1-3: Create list and check
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    head = list_from_array(arr, 5);
    printf("%d", list_length(head));  // 5
    printf(" %d", list_get(head, 0)); // 1
    printf(" %d", list_get(head, 4)); // 5

    // Test 4-6: Reverse list
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    head = list_from_array(arr, 5);
    head = list_reverse(head);
    printf(" %d", list_get(head, 0)); // 5
    printf(" %d", list_get(head, 2)); // 3
    printf(" %d", list_get(head, 4)); // 1

    // Test 7: Middle node
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    head = list_from_array(arr, 5);
    printf(" %d", val[list_middle(head)]); // 3

    // Test 8-12: Merge sort
    list_init();
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=4;
    arr[5]=2; arr[6]=7; arr[7]=6;
    head = list_from_array(arr, 8);
    head = merge_sort(head);
    printf(" %d", list_get(head, 0)); // 1
    printf(" %d", list_get(head, 1)); // 2
    printf(" %d", list_get(head, 3)); // 4
    printf(" %d", list_get(head, 5)); // 6
    printf(" %d", list_get(head, 7)); // 8

    // Test 13-14: Cycle detection - no cycle
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3;
    head = list_from_array(arr, 3);
    printf(" %d", detect_cycle(head)); // 0

    // With cycle: 1->2->3->4->2(cycle)
    list_init();
    arr[0]=10; arr[1]=20; arr[2]=30; arr[3]=40;
    head = list_from_array(arr, 4);
    // Connect node 3 (idx 3) back to node 1 (idx 1)
    nxt[3] = 1;
    printf(" %d", detect_cycle(head)); // 1

    // Test 15: Find cycle start
    printf(" %d", find_cycle_start(head)); // 20

    // Test 16-19: Reverse k-group (k=2)
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5; arr[5]=6;
    head = list_from_array(arr, 6);
    head = reverse_k_group(head, 2);
    // 2,1,4,3,6,5
    printf(" %d", list_get(head, 0)); // 2
    printf(" %d", list_get(head, 1)); // 1
    printf(" %d", list_get(head, 2)); // 4
    printf(" %d", list_get(head, 3)); // 3

    // Test 20-22: Reverse k-group (k=3)
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5; arr[5]=6;
    head = list_from_array(arr, 6);
    head = reverse_k_group(head, 3);
    // 3,2,1,6,5,4
    printf(" %d", list_get(head, 0)); // 3
    printf(" %d", list_get(head, 2)); // 1
    printf(" %d", list_get(head, 3)); // 6

    // Test 23-25: Remove duplicates
    list_init();
    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=3; arr[4]=3; arr[5]=3; arr[6]=4;
    head = list_from_array(arr, 7);
    head = remove_duplicates(head);
    printf(" %d", list_length(head)); // 4
    printf(" %d", list_get(head, 0)); // 1
    printf(" %d", list_get(head, 3)); // 4

    // Test 26-27: Palindrome check
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=1;
    head = list_from_array(arr, 5);
    printf(" %d", is_palindrome(head)); // 1

    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3;
    head = list_from_array(arr, 3);
    printf(" %d", is_palindrome(head)); // 0

    // Test 28-30: Insert into sorted
    list_init();
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7;
    head = list_from_array(arr, 4);
    head = insert_sorted(head, 4);
    printf(" %d", list_length(head)); // 5
    printf(" %d", list_get(head, 2)); // 4
    head = insert_sorted(head, 0);
    printf(" %d", list_get(head, 0)); // 0

    // Test 31-33: Merge two sorted lists
    list_init();
    arr[0]=1; arr[1]=3; arr[2]=5;
    head = list_from_array(arr, 3);
    arr[0]=2; arr[1]=4; arr[2]=6;
    head2 = list_from_array(arr, 3);
    h = merge_sorted(head, head2);
    printf(" %d", list_get(h, 0)); // 1
    printf(" %d", list_get(h, 3)); // 4
    printf(" %d", list_get(h, 5)); // 6

    // Test 34-36: Interleave
    list_init();
    arr[0]=1; arr[1]=3; arr[2]=5;
    head = list_from_array(arr, 3);
    arr[0]=2; arr[1]=4; arr[2]=6;
    head2 = list_from_array(arr, 3);
    h = interleave(head, head2);
    printf(" %d", list_get(h, 0)); // 1
    printf(" %d", list_get(h, 1)); // 2
    printf(" %d", list_get(h, 4)); // 5

    // Test 37-39: Rotate list
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    head = list_from_array(arr, 5);
    head = rotate_list(head, 2);
    // rotate right by 2: 4,5,1,2,3
    printf(" %d", list_get(head, 0)); // 4
    printf(" %d", list_get(head, 1)); // 5
    printf(" %d", list_get(head, 2)); // 1

    // Test 40-42: Partition around 3
    list_init();
    arr[0]=5; arr[1]=1; arr[2]=4; arr[3]=2; arr[4]=3;
    head = list_from_array(arr, 5);
    head = partition_list(head, 3);
    // less than 3: 1,2 then >=3: 5,4,3
    printf(" %d", list_get(head, 0)); // 1
    printf(" %d", list_get(head, 1)); // 2
    printf(" %d", list_get(head, 2)); // 5

    // Test 43-44: Sum and max
    list_init();
    arr[0]=10; arr[1]=20; arr[2]=30; arr[3]=40; arr[4]=50;
    head = list_from_array(arr, 5);
    printf(" %d", list_sum(head)); // 150
    printf(" %d", list_max(head)); // 50

    // Test 45-47: Large merge sort
    list_init();
    arr[0]=9; arr[1]=7; arr[2]=5; arr[3]=3; arr[4]=1;
    arr[5]=8; arr[6]=6; arr[7]=4; arr[8]=2; arr[9]=0;
    head = list_from_array(arr, 10);
    head = merge_sort(head);
    printf(" %d", list_get(head, 0)); // 0
    printf(" %d", list_get(head, 5)); // 5
    printf(" %d", list_get(head, 9)); // 9

    // Test 48-50: Reverse k=4
    list_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    arr[4]=5; arr[5]=6; arr[6]=7; arr[7]=8;
    head = list_from_array(arr, 8);
    head = reverse_k_group(head, 4);
    // 4,3,2,1,8,7,6,5
    printf(" %d", list_get(head, 0)); // 4
    printf(" %d", list_get(head, 3)); // 1
    printf(" %d", list_get(head, 7)); // 5

    printf("\n");
    return 0;
}
