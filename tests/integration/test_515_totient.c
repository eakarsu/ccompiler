int printf(const char *fmt, ...);
// EXPECT: Euler's totient function phi(n):\nphi(1) = 1\nphi(2) = 1\nphi(3) = 2\nphi(4) = 2\nphi(5) = 4\nphi(6) = 2\nphi(7) = 6\nphi(8) = 4\nphi(9) = 6\nphi(10) = 4\nphi(11) = 10\nphi(12) = 4\nphi(13) = 12\nphi(14) = 6\nphi(15) = 8\nphi(16) = 8\nphi(17) = 16\nphi(18) = 6\nphi(19) = 18\nphi(20) = 8\nphi(21) = 12\nphi(22) = 10\nphi(23) = 22\nphi(24) = 8\nphi(25) = 20\nphi(26) = 12\nphi(27) = 18\nphi(28) = 12\nphi(29) = 28\nphi(30) = 8\n\nVerification:\nphi(1): fast=1, brute=1, match=1\nphi(2): fast=1, brute=1, match=1\nphi(3): fast=2, brute=2, match=1\nphi(4): fast=2, brute=2, match=1\nphi(5): fast=4, brute=4, match=1\nphi(6): fast=2, brute=2, match=1\nphi(7): fast=6, brute=6, match=1\nphi(8): fast=4, brute=4, match=1\nphi(9): fast=6, brute=6, match=1\nphi(10): fast=4, brute=4, match=1\nphi(11): fast=10, brute=10, match=1\nphi(12): fast=4, brute=4, match=1\nphi(13): fast=12, brute=12, match=1\nphi(14): fast=6, brute=6, match=1\nphi(15): fast=8, brute=8, match=1\nphi(16): fast=8, brute=8, match=1\nphi(17): fast=16, brute=16, match=1\nphi(18): fast=6, brute=6, match=1\nphi(19): fast=18, brute=18, match=1\nphi(20): fast=8, brute=8, match=1\n\nDivisor sum property: sum phi(d) for d|n = n:\nn=1: sum=1, match=1\nn=2: sum=2, match=1\nn=3: sum=3, match=1\nn=4: sum=4, match=1\nn=5: sum=5, match=1\nn=6: sum=6, match=1\nn=7: sum=7, match=1\nn=8: sum=8, match=1\nn=9: sum=9, match=1\nn=10: sum=10, match=1\nn=11: sum=11, match=1\nn=12: sum=12, match=1\n\nTotient of primes (should be p-1):\nphi(2) = 1\nphi(3) = 2\nphi(5) = 4\nphi(7) = 6\nphi(11) = 10\nphi(13) = 12\nphi(17) = 16\nphi(19) = 18
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

// Brute force totient for verification
int totient_brute(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) count++;
    }
    return count;
}

int main(void) {
    int i, j;

    printf("Euler's totient function phi(n):\n");
    for (i = 1; i <= 30; i++) {
        printf("phi(%d) = %d\n", i, euler_totient(i));
    }

    // Verify against brute force
    printf("\nVerification:\n");
    for (i = 1; i <= 20; i++) {
        int fast = euler_totient(i);
        int brute = totient_brute(i);
        printf("phi(%d): fast=%d, brute=%d, match=%d\n",
               i, fast, brute, fast == brute);
    }

    // Property: sum of phi(d) for d|n equals n
    printf("\nDivisor sum property: sum phi(d) for d|n = n:\n");
    for (i = 1; i <= 12; i++) {
        int sum = 0;
        for (j = 1; j <= i; j++) {
            if (i % j == 0) {
                sum = sum + euler_totient(j);
            }
        }
        printf("n=%d: sum=%d, match=%d\n", i, sum, sum == i);
    }

    // Totient of primes should be p-1
    int primes[8];
    primes[0] = 2; primes[1] = 3; primes[2] = 5; primes[3] = 7;
    primes[4] = 11; primes[5] = 13; primes[6] = 17; primes[7] = 19;
    printf("\nTotient of primes (should be p-1):\n");
    for (i = 0; i < 8; i++) {
        printf("phi(%d) = %d\n", primes[i], euler_totient(primes[i]));
    }

    return 0;
}
