// stress_009_array_torture.c - Array edge cases stress test
// EXPECT: zero_idx=60
// EXPECT: neg_off=50
// EXPECT: neg_off2=30
// EXPECT: fnptr_arr=26,14,120,3,2
// EXPECT: cube_sum=378
// EXPECT: cube_corner=27
// EXPECT: cube_mid=14
// EXPECT: struct_arr=42,360
// EXPECT: struct_arr_elem=50
// EXPECT: desig_sum=2000
// EXPECT: desig_gap=0
// EXPECT: ptr2d=12
// EXPECT: row_sum=58
// EXPECT: commute=40,40
// EXPECT: large_sort=1
// EXPECT: arr_size=40
// EXPECT: ptr_size=8
int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// --- Zero-index access after pointer arithmetic ---
void test_zero_index(void) {
    int arr[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int *p = arr + 5;
    printf("zero_idx=%d\n", p[0]); // 60
}

// --- Negative pointer offsets ---
void test_neg_offset(void) {
    int arr[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int *p = &arr[7];
    printf("neg_off=%d\n", p[-3]); // arr[4] = 50
    printf("neg_off2=%d\n", *(p - 5)); // arr[2] = 30
}

// --- Array of function pointers called in loop ---
typedef int (*binop)(int, int);
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int my_div(int a, int b) { return a / b; }
int my_mod(int a, int b) { return a % b; }

void test_fn_ptr_array(void) {
    binop ops[5];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = my_div;
    ops[4] = my_mod;

    int results[5];
    int i;
    for (i = 0; i < 5; i++) {
        results[i] = ops[i](20, 6);
    }
    // add(20,6)=26, sub=14, mul=120, div=3, mod=2
    printf("fnptr_arr=%d,%d,%d,%d,%d\n", results[0], results[1], results[2], results[3], results[4]);
}

// --- 3D array access ---
void test_3d_array(void) {
    int cube[3][3][3];
    int i, j, k;
    int val = 1;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            for (k = 0; k < 3; k++)
                cube[i][j][k] = val++;

    // Sum all 27 elements: 1+2+...+27 = 378
    int sum = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            for (k = 0; k < 3; k++)
                sum += cube[i][j][k];
    printf("cube_sum=%d\n", sum);

    // Access specific elements
    printf("cube_corner=%d\n", cube[2][2][2]); // 27
    printf("cube_mid=%d\n", cube[1][1][1]); // 14
}

// --- Array as struct member accessed via pointer ---
struct with_arr {
    int id;
    int data[8];
    int checksum;
};

void test_struct_array_member(void) {
    struct with_arr s;
    s.id = 42;
    int i;
    for (i = 0; i < 8; i++) s.data[i] = (i + 1) * 10;
    s.checksum = 0;
    for (i = 0; i < 8; i++) s.checksum += s.data[i];
    // checksum = 10+20+...+80 = 360

    struct with_arr *p = &s;
    printf("struct_arr=%d,%d\n", p->id, p->checksum); // 42,360
    printf("struct_arr_elem=%d\n", p->data[4]); // 50
}

// --- Designated initializer with gaps ---
void test_designated_init(void) {
    int arr[10] = {[0] = 100, [3] = 300, [7] = 700, [9] = 900};
    // Unspecified elements are 0
    int sum = 0;
    int i;
    for (i = 0; i < 10; i++) sum += arr[i];
    // 100+0+0+300+0+0+0+700+0+900 = 2000
    printf("desig_sum=%d\n", sum);
    printf("desig_gap=%d\n", arr[5]); // 0
}

// --- 2D array with pointer access ---
void test_2d_ptr_access(void) {
    int mat[4][4];
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[i][j] = i * 4 + j + 1;

    // Access via pointer arithmetic: mat[2][3] = 12
    int *base = &mat[0][0];
    printf("ptr2d=%d\n", *(base + 2 * 4 + 3)); // 12

    // Row sum of row 3: 13+14+15+16 = 58
    int sum = 0;
    int *row3 = &mat[3][0];
    for (i = 0; i < 4; i++) sum += row3[i];
    printf("row_sum=%d\n", sum);
}

// --- Array subscript commutativity: a[i] == i[a] ---
void test_subscript_commute(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int v1 = arr[3];
    int v2 = 3[arr];
    printf("commute=%d,%d\n", v1, v2); // 40,40
}

// --- Large array sort and verify ---
void test_large_sort(void) {
    int arr[100];
    int i;
    // Initialize in reverse
    for (i = 0; i < 100; i++) arr[i] = 100 - i;

    // Bubble sort
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        for (i = 0; i < 99; i++) {
            if (arr[i] > arr[i + 1]) {
                int t = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = t;
                swapped = 1;
            }
        }
    }

    int ok = 1;
    for (i = 0; i < 100; i++) {
        if (arr[i] != i + 1) ok = 0;
    }
    printf("large_sort=%d\n", ok);
}

// --- Array decay and sizeof ---
void test_array_decay(void) {
    int arr[10];
    int sz_arr = (int)sizeof(arr);       // 40
    int *p = arr;
    int sz_ptr = (int)sizeof(p);         // 8 on 64-bit
    printf("arr_size=%d\n", sz_arr);
    printf("ptr_size=%d\n", sz_ptr);
}

int main(void) {
    test_zero_index();
    test_neg_offset();
    test_fn_ptr_array();
    test_3d_array();
    test_struct_array_member();
    test_designated_init();
    test_2d_ptr_access();
    test_subscript_commute();
    test_large_sort();
    test_array_decay();
    return 0;
}
