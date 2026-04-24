int printf(const char *fmt, ...);
// EXPECT: is_even(0)=1 is_odd(0)=0\nis_even(1)=0 is_odd(1)=1\nis_even(2)=1 is_odd(2)=0\nis_even(3)=0 is_odd(3)=1\nis_even(4)=1 is_odd(4)=0\nis_even(5)=0 is_odd(5)=1\nis_even(6)=1 is_odd(6)=0\nis_even(7)=0 is_odd(7)=1\nis_even(8)=1 is_odd(8)=0\nis_even(9)=0 is_odd(9)=1\nis_even(10)=1 is_odd(10)=0\nfunc_a(5)=7\nfunc_b(5)=14\nfunc_a(8)=31\nfunc_b(8)=30\nping(10)=10\npong(10)=10\ndescent_a(9)=9\ndescent_b(9)=9\ndescent_c(9)=9
// Test 203: Mutual recursion (even/odd check and more)

int is_odd(int n);

int is_even(int n) {
    if (n == 0) return 1;
    if (n < 0) return is_even(-n);
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n == 0) return 0;
    if (n < 0) return is_odd(-n);
    return is_even(n - 1);
}

int func_a(int n);
int func_b(int n);

int func_a(int n) {
    if (n <= 0) return 1;
    return func_b(n - 1) + 1;
}

int func_b(int n) {
    if (n <= 0) return 0;
    return func_a(n - 1) * 2;
}

int ping(int n);
int pong(int n);

int ping(int n) {
    if (n <= 0) return 0;
    return 1 + pong(n - 1);
}

int pong(int n) {
    if (n <= 0) return 0;
    return 1 + ping(n - 1);
}

int descent_a(int n);
int descent_b(int n);
int descent_c(int n);

int descent_a(int n) {
    if (n <= 0) return 0;
    return 1 + descent_b(n - 1);
}

int descent_b(int n) {
    if (n <= 0) return 0;
    return 1 + descent_c(n - 1);
}

int descent_c(int n) {
    if (n <= 0) return 0;
    return 1 + descent_a(n - 1);
}

int main(void) {
    int i = 0;
    while (i <= 10) {
        printf("is_even(%d)=%d ", i, is_even(i));
        printf("is_odd(%d)=%d\n", i, is_odd(i));
        i = i + 1;
    }

    printf("func_a(5)=%d\n", func_a(5));
    printf("func_b(5)=%d\n", func_b(5));
    printf("func_a(8)=%d\n", func_a(8));
    printf("func_b(8)=%d\n", func_b(8));

    printf("ping(10)=%d\n", ping(10));
    printf("pong(10)=%d\n", pong(10));

    printf("descent_a(9)=%d\n", descent_a(9));
    printf("descent_b(9)=%d\n", descent_b(9));
    printf("descent_c(9)=%d\n", descent_c(9));

    return 0;
}
