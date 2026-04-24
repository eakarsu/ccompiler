int printf(const char *fmt, ...);

// Stress test pointer indirection chains

int val_through_1(int *p) {
    return *p;
}

int val_through_2(int **pp) {
    return **pp;
}

int val_through_3(int ***ppp) {
    return ***ppp;
}

int val_through_4(int ****pppp) {
    return ****pppp;
}

int val_through_5(int *****ppppp) {
    return *****ppppp;
}

void set_through_1(int *p, int v) {
    *p = v;
}

void set_through_2(int **pp, int v) {
    **pp = v;
}

void set_through_3(int ***ppp, int v) {
    ***ppp = v;
}

void swap_ptr(int **a, int **b) {
    int *tmp = *a;
    *a = *b;
    *b = tmp;
}

struct Node {
    int value;
    struct Node *next;
};

int list_sum(struct Node *head) {
    int total = 0;
    while (head != 0) {
        total = total + head->value;
        head = head->next;
    }
    return total;
}

int list_length(struct Node *head) {
    int cnt = 0;
    while (head != 0) {
        cnt = cnt + 1;
        head = head->next;
    }
    return cnt;
}

int list_max(struct Node *head) {
    int mx = head->value;
    head = head->next;
    while (head != 0) {
        if (head->value > mx) mx = head->value;
        head = head->next;
    }
    return mx;
}

int array_via_ptr(int *base, int idx) {
    return *(base + idx);
}

void array_set_via_ptr(int *base, int idx, int val) {
    *(base + idx) = val;
}

int ptr_arith_sum(int *arr, int n) {
    int total = 0;
    int *end = arr + n;
    int *p = arr;
    while (p < end) {
        total = total + *p;
        p = p + 1;
    }
    return total;
}

int ptr_diff_test(int *a, int *b) {
    return (int)(b - a);
}

struct Matrix {
    int data[4][4];
};

void matrix_set(struct Matrix *m, int r, int c, int v) {
    m->data[r][c] = v;
}

int matrix_get(struct Matrix *m, int r, int c) {
    return m->data[r][c];
}

void matrix_identity(struct Matrix *m) {
    int i = 0;
    while (i < 4) {
        int j = 0;
        while (j < 4) {
            if (i == j) m->data[i][j] = 1;
            else m->data[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

int matrix_trace(struct Matrix *m) {
    return m->data[0][0] + m->data[1][1] + m->data[2][2] + m->data[3][3];
}

int main() {
    int x = 42;
    int *p1 = &x;
    int **p2 = &p1;
    int ***p3 = &p2;
    int ****p4 = &p3;
    int *****p5 = &p4;

    printf("val1: %d\n", val_through_1(p1));
    // EXPECT: val1: 42
    printf("val2: %d\n", val_through_2(p2));
    // EXPECT: val2: 42
    printf("val3: %d\n", val_through_3(p3));
    // EXPECT: val3: 42
    printf("val4: %d\n", val_through_4(p4));
    // EXPECT: val4: 42
    printf("val5: %d\n", val_through_5(p5));
    // EXPECT: val5: 42

    set_through_1(p1, 100);
    printf("set1: %d\n", x);
    // EXPECT: set1: 100

    set_through_2(p2, 200);
    printf("set2: %d\n", x);
    // EXPECT: set2: 200

    set_through_3(p3, 300);
    printf("set3: %d\n", x);
    // EXPECT: set3: 300

    int a = 10;
    int b = 20;
    int *pa = &a;
    int *pb = &b;
    swap_ptr(&pa, &pb);
    printf("swap a: %d\n", *pa);
    // EXPECT: swap a: 20
    printf("swap b: %d\n", *pb);
    // EXPECT: swap b: 10

    struct Node n5;
    n5.value = 50; n5.next = 0;
    struct Node n4;
    n4.value = 40; n4.next = &n5;
    struct Node n3;
    n3.value = 30; n3.next = &n4;
    struct Node n2;
    n2.value = 20; n2.next = &n3;
    struct Node n1;
    n1.value = 10; n1.next = &n2;

    printf("list sum: %d\n", list_sum(&n1));
    // EXPECT: list sum: 150
    printf("list len: %d\n", list_length(&n1));
    // EXPECT: list len: 5
    printf("list max: %d\n", list_max(&n1));
    // EXPECT: list max: 50

    int arr[10];
    int i = 0;
    while (i < 10) {
        array_set_via_ptr(arr, i, (i + 1) * 10);
        i = i + 1;
    }
    printf("arr[3]: %d\n", array_via_ptr(arr, 3));
    // EXPECT: arr[3]: 40
    printf("arr[9]: %d\n", array_via_ptr(arr, 9));
    // EXPECT: arr[9]: 100
    printf("ptr sum: %d\n", ptr_arith_sum(arr, 10));
    // EXPECT: ptr sum: 550

    printf("ptr diff: %d\n", ptr_diff_test(&arr[2], &arr[7]));
    // EXPECT: ptr diff: 5

    struct Matrix m;
    matrix_identity(&m);
    printf("trace I: %d\n", matrix_trace(&m));
    // EXPECT: trace I: 4
    matrix_set(&m, 0, 0, 5);
    matrix_set(&m, 1, 1, 10);
    printf("trace mod: %d\n", matrix_trace(&m));
    // EXPECT: trace mod: 17
    printf("m[0][0]: %d\n", matrix_get(&m, 0, 0));
    // EXPECT: m[0][0]: 5

    return 0;
}
