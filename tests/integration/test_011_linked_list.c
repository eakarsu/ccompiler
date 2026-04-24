int printf(const char *fmt, ...);
// EXPECT: insert 10: 1\ninsert 20: 1\ninsert 30: 1\ninsert 40: 1\ninsert 50: 1\ncount: 5\nfind 10: 1\nfind 30: 1\nfind 50: 1\nfind 99: 0\nget 0: 10\nget 1: 20\nget 2: 30\nget 3: 40\nget 4: 50\ndelete 30: 1\ncount after del: 4\nfind 30 after del: 0\nget 0: 10\nget 1: 20\nget 2: 40\nget 3: 50\nreverse done\nget 0: 50\nget 1: 40\nget 2: 20\nget 3: 10\ninsert front 5: 1\nget 0: 5\ncount: 5\ndelete head 5: 1\ncount: 4\nget 0: 50\ndelete tail 10: 1\ncount: 3\nget 0: 50\nget 1: 40\nget 2: 20\nclear done\ncount: 0\nfind 50: 0\nbuild sorted: done\nsorted 0: 2\nsorted 1: 5\nsorted 2: 8\nsorted 3: 11\nsorted 4: 14\nis sorted: 1\nsum: 40\nmin: 2\nmax: 14\nnth from end 0: 14\nnth from end 2: 8

// ============================================================
// test_011_linked_list.c
// Linked list using struct array pool (no malloc)
// ============================================================

struct Node {
    int data;
    int next; // index into pool, -1 = null
    int used;
};

struct Node pool[100];
int head;
int pool_size;

// -----------------------------------------------------------
// Initialize the pool
// -----------------------------------------------------------
void pool_init(void) {
    int i;
    for (i = 0; i < 100; i = i + 1) {
        pool[i].data = 0;
        pool[i].next = -1;
        pool[i].used = 0;
    }
    head = -1;
    pool_size = 0;
}

// -----------------------------------------------------------
// Allocate a node from pool
// -----------------------------------------------------------
int pool_alloc(void) {
    int i;
    for (i = 0; i < 100; i = i + 1) {
        if (pool[i].used == 0) {
            pool[i].used = 1;
            pool[i].next = -1;
            pool_size = pool_size + 1;
            return i;
        }
    }
    return -1;
}

// -----------------------------------------------------------
// Free a node back to pool
// -----------------------------------------------------------
void pool_free(int idx) {
    if (idx >= 0 && idx < 100) {
        pool[idx].used = 0;
        pool[idx].data = 0;
        pool[idx].next = -1;
        pool_size = pool_size - 1;
    }
}

// -----------------------------------------------------------
// Insert at tail
// -----------------------------------------------------------
int insert_tail(int val) {
    int n;
    int cur;
    n = pool_alloc();
    if (n == -1) {
        return 0;
    }
    pool[n].data = val;
    pool[n].next = -1;
    if (head == -1) {
        head = n;
        return 1;
    }
    cur = head;
    while (pool[cur].next != -1) {
        cur = pool[cur].next;
    }
    pool[cur].next = n;
    return 1;
}

// -----------------------------------------------------------
// Insert at front
// -----------------------------------------------------------
int insert_front(int val) {
    int n;
    n = pool_alloc();
    if (n == -1) {
        return 0;
    }
    pool[n].data = val;
    pool[n].next = head;
    head = n;
    return 1;
}

// -----------------------------------------------------------
// Count nodes
// -----------------------------------------------------------
int count_nodes(void) {
    int cnt;
    int cur;
    cnt = 0;
    cur = head;
    while (cur != -1) {
        cnt = cnt + 1;
        cur = pool[cur].next;
    }
    return cnt;
}

// -----------------------------------------------------------
// Find a value, return 1 if found, 0 otherwise
// -----------------------------------------------------------
int find_val(int val) {
    int cur;
    cur = head;
    while (cur != -1) {
        if (pool[cur].data == val) {
            return 1;
        }
        cur = pool[cur].next;
    }
    return 0;
}

// -----------------------------------------------------------
// Get value at index
// -----------------------------------------------------------
int get_at(int idx) {
    int cur;
    int i;
    cur = head;
    i = 0;
    while (cur != -1) {
        if (i == idx) {
            return pool[cur].data;
        }
        i = i + 1;
        cur = pool[cur].next;
    }
    return -999;
}

// -----------------------------------------------------------
// Delete by value
// -----------------------------------------------------------
int delete_val(int val) {
    int cur;
    int prev;
    if (head == -1) {
        return 0;
    }
    if (pool[head].data == val) {
        cur = head;
        head = pool[head].next;
        pool_free(cur);
        return 1;
    }
    prev = head;
    cur = pool[head].next;
    while (cur != -1) {
        if (pool[cur].data == val) {
            pool[prev].next = pool[cur].next;
            pool_free(cur);
            return 1;
        }
        prev = cur;
        cur = pool[cur].next;
    }
    return 0;
}

