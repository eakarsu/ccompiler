int printf(const char *fmt, ...);

/* test_027_void_ptr.c
   Tests: void* as generic pointer, cast to/from void*, void* function params,
          void* return value, void* array, generic swap
*/

/* generic swap of two objects of given byte size */
void swap_bytes(void *a, void *b, int size) {
    char *ca = (char *)a;
    char *cb = (char *)b;
    int i;
    for (i = 0; i < size; i++) {
        char tmp = ca[i];
        ca[i] = cb[i];
        cb[i] = tmp;
    }
}

/* generic fill: set every byte in region to val */
void fill_bytes(void *dst, int size, int val) {
    char *p = (char *)dst;
    int i;
    for (i = 0; i < size; i++) p[i] = (char)val;
}

/* return void* to element at index, treating buffer as int array */
void *int_at(void *base, int idx) {
    char *p = (char *)base;
    return (void *)(p + idx * 4);
}

/* sum int array via void* pointer */
int sum_via_void(void *data, int n) {
    int *p = (int *)data;
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += p[i];
    return s;
}

/* copy memory via void* */
void mem_copy(void *dst, const void *src, int n) {
    char *d = (char *)dst;
    const char *s = (const char *)src;
    int i;
    for (i = 0; i < n; i++) d[i] = s[i];
}

/* find first int equal to target in void* array, return index or -1 */
int find_int(void *data, int n, int target) {
    int *p = (int *)data;
    int i;
    for (i = 0; i < n; i++) {
        if (p[i] == target) return i;
    }
    return -1;
}

int main(void) {
    /* basic void* cast to int* and dereference */
    int x = 42;
    void *vp = &x;
    int *ip = (int *)vp;
    printf("%d\n", *ip);
    // EXPECT: 42

    /* write through void* (cast first) */
    *(int *)vp = 77;
    printf("%d\n", x);
    // EXPECT: 77

    /* void* to char* */
    char c = 'Z';
    void *vcp = &c;
    char *cp = (char *)vcp;
    printf("%c\n", *cp);
    // EXPECT: Z

    /* swap_bytes for ints */
    int a = 10, b = 20;
    swap_bytes(&a, &b, 4);
    printf("%d\n", a);
    // EXPECT: 20
    printf("%d\n", b);
    // EXPECT: 10

    /* swap_bytes for chars */
    char ca = 'A', cb = 'B';
    swap_bytes(&ca, &cb, 1);
    printf("%c\n", ca);
    // EXPECT: B
    printf("%c\n", cb);
    // EXPECT: A

    /* fill_bytes on int array */
    int arr[4] = {1, 2, 3, 4};
    fill_bytes(arr, 16, 0);
    printf("%d\n", arr[0]);
    // EXPECT: 0
    printf("%d\n", arr[3]);
    // EXPECT: 0

    /* int_at: void* return for element access */
    int data[5] = {10, 20, 30, 40, 50};
    void *elem = int_at(data, 2);
    printf("%d\n", *(int *)elem);
    // EXPECT: 30

    elem = int_at(data, 0);
    printf("%d\n", *(int *)elem);
    // EXPECT: 10

    elem = int_at(data, 4);
    printf("%d\n", *(int *)elem);
    // EXPECT: 50

    /* modify via void* return */
    *(int *)int_at(data, 1) = 99;
    printf("%d\n", data[1]);
    // EXPECT: 99

    /* sum_via_void */
    int nums[4] = {1, 2, 3, 4};
    printf("%d\n", sum_via_void(nums, 4));
    // EXPECT: 10

    /* mem_copy via void* */
    int src[3] = {7, 8, 9};
    int dst[3] = {0, 0, 0};
    mem_copy(dst, src, 12);
    printf("%d\n", dst[0]);
    // EXPECT: 7
    printf("%d\n", dst[1]);
    // EXPECT: 8
    printf("%d\n", dst[2]);
    // EXPECT: 9

    /* array of void* pointing to various ints */
    int v0 = 100, v1 = 200, v2 = 300;
    void *vptrs[3] = {&v0, &v1, &v2};
    printf("%d\n", *(int *)vptrs[0]);
    // EXPECT: 100
    printf("%d\n", *(int *)vptrs[1]);
    // EXPECT: 200
    printf("%d\n", *(int *)vptrs[2]);
    // EXPECT: 300

    /* modify via void* array element */
    *(int *)vptrs[1] = 999;
    printf("%d\n", v1);
    // EXPECT: 999

    /* find_int */
    int search[6] = {5, 3, 8, 1, 9, 2};
    printf("%d\n", find_int(search, 6, 8));
    // EXPECT: 2
    printf("%d\n", find_int(search, 6, 5));
    // EXPECT: 0
    printf("%d\n", find_int(search, 6, 2));
    // EXPECT: 5
    printf("%d\n", find_int(search, 6, 7));
    // EXPECT: -1

    /* pass void* array to functions in sequence */
    int chain[3] = {1, 1, 1};
    *(int *)int_at(chain, 0) = 10;
    *(int *)int_at(chain, 1) = 20;
    *(int *)int_at(chain, 2) = 30;
    printf("%d\n", sum_via_void(chain, 3));
    // EXPECT: 60

    /* void* null check */
    void *null_vp = 0;
    if (null_vp == 0) printf("null\n");
    // EXPECT: null

    /* reassign void* */
    int p1 = 11, p2 = 22;
    void *vr = &p1;
    printf("%d\n", *(int *)vr);
    // EXPECT: 11
    vr = &p2;
    printf("%d\n", *(int *)vr);
    // EXPECT: 22

    return 0;
}
