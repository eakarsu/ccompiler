int printf(const char *fmt, ...);

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
    int aa = a;
    int bb = b;
    if (aa < 0) aa = -aa;
    if (bb < 0) bb = -bb;
    return aa / gcd(aa, bb) * bb;
}

void extended_gcd(int a, int b, int *g, int *x, int *y) {
    if (a == 0) {
        *g = b;
        *x = 0;
        *y = 1;
        return;
    }
    int g1;
    int x1;
    int y1;
    extended_gcd(b % a, a, &g1, &x1, &y1);
    *g = g1;
    *x = y1 - (b / a) * x1;
    *y = x1;
}

int mod_inverse(int a, int m) {
    int g;
    int x;
    int y;
    extended_gcd(a, m, &g, &x, &y);
    if (g != 1) return -1;
    int result = x % m;
    if (result < 0) result = result + m;
    return result;
}

int are_coprime(int a, int b) {
    return gcd(a, b) == 1;
}

int lcm_chain(int arr[], int n) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
}

int gcd_chain(int arr[], int n) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = gcd(result, arr[i]);
    }
    return result;
}

int euler_totient(int n) {
    int result = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (gcd(i, n) == 1) result = result + 1;
    }
    return result;
}

int count_coprimes_in_range(int lo, int hi, int m) {
    int count = 0;
    int i;
    for (i = lo; i <= hi; i++) {
        if (gcd(i, m) == 1) count = count + 1;
    }
    return count;
}

int gcd_of_sum_and_product(int a, int b) {
    int s = a + b;
    int p = a * b;
    return gcd(s, p);
}

int reduce_fraction_num(int num, int den) {
    int g = gcd(num, den);
    if (g == 0) return num;
    if (den < 0) g = -g;
    return num / g;
}

int reduce_fraction_den(int num, int den) {
    int g = gcd(num, den);
    if (g == 0) return den;
    if (den < 0) g = -g;
    return den / g;
}

int main(void) {
    // EXPECT: gcd(12,8)=4
    printf("gcd(12,8)=%d\n", gcd(12, 8));

    // EXPECT: gcd(17,13)=1
    printf("gcd(17,13)=%d\n", gcd(17, 13));

    // EXPECT: gcd(0,5)=5
    printf("gcd(0,5)=%d\n", gcd(0, 5));

    // EXPECT: gcd(-12,8)=4
    printf("gcd(-12,8)=%d\n", gcd(-12, 8));

    // EXPECT: lcm(4,6)=12
    printf("lcm(4,6)=%d\n", lcm(4, 6));

    // EXPECT: lcm(7,13)=91
    printf("lcm(7,13)=%d\n", lcm(7, 13));

    int g;
    int x;
    int y;
    extended_gcd(35, 15, &g, &x, &y);
    // EXPECT: ext_gcd(35,15): g=5 x=1 y=-2
    printf("ext_gcd(35,15): g=%d x=%d y=%d\n", g, x, y);

    extended_gcd(99, 78, &g, &x, &y);
    // EXPECT: ext_gcd(99,78): g=3 x=-11 y=14
    printf("ext_gcd(99,78): g=%d x=%d y=%d\n", g, x, y);

    // EXPECT: verify: 99*-11+78*14=3
    printf("verify: 99*-11+78*14=%d\n", 99 * x + 78 * y);

    // EXPECT: mod_inv(3,7)=5
    printf("mod_inv(3,7)=%d\n", mod_inverse(3, 7));

    // EXPECT: mod_inv(7,11)=8
    printf("mod_inv(7,11)=%d\n", mod_inverse(7, 11));

    // EXPECT: mod_inv(4,6)=-1
    printf("mod_inv(4,6)=%d\n", mod_inverse(4, 6));

    // EXPECT: verify: 3*5 mod 7=1
    printf("verify: 3*5 mod 7=%d\n", (3 * 5) % 7);

    // EXPECT: coprime(8,15)=1
    printf("coprime(8,15)=%d\n", are_coprime(8, 15));

    // EXPECT: coprime(8,12)=0
    printf("coprime(8,12)=%d\n", are_coprime(8, 12));

    int arr1[4];
    arr1[0] = 4;
    arr1[1] = 6;
    arr1[2] = 10;
    arr1[3] = 15;
    // EXPECT: lcm_chain=60
    printf("lcm_chain=%d\n", lcm_chain(arr1, 4));

    int arr2[3];
    arr2[0] = 24;
    arr2[1] = 36;
    arr2[2] = 48;
    // EXPECT: gcd_chain=12
    printf("gcd_chain=%d\n", gcd_chain(arr2, 3));

    // EXPECT: totient(12)=4
    printf("totient(12)=%d\n", euler_totient(12));

    // EXPECT: totient(7)=6
    printf("totient(7)=%d\n", euler_totient(7));

    // EXPECT: coprimes(1,20,6)=7
    printf("coprimes(1,20,6)=%d\n", count_coprimes_in_range(1, 20, 6));

    // EXPECT: reduce(6/9)=2/3
    printf("reduce(6/9)=%d/%d\n", reduce_fraction_num(6, 9), reduce_fraction_den(6, 9));

    // EXPECT: reduce(-8/12)=-2/3
    printf("reduce(-8/12)=%d/%d\n", reduce_fraction_num(-8, 12), reduce_fraction_den(-8, 12));

    return 0;
}
