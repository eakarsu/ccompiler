int printf(const char *fmt, ...);

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

int gcd_chain(int arr[], int n) {
    int result = arr[0];
    int i = 1;
    while (i < n) {
        result = gcd(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int lcm_chain(int arr[], int n) {
    int result = arr[0];
    int i = 1;
    while (i < n) {
        result = lcm(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int is_coprime(int a, int b) {
    return gcd(a, b) == 1;
}

int euler_totient(int n) {
    int result = 0;
    int i = 1;
    while (i <= n) {
        if (gcd(i, n) == 1) {
            result = result + 1;
        }
        i = i + 1;
    }
    return result;
}

int count_divisors(int n) {
    int count = 0;
    int i = 1;
    while (i * i <= n) {
        if (n % i == 0) {
            count = count + 1;
            if (i != n / i) {
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

int sum_divisors(int n) {
    int sum = 0;
    int i = 1;
    while (i * i <= n) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
        i = i + 1;
    }
    return sum;
}

int is_perfect(int n) {
    return sum_divisors(n) - n == n;
}

int count_coprimes(int n) {
    int count = 0;
    int i = 1;
    while (i < n) {
        if (gcd(i, n) == 1) count = count + 1;
        i = i + 1;
    }
    return count;
}

int sum_of_gcd_pairs(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        int j = i + 1;
        while (j <= n) {
            sum = sum + gcd(i, j);
            j = j + 1;
        }
        i = i + 1;
    }
    return sum;
}

int main(void) {
    // EXPECT: gcd(12,8) = 4
    printf("gcd(12,8) = %d\n", gcd(12, 8));
    // EXPECT: gcd(35,14) = 7
    printf("gcd(35,14) = %d\n", gcd(35, 14));
    // EXPECT: gcd(100,75) = 25
    printf("gcd(100,75) = %d\n", gcd(100, 75));

    // EXPECT: lcm(4,6) = 12
    printf("lcm(4,6) = %d\n", lcm(4, 6));
    // EXPECT: lcm(12,18) = 36
    printf("lcm(12,18) = %d\n", lcm(12, 18));

    int arr1[4];
    arr1[0] = 12; arr1[1] = 18; arr1[2] = 24; arr1[3] = 36;
    // EXPECT: gcd_chain = 6
    printf("gcd_chain = %d\n", gcd_chain(arr1, 4));

    int arr2[3];
    arr2[0] = 4; arr2[1] = 6; arr2[2] = 10;
    // EXPECT: lcm_chain = 60
    printf("lcm_chain = %d\n", lcm_chain(arr2, 3));

    // EXPECT: coprime(8,15) = 1
    printf("coprime(8,15) = %d\n", is_coprime(8, 15));
    // EXPECT: coprime(12,9) = 0
    printf("coprime(12,9) = %d\n", is_coprime(12, 9));

    // EXPECT: phi(1) = 1
    printf("phi(1) = %d\n", euler_totient(1));
    // EXPECT: phi(10) = 4
    printf("phi(10) = %d\n", euler_totient(10));
    // EXPECT: phi(12) = 4
    printf("phi(12) = %d\n", euler_totient(12));
    // EXPECT: phi(7) = 6
    printf("phi(7) = %d\n", euler_totient(7));

    // EXPECT: divs(12) = 6
    printf("divs(12) = %d\n", count_divisors(12));
    // EXPECT: divs(28) = 6
    printf("divs(28) = %d\n", count_divisors(28));

    // EXPECT: sum_divs(12) = 28
    printf("sum_divs(12) = %d\n", sum_divisors(12));

    // EXPECT: perfect(6) = 1
    printf("perfect(6) = %d\n", is_perfect(6));
    // EXPECT: perfect(28) = 1
    printf("perfect(28) = %d\n", is_perfect(28));
    // EXPECT: perfect(12) = 0
    printf("perfect(12) = %d\n", is_perfect(12));

    // EXPECT: coprimes(12) = 4
    printf("coprimes(12) = %d\n", count_coprimes(12));

    // EXPECT: gcd_pair_sum(6) = 20
    printf("gcd_pair_sum(6) = %d\n", sum_of_gcd_pairs(6));

    return 0;
}
