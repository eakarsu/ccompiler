int printf(const char *fmt, ...);

int goto_forward_backward(int n) {
    int result = 0;
    int i = 0;

    loop_start:
    if (i >= n) goto loop_end;
    result = result + i * i;
    i = i + 1;
    goto loop_start;
    loop_end:
    return result;
}

void goto_skip_blocks(int mode) {
    if (mode == 1) goto phase2;
    if (mode == 2) goto phase3;

    printf("phase1 entered\n");

    phase2:
    printf("phase2 entered\n");

    phase3:
    printf("phase3 entered\n");
}

void goto_error_cleanup(int step_fail) {
    int resources = 0;

    resources = resources + 1;
    if (step_fail == 1) goto cleanup;

    resources = resources + 1;
    if (step_fail == 2) goto cleanup;

    resources = resources + 1;
    if (step_fail == 3) goto cleanup;

    resources = resources + 1;
    printf("all steps ok, resources: %d\n", resources);
    goto done;

    cleanup:
    printf("cleanup at step %d, resources: %d\n", step_fail, resources);
    done:
    return;
}

void goto_nested_escape() {
    int i;
    int j;
    int k;
    int found = 0;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            for (k = 0; k < 10; k = k + 1) {
                if (i * 100 + j * 10 + k == 357) {
                    found = 1;
                    goto escaped;
                }
            }
        }
    }
    escaped:
    printf("escaped at %d %d %d, found: %d\n", i, j, k, found);
}

int goto_state_machine(int input) {
    int state = 0;
    int pos = 0;
    int len = 0;
    int temp = input;
    while (temp > 0) {
        len = len + 1;
        temp = temp / 10;
    }

    int digits[10];
    temp = input;
    int idx = len - 1;
    while (idx >= 0) {
        digits[idx] = temp % 10;
        temp = temp / 10;
        idx = idx - 1;
    }

    int accepted = 0;

    state_A:
    if (pos >= len) goto reject;
    if (digits[pos] == 1) { pos = pos + 1; goto state_B; }
    goto reject;

    state_B:
    if (pos >= len) goto reject;
    if (digits[pos] == 2) { pos = pos + 1; goto state_C; }
    if (digits[pos] == 1) { pos = pos + 1; goto state_B; }
    goto reject;

    state_C:
    if (pos >= len) goto accept;
    if (digits[pos] == 3) { pos = pos + 1; goto state_C; }
    goto reject;

    accept:
    accepted = 1;
    goto end;

    reject:
    accepted = 0;

    end:
    return accepted;
}

int main() {
    // EXPECT: sum squares 5: 30
    printf("sum squares 5: %d\n", goto_forward_backward(5));

    // EXPECT: phase1 entered
    // EXPECT: phase2 entered
    // EXPECT: phase3 entered
    goto_skip_blocks(0);
    // EXPECT: phase2 entered
    // EXPECT: phase3 entered
    goto_skip_blocks(1);
    // EXPECT: phase3 entered
    goto_skip_blocks(2);

    // EXPECT: all steps ok, resources: 4
    goto_error_cleanup(0);
    // EXPECT: cleanup at step 2, resources: 2
    goto_error_cleanup(2);

    // EXPECT: escaped at 3 5 7, found: 1
    goto_nested_escape();

    // EXPECT: accept 12: 1
    printf("accept 12: %d\n", goto_state_machine(12));
    // EXPECT: accept 1123: 1
    printf("accept 1123: %d\n", goto_state_machine(1123));
    // EXPECT: accept 1233: 1
    printf("accept 1233: %d\n", goto_state_machine(1233));
    // EXPECT: accept 21: 0
    printf("accept 21: %d\n", goto_state_machine(21));
    // EXPECT: accept 124: 0
    printf("accept 124: %d\n", goto_state_machine(124));

    return 0;
}
