int printf(const char *fmt, ...);

int find_first_pair(int target) {
    int i;
    int j;
    int found_i = -1;
    int found_j = -1;
    for (i = 0; i < 100; i = i + 1) {
        for (j = i + 1; j < 100; j = j + 1) {
            if (i + j == target && (i * j) % 7 == 0) {
                found_i = i;
                found_j = j;
                goto done_pair;
            }
        }
    }
    done_pair:
    return found_i * 1000 + found_j;
}

void break_from_while_in_for() {
    int i;
    int total = 0;
    for (i = 0; i < 10; i = i + 1) {
        int j = 0;
        while (j < 10) {
            if (j > i) {
                break;
            }
            total = total + 1;
            j = j + 1;
        }
    }
    printf("while-in-for total: %d\n", total);
}

void break_from_switch_in_loop() {
    int i;
    int count = 0;
    for (i = 0; i < 20; i = i + 1) {
        int rem = i % 5;
        switch (rem) {
            case 0:
                count = count + 10;
                break;
            case 1:
                count = count + 1;
                break;
            case 2:
                break;
            case 3:
                count = count + 3;
                break;
            default:
                count = count - 1;
                break;
        }
        if (count > 50) {
            break;
        }
    }
    printf("switch-in-loop count: %d\n", count);
    printf("stopped at i: %d\n", i);
}

int search_3d() {
    int a;
    int b;
    int c;
    int found = 0;
    int fa = -1;
    int fb = -1;
    int fc = -1;
    for (a = 0; a < 10 && !found; a = a + 1) {
        for (b = 0; b < 10 && !found; b = b + 1) {
            for (c = 0; c < 10; c = c + 1) {
                if (a * a + b * b + c * c == 50) {
                    fa = a;
                    fb = b;
                    fc = c;
                    found = 1;
                    break;
                }
            }
        }
    }
    return fa * 100 + fb * 10 + fc;
}

void nested_do_while_break() {
    int outer = 0;
    int total = 0;
    do {
        int inner = 0;
        do {
            total = total + 1;
            if (total > 15) {
                break;
            }
            inner = inner + 1;
        } while (inner < 5);
        outer = outer + 1;
        if (outer >= 10) {
            break;
        }
    } while (1);
    printf("do-while break total: %d\n", total);
    printf("do-while outer: %d\n", outer);
}

int main() {
    int pair = find_first_pair(10);
    // EXPECT: pair: 10
    printf("pair: %d\n", pair);

    // EXPECT: while-in-for total: 55
    break_from_while_in_for();

    // EXPECT: switch-in-loop count: 53
    // EXPECT: stopped at i: 18
    break_from_switch_in_loop();

    int loc = search_3d();
    // EXPECT: 3d search: 17
    printf("3d search: %d\n", loc);

    // EXPECT: do-while break total: 22
    // EXPECT: do-while outer: 10
    nested_do_while_break();

    int arr[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = (i * 7 + 3) % 13;
    }
    int max_val = arr[0];
    int max_idx = 0;
    for (i = 1; i < 10; i = i + 1) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_idx = i;
        }
    }
    // EXPECT: max val: 12 at idx: 5
    printf("max val: %d at idx: %d\n", max_val, max_idx);

    return 0;
}
