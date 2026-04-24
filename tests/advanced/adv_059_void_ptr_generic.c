// EXPECT: === Void Pointer Generic Patterns ===
// EXPECT: swap_int: a=20 b=10
// EXPECT: swap_char: a=y b=x
// EXPECT: swap_struct: a=(3,4) b=(1,2)
// EXPECT: generic_max_int: 42
// EXPECT: generic_max_char: z
// EXPECT: sorted_ints: 1 2 3 4 5
// EXPECT: sorted_desc: 5 4 3 2 1
// EXPECT: void_array: 10 20 30 40 50
// EXPECT: generic_find: found 30 at index 2
// EXPECT: generic_find: 99 not found
// EXPECT: generic_copy: 1 2 3 4 5
// EXPECT: generic_fill: 7 7 7 7 7
// EXPECT: generic_reverse: 5 4 3 2 1
// EXPECT: callback_apply: 2 4 6 8 10
// EXPECT: generic_reduce: sum=15
// EXPECT: accumulate_product: 120
// EXPECT: type_tagged: int=42 char=A
// EXPECT: void_ptr_chain: 100

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

struct Point {
    int x;
    int y;
};

// Generic swap: swaps n bytes at two addresses
void generic_swap(void *a, void *b, int size) {
    char *pa = (char *)a;
    char *pb = (char *)b;
    int i;
    for (i = 0; i < size; i++) {
        char tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

// Generic copy: copies n elements of elem_size
void generic_copy(void *dst, void *src, int count, int elem_size) {
    char *d = (char *)dst;
    char *s = (char *)src;
    int total = count * elem_size;
    int i;
    for (i = 0; i < total; i++) {
        d[i] = s[i];
    }
}

// Generic fill: fill n elements with pattern
void generic_fill(void *arr, void *value, int count, int elem_size) {
    char *a = (char *)arr;
    char *v = (char *)value;
    int i;
    int j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < elem_size; j++) {
            a[i * elem_size + j] = v[j];
        }
    }
}

// Generic reverse
void generic_reverse(void *arr, int count, int elem_size) {
    int left = 0;
    int right = count - 1;
    char *a = (char *)arr;
    while (left < right) {
        // Swap elements at left and right
        int i;
        for (i = 0; i < elem_size; i++) {
            char tmp = a[left * elem_size + i];
            a[left * elem_size + i] = a[right * elem_size + i];
            a[right * elem_size + i] = tmp;
        }
        left++;
        right--;
    }
}

// Comparison functions
int cmp_int_asc(void *a, void *b) {
    int ia = *(int *)a;
    int ib = *(int *)b;
    return ia - ib;
}

int cmp_int_desc(void *a, void *b) {
    int ia = *(int *)a;
    int ib = *(int *)b;
    return ib - ia;
}

int cmp_char(void *a, void *b) {
    char ca = *(char *)a;
    char cb = *(char *)b;
    return ca - cb;
}

// Generic bubble sort
void generic_sort(void *arr, int count, int elem_size,
                  int (*cmp)(void *, void *)) {
    char *a = (char *)arr;
    int i;
    int j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - 1 - i; j++) {
            void *pj = a + j * elem_size;
            void *pj1 = a + (j + 1) * elem_size;
            if (cmp(pj, pj1) > 0) {
                generic_swap(pj, pj1, elem_size);
            }
        }
    }
}

// Generic find
int generic_find(void *arr, int count, int elem_size,
                 void *target, int (*cmp)(void *, void *)) {
    char *a = (char *)arr;
    int i;
    for (i = 0; i < count; i++) {
        if (cmp(a + i * elem_size, target) == 0) {
            return i;
        }
    }
    return -1;
}

// Generic max
void *generic_max(void *arr, int count, int elem_size,
                  int (*cmp)(void *, void *)) {
    char *a = (char *)arr;
    char *best = a;
    int i;
    for (i = 1; i < count; i++) {
        if (cmp(a + i * elem_size, best) > 0) {
            best = a + i * elem_size;
        }
    }
    return best;
}

// Callback: apply function to each element
typedef int (*IntTransform)(int);

int double_val(int x) { return x * 2; }

void apply_int(int *arr, int count, IntTransform fn) {
    int i;
    for (i = 0; i < count; i++) {
        arr[i] = fn(arr[i]);
    }
}

// Generic reduce
typedef int (*IntReducer)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int reduce_int(int *arr, int count, int init, IntReducer fn) {
    int acc = init;
    int i;
    for (i = 0; i < count; i++) {
        acc = fn(acc, arr[i]);
    }
    return acc;
}

// Tagged union via void pointer
struct Tagged {
    int type;  // 0=int, 1=char
    void *data;
};

