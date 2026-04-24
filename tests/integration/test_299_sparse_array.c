int printf(const char *fmt, ...);
// EXPECT: sparse: [5 0 0 10 0 0 0 15 0 20] (nnz=4)\nget[0]=5 get[1]=0 get[3]=10 get[7]=15\nafter update[3]=100: get[3]=100\nafter remove[7]: [5 0 0 100 0 0 0 0 0 20] (nnz=3)\nsb: [2 0 0 3 0 7 0 0 0 0] (nnz=3)\ndot(sa, sb) = 310\nsa + sb: [7 0 0 103 0 7 0 0 0 20] (nnz=4)
// Test: sparse array representation

struct SparseEntry {
    int index;
    int value;
};

struct SparseArray {
    struct SparseEntry entries[20];
    int count;
    int size;  // logical size
};

void sparse_init(struct SparseArray *sa, int size) {
    sa->count = 0;
    sa->size = size;
}

void sparse_set(struct SparseArray *sa, int idx, int val) {
    // Check if already exists
    int i;
    for (i = 0; i < sa->count; i++) {
        if (sa->entries[i].index == idx) {
            if (val == 0) {
                // Remove by shifting
                int j;
                for (j = i; j < sa->count - 1; j++) {
                    sa->entries[j] = sa->entries[j + 1];
                }
                sa->count = sa->count - 1;
            } else {
                sa->entries[i].value = val;
            }
            return;
        }
    }
    if (val != 0) {
        sa->entries[sa->count].index = idx;
        sa->entries[sa->count].value = val;
        sa->count = sa->count + 1;
    }
}

int sparse_get(struct SparseArray *sa, int idx) {
    int i;
    for (i = 0; i < sa->count; i++) {
        if (sa->entries[i].index == idx) {
            return sa->entries[i].value;
        }
    }
    return 0;
}

void sparse_print(struct SparseArray *sa) {
    printf("[");
    int i;
    for (i = 0; i < sa->size; i++) {
        if (i > 0) printf(" ");
        printf("%d", sparse_get(sa, i));
    }
    printf("] (nnz=%d)\n", sa->count);
}

// Dot product of two sparse arrays
int sparse_dot(struct SparseArray *a, struct SparseArray *b) {
    int sum = 0;
    int i; int j;
    for (i = 0; i < a->count; i++) {
        for (j = 0; j < b->count; j++) {
            if (a->entries[i].index == b->entries[j].index) {
                sum = sum + a->entries[i].value * b->entries[j].value;
            }
        }
    }
    return sum;
}

// Add two sparse arrays
void sparse_add(struct SparseArray *a, struct SparseArray *b, struct SparseArray *c) {
    int max_size = a->size;
    if (b->size > max_size) max_size = b->size;
    sparse_init(c, max_size);
    int i;
    for (i = 0; i < a->count; i++) {
        sparse_set(c, a->entries[i].index, a->entries[i].value);
    }
    for (i = 0; i < b->count; i++) {
        int cur = sparse_get(c, b->entries[i].index);
        sparse_set(c, b->entries[i].index, cur + b->entries[i].value);
    }
}

int main(void) {
    struct SparseArray sa;
    sparse_init(&sa, 10);

    // Set some values
    sparse_set(&sa, 0, 5);
    sparse_set(&sa, 3, 10);
    sparse_set(&sa, 7, 15);
    sparse_set(&sa, 9, 20);

    printf("sparse: ");
    sparse_print(&sa);

    // Get values
    printf("get[0]=%d get[1]=%d get[3]=%d get[7]=%d\n",
           sparse_get(&sa, 0), sparse_get(&sa, 1),
           sparse_get(&sa, 3), sparse_get(&sa, 7));

    // Update
    sparse_set(&sa, 3, 100);
    printf("after update[3]=100: get[3]=%d\n", sparse_get(&sa, 3));

    // Remove (set to 0)
    sparse_set(&sa, 7, 0);
    printf("after remove[7]: ");
    sparse_print(&sa);

    // Second sparse array
    struct SparseArray sb;
    sparse_init(&sb, 10);
    sparse_set(&sb, 0, 2);
    sparse_set(&sb, 3, 3);
    sparse_set(&sb, 5, 7);

    printf("sb: ");
    sparse_print(&sb);

    // Dot product
    printf("dot(sa, sb) = %d\n", sparse_dot(&sa, &sb));

    // Add
    struct SparseArray sc;
    sparse_add(&sa, &sb, &sc);
    printf("sa + sb: ");
    sparse_print(&sc);

    return 0;
}
