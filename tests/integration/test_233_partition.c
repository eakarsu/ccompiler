int printf(const char *fmt, ...);
// EXPECT: === partition counts ===\np(0)=1\np(1)=1\np(2)=2\np(3)=3\np(4)=5\np(5)=7\np(6)=11\np(7)=15\np(8)=22\np(9)=30\np(10)=42\np(11)=56\np(12)=77\n=== partitions of 5 ===\n5\n4+1\n3+2\n3+1+1\n2+2+1\n2+1+1+1\n1+1+1+1+1\n=== partitions of 6 ===\n6\n5+1\n4+2\n4+1+1\n3+3\n3+2+1\n3+1+1+1\n2+2+2\n2+2+1+1\n2+1+1+1+1\n1+1+1+1+1+1\n=== distinct partitions ===\ndp(1)=1\ndp(2)=1\ndp(3)=2\ndp(4)=2\ndp(5)=3\ndp(6)=4\ndp(7)=5\ndp(8)=6\ndp(9)=8\ndp(10)=10\n=== partition into k parts ===\np(6,2)=3\np(6,3)=3\np(8,3)=5\n=== odd partitions ===\nop(1)=1\nop(2)=1\nop(3)=2\nop(4)=2\nop(5)=3\nop(6)=4\nop(7)=5\nop(8)=6\nop(9)=8\nop(10)=10
// Test 233: Recursive integer partition

int partition_count(int n, int max_val) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    if (max_val == 0) return 0;
    return partition_count(n - max_val, max_val) + partition_count(n, max_val - 1);
}

int parts[50];

void print_partition(int len) {
    int i = 0;
    while (i < len) {
        if (i > 0) printf("+");
        printf("%d", parts[i]);
        i = i + 1;
    }
    printf("\n");
}

void generate_partitions(int n, int max_val, int depth) {
    if (n == 0) {
        print_partition(depth);
        return;
    }
    int i = max_val;
    if (i > n) i = n;
    while (i >= 1) {
        parts[depth] = i;
        generate_partitions(n - i, i, depth + 1);
        i = i - 1;
    }
}

int distinct_partition_count(int n, int max_val) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    if (max_val == 0) return 0;
    return distinct_partition_count(n - max_val, max_val - 1) +
           distinct_partition_count(n, max_val - 1);
}

int partition_into_k(int n, int k, int max_val) {
    if (n == 0 && k == 0) return 1;
    if (n <= 0 || k <= 0) return 0;
    if (max_val <= 0) return 0;
    return partition_into_k(n - max_val, k - 1, max_val) +
           partition_into_k(n, k, max_val - 1);
}

int odd_partition_count(int n, int max_odd) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    if (max_odd <= 0) return 0;
    int next_odd = max_odd - 2;
    return odd_partition_count(n - max_odd, max_odd) +
           odd_partition_count(n, next_odd);
}

int next_odd(int n) {
    if ((n / 2) * 2 == n) return n - 1;
    return n;
}

int main(void) {
    printf("=== partition counts ===\n");
    int i = 0;
    while (i <= 12) {
        printf("p(%d)=%d\n", i, partition_count(i, i));
        i = i + 1;
    }

    printf("=== partitions of 5 ===\n");
    generate_partitions(5, 5, 0);

    printf("=== partitions of 6 ===\n");
    generate_partitions(6, 6, 0);

    printf("=== distinct partitions ===\n");
    i = 1;
    while (i <= 10) {
        printf("dp(%d)=%d\n", i, distinct_partition_count(i, i));
        i = i + 1;
    }

    printf("=== partition into k parts ===\n");
    printf("p(6,2)=%d\n", partition_into_k(6, 2, 6));
    printf("p(6,3)=%d\n", partition_into_k(6, 3, 6));
    printf("p(8,3)=%d\n", partition_into_k(8, 3, 8));

    printf("=== odd partitions ===\n");
    i = 1;
    while (i <= 10) {
        printf("op(%d)=%d\n", i, odd_partition_count(i, next_odd(i)));
        i = i + 1;
    }

    return 0;
}
