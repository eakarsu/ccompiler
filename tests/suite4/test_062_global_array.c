int printf(const char *fmt, ...);

// Test global arrays, global string arrays, accessing from multiple functions.

int g_arr[10];
int g_init_arr[5] = {10, 20, 30, 40, 50};
char g_str[] = "hello";
char g_str2[] = "world";
int g_matrix[3][3];

void fill_array(void) {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        g_arr[i] = i * i;
    }
}

int sum_array(int *arr, int n) {
    int i;
    int total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

int get_element(int idx) {
    return g_init_arr[idx];
}

void set_element(int idx, int val) {
    g_init_arr[idx] = val;
}

void reverse_array(int *arr, int n) {
    int i;
    int tmp;
    for (i = 0; i < n / 2; i = i + 1) {
        tmp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = tmp;
    }
}

void fill_matrix(void) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            g_matrix[i][j] = i * 3 + j + 1;
        }
    }
}

int matrix_sum(void) {
    int i;
    int j;
    int total = 0;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            total = total + g_matrix[i][j];
        }
    }
    return total;
}

int matrix_diagonal(void) {
    return g_matrix[0][0] + g_matrix[1][1] + g_matrix[2][2];
}

void zero_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        arr[i] = 0;
    }
}

int main(void) {
    // Test uninitialized global array (should be zero)
    // EXPECT: 0
    printf("%d\n", g_arr[0]);
    // EXPECT: 0
    printf("%d\n", g_arr[9]);

    fill_array();
    // g_arr = {0,1,4,9,16,25,36,49,64,81}
    // EXPECT: 0
    printf("%d\n", g_arr[0]);
    // EXPECT: 9
    printf("%d\n", g_arr[3]);
    // EXPECT: 81
    printf("%d\n", g_arr[9]);

    // sum = 0+1+4+9+16+25+36+49+64+81 = 285
    // EXPECT: 285
    printf("%d\n", sum_array(g_arr, 10));

    // Test initialized global array
    // EXPECT: 10
    printf("%d\n", g_init_arr[0]);
    // EXPECT: 50
    printf("%d\n", g_init_arr[4]);
    // EXPECT: 30
    printf("%d\n", get_element(2));

    // sum = 10+20+30+40+50 = 150
    // EXPECT: 150
    printf("%d\n", sum_array(g_init_arr, 5));

    set_element(2, 99);
    // EXPECT: 99
    printf("%d\n", g_init_arr[2]);

    // Test global string arrays
    // EXPECT: hello
    printf("%s\n", g_str);
    // EXPECT: world
    printf("%s\n", g_str2);

    // Modify a char in the string
    g_str[0] = 'H';
    // EXPECT: Hello
    printf("%s\n", g_str);

    // Test reverse
    g_init_arr[0] = 10;
    g_init_arr[1] = 20;
    g_init_arr[2] = 99;
    g_init_arr[3] = 40;
    g_init_arr[4] = 50;
    reverse_array(g_init_arr, 5);
    // EXPECT: 50
    printf("%d\n", g_init_arr[0]);
    // EXPECT: 10
    printf("%d\n", g_init_arr[4]);

    // Test matrix
    fill_matrix();
    // matrix = {{1,2,3},{4,5,6},{7,8,9}}
    // EXPECT: 1
    printf("%d\n", g_matrix[0][0]);
    // EXPECT: 5
    printf("%d\n", g_matrix[1][1]);
    // EXPECT: 9
    printf("%d\n", g_matrix[2][2]);

    // sum = 1+2+3+4+5+6+7+8+9 = 45
    // EXPECT: 45
    printf("%d\n", matrix_sum());

    // diagonal = 1+5+9 = 15
    // EXPECT: 15
    printf("%d\n", matrix_diagonal());

    // Test zeroing
    zero_array(g_arr, 10);
    // EXPECT: 0
    printf("%d\n", sum_array(g_arr, 10));

    return 0;
}
