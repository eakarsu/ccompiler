int printf(const char *fmt, ...);
// EXPECT: sorted before: 1 1 2 3 3 3 4 5\nsorted after:  1 2 3 4 5\nnew len = 5\nall same: 7\nno dups: 1 2 3 4\nunsorted before: 3 1 4 1 5 9 3 5\nunsorted after:  3 1 4 5 9\nnew len = 5\nsorted: 1 1 2 2 3 3 4 4 5 5\ndeduped: 1 2 3 4 5
// Test: array deduplication

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Remove duplicates from sorted array, return new length
int dedup_sorted(int *arr, int n) {
    if (n <= 1) return n;
    int write = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] != arr[i - 1]) {
            arr[write] = arr[i];
            write = write + 1;
        }
    }
    return write;
}

// Remove duplicates from unsorted array (O(n^2)), return new length
int dedup_unsorted(int *arr, int n) {
    if (n <= 1) return n;
    int write = 0;
    int i; int j;
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < write; j++) {
            if (arr[j] == arr[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            arr[write] = arr[i];
            write = write + 1;
        }
    }
    return write;
}

// Bubble sort
void sort_arr(int *arr, int n) {
    int i; int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main(void) {
    // Sorted array dedup
    int s1[8];
    s1[0]=1; s1[1]=1; s1[2]=2; s1[3]=3;
    s1[4]=3; s1[5]=3; s1[6]=4; s1[7]=5;
    printf("sorted before: ");
    print_arr(s1, 8);
    int n1 = dedup_sorted(s1, 8);
    printf("sorted after:  ");
    print_arr(s1, n1);
    printf("new len = %d\n", n1);

    // All same
    int s2[4];
    s2[0]=7; s2[1]=7; s2[2]=7; s2[3]=7;
    int n2 = dedup_sorted(s2, 4);
    printf("all same: ");
    print_arr(s2, n2);

    // No duplicates
    int s3[4];
    s3[0]=1; s3[1]=2; s3[2]=3; s3[3]=4;
    int n3 = dedup_sorted(s3, 4);
    printf("no dups: ");
    print_arr(s3, n3);

    // Unsorted dedup
    int u1[8];
    u1[0]=3; u1[1]=1; u1[2]=4; u1[3]=1;
    u1[4]=5; u1[5]=9; u1[6]=3; u1[7]=5;
    printf("unsorted before: ");
    print_arr(u1, 8);
    int n4 = dedup_unsorted(u1, 8);
    printf("unsorted after:  ");
    print_arr(u1, n4);
    printf("new len = %d\n", n4);

    // Sort then dedup
    int u2[10];
    u2[0]=5; u2[1]=3; u2[2]=1; u2[3]=3; u2[4]=5;
    u2[5]=2; u2[6]=4; u2[7]=1; u2[8]=2; u2[9]=4;
    sort_arr(u2, 10);
    printf("sorted: ");
    print_arr(u2, 10);
    int n5 = dedup_sorted(u2, 10);
    printf("deduped: ");
    print_arr(u2, n5);

    return 0;
}
