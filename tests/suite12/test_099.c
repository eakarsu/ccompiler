int printf(const char *fmt, ...);

// Goto spaghetti: forward, backward, and cross jumps

int forward_skip(int x) {
    int result;
    result = 0;

    if (x > 10) goto big;
    if (x > 5) goto medium;
    result = x * 100;
    goto done;

medium:
    result = x * 10;
    goto done;

big:
    result = x;

done:
    return result;
}

int backward_loop(int n) {
    int sum;
    int i;
    sum = 0;
    i = 0;

loop_start:
    if (i >= n) goto loop_end;
    sum = sum + i;
    i = i + 1;
    goto loop_start;

loop_end:
    return sum;
}

int cross_jump(int a, int b) {
    int x;
    int y;
    x = a;
    y = b;

    if (x > y) goto x_bigger;
    goto y_bigger_or_equal;

x_bigger:
    x = x - y;
    if (x == 0) goto both_zero_check;
    if (x > y) goto x_bigger;
    goto y_bigger_or_equal;

y_bigger_or_equal:
    y = y - x;
    if (y == 0) goto both_zero_check;
    if (x > y) goto x_bigger;
    goto y_bigger_or_equal;

both_zero_check:
    if (x == 0) return y;
    return x;
}

int state_machine(int input) {
    int state;
    int count;
    int pos;
    int data[8];
    int len;

    data[0] = 1; data[1] = 2; data[2] = 0;
    data[3] = 3; data[4] = 1; data[5] = 0;
    data[6] = 2; data[7] = 0;
    len = 8;
    state = 0;
    count = 0;
    pos = 0;

state_a:
    if (pos >= len) goto state_done;
    if (data[pos] == 0) {
        count = count + 10;
        pos = pos + 1;
        goto state_a;
    }
    if (data[pos] == 1) {
        count = count + 1;
        pos = pos + 1;
        goto state_b;
    }
    if (data[pos] == 2) {
        count = count + 2;
        pos = pos + 1;
        goto state_c;
    }
    pos = pos + 1;
    goto state_a;

state_b:
    if (pos >= len) goto state_done;
    if (data[pos] == 0) {
        count = count + 20;
        pos = pos + 1;
        goto state_a;
    }
    if (data[pos] == 2) {
        count = count + 4;
        pos = pos + 1;
        goto state_c;
    }
    count = count + 3;
    pos = pos + 1;
    goto state_b;

state_c:
    if (pos >= len) goto state_done;
    if (data[pos] == 0) {
        count = count + 30;
        pos = pos + 1;
        goto state_a;
    }
    if (data[pos] == 1) {
        count = count + 5;
        pos = pos + 1;
        goto state_b;
    }
    count = count + 6;
    pos = pos + 1;
    goto state_c;

state_done:
    return count;
}

int zigzag(int n) {
    int val;
    int i;
    val = 0;
    i = 0;

step1:
    if (i >= n) goto finish;
    val = val + 1;
    i = i + 1;
    goto step2;

step2:
    if (i >= n) goto finish;
    val = val + 2;
    i = i + 1;
    goto step3;

step3:
    if (i >= n) goto finish;
    val = val + 3;
    i = i + 1;
    goto step1;

finish:
    return val;
}

int multi_exit(int a, int b, int c) {
    int x;
    x = a;

check1:
    if (x > 100) goto exit_big;
    x = x + b;

check2:
    if (x > 100) goto exit_big;
    x = x + c;

check3:
    if (x > 100) goto exit_big;
    x = x * 2;
    if (x < 50) goto check1;
    goto exit_normal;

exit_big:
    return x;

exit_normal:
    return -x;
}

int main() {
    // Forward skip tests
    printf("%d\n", forward_skip(3));   // EXPECT: 300
    printf("%d\n", forward_skip(7));   // EXPECT: 70
    printf("%d\n", forward_skip(15));  // EXPECT: 15

    // Backward loop (sum 0..n-1)
    printf("%d\n", backward_loop(5));   // EXPECT: 10
    printf("%d\n", backward_loop(10));  // EXPECT: 45
    printf("%d\n", backward_loop(0));   // EXPECT: 0

    // Cross jump (GCD)
    printf("%d\n", cross_jump(12, 8));   // EXPECT: 4
    printf("%d\n", cross_jump(7, 7));    // EXPECT: 7
    printf("%d\n", cross_jump(100, 75)); // EXPECT: 25
    printf("%d\n", cross_jump(17, 13));  // EXPECT: 1

    // State machine
    // data = [1,2,0,3,1,0,2,0]
    // pos0: d=1, state_a: count+=1=1, goto state_b
    // pos1: d=2, state_b: count+=4=5, goto state_c
    // pos2: d=0, state_c: count+=30=35, goto state_a
    // pos3: d=3, state_a: (not 0,1,2) pos++, goto state_a
    // pos4: d=1, state_a: count+=1=36, goto state_b
    // pos5: d=0, state_b: count+=20=56, goto state_a
    // pos6: d=2, state_a: count+=2=58, goto state_c
    // pos7: d=0, state_c: count+=30=88, goto state_a
    // pos8: done
    printf("%d\n", state_machine(0)); // EXPECT: 88

    // Zigzag: adds 1,2,3,1,2,3,...
    // n=6: 1+2+3+1+2+3=12
    printf("%d\n", zigzag(6)); // EXPECT: 12
    // n=7: 1+2+3+1+2+3+1=13
    printf("%d\n", zigzag(7)); // EXPECT: 13
    // n=1: 1
    printf("%d\n", zigzag(1)); // EXPECT: 1

    // Multi-exit
    // a=5,b=10,c=20: x=5; x+=10=15; x+=20=35; x*2=70<50?no => exit_normal=-70
    printf("%d\n", multi_exit(5, 10, 20)); // EXPECT: -70

    // a=1,b=3,c=2: x=1; +3=4; +2=6; *2=12<50 goto check1
    // x=12; +3=15; +2=17; *2=34<50 goto check1
    // x=34; +3=37; +2=39; *2=78 >=50 exit_normal=-78
    printf("%d\n", multi_exit(1, 3, 2)); // EXPECT: -78

    // a=50,b=60,c=0: x=50; +60=110>100 exit_big=110
    printf("%d\n", multi_exit(50, 60, 0)); // EXPECT: 110

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
