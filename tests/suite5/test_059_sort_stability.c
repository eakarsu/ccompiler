int printf(const char *fmt, ...);

struct pair {
    int key;
    int val;
};

void stable_insertion_sort(struct pair arr[], int n) {
    int i;
    int j;
    struct pair key_pair;
    i = 1;
    while (i < n) {
        key_pair.key = arr[i].key;
        key_pair.val = arr[i].val;
        j = i - 1;
        while (j >= 0 && arr[j].key > key_pair.key) {
            arr[j + 1].key = arr[j].key;
            arr[j + 1].val = arr[j].val;
            j = j - 1;
        }
        arr[j + 1].key = key_pair.key;
        arr[j + 1].val = key_pair.val;
        i = i + 1;
    }
}

void stable_merge(struct pair arr[], struct pair tmp[], int l, int m, int r) {
    int i;
    int j;
    int k;
    i = l;
    j = m;
    k = l;
    while (i < m && j < r) {
        if (arr[i].key <= arr[j].key) {
            tmp[k].key = arr[i].key;
            tmp[k].val = arr[i].val;
            i = i + 1;
        } else {
            tmp[k].key = arr[j].key;
            tmp[k].val = arr[j].val;
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < m) {
        tmp[k].key = arr[i].key;
        tmp[k].val = arr[i].val;
        i = i + 1;
        k = k + 1;
    }
    while (j < r) {
        tmp[k].key = arr[j].key;
        tmp[k].val = arr[j].val;
        j = j + 1;
        k = k + 1;
    }
    i = l;
    while (i < r) {
        arr[i].key = tmp[i].key;
        arr[i].val = tmp[i].val;
        i = i + 1;
    }
}

void stable_merge_sort(struct pair arr[], struct pair tmp[], int n) {
    int w;
    int l;
    int m;
    int r;
    w = 1;
    while (w < n) {
        l = 0;
        while (l < n) {
            m = l + w;
            if (m > n) { m = n; }
            r = l + w + w;
            if (r > n) { r = n; }
            if (m < r) {
                stable_merge(arr, tmp, l, m, r);
            }
            l = l + w + w;
        }
        w = w * 2;
    }
}

int check_stability(struct pair arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i].key == arr[i + 1].key) {
            if (arr[i].val > arr[i + 1].val) {
                return 0;
            }
        }
        i = i + 1;
    }
    return 1;
}

int is_key_sorted(struct pair arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i].key > arr[i + 1].key) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int main(void) {
    struct pair a[10];
    struct pair tmp[10];
    struct pair b[8];
    struct pair tb[8];
    int i;

    a[0].key = 3; a[0].val = 1;
    a[1].key = 1; a[1].val = 2;
    a[2].key = 3; a[2].val = 3;
    a[3].key = 2; a[3].val = 4;
    a[4].key = 1; a[4].val = 5;
    a[5].key = 2; a[5].val = 6;
    a[6].key = 3; a[6].val = 7;
    a[7].key = 1; a[7].val = 8;

    stable_insertion_sort(a, 8);
    // EXPECT: ins_sorted: 1
    printf("ins_sorted: %d\n", is_key_sorted(a, 8));
    // EXPECT: ins_stable: 1
    printf("ins_stable: %d\n", check_stability(a, 8));

    i = 0;
    while (i < 8) {
        printf("%d-%d ", a[i].key, a[i].val);
        i = i + 1;
    }
    // EXPECT: 1-2 1-5 1-8 2-4 2-6 3-1 3-3 3-7 done
    printf("done\n");

    b[0].key = 5; b[0].val = 1;
    b[1].key = 3; b[1].val = 2;
    b[2].key = 5; b[2].val = 3;
    b[3].key = 1; b[3].val = 4;
    b[4].key = 3; b[4].val = 5;
    b[5].key = 1; b[5].val = 6;
    b[6].key = 5; b[6].val = 7;
    b[7].key = 3; b[7].val = 8;

    stable_merge_sort(b, tb, 8);
    // EXPECT: ms_sorted: 1
    printf("ms_sorted: %d\n", is_key_sorted(b, 8));
    // EXPECT: ms_stable: 1
    printf("ms_stable: %d\n", check_stability(b, 8));

    i = 0;
    while (i < 8) {
        printf("%d-%d ", b[i].key, b[i].val);
        i = i + 1;
    }
    // EXPECT: 1-4 1-6 3-2 3-5 3-8 5-1 5-3 5-7 done
    printf("done\n");

    a[0].key = 1; a[0].val = 10;
    a[1].key = 1; a[1].val = 20;
    a[2].key = 1; a[2].val = 30;
    a[3].key = 1; a[3].val = 40;
    a[4].key = 1; a[4].val = 50;
    stable_insertion_sort(a, 5);
    // EXPECT: allsame_ins: 1
    printf("allsame_ins: %d\n", check_stability(a, 5));
    // EXPECT: v0: 10
    printf("v0: %d\n", a[0].val);
    // EXPECT: v4: 50
    printf("v4: %d\n", a[4].val);

    a[0].key = 2; a[0].val = 1;
    a[1].key = 2; a[1].val = 2;
    a[2].key = 2; a[2].val = 3;
    a[3].key = 2; a[3].val = 4;
    a[4].key = 2; a[4].val = 5;
    stable_merge_sort(a, tmp, 5);
    // EXPECT: allsame_ms: 1
    printf("allsame_ms: %d\n", check_stability(a, 5));
    // EXPECT: mv0: 1
    printf("mv0: %d\n", a[0].val);
    // EXPECT: mv4: 5
    printf("mv4: %d\n", a[4].val);

    a[0].key = 10; a[0].val = 1;
    a[1].key = 5; a[1].val = 2;
    stable_insertion_sort(a, 2);
    // EXPECT: two_sort: 5
    printf("two_sort: %d\n", a[0].key);
    // EXPECT: two_val: 2
    printf("two_val: %d\n", a[0].val);

    a[0].key = 42; a[0].val = 99;
    stable_insertion_sort(a, 1);
    // EXPECT: one: 42-99
    printf("one: %d-%d\n", a[0].key, a[0].val);

    return 0;
}
