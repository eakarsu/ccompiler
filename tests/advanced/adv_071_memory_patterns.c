// EXPECT: === Memory Patterns ===
// EXPECT: Allocated array of 10 ints
// EXPECT: Array: 0 2 4 6 8 10 12 14 16 18
// EXPECT: Sum: 90
// EXPECT: Allocated 5 points
// EXPECT: Point 0: (0, 0)
// EXPECT: Point 1: (1, 2)
// EXPECT: Point 2: (2, 4)
// EXPECT: Point 3: (3, 6)
// EXPECT: Point 4: (4, 8)
// EXPECT: Freed points
// EXPECT: Realloc test: 1 2 3 4 5 6 7 8 9 10
// EXPECT: Matrix allocated: 4x4
// EXPECT: Matrix diagonal: 1 1 1 1
// EXPECT: Matrix freed
// EXPECT: Null check passed
// EXPECT: Double-free avoided
// EXPECT: Zero-size handled
// EXPECT: All memory tests passed

int printf(const char *fmt, ...);
void *malloc(unsigned long size);
void free(void *ptr);
void *calloc(unsigned long count, unsigned long size);
void *realloc(void *ptr, unsigned long size);

typedef struct {
    int x;
    int y;
} Point;

void test_array_alloc(void) {
    int *arr = (int *)malloc(10 * sizeof(int));
    if (!arr) {
        printf("malloc failed\n");
        return;
    }
    printf("Allocated array of 10 ints\n");

    int i;
    for (i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }

    printf("Array:");
    for (i = 0; i < 10; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");

    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + arr[i];
    }
    printf("Sum: %d\n", sum);

    free(arr);
}

void test_struct_array(void) {
    Point *pts = (Point *)malloc(5 * sizeof(Point));
    if (!pts) {
        printf("malloc failed\n");
        return;
    }
    printf("Allocated 5 points\n");

    int i;
    for (i = 0; i < 5; i++) {
        pts[i].x = i;
        pts[i].y = i * 2;
    }

    for (i = 0; i < 5; i++) {
        printf("Point %d: (%d, %d)\n", i, pts[i].x, pts[i].y);
    }

    free(pts);
    printf("Freed points\n");
}

void test_realloc(void) {
    int *arr = (int *)malloc(5 * sizeof(int));
    int i;
    for (i = 0; i < 5; i++) {
        arr[i] = i + 1;
    }

    arr = (int *)realloc(arr, 10 * sizeof(int));
    for (i = 5; i < 10; i++) {
        arr[i] = i + 1;
    }

    printf("Realloc test:");
    for (i = 0; i < 10; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");

    free(arr);
}

void test_matrix(void) {
    int rows = 4;
    int cols = 4;
    int **mat = (int **)malloc(rows * sizeof(int *));
    int i;
    int j;

    for (i = 0; i < rows; i++) {
        mat[i] = (int *)calloc(cols, sizeof(int));
    }
    printf("Matrix allocated: %dx%d\n", rows, cols);

    for (i = 0; i < rows; i++) {
        mat[i][i] = 1;
    }

    printf("Matrix diagonal:");
    for (i = 0; i < rows; i++) {
        printf(" %d", mat[i][i]);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
    printf("Matrix freed\n");
}

void test_null_checks(void) {
    int *p = (int *)0;
    if (!p) {
        printf("Null check passed\n");
    }

    int *q = (int *)malloc(sizeof(int));
    free(q);
    q = (int *)0;
    if (!q) {
        printf("Double-free avoided\n");
    }

    int *z = (int *)malloc(0);
    if (z) {
        free(z);
    }
    printf("Zero-size handled\n");
}

int main(void) {
    printf("=== Memory Patterns ===\n");
    test_array_alloc();
    test_struct_array();
    test_realloc();
    test_matrix();
    test_null_checks();
    printf("All memory tests passed\n");
    return 0;
}
