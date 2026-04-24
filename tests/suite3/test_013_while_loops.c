int printf(const char *fmt, ...);

/* test_013_while_loops.c
   Tests: while loops, nested while, while with break/continue,
   do-while, do-while with complex conditions.
*/

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) n = n / 2;
        else n = 3 * n + 1;
        steps++;
    }
    return steps;
}
/* collatz_steps(6): 6->3->10->5->16->8->4->2->1 = 8 steps */
/* collatz_steps(1): 0 steps */

int sum_digits(int n) {
    int s = 0;
    if (n < 0) n = -n;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}
/* sum_digits(1234) = 1+2+3+4 = 10 */

int count_bits(int n) {
    int count = 0;
    while (n > 0) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
/* count_bits(7) = 3, count_bits(8) = 1 */

/* nested while: compute product of two using repeated addition */
int mul_loop(int a, int b) {
    int result = 0;
    int i = 0;
    while (i < a) {
        int j = 0;
        while (j < b) {
            result++;
            j++;
        }
        i++;
    }
    return result;
}
/* mul_loop(4, 3) = 12 */

/* while with break: find first multiple of 7 >= n */
int first_mult7(int n) {
    int x = n;
    while (1) {
        if (x % 7 == 0) break;
        x++;
    }
    return x;
}
/* first_mult7(15) = 21 */

/* while with continue: sum numbers 1..20 not divisible by 4 */
int sum_skip4(void) {
    int s = 0, i = 0;
    while (i < 20) {
        i++;
        if (i % 4 == 0) continue;
        s += i;
    }
    return s;
    /* sum 1..20 = 210, minus (4+8+12+16+20) = 60, => 150 */
}

/* do-while: always executes at least once */
int do_while_count(int n) {
    int count = 0;
    do {
        count++;
        n--;
    } while (n > 0);
    return count;
}
/* do_while_count(0) = 1 (executes once even though n=0) */
/* do_while_count(5) = 5 */

/* do-while reading digits from number */
int digit_count(int n) {
    int count = 0;
    if (n < 0) n = -n;
    do {
        count++;
        n /= 10;
    } while (n > 0);
    return count;
}
/* digit_count(0) = 1 */
/* digit_count(999) = 3 */
/* digit_count(1000) = 4 */

/* do-while with complex condition: stop when sum exceeds limit or count >= max */
int do_sum_limit(int limit, int max_count) {
    int s = 0, i = 1;
    do {
        s += i;
        i++;
    } while (s <= limit && i <= max_count);
    return s;
}
/* do_sum_limit(10, 100): 1+2+3+4=10 (s=10 <= 10, continue), 1+2+3+4+5=15 > 10 stop => returns 15 */
/* do_sum_limit(100, 3): 1+2+3=6, i=4 > 3, stop => returns 6 */

/* nested while counting down */
int nested_while_sum(int n) {
    int s = 0, i = n;
    while (i > 0) {
        int j = i;
        while (j > 0) {
            s += j;
            j--;
        }
        i--;
    }
    return s;
}
/* n=3: i=3: j=3,2,1 (sum+=6); i=2: j=2,1 (sum+=3); i=1: j=1 (sum+=1) => 10 */

int main(void) {
    printf("%d\n", collatz_steps(6));  // EXPECT: 8
    printf("%d\n", collatz_steps(1));  // EXPECT: 0
    printf("%d\n", collatz_steps(27)); // EXPECT: 111

    printf("%d\n", sum_digits(1234));  // EXPECT: 10
    printf("%d\n", sum_digits(0));     // EXPECT: 0
    printf("%d\n", sum_digits(9999));  // EXPECT: 36

    printf("%d\n", count_bits(7));  // EXPECT: 3
    printf("%d\n", count_bits(8));  // EXPECT: 1
    printf("%d\n", count_bits(15)); // EXPECT: 4
    printf("%d\n", count_bits(0));  // EXPECT: 0

    printf("%d\n", mul_loop(4, 3)); // EXPECT: 12
    printf("%d\n", mul_loop(5, 5)); // EXPECT: 25
    printf("%d\n", mul_loop(0, 7)); // EXPECT: 0

    printf("%d\n", first_mult7(15)); // EXPECT: 21
    printf("%d\n", first_mult7(14)); // EXPECT: 14
    printf("%d\n", first_mult7(1));  // EXPECT: 7

    printf("%d\n", sum_skip4()); // EXPECT: 150

    printf("%d\n", do_while_count(0)); // EXPECT: 1
    printf("%d\n", do_while_count(5)); // EXPECT: 5

    printf("%d\n", digit_count(0));    // EXPECT: 1
    printf("%d\n", digit_count(999));  // EXPECT: 3
    printf("%d\n", digit_count(1000)); // EXPECT: 4

    printf("%d\n", do_sum_limit(10, 100)); // EXPECT: 15
    printf("%d\n", do_sum_limit(100, 3));  // EXPECT: 6

    printf("%d\n", nested_while_sum(3)); // EXPECT: 10
    printf("%d\n", nested_while_sum(4)); // EXPECT: 20

    /* while loop that never runs body when condition false initially */
    int x = 0;
    while (x > 0) { x++; }
    printf("%d\n", x); // EXPECT: 0

    /* do-while that runs exactly once */
    int y = 0;
    do { y = 42; } while (0);
    printf("%d\n", y); // EXPECT: 42

    return 0;
}
