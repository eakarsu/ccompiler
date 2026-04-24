int printf(const char *fmt, ...);

/* test_012_for_loops.c
   Tests: basic for, nested for (3 levels), comma operator in for,
   infinite for with break, for with continue, counting patterns.
*/

int sum_n(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) {
        s += i;
    }
    return s;
}

int sum_even(int n) {
    int s = 0;
    for (int i = 2; i <= n; i += 2) {
        s += i;
    }
    return s;
}

/* nested 2-level: count pairs (i,j) where i+j < 8, 1<=i<=5, 1<=j<=5 */
int count_pairs(void) {
    int count = 0;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            if (i + j < 8) count++;
        }
    }
    return count;
}

/* nested 3-level: count triples (i,j,k) where each in [1,3] */
int count_triples(void) {
    int count = 0;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            for (int k = 1; k <= 3; k++) {
                count++;
            }
        }
    }
    return count;
}

/* for with comma operator in init and update */
int comma_for(void) {
    int a, b, s = 0;
    for (a = 0, b = 10; a < b; a++, b--) {
        s += a + b;
    }
    return s;
}
/* a=0,b=10: s+=10; a=1,b=9: s+=10; a=2,b=8: s+=10; a=3,b=7: s+=10; a=4,b=6: s+=10; a=5,b=5: stop
   5 iterations * 10 = 50 */

/* empty condition = infinite, break out */
int for_break(void) {
    int i = 0;
    for (;;) {
        i++;
        if (i == 7) break;
    }
    return i;
}

/* for with continue: sum only odd numbers 1..10 */
int for_continue(void) {
    int s = 0;
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) continue;
        s += i;
    }
    return s;
    /* 1+3+5+7+9 = 25 */
}

/* break from inner loop only */
int break_inner(void) {
    int s = 0;
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            if (j == 3) break;
            s += j;
        }
    }
    return s;
    /* each i: j=1,2 then break. sum per i = 3. total = 4*3 = 12 */
}

/* for with empty init and empty update */
int for_empty_parts(void) {
    int i = 0, s = 0;
    for (; i < 5;) {
        s += i;
        i++;
    }
    return s;
    /* 0+1+2+3+4 = 10 */
}

/* counting pattern: number of divisors of 36 */
int count_divisors(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) count++;
    }
    return count;
}

/* multiplication table diagonal sum: sum of i*i for i=1..5 */
int diag_sum(void) {
    int s = 0;
    for (int i = 1; i <= 5; i++) {
        s += i * i;
    }
    return s;
    /* 1+4+9+16+25 = 55 */
}

/* 3-level nested sum */
int triple_sum(void) {
    int s = 0;
    for (int i = 1; i <= 2; i++) {
        for (int j = 1; j <= 2; j++) {
            for (int k = 1; k <= 2; k++) {
                s += i * j * k;
            }
        }
    }
    return s;
    /* (1*1*1)+(1*1*2)+(1*2*1)+(1*2*2)+(2*1*1)+(2*1*2)+(2*2*1)+(2*2*2)
       = 1+2+2+4+2+4+4+8 = 27 */
}

int main(void) {
    printf("%d\n", sum_n(10));       // EXPECT: 55
    printf("%d\n", sum_n(5));        // EXPECT: 15
    printf("%d\n", sum_even(10));    // EXPECT: 30
    printf("%d\n", count_pairs());   // EXPECT: 19
    printf("%d\n", count_triples()); // EXPECT: 27
    printf("%d\n", comma_for());     // EXPECT: 50
    printf("%d\n", for_break());     // EXPECT: 7
    printf("%d\n", for_continue());  // EXPECT: 25
    printf("%d\n", break_inner());   // EXPECT: 12
    printf("%d\n", for_empty_parts()); // EXPECT: 10
    printf("%d\n", count_divisors(36)); // EXPECT: 9
    printf("%d\n", diag_sum());      // EXPECT: 55
    printf("%d\n", triple_sum());    // EXPECT: 27

    /* inline loop tests */
    /* sum squares 1..4 = 1+4+9+16 = 30 */
    int s = 0;
    for (int i = 1; i <= 4; i++) s += i * i;
    printf("%d\n", s); // EXPECT: 30

    /* count multiples of 3 in [1,20] */
    int cnt = 0;
    for (int i = 1; i <= 20; i++) if (i % 3 == 0) cnt++;
    printf("%d\n", cnt); // EXPECT: 6

    /* reverse count: 5 downto 1, sum */
    int rs = 0;
    for (int i = 5; i >= 1; i--) rs += i;
    printf("%d\n", rs); // EXPECT: 15

    /* nested: build row sums. For i in [1,3], sum j in [1,i] */
    /* i=1: 1; i=2: 3; i=3: 6 => total = 10 */
    int ts = 0;
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= i; j++)
            ts += j;
    printf("%d\n", ts); // EXPECT: 10

    return 0;
}
