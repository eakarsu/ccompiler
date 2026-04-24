int printf(const char *fmt, ...);

// Hash set with union, intersection, and difference operations

enum { SET_SIZE = 32, EMPTY_SLOT = -999999 };

struct HashSet {
    int data[32];
    int count;
};

int set_hash(int val) {
    int h = val;
    if (h < 0) h = -h;
    return h % SET_SIZE;
}

void set_init(struct HashSet *s) {
    int i;
    for (i = 0; i < SET_SIZE; i++) {
        s->data[i] = EMPTY_SLOT;
    }
    s->count = 0;
}

int set_contains(struct HashSet *s, int val) {
    int idx;
    int start;
    idx = set_hash(val);
    start = idx;
    while (1) {
        if (s->data[idx] == EMPTY_SLOT) return 0;
        if (s->data[idx] == val) return 1;
        idx = (idx + 1) % SET_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int set_add(struct HashSet *s, int val) {
    int idx;
    int start;
    if (set_contains(s, val)) return 0;
    if (s->count >= SET_SIZE) return 0;
    idx = set_hash(val);
    start = idx;
    while (1) {
        if (s->data[idx] == EMPTY_SLOT) {
            s->data[idx] = val;
            s->count = s->count + 1;
            return 1;
        }
        idx = (idx + 1) % SET_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

void set_to_array(struct HashSet *s, int *arr, int *n) {
    int i;
    *n = 0;
    for (i = 0; i < SET_SIZE; i++) {
        if (s->data[i] != EMPTY_SLOT) {
            arr[*n] = s->data[i];
            *n = *n + 1;
        }
    }
}

void set_union(struct HashSet *a, struct HashSet *b, struct HashSet *result) {
    int i;
    set_init(result);
    for (i = 0; i < SET_SIZE; i++) {
        if (a->data[i] != EMPTY_SLOT) {
            set_add(result, a->data[i]);
        }
    }
    for (i = 0; i < SET_SIZE; i++) {
        if (b->data[i] != EMPTY_SLOT) {
            set_add(result, b->data[i]);
        }
    }
}

void set_intersect(struct HashSet *a, struct HashSet *b, struct HashSet *result) {
    int i;
    set_init(result);
    for (i = 0; i < SET_SIZE; i++) {
        if (a->data[i] != EMPTY_SLOT) {
            if (set_contains(b, a->data[i])) {
                set_add(result, a->data[i]);
            }
        }
    }
}

void set_difference(struct HashSet *a, struct HashSet *b, struct HashSet *result) {
    int i;
    set_init(result);
    for (i = 0; i < SET_SIZE; i++) {
        if (a->data[i] != EMPTY_SLOT) {
            if (!set_contains(b, a->data[i])) {
                set_add(result, a->data[i]);
            }
        }
    }
}

void sort_array(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

int main() {
    struct HashSet a;
    struct HashSet b;
    struct HashSet result;
    int arr[32];
    int n;
    int i;

    set_init(&a);
    set_init(&b);

    set_add(&a, 1);
    set_add(&a, 3);
    set_add(&a, 5);
    set_add(&a, 7);
    set_add(&a, 9);

    printf("%d\n", a.count);
    // EXPECT: 5

    set_add(&b, 2);
    set_add(&b, 3);
    set_add(&b, 5);
    set_add(&b, 8);
    set_add(&b, 9);
    set_add(&b, 11);

    printf("%d\n", b.count);
    // EXPECT: 6

    printf("%d\n", set_contains(&a, 5));
    // EXPECT: 1

    printf("%d\n", set_contains(&a, 4));
    // EXPECT: 0

    set_add(&a, 5);
    printf("%d\n", a.count);
    // EXPECT: 5

    set_union(&a, &b, &result);
    printf("%d\n", result.count);
    // EXPECT: 8

    set_to_array(&result, arr, &n);
    sort_array(arr, n);
    for (i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
    // EXPECT: 1
    // EXPECT: 2
    // EXPECT: 3
    // EXPECT: 5
    // EXPECT: 7
    // EXPECT: 8
    // EXPECT: 9
    // EXPECT: 11

    set_intersect(&a, &b, &result);
    printf("%d\n", result.count);
    // EXPECT: 3

    set_to_array(&result, arr, &n);
    sort_array(arr, n);
    for (i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
    // EXPECT: 3
    // EXPECT: 5
    // EXPECT: 9

    set_difference(&a, &b, &result);
    printf("%d\n", result.count);
    // EXPECT: 2

    set_to_array(&result, arr, &n);
    sort_array(arr, n);
    for (i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
    // EXPECT: 1
    // EXPECT: 7

    set_difference(&b, &a, &result);
    printf("%d\n", result.count);
    // EXPECT: 3

    set_to_array(&result, arr, &n);
    sort_array(arr, n);
    for (i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
    // EXPECT: 2
    // EXPECT: 8
    // EXPECT: 11

    return 0;
}
