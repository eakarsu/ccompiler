int printf(const char *fmt, ...);
// EXPECT: === Test 1: Forward goto ===\nskipped=0\nafter=1\n=== Test 2: Backward goto (manual loop) ===\ni=0\ni=1\ni=2\ni=3\ni=4\nloop done, i=5\n=== Test 3: goto out of nested loops ===\ni=0 j=0\ni=0 j=1\ni=0 j=2\ni=1 j=0\ni=1 j=1\ni=1 j=2\ni=2 j=0\ni=2 j=1\nbroke out at i=2 j=1\n=== Test 4: Error-handling goto cleanup ===\nstep 1 ok\nstep 2 ok\nstep 3 ok\ncleaning up c\ncleaning up b\ncleaning up a\nresult=0\nstep 1 ok\ncleaning up a\nresult=-1\n=== Test 5: Multiple labels ===\nat label_a\nat label_b\nat label_c\ndone with value=30\n=== Test 6: goto in if-else ===\ntook true branch\nafter if-else, x=42\n=== Test 7: Label after a loop ===\nloop: 0\nloop: 1\nloop: 2\nloop: 3\nloop: 4\npost_loop reached, sum=10\n=== Test 8: State machine ===\nstate_idle: event=1\nstate_running: event=2\nstate_done: result=100\nstate_idle: event=3\nstate_done: result=200\n=== Test 9: goto with variable declarations ===\nval=10\nval=20\nval=30\nsum=60\n=== Test 10: Multiple gotos to same label ===\npath=1\ncommon: x=10\npath=2\ncommon: x=20\npath=3\ncommon: x=30\nAll goto tests passed!

// ==========================================================
// Test 1: Forward goto - skip over code
// ==========================================================
void test_forward_goto(void) {
    int skipped = 0;
    printf("=== Test 1: Forward goto ===\n");

    goto skip_ahead;
    skipped = 1;  // this should not execute

skip_ahead:
    printf("skipped=%d\n", skipped);

    int after = 1;
    printf("after=%d\n", after);
}

// ==========================================================
// Test 2: Backward goto - create a manual loop
// ==========================================================
void test_backward_goto(void) {
    printf("=== Test 2: Backward goto (manual loop) ===\n");
    int i = 0;

loop_start:
    if (i >= 5) goto loop_end;
    printf("i=%d\n", i);
    i = i + 1;
    goto loop_start;

loop_end:
    printf("loop done, i=%d\n", i);
}

// ==========================================================
// Test 3: goto out of nested loops (break out of 2 loops)
// ==========================================================
void test_nested_loop_break(void) {
    printf("=== Test 3: goto out of nested loops ===\n");
    int i = 0;
    int j = 0;

    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            printf("i=%d j=%d\n", i, j);
            if (i == 2 && j == 1) {
                goto broke_out;
            }
        }
    }

broke_out:
    printf("broke out at i=%d j=%d\n", i, j);
}

// ==========================================================
// Test 4: Error-handling pattern - goto cleanup
// ==========================================================
int do_work(int fail_at) {
    int a = 0;
    int b = 0;
    int c = 0;
    int result = 0;

    // step 1: allocate a
    a = 1;
    printf("step 1 ok\n");
    if (fail_at == 1) {
        result = -1;
        goto cleanup_a;
    }

    // step 2: allocate b
    b = 1;
    printf("step 2 ok\n");
    if (fail_at == 2) {
        result = -2;
        goto cleanup_b;
    }

    // step 3: allocate c
    c = 1;
    printf("step 3 ok\n");
    if (fail_at == 3) {
        result = -3;
        goto cleanup_c;
    }

    // success path
    result = 0;

cleanup_c:
    if (c) printf("cleaning up c\n");
cleanup_b:
    if (b) printf("cleaning up b\n");
cleanup_a:
    if (a) printf("cleaning up a\n");

    return result;
}

