int printf(const char *fmt, ...);
// EXPECT: sum = 150\nvia pointer: 10 20 30\nmiddle element: 30\nprev element: 20\nnext element: 40\nstring: hello\nlength: 5\nfind 'l': 2\nfind 'x': -1\nreversed: 1 2 3 4\nvia ptr array: 10 20 30\niteration: 100 200 300 400 500\narr[0] == *arr: 1\narr[2] == *(arr+2): 1\nrow access: 11 22 33
// Test: array decay to pointer

int sum_array(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i++) {
        s = s + arr[i];
    }
    return s;
}

int find_char(char *str, char target) {
    int i = 0;
    while (str[i] != 0) {
        if (str[i] == target) return i;
        i++;
    }
    return -1;
}

int string_length(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

void reverse_array(int *arr, int len) {
    int i;
    for (i = 0; i < len / 2; i++) {
        int tmp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = tmp;
    }
}

int main(void) {
    // Array decays to pointer when passed to function
    int nums[5];
    nums[0] = 10; nums[1] = 20; nums[2] = 30; nums[3] = 40; nums[4] = 50;
    printf("sum = %d\n", sum_array(nums, 5));

    // Array name as pointer
    int *p = nums;
    printf("via pointer: %d %d %d\n", p[0], p[1], p[2]);

    // Pointer arithmetic on decayed array
    int *mid = nums + 2;
    printf("middle element: %d\n", *mid);
    printf("prev element: %d\n", *(mid - 1));
    printf("next element: %d\n", *(mid + 1));

    // Char array decays to char*
    char str[6];
    str[0] = 'h'; str[1] = 'e'; str[2] = 'l';
    str[3] = 'l'; str[4] = 'o'; str[5] = 0;
    printf("string: %s\n", str);
    printf("length: %d\n", string_length(str));
    printf("find 'l': %d\n", find_char(str, 'l'));
    printf("find 'x': %d\n", find_char(str, 'x'));

    // Modify array through pointer
    int arr2[4];
    arr2[0] = 4; arr2[1] = 3; arr2[2] = 2; arr2[3] = 1;
    reverse_array(arr2, 4);
    printf("reversed: %d %d %d %d\n", arr2[0], arr2[1], arr2[2], arr2[3]);

    // Array of pointers
    int a = 10;
    int b = 20;
    int c = 30;
    int *ptrs[3];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    printf("via ptr array: %d %d %d\n", *ptrs[0], *ptrs[1], *ptrs[2]);

    // Pointer to array element
    int arr3[5];
    arr3[0] = 100; arr3[1] = 200; arr3[2] = 300; arr3[3] = 400; arr3[4] = 500;
    int *end = arr3 + 5;
    int *iter;
    printf("iteration: ");
    int first = 1;
    for (iter = arr3; iter != end; iter++) {
        if (!first) printf(" ");
        printf("%d", *iter);
        first = 0;
    }
    printf("\n");

    // Decay in expressions
    int darr[3];
    darr[0] = 1; darr[1] = 2; darr[2] = 3;
    printf("arr[0] == *arr: %d\n", darr[0] == *darr);
    printf("arr[2] == *(arr+2): %d\n", darr[2] == *(darr + 2));

    // Nested array access
    int matrix[3];
    matrix[0] = 11; matrix[1] = 22; matrix[2] = 33;
    int *row = matrix;
    printf("row access: %d %d %d\n", row[0], row[1], row[2]);

    return 0;
}