int main(void) {
    printf("=== Void Pointer Generic Patterns ===\n");

    // Swap ints
    {
        int a = 10;
        int b = 20;
        generic_swap(&a, &b, sizeof(int));
        printf("swap_int: a=%d b=%d\n", a, b);
    }

    // Swap chars
    {
        char a = 'x';
        char b = 'y';
        generic_swap(&a, &b, sizeof(char));
        printf("swap_char: a=%c b=%c\n", a, b);
    }

    // Swap structs
    {
        struct Point a;
        struct Point b;
        a.x = 1; a.y = 2;
        b.x = 3; b.y = 4;
        generic_swap(&a, &b, sizeof(struct Point));
        printf("swap_struct: a=(%d,%d) b=(%d,%d)\n", a.x, a.y, b.x, b.y);
    }

    // Generic max of ints
    {
        int arr[5];
        arr[0] = 10; arr[1] = 42; arr[2] = 7; arr[3] = 33; arr[4] = 25;
        int *mx = (int *)generic_max(arr, 5, sizeof(int), cmp_int_asc);
        printf("generic_max_int: %d\n", *mx);
    }

    // Generic max of chars
    {
        char arr[4];
        arr[0] = 'a'; arr[1] = 'z'; arr[2] = 'm'; arr[3] = 'b';
        char *mx = (char *)generic_max(arr, 4, sizeof(char), cmp_char);
        printf("generic_max_char: %c\n", *mx);
    }

    // Sort ints ascending
    {
        int arr[5];
        arr[0] = 5; arr[1] = 3; arr[2] = 1; arr[3] = 4; arr[4] = 2;
        generic_sort(arr, 5, sizeof(int), cmp_int_asc);
        printf("sorted_ints: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Sort ints descending
    {
        int arr[5];
        arr[0] = 1; arr[1] = 4; arr[2] = 2; arr[3] = 5; arr[4] = 3;
        generic_sort(arr, 5, sizeof(int), cmp_int_desc);
        printf("sorted_desc: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Void pointer array (array of void*)
    {
        int a = 10;
        int b = 20;
        int c = 30;
        int d = 40;
        int e = 50;
        void *ptrs[5];
        ptrs[0] = &a;
        ptrs[1] = &b;
        ptrs[2] = &c;
        ptrs[3] = &d;
        ptrs[4] = &e;
        printf("void_array: %d %d %d %d %d\n",
               *(int *)ptrs[0], *(int *)ptrs[1], *(int *)ptrs[2],
               *(int *)ptrs[3], *(int *)ptrs[4]);
    }

    // Generic find
    {
        int arr[5];
        arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
        int target = 30;
        int idx = generic_find(arr, 5, sizeof(int), &target, cmp_int_asc);
        printf("generic_find: found %d at index %d\n", target, idx);

        target = 99;
        idx = generic_find(arr, 5, sizeof(int), &target, cmp_int_asc);
        printf("generic_find: %d not found\n", target);
    }

    // Generic copy
    {
        int src[5];
        int dst[5];
        src[0] = 1; src[1] = 2; src[2] = 3; src[3] = 4; src[4] = 5;
        generic_copy(dst, src, 5, sizeof(int));
        printf("generic_copy: %d %d %d %d %d\n",
               dst[0], dst[1], dst[2], dst[3], dst[4]);
    }

    // Generic fill
    {
        int arr[5];
        int val = 7;
        generic_fill(arr, &val, 5, sizeof(int));
        printf("generic_fill: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Generic reverse
    {
        int arr[5];
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
        generic_reverse(arr, 5, sizeof(int));
        printf("generic_reverse: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Apply callback
    {
        int arr[5];
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
        apply_int(arr, 5, double_val);
        printf("callback_apply: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Reduce (sum)
    {
        int arr[5];
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
        int sum = reduce_int(arr, 5, 0, add);
        printf("generic_reduce: sum=%d\n", sum);
    }

    // Reduce (product)
    {
        int arr[5];
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
        int prod = reduce_int(arr, 5, 1, mul);
        printf("accumulate_product: %d\n", prod);
    }

    // Tagged union pattern
    {
        int ival = 42;
        char cval = 'A';

        struct Tagged tags[2];
        tags[0].type = 0;
        tags[0].data = &ival;
        tags[1].type = 1;
        tags[1].data = &cval;

        printf("type_tagged:");
        int i;
        for (i = 0; i < 2; i++) {
            if (tags[i].type == 0) {
                printf(" int=%d", *(int *)tags[i].data);
            } else {
                printf(" char=%c", *(char *)tags[i].data);
            }
        }
        printf("\n");
    }

    // Chained void pointer (pointer to pointer pattern)
    {
        int val = 100;
        void *p1 = &val;
        void *p2 = &p1;
        // Dereference: p2 -> p1 -> val
        void *inner = *(void **)p2;
        int result = *(int *)inner;
        printf("void_ptr_chain: %d\n", result);
    }

    return 0;
}
