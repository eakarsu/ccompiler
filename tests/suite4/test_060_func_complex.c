int printf(const char *fmt, ...);

// --- Mutual recursion ---
int is_even_rec(int n);
int is_odd_rec(int n);

int is_even_rec(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 1;
    return is_odd_rec(n - 1);
}

int is_odd_rec(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    return is_even_rec(n - 1);
}

// --- Deep call chain ---
int step1(int x) { return x + 1; }
int step2(int x) { return step1(x) * 2; }
int step3(int x) { return step2(x) + 3; }
int step4(int x) { return step3(x) - 1; }
int step5(int x) { return step4(x) * step1(x); }

// step1(5) = 6
// step2(5) = 6*2 = 12
// step3(5) = 12+3 = 15
// step4(5) = 15-1 = 14
// step5(5) = 14 * 6 = 84

// --- Functions calling multiple other functions ---
int abs_val(int x) {
    return x < 0 ? -x : x;
}

int max2(int a, int b) {
    return a > b ? a : b;
}

int min2(int a, int b) {
    return a < b ? a : b;
}

int clamp(int val, int lo, int hi) {
    return max2(lo, min2(val, hi));
}

int in_range(int val, int lo, int hi) {
    return (val >= lo && val <= hi) ? 1 : 0;
}

// Complex function using many helpers
int analyze(int a, int b, int c) {
    int mx = max2(max2(a, b), c);
    int mn = min2(min2(a, b), c);
    int range = mx - mn;
    int sum = a + b + c;
    int avg = sum / 3;
    return range * 100 + avg;
}

// --- Mutual recursion: Hofstadter Female/Male sequences ---
int female(int n);
int male(int n);

int female(int n) {
    if (n == 0) return 1;
    return n - male(female(n - 1));
}

int male(int n) {
    if (n == 0) return 0;
    return n - female(male(n - 1));
}

// --- Accumulation with helper functions ---
int square(int x) { return x * x; }
int cube(int x) { return x * x * x; }

int sum_of_squares(int n) {
    int total = 0;
    int i;
    for (i = 1; i <= n; i = i + 1) {
        total = total + square(i);
    }
    return total;
}

int sum_of_cubes(int n) {
    int total = 0;
    int i;
    for (i = 1; i <= n; i = i + 1) {
        total = total + cube(i);
    }
    return total;
}

// --- GCD/LCM chain ---
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
    int g = gcd(a, b);
    return (a / g) * b;
}

int lcm3(int a, int b, int c) {
    return lcm(lcm(a, b), c);
}

// --- Array operations using function composition ---
void fill_sequence(int *arr, int n, int start, int step) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        arr[i] = start + i * step;
    }
}

int array_sum(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

int array_max(int *arr, int n) {
    int mx = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        mx = max2(mx, arr[i]);
    }
    return mx;
}

int dot_product(int *a, int *b, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + a[i] * b[i];
    }
    return total;
}

// --- Collatz sequence length ---
int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
    }
    return steps;
}

// --- Power using repeated squaring ---
int fast_power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

// --- Complex computation combining many functions ---
int complex_compute(int x) {
    int a = square(x);
    int b = cube(x);
    int c = abs_val(a - b);
    int d = gcd(a, b);
    int e = max2(c, d);
    return e + clamp(x, -10, 10);
}

int main(void) {
    // Mutual recursion: is_even, is_odd
    // EXPECT: 1
    printf("%d\n", is_even_rec(0));
    // EXPECT: 0
    printf("%d\n", is_odd_rec(0));
    // EXPECT: 0
    printf("%d\n", is_even_rec(7));
    // EXPECT: 1
    printf("%d\n", is_odd_rec(7));
    // EXPECT: 1
    printf("%d\n", is_even_rec(10));

    // Deep call chain
    // EXPECT: 6
    printf("%d\n", step1(5));
    // EXPECT: 12
    printf("%d\n", step2(5));
    // EXPECT: 15
    printf("%d\n", step3(5));
    // EXPECT: 14
    printf("%d\n", step4(5));
    // EXPECT: 84
    printf("%d\n", step5(5));

    // Clamp
    // EXPECT: 5
    printf("%d\n", clamp(5, 0, 10));
    // EXPECT: 0
    printf("%d\n", clamp(-3, 0, 10));
    // EXPECT: 10
    printf("%d\n", clamp(15, 0, 10));

    // Analyze: a=10, b=3, c=7
    // mx=10, mn=3, range=7, sum=20, avg=6
    // result = 7*100 + 6 = 706
    // EXPECT: 706
    printf("%d\n", analyze(10, 3, 7));

    // Hofstadter Female/Male sequences
    // F(0)=1, F(1)=1-M(F(0))=1-M(1)=1-0=1, F(2)=2-M(F(1))=2-M(1)=2-0=2
    // M(0)=0, M(1)=1-F(M(0))=1-F(0)=1-1=0, M(2)=2-F(M(1))=2-F(0)=2-1=1
    // F(3)=3-M(F(2))=3-M(2)=3-1=2
    // F(4)=4-M(F(3))=4-M(2)=4-1=3
    // EXPECT: 1
    printf("%d\n", female(0));
    // EXPECT: 1
    printf("%d\n", female(1));
    // EXPECT: 2
    printf("%d\n", female(2));
    // EXPECT: 2
    printf("%d\n", female(3));
    // EXPECT: 0
    printf("%d\n", male(0));
    // EXPECT: 0
    printf("%d\n", male(1));
    // EXPECT: 1
    printf("%d\n", male(2));

    // Sum of squares: 1+4+9+16+25 = 55
    // EXPECT: 55
    printf("%d\n", sum_of_squares(5));
    // Sum of cubes: 1+8+27+64+125 = 225
    // EXPECT: 225
    printf("%d\n", sum_of_cubes(5));

    // GCD / LCM
    // EXPECT: 6
    printf("%d\n", gcd(12, 18));
    // EXPECT: 36
    printf("%d\n", lcm(12, 18));
    // lcm3(4, 6, 10) = lcm(lcm(4,6), 10) = lcm(12, 10) = 60
    // EXPECT: 60
    printf("%d\n", lcm3(4, 6, 10));

    // Array operations
    int arr[5];
    fill_sequence(arr, 5, 10, 3);
    // arr = {10, 13, 16, 19, 22}
    // EXPECT: 10 13 16 19 22
    printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    // sum = 80
    // EXPECT: 80
    printf("%d\n", array_sum(arr, 5));
    // EXPECT: 22
    printf("%d\n", array_max(arr, 5));

    // Dot product
    int v1[] = {1, 2, 3};
    int v2[] = {4, 5, 6};
    // 1*4 + 2*5 + 3*6 = 4+10+18 = 32
    // EXPECT: 32
    printf("%d\n", dot_product(v1, v2, 3));

    // Collatz steps
    // 6 -> 3 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1 = 8 steps
    // EXPECT: 8
    printf("%d\n", collatz_steps(6));
    // 27 is famous: 111 steps
    // EXPECT: 111
    printf("%d\n", collatz_steps(27));

    // Fast power
    // EXPECT: 32
    printf("%d\n", fast_power(2, 5));
    // EXPECT: 243
    printf("%d\n", fast_power(3, 5));
    // EXPECT: 1
    printf("%d\n", fast_power(7, 0));

    // Complex compute: x=3
    // square(3)=9, cube(3)=27, abs(9-27)=18, gcd(9,27)=9, max(18,9)=18
    // clamp(3,-10,10)=3, result=18+3=21
    // EXPECT: 21
    printf("%d\n", complex_compute(3));

    return 0;
}
