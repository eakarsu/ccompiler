int printf(const char *fmt, ...);
// EXPECT: gcd_r(48,18)=6\ngcd_r(100,75)=25\ngcd_i(48,18)=6\next_gcd(35,15): g=5 x=1 y=-2\nverify: 35*1+15*-2=5\next_gcd(12,7): g=1 x=3 y=-5\nlcm(4,6)=12\nlcm(12,18)=36\ncoprime(8,15)=1\ncoprime(8,12)=0\nphi(1)=1\nphi(6)=2\nphi(10)=4\nphi(12)=4\ninv(3,7)=5\ninv(5,11)=9\ninv(4,6)=-1\ngcd_arr=6\nlcm_arr=72
// Test 231: Recursive GCD variants and Euclidean algorithm

int gcd_recursive(int a, int b) {
    if (b == 0) return a;
    return gcd_recursive(b, a - (a / b) * b);
}

int gcd_iterative(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

void extended_gcd(int a, int b, int *g, int *x, int *y) {
    if (a == 0) {
        *g = b;
        *x = 0;
        *y = 1;
        return;
    }
    int g1 = 0;
    int x1 = 0;
    int y1 = 0;
    extended_gcd(b - (b / a) * a, a, &g1, &x1, &y1);
    *g = g1;
    *x = y1 - (b / a) * x1;
    *y = x1;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g = gcd_recursive(a, b);
    return (a / g) * b;
}

int coprime(int a, int b) {
    return gcd_recursive(a, b) == 1;
}

int euler_totient(int n) {
    int result = 0;
    int i = 1;
    while (i <= n) {
        if (coprime(n, i)) result = result + 1;
        i = i + 1;
    }
    return result;
}

int mod_inverse(int a, int m) {
    int g = 0;
    int x = 0;
    int y = 0;
    extended_gcd(a, m, &g, &x, &y);
    if (g != 1) return -1;
    int result = x;
    while (result < 0) result = result + m;
    return result - (result / m) * m;
}

int gcd_of_array(int *arr, int n) {
    int result = arr[0];
    int i = 1;
    while (i < n) {
        result = gcd_recursive(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int lcm_of_array(int *arr, int n) {
    int result = arr[0];
    int i = 1;
    while (i < n) {
        result = lcm(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int main(void) {
    printf("gcd_r(48,18)=%d\n", gcd_recursive(48, 18));
    printf("gcd_r(100,75)=%d\n", gcd_recursive(100, 75));
    printf("gcd_i(48,18)=%d\n", gcd_iterative(48, 18));

    int g = 0;
    int x = 0;
    int y = 0;
    extended_gcd(35, 15, &g, &x, &y);
    printf("ext_gcd(35,15): g=%d x=%d y=%d\n", g, x, y);
    printf("verify: 35*%d+15*%d=%d\n", x, y, 35*x + 15*y);

    extended_gcd(12, 7, &g, &x, &y);
    printf("ext_gcd(12,7): g=%d x=%d y=%d\n", g, x, y);

    printf("lcm(4,6)=%d\n", lcm(4, 6));
    printf("lcm(12,18)=%d\n", lcm(12, 18));

    printf("coprime(8,15)=%d\n", coprime(8, 15));
    printf("coprime(8,12)=%d\n", coprime(8, 12));

    printf("phi(1)=%d\n", euler_totient(1));
    printf("phi(6)=%d\n", euler_totient(6));
    printf("phi(10)=%d\n", euler_totient(10));
    printf("phi(12)=%d\n", euler_totient(12));

    printf("inv(3,7)=%d\n", mod_inverse(3, 7));
    printf("inv(5,11)=%d\n", mod_inverse(5, 11));
    printf("inv(4,6)=%d\n", mod_inverse(4, 6));

    int arr[4];
    arr[0] = 12; arr[1] = 18; arr[2] = 24; arr[3] = 36;
    printf("gcd_arr=%d\n", gcd_of_array(arr, 4));
    printf("lcm_arr=%d\n", lcm_of_array(arr, 4));

    return 0;
}
