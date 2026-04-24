int printf(const char *fmt, ...);
// EXPECT: sum_odd(10)=25\nsum_odd(20)=100\nsum_odd(1)=1\nsum_odd(15)=64\nnon_div(20,3)=14\nnon_div(100,7)=86\nnon_div(10,2)=5\nskip_sum(arr,8,0)=16\nskip_sum(arr2,6,10)=90\nFiltered: 0 6 12 18 24 \nCascade: 180
int sum_odd(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        if (i % 2 == 0) {
            i++;
            continue;
        }
        sum = sum + i;
        i++;
    }
    return sum;
}

int count_non_divisible(int n, int d) {
    int count = 0;
    int i = 1;
    while (i <= n) {
        if (i % d == 0) {
            i++;
            continue;
        }
        count++;
        i++;
    }
    return count;
}

int skip_and_sum(int arr[], int len, int skip_val) {
    int sum = 0;
    int i = 0;
    while (i < len) {
        if (arr[i] == skip_val) {
            i++;
            continue;
        }
        sum = sum + arr[i];
        i++;
    }
    return sum;
}

int main(void) {
    printf("sum_odd(10)=%d\n", sum_odd(10));
    printf("sum_odd(20)=%d\n", sum_odd(20));
    printf("sum_odd(1)=%d\n", sum_odd(1));
    printf("sum_odd(15)=%d\n", sum_odd(15));

    printf("non_div(20,3)=%d\n", count_non_divisible(20, 3));
    printf("non_div(100,7)=%d\n", count_non_divisible(100, 7));
    printf("non_div(10,2)=%d\n", count_non_divisible(10, 2));

    int arr1[8];
    arr1[0] = 1; arr1[1] = 0; arr1[2] = 3; arr1[3] = 0;
    arr1[4] = 5; arr1[5] = 0; arr1[6] = 7; arr1[7] = 0;
    printf("skip_sum(arr,8,0)=%d\n", skip_and_sum(arr1, 8, 0));

    int arr2[6];
    arr2[0] = 10; arr2[1] = 20; arr2[2] = 10; arr2[3] = 30;
    arr2[4] = 10; arr2[5] = 40;
    printf("skip_sum(arr2,6,10)=%d\n", skip_and_sum(arr2, 6, 10));

    printf("Filtered: ");
    int data[10];
    int i;
    for (i = 0; i < 10; i++) {
        data[i] = i * 3;
    }
    i = 0;
    while (i < 10) {
        if (data[i] % 2 != 0) {
            i++;
            continue;
        }
        printf("%d ", data[i]);
        i++;
    }
    printf("\n");

    printf("Cascade: ");
    i = 0;
    int total = 0;
    while (i < 50) {
        i++;
        if (i % 2 == 0) continue;
        if (i % 5 == 0) continue;
        if (i > 30) continue;
        total = total + i;
    }
    printf("%d\n", total);
    return 0;
}
