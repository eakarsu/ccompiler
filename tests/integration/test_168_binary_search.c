int printf(const char *fmt, ...);
// EXPECT: Array: 1 4 7 10 13 16 19 22 25 28 31 34 37 40 43 \nsearch(1)=0\nsearch(10)=3\nsearch(22)=7\nsearch(43)=14\nsearch(0)=-1\nsearch(50)=-1\nBounds tests:\nlb(3)=1 ub(3)=4\nlb(5)=4 ub(5)=6\nlb(7)=6 ub(7)=9\nlb(9)=9 ub(9)=11\nlb(6)=6 ub(6)=6\nInteger sqrt:\nisqrt(0)=0\nisqrt(1)=1\nisqrt(4)=2\nisqrt(9)=3\nisqrt(15)=3\nisqrt(100)=10\nisqrt(1000)=31
int binary_search(int arr[], int len, int target) {
    int lo = 0;
    int hi = len - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int lower_bound(int arr[], int len, int target) {
    int lo = 0;
    int hi = len;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int upper_bound(int arr[], int len, int target) {
    int lo = 0;
    int hi = len;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int int_sqrt(int n) {
    int lo = 0;
    int hi = n;
    if (hi > 46340) hi = 46340;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid * mid == n) {
            return mid;
        } else if (mid * mid < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

int main(void) {
    int arr[15];
    int i;
    for (i = 0; i < 15; i++) {
        arr[i] = i * 3 + 1;
    }
    printf("Array: ");
    for (i = 0; i < 15; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int targets[6];
    targets[0] = 1; targets[1] = 10; targets[2] = 22;
    targets[3] = 43; targets[4] = 0; targets[5] = 50;
    for (i = 0; i < 6; i++) {
        printf("search(%d)=%d\n", targets[i], binary_search(arr, 15, targets[i]));
    }

    int arr2[12];
    arr2[0] = 1; arr2[1] = 3; arr2[2] = 3; arr2[3] = 3;
    arr2[4] = 5; arr2[5] = 5; arr2[6] = 7; arr2[7] = 7;
    arr2[8] = 7; arr2[9] = 9; arr2[10] = 9; arr2[11] = 11;
    printf("Bounds tests:\n");
    int bvals[5];
    bvals[0] = 3; bvals[1] = 5; bvals[2] = 7; bvals[3] = 9; bvals[4] = 6;
    for (i = 0; i < 5; i++) {
        printf("lb(%d)=%d ub(%d)=%d\n", bvals[i], lower_bound(arr2, 12, bvals[i]),
               bvals[i], upper_bound(arr2, 12, bvals[i]));
    }

    printf("Integer sqrt:\n");
    int sqvals[7];
    sqvals[0] = 0; sqvals[1] = 1; sqvals[2] = 4; sqvals[3] = 9;
    sqvals[4] = 15; sqvals[5] = 100; sqvals[6] = 1000;
    for (i = 0; i < 7; i++) {
        printf("isqrt(%d)=%d\n", sqvals[i], int_sqrt(sqvals[i]));
    }

    return 0;
}
