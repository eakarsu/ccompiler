// Initialization edge cases: partial init, nested, global vs local, static, zero-init, brace scalar
int printf(const char *fmt, ...);

// Global variables with init
int g_arr[10] = {1, 2, 3};
int g_val = 42;

struct pair {
    int a;
    int b;
};

struct nested {
    struct pair p;
    int arr[4];
    int x;
};

struct big {
    int data[20];
    struct pair p;
    int flag;
};

// Global struct init
struct pair g_pair = {100, 200};

void test_partial_array(void) {
    int arr[10] = {1, 2, 3};
    int i;
    for (i = 0; i < 10; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
    // EXPECT: 1 2 3 0 0 0 0 0 0 0
}

void test_full_array(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
    // EXPECT: 10 20 30 40 50
}

void test_nested_struct_partial(void) {
    struct nested n = {{1, 2}};
    printf("p=%d,%d arr=%d,%d,%d,%d x=%d\n",
           n.p.a, n.p.b, n.arr[0], n.arr[1], n.arr[2], n.arr[3], n.x);
    // EXPECT: p=1,2 arr=0,0,0,0 x=0
}

void test_nested_struct_full(void) {
    struct nested n = {{10, 20}, {1, 2, 3, 4}, 99};
    printf("p=%d,%d arr=%d,%d,%d,%d x=%d\n",
           n.p.a, n.p.b, n.arr[0], n.arr[1], n.arr[2], n.arr[3], n.x);
    // EXPECT: p=10,20 arr=1,2,3,4 x=99
}

void test_global_partial_array(void) {
    int i;
    for (i = 0; i < 10; i++) {
        if (i > 0) printf(" ");
        printf("%d", g_arr[i]);
    }
    printf("\n");
    // EXPECT: 1 2 3 0 0 0 0 0 0 0
}

void test_global_val(void) {
    printf("g_val=%d\n", g_val);
    // EXPECT: g_val=42
}

void test_global_struct(void) {
    printf("g_pair=%d,%d\n", g_pair.a, g_pair.b);
    // EXPECT: g_pair=100,200
}

int count_calls(void) {
    static int counter = 0;
    counter++;
    return counter;
}

void test_static_local(void) {
    int i;
    for (i = 0; i < 5; i++) {
        printf("call %d\n", count_calls());
    }
    // EXPECT: call 1
    // EXPECT: call 2
    // EXPECT: call 3
    // EXPECT: call 4
    // EXPECT: call 5
}

void test_static_local_init(void) {
    static int x = 77;
    printf("static=%d\n", x);
    // EXPECT: static=77
    x = 88;
    printf("static=%d\n", x);
    // EXPECT: static=88
}

void test_zero_init_big_struct(void) {
    struct big b = {0};
    int i;
    int all_zero = 1;
    for (i = 0; i < 20; i++) {
        if (b.data[i] != 0) all_zero = 0;
    }
    if (b.p.a != 0 || b.p.b != 0) all_zero = 0;
    if (b.flag != 0) all_zero = 0;
    printf("all_zero=%d\n", all_zero);
    // EXPECT: all_zero=1
}

void test_brace_enclosed_scalar(void) {
    int x = {42};
    printf("brace scalar=%d\n", x);
    // EXPECT: brace scalar=42
}

void test_reinit_in_loop(void) {
    int i;
    for (i = 0; i < 5; i++) {
        int arr[3] = {i, i * 10, i * 100};
        printf("%d %d %d\n", arr[0], arr[1], arr[2]);
    }
    // EXPECT: 0 0 0
    // EXPECT: 1 10 100
    // EXPECT: 2 20 200
    // EXPECT: 3 30 300
    // EXPECT: 4 40 400
}

void test_char_array_init(void) {
    char s1[] = "hello";
    char s2[10] = "hi";
    printf("s1=%s len=%d\n", s1, (int)sizeof(s1));
    // EXPECT: s1=hello len=6
    printf("s2=%s len=%d\n", s2, (int)sizeof(s2));
    // EXPECT: s2=hi len=10
    printf("s2[2]=%d s2[9]=%d\n", s2[2], s2[9]);
    // EXPECT: s2[2]=0 s2[9]=0
}

void test_struct_array_partial(void) {
    struct pair arr[5] = {{1, 2}, {3, 4}};
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("{%d,%d}", arr[i].a, arr[i].b);
    }
    printf("\n");
    // EXPECT: {1,2} {3,4} {0,0} {0,0} {0,0}
}

void test_mixed_init_loop(void) {
    int i;
    for (i = 0; i < 3; i++) {
        struct nested n = {{i, i+1}, {10, 20, 30, 40}, i*100};
        printf("iter %d: p=%d,%d x=%d\n", i, n.p.a, n.p.b, n.x);
    }
    // EXPECT: iter 0: p=0,1 x=0
    // EXPECT: iter 1: p=1,2 x=100
    // EXPECT: iter 2: p=2,3 x=200
}

void test_single_element_array(void) {
    int arr[1] = {999};
    printf("single=%d\n", arr[0]);
    // EXPECT: single=999
}

void test_nested_zero(void) {
    struct nested n = {0};
    printf("nz: %d %d %d %d %d %d %d\n",
           n.p.a, n.p.b, n.arr[0], n.arr[1], n.arr[2], n.arr[3], n.x);
    // EXPECT: nz: 0 0 0 0 0 0 0
}

int main(void) {
    test_partial_array();
    test_full_array();
    test_nested_struct_partial();
    test_nested_struct_full();
    test_global_partial_array();
    test_global_val();
    test_global_struct();
    test_static_local();
    test_static_local_init();
    test_zero_init_big_struct();
    test_brace_enclosed_scalar();
    test_reinit_in_loop();
    test_char_array_init();
    test_struct_array_partial();
    test_mixed_init_loop();
    test_single_element_array();
    test_nested_zero();
    return 0;
}
