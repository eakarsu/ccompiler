int printf(const char *fmt, ...);

struct Node {
    int value;
    int next;
};

struct Node nodes[64];
int node_count;

int new_node(int val) {
    int idx = node_count;
    nodes[idx].value = val;
    nodes[idx].next = -1;
    node_count = node_count + 1;
    return idx;
}

int list_prepend(int head, int val) {
    int n = new_node(val);
    nodes[n].next = head;
    return n;
}

int list_append(int head, int val) {
    int n = new_node(val);
    if (head == -1) return n;
    int cur = head;
    while (nodes[cur].next != -1) {
        cur = nodes[cur].next;
    }
    nodes[cur].next = n;
    return head;
}

int rec_length(int head) {
    if (head == -1) return 0;
    return 1 + rec_length(nodes[head].next);
}

int rec_sum(int head) {
    if (head == -1) return 0;
    return nodes[head].value + rec_sum(nodes[head].next);
}

int rec_find(int head, int val) {
    if (head == -1) return -1;
    if (nodes[head].value == val) return head;
    return rec_find(nodes[head].next, val);
}

int rec_max(int head) {
    if (nodes[head].next == -1) return nodes[head].value;
    int sub_max = rec_max(nodes[head].next);
    if (nodes[head].value > sub_max) return nodes[head].value;
    return sub_max;
}

int rec_min(int head) {
    if (nodes[head].next == -1) return nodes[head].value;
    int sub_min = rec_min(nodes[head].next);
    if (nodes[head].value < sub_min) return nodes[head].value;
    return sub_min;
}

int rec_nth(int head, int n) {
    if (head == -1) return -1;
    if (n == 0) return nodes[head].value;
    return rec_nth(nodes[head].next, n - 1);
}

int rec_count_val(int head, int val) {
    if (head == -1) return 0;
    int c = rec_count_val(nodes[head].next, val);
    if (nodes[head].value == val) c = c + 1;
    return c;
}

int rec_reverse(int head) {
    if (head == -1) return -1;
    if (nodes[head].next == -1) return head;
    int new_head = rec_reverse(nodes[head].next);
    int tail = new_head;
    while (nodes[tail].next != -1) {
        tail = nodes[tail].next;
    }
    nodes[tail].next = head;
    nodes[head].next = -1;
    return new_head;
}

int rec_last(int head) {
    if (nodes[head].next == -1) return nodes[head].value;
    return rec_last(nodes[head].next);
}

int rec_is_sorted(int head) {
    if (head == -1) return 1;
    if (nodes[head].next == -1) return 1;
    if (nodes[head].value > nodes[nodes[head].next].value) return 0;
    return rec_is_sorted(nodes[head].next);
}

int rec_contains_dup(int head) {
    if (head == -1) return 0;
    int cur = nodes[head].next;
    while (cur != -1) {
        if (nodes[cur].value == nodes[head].value) return 1;
        cur = nodes[cur].next;
    }
    return rec_contains_dup(nodes[head].next);
}

void print_list(int head) {
    if (head == -1) return;
    printf("%d", nodes[head].value);
    if (nodes[head].next != -1) printf("->");
    print_list(nodes[head].next);
}

int main(void) {
    node_count = 0;
    int list = -1;
    list = list_append(list, 10);
    list = list_append(list, 20);
    list = list_append(list, 30);
    list = list_append(list, 40);
    list = list_append(list, 50);

    // EXPECT: list=10->20->30->40->50
    printf("list=");
    print_list(list);
    printf("\n");

    // EXPECT: len=5
    printf("len=%d\n", rec_length(list));
    // EXPECT: sum=150
    printf("sum=%d\n", rec_sum(list));
    // EXPECT: max=50
    printf("max=%d\n", rec_max(list));
    // EXPECT: min=10
    printf("min=%d\n", rec_min(list));

    int found = rec_find(list, 30);
    // EXPECT: find(30)=30
    printf("find(30)=%d\n", nodes[found].value);
    // EXPECT: find(99)=-1
    printf("find(99)=%d\n", rec_find(list, 99));

    // EXPECT: nth(0)=10
    printf("nth(0)=%d\n", rec_nth(list, 0));
    // EXPECT: nth(2)=30
    printf("nth(2)=%d\n", rec_nth(list, 2));
    // EXPECT: nth(4)=50
    printf("nth(4)=%d\n", rec_nth(list, 4));

    // EXPECT: last=50
    printf("last=%d\n", rec_last(list));
    // EXPECT: sorted=1
    printf("sorted=%d\n", rec_is_sorted(list));

    int rev = rec_reverse(list);
    // EXPECT: rev=50->40->30->20->10
    printf("rev=");
    print_list(rev);
    printf("\n");

    node_count = 0;
    int list2 = -1;
    list2 = list_append(list2, 5);
    list2 = list_append(list2, 3);
    list2 = list_append(list2, 5);
    list2 = list_append(list2, 7);
    list2 = list_append(list2, 5);

    // EXPECT: count(5)=3
    printf("count(5)=%d\n", rec_count_val(list2, 5));
    // EXPECT: count(7)=1
    printf("count(7)=%d\n", rec_count_val(list2, 7));
    // EXPECT: hasdup=1
    printf("hasdup=%d\n", rec_contains_dup(list2));

    node_count = 0;
    int list3 = -1;
    list3 = list_append(list3, 1);
    list3 = list_append(list3, 2);
    list3 = list_append(list3, 3);
    // EXPECT: nodup=0
    printf("nodup=%d\n", rec_contains_dup(list3));
    // EXPECT: sorted3=1
    printf("sorted3=%d\n", rec_is_sorted(list3));

    node_count = 0;
    int list4 = -1;
    list4 = list_prepend(list4, 1);
    list4 = list_prepend(list4, 2);
    list4 = list_prepend(list4, 3);
    // EXPECT: prepend=3->2->1
    printf("prepend=");
    print_list(list4);
    printf("\n");

    return 0;
}
