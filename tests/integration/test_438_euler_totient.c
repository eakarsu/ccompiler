int printf(const char *fmt, ...);
// EXPECT: Euler totient values:\nphi(1) = 1\nphi(2) = 1\nphi(3) = 2\nphi(4) = 2\nphi(5) = 4\nphi(6) = 2\nphi(7) = 6\nphi(8) = 4\nphi(9) = 6\nphi(10) = 4\nphi(11) = 10\nphi(12) = 4\nphi(13) = 12\nphi(14) = 6\nphi(15) = 8\nphi(16) = 8\nphi(17) = 16\nphi(18) = 6\nphi(19) = 18\nphi(20) = 8\n\nVerification (brute force):\nAll match up to 50: 1\n\nTotient sieve sum(phi(i), i=1..30): 278\nphi(97) = 96\nphi(100) = 40
// Test: Euler's totient function

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int euler_totient(int n) {
    int result = n;
    int p = 2;
    int temp = n;

    while (p * p <= temp) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp = temp / p;
            }
            result = result - result / p;
        }
        p++;
    }
    if (temp > 1) {
        result = result - result / temp;
    }
    return result;
}

int euler_totient_brute(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) count++;
    }
    return count;
}

void totient_sieve(int n, int phi[]) {
    int i, j;
    for (i = 0; i <= n; i++) phi[i] = i;

    for (i = 2; i <= n; i++) {
        if (phi[i] == i) {
            for (j = i; j <= n; j = j + i) {
                phi[j] = phi[j] - phi[j] / i;
            }
        }
    }
}

int main(void) {
    printf("Euler totient values:\n");
    int i;
    for (i = 1; i <= 20; i++) {
        printf("phi(%d) = %d\n", i, euler_totient(i));
    }

    printf("\nVerification (brute force):\n");
    int all_match = 1;
    for (i = 1; i <= 50; i++) {
        int fast = euler_totient(i);
        int brute = euler_totient_brute(i);
        if (fast != brute) {
            printf("MISMATCH at %d: %d vs %d\n", i, fast, brute);
            all_match = 0;
        }
    }
    printf("All match up to 50: %d\n", all_match);

    int phi[64];
    totient_sieve(30, phi);
    printf("\nTotient sieve sum(phi(i), i=1..30): ");
    int sum = 0;
    for (i = 1; i <= 30; i++) sum = sum + phi[i];
    printf("%d\n", sum);

    printf("phi(97) = %d\n", euler_totient(97));
    printf("phi(100) = %d\n", euler_totient(100));

    return 0;
}
