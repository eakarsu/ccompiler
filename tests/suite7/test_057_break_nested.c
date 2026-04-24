int printf(const char *fmt, ...);

int inner_break_only(void) {
    int sum = 0;
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            if (j == 3) break;
            sum = sum + 1;
        }
    }
    return sum;
}

int flag_outer_break(int target) {
    int found_i = -1;
    int found_j = -1;
    int done = 0;
    int i;
    int j;
    for (i = 0; i < 5 && done == 0; i = i + 1) {
        for (j = 0; j < 5 && done == 0; j = j + 1) {
            if (i * 5 + j == target) {
                found_i = i;
                found_j = j;
                done = 1;
            }
        }
    }
    return found_i * 10 + found_j;
}

int break_while_in_for(void) {
    int total = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int j = 0;
        while (j < 100) {
            if (j >= i + 1) break;
            total = total + 1;
            j = j + 1;
        }
    }
    return total;
}

int break_for_in_while(void) {
    int total = 0;
    int outer = 0;
    while (outer < 3) {
        int k;
        for (k = 0; k < 10; k = k + 1) {
            if (k == outer + 2) break;
            total = total + 1;
        }
        outer = outer + 1;
    }
    return total;
}

int triple_nested_break(void) {
    int count = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            for (k = 0; k < 5; k = k + 1) {
                if (k == 2) break;
                count = count + 1;
            }
        }
    }
    return count;
}

int goto_outer_break(void) {
    int sum = 0;
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            if (i + j == 7) goto outer_done;
            sum = sum + 1;
        }
    }
outer_done:
    return sum;
}

int break_in_switch_in_loop(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        switch (i % 3) {
            case 0:
                sum = sum + 1;
                break;
            case 1:
                sum = sum + 10;
                break;
            case 2:
                sum = sum + 100;
                break;
        }
    }
    return sum;
}

int do_while_break(void) {
    int count = 0;
    do {
        count = count + 1;
        if (count == 5) break;
    } while (count < 100);
    return count;
}

int break_preserves_outer(void) {
    int outer_count = 0;
    int inner_total = 0;
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        outer_count = outer_count + 1;
        for (j = 0; j < 10; j = j + 1) {
            if (j == 2) break;
            inner_total = inner_total + 1;
        }
    }
    return outer_count * 100 + inner_total;
}

int break_with_side_effects(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = 0;
    }
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = 1;
        if (i == 4) break;
    }
    int sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + arr[i];
    }
    return sum;
}

int main(void) {
    // EXPECT: inner_break: 15
    printf("inner_break: %d\n", inner_break_only());
    // EXPECT: flag 0: 0
    printf("flag 0: %d\n", flag_outer_break(0));
    // EXPECT: flag 12: 22
    printf("flag 12: %d\n", flag_outer_break(12));
    // EXPECT: flag 24: 44
    printf("flag 24: %d\n", flag_outer_break(24));
    // EXPECT: while_in_for: 10
    printf("while_in_for: %d\n", break_while_in_for());
    // EXPECT: for_in_while: 9
    printf("for_in_while: %d\n", break_for_in_while());
    // EXPECT: triple: 50
    printf("triple: %d\n", triple_nested_break());
    // EXPECT: goto_outer: 7
    printf("goto_outer: %d\n", goto_outer_break());
    // EXPECT: sw_in_loop: 334
    printf("sw_in_loop: %d\n", break_in_switch_in_loop());
    // EXPECT: do_break: 5
    printf("do_break: %d\n", do_while_break());
    // EXPECT: preserves: 1020
    printf("preserves: %d\n", break_preserves_outer());
    // EXPECT: sideeff: 5
    printf("sideeff: %d\n", break_with_side_effects());
    // EXPECT: flag 7: 12
    printf("flag 7: %d\n", flag_outer_break(7));
    // EXPECT: flag 3: 3
    printf("flag 3: %d\n", flag_outer_break(3));
    // EXPECT: flag 20: 40
    printf("flag 20: %d\n", flag_outer_break(20));

    return 0;
}
