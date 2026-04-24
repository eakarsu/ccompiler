int printf(const char *fmt, ...);

// Control flow nightmare: goto, nested loops, break, continue, deep nesting,
// long if-else chains, switch with goto, multiple exit points.

int multi_exit_loop(int n) {
    int i = 0;
    int sum = 0;
    for (;;) {
        if (i >= n) break;
        if (i == 7) return sum;
        if (i % 3 == 0 && i > 0) { sum += 100; i++; continue; }
        sum += i;
        i++;
    }
    return sum;
}

int nested_break(int rows, int cols, int target) {
    int i, j;
    int found_i = -1, found_j = -1;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int val = i * cols + j;
            if (val == target) {
                found_i = i;
                found_j = j;
                goto done;
            }
        }
    }
done:
    return found_i * 1000 + found_j;
}

int nested_continue(void) {
    int sum = 0;
    int i, j;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;
        for (j = 0; j < 10; j++) {
            if (j % 3 == 0) continue;
            sum++;
        }
    }
    return sum;
}

int long_if_else(int x) {
    if (x == 0) return 100;
    else if (x == 1) return 101;
    else if (x == 2) return 102;
    else if (x == 3) return 103;
    else if (x == 4) return 104;
    else if (x == 5) return 105;
    else if (x == 6) return 106;
    else if (x == 7) return 107;
    else if (x == 8) return 108;
    else if (x == 9) return 109;
    else if (x == 10) return 110;
    else if (x == 11) return 111;
    else if (x == 12) return 112;
    else if (x == 13) return 113;
    else if (x == 14) return 114;
    else if (x == 15) return 115;
    else if (x == 16) return 116;
    else if (x == 17) return 117;
    else if (x == 18) return 118;
    else if (x == 19) return 119;
    else return -1;
}

int switch_goto_mix(int val) {
    int result = 0;
    switch (val) {
        case 0: result = 10; goto add_bonus;
        case 1: result = 20; break;
        case 2: result = 30; goto add_bonus;
        case 3: result = 40; break;
        case 4: result = 50; goto double_it;
        default: result = -1; break;
    }
    goto finish;
add_bonus:
    result += 5;
    goto finish;
double_it:
    result = result * 2;
finish:
    return result;
}

int goto_in_loop(void) {
    int sum = 0;
    int i = 0;
loop_start:
    if (i >= 20) goto loop_end;
    if (i == 10) {
        sum += 1000;
        i = 15;
        goto loop_start;
    }
    sum += i;
    i++;
    goto loop_start;
loop_end:
    return sum;
}

int deeply_nested_return(int a, int b, int c, int d) {
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (d > 0) {
                    return a + b + c + d;
                } else {
                    if (d == 0) {
                        return a + b + c;
                    } else {
                        return -(a + b + c);
                    }
                }
            } else {
                return a + b;
            }
        } else {
            return a;
        }
    } else {
        return 0;
    }
}

int goto_skip_init(int flag) {
    int x = 0;
    if (flag) goto skip;
    x = 42;
skip:
    return x;
}

int complex_for_loop(void) {
    int sum = 0;
    int i, j;
    for (i = 0, j = 100; i < j; i++, j--) {
        sum += i - (100 - j);
    }
    return sum;
}

int do_while_goto(void) {
    int count = 0;
    int i = 0;
    do {
        i++;
        if (i == 5) goto skip_inc;
        count += i;
    skip_inc:
        if (i >= 10) break;
    } while (1);
    return count;
}

int main(void) {
    // multi_exit_loop: i=0 sum=0, i=1 sum=1, i=2 sum=3, i=3(div3) sum=103,
    // i=4 sum=107, i=5 sum=112, i=6(div3) sum=212, i=7 return 212
    printf("%d\n", multi_exit_loop(20));
    // EXPECT: 212

    // nested_break: target=13, 13 = 1*10+3, found_i=1, found_j=3 => 1*1000+3=1003
    printf("%d\n", nested_break(5, 10, 13));
    // EXPECT: 1003

    // nested_continue: i odd (1,3,5,7,9)=5 values, j not div3 (1,2,4,5,7,8)=6 values => 5*6=30
    printf("%d\n", nested_continue());
    // EXPECT: 30

    // long_if_else chain
    printf("%d\n", long_if_else(0));
    // EXPECT: 100
    printf("%d\n", long_if_else(15));
    // EXPECT: 115
    printf("%d\n", long_if_else(19));
    // EXPECT: 119
    printf("%d\n", long_if_else(20));
    // EXPECT: -1

    // switch_goto_mix
    printf("%d\n", switch_goto_mix(0));
    // EXPECT: 15
    printf("%d\n", switch_goto_mix(1));
    // EXPECT: 20
    printf("%d\n", switch_goto_mix(2));
    // EXPECT: 35
    printf("%d\n", switch_goto_mix(4));
    // EXPECT: 100

    // goto_in_loop: sum of 0..9 = 45, then +1000, then 15+16+17+18+19=85 => 45+1000+85=1130
    printf("%d\n", goto_in_loop());
    // EXPECT: 1130

    // deeply_nested_return
    printf("%d\n", deeply_nested_return(1, 2, 3, 4));
    // EXPECT: 10
    printf("%d\n", deeply_nested_return(1, 2, 3, 0));
    // EXPECT: 6
    printf("%d\n", deeply_nested_return(1, 2, 3, -1));
    // EXPECT: -6
    printf("%d\n", deeply_nested_return(1, 2, -1, 0));
    // EXPECT: 3
    printf("%d\n", deeply_nested_return(1, -1, 0, 0));
    // EXPECT: 1
    printf("%d\n", deeply_nested_return(-1, 0, 0, 0));
    // EXPECT: 0

    // goto_skip_init
    printf("%d\n", goto_skip_init(0));
    // EXPECT: 42
    printf("%d\n", goto_skip_init(1));
    // EXPECT: 0

    // complex_for_loop: i from 0,j from 100, i<j => meet at i=50,j=50
    // sum += i - (100-j) = i - (100-j). Since j=100-i, 100-j=i, so i - i = 0 each iteration => sum=0
    printf("%d\n", complex_for_loop());
    // EXPECT: 0

    // do_while_goto: i=1 count=1, i=2 count=3, i=3 count=6, i=4 count=10,
    // i=5 skip count stays 10, i=6 count=16, i=7 count=23, i=8 count=31,
    // i=9 count=40, i=10 count=50, then break
    printf("%d\n", do_while_goto());
    // EXPECT: 50

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
