int printf(const char *fmt, ...);

// Complex loop with 5 different variables

int multi_var_loop1() {
    int a = 0;
    int b = 1;
    int c = 0;
    int d = 100;
    int e = 1;
    int i = 0;
    while (i < 20) {
        a = a + i;
        b = b * (i % 3 + 1);
        c = c + (i * i);
        d = d - (i % 5);
        e = e + (i & 1);
        i = i + 1;
    }
    return a + (b % 1000) + c + d + e;
}

int multi_var_loop2(int n) {
    int sum = 0;
    int product = 1;
    int count = 0;
    int min_val = n;
    int max_val = 0;
    int i = 1;
    while (i <= n) {
        int val = (i * 7 + 3) % 100;
        sum = sum + val;
        if (val > 1 && product < 10000) {
            product = product * (val % 10 + 1);
        }
        count = count + 1;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
        i = i + 1;
    }
    return sum + count + min_val + max_val;
}

int nested_multi_var(int rows, int cols) {
    int total = 0;
    int row_max = 0;
    int col_sum = 0;
    int diag = 0;
    int anti_diag = 0;
    int r = 0;
    while (r < rows) {
        int row_sum = 0;
        int c = 0;
        while (c < cols) {
            int val = (r + 1) * (c + 1);
            total = total + val;
            row_sum = row_sum + val;
            if (c == 0) col_sum = col_sum + val;
            if (r == c) diag = diag + val;
            if (r + c == rows - 1) anti_diag = anti_diag + val;
            c = c + 1;
        }
        if (row_sum > row_max) row_max = row_sum;
        r = r + 1;
    }
    return total + row_max + col_sum + diag + anti_diag;
}

int do_while_complex(int start) {
    int a = start;
    int b = 0;
    int c = 1;
    int d = start;
    int e = 0;
    do {
        b = b + a % 10;
        c = c * (a % 3 + 1);
        if (c > 10000) c = c % 10000;
        d = d / 2;
        e = e + 1;
        a = a - 1;
    } while (a > 0);
    return b + (c % 1000) + d + e;
}

int for_with_continue(int n) {
    int sum = 0;
    int skip_count = 0;
    int process_count = 0;
    int last_processed = 0;
    int max_gap = 0;
    int prev_processed = -1;
    int i = 0;
    while (i < n) {
        if (i % 3 == 0 && i % 5 == 0) {
            skip_count = skip_count + 1;
            i = i + 1;
            continue;
        }
        if (i % 7 == 0) {
            skip_count = skip_count + 1;
            i = i + 1;
            continue;
        }
        sum = sum + i;
        process_count = process_count + 1;
        last_processed = i;
        if (prev_processed >= 0) {
            int gap = i - prev_processed;
            if (gap > max_gap) max_gap = gap;
        }
        prev_processed = i;
        i = i + 1;
    }
    return sum + skip_count + process_count + last_processed + max_gap;
}

int convergence_loop(int x) {
    int iter = 0;
    int val = x;
    int prev = 0;
    int diff = x;
    int total = 0;
    while (diff > 0 && iter < 100) {
        prev = val;
        if (val % 2 == 0) val = val / 2;
        else val = val + 1;
        diff = prev - val;
        if (diff < 0) diff = -diff;
        total = total + val;
        iter = iter + 1;
    }
    return total + iter;
}

int interleaved_loops(int n) {
    int arr[50];
    int i = 0;
    while (i < n && i < 50) {
        arr[i] = i * i;
        i = i + 1;
    }

    int forward_sum = 0;
    int backward_sum = 0;
    int lo = 0;
    int hi = n - 1;
    int steps = 0;
    while (lo < hi) {
        forward_sum = forward_sum + arr[lo];
        backward_sum = backward_sum + arr[hi];
        lo = lo + 1;
        hi = hi - 1;
        steps = steps + 1;
    }
    return forward_sum + backward_sum + steps;
}

int main() {
    int r1 = multi_var_loop1();
    printf("multi var 1: %d\n", r1);
    // EXPECT: multi var 1: 3043

    int r2 = multi_var_loop2(50);
    printf("multi var 2: %d\n", r2);
    // EXPECT: multi var 2: 2525

    int r3 = nested_multi_var(5, 5);
    printf("nested 5x5: %d\n", r3);
    // EXPECT: nested 5x5: 405

    int r4 = nested_multi_var(10, 10);
    printf("nested 10x10: %d\n", r4);
    // EXPECT: nested 10x10: 4235

    int r5 = do_while_complex(100);
    printf("do while 100: %d\n", r5);
    // EXPECT: do while 100: 782

    int r6 = do_while_complex(50);
    printf("do while 50: %d\n", r6);
    // EXPECT: do while 50: 1011

    int r7 = for_with_continue(100);
    printf("continue 100: %d\n", r7);
    // EXPECT: continue 100: 4102

    int r8 = for_with_continue(50);
    printf("continue 50: %d\n", r8);
    // EXPECT: continue 50: 1040

    int r9 = convergence_loop(1000);
    printf("converge 1000: %d\n", r9);
    // EXPECT: converge 1000: 1423

    int r10 = convergence_loop(64);
    printf("converge 64: %d\n", r10);
    // EXPECT: converge 64: 304

    int r11 = interleaved_loops(20);
    printf("interleave 20: %d\n", r11);
    // EXPECT: interleave 20: 2480

    int r12 = interleaved_loops(10);
    printf("interleave 10: %d\n", r12);
    // EXPECT: interleave 10: 290

    return 0;
}
