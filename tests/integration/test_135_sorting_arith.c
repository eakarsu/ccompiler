int printf(const char *fmt, ...);
// EXPECT: original: 64 34 25 12 22 11 90 1 \nbubble: 1 11 12 22 25 34 64 90 \nselection: 1 11 12 22 25 34 64 90 \ninsertion: 1 11 12 22 25 34 64 90 \nall match: 1\nre-sorted: 1 11 12 22 25 34 64 90 \nreverse: 80 70 60 50 40 30 20 10 \nsorted: 10 20 30 40 50 60 70 80 
// Test: sorting with arithmetic comparisons

void bubble_sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selection_sort(int arr[], int n) {
    int i, j, min_idx, temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

void insertion_sort(int arr[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int a[8];
    int b[8];
    int c[8];
    int i;

    /* Initialize with unsorted data */
    a[0] = 64; a[1] = 34; a[2] = 25; a[3] = 12;
    a[4] = 22; a[5] = 11; a[6] = 90; a[7] = 1;

    /* Copy for each sort */
    for (i = 0; i < 8; i++) {
        b[i] = a[i];
        c[i] = a[i];
    }

    printf("original: ");
    print_arr(a, 8);

    bubble_sort(a, 8);
    printf("bubble: ");
    print_arr(a, 8);

    selection_sort(b, 8);
    printf("selection: ");
    print_arr(b, 8);

    insertion_sort(c, 8);
    printf("insertion: ");
    print_arr(c, 8);

    /* Verify all sorts produce same result */
    {
        int match;
        match = 1;
        for (i = 0; i < 8; i++) {
            if (a[i] != b[i] || b[i] != c[i]) {
                match = 0;
            }
        }
        printf("all match: %d\n", match);
    }

    /* Sort already sorted */
    bubble_sort(a, 8);
    printf("re-sorted: ");
    print_arr(a, 8);

    /* Reverse sorted input */
    for (i = 0; i < 8; i++) {
        a[i] = 80 - i * 10;
    }
    printf("reverse: ");
    print_arr(a, 8);
    insertion_sort(a, 8);
    printf("sorted: ");
    print_arr(a, 8);

    return 0;
}
