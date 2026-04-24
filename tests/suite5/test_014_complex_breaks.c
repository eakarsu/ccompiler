int printf(const char *fmt, ...);

int find_pair_sum(int arr[], int n, int target) {
    int i = 0;
    int found = 0;
    int ri = -1;
    int rj = -1;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            if (arr[i] + arr[j] == target) {
                ri = i;
                rj = j;
                found = 1;
                goto done;
            }
            j = j + 1;
        }
        i = i + 1;
    }
done:
    if (found) return ri * 10 + rj;
    return -1;
}

int search_3d(int n) {
    int x = 0;
    int result = -1;
    while (x < n) {
        int y = 0;
        while (y < n) {
            int z = 0;
            while (z < n) {
                if (x * x + y * y + z * z == 14) {
                    result = x * 100 + y * 10 + z;
                    goto found3d;
                }
                z = z + 1;
            }
            y = y + 1;
        }
        x = x + 1;
    }
found3d:
    return result;
}

int break_with_flag(int data[], int n) {
    int sum = 0;
    int stop = 0;
    int i = 0;
    while (i < n && stop == 0) {
        if (data[i] < 0) {
            stop = 1;
        } else {
            sum = sum + data[i];
        }
        i = i + 1;
    }
    return sum;
}

int nested_break_continue(int mat[], int r, int c) {
    int count = 0;
    int i = 0;
    while (i < r) {
        int j = 0;
        int row_has_neg = 0;
        while (j < c) {
            if (mat[i * c + j] < 0) {
                row_has_neg = 1;
                break;
            }
            j = j + 1;
        }
        if (row_has_neg == 0) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int first_dup(int arr[], int n) {
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            if (arr[i] == arr[j]) {
                return arr[i];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return -1;
}

int early_exit_chain(int a, int b, int c) {
    if (a < 0) return -1;
    if (b < 0) return -2;
    if (c < 0) return -3;
    if (a + b + c > 100) return -4;
    return a + b + c;
}

int while_break_val(int arr[], int n) {
    int i = 0;
    int last = -1;
    while (i < n) {
        if (arr[i] == 0) break;
        last = arr[i];
        i = i + 1;
    }
    return last;
}

int count_until(int arr[], int n, int sentinel) {
    int cnt = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] == sentinel) break;
        cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int a1[5] = {2, 7, 11, 15, 1};
    // EXPECT: pair(9)=1
    printf("pair(9)=%d\n", find_pair_sum(a1, 5, 9));
    // EXPECT: pair(99)=-1
    printf("pair(99)=%d\n", find_pair_sum(a1, 5, 99));

    // EXPECT: search3d=123
    printf("search3d=%d\n", search_3d(5));

    int d1[6] = {3, 5, 7, -1, 2, 4};
    // EXPECT: flag=15
    printf("flag=%d\n", break_with_flag(d1, 6));

    int d2[4] = {3, 5, 7, 9};
    // EXPECT: flag2=24
    printf("flag2=%d\n", break_with_flag(d2, 4));

    int mat[9] = {1,2,3, 4,-5,6, 7,8,9};
    // EXPECT: noneg=2
    printf("noneg=%d\n", nested_break_continue(mat, 3, 3));

    int mat2[6] = {1,2, 3,4, 5,6};
    // EXPECT: noneg2=3
    printf("noneg2=%d\n", nested_break_continue(mat2, 3, 2));

    int dup1[5] = {3, 1, 4, 1, 5};
    // EXPECT: dup=1
    printf("dup=%d\n", first_dup(dup1, 5));

    int dup2[4] = {1, 2, 3, 4};
    // EXPECT: nodup=-1
    printf("nodup=%d\n", first_dup(dup2, 4));

    // EXPECT: chain(10,20,30)=60
    printf("chain(10,20,30)=%d\n", early_exit_chain(10, 20, 30));
    // EXPECT: chain(-1,20,30)=-1
    printf("chain(-1,20,30)=%d\n", early_exit_chain(-1, 20, 30));
    // EXPECT: chain(5,-2,3)=-2
    printf("chain(5,-2,3)=%d\n", early_exit_chain(5, -2, 3));
    // EXPECT: chain(50,50,50)=-4
    printf("chain(50,50,50)=%d\n", early_exit_chain(50, 50, 50));

    int w1[5] = {5, 3, 0, 7, 2};
    // EXPECT: wbreak=3
    printf("wbreak=%d\n", while_break_val(w1, 5));

    int cu[6] = {4, 8, 2, 99, 3, 1};
    // EXPECT: count=3
    printf("count=%d\n", count_until(cu, 6, 99));
    // EXPECT: countall=6
    printf("countall=%d\n", count_until(cu, 6, 77));

    return 0;
}
