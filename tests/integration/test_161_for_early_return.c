int printf(const char *fmt, ...);
// EXPECT: find_index(30)=2\nfind_index(80)=7\nfind_index(10)=0\nfind_index(99)=-1\nis_sorted(sorted)=1\nis_sorted(unsorted)=0\nhas_dup(yes)=1\nhas_dup(no)=0\nprime_after(10)=11\nprime_after(20)=23\nprime_after(100)=101\nall_pos(yes)=1\nall_pos(no)=0
int find_index(int arr[], int len, int target) {
    int i;
    for (i = 0; i < len; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int is_sorted(int arr[], int len) {
    int i;
    for (i = 0; i < len - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int has_duplicate(int arr[], int len) {
    int i;
    int j;
    for (i = 0; i < len; i++) {
        for (j = i + 1; j < len; j++) {
            if (arr[i] == arr[j]) {
                return 1;
            }
        }
    }
    return 0;
}

int first_prime_after(int n) {
    int candidate = n + 1;
    int i;
    for (;;) {
        int is_prime = 1;
        for (i = 2; i * i <= candidate; i++) {
            if (candidate % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            return candidate;
        }
        candidate++;
    }
    return -1;
}

int all_positive(int arr[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (arr[i] <= 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int arr1[8];
    arr1[0] = 10; arr1[1] = 20; arr1[2] = 30; arr1[3] = 40;
    arr1[4] = 50; arr1[5] = 60; arr1[6] = 70; arr1[7] = 80;

    printf("find_index(30)=%d\n", find_index(arr1, 8, 30));
    printf("find_index(80)=%d\n", find_index(arr1, 8, 80));
    printf("find_index(10)=%d\n", find_index(arr1, 8, 10));
    printf("find_index(99)=%d\n", find_index(arr1, 8, 99));

    printf("is_sorted(sorted)=%d\n", is_sorted(arr1, 8));
    int arr2[4];
    arr2[0] = 5; arr2[1] = 3; arr2[2] = 8; arr2[3] = 1;
    printf("is_sorted(unsorted)=%d\n", is_sorted(arr2, 4));

    int arr3[5];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 2; arr3[4] = 5;
    printf("has_dup(yes)=%d\n", has_duplicate(arr3, 5));
    int arr4[4];
    arr4[0] = 1; arr4[1] = 2; arr4[2] = 3; arr4[3] = 4;
    printf("has_dup(no)=%d\n", has_duplicate(arr4, 4));

    printf("prime_after(10)=%d\n", first_prime_after(10));
    printf("prime_after(20)=%d\n", first_prime_after(20));
    printf("prime_after(100)=%d\n", first_prime_after(100));

    int arr5[3];
    arr5[0] = 1; arr5[1] = 5; arr5[2] = 10;
    printf("all_pos(yes)=%d\n", all_positive(arr5, 3));
    arr5[1] = -3;
    printf("all_pos(no)=%d\n", all_positive(arr5, 3));

    return 0;
}
