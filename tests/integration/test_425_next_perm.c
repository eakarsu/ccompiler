int printf(const char *fmt, ...);
// EXPECT: All permutations of {1,2,3}:\n1 2 3\n1 3 2\n2 1 3\n2 3 1\n3 1 2\n3 2 1\nCount: 6\n\nNext of 1 3 2: 2 1 3\nNext of 3 2 1: 1 2 3\nWrapped: 1\n\nNext of 1 1 2: 1 2 1\nNext of 1 2 1: 2 1 1
// Test: Next permutation

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int arr[], int left, int right) {
    while (left < right) {
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
}

int next_permutation(int arr[], int n) {
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    if (i < 0) {
        reverse(arr, 0, n - 1);
        return 0;
    }
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    swap(&arr[i], &arr[j]);
    reverse(arr, i + 1, n - 1);
    return 1;
}

void print_array(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int factorial(int n) {
    int r = 1;
    int i;
    for (i = 2; i <= n; i++) r = r * i;
    return r;
}

int main(void) {
    int a[] = {1, 2, 3};
    printf("All permutations of {1,2,3}:\n");
    int count = 0;
    int total = factorial(3);
    int i;
    for (i = 0; i < total; i++) {
        print_array(a, 3);
        next_permutation(a, 3);
        count++;
    }
    printf("Count: %d\n", count);

    int b[] = {1, 3, 2};
    printf("\nNext of 1 3 2: ");
    next_permutation(b, 3);
    print_array(b, 3);

    int c[] = {3, 2, 1};
    printf("Next of 3 2 1: ");
    int has_next = next_permutation(c, 3);
    print_array(c, 3);
    printf("Wrapped: %d\n", !has_next);

    int d[] = {1, 1, 2};
    printf("\nNext of 1 1 2: ");
    next_permutation(d, 3);
    print_array(d, 3);
    printf("Next of 1 2 1: ");
    next_permutation(d, 3);
    print_array(d, 3);

    return 0;
}
