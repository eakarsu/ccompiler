int printf(const char *fmt, ...);
// EXPECT: Original: -5 12 -3 7 0 -8 15 2\nAscending: -8 -5 -3 0 2 7 12 15\nDescending: 15 12 7 2 0 -3 -5 -8\nBy abs value: 0 2 -3 -5 7 -8 12 15\nEven first: -8 0 2 12 -5 -3 7 15\nSort methods applied: asc desc abs even

typedef int (*Comparator)(int, int);

int ascending(int a, int b) { return a - b; }
int descending(int a, int b) { return b - a; }
int by_abs(int a, int b) {
    int aa = a < 0 ? -a : a;
    int ab = b < 0 ? -b : b;
    return aa - ab;
}
int even_first(int a, int b) {
    int ea = a % 2;
    int eb = b % 2;
    if (ea < 0) ea = -ea;
    if (eb < 0) eb = -eb;
    if (ea != eb) return ea - eb;
    return a - b;
}

void bubble_sort(int *arr, int n, Comparator cmp) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int main(void) {
    int original[8];
    original[0] = -5; original[1] = 12; original[2] = -3; original[3] = 7;
    original[4] = 0; original[5] = -8; original[6] = 15; original[7] = 2;
    int n = 8;
    int work[8];

    printf("Original: ");
    print_array(original, n);

    copy_arr(work, original, n);
    bubble_sort(work, n, ascending);
    printf("Ascending: ");
    print_array(work, n);

    copy_arr(work, original, n);
    bubble_sort(work, n, descending);
    printf("Descending: ");
    print_array(work, n);

    copy_arr(work, original, n);
    bubble_sort(work, n, by_abs);
    printf("By abs value: ");
    print_array(work, n);

    copy_arr(work, original, n);
    bubble_sort(work, n, even_first);
    printf("Even first: ");
    print_array(work, n);

    Comparator comparators[4];
    comparators[0] = ascending;
    comparators[1] = descending;
    comparators[2] = by_abs;
    comparators[3] = even_first;
    char *names[4];
    names[0] = "asc"; names[1] = "desc"; names[2] = "abs"; names[3] = "even";

    printf("Sort methods applied: %s %s %s %s\n", names[0], names[1], names[2], names[3]);

    return 0;
}
