int printf(const char *fmt, ...);
// EXPECT: === Goto and Labels Tests ===
// EXPECT: forward_jump: before goto
// EXPECT: forward_jump: after label
// EXPECT: backward_jump: count 0 1 2 3 4
// EXPECT: skip_code: A then C
// EXPECT: goto_else: took else path
// EXPECT: nested_goto_out: i=0 j=0
// EXPECT: nested_goto_out: i=0 j=1
// EXPECT: nested_goto_out: i=0 j=2
// EXPECT: nested_goto_out: broke out at i=0 j=2
// EXPECT: goto_skip_init: x is still 10
// EXPECT: multi_label: visited A
// EXPECT: multi_label: visited B
// EXPECT: multi_label: visited C
// EXPECT: multi_label: done
// EXPECT: goto_loop_sum: sum=55
// EXPECT: goto_cleanup: resource acquired
// EXPECT: goto_cleanup: cleaning up, code=0
// EXPECT: goto_error: error case
// EXPECT: goto_error: cleaning up, code=1
// EXPECT: goto_chain: start->mid->end val=3
// EXPECT: goto_in_switch: found 3 via goto
// EXPECT: goto_countdown: 5 4 3 2 1 liftoff
// EXPECT: goto_search: found target 7 at [1][2]
// EXPECT: goto_state: state machine: A B C D done
// EXPECT: goto_skip_block: before block
// EXPECT: goto_skip_block: after block, x=10
// EXPECT: goto_alternating: a b a done
// EXPECT: goto_accumulate: sum of 1..10=55
// EXPECT: computed_jump: action for 0: alpha
// EXPECT: computed_jump: action for 1: beta
// EXPECT: computed_jump: action for 2: gamma
// EXPECT: computed_jump: action for 3: delta
// EXPECT: goto_nested_cleanup: level 3 cleanup
// EXPECT: goto_nested_cleanup: level 2 cleanup
// EXPECT: goto_nested_cleanup: level 1 cleanup
// EXPECT: goto_nested_cleanup: all cleaned
// forward_jump: before goto
// forward_jump: after label
// backward_jump: count 0 1 2 3 4
// skip_code: A then C
// goto_else: took else path
// nested_goto_out: i=0 j=0
// nested_goto_out: i=0 j=1
// nested_goto_out: i=0 j=2
// nested_goto_out: broke out at i=0 j=2
// goto_skip_init: x is still 10
// multi_label: visited A
// multi_label: visited B
// multi_label: visited C
// multi_label: done
// goto_loop_sum: sum=55
// goto_cleanup: resource acquired
// goto_cleanup: cleaning up, code=0
// goto_error: error case
// goto_error: cleaning up, code=1
// goto_chain: start->mid->end val=3
// goto_in_switch: found 3 via goto
// goto_countdown: 5 4 3 2 1 liftoff
// goto_search: found target 7 at [1][2]
// goto_state: state machine: A B C D done
// goto_skip_block: before block
// goto_skip_block: after block, x=10
// goto_alternating: a b a done
// goto_accumulate: sum of 1..10=55
// computed_jump: action for 0: alpha
// computed_jump: action for 1: beta
// computed_jump: action for 2: gamma
// computed_jump: action for 3: delta
// goto_nested_cleanup: level 3 cleanup
// goto_nested_cleanup: level 2 cleanup
// goto_nested_cleanup: level 1 cleanup
// goto_nested_cleanup: all cleaned

int main(void) {
    int i, j, sum, x, code;

    printf("=== Goto and Labels Tests ===\n");

    // Forward jump
    printf("forward_jump: before goto\n");
    goto forward_target;
    printf("forward_jump: this should not print\n");
forward_target:
    printf("forward_jump: after label\n");

    // Backward jump (loop via goto)
    printf("backward_jump: count");
    i = 0;
back_loop:
    if (i >= 5) goto back_done;
    printf(" %d", i);
    i++;
    goto back_loop;
back_done:
    printf("\n");

    // Skip over code
    printf("skip_code: A");
    goto skip_b;
    printf(" B");
skip_b:
    printf(" then C\n");

    // Goto into else-like path
    x = 5;
    if (x > 10) {
        printf("goto_else: took if path\n");
        goto end_if_else;
    }
    goto else_path;
    printf("goto_else: unreachable\n");
else_path:
    printf("goto_else: took else path\n");
end_if_else:

    // Goto out of nested loops
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("nested_goto_out: i=%d j=%d\n", i, j);
            if (j == 2) goto outer_break;
        }
    }
outer_break:
    printf("nested_goto_out: broke out at i=%d j=%d\n", i, j);

    // Goto skipping initialization
    x = 10;
    goto skip_init;
    x = 999;
skip_init:
    printf("goto_skip_init: x is still %d\n", x);

    // Multiple labels in sequence
    goto label_a;
label_c:
    printf("multi_label: visited C\n");
    goto label_done;
label_a:
    printf("multi_label: visited A\n");
    goto label_b;
