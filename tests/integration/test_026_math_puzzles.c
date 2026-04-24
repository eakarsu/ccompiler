int printf(const char *fmt, ...);
// EXPECT: 1:0\n2:1\n3:7\n4:8\n5:111\n6:6\n7:6\n8:36\n9:0\n10:1\n11:1\n12:1\n13:2\n14:4\n15:321\n16:21\n17:5\n18:1\n19:1\n20:1\n21:1\n22:0\n23:1\n24:1\n25:1\n26:1\n27:0\n28:0\n29:1\n30:1\n31:1\n32:0\n33:0\n34:3\n35:-1\n36:0\n37:4\n38:1\n39:25\n40:12\n41:21\n42:1\n43:1\n44:0\n45:0\n46:10\n47:7\n48:9\n49:6\n50:120\n51:1\n52:5040\n53:1\n54:0\n55:1\n56:0\n57:1\n58:0
// =============================================================================
// TEST 026: Math Puzzles
// =============================================================================
// Tests various mathematical puzzles and number theory: Collatz conjecture,
// digit sum, Armstrong numbers, perfect numbers, happy numbers, Kaprekar
// routine, and more.
// =============================================================================

// -----------------------------------------------------------------------------
// Collatz conjecture: count steps to reach 1
// If even: n/2, if odd: 3n+1
// -----------------------------------------------------------------------------
int collatz_steps(int n) {
    int steps;
    steps = 0;
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

// -----------------------------------------------------------------------------
// Digit sum
// -----------------------------------------------------------------------------
int digit_sum(int n) {
    int s;
    if (n < 0) n = -n;
    s = 0;
    while (n > 0) {
        s = s + (n % 10);
        n = n / 10;
    }
    return s;
}

// -----------------------------------------------------------------------------
// Number of digits
// -----------------------------------------------------------------------------
int num_digits(int n) {
    int d;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    d = 0;
    while (n > 0) {
        d = d + 1;
        n = n / 10;
    }
    return d;
}

// -----------------------------------------------------------------------------
// Reverse a number
// -----------------------------------------------------------------------------
int reverse_number(int n) {
    int rev;
    int neg;
    neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    if (neg) return -rev;
    return rev;
}

// -----------------------------------------------------------------------------
// Power function (base^exp, integer)
// -----------------------------------------------------------------------------
int power(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Armstrong number check
// An Armstrong number of n digits equals the sum of its digits each raised
// to the power of n. E.g., 153 = 1^3 + 5^3 + 3^3
// -----------------------------------------------------------------------------
int is_armstrong(int n) {
    int digits;
    int sum;
    int temp;
    int d;
    if (n < 0) return 0;
    digits = num_digits(n);
    sum = 0;
    temp = n;
    while (temp > 0) {
        d = temp % 10;
        sum = sum + power(d, digits);
        temp = temp / 10;
    }
    return (sum == n) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Perfect number check
// A perfect number equals the sum of its proper divisors
// E.g., 6 = 1 + 2 + 3
// -----------------------------------------------------------------------------
int is_perfect(int n) {
    int i;
    int sum;
    if (n <= 1) return 0;
    sum = 1;
    for (i = 2; i * i <= n; i = i + 1) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return (sum == n) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Sum of proper divisors
// -----------------------------------------------------------------------------
int sum_divisors(int n) {
    int i;
    int sum;
    if (n <= 1) return 0;
    sum = 1;
    for (i = 2; i * i <= n; i = i + 1) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Happy number check
// Sum of squares of digits repeatedly; happy if reaches 1
// Limit iterations to avoid infinite loop
// -----------------------------------------------------------------------------
int sum_digit_squares(int n) {
    int s;
    int d;
    s = 0;
    while (n > 0) {
        d = n % 10;
        s = s + d * d;
        n = n / 10;
    }
    return s;
}

int is_happy(int n) {
    int slow;
    int fast;
    int i;
    // Use Floyd's cycle detection
    slow = n;
    fast = n;
    for (i = 0; i < 100; i = i + 1) {
        slow = sum_digit_squares(slow);
        fast = sum_digit_squares(sum_digit_squares(fast));
        if (slow == 1) return 1;
        if (fast == 1) return 1;
        if (slow == fast) return 0; // cycle detected
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Kaprekar routine for 4-digit numbers
// Returns number of steps to reach 6174 (Kaprekar constant)
// Returns -1 if number has all same digits
// -----------------------------------------------------------------------------

// Sort 4 digits descending
int sort_desc_4(int digits[4]) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < 3; i = i + 1) {
        for (j = i + 1; j < 4; j = j + 1) {
            if (digits[j] > digits[i]) {
                tmp = digits[i];
                digits[i] = digits[j];
                digits[j] = tmp;
            }
        }
    }
    return digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3];
}

// Sort 4 digits ascending
int sort_asc_4(int digits[4]) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < 3; i = i + 1) {
        for (j = i + 1; j < 4; j = j + 1) {
            if (digits[j] < digits[i]) {
                tmp = digits[i];
                digits[i] = digits[j];
                digits[j] = tmp;
            }
        }
    }
    return digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3];
}

int kaprekar_steps(int n) {
    int steps;
    int digits[4];
    int desc_val;
    int asc_val;
    steps = 0;
    while (n != 6174 && steps < 20) {
        digits[0] = (n / 1000) % 10;
        digits[1] = (n / 100) % 10;
        digits[2] = (n / 10) % 10;
        digits[3] = n % 10;
        // Check if all digits are the same
        if (digits[0] == digits[1] && digits[1] == digits[2] && digits[2] == digits[3]) {
            return -1;
        }
        desc_val = sort_desc_4(digits);
        // Re-extract digits from n for ascending
        digits[0] = (n / 1000) % 10;
        digits[1] = (n / 100) % 10;
        digits[2] = (n / 10) % 10;
        digits[3] = n % 10;
        asc_val = sort_asc_4(digits);
        n = desc_val - asc_val;
        steps = steps + 1;
    }
    return steps;
}

// -----------------------------------------------------------------------------
// GCD using Euclidean algorithm
// -----------------------------------------------------------------------------
int gcd(int a, int b) {
    int t;
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// -----------------------------------------------------------------------------
// LCM
// -----------------------------------------------------------------------------
int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g;
    g = gcd(a, b);
    return (a / g) * b;
}

// -----------------------------------------------------------------------------
// Is prime
// -----------------------------------------------------------------------------
int is_prime(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i * i <= n; i = i + 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Count primes up to n (sieve-like approach)
// -----------------------------------------------------------------------------
int count_primes(int n) {
    int i;
    int count;
    count = 0;
    for (i = 2; i <= n; i = i + 1) {
        if (is_prime(i)) count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Digital root: keep summing digits until single digit
// -----------------------------------------------------------------------------
int digital_root(int n) {
    if (n < 0) n = -n;
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

// -----------------------------------------------------------------------------
// Factorial
// -----------------------------------------------------------------------------
int factorial(int n) {
    int result;
    result = 1;
    while (n > 1) {
        result = result * n;
        n = n - 1;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Sum of factorials of digits
// (Used for checking numbers like 145 = 1! + 4! + 5!)
// -----------------------------------------------------------------------------
int sum_digit_factorials(int n) {
    int s;
    int d;
    s = 0;
    while (n > 0) {
        d = n % 10;
        s = s + factorial(d);
        n = n / 10;
    }
    return s;
}

// -----------------------------------------------------------------------------
// Is number a factorial sum number? (like 145)
// -----------------------------------------------------------------------------
int is_factorial_sum(int n) {
    return (sum_digit_factorials(n) == n) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Abundant number: sum of proper divisors > number
// -----------------------------------------------------------------------------
int is_abundant(int n) {
    if (n <= 0) return 0;
    return (sum_divisors(n) > n) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Deficient number: sum of proper divisors < number
// -----------------------------------------------------------------------------
int is_deficient(int n) {
    if (n <= 1) return 0;
    return (sum_divisors(n) < n) ? 1 : 0;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    int i;
    int count;

    test_num = 0;

    // ---- Collatz conjecture steps ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(1)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(2)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(3)); // 7

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(6)); // 8

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(27)); // 111

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, collatz_steps(10)); // 6

    // ---- Digit sum ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digit_sum(123)); // 6

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digit_sum(9999)); // 36

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digit_sum(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digit_sum(100)); // 1

    // ---- Number of digits ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, num_digits(0)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, num_digits(5)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, num_digits(99)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, num_digits(1000)); // 4

    // ---- Reverse number ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_number(123)); // 321

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_number(1200)); // 21

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_number(5)); // 5

    // ---- Armstrong numbers ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(153)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(370)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(371)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(407)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(100)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_armstrong(1)); // 1

    // ---- Perfect numbers ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_perfect(6)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_perfect(28)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_perfect(496)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_perfect(12)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_perfect(1)); // 0

    // ---- Happy numbers ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_happy(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_happy(7)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_happy(10)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_happy(4)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_happy(2)); // 0

    // ---- Kaprekar routine ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, kaprekar_steps(3524)); // steps to 6174

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, kaprekar_steps(1111)); // -1 (all same)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, kaprekar_steps(6174)); // 0 (already there)

    // ---- GCD ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, gcd(12, 8)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, gcd(17, 13)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, gcd(100, 75)); // 25

    // ---- LCM ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcm(4, 6)); // 12

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lcm(3, 7)); // 21

    // ---- Primes ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_prime(2)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_prime(17)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_prime(4)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_prime(1)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_primes(30)); // 10

    // ---- Digital root ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digital_root(493)); // 7

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digital_root(9)); // 9

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, digital_root(12345)); // 6

    // ---- Factorial ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial(5)); // 120

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial(0)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial(7)); // 5040

    // ---- Factorial sum number ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_factorial_sum(145)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_factorial_sum(100)); // 0

    // ---- Abundant/Deficient ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_abundant(12)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_abundant(6)); // 0 (perfect, not abundant)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_deficient(8)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_deficient(12)); // 0

    return 0;
}
