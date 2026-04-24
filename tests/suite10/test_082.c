int printf(const char *fmt, ...);

// Strategy Pattern: function pointer swap for sorting strategies

typedef int (*CompareFunc)(int, int);

int ascending(int a, int b) {
    return a - b;
}

int descending(int a, int b) {
    return b - a;
}

int abs_ascending(int a, int b) {
    int aa;
    int ab;
    if (a < 0) aa = -a; else aa = a;
    if (b < 0) ab = -b; else ab = b;
    return aa - ab;
}

struct Sorter {
    CompareFunc strategy;
    char *name;
};

void sorter_init(struct Sorter *s, CompareFunc cmp, char *name) {
    s->strategy = cmp;
    s->name = name;
}

void sorter_set_strategy(struct Sorter *s, CompareFunc cmp, char *name) {
    s->strategy = cmp;
    s->name = name;
}

void sorter_sort(struct Sorter *s, int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (s->strategy(arr[j], arr[j + 1]) > 0) {
                int tmp;
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void print_array(char *label, int *arr, int n) {
    printf("%s:", label);
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int even_first(int a, int b) {
    int a_even;
    int b_even;
    a_even = (a % 2 == 0) ? 1 : 0;
    b_even = (b % 2 == 0) ? 1 : 0;
    if (a_even != b_even) {
        return b_even - a_even;
    }
    return a - b;
}

int main() {
    int arr[6];
    arr[0] = 5; arr[1] = 2; arr[2] = 8; arr[3] = 1; arr[4] = 9; arr[5] = 3;

    struct Sorter sorter;
    sorter_init(&sorter, ascending, "ascending");
    printf("Strategy: %s\n", sorter.name); // EXPECT: Strategy: ascending

    sorter_sort(&sorter, arr, 6);
    print_array("Ascending", arr, 6); // EXPECT: Ascending: 1 2 3 5 8 9

    sorter_set_strategy(&sorter, descending, "descending");
    printf("Strategy: %s\n", sorter.name); // EXPECT: Strategy: descending

    sorter_sort(&sorter, arr, 6);
    print_array("Descending", arr, 6); // EXPECT: Descending: 9 8 5 3 2 1

    int arr2[5];
    arr2[0] = -3; arr2[1] = 7; arr2[2] = -1; arr2[3] = 4; arr2[4] = -5;

    sorter_set_strategy(&sorter, abs_ascending, "abs_ascending");
    printf("Strategy: %s\n", sorter.name); // EXPECT: Strategy: abs_ascending

    sorter_sort(&sorter, arr2, 5);
    print_array("AbsAsc", arr2, 5); // EXPECT: AbsAsc: -1 -3 4 -5 7

    int arr3[6];
    arr3[0] = 7; arr3[1] = 2; arr3[2] = 5; arr3[3] = 4; arr3[4] = 3; arr3[5] = 6;

    sorter_set_strategy(&sorter, even_first, "even_first");
    printf("Strategy: %s\n", sorter.name); // EXPECT: Strategy: even_first

    sorter_sort(&sorter, arr3, 6);
    print_array("EvenFirst", arr3, 6); // EXPECT: EvenFirst: 2 4 6 3 5 7

    printf("Strategies tested: %d\n", 4); // EXPECT: Strategies tested: 4
    printf("Done\n"); // EXPECT: Done
    return 0;
}
