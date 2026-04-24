int printf(const char *fmt, ...);

int global_arr[5];

int sum_array(int *a, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s = s + a[i];
    }
    return s;
}

int max_array(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (a[i] > m) {
            m = a[i];
        }
    }
    return m;
}

int min_array(int *a, int n) {
    int m = a[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (a[i] < m) {
            m = a[i];
        }
    }
    return m;
}

void fill_sequential(int *a, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        a[i] = i * i;
    }
}

int count_positive(int *a, int n) {
    int c = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] > 0) {
            c = c + 1;
        }
    }
    return c;
}

int dot_product(int *a, int *b, int n) {
    int result = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        result = result + a[i] * b[i];
    }
    return result;
}

int main(void) {
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    // EXPECT: 10
    printf("%d\n", arr[0]);
    // EXPECT: 30
    printf("%d\n", arr[2]);
    // EXPECT: 50
    printf("%d\n", arr[4]);

    // EXPECT: 150
    printf("%d\n", sum_array(arr, 5));

    // EXPECT: 50
    printf("%d\n", max_array(arr, 5));

    // EXPECT: 10
    printf("%d\n", min_array(arr, 5));

    arr[2] = 99;
    // EXPECT: 99
    printf("%d\n", arr[2]);

    // EXPECT: 219
    printf("%d\n", sum_array(arr, 5));

    fill_sequential(global_arr, 5);
    // EXPECT: 0
    printf("%d\n", global_arr[0]);
    // EXPECT: 1
    printf("%d\n", global_arr[1]);
    // EXPECT: 4
    printf("%d\n", global_arr[2]);
    // EXPECT: 9
    printf("%d\n", global_arr[3]);
    // EXPECT: 16
    printf("%d\n", global_arr[4]);

    int mixed[6];
    mixed[0] = -3;
    mixed[1] = 0;
    mixed[2] = 5;
    mixed[3] = -1;
    mixed[4] = 7;
    mixed[5] = 0;

    // EXPECT: 2
    printf("%d\n", count_positive(mixed, 6));

    int vec_a[3];
    vec_a[0] = 1;
    vec_a[1] = 2;
    vec_a[2] = 3;
    int vec_b[3];
    vec_b[0] = 4;
    vec_b[1] = 5;
    vec_b[2] = 6;

    // dot = 1*4 + 2*5 + 3*6 = 4+10+18 = 32
    // EXPECT: 32
    printf("%d\n", dot_product(vec_a, vec_b, 3));

    char greeting[6];
    greeting[0] = 'H';
    greeting[1] = 'e';
    greeting[2] = 'l';
    greeting[3] = 'l';
    greeting[4] = 'o';
    greeting[5] = 0;
    // EXPECT: Hello
    printf("%s\n", greeting);

    int len = 0;
    while (greeting[len] != 0) {
        len = len + 1;
    }
    // EXPECT: 5
    printf("%d\n", len);

    // EXPECT: 20
    printf("%d\n", sizeof(arr));

    int big[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        big[i] = i + 1;
    }
    // EXPECT: 55
    printf("%d\n", sum_array(big, 10));

    // EXPECT: 10
    printf("%d\n", max_array(big, 10));

    // EXPECT: 1
    printf("%d\n", min_array(big, 10));

    return 0;
}
