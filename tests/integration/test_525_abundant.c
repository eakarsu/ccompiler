int printf(const char *fmt, ...);
// EXPECT: Classification (1-40):\n1: s(n)=0 (deficient)\n2: s(n)=1 (deficient)\n3: s(n)=1 (deficient)\n4: s(n)=3 (deficient)\n5: s(n)=1 (deficient)\n6: s(n)=6 (perfect)\n7: s(n)=1 (deficient)\n8: s(n)=7 (deficient)\n9: s(n)=4 (deficient)\n10: s(n)=8 (deficient)\n11: s(n)=1 (deficient)\n12: s(n)=16 (abundant)\n13: s(n)=1 (deficient)\n14: s(n)=10 (deficient)\n15: s(n)=9 (deficient)\n16: s(n)=15 (deficient)\n17: s(n)=1 (deficient)\n18: s(n)=21 (abundant)\n19: s(n)=1 (deficient)\n20: s(n)=22 (abundant)\n21: s(n)=11 (deficient)\n22: s(n)=14 (deficient)\n23: s(n)=1 (deficient)\n24: s(n)=36 (abundant)\n25: s(n)=6 (deficient)\n26: s(n)=16 (deficient)\n27: s(n)=13 (deficient)\n28: s(n)=28 (perfect)\n29: s(n)=1 (deficient)\n30: s(n)=42 (abundant)\n31: s(n)=1 (deficient)\n32: s(n)=31 (deficient)\n33: s(n)=15 (deficient)\n34: s(n)=20 (deficient)\n35: s(n)=13 (deficient)\n36: s(n)=55 (abundant)\n37: s(n)=1 (deficient)\n38: s(n)=22 (deficient)\n39: s(n)=17 (deficient)\n40: s(n)=50 (abundant)\n\nUp to 100:\nAbundant: 22\nPerfect: 2\nDeficient: 76\n\nFirst 15 abundant numbers:\n12 (abundance: 4)\n18 (abundance: 3)\n20 (abundance: 2)\n24 (abundance: 12)\n30 (abundance: 12)\n36 (abundance: 19)\n40 (abundance: 10)\n42 (abundance: 12)\n48 (abundance: 28)\n54 (abundance: 12)\n56 (abundance: 8)\n60 (abundance: 48)\n66 (abundance: 12)\n70 (abundance: 4)\n72 (abundance: 51)\n\nNumbers up to 50 expressible as sum of two abundant numbers:\n24 30 32 36 38 40 42 44 48 50 \n\nPrimitive abundant numbers up to 100:\n12 18 20 30 42 56 66 70 78 88 
// Test: Abundant and deficient numbers

int sum_proper_divisors(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum;
}

int main(void) {
    int i, j;

    // Classify numbers as abundant, perfect, or deficient
    printf("Classification (1-40):\n");
    for (i = 1; i <= 40; i++) {
        int s = sum_proper_divisors(i);
        char *type;
        if (s > i) type = "abundant";
        else if (s == i) type = "perfect";
        else type = "deficient";
        printf("%d: s(n)=%d (%s)\n", i, s, type);
    }

    // Count each type up to 100
    int abundant = 0, perfect = 0, deficient = 0;
    for (i = 1; i <= 100; i++) {
        int s = sum_proper_divisors(i);
        if (s > i) abundant++;
        else if (s == i) perfect++;
        else deficient++;
    }
    printf("\nUp to 100:\n");
    printf("Abundant: %d\n", abundant);
    printf("Perfect: %d\n", perfect);
    printf("Deficient: %d\n", deficient);

    // First 15 abundant numbers
    printf("\nFirst 15 abundant numbers:\n");
    int count = 0;
    for (i = 1; count < 15; i++) {
        if (sum_proper_divisors(i) > i) {
            printf("%d (abundance: %d)\n", i, sum_proper_divisors(i) - i);
            count++;
        }
    }

    // Abundant number sums: numbers expressible as sum of two abundant numbers
    // Every integer > 20161 can be expressed as sum of two abundant numbers
    int abundant_list[50];
    int na = 0;
    for (i = 1; i <= 100 && na < 50; i++) {
        if (sum_proper_divisors(i) > i) {
            abundant_list[na] = i;
            na++;
        }
    }

    printf("\nNumbers up to 50 expressible as sum of two abundant numbers:\n");
    for (i = 1; i <= 50; i++) {
        int found = 0;
        for (j = 0; j < na && abundant_list[j] < i; j++) {
            int rem = i - abundant_list[j];
            if (rem > 0 && sum_proper_divisors(rem) > rem) {
                found = 1;
                break;
            }
        }
        if (found) printf("%d ", i);
    }
    printf("\n");

    // Primitive abundant: abundant but no proper divisor is abundant
    printf("\nPrimitive abundant numbers up to 100:\n");
    for (i = 2; i <= 100; i++) {
        if (sum_proper_divisors(i) > i) {
            int prim = 1;
            for (j = 2; j < i; j++) {
                if (i % j == 0 && j > 1 && sum_proper_divisors(j) > j) {
                    prim = 0;
                    break;
                }
            }
            if (prim) printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
