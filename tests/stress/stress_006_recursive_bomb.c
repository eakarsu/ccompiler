// stress_006_recursive_bomb.c - Heavy recursion stress test
// EXPECT: ackermann=61
// EXPECT: tree_sum=120
// EXPECT: mutual=79
// EXPECT: quicksort=1
// EXPECT: mergesort=1
// EXPECT: fib20=6765
// EXPECT: hanoi=1023
// EXPECT: power=1048576
// EXPECT: gcd=6
int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// --- Ackermann function (small args) ---
int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

// --- Recursive tree building and summing ---
struct tnode {
    int val;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *make_tree(int depth, int val) {
    if (depth == 0) return 0;
    struct tnode *n = (struct tnode *)malloc(sizeof(struct tnode));
    n->val = val;
    n->left = make_tree(depth - 1, val * 2);
    n->right = make_tree(depth - 1, val * 2 + 1);
    return n;
}

int sum_tree(struct tnode *n) {
    if (!n) return 0;
    return n->val + sum_tree(n->left) + sum_tree(n->right);
}

void free_tree(struct tnode *n) {
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

// --- Mutual recursion: 4-way A->B->C->D->A ---
int mut_a(int n);
int mut_b(int n);
int mut_c(int n);
int mut_d(int n);

int mut_a(int n) { if (n <= 0) return 1; return n + mut_b(n - 1); }
int mut_b(int n) { if (n <= 0) return 2; return n + mut_c(n - 1); }
int mut_c(int n) { if (n <= 0) return 3; return n + mut_d(n - 1); }
int mut_d(int n) { if (n <= 0) return 4; return n + mut_a(n - 1); }

// --- Recursive quicksort ---
void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

int partition(int *arr, int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    int j;
    for (j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

void quicksort(int *arr, int lo, int hi) {
    if (lo < hi) {
        int p = partition(arr, lo, hi);
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

// --- Recursive mergesort ---
void merge(int *arr, int *tmp, int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= hi) tmp[k++] = arr[j++];
    for (i = lo; i <= hi; i++) arr[i] = tmp[i];
}

void mergesort(int *arr, int *tmp, int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mergesort(arr, tmp, lo, mid);
        mergesort(arr, tmp, mid + 1, hi);
        merge(arr, tmp, lo, mid, hi);
    }
}

// --- Recursive fibonacci with memoization ---
int fib_memo[50];
int fib(int n) {
    if (n <= 1) return n;
    if (fib_memo[n] != 0) return fib_memo[n];
    fib_memo[n] = fib(n - 1) + fib(n - 2);
    return fib_memo[n];
}

// --- Tower of Hanoi counting moves ---
int hanoi_count;
void hanoi(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    hanoi_count++;
    hanoi(n - 1, aux, to, from);
}

// --- Recursive power ---
long power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        long half = power(base, exp / 2);
        return half * half;
    }
    return base * power(base, exp - 1);
}

// --- Recursive GCD ---
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(void) {
    // Ackermann(3,3) = 61
    printf("ackermann=%d\n", ackermann(3, 3));

    // Tree of depth 4, root=1
    struct tnode *tree = make_tree(4, 1);
    int tsum = sum_tree(tree);
    printf("tree_sum=%d\n", tsum);
    free_tree(tree);

    // Mutual recursion: mut_a(12)
    printf("mutual=%d\n", mut_a(12));

    // Quicksort test
    int qa[10] = {9, 3, 7, 1, 8, 2, 6, 4, 10, 5};
    quicksort(qa, 0, 9);
    int sorted_ok = 1;
    int i;
    for (i = 0; i < 10; i++) {
        if (qa[i] != i + 1) sorted_ok = 0;
    }
    printf("quicksort=%d\n", sorted_ok);

    // Mergesort test
    int ma[10] = {10, 4, 6, 2, 8, 1, 9, 3, 5, 7};
    int tmp[10];
    mergesort(ma, tmp, 0, 9);
    sorted_ok = 1;
    for (i = 0; i < 10; i++) {
        if (ma[i] != i + 1) sorted_ok = 0;
    }
    printf("mergesort=%d\n", sorted_ok);

    // Fibonacci(20) = 6765
    for (i = 0; i < 50; i++) fib_memo[i] = 0;
    printf("fib20=%d\n", fib(20));

    // Hanoi(10) = 1023 moves
    hanoi_count = 0;
    hanoi(10, 0, 2, 1);
    printf("hanoi=%d\n", hanoi_count);

    // power(2,20) = 1048576
    printf("power=%ld\n", power(2, 20));

    // gcd(48, 18) = 6
    printf("gcd=%d\n", gcd(48, 18));

    return 0;
}