void test_cleanup_pattern(void) {
    printf("=== Test 4: Error-handling goto cleanup ===\n");
    int r;

    r = do_work(0);  // no failure, all cleanup runs
    printf("result=%d\n", r);

    r = do_work(1);  // fail at step 1
    printf("result=%d\n", r);
}

// ==========================================================
// Test 5: Multiple labels in one function
// ==========================================================
void test_multiple_labels(void) {
    printf("=== Test 5: Multiple labels ===\n");
    int value = 0;

    goto label_a;

label_c:
    value = value + 10;
    printf("at label_c\n");
    goto label_done;

label_a:
    value = value + 10;
    printf("at label_a\n");
    goto label_b;

label_b:
    value = value + 10;
    printf("at label_b\n");
    goto label_c;

label_done:
    printf("done with value=%d\n", value);
}

// ==========================================================
// Test 6: goto in if-else - jump from one branch
// ==========================================================
void test_goto_in_if_else(void) {
    printf("=== Test 6: goto in if-else ===\n");
    int x = 0;
    int cond = 1;

    if (cond) {
        printf("took true branch\n");
        x = 42;
        goto after_if;
    } else {
        printf("took false branch\n");
        x = 99;
    }
    x = 0;  // this should be skipped when cond is true

after_if:
    printf("after if-else, x=%d\n", x);
}

// ==========================================================
// Test 7: Label after a loop
// ==========================================================
void test_label_after_loop(void) {
    printf("=== Test 7: Label after a loop ===\n");
    int sum = 0;
    int i;

    for (i = 0; i < 10; i = i + 1) {
        printf("loop: %d\n", i);
        sum = sum + i;
        if (i == 4) goto post_loop;
    }

post_loop:
    printf("post_loop reached, sum=%d\n", sum);
}

// ==========================================================
// Test 8: State machine using goto (3 states)
// ==========================================================
void run_state_machine(int event) {
    int result = 0;

state_idle:
    printf("state_idle: event=%d\n", event);
    if (event == 1) {
        event = 2;
        goto state_running;
    }
    if (event == 3) {
        result = 200;
        goto state_done;
    }
    goto state_done;

state_running:
    printf("state_running: event=%d\n", event);
    if (event == 2) {
        result = 100;
        goto state_done;
    }
    goto state_idle;

state_done:
    printf("state_done: result=%d\n", result);
}

void test_state_machine(void) {
    printf("=== Test 8: State machine ===\n");
    run_state_machine(1);
    run_state_machine(3);
}

// ==========================================================
// Test 9: goto with variable declarations between label and goto
// ==========================================================
void test_goto_with_decls(void) {
    printf("=== Test 9: goto with variable declarations ===\n");
    int count = 0;
    int sum = 0;

again:
    ;  // empty statement after label to allow declaration next
    int val = (count + 1) * 10;
    printf("val=%d\n", val);
    sum = sum + val;
    count = count + 1;
    if (count < 3) goto again;

    printf("sum=%d\n", sum);
}

// ==========================================================
// Test 10: Multiple gotos to same label
// ==========================================================
void test_multi_goto_same_label(int path) {
    int x = 0;

    printf("path=%d\n", path);

    if (path == 1) {
        x = 10;
        goto common;
    }

    if (path == 2) {
        x = 20;
        goto common;
    }

    x = 30;
    goto common;

common:
    printf("common: x=%d\n", x);
}

void test_multiple_gotos(void) {
    printf("=== Test 10: Multiple gotos to same label ===\n");
    test_multi_goto_same_label(1);
    test_multi_goto_same_label(2);
    test_multi_goto_same_label(3);
}

// ==========================================================
// Main
// ==========================================================
int main(void) {
    test_forward_goto();
    test_backward_goto();
    test_nested_loop_break();
    test_cleanup_pattern();
    test_multiple_labels();
    test_goto_in_if_else();
    test_label_after_loop();
    test_state_machine();
    test_goto_with_decls();
    test_multiple_gotos();

    printf("All goto tests passed!\n");
    return 0;
}
