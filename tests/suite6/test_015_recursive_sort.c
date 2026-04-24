int printf(const char *fmt, ...);

int temp[64];

void merge(int arr[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i = i + 1;
        } else {
            temp[k] = arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        temp[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j <= right) {
        temp[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }
    i = left;
    while (i <= right) {
        arr[i] = temp[i];
        i = i + 1;
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    int j = low;
    int tmp;
    while (j < high) {
        if (arr[j] <= pivot) {
            i = i + 1;
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
        j = j + 1;
    }
    tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

void quicksort(int arr[], int low, int high) {
    if (low >= high) return;
    int pi = partition(arr, low, high);
    quicksort(arr, low, pi - 1);
    quicksort(arr, pi + 1, high);
}

int is_sorted(int arr[], int n) {
    int i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) return 0;
        i = i + 1;
    }
    return 1;
}

void print_array(int arr[], int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(",");
        printf("%d", arr[i]);
        i = i + 1;
    }
}

void copy_array(int dst[], int src[], int n) {
    int i = 0;
    while (i < n) {
        dst[i] = src[i];
        i = i + 1;
    }
}

int main(void) {
    int a1[8];
    a1[0]=38; a1[1]=27; a1[2]=43; a1[3]=3;
    a1[4]=9; a1[5]=82; a1[6]=10; a1[7]=1;

    merge_sort(a1, 0, 7);
    // EXPECT: msort=1,3,9,10,27,38,43,82
    printf("msort=");
    print_array(a1, 8);
    printf("\n");
    // EXPECT: msorted=1
    printf("msorted=%d\n", is_sorted(a1, 8));

    int a2[10];
    a2[0]=64; a2[1]=25; a2[2]=12; a2[3]=22; a2[4]=11;
    a2[5]=90; a2[6]=33; a2[7]=77; a2[8]=44; a2[9]=55;

    quicksort(a2, 0, 9);
    // EXPECT: qsort=11,12,22,25,33,44,55,64,77,90
    printf("qsort=");
    print_array(a2, 10);
    printf("\n");
    // EXPECT: qsorted=1
    printf("qsorted=%d\n", is_sorted(a2, 10));

    int a3[1];
    a3[0] = 42;
    merge_sort(a3, 0, 0);
    // EXPECT: single=42
    printf("single=%d\n", a3[0]);

    int a4[2];
    a4[0] = 5; a4[1] = 3;
    merge_sort(a4, 0, 1);
    // EXPECT: pair=3,5
    printf("pair=");
    print_array(a4, 2);
    printf("\n");

    int a5[6];
    a5[0]=1; a5[1]=2; a5[2]=3; a5[3]=4; a5[4]=5; a5[5]=6;
    merge_sort(a5, 0, 5);
    // EXPECT: already=1,2,3,4,5,6
    printf("already=");
    print_array(a5, 6);
    printf("\n");

    int a6[6];
    a6[0]=6; a6[1]=5; a6[2]=4; a6[3]=3; a6[4]=2; a6[5]=1;
    quicksort(a6, 0, 5);
    // EXPECT: reversed=1,2,3,4,5,6
    printf("reversed=");
    print_array(a6, 6);
    printf("\n");

    int a7[5];
    a7[0]=3; a7[1]=3; a7[2]=3; a7[3]=3; a7[4]=3;
    quicksort(a7, 0, 4);
    // EXPECT: allsame=3,3,3,3,3
    printf("allsame=");
    print_array(a7, 5);
    printf("\n");

    int a8[12];
    a8[0]=99; a8[1]=1; a8[2]=50; a8[3]=25; a8[4]=75; a8[5]=12;
    a8[6]=88; a8[7]=37; a8[8]=63; a8[9]=6; a8[10]=44; a8[11]=31;
    merge_sort(a8, 0, 11);
    // EXPECT: big=1,6,12,25,31,37,44,50,63,75,88,99
    printf("big=");
    print_array(a8, 12);
    printf("\n");
    // EXPECT: bigsorted=1
    printf("bigsorted=%d\n", is_sorted(a8, 12));

    int a9[8];
    a9[0]=-5; a9[1]=3; a9[2]=-1; a9[3]=7; a9[4]=0; a9[5]=-3; a9[6]=2; a9[7]=-8;
    quicksort(a9, 0, 7);
    // EXPECT: neg=-8,-5,-3,-1,0,2,3,7
    printf("neg=");
    print_array(a9, 8);
    printf("\n");
    // EXPECT: negsorted=1
    printf("negsorted=%d\n", is_sorted(a9, 8));

    int a10[8];
    int a10_orig[8];
    a10_orig[0]=5; a10_orig[1]=2; a10_orig[2]=8; a10_orig[3]=1;
    a10_orig[4]=9; a10_orig[5]=3; a10_orig[6]=7; a10_orig[7]=4;
    copy_array(a10, a10_orig, 8);
    merge_sort(a10, 0, 7);
    // EXPECT: mcopy=1,2,3,4,5,7,8,9
    printf("mcopy=");
    print_array(a10, 8);
    printf("\n");

    copy_array(a10, a10_orig, 8);
    quicksort(a10, 0, 7);
    // EXPECT: qcopy=1,2,3,4,5,7,8,9
    printf("qcopy=");
    print_array(a10, 8);
    printf("\n");

    return 0;
}
