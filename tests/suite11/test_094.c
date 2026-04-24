int printf(const char *fmt, ...);

// Test: Array decay to pointer in various contexts

int sum_ptr(int *p, int n) {
    int s;
    s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + p[i];
    }
    return s;
}

int first_element(int *p) {
    return *p;
}

int element_at(int *p, int idx) {
    return *(p + idx);
}

void fill_array(int *p, int n, int val) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        p[i] = val + i;
    }
}

int string_length(char *s) {
    int len;
    len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

char first_char(char *s) {
    return s[0];
}

int main() {
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    // Array decays to pointer when passed to function
    int s;
    s = sum_ptr(arr, 5);
    printf("%d\n", s); // EXPECT: 150

    // Array name as pointer
    printf("%d\n", first_element(arr)); // EXPECT: 10

    // Array + offset as pointer
    printf("%d\n", first_element(arr + 2)); // EXPECT: 30

    // element_at with array decay
    printf("%d\n", element_at(arr, 3)); // EXPECT: 40

    // fill_array: array decays when passed
    int buf[4];
    fill_array(buf, 4, 100);
    printf("%d\n", buf[0]); // EXPECT: 100
    printf("%d\n", buf[1]); // EXPECT: 101
    printf("%d\n", buf[2]); // EXPECT: 102
    printf("%d\n", buf[3]); // EXPECT: 103

    // Pointer arithmetic on decayed array
    int *p;
    p = arr;
    printf("%d\n", *p);       // EXPECT: 10
    printf("%d\n", *(p + 1)); // EXPECT: 20
    printf("%d\n", *(p + 4)); // EXPECT: 50

    // Array subscript vs pointer dereference
    printf("%d\n", arr[2]);     // EXPECT: 30
    printf("%d\n", *(arr + 2)); // EXPECT: 30

    // Pointer assigned from array, then indexed
    int *q;
    q = arr;
    printf("%d\n", q[0]); // EXPECT: 10
    printf("%d\n", q[4]); // EXPECT: 50

    // Array of chars decays to char*
    char str[6];
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = 0;
    printf("%d\n", string_length(str)); // EXPECT: 5
    printf("%d\n", (int)first_char(str)); // EXPECT: 72

    // String literal decays to char*
    printf("%d\n", string_length("world")); // EXPECT: 5

    // Array in conditional expression (decays)
    int cond_arr[3];
    cond_arr[0] = 77;
    cond_arr[1] = 88;
    cond_arr[2] = 99;
    int *cp;
    cp = cond_arr;
    if (cp == cond_arr) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // Pointer subtraction on decayed arrays
    int big[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        big[i] = i * i;
    }
    int *start;
    int *end;
    start = big;
    end = big + 10;
    int diff;
    diff = (int)(end - start);
    printf("%d\n", diff); // EXPECT: 10

    // Decayed array passed to sum
    printf("%d\n", sum_ptr(big, 10)); // EXPECT: 285

    // Sub-array (pointer to middle)
    printf("%d\n", sum_ptr(big + 5, 5)); // EXPECT: 255

    // 2D array row decay
    int matrix[3][4];
    for (i = 0; i < 3; i = i + 1) {
        int j;
        for (j = 0; j < 4; j = j + 1) {
            matrix[i][j] = i * 4 + j;
        }
    }
    // Each row is int[4], decays to int*
    printf("%d\n", sum_ptr(matrix[0], 4)); // EXPECT: 6
    printf("%d\n", sum_ptr(matrix[1], 4)); // EXPECT: 22
    printf("%d\n", sum_ptr(matrix[2], 4)); // EXPECT: 38

    // Pointer to array element
    int *mp;
    mp = &matrix[1][2];
    printf("%d\n", *mp); // EXPECT: 6

    return 0;
}
