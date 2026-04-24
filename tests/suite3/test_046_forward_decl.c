int printf(const char *fmt, ...);

/* Forward declarations before definitions */
int square(int x);
int cube(int x);
int power(int base, int exp);
int gcd(int a, int b);
int lcm(int a, int b);
int abs_val(int x);
int sign(int x);
int is_even(int n);
int is_odd(int n);
int collatz_steps(int n);
int sum_divisors(int n);
int is_perfect(int n);

/* Mutual recursion forward declarations */
int is_even_mutual(int n);
int is_odd_mutual(int n);
int count_even_steps(int n);
int count_odd_steps(int n);
int ping(int n);
int pong(int n);

/* Definitions come AFTER use sites (enabled by forward decls above) */

/* Uses square before its definition */
int sum_of_squares(int a, int b) {
    return square(a) + square(b);
}

/* Uses cube before its definition */
int sum_of_cubes(int a, int b, int c) {
    return cube(a) + cube(b) + cube(c);
}

/* Uses power before its definition */
int hypercube_vol(int side) {
    return power(side, 4);
}

/* Uses gcd/lcm */
int gcd_lcm_sum(int a, int b) {
    return gcd(a, b) + lcm(a, b);
}

/* Uses abs_val and sign */
int normalized(int x) {
    if (x == 0) return 0;
    return sign(x) * abs_val(x); /* always equals x */
}

/* Mutual recursion: is_even / is_odd */
int count_chain(int n) {
    /* Count: how many times we alternate is_even/is_odd until reaching 0 or 1 */
    if (n < 0) n = -n;
    if (n == 0) return is_even_mutual(0); /* 1 */
    return is_even_mutual(n) + is_odd_mutual(n);
}

/* Uses collatz_steps before definition */
int total_collatz(int start, int end) {
    int total = 0;
    int i;
    for (i = start; i <= end; i++) {
        total += collatz_steps(i);
    }
    return total;
}

/* Uses sum_divisors and is_perfect */
int count_perfect_in_range(int lo, int hi) {
    int count = 0;
    int i;
    for (i = lo; i <= hi; i++) {
        if (is_perfect(i)) count++;
    }
    return count;
}

/* Now the actual definitions */

int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) result *= base;
    return result;
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return abs_val(a) / gcd(a, b) * abs_val(b);
}

int is_even(int n) { return n % 2 == 0; }
int is_odd(int n)  { return n % 2 != 0; }

/* Mutual recursion definitions */
int is_even_mutual(int n) {
    if (n == 0) return 1;
    if (n < 0) return is_even_mutual(-n);
    return is_odd_mutual(n - 1);
}

int is_odd_mutual(int n) {
    if (n == 0) return 0;
    if (n < 0) return is_odd_mutual(-n);
    return is_even_mutual(n - 1);
}

int count_even_steps(int n) {
    if (n <= 0) return 0;
    return is_even_mutual(n) + count_even_steps(n - 1);
}

int count_odd_steps(int n) {
    if (n <= 0) return 0;
    return is_odd_mutual(n) + count_odd_steps(n - 1);
}

int ping(int n) {
    if (n <= 0) return 0;
    return 1 + pong(n - 1);
}

int pong(int n) {
    if (n <= 0) return 0;
    return 1 + ping(n - 1);
}

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) n = n / 2;
        else n = 3 * n + 1;
        steps++;
    }
    return steps;
}

int sum_divisors(int n) {
    int s = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (n % i == 0) s += i;
    }
    return s;
}

int is_perfect(int n) {
    if (n < 2) return 0;
    return sum_divisors(n) == n;
}

int main(void) {
    /* Forward declared functions */
    printf("%d\n", square(7));            // EXPECT: 49
    printf("%d\n", cube(4));              // EXPECT: 64
    printf("%d\n", power(2, 10));         // EXPECT: 1024
    printf("%d\n", gcd(48, 18));          // EXPECT: 6
    printf("%d\n", lcm(4, 6));            // EXPECT: 12
    printf("%d\n", abs_val(-99));         // EXPECT: 99
    printf("%d\n", sign(-5));             // EXPECT: -1
    printf("%d\n", sign(0));              // EXPECT: 0
    printf("%d\n", sign(7));              // EXPECT: 1

    /* Functions that use forward decls */
    printf("%d\n", sum_of_squares(3, 4));     // EXPECT: 25
    printf("%d\n", sum_of_cubes(1, 2, 3));    // EXPECT: 36
    /* 1 + 8 + 27 = 36 */
    printf("%d\n", hypercube_vol(3));          // EXPECT: 81
    printf("%d\n", gcd_lcm_sum(6, 10));        // EXPECT: 32
    /* gcd(6,10)=2, lcm(6,10)=30, sum=32 */
    printf("%d\n", normalized(-42));            // EXPECT: -42
    printf("%d\n", normalized(0));              // EXPECT: 0
    printf("%d\n", normalized(17));             // EXPECT: 17

    /* Mutual recursion */
    printf("%d\n", is_even_mutual(0));   // EXPECT: 1
    printf("%d\n", is_even_mutual(1));   // EXPECT: 0
    printf("%d\n", is_even_mutual(4));   // EXPECT: 1
    printf("%d\n", is_even_mutual(7));   // EXPECT: 0
    printf("%d\n", is_odd_mutual(0));    // EXPECT: 0
    printf("%d\n", is_odd_mutual(1));    // EXPECT: 1
    printf("%d\n", is_odd_mutual(4));    // EXPECT: 0
    printf("%d\n", is_odd_mutual(7));    // EXPECT: 1

    /* ping/pong mutual recursion */
    printf("%d\n", ping(0));    // EXPECT: 0
    printf("%d\n", ping(1));    // EXPECT: 1
    printf("%d\n", ping(6));    // EXPECT: 6
    printf("%d\n", pong(5));    // EXPECT: 5

    /* Collatz steps */
    printf("%d\n", collatz_steps(1));   // EXPECT: 0
    printf("%d\n", collatz_steps(2));   // EXPECT: 1
    printf("%d\n", collatz_steps(3));   // EXPECT: 7
    printf("%d\n", collatz_steps(6));   // EXPECT: 8
    printf("%d\n", collatz_steps(7));   // EXPECT: 16

    /* Perfect numbers */
    printf("%d\n", is_perfect(6));      // EXPECT: 1
    printf("%d\n", is_perfect(28));     // EXPECT: 1
    printf("%d\n", is_perfect(12));     // EXPECT: 0
    printf("%d\n", count_perfect_in_range(1, 28)); // EXPECT: 2

    /* Count even steps 1..10 */
    printf("%d\n", count_even_steps(10)); // EXPECT: 5
    printf("%d\n", count_odd_steps(10));  // EXPECT: 5

    return 0;
}
