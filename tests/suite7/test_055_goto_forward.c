int printf(const char *fmt, ...);

int skip_code(int x) {
    int result = 0;
    if (x > 10) goto done;
    result = x * 2;
done:
    return result;
}

int skip_over_decl(int x) {
    int a = 1;
    if (x == 0) goto end;
    a = a + x;
    {
        int b = a * 3;
        a = b;
    }
end:
    return a;
}

int multi_goto(int x) {
    if (x == 1) goto label_a;
    if (x == 2) goto label_b;
    if (x == 3) goto label_c;
    return -1;

label_a:
    return 10;
label_b:
    return 20;
label_c:
    return 30;
}

int goto_past_block(int x) {
    int val = 0;
    if (x > 0) goto positive;
    val = -100;
    goto finish;
positive:
    val = 100;
finish:
    return val;
}

int error_pattern(int step1, int step2, int step3) {
    int result = 0;
    result = step1;
    if (result < 0) goto error;
    result = result + step2;
    if (result < 0) goto error;
    result = result + step3;
    if (result < 0) goto error;
    return result;
error:
    return -999;
}

int goto_in_loop(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        if (i == 5) goto done;
        sum = sum + i;
        i = i + 1;
    }
done:
    return sum;
}

int goto_skip_else(int x) {
    int r = 0;
    if (x > 0) {
        r = 1;
        goto after;
    } else {
        r = 2;
    }
after:
    r = r + 10;
    return r;
}

int chain_goto(int x) {
    int val = 0;
    goto start;
step3:
    val = val + 100;
    goto done;
step2:
    val = val + 10;
    goto step3;
step1:
    val = val + 1;
    goto step2;
start:
    if (x >= 1) goto step1;
    if (x == 0) goto done;
done:
    return val;
}

int goto_break_equiv(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 100; i = i + 1) {
        if (i >= 10) goto out;
        sum = sum + i;
    }
out:
    return sum;
}

int main(void) {
    // EXPECT: skip 5: 10
    printf("skip 5: %d\n", skip_code(5));
    // EXPECT: skip 15: 0
    printf("skip 15: %d\n", skip_code(15));
    // EXPECT: skipdecl 0: 1
    printf("skipdecl 0: %d\n", skip_over_decl(0));
    // EXPECT: skipdecl 3: 12
    printf("skipdecl 3: %d\n", skip_over_decl(3));
    // EXPECT: multi 1: 10
    printf("multi 1: %d\n", multi_goto(1));
    // EXPECT: multi 2: 20
    printf("multi 2: %d\n", multi_goto(2));
    // EXPECT: multi 3: 30
    printf("multi 3: %d\n", multi_goto(3));
    // EXPECT: multi 9: -1
    printf("multi 9: %d\n", multi_goto(9));
    // EXPECT: past 5: 100
    printf("past 5: %d\n", goto_past_block(5));
    // EXPECT: past -5: -100
    printf("past -5: %d\n", goto_past_block(-5));
    // EXPECT: error 1,2,3: 6
    printf("error 1,2,3: %d\n", error_pattern(1, 2, 3));
    // EXPECT: error 1,-5,3: -999
    printf("error 1,-5,3: %d\n", error_pattern(1, -5, 3));
    // EXPECT: inloop 10: 10
    printf("inloop 10: %d\n", goto_in_loop(10));
    // EXPECT: inloop 3: 3
    printf("inloop 3: %d\n", goto_in_loop(3));
    // EXPECT: skipelse 5: 11
    printf("skipelse 5: %d\n", goto_skip_else(5));
    // EXPECT: skipelse -5: 12
    printf("skipelse -5: %d\n", goto_skip_else(-5));
    // EXPECT: chain 1: 111
    printf("chain 1: %d\n", chain_goto(1));
    // EXPECT: chain 0: 0
    printf("chain 0: %d\n", chain_goto(0));
    // EXPECT: breakequiv: 45
    printf("breakequiv: %d\n", goto_break_equiv());

    return 0;
}
