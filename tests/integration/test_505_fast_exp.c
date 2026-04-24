int printf(const char *fmt, ...);
// EXPECT: Fast exponentiation:\n2^0 = 1\n2^1 = 2\n2^10 = 1024\n2^15 = 32768\n3^7 = 2187\n5^5 = 3125\n7^4 = 2401\n\nMultiplication counts (fast exp):\nexp=1: naive=0, fast=1\nexp=2: naive=1, fast=2\nexp=4: naive=3, fast=3\nexp=8: naive=7, fast=4\nexp=15: naive=14, fast=7\nexp=16: naive=15, fast=5\nexp=31: naive=30, fast=9\nexp=100: naive=99, fast=9\n\nModular exponentiation (mod 1000):\n2^20 mod 1000 = 576\n3^15 mod 1000 = 907\n7^13 mod 1000 = 407\n13^17 mod 1000 = 933\n\nBinary expansion of 2^13:\nStep 0: multiply by 2, result = 2\nStep 2: multiply by 16, result = 32\nStep 3: multiply by 256, result = 8192\nFinal: 2^13 = 8192
// Test: Fast exponentiation (binary exponentiation)

int power(int base, int exp) {
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

int mod_power(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

// Count number of multiplications needed
int count_mults(int exp) {
    int count = 0;
    while (exp > 0) {
        if (exp % 2 == 1) {
            count++;
        }
        if (exp > 1) {
            count++; // for squaring
        }
        exp = exp / 2;
    }
    return count;
}

int main(void) {
    int i;

    printf("Fast exponentiation:\n");

    // Basic powers
    printf("2^0 = %d\n", power(2, 0));
    printf("2^1 = %d\n", power(2, 1));
    printf("2^10 = %d\n", power(2, 10));
    printf("2^15 = %d\n", power(2, 15));
    printf("3^7 = %d\n", power(3, 7));
    printf("5^5 = %d\n", power(5, 5));
    printf("7^4 = %d\n", power(7, 4));

    // Compare naive vs fast exponentiation multiplications
    printf("\nMultiplication counts (fast exp):\n");
    int exponents[8];
    exponents[0] = 1;
    exponents[1] = 2;
    exponents[2] = 4;
    exponents[3] = 8;
    exponents[4] = 15;
    exponents[5] = 16;
    exponents[6] = 31;
    exponents[7] = 100;

    for (i = 0; i < 8; i++) {
        int naive = exponents[i] - 1;
        if (naive < 0) naive = 0;
        int fast = count_mults(exponents[i]);
        printf("exp=%d: naive=%d, fast=%d\n", exponents[i], naive, fast);
    }

    // Modular exponentiation
    printf("\nModular exponentiation (mod 1000):\n");
    printf("2^20 mod 1000 = %d\n", mod_power(2, 20, 1000));
    printf("3^15 mod 1000 = %d\n", mod_power(3, 15, 1000));
    printf("7^13 mod 1000 = %d\n", mod_power(7, 13, 1000));
    printf("13^17 mod 1000 = %d\n", mod_power(13, 17, 1000));

    // Verify: compute 2^10 step by step
    printf("\nBinary expansion of 2^13:\n");
    int base = 2;
    int exp = 13; // 1101 in binary
    int result = 1;
    int step = 0;
    int b = base;
    int e = exp;
    while (e > 0) {
        if (e % 2 == 1) {
            result = result * b;
            printf("Step %d: multiply by %d, result = %d\n", step, b, result);
        }
        b = b * b;
        e = e / 2;
        step++;
    }
    printf("Final: 2^13 = %d\n", result);

    return 0;
}
