int printf(const char *fmt, ...);

// Test global pointer variables, pointing to global arrays, global struct pointers.

int g_val = 42;
int g_arr[5] = {10, 20, 30, 40, 50};
int *g_ptr;
int *g_arr_ptr;

struct Node {
    int data;
    int next_idx;
};

struct Node g_nodes[5];
struct Node *g_node_ptr;

char g_msg[] = "hello";
char *g_str_ptr;

int g_a = 100;
int g_b = 200;
int *g_sel;

void init_pointers(void) {
    g_ptr = &g_val;
    g_arr_ptr = g_arr;
    g_node_ptr = &g_nodes[0];
    g_str_ptr = g_msg;
}

int read_via_ptr(void) {
    return *g_ptr;
}

void write_via_ptr(int val) {
    *g_ptr = val;
}

int array_via_ptr(int idx) {
    return *(g_arr_ptr + idx);
}

void set_array_via_ptr(int idx, int val) {
    *(g_arr_ptr + idx) = val;
}

int sum_via_ptr(int *arr, int n) {
    int i;
    int total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + *(arr + i);
    }
    return total;
}

void init_nodes(void) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        g_nodes[i].data = (i + 1) * 10;
        g_nodes[i].next_idx = i + 1;
    }
    g_nodes[4].next_idx = -1;
}

int walk_nodes(void) {
    int sum = 0;
    int idx = 0;
    while (idx >= 0) {
        sum = sum + g_nodes[idx].data;
        idx = g_nodes[idx].next_idx;
    }
    return sum;
}

int select_via_ptr(int choose_a) {
    if (choose_a) {
        g_sel = &g_a;
    } else {
        g_sel = &g_b;
    }
    return *g_sel;
}

void increment_via_ptr(int *p) {
    *p = *p + 1;
}

int ptr_to_array_element(void) {
    int *p = &g_arr[2];
    return *p;
}

int ptr_arith_test(void) {
    int *start = g_arr;
    int *end = g_arr + 4;
    return *end - *start;
}

int main(void) {
    init_pointers();

    // Read global via pointer
    // EXPECT: 42
    printf("%d\n", read_via_ptr());

    // Write global via pointer
    write_via_ptr(99);
    // EXPECT: 99
    printf("%d\n", g_val);
    // EXPECT: 99
    printf("%d\n", read_via_ptr());

    // Array access via global pointer
    // EXPECT: 10
    printf("%d\n", array_via_ptr(0));
    // EXPECT: 30
    printf("%d\n", array_via_ptr(2));
    // EXPECT: 50
    printf("%d\n", array_via_ptr(4));

    // Modify array via pointer
    set_array_via_ptr(1, 77);
    // EXPECT: 77
    printf("%d\n", g_arr[1]);

    // Sum via pointer: 10+77+30+40+50 = 207
    // EXPECT: 207
    printf("%d\n", sum_via_ptr(g_arr, 5));

    // String pointer
    // EXPECT: hello
    printf("%s\n", g_str_ptr);

    // Modify string via pointer
    g_str_ptr[0] = 'H';
    // EXPECT: Hello
    printf("%s\n", g_msg);

    // Node traversal
    init_nodes();
    // nodes: 10,20,30,40,50. sum = 150
    // EXPECT: 150
    printf("%d\n", walk_nodes());

    // Node pointer
    g_node_ptr = &g_nodes[2];
    // EXPECT: 30
    printf("%d\n", g_node_ptr->data);

    g_node_ptr->data = 99;
    // EXPECT: 99
    printf("%d\n", g_nodes[2].data);

    // Select between globals
    // EXPECT: 100
    printf("%d\n", select_via_ptr(1));
    // EXPECT: 200
    printf("%d\n", select_via_ptr(0));

    // Increment via pointer
    increment_via_ptr(&g_a);
    // EXPECT: 101
    printf("%d\n", g_a);

    increment_via_ptr(&g_b);
    increment_via_ptr(&g_b);
    // EXPECT: 202
    printf("%d\n", g_b);

    // Pointer to specific array element
    g_arr[2] = 30;
    // EXPECT: 30
    printf("%d\n", ptr_to_array_element());

    // Pointer arithmetic: g_arr[4]-g_arr[0] = 50-10 = 40
    g_arr[0] = 10;
    g_arr[4] = 50;
    // EXPECT: 40
    printf("%d\n", ptr_arith_test());

    // Reassign global pointer
    g_ptr = &g_a;
    // EXPECT: 101
    printf("%d\n", *g_ptr);
    g_ptr = &g_b;
    // EXPECT: 202
    printf("%d\n", *g_ptr);

    return 0;
}
