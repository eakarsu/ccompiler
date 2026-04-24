int printf(const char *fmt, ...);
// EXPECT: evens=4\nodds=4\nfirst_even=1\nfirst_gt5=5\ndoubled: 2 4 6 8 10 12 14 16\nsquared: 1 4 9 16 25 36 49 64\neven_filtered: 2 4 6 8\ngt5_filtered: 6 7 8\neven_squared: 1 4 3 16 5 36 7 64
// Test 209: Callback pattern with function pointers

typedef int (*predicate)(int);
typedef int (*transform)(int);
typedef void (*action)(int);

int is_even(int x) { return (x / 2) * 2 == x; }
int is_odd(int x) { return !is_even(x); }
int is_positive(int x) { return x > 0; }
int is_greater_than_5(int x) { return x > 5; }

int double_it(int x) { return x * 2; }
int square_it(int x) { return x * x; }
int negate_it(int x) { return -x; }
int inc_it(int x) { return x + 1; }

int count_if(int *arr, int n, predicate pred) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

int find_first(int *arr, int n, predicate pred) {
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) return i;
        i = i + 1;
    }
    return -1;
}

void transform_if(int *arr, int n, predicate pred, transform tr) {
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) {
            arr[i] = tr(arr[i]);
        }
        i = i + 1;
    }
}

void map_array(int *src, int *dst, int n, transform tr) {
    int i = 0;
    while (i < n) {
        dst[i] = tr(src[i]);
        i = i + 1;
    }
}

int filter_array(int *src, int *dst, int n, predicate pred) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (pred(src[i])) {
            dst[count] = src[i];
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    int arr[8];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4;
    arr[4] = 5; arr[5] = 6; arr[6] = 7; arr[7] = 8;

    printf("evens=%d\n", count_if(arr, 8, is_even));
    printf("odds=%d\n", count_if(arr, 8, is_odd));
    printf("first_even=%d\n", find_first(arr, 8, is_even));
    printf("first_gt5=%d\n", find_first(arr, 8, is_greater_than_5));

    int mapped[8];
    map_array(arr, mapped, 8, double_it);
    printf("doubled: ");
    print_arr(mapped, 8);

    map_array(arr, mapped, 8, square_it);
    printf("squared: ");
    print_arr(mapped, 8);

    int filtered[8];
    int fcount = filter_array(arr, filtered, 8, is_even);
    printf("even_filtered: ");
    print_arr(filtered, fcount);

    fcount = filter_array(arr, filtered, 8, is_greater_than_5);
    printf("gt5_filtered: ");
    print_arr(filtered, fcount);

    int work[8];
    int j = 0;
    while (j < 8) { work[j] = arr[j]; j = j + 1; }
    transform_if(work, 8, is_even, square_it);
    printf("even_squared: ");
    print_arr(work, 8);

    return 0;
}