label_b:
    printf("multi_label: visited B\n");
    goto label_c;
label_done:
    printf("multi_label: done\n");

    // Goto-based loop summing 1 to 10
    sum = 0;
    i = 1;
goto_loop_top:
    if (i > 10) goto goto_loop_end;
    sum = sum + i;
    i++;
    goto goto_loop_top;
goto_loop_end:
    printf("goto_loop_sum: sum=%d\n", sum);

    // Goto for cleanup pattern (success path)
    code = 0;
    printf("goto_cleanup: resource acquired\n");
    if (code == 0) goto cleanup;
    printf("goto_cleanup: processing\n");
cleanup:
    printf("goto_cleanup: cleaning up, code=%d\n", code);

    // Goto for error path
    code = 1;
    if (code != 0) {
        printf("goto_error: error case\n");
        goto error_cleanup;
    }
    printf("goto_error: success\n");
error_cleanup:
    printf("goto_error: cleaning up, code=%d\n", code);

    // Goto chain: multiple jumps
    x = 0;
    goto chain_start;
chain_mid:
    x = x + 1;
    goto chain_end;
chain_start:
    x = x + 2;
    goto chain_mid;
chain_end:
    printf("goto_chain: start->mid->end val=%d\n", x);

    // Goto from within a switch
    i = 3;
    switch (i) {
    case 1:
        printf("goto_in_switch: found 1\n");
        break;
    case 3:
        goto switch_special;
        break;
    default:
        printf("goto_in_switch: default\n");
        break;
    }
    goto switch_done;
switch_special:
    printf("goto_in_switch: found 3 via goto\n");
switch_done:

    // Goto countdown
    printf("goto_countdown:");
    i = 5;
count_down:
    if (i <= 0) goto liftoff;
    printf(" %d", i);
    i--;
    goto count_down;
liftoff:
    printf(" liftoff\n");

    // Goto to break out of 2D search
    int grid[3][3];
    grid[0][0] = 1; grid[0][1] = 2; grid[0][2] = 3;
    grid[1][0] = 4; grid[1][1] = 5; grid[1][2] = 7;
    grid[2][0] = 8; grid[2][1] = 9; grid[2][2] = 6;
    int fi = -1;
    int fj = -1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (grid[i][j] == 7) {
                fi = i;
                fj = j;
                goto found_target;
            }
        }
    }
found_target:
    printf("goto_search: found target 7 at [%d][%d]\n", fi, fj);

    // State machine with goto
    printf("goto_state: state machine:");
    goto state_A;
state_A:
    printf(" A");
    goto state_B;
state_B:
    printf(" B");
    goto state_C;
state_C:
    printf(" C");
    goto state_D;
state_D:
    printf(" D");
    goto state_end;
state_end:
    printf(" done\n");

    // Goto skipping a block
    x = 10;
    printf("goto_skip_block: before block\n");
    goto after_block;
    {
        x = 20;
        printf("goto_skip_block: inside block\n");
    }
after_block:
    printf("goto_skip_block: after block, x=%d\n", x);

    // Alternating between two labels
    i = 0;
    printf("goto_alternating:");
    goto alt_a;
alt_a:
    printf(" a");
    i++;
    if (i >= 3) goto alt_done;
    goto alt_b;
alt_b:
    printf(" b");
    i++;
    if (i >= 5) goto alt_done;
    goto alt_a;
alt_done:
    printf(" done\n");

    // Goto with accumulation
    sum = 0;
    i = 1;
acc_top:
    if (i > 10) goto acc_end;
    sum = sum + i;
    i = i + 1;
    goto acc_top;
acc_end:
    printf("goto_accumulate: sum of 1..10=%d\n", sum);

    // Simulated computed jump via lookup table (using if-goto chain)
    for (i = 0; i < 4; i++) {
        printf("computed_jump: action for %d: ", i);
        if (i == 0) goto act_0;
        if (i == 1) goto act_1;
        if (i == 2) goto act_2;
        if (i == 3) goto act_3;
        goto act_end;
    act_0:
        printf("alpha\n");
        goto act_end;
    act_1:
        printf("beta\n");
        goto act_end;
    act_2:
        printf("gamma\n");
        goto act_end;
    act_3:
        printf("delta\n");
        goto act_end;
    act_end:
        ;
    }

    // Nested cleanup with goto (multi-level resource cleanup)
    int level = 0;
    level = 1;
    // Simulate acquiring resources at each level
    level = 2;
    level = 3;
    // Simulate failure at level 3
    goto nested_cleanup_3;

    goto nested_done;  // Skip cleanup in success case

nested_cleanup_3:
    printf("goto_nested_cleanup: level 3 cleanup\n");
nested_cleanup_2:
    printf("goto_nested_cleanup: level 2 cleanup\n");
nested_cleanup_1:
    printf("goto_nested_cleanup: level 1 cleanup\n");
    printf("goto_nested_cleanup: all cleaned\n");
nested_done:

    return 0;
}
