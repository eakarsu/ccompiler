int printf(const char *fmt, ...);
// EXPECT: === While and Do-While Tests ===
// EXPECT: count_up: 1 2 3 4 5 6 7 8 9 10
// EXPECT: count_down: 10 9 8 7 6 5 4 3 2 1
// EXPECT: sum_while: sum(1..100)=5050
// EXPECT: while_div: 1024/2 steps: 10
// EXPECT: while_find: found 42 at pos 4
// EXPECT: while_not_found: -1 not found
// EXPECT: while_skip: 1 3 5 7 9
// EXPECT: while_break: stopped at 1 stopped at 2 stopped at 3 stopped at 4 stopped at 5
// EXPECT: while_nested:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)(2,0)(2,1)(2,2)
// EXPECT: while_gcd: gcd(252,105)=21
// EXPECT: while_digits: digits(12345)=5
// EXPECT: while_reverse: reverse(12345)=54321
// EXPECT: while_power: 2^20=1048576
// EXPECT: do_basic: 1 2 3 4 5
// EXPECT: do_at_least_once: executed once, val=0
// EXPECT: do_sum: sum(1..10)=55
// EXPECT: do_countdown: 5 4 3 2 1
// EXPECT: do_until_cond: reached 64
// EXPECT: do_nested:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)
// EXPECT: do_break: stopped at 1 stopped at 2 stopped at 3
// EXPECT: do_continue: 1 2 4 5
// EXPECT: do_digits_sum: digitsum(9876)=30
// EXPECT: while_do_combo: outer=3 inner_sum=6
// EXPECT: while_do_combo: outer=2 inner_sum=3
// EXPECT: while_do_combo: outer=1 inner_sum=1
// EXPECT: collatz: 6 steps from 10 to 1
// EXPECT: binary: binary(42)=101010
// EXPECT: while_flag: iterations=5
// EXPECT: do_menu_sim: action 1
// EXPECT: do_menu_sim: action 2
// EXPECT: do_menu_sim: action 3
// EXPECT: do_menu_sim: done
// EXPECT: nested_while_break:(0,0)(0,1)(1,0)(1,1)(2,0)(2,1)
// EXPECT: do_fib: fib<=100: 1 1 2 3 5 8 13 21 34 55 89
// count_up: 1 2 3 4 5 6 7 8 9 10
// count_down: 10 9 8 7 6 5 4 3 2 1
// sum_while: sum(1..100)=5050
// while_div: 1024/2 steps: 10
// while_find: found 42 at pos 4
// while_not_found: -1 not found
// while_skip: 1 3 5 7 9
// while_break: stopped at 1 stopped at 2 stopped at 3 stopped at 4 stopped at 5
// while_nested:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)(2,0)(2,1)(2,2)
// while_gcd: gcd(252,105)=21
// while_digits: digits(12345)=5
// while_reverse: reverse(12345)=54321
// while_power: 2^20=1048576
// do_basic: 1 2 3 4 5
// do_at_least_once: executed once, val=0
// do_sum: sum(1..10)=55
// do_countdown: 5 4 3 2 1
// do_until_cond: reached 64
// do_nested:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)
// do_break: stopped at 1 stopped at 2 stopped at 3
// do_continue: 1 2 4 5
// do_digits_sum: digitsum(9876)=30
// while_do_combo: outer=3 inner_sum=6
// while_do_combo: outer=2 inner_sum=3
// while_do_combo: outer=1 inner_sum=1
// collatz: 6 steps from 10 to 1
// binary: binary(42)=101010
// while_flag: iterations=5
// do_menu_sim: action 1
// do_menu_sim: action 2
// do_menu_sim: action 3
// do_menu_sim: done
// nested_while_break:(0,0)(0,1)(1,0)(1,1)(2,0)(2,1)
// do_fib: fib<=100: 1 1 2 3 5 8 13 21 34 55 89

