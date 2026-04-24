int printf(const char *fmt, ...);

int main(void) {
    // Test 1: simple forward goto
    int x = 0;
    goto forward1;
    x = 999;
forward1:
    // EXPECT: forward_goto: 0
    printf("forward_goto: %d\n", x);

    // Test 2: goto skipping multiple statements
    int a = 1;
    int b = 2;
    int c = 3;
    goto skip_all;
    a = 100;
    b = 200;
    c = 300;
skip_all:
    // EXPECT: skip_all: 1 2 3
    printf("skip_all: %d %d %d\n", a, b, c);

    // Test 3: backward goto (loop-like)
    int counter = 0;
loop_start:
    if (counter >= 5) goto loop_end;
    counter = counter + 1;
    goto loop_start;
loop_end:
    // EXPECT: back_goto: 5
    printf("back_goto: %d\n", counter);

    // Test 4: goto-based sum loop
    int sum = 0;
    int i = 1;
sum_loop:
    if (i > 10) goto sum_done;
    sum = sum + i;
    i = i + 1;
    goto sum_loop;
sum_done:
    // EXPECT: goto_sum: 55
    printf("goto_sum: %d\n", sum);

    // Test 5: goto out of a for loop
    int escaped = 0;
    for (i = 0; i < 100; i = i + 1) {
        if (i == 7) {
            escaped = i;
            goto escaped_for;
        }
    }
escaped_for:
    // EXPECT: escape_for: 7
    printf("escape_for: %d\n", escaped);

    // Test 6: goto out of a while loop
    i = 0;
    while (1) {
        if (i == 12) goto escaped_while;
        i = i + 1;
    }
escaped_while:
    // EXPECT: escape_while: 12
    printf("escape_while: %d\n", i);

    // Test 7: goto out of nested loops
    int oi = -1;
    int oj = -1;
    for (i = 0; i < 10; i = i + 1) {
        int j;
        for (j = 0; j < 10; j = j + 1) {
            if (i == 3 && j == 5) {
                oi = i;
                oj = j;
                goto found_it;
            }
        }
    }
found_it:
    // EXPECT: nested_escape: 3 5
    printf("nested_escape: %d %d\n", oi, oj);

    // Test 8: goto in if-else
    int path = 0;
    int cond = 1;
    if (cond) {
        path = 1;
        goto after_if;
    } else {
        path = 2;
    }
    path = 3;
after_if:
    // EXPECT: if_goto: 1
    printf("if_goto: %d\n", path);

    // Test 9: goto into nested block (forward)
    int val = 10;
    goto inside_block;
    val = 0;
    {
        val = 5;
inside_block:
        val = val + 1;
    }
    // val was 10, then +1 = 11
    // EXPECT: into_block: 11
    printf("into_block: %d\n", val);

    // Test 10: multiple labels, conditional goto
    int step = 0;
    int which = 2;
    if (which == 1) goto label_a;
    if (which == 2) goto label_b;
    if (which == 3) goto label_c;
    goto label_end;
label_a:
    step = 1;
    goto label_end;
label_b:
    step = 2;
    goto label_end;
label_c:
    step = 3;
    goto label_end;
label_end:
    // EXPECT: multi_label: 2
    printf("multi_label: %d\n", step);

    // Test 11: goto-based state machine
    int state = 0;
    int output = 0;
    int input_data[6];
    input_data[0] = 1;
    input_data[1] = 2;
    input_data[2] = 0;
    input_data[3] = 1;
    input_data[4] = 0;
    input_data[5] = -1;
    int idx = 0;

state_idle:
    if (input_data[idx] == -1) goto state_done;
    if (input_data[idx] == 0) {
        idx = idx + 1;
        goto state_idle;
    }
    state = 1;
    goto state_active;

state_active:
    output = output + input_data[idx];
    idx = idx + 1;
    if (input_data[idx] == -1) goto state_done;
    if (input_data[idx] == 0) {
        state = 0;
        idx = idx + 1;
        goto state_idle;
    }
    goto state_active;

state_done:
    // output = 1+2 + 1 = 4
    // EXPECT: state_machine: 4
    printf("state_machine: %d\n", output);

    // Test 12: goto skipping variable initialization
    int z = 0;
    goto skip_init;
    z = 42;
skip_init:
    // EXPECT: skip_init_val: 0
    printf("skip_init_val: %d\n", z);

    // Test 13: backward goto countdown
    int cd = 10;
    int cd_sum = 0;
cd_top:
    if (cd <= 0) goto cd_end;
    cd_sum = cd_sum + cd;
    cd = cd - 1;
    goto cd_top;
cd_end:
    // 10+9+8+7+6+5+4+3+2+1 = 55
    // EXPECT: goto_countdown: 55
    printf("goto_countdown: %d\n", cd_sum);

    // Test 14: goto out of do-while
    int dw_val = 0;
    do {
        dw_val = dw_val + 1;
        if (dw_val == 4) goto out_of_dw;
    } while (dw_val < 100);
out_of_dw:
    // EXPECT: dw_goto: 4
    printf("dw_goto: %d\n", dw_val);

    // Test 15: goto-based error handling pattern
    int resource1 = 0;
    int resource2 = 0;
    int err = 0;

    resource1 = 1; // acquire resource 1
    if (resource1 == 0) goto cleanup;

    resource2 = 1; // acquire resource 2
    // Simulate: resource2 acquired fine
    if (resource2 == 0) goto cleanup_r1;

    err = 0; // success
    // EXPECT: err_handling: 0 1 1
    printf("err_handling: %d %d %d\n", err, resource1, resource2);
    goto cleanup_all;

cleanup_r1:
    resource1 = 0;
cleanup:
    err = 1;
cleanup_all:
    // EXPECT: cleanup_done: 0
    printf("cleanup_done: %d\n", err);

    return 0;
}
