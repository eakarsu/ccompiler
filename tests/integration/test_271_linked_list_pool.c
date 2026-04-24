int printf(const char *fmt, ...);
// EXPECT: after push_back: 10 -> 20 -> 30\nafter push_front: 1 -> 5 -> 10 -> 20 -> 30\nlength = 5\nsum = 66\nfind 20 = 3\nfind 99 = -1\nfinal: 1 -> 5 -> 10 -> 20 -> 30 -> 40 -> 50\nlength = 7
// Test: pointer-based linked list using array pool

struct Node {
    int data;
    int next;  // index into pool, -1 = null
};

struct Node pool[20];
int pool_top = 0;
int head = -1;

int alloc_node(int val) {
    int idx = pool_top;
    pool_top = pool_top + 1;
    pool[idx].data = val;
    pool[idx].next = -1;
    return idx;
}

void push_front(int val) {
    int idx = alloc_node(val);
    pool[idx].next = head;
    head = idx;
}

void push_back(int val) {
    int idx = alloc_node(val);
    if (head == -1) {
        head = idx;
        return;
    }
    int cur = head;
    while (pool[cur].next != -1) {
        cur = pool[cur].next;
    }
    pool[cur].next = idx;
}

void print_list(void) {
    int cur = head;
    while (cur != -1) {
        printf("%d", pool[cur].data);
        if (pool[cur].next != -1) printf(" -> ");
        cur = pool[cur].next;
    }
    printf("\n");
}

int list_length(void) {
    int cnt = 0;
    int cur = head;
    while (cur != -1) {
        cnt = cnt + 1;
        cur = pool[cur].next;
    }
    return cnt;
}

int list_sum(void) {
    int s = 0;
    int cur = head;
    while (cur != -1) {
        s = s + pool[cur].data;
        cur = pool[cur].next;
    }
    return s;
}

int find_val(int val) {
    int cur = head;
    int pos = 0;
    while (cur != -1) {
        if (pool[cur].data == val) return pos;
        cur = pool[cur].next;
        pos = pos + 1;
    }
    return -1;
}

int main(void) {
    // Build list: push back
    push_back(10);
    push_back(20);
    push_back(30);
    printf("after push_back: ");
    print_list();

    // Push front
    push_front(5);
    push_front(1);
    printf("after push_front: ");
    print_list();

    printf("length = %d\n", list_length());
    printf("sum = %d\n", list_sum());

    // Find
    printf("find 20 = %d\n", find_val(20));
    printf("find 99 = %d\n", find_val(99));

    // More elements
    push_back(40);
    push_back(50);
    printf("final: ");
    print_list();
    printf("length = %d\n", list_length());

    return 0;
}
