int printf(const char *fmt, ...);

struct record {
    int primary;
    int secondary;
    int id;
};

void swap_rec(struct record arr[], int i, int j) {
    struct record tmp;
    tmp.primary = arr[i].primary;
    tmp.secondary = arr[i].secondary;
    tmp.id = arr[i].id;
    arr[i].primary = arr[j].primary;
    arr[i].secondary = arr[j].secondary;
    arr[i].id = arr[j].id;
    arr[j].primary = tmp.primary;
    arr[j].secondary = tmp.secondary;
    arr[j].id = tmp.id;
}

int compare_rec(struct record *a, struct record *b) {
    if (a->primary < b->primary) return -1;
    if (a->primary > b->primary) return 1;
    if (a->secondary < b->secondary) return -1;
    if (a->secondary > b->secondary) return 1;
    return 0;
}

void multi_key_sort(struct record arr[], int n) {
    int i;
    int j;
    struct record key;
    i = 1;
    while (i < n) {
        key.primary = arr[i].primary;
        key.secondary = arr[i].secondary;
        key.id = arr[i].id;
        j = i - 1;
        while (j >= 0 && compare_rec(&arr[j], &key) > 0) {
            arr[j + 1].primary = arr[j].primary;
            arr[j + 1].secondary = arr[j].secondary;
            arr[j + 1].id = arr[j].id;
            j = j - 1;
        }
        arr[j + 1].primary = key.primary;
        arr[j + 1].secondary = key.secondary;
        arr[j + 1].id = key.id;
        i = i + 1;
    }
}

int is_multi_sorted(struct record arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (compare_rec(&arr[i], &arr[i + 1]) > 0) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void sort_by_secondary_only(struct record arr[], int n) {
    int i;
    int j;
    struct record key;
    i = 1;
    while (i < n) {
        key.primary = arr[i].primary;
        key.secondary = arr[i].secondary;
        key.id = arr[i].id;
        j = i - 1;
        while (j >= 0 && arr[j].secondary > key.secondary) {
            arr[j + 1].primary = arr[j].primary;
            arr[j + 1].secondary = arr[j].secondary;
            arr[j + 1].id = arr[j].id;
            j = j - 1;
        }
        arr[j + 1].primary = key.primary;
        arr[j + 1].secondary = key.secondary;
        arr[j + 1].id = key.id;
        i = i + 1;
    }
}

void print_records(struct record arr[], int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("(%d,%d,%d) ", arr[i].primary,
               arr[i].secondary, arr[i].id);
        i = i + 1;
    }
}

int main(void) {
    struct record a[10];
    int i;

    a[0].primary = 3; a[0].secondary = 2; a[0].id = 1;
    a[1].primary = 1; a[1].secondary = 3; a[1].id = 2;
    a[2].primary = 3; a[2].secondary = 1; a[2].id = 3;
    a[3].primary = 2; a[3].secondary = 2; a[3].id = 4;
    a[4].primary = 1; a[4].secondary = 1; a[4].id = 5;
    a[5].primary = 2; a[5].secondary = 3; a[5].id = 6;
    a[6].primary = 3; a[6].secondary = 3; a[6].id = 7;
    a[7].primary = 1; a[7].secondary = 2; a[7].id = 8;

    multi_key_sort(a, 8);
    // EXPECT: multi_ok: 1
    printf("multi_ok: %d\n", is_multi_sorted(a, 8));

    print_records(a, 8);
    // EXPECT: (1,1,5) (1,2,8) (1,3,2) (2,2,4) (2,3,6) (3,1,3) (3,2,1) (3,3,7) done
    printf("done\n");

    // EXPECT: first: 1-1
    printf("first: %d-%d\n", a[0].primary, a[0].secondary);
    // EXPECT: last: 3-3
    printf("last: %d-%d\n", a[7].primary, a[7].secondary);

    a[0].primary = 5; a[0].secondary = 1; a[0].id = 1;
    a[1].primary = 5; a[1].secondary = 3; a[1].id = 2;
    a[2].primary = 5; a[2].secondary = 2; a[2].id = 3;
    a[3].primary = 5; a[3].secondary = 5; a[3].id = 4;
    a[4].primary = 5; a[4].secondary = 4; a[4].id = 5;
    multi_key_sort(a, 5);
    // EXPECT: same_pri: 1
    printf("same_pri: %d\n", is_multi_sorted(a, 5));
    // EXPECT: sec_order: 1 2 3 4 5
    printf("sec_order: %d %d %d %d %d\n",
           a[0].secondary, a[1].secondary,
           a[2].secondary, a[3].secondary,
           a[4].secondary);

    a[0].primary = 1; a[0].secondary = 5; a[0].id = 1;
    a[1].primary = 2; a[1].secondary = 5; a[1].id = 2;
    a[2].primary = 3; a[2].secondary = 5; a[2].id = 3;
    a[3].primary = 4; a[3].secondary = 5; a[3].id = 4;
    multi_key_sort(a, 4);
    // EXPECT: same_sec: 1
    printf("same_sec: %d\n", is_multi_sorted(a, 4));
    // EXPECT: pri_order: 1 2 3 4
    printf("pri_order: %d %d %d %d\n",
           a[0].primary, a[1].primary,
           a[2].primary, a[3].primary);

    a[0].primary = 10; a[0].secondary = 20; a[0].id = 1;
    a[1].primary = 10; a[1].secondary = 10; a[1].id = 2;
    a[2].primary = 5; a[2].secondary = 30; a[2].id = 3;
    a[3].primary = 5; a[3].secondary = 15; a[3].id = 4;
    a[4].primary = 20; a[4].secondary = 5; a[4].id = 5;
    a[5].primary = 20; a[5].secondary = 1; a[5].id = 6;

    sort_by_secondary_only(a, 6);
    // EXPECT: sec1: 1
    printf("sec1: %d\n", a[0].secondary);
    // EXPECT: sec6: 30
    printf("sec6: %d\n", a[5].secondary);

    a[0].primary = 10; a[0].secondary = 20; a[0].id = 1;
    a[1].primary = 10; a[1].secondary = 10; a[1].id = 2;
    a[2].primary = 5; a[2].secondary = 30; a[2].id = 3;
    a[3].primary = 5; a[3].secondary = 15; a[3].id = 4;
    a[4].primary = 20; a[4].secondary = 5; a[4].id = 5;
    a[5].primary = 20; a[5].secondary = 1; a[5].id = 6;
    multi_key_sort(a, 6);
    // EXPECT: full_sorted: 1
    printf("full_sorted: %d\n", is_multi_sorted(a, 6));
    print_records(a, 6);
    // EXPECT: (5,15,4) (5,30,3) (10,10,2) (10,20,1) (20,1,6) (20,5,5) done
    printf("done\n");

    // EXPECT: cmp_eq: 0
    printf("cmp_eq: %d\n", compare_rec(&a[0], &a[0]));

    a[0].primary = 1; a[0].secondary = 1; a[0].id = 1;
    a[1].primary = 2; a[1].secondary = 2; a[1].id = 2;
    // EXPECT: cmp_lt: -1
    printf("cmp_lt: %d\n", compare_rec(&a[0], &a[1]));
    // EXPECT: cmp_gt: 1
    printf("cmp_gt: %d\n", compare_rec(&a[1], &a[0]));

    return 0;
}
