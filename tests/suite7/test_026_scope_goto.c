int printf(const char *fmt, ...);

void test_goto_forward(void) {
    int x = 10;
    // EXPECT: before goto x = 10
    printf("before goto x = %d\n", x);
    goto skip;
    x = 999;
skip:
    // EXPECT: after goto x = 10
    printf("after goto x = %d\n", x);
}

void test_goto_backward(void) {
    int count = 0;
    int x = 5;
loop_start:
    if (count >= 3) goto loop_end;
    count = count + 1;
    x = x + count;
    goto loop_start;
loop_end:
    // EXPECT: backward goto x = 11
    printf("backward goto x = %d\n", x);
    // EXPECT: backward goto count = 3
    printf("backward goto count = %d\n", count);
}

void test_goto_skip_block(void) {
    int a = 1;
    goto after_block;
    {
        int a = 999;
        a = a + 1;
    }
after_block:
    // EXPECT: skip block a = 1
    printf("skip block a = %d\n", a);
}

void test_goto_into_shared_scope(void) {
    int result = 0;
    int flag = 1;
    if (flag) {
        result = 10;
        goto merge;
    }
    result = 20;
merge:
    // EXPECT: merge result = 10
    printf("merge result = %d\n", result);
}

void test_goto_accumulate(void) {
    int sum = 0;
    int i = 0;
begin:
    if (i >= 5) goto done;
    {
        int val = i * 2;
        sum = sum + val;
    }
    i = i + 1;
    goto begin;
done:
    // EXPECT: goto accum sum = 20
    printf("goto accum sum = %d\n", sum);
    // EXPECT: goto accum i = 5
    printf("goto accum i = %d\n", i);
}

void test_goto_multi_label(void) {
    int state = 0;
    int x = 0;
    // EXPECT: state_a x = 10
    // EXPECT: state_b x = 30
    // EXPECT: state_c x = 130
    // EXPECT: final state x = 130
    goto state_a;

state_c:
    x = x + 100;
    printf("state_c x = %d\n", x);
    goto state_end;

state_a:
    x = x + 10;
    state = 1;
    printf("state_a x = %d\n", x);
    goto state_b;

state_b:
    x = x + 20;
    printf("state_b x = %d\n", x);
    goto state_c;

state_end:
    printf("final state x = %d\n", x);
}

void test_goto_with_vars(void) {
    int a = 5;
    int b = 10;
    // EXPECT: pre-goto a=5 b=10
    printf("pre-goto a=%d b=%d\n", a, b);
    a = a + 1;
    b = b + 1;
    goto checkpoint;
    a = 999;
    b = 999;
checkpoint:
    // EXPECT: post-goto a=6 b=11
    printf("post-goto a=%d b=%d\n", a, b);
}

int main(void) {
    test_goto_forward();
    test_goto_backward();
    test_goto_skip_block();
    test_goto_into_shared_scope();
    test_goto_accumulate();
    test_goto_multi_label();
    test_goto_with_vars();

    int done = 0;
    int val = 0;
start:
    if (done) goto finish;
    val = 42;
    done = 1;
    goto start;
finish:
    // EXPECT: main goto val = 42
    printf("main goto val = %d\n", val);

    return 0;
}
