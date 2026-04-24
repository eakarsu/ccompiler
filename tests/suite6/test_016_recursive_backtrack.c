int printf(const char *fmt, ...);

int board[8];

int is_safe(int row, int col, int n) {
    int i = 0;
    while (i < row) {
        if (board[i] == col) return 0;
        int rd = row - i;
        int cd = board[i] - col;
        if (cd < 0) cd = -cd;
        if (rd == cd) return 0;
        i = i + 1;
    }
    return 1;
}

int solve_nqueens(int row, int n) {
    if (row == n) return 1;
    int count = 0;
    int col = 0;
    while (col < n) {
        if (is_safe(row, col, n)) {
            board[row] = col;
            count = count + solve_nqueens(row + 1, n);
        }
        col = col + 1;
    }
    return count;
}

int perm_arr[8];
int perm_used[8];
int perm_count;

void count_permutations(int pos, int n) {
    if (pos == n) {
        perm_count = perm_count + 1;
        return;
    }
    int i = 0;
    while (i < n) {
        if (perm_used[i] == 0) {
            perm_arr[pos] = i;
            perm_used[i] = 1;
            count_permutations(pos + 1, n);
            perm_used[i] = 0;
        }
        i = i + 1;
    }
}

int subset_sum(int arr[], int n, int idx, int target) {
    if (target == 0) return 1;
    if (idx >= n) return 0;
    if (target < 0) return 0;
    if (subset_sum(arr, n, idx + 1, target - arr[idx])) return 1;
    return subset_sum(arr, n, idx + 1, target);
}

int count_subset_sum(int arr[], int n, int idx, int target) {
    if (target == 0) return 1;
    if (idx >= n) return 0;
    if (target < 0) return 0;
    return count_subset_sum(arr, n, idx + 1, target - arr[idx]) +
           count_subset_sum(arr, n, idx + 1, target);
}

int combination_count;

void count_combinations(int arr[], int n, int start, int chosen[], int chosen_len, int k) {
    if (chosen_len == k) {
        combination_count = combination_count + 1;
        return;
    }
    int i = start;
    while (i < n) {
        chosen[chosen_len] = arr[i];
        count_combinations(arr, n, i + 1, chosen, chosen_len + 1, k);
        i = i + 1;
    }
}

int can_partition(int arr[], int n, int idx, int sum1, int sum2) {
    if (idx == n) {
        if (sum1 == sum2) return 1;
        return 0;
    }
    if (can_partition(arr, n, idx + 1, sum1 + arr[idx], sum2)) return 1;
    return can_partition(arr, n, idx + 1, sum1, sum2 + arr[idx]);
}

int derangement_count;

void count_derangements(int arr[], int n, int pos) {
    if (pos == n) {
        derangement_count = derangement_count + 1;
        return;
    }
    int i = pos;
    while (i < n) {
        int tmp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = tmp;
        if (arr[pos] != pos) {
            count_derangements(arr, n, pos + 1);
        }
        tmp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = tmp;
        i = i + 1;
    }
}

int main(void) {
    // EXPECT: queens(4)=2
    printf("queens(4)=%d\n", solve_nqueens(0, 4));
    // EXPECT: queens(5)=10
    printf("queens(5)=%d\n", solve_nqueens(0, 5));
    // EXPECT: queens(6)=4
    printf("queens(6)=%d\n", solve_nqueens(0, 6));

    perm_count = 0;
    int i = 0;
    while (i < 8) { perm_used[i] = 0; i = i + 1; }
    count_permutations(0, 3);
    // EXPECT: perm(3)=6
    printf("perm(3)=%d\n", perm_count);

    perm_count = 0;
    i = 0;
    while (i < 8) { perm_used[i] = 0; i = i + 1; }
    count_permutations(0, 4);
    // EXPECT: perm(4)=24
    printf("perm(4)=%d\n", perm_count);

    perm_count = 0;
    i = 0;
    while (i < 8) { perm_used[i] = 0; i = i + 1; }
    count_permutations(0, 5);
    // EXPECT: perm(5)=120
    printf("perm(5)=%d\n", perm_count);

    int s1[5];
    s1[0]=3; s1[1]=7; s1[2]=1; s1[3]=8; s1[4]=4;
    // EXPECT: subsum(11)=1
    printf("subsum(11)=%d\n", subset_sum(s1, 5, 0, 11));
    // EXPECT: subsum(100)=0
    printf("subsum(100)=%d\n", subset_sum(s1, 5, 0, 100));
    // EXPECT: subsum(23)=1
    printf("subsum(23)=%d\n", subset_sum(s1, 5, 0, 23));

    int s2[4];
    s2[0]=1; s2[1]=2; s2[2]=3; s2[3]=4;
    // EXPECT: cntsum(5)=2
    printf("cntsum(5)=%d\n", count_subset_sum(s2, 4, 0, 5));
    // EXPECT: cntsum(10)=1
    printf("cntsum(10)=%d\n", count_subset_sum(s2, 4, 0, 10));

    int c1[5];
    c1[0]=1; c1[1]=2; c1[2]=3; c1[3]=4; c1[4]=5;
    int chosen[5];
    combination_count = 0;
    count_combinations(c1, 5, 0, chosen, 0, 2);
    // EXPECT: C(5,2)=10
    printf("C(5,2)=%d\n", combination_count);

    combination_count = 0;
    count_combinations(c1, 5, 0, chosen, 0, 3);
    // EXPECT: C(5,3)=10
    printf("C(5,3)=%d\n", combination_count);

    int p1[4];
    p1[0]=1; p1[1]=5; p1[2]=5; p1[3]=1;
    // EXPECT: canpart1=1
    printf("canpart1=%d\n", can_partition(p1, 4, 0, 0, 0));

    int p2[3];
    p2[0]=1; p2[1]=2; p2[2]=5;
    // EXPECT: canpart2=0
    printf("canpart2=%d\n", can_partition(p2, 3, 0, 0, 0));

    int d1[4];
    d1[0]=0; d1[1]=1; d1[2]=2; d1[3]=3;
    derangement_count = 0;
    count_derangements(d1, 4, 0);
    // EXPECT: derange(4)=9
    printf("derange(4)=%d\n", derangement_count);

    return 0;
}
