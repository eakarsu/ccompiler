int printf(const char *fmt, ...);

int error_cleanup(int step) {
    int r1 = 0;
    int r2 = 0;
    int r3 = 0;
    int result = -1;

    r1 = 10;
    if (step < 1) goto cleanup3;

    r2 = 20;
    if (step < 2) goto cleanup2;

    r3 = 30;
    if (step < 3) goto cleanup1;

    result = r1 + r2 + r3;
    goto done;

cleanup1:
    r3 = 0;
cleanup2:
    r2 = 0;
cleanup3:
    r1 = 0;
    result = r1 + r2 + r3;

done:
    return result;
}

int multi_level_break(int data[], int rows, int cols) {
    int found_r = -1;
    int found_c = -1;
    int r = 0;
    int c;
    while (r < rows) {
        c = 0;
        while (c < cols) {
            if (data[r * cols + c] == 99) {
                found_r = r;
                found_c = c;
                goto found;
            }
            c = c + 1;
        }
        r = r + 1;
    }
found:
    return found_r * 100 + found_c;
}

int state_machine(int inputs[], int n) {
    int state = 0;
    int output = 0;
    int i = 0;

    while (i < n) {
        int inp = inputs[i];
        switch (state) {
            case 0:
                if (inp == 1) state = 1;
                else state = 0;
                break;
            case 1:
                if (inp == 2) state = 2;
                else if (inp == 1) state = 1;
                else state = 0;
                break;
            case 2:
                if (inp == 3) {
                    state = 3;
                    output = output + 1;
                } else if (inp == 1) {
                    state = 1;
                } else {
                    state = 0;
                }
                break;
            case 3:
                if (inp == 1) state = 1;
                else state = 0;
                break;
        }
        i = i + 1;
    }
    return output;
}

int forward_goto(int x) {
    if (x > 10) goto big;
    if (x > 5) goto medium;
    return 1;
medium:
    return 2;
big:
    return 3;
}

int backward_goto(int n) {
    int sum = 0;
    int i = 0;
loop_start:
    if (i >= n) goto loop_end;
    sum = sum + i;
    i = i + 1;
    goto loop_start;
loop_end:
    return sum;
}

int goto_skip(int arr[], int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] < 0) goto skip;
        sum = sum + arr[i];
skip:
        i = i + 1;
    }
    return sum;
}

int retry_pattern(int attempts) {
    int tries = 0;
    int result = 0;
retry:
    tries = tries + 1;
    if (tries < attempts) {
        result = result + tries;
        goto retry;
    }
    return result;
}

int main(void) {
    // EXPECT: cleanup(0)=0
    printf("cleanup(0)=%d\n", error_cleanup(0));
    // EXPECT: cleanup(1)=0
    printf("cleanup(1)=%d\n", error_cleanup(1));
    // EXPECT: cleanup(2)=0
    printf("cleanup(2)=%d\n", error_cleanup(2));
    // EXPECT: cleanup(3)=60
    printf("cleanup(3)=%d\n", error_cleanup(3));

    int grid[12] = {1,2,3,4, 5,6,99,8, 9,10,11,12};
    // EXPECT: found=102
    printf("found=%d\n", multi_level_break(grid, 3, 4));

    int grid2[6] = {1,2,3,4,5,6};
    // EXPECT: notfound=-101
    printf("notfound=%d\n", multi_level_break(grid2, 2, 3));

    int seq1[6] = {1, 2, 3, 1, 2, 3};
    // EXPECT: matches=2
    printf("matches=%d\n", state_machine(seq1, 6));

    int seq2[4] = {1, 1, 2, 3};
    // EXPECT: matches2=1
    printf("matches2=%d\n", state_machine(seq2, 4));

    // EXPECT: fwd(3)=1
    printf("fwd(3)=%d\n", forward_goto(3));
    // EXPECT: fwd(7)=2
    printf("fwd(7)=%d\n", forward_goto(7));
    // EXPECT: fwd(15)=3
    printf("fwd(15)=%d\n", forward_goto(15));

    // EXPECT: back(5)=10
    printf("back(5)=%d\n", backward_goto(5));
    // EXPECT: back(0)=0
    printf("back(0)=%d\n", backward_goto(0));

    int mixed[6] = {3, -1, 5, -2, 7, 1};
    // EXPECT: skip=16
    printf("skip=%d\n", goto_skip(mixed, 6));

    // EXPECT: retry(4)=6
    printf("retry(4)=%d\n", retry_pattern(4));
    // EXPECT: retry(1)=0
    printf("retry(1)=%d\n", retry_pattern(1));

    return 0;
}
