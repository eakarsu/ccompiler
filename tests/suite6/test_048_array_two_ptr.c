int printf(const char *fmt, ...);

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void sort_array(int *a, int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

int pair_with_sum(int *a, int n, int target, int *i_out, int *j_out) {
    int sorted[20];
    int i;
    int left;
    int right;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);

    left = 0;
    right = n - 1;
    while (left < right) {
        int s = sorted[left] + sorted[right];
        if (s == target) {
            *i_out = sorted[left];
            *j_out = sorted[right];
            return 1;
        } else if (s < target) {
            left++;
        } else {
            right--;
        }
    }
    return 0;
}

int count_pairs_with_sum(int *a, int n, int target) {
    int sorted[20];
    int i;
    int count = 0;
    int left;
    int right;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);

    left = 0;
    right = n - 1;
    while (left < right) {
        int s = sorted[left] + sorted[right];
        if (s == target) {
            if (sorted[left] == sorted[right]) {
                int cnt = right - left + 1;
                count = count + cnt * (cnt - 1) / 2;
                break;
            }
            int lc = 1;
            int rc = 1;
            while (left + lc < right && sorted[left + lc] == sorted[left]) lc++;
            while (right - rc > left && sorted[right - rc] == sorted[right]) rc++;
            count = count + lc * rc;
            left = left + lc;
            right = right - rc;
        } else if (s < target) {
            left++;
        } else {
            right--;
        }
    }
    return count;
}

int remove_duplicates_sorted(int *a, int n) {
    int i;
    int j;
    if (n == 0) return 0;
    j = 0;
    for (i = 1; i < n; i++) {
        if (a[i] != a[j]) {
            j++;
            a[j] = a[i];
        }
    }
    return j + 1;
}

int container_water(int *h, int n) {
    int left = 0;
    int right = n - 1;
    int max_water = 0;
    int water;
    int min_h;
    while (left < right) {
        if (h[left] < h[right]) {
            min_h = h[left];
        } else {
            min_h = h[right];
        }
        water = min_h * (right - left);
        if (water > max_water) max_water = water;
        if (h[left] < h[right]) {
            left++;
        } else {
            right--;
        }
    }
    return max_water;
}

int main(void) {
    int a[8];
    int v1;
    int v2;
    int found;

    a[0] = 1; a[1] = 4; a[2] = 45; a[3] = 6;
    a[4] = 10; a[5] = 8;
    found = pair_with_sum(a, 6, 16, &v1, &v2);
    // EXPECT: found=1 pair=6+10
    printf("found=%d pair=%d+%d\n", found, v1, v2);

    found = pair_with_sum(a, 6, 100, &v1, &v2);
    // EXPECT: not_found=0
    printf("not_found=%d\n", found);

    found = pair_with_sum(a, 6, 5, &v1, &v2);
    // EXPECT: found5=1 pair=1+4
    printf("found5=%d pair=%d+%d\n", found, v1, v2);

    a[0] = 1; a[1] = 5; a[2] = 7; a[3] = -1;
    a[4] = 5;
    int cnt = count_pairs_with_sum(a, 5, 6);
    // EXPECT: pairs_6=3
    printf("pairs_6=%d\n", cnt);

    a[0] = 1; a[1] = 1; a[2] = 1; a[3] = 1;
    cnt = count_pairs_with_sum(a, 4, 2);
    // EXPECT: pairs_2=6
    printf("pairs_2=%d\n", cnt);

    int b[8];
    b[0] = 1; b[1] = 1; b[2] = 2; b[3] = 3;
    b[4] = 3; b[5] = 4; b[6] = 4; b[7] = 5;
    int new_len = remove_duplicates_sorted(b, 8);
    // EXPECT: dedup_len=5
    printf("dedup_len=%d\n", new_len);
    // EXPECT: 1 2 3 4 5
    print_arr(b, new_len);

    b[0] = 1; b[1] = 1; b[2] = 1; b[3] = 1;
    new_len = remove_duplicates_sorted(b, 4);
    // EXPECT: all_same=1
    printf("all_same=%d\n", new_len);
    // EXPECT: 1
    print_arr(b, new_len);

    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4; b[4] = 5;
    new_len = remove_duplicates_sorted(b, 5);
    // EXPECT: no_dups=5
    printf("no_dups=%d\n", new_len);

    int h[9];
    h[0] = 1; h[1] = 8; h[2] = 6; h[3] = 2;
    h[4] = 5; h[5] = 4; h[6] = 8; h[7] = 3; h[8] = 7;
    int water = container_water(h, 9);
    // EXPECT: water=49
    printf("water=%d\n", water);

    h[0] = 1; h[1] = 1;
    water = container_water(h, 2);
    // EXPECT: water_2=1
    printf("water_2=%d\n", water);

    h[0] = 4; h[1] = 3; h[2] = 2; h[3] = 1; h[4] = 4;
    water = container_water(h, 5);
    // EXPECT: water_v=16
    printf("water_v=%d\n", water);

    h[0] = 1; h[1] = 2; h[2] = 1;
    water = container_water(h, 3);
    // EXPECT: water_3=2
    printf("water_3=%d\n", water);

    // EXPECT: done
    printf("done\n");

    return 0;
}