int main(void) {
    int i, j, sum, n, r;

    printf("=== While and Do-While Tests ===\n");

    // Basic while: count up
    printf("count_up:");
    i = 1;
    while (i <= 10) {
        printf(" %d", i);
        i++;
    }
    printf("\n");

    // Count down
    printf("count_down:");
    i = 10;
    while (i >= 1) {
        printf(" %d", i);
        i--;
    }
    printf("\n");

    // Sum with while
    sum = 0;
    i = 1;
    while (i <= 100) {
        sum = sum + i;
        i++;
    }
    printf("sum_while: sum(1..100)=%d\n", sum);

    // While with division
    n = 1024;
    i = 0;
    while (n > 1) {
        n = n / 2;
        i++;
    }
    printf("while_div: 1024/2 steps: %d\n", i);

    // While: find element
    int arr[6];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 42; arr[5] = 50;
    i = 0;
    while (i < 6 && arr[i] != 42) {
        i++;
    }
    if (i < 6) {
        printf("while_find: found 42 at pos %d\n", i);
    }

    // While: not found
    i = 0;
    while (i < 6 && arr[i] != -1) {
        i++;
    }
    if (i >= 6) {
        printf("while_not_found: -1 not found\n");
    }

    // While with continue (skip even)
    printf("while_skip:");
    i = 0;
    while (i < 10) {
        i++;
        if (i % 2 == 0) continue;
        printf(" %d", i);
    }
    printf("\n");

    // While with break
    printf("while_break:");
    i = 0;
    while (i < 100) {
        i++;
        if (i > 5) break;
        printf(" stopped at %d", i);
    }
    printf("\n");

    // Nested while
    printf("while_nested:");
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            printf("(%d,%d)", i, j);
            j++;
        }
        i++;
    }
    printf("\n");

    // GCD with while
    int a = 252;
    int b = 105;
    int ga = a;
    int gb = b;
    while (gb != 0) {
        int temp = gb;
        gb = ga % gb;
        ga = temp;
    }
    printf("while_gcd: gcd(%d,%d)=%d\n", a, b, ga);

    // Count digits
    n = 12345;
    int digits = 0;
    int tmp = n;
    while (tmp > 0) {
        digits++;
        tmp = tmp / 10;
    }
    printf("while_digits: digits(%d)=%d\n", n, digits);

    // Reverse a number
    n = 12345;
    int rev = 0;
    tmp = n;
    while (tmp > 0) {
        rev = rev * 10 + tmp % 10;
        tmp = tmp / 10;
    }
    printf("while_reverse: reverse(%d)=%d\n", n, rev);

    // Power of 2 with while
    r = 1;
    i = 0;
    while (i < 20) {
        r = r * 2;
        i++;
    }
    printf("while_power: 2^20=%d\n", r);

    // Basic do-while
    printf("do_basic:");
    i = 1;
    do {
        printf(" %d", i);
        i++;
    } while (i <= 5);
    printf("\n");

    // Do-while executes at least once
    i = 0;
    do {
        i++;
    } while (i > 100);
    printf("do_at_least_once: executed once, val=%d\n", i - 1);

    // Do-while sum
    sum = 0;
    i = 1;
    do {
        sum = sum + i;
        i++;
    } while (i <= 10);
    printf("do_sum: sum(1..10)=%d\n", sum);

    // Do-while countdown
    printf("do_countdown:");
    i = 5;
    do {
        printf(" %d", i);
        i--;
    } while (i >= 1);
    printf("\n");

    // Do-while until condition
    n = 1;
    do {
        n = n * 2;
    } while (n < 64);
    printf("do_until_cond: reached %d\n", n);

    // Nested do-while
    printf("do_nested:");
    i = 0;
    do {
        j = 0;
        do {
            printf("(%d,%d)", i, j);
            j++;
        } while (j < 3);
        i++;
    } while (i < 2);
    printf("\n");

    // Do-while with break
    printf("do_break:");
    i = 0;
    do {
        i++;
        if (i > 3) break;
        printf(" stopped at %d", i);
    } while (i < 100);
    printf("\n");

    // Do-while with continue
    printf("do_continue:");
    i = 0;
    do {
        i++;
        if (i == 3) continue;
        if (i > 5) break;
        printf(" %d", i);
    } while (i < 10);
    printf("\n");

    // Do-while: digit sum
    n = 9876;
    sum = 0;
    tmp = n;
    do {
        sum = sum + tmp % 10;
        tmp = tmp / 10;
    } while (tmp > 0);
    printf("do_digits_sum: digitsum(%d)=%d\n", n, sum);

    // Combined while and do-while
    i = 3;
    while (i > 0) {
        sum = 0;
        j = 1;
        do {
            sum = sum + j;
            j++;
        } while (j <= i);
        printf("while_do_combo: outer=%d inner_sum=%d\n", i, sum);
        i--;
    }

    // Collatz with while
    n = 10;
    int steps = 0;
    tmp = n;
    while (tmp != 1) {
        if (tmp % 2 == 0) {
            tmp = tmp / 2;
        } else {
            tmp = 3 * tmp + 1;
        }
        steps++;
    }
    printf("collatz: %d steps from %d to 1\n", steps, n);

    // Print binary representation using while
    n = 42;
    // First, find the highest bit position
    int bits[32];
    int nbits = 0;
    tmp = n;
    while (tmp > 0) {
        bits[nbits] = tmp % 2;
        tmp = tmp / 2;
        nbits++;
    }
    printf("binary: binary(%d)=", n);
    i = nbits - 1;
    while (i >= 0) {
        printf("%d", bits[i]);
        i--;
    }
    printf("\n");

    // While with flag variable
    int flag = 1;
    int iterations = 0;
    while (flag) {
        iterations++;
        if (iterations >= 5) {
            flag = 0;
        }
    }
    printf("while_flag: iterations=%d\n", iterations);

    // Do-while simulating menu loop
    int actions[4];
    actions[0] = 1; actions[1] = 2; actions[2] = 3; actions[3] = 0;
    i = 0;
    do {
        int action = actions[i];
        if (action == 0) {
            printf("do_menu_sim: done\n");
        } else {
            printf("do_menu_sim: action %d\n", action);
        }
        i++;
    } while (actions[i - 1] != 0);

    // Nested while with break (break inner at j=2)
    printf("nested_while_break:");
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 5) {
            if (j >= 2) break;
            printf("(%d,%d)", i, j);
            j++;
        }
        i++;
    }
    printf("\n");

    // Do-while generating Fibonacci until > 100
    printf("do_fib: fib<=100:");
    int fa = 0;
    int fb = 1;
    do {
        int next = fa + fb;
        fa = fb;
        fb = next;
        if (fa <= 100) {
            printf(" %d", fa);
        }
    } while (fa <= 100);
    printf("\n");

    return 0;
}
