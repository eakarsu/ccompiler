int printf(const char *fmt, ...);
// EXPECT: First divisors:\nfirst_div(12,2)=2\nfirst_div(17,2)=17\nfirst_div(100,2)=2\nfirst_div(37,2)=37\nfirst_div(48,2)=2\nCount until sum:\ncount_sum(10)=4\ncount_sum(55)=10\ncount_sum(100)=14\ncount_sum(500)=32\nfind 'l' in hello world: 2\nfind 'w' in hello world: 6\nfind 'z' in hello world: -1\narr[0]=0 arr[1]=1 arr[2]=4 arr[3]=9 arr[4]=16 arr[5]=25 \nBroke at j=6
int find_first_div(int n, int start) {
    int i = start;
    while (i <= n) {
        if (n % i == 0) {
            break;
        }
        i++;
    }
    return i;
}

int count_until_sum(int target) {
    int sum = 0;
    int count = 0;
    int i = 1;
    while (i <= 1000) {
        sum = sum + i;
        count++;
        if (sum >= target) {
            break;
        }
        i++;
    }
    return count;
}

int find_char(char *str, char target) {
    int pos = 0;
    while (str[pos] != 0) {
        if (str[pos] == target) {
            break;
        }
        pos++;
    }
    if (str[pos] == 0) {
        return -1;
    }
    return pos;
}

int main(void) {
    int i;
    printf("First divisors:\n");
    int nums[5];
    nums[0] = 12;
    nums[1] = 17;
    nums[2] = 100;
    nums[3] = 37;
    nums[4] = 48;
    for (i = 0; i < 5; i++) {
        printf("first_div(%d,2)=%d\n", nums[i], find_first_div(nums[i], 2));
    }

    printf("Count until sum:\n");
    int targets[4];
    targets[0] = 10;
    targets[1] = 55;
    targets[2] = 100;
    targets[3] = 500;
    for (i = 0; i < 4; i++) {
        printf("count_sum(%d)=%d\n", targets[i], count_until_sum(targets[i]));
    }

    char str1[20];
    str1[0] = 'h'; str1[1] = 'e'; str1[2] = 'l'; str1[3] = 'l'; str1[4] = 'o';
    str1[5] = ' '; str1[6] = 'w'; str1[7] = 'o'; str1[8] = 'r'; str1[9] = 'l';
    str1[10] = 'd'; str1[11] = 0;
    printf("find 'l' in hello world: %d\n", find_char(str1, 'l'));
    printf("find 'w' in hello world: %d\n", find_char(str1, 'w'));
    printf("find 'z' in hello world: %d\n", find_char(str1, 'z'));

    int arr[10];
    for (i = 0; i < 10; i++) {
        arr[i] = i * i;
    }
    int j = 0;
    while (j < 10) {
        if (arr[j] > 30) {
            break;
        }
        printf("arr[%d]=%d ", j, arr[j]);
        j++;
    }
    printf("\nBroke at j=%d\n", j);
    return 0;
}