// -----------------------------------------------------------
// Reverse the list
// -----------------------------------------------------------
void reverse_list(void) {
    int prev;
    int cur;
    int nxt;
    prev = -1;
    cur = head;
    while (cur != -1) {
        nxt = pool[cur].next;
        pool[cur].next = prev;
        prev = cur;
        cur = nxt;
    }
    head = prev;
}

// -----------------------------------------------------------
// Clear the entire list
// -----------------------------------------------------------
void clear_list(void) {
    int cur;
    int nxt;
    cur = head;
    while (cur != -1) {
        nxt = pool[cur].next;
        pool_free(cur);
        cur = nxt;
    }
    head = -1;
}

// -----------------------------------------------------------
// Insert in sorted order (ascending)
// -----------------------------------------------------------
int insert_sorted(int val) {
    int n;
    int cur;
    int prev;
    n = pool_alloc();
    if (n == -1) {
        return 0;
    }
    pool[n].data = val;
    if (head == -1 || pool[head].data >= val) {
        pool[n].next = head;
        head = n;
        return 1;
    }
    prev = head;
    cur = pool[head].next;
    while (cur != -1 && pool[cur].data < val) {
        prev = cur;
        cur = pool[cur].next;
    }
    pool[n].next = cur;
    pool[prev].next = n;
    return 1;
}

// -----------------------------------------------------------
// Check if sorted ascending
// -----------------------------------------------------------
int is_sorted(void) {
    int cur;
    if (head == -1) {
        return 1;
    }
    cur = head;
    while (pool[cur].next != -1) {
        if (pool[cur].data > pool[pool[cur].next].data) {
            return 0;
        }
        cur = pool[cur].next;
    }
    return 1;
}

// -----------------------------------------------------------
// Sum of all node values
// -----------------------------------------------------------
int sum_list(void) {
    int cur;
    int s;
    s = 0;
    cur = head;
    while (cur != -1) {
        s = s + pool[cur].data;
        cur = pool[cur].next;
    }
    return s;
}

// -----------------------------------------------------------
// Minimum value
// -----------------------------------------------------------
int min_list(void) {
    int cur;
    int m;
    if (head == -1) {
        return -999;
    }
    m = pool[head].data;
    cur = pool[head].next;
    while (cur != -1) {
        if (pool[cur].data < m) {
            m = pool[cur].data;
        }
        cur = pool[cur].next;
    }
    return m;
}

// -----------------------------------------------------------
// Maximum value
// -----------------------------------------------------------
int max_list(void) {
    int cur;
    int m;
    if (head == -1) {
        return -999;
    }
    m = pool[head].data;
    cur = pool[head].next;
    while (cur != -1) {
        if (pool[cur].data > m) {
            m = pool[cur].data;
        }
        cur = pool[cur].next;
    }
    return m;
}

// -----------------------------------------------------------
// Nth node from the end (0-indexed)
// -----------------------------------------------------------
int nth_from_end(int n) {
    int len;
    int idx;
    len = count_nodes();
    if (n >= len) {
        return -999;
    }
    idx = len - 1 - n;
    return get_at(idx);
}

// -----------------------------------------------------------
// Print helper: print list elements
// -----------------------------------------------------------
void print_list(void) {
    int cur;
    cur = head;
    while (cur != -1) {
        printf("%d ", pool[cur].data);
        cur = pool[cur].next;
    }
    printf("%c", 10);
}

// -----------------------------------------------------------
// Insert at position
// -----------------------------------------------------------
int insert_at(int pos, int val) {
    int n;
    int cur;
    int prev;
    int i;
    if (pos == 0) {
        return insert_front(val);
    }
    n = pool_alloc();
    if (n == -1) {
        return 0;
    }
    pool[n].data = val;
    cur = head;
    prev = -1;
    i = 0;
    while (cur != -1 && i < pos) {
        prev = cur;
        cur = pool[cur].next;
        i = i + 1;
    }
    if (prev == -1) {
        pool_free(n);
        return 0;
    }
    pool[n].next = cur;
    pool[prev].next = n;
    return 1;
}

// -----------------------------------------------------------
// Remove duplicates from sorted list
// -----------------------------------------------------------
void remove_dups_sorted(void) {
    int cur;
    int nxt;
    cur = head;
    while (cur != -1 && pool[cur].next != -1) {
        if (pool[cur].data == pool[pool[cur].next].data) {
            nxt = pool[cur].next;
            pool[cur].next = pool[nxt].next;
            pool_free(nxt);
        } else {
            cur = pool[cur].next;
        }
    }
}

// -----------------------------------------------------------
// Count occurrences of a value
// -----------------------------------------------------------
int count_val(int val) {
    int cur;
    int cnt;
    cnt = 0;
    cur = head;
    while (cur != -1) {
        if (pool[cur].data == val) {
            cnt = cnt + 1;
        }
        cur = pool[cur].next;
    }
    return cnt;
}

