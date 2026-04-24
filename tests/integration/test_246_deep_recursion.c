int printf(const char *fmt, ...);
// EXPECT: sum(100)=5050\nsum(500)=125250\nsum(1000)=500500\ncount(500)=500\nack(0,0)=1\nack(1,1)=3\nack(2,2)=7\nack(2,3)=9\nack(3,2)=29\nack(3,4)=125\ndeep_add(0,100)=100\ndeep_add(0,500)=500\nrec_mul(7,8)=56\nrec_mul(13,11)=143\nrec_mul(-3,5)=-15\nrec_div(100,7)=14\nrec_div(42,6)=7\nfib(15)=987\ntrib(12)=927\ntail_sum(1000)=500500\nchain(30)=30\nchain(99)=99
// Test 246: Stress test - deep recursion

int sum_to_n(int n) {
    if (n <= 0) return 0;
    return n + sum_to_n(n - 1);
}

int count_down(int n) {
    if (n <= 0) return 0;
    return 1 + count_down(n - 1);
}

int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int deeply_nested_add(int a, int depth) {
    if (depth <= 0) return a;
    return deeply_nested_add(a + 1, depth - 1);
}

int recursive_multiply(int a, int b) {
    if (b == 0) return 0;
    if (b < 0) return -recursive_multiply(a, -b);
    return a + recursive_multiply(a, b - 1);
}

int recursive_divide(int a, int b) {
    if (a < b) return 0;
    return 1 + recursive_divide(a - b, b);
}

int binary_recursion(int n) {
    if (n <= 1) return 1;
    return binary_recursion(n - 1) + binary_recursion(n - 2);
}

int ternary_recursion(int n) {
    if (n <= 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    return ternary_recursion(n - 1) + ternary_recursion(n - 2) + ternary_recursion(n - 3);
}

int tail_sum(int n, int acc) {
    if (n <= 0) return acc;
    return tail_sum(n - 1, acc + n);
}

int chain_a(int n);
int chain_b(int n);
int chain_c(int n);

int chain_a(int n) {
    if (n <= 0) return 0;
    return 1 + chain_b(n - 1);
}
int chain_b(int n) {
    if (n <= 0) return 0;
    return 1 + chain_c(n - 1);
}
int chain_c(int n) {
    if (n <= 0) return 0;
    return 1 + chain_a(n - 1);
}

int main(void) {
    printf("sum(100)=%d\n", sum_to_n(100));
    printf("sum(500)=%d\n", sum_to_n(500));
    printf("sum(1000)=%d\n", sum_to_n(1000));

    printf("count(500)=%d\n", count_down(500));

    printf("ack(0,0)=%d\n", ackermann(0, 0));
    printf("ack(1,1)=%d\n", ackermann(1, 1));
    printf("ack(2,2)=%d\n", ackermann(2, 2));
    printf("ack(2,3)=%d\n", ackermann(2, 3));
    printf("ack(3,2)=%d\n", ackermann(3, 2));
    printf("ack(3,4)=%d\n", ackermann(3, 4));

    printf("deep_add(0,100)=%d\n", deeply_nested_add(0, 100));
    printf("deep_add(0,500)=%d\n", deeply_nested_add(0, 500));

    printf("rec_mul(7,8)=%d\n", recursive_multiply(7, 8));
    printf("rec_mul(13,11)=%d\n", recursive_multiply(13, 11));
    printf("rec_mul(-3,5)=%d\n", recursive_multiply(-3, 5));

    printf("rec_div(100,7)=%d\n", recursive_divide(100, 7));
    printf("rec_div(42,6)=%d\n", recursive_divide(42, 6));

    printf("fib(15)=%d\n", binary_recursion(15));
    printf("trib(12)=%d\n", ternary_recursion(12));

    printf("tail_sum(1000)=%d\n", tail_sum(1000, 0));

    printf("chain(30)=%d\n", chain_a(30));
    printf("chain(99)=%d\n", chain_a(99));

    return 0;
}
