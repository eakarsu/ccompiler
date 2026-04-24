int printf(const char *fmt, ...);
// EXPECT: Array: 45 12 78 34 56 12 89 23 67 12 90 45 \nsearch(78)=2\nsearch(99)=-1\nsearch(12)=1\nmax: data[10]=90\nmin: data[1]=12\ncount(12)=3\ncount(45)=2\ncount(99)=0\nsecond_max=89\nFind all 12s: idx=1 idx=5 idx=9 \nElements > 50: 78 56 89 67 90 
int linear_search(int arr[], int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int find_max(int arr[], int n) {
    int max = arr[0];
    int idx = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
            idx = i;
        }
    }
    return idx;
}

int find_min(int arr[], int n) {
    int min = arr[0];
    int idx = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
            idx = i;
        }
    }
    return idx;
}

int count_occurrences(int arr[], int n, int val) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == val) count++;
    }
    return count;
}

int find_second_max(int arr[], int n) {
    int max1 = arr[0];
    int max2 = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max1) {
            max2 = max1;
            max1 = arr[i];
        } else if (arr[i] > max2 && arr[i] != max1) {
            max2 = arr[i];
        }
    }
    return max2;
}

int main(void) {
    int data[12];
    data[0] = 45; data[1] = 12; data[2] = 78; data[3] = 34;
    data[4] = 56; data[5] = 12; data[6] = 89; data[7] = 23;
    data[8] = 67; data[9] = 12; data[10] = 90; data[11] = 45;

    printf("Array: ");
    int i;
    for (i = 0; i < 12; i++) printf("%d ", data[i]);
    printf("\n");

    printf("search(78)=%d\n", linear_search(data, 12, 78));
    printf("search(99)=%d\n", linear_search(data, 12, 99));
    printf("search(12)=%d\n", linear_search(data, 12, 12));

    int mi = find_max(data, 12);
    printf("max: data[%d]=%d\n", mi, data[mi]);
    mi = find_min(data, 12);
    printf("min: data[%d]=%d\n", mi, data[mi]);

    printf("count(12)=%d\n", count_occurrences(data, 12, 12));
    printf("count(45)=%d\n", count_occurrences(data, 12, 45));
    printf("count(99)=%d\n", count_occurrences(data, 12, 99));

    printf("second_max=%d\n", find_second_max(data, 12));

    printf("Find all 12s: ");
    for (i = 0; i < 12; i++) {
        if (data[i] == 12) printf("idx=%d ", i);
    }
    printf("\n");

    printf("Elements > 50: ");
    for (i = 0; i < 12; i++) {
        if (data[i] > 50) printf("%d ", data[i]);
    }
    printf("\n");

    return 0;
}
