int printf(const char *fmt, ...);
// EXPECT: === C(5,2) ===\n1 2\n1 3\n1 4\n1 5\n2 3\n2 4\n2 5\n3 4\n3 5\n4 5\ncount: 10\n=== C(5,3) ===\n1 2 3\n1 2 4\n1 2 5\n1 3 4\n1 3 5\n1 4 5\n2 3 4\n2 3 5\n2 4 5\n3 4 5\ncount: 10\n=== choose formula ===\nC(0,0)=1 \nC(1,0)=1 C(1,1)=1 \nC(2,0)=1 C(2,1)=2 C(2,2)=1 \nC(3,0)=1 C(3,1)=3 C(3,2)=3 C(3,3)=1 \nC(4,0)=1 C(4,1)=4 C(4,2)=6 C(4,3)=4 C(4,4)=1 \nC(5,0)=1 C(5,1)=5 C(5,2)=10 C(5,3)=10 C(5,4)=5 C(5,5)=1 \nC(6,0)=1 C(6,1)=6 C(6,2)=15 C(6,3)=20 C(6,4)=15 C(6,5)=6 C(6,6)=1 \n=== subsets of {1,2,3} ===\n{}\n{3}\n{2}\n{2,3}\n{1}\n{1,3}\n{1,2}\n{1,2,3}
// Test 216: Recursive combination generation

int combo_count;
int combo[20];

void print_combo(int len) {
    int i = 0;
    while (i < len) {
        if (i > 0) printf(" ");
        printf("%d", combo[i]);
        i = i + 1;
    }
    printf("\n");
}

void combinations(int *arr, int n, int k, int start, int depth) {
    if (depth == k) {
        print_combo(k);
        combo_count = combo_count + 1;
        return;
    }
    int i = start;
    while (i < n) {
        combo[depth] = arr[i];
        combinations(arr, n, k, i + 1, depth + 1);
        i = i + 1;
    }
}

int choose(int n, int k) {
    if (k == 0 || k == n) return 1;
    return choose(n - 1, k - 1) + choose(n - 1, k);
}

void subsets(int *arr, int n, int idx, int *subset, int sublen) {
    if (idx == n) {
        printf("{");
        int i = 0;
        while (i < sublen) {
            if (i > 0) printf(",");
            printf("%d", subset[i]);
            i = i + 1;
        }
        printf("}\n");
        return;
    }
    subsets(arr, n, idx + 1, subset, sublen);
    subset[sublen] = arr[idx];
    subsets(arr, n, idx + 1, subset, sublen + 1);
}

int main(void) {
    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;

    printf("=== C(5,2) ===\n");
    combo_count = 0;
    combinations(arr, 5, 2, 0, 0);
    printf("count: %d\n", combo_count);

    printf("=== C(5,3) ===\n");
    combo_count = 0;
    combinations(arr, 5, 3, 0, 0);
    printf("count: %d\n", combo_count);

    printf("=== choose formula ===\n");
    int n = 0;
    while (n <= 6) {
        int k = 0;
        while (k <= n) {
            printf("C(%d,%d)=%d ", n, k, choose(n, k));
            k = k + 1;
        }
        printf("\n");
        n = n + 1;
    }

    printf("=== subsets of {1,2,3} ===\n");
    int small[3];
    small[0] = 1; small[1] = 2; small[2] = 3;
    int subset[3];
    subsets(small, 3, 0, subset, 0);

    return 0;
}
