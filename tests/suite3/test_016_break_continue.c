int printf(const char *fmt, ...);

/* test_016_break_continue.c
   Tests: break in for/while/do-while/switch, continue in for/while/do-while,
   break/continue in nested loops (which loop affected).
*/

/* break in for */
int break_for(void) {
    int i;
    for (i = 0; i < 100; i++) {
        if (i == 5) break;
    }
    return i;
}
/* returns 5 */

/* break in while */
int break_while(void) {
    int i = 0;
    while (i < 100) {
        if (i == 7) break;
        i++;
    }
    return i;
}
/* returns 7 */

/* break in do-while */
int break_dowhile(void) {
    int i = 0;
    do {
        if (i == 3) break;
        i++;
    } while (i < 100);
    return i;
}
/* returns 3 */

/* break in switch inside loop: only breaks switch, not loop */
int break_switch_in_loop(void) {
    int s = 0;
    for (int i = 1; i <= 5; i++) {
        switch (i) {
            case 3: break;     /* only breaks switch */
            default: s += i;
        }
    }
    return s;
    /* i=1: default s+=1; i=2: default s+=2; i=3: case 3 break (switch); i=4: default s+=4; i=5: default s+=5 */
    /* s = 1+2+4+5 = 12 */
}

/* continue in for */
int continue_for(void) {
    int s = 0;
    for (int i = 1; i <= 10; i++) {
        if (i % 3 == 0) continue;
        s += i;
    }
    return s;
    /* skip 3,6,9; sum = 1+2+4+5+7+8+10 = 37 */
}

/* continue in while */
int continue_while(void) {
    int s = 0, i = 0;
    while (i < 10) {
        i++;
        if (i == 5) continue;
        s += i;
    }
    return s;
    /* skip i=5; sum = 1+2+3+4+6+7+8+9+10 = 50 */
}

/* continue in do-while */
int continue_dowhile(void) {
    int s = 0, i = 0;
    do {
        i++;
        if (i % 2 == 0) continue;
        s += i;
    } while (i < 10);
    return s;
    /* sum odd numbers 1,3,5,7,9 = 25 */
}

/* break affects innermost loop only */
int break_inner_loop(void) {
    int s = 0;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 10; j++) {
            if (j > i) break;
            s += j;
        }
    }
    return s;
    /* i=1: j=1 (j>1 break) s+=1
       i=2: j=1,2 (j>2 break) s+=3
       i=3: j=1,2,3 (j>3 break) s+=6
       total = 1+3+6 = 10 */
}

/* continue affects innermost loop only */
int continue_inner_loop(void) {
    int s = 0;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 4; j++) {
            if (j == 2) continue;
            s += i * j;
        }
    }
    return s;
    /* For each i, j=1,3,4 (j=2 skipped)
       i=1: 1+3+4=8
       i=2: 2+6+8=16
       i=3: 3+9+12=24
       total = 48 */
}

/* break with flag pattern */
int break_with_flag(int n) {
    int found = 0;
    for (int i = 2; i < n; i++) {
        if (n % i == 0) { found = i; break; }
    }
    return found;
}
/* smallest factor > 1 of n, or 0 if prime */
/* break_with_flag(7) = 0 (prime) */
/* break_with_flag(12) = 2 */
/* break_with_flag(15) = 3 */

/* nested break: outer break via flag */
int outer_break(int target) {
    int s = 0;
    int done = 0;
    for (int i = 1; i <= 5 && !done; i++) {
        for (int j = 1; j <= 5; j++) {
            s += i * j;
            if (i * j >= target) { done = 1; break; }
        }
    }
    return s;
}
/* outer_break(10): i=1: j=1(s=1),2(s=3),3(s=6),4(s=10) 1*4=4<10, j=5(s=11) 1*5=5<10; i=2: j=1(s=13),2(s=17),3(s=23),4(s=31),5(s=41); i=3: j=1(s=44),2(s=50),3(s=59)... wait let me re-check:
Actually s accumulates. At i=1: j=1:s=1, j=2:s=3, j=3:s=6, j=4:s=10 (1*4=4<10), j=5:s=15 (1*5=5<10). No break in i=1.
i=2: j=1:s=17, j=2:s=21 (2*2=4<10), j=3:s=27 (2*3=6<10), j=4:s=35 (2*4=8<10), j=5:s=45 (2*5=10>=10 => done=1, break)
outer loop: !done=false => stops. returns 45 */

int main(void) {
    printf("%d\n", break_for());      // EXPECT: 5
    printf("%d\n", break_while());    // EXPECT: 7
    printf("%d\n", break_dowhile());  // EXPECT: 3

    printf("%d\n", break_switch_in_loop()); // EXPECT: 12

    printf("%d\n", continue_for());     // EXPECT: 37
    printf("%d\n", continue_while());   // EXPECT: 50
    printf("%d\n", continue_dowhile()); // EXPECT: 25

    printf("%d\n", break_inner_loop());    // EXPECT: 10
    printf("%d\n", continue_inner_loop()); // EXPECT: 48

    printf("%d\n", break_with_flag(7));  // EXPECT: 0
    printf("%d\n", break_with_flag(12)); // EXPECT: 2
    printf("%d\n", break_with_flag(15)); // EXPECT: 3

    printf("%d\n", outer_break(10)); // EXPECT: 45

    /* Inline tests */
    /* break exits loop, check value of counter */
    int k = 0;
    for (int i = 0; i < 20; i++) { k = i; if (k == 9) break; }
    printf("%d\n", k); // EXPECT: 9

    /* continue: count times loop body runs after continue */
    int runs = 0;
    for (int i = 0; i < 10; i++) {
        if (i < 5) continue;
        runs++;
    }
    printf("%d\n", runs); // EXPECT: 5

    /* do-while that uses break immediately on first iteration */
    int val = 0;
    do {
        val = 1;
        break;
        val = 2;
    } while (1);
    printf("%d\n", val); // EXPECT: 1

    return 0;
}
