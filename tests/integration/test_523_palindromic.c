int printf(const char *fmt, ...);
// EXPECT: Palindromic numbers up to 200:\n1 2 3 4 5 6 7 8 9 11 22 33 44 55 66 77 88 99 101 111 121 131 141 151 161 171 181 191 \nCount: 28\n\nThree-digit palindromes:\nCount of 3-digit palindromes: 90\n\nPalindromic primes up to 500:\n2 3 5 7 11 101 131 151 181 191 313 353 373 383 \n\nReverse-and-add process:\n56: 121 (palindrome in 1 steps)\n78: 165 726 1353 4884 (palindrome in 4 steps)\n89: 187 968 1837 9218 17347 91718 173437 907808 1716517 8872688 \n123: 444 (palindrome in 1 steps)\n59: 154 605 1111 (palindrome in 3 steps)\n69: 165 726 1353 4884 (palindrome in 4 steps)\n\nLargest palindrome from product of two 2-digit numbers:\n91 x 99 = 9009
// Test: Palindromic numbers

int reverse_num(int n) {
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }
    return rev;
}

int is_palindrome(int n) {
    if (n < 0) return 0;
    return n == reverse_num(n);
}

int main(void) {
    int i;

    // Find all palindromic numbers up to 200
    printf("Palindromic numbers up to 200:\n");
    int pal_count = 0;
    for (i = 1; i <= 200; i++) {
        if (is_palindrome(i)) {
            printf("%d ", i);
            pal_count++;
        }
    }
    printf("\nCount: %d\n", pal_count);

    // Three-digit palindromes
    printf("\nThree-digit palindromes:\n");
    int count3 = 0;
    for (i = 100; i <= 999; i++) {
        if (is_palindrome(i)) {
            count3++;
        }
    }
    printf("Count of 3-digit palindromes: %d\n", count3);

    // Find palindromes that are also prime
    printf("\nPalindromic primes up to 500:\n");
    for (i = 2; i <= 500; i++) {
        if (is_palindrome(i)) {
            int is_prime = 1;
            int j;
            if (i < 2) is_prime = 0;
            for (j = 2; j * j <= i; j++) {
                if (i % j == 0) { is_prime = 0; break; }
            }
            if (is_prime) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");

    // 196-algorithm: reverse and add, check if palindrome
    printf("\nReverse-and-add process:\n");
    int starts[6];
    starts[0] = 56;
    starts[1] = 78;
    starts[2] = 89;
    starts[3] = 123;
    starts[4] = 59;
    starts[5] = 69;

    for (i = 0; i < 6; i++) {
        int n = starts[i];
        printf("%d: ", n);
        int steps = 0;
        while (!is_palindrome(n) && steps < 10) {
            n = n + reverse_num(n);
            printf("%d ", n);
            steps++;
        }
        if (is_palindrome(n)) {
            printf("(palindrome in %d steps)", steps);
        }
        printf("\n");
    }

    // Largest palindrome from product of two 2-digit numbers
    printf("\nLargest palindrome from product of two 2-digit numbers:\n");
    int max_pal = 0;
    int best_a = 0, best_b = 0;
    int a, b;
    for (a = 10; a <= 99; a++) {
        for (b = a; b <= 99; b++) {
            int prod = a * b;
            if (is_palindrome(prod) && prod > max_pal) {
                max_pal = prod;
                best_a = a;
                best_b = b;
            }
        }
    }
    printf("%d x %d = %d\n", best_a, best_b, max_pal);

    return 0;
}