// -----------------------------------------------------------
// Swap data of two adjacent nodes at index i and i+1
// -----------------------------------------------------------
int swap_adjacent(int i) {
    int cur;
    int j;
    int tmp;
    cur = head;
    j = 0;
    while (cur != -1 && j < i) {
        cur = pool[cur].next;
        j = j + 1;
    }
    if (cur == -1 || pool[cur].next == -1) {
        return 0;
    }
    tmp = pool[cur].data;
    pool[cur].data = pool[pool[cur].next].data;
    pool[pool[cur].next].data = tmp;
    return 1;
}

// -----------------------------------------------------------
// Bubble sort the list by swapping data
// -----------------------------------------------------------
void sort_list(void) {
    int n;
    int i;
    int cur;
    int tmp;
    int swapped;
    n = count_nodes();
    if (n <= 1) {
        return;
    }
    swapped = 1;
    while (swapped) {
        swapped = 0;
        cur = head;
        while (cur != -1 && pool[cur].next != -1) {
            if (pool[cur].data > pool[pool[cur].next].data) {
                tmp = pool[cur].data;
                pool[cur].data = pool[pool[cur].next].data;
                pool[pool[cur].next].data = tmp;
                swapped = 1;
            }
            cur = pool[cur].next;
        }
    }
}

// -----------------------------------------------------------
// MAIN
// -----------------------------------------------------------
int main(void) {
    int r;
    int i;

    pool_init();

    // Insert 5 elements at tail
    r = insert_tail(10);
    printf("insert 10: %d\n", r);
    r = insert_tail(20);
    printf("insert 20: %d\n", r);
    r = insert_tail(30);
    printf("insert 30: %d\n", r);
    r = insert_tail(40);
    printf("insert 40: %d\n", r);
    r = insert_tail(50);
    printf("insert 50: %d\n", r);

    // Count
    printf("count: %d\n", count_nodes());

    // Find
    printf("find 10: %d\n", find_val(10));
    printf("find 30: %d\n", find_val(30));
    printf("find 50: %d\n", find_val(50));
    printf("find 99: %d\n", find_val(99));

    // Get by index
    printf("get 0: %d\n", get_at(0));
    printf("get 1: %d\n", get_at(1));
    printf("get 2: %d\n", get_at(2));
    printf("get 3: %d\n", get_at(3));
    printf("get 4: %d\n", get_at(4));

    // Delete 30
    r = delete_val(30);
    printf("delete 30: %d\n", r);
    printf("count after del: %d\n", count_nodes());
    printf("find 30 after del: %d\n", find_val(30));
    printf("get 0: %d\n", get_at(0));
    printf("get 1: %d\n", get_at(1));
    printf("get 2: %d\n", get_at(2));
    printf("get 3: %d\n", get_at(3));

    // Reverse: 10 20 40 50 -> 50 40 20 10
    reverse_list();
    printf("reverse done\n");
    printf("get 0: %d\n", get_at(0));
    printf("get 1: %d\n", get_at(1));
    printf("get 2: %d\n", get_at(2));
    printf("get 3: %d\n", get_at(3));

    // Insert at front
    r = insert_front(5);
    printf("insert front 5: %d\n", r);
    printf("get 0: %d\n", get_at(0));
    printf("count: %d\n", count_nodes());

    // Delete head (5)
    r = delete_val(5);
    printf("delete head 5: %d\n", r);
    printf("count: %d\n", count_nodes());
    printf("get 0: %d\n", get_at(0));

    // Delete tail (10) - list is 50 40 20 10
    r = delete_val(10);
    printf("delete tail 10: %d\n", r);
    printf("count: %d\n", count_nodes());
    // list is now 50 40 20
    printf("get 0: %d\n", get_at(0));
    printf("get 1: %d\n", get_at(1));
    printf("get 2: %d\n", get_at(2));

    // Clear
    clear_list();
    printf("clear done\n");
    printf("count: %d\n", count_nodes());
    printf("find 50: %d\n", find_val(50));

    // Build sorted list
    insert_sorted(8);
    insert_sorted(2);
    insert_sorted(14);
    insert_sorted(5);
    insert_sorted(11);
    printf("build sorted: done\n");
    printf("sorted 0: %d\n", get_at(0));
    printf("sorted 1: %d\n", get_at(1));
    printf("sorted 2: %d\n", get_at(2));
    printf("sorted 3: %d\n", get_at(3));
    printf("sorted 4: %d\n", get_at(4));
    printf("is sorted: %d\n", is_sorted());
    printf("sum: %d\n", sum_list());
    printf("min: %d\n", min_list());
    printf("max: %d\n", max_list());
    printf("nth from end 0: %d\n", nth_from_end(0));
    printf("nth from end 2: %d\n", nth_from_end(2));

    return 0;
}
