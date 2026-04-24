int printf(const char *fmt, ...);

int classify_and_count(int *arr, int len) {
    int total = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        int v = arr[i];
        if (v > 0) {
            switch (v % 3) {
            case 0: total = total + 3; break;
            case 1: total = total + 1; break;
            case 2: total = total + 2; break;
            }
        } else if (v < 0) {
            total = total - 1;
        }
    }
    return total;
}

int main(void) {
    // Test 1: for inside if inside for
    int sum = 0;
    int i;
    int j;
    for (i = 1; i <= 5; i = i + 1) {
        if (i % 2 == 1) {
            for (j = 0; j < i; j = j + 1) {
                sum = sum + 1;
            }
        }
    }
    // odd i: 1(1 iter) + 3(3 iter) + 5(5 iter) = 9
    // EXPECT: for_if_for: 9
    printf("for_if_for: %d\n", sum);

    // Test 2: while inside for inside if
    int prod = 1;
    for (i = 1; i <= 4; i = i + 1) {
        if (i <= 3) {
            int k = i;
            while (k > 0) {
                prod = prod + k;
                k = k - 1;
            }
        }
    }
    // i=1: k=1 -> prod=1+1=2
    // i=2: k=2 -> prod=2+2=4, k=1 -> prod=4+1=5
    // i=3: k=3 -> prod=5+3=8, k=2 -> prod=8+2=10, k=1 -> prod=10+1=11
    // i=4: skipped
    // EXPECT: while_in_for: 11
    printf("while_in_for: %d\n", prod);

    // Test 3: switch inside for with nested if
    int score = 0;
    for (i = 0; i < 10; i = i + 1) {
        switch (i % 4) {
        case 0:
            if (i < 5)
                score = score + 10;
            else
                score = score + 20;
            break;
        case 1:
            score = score + 1;
            break;
        case 2:
            score = score + 2;
            break;
        case 3:
            score = score + 3;
            break;
        }
    }
    // i=0: case0, i<5 -> +10 (score=10)
    // i=1: case1 -> +1 (11)
    // i=2: case2 -> +2 (13)
    // i=3: case3 -> +3 (16)
    // i=4: case0, i<5 -> +10 (26)
    // i=5: case1 -> +1 (27)
    // i=6: case2 -> +2 (29)
    // i=7: case3 -> +3 (32)
    // i=8: case0, i>=5 -> +20 (52)
    // i=9: case1 -> +1 (53)
    // EXPECT: switch_if_for: 53
    printf("switch_if_for: %d\n", score);

    // Test 4: deeply nested loops (4 levels)
    int deep = 0;
    int k;
    int l;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            for (k = 0; k < 2; k = k + 1) {
                for (l = 0; l < 2; l = l + 1) {
                    deep = deep + 1;
                }
            }
        }
    }
    // 2*3*2*2 = 24
    // EXPECT: deep_nest: 24
    printf("deep_nest: %d\n", deep);

    // Test 5: for inside do-while inside if
    int dw_total = 0;
    int pass = 0;
    do {
        if (pass < 3) {
            for (i = 0; i <= pass; i = i + 1) {
                dw_total = dw_total + 1;
            }
        }
        pass = pass + 1;
    } while (pass < 5);
    // pass=0: for 0..0 -> 1 (total=1)
    // pass=1: for 0..1 -> 2 (total=3)
    // pass=2: for 0..2 -> 3 (total=6)
    // pass=3: skip
    // pass=4: skip
    // EXPECT: for_in_dw: 6
    printf("for_in_dw: %d\n", dw_total);

    // Test 6: nested if-else chains inside loop
    int classify = 0;
    for (i = 0; i < 20; i = i + 1) {
        if (i < 5) {
            classify = classify + 1;
        } else if (i < 10) {
            if (i % 2 == 0) {
                classify = classify + 2;
            } else {
                classify = classify + 3;
            }
        } else if (i < 15) {
            classify = classify + 4;
        } else {
            if (i == 17) {
                classify = classify + 10;
            } else {
                classify = classify + 5;
            }
        }
    }
    // i=0..4: +1 each = 5
    // i=5: odd +3=8; i=6: even +2=10; i=7: odd +3=13; i=8: even +2=15; i=9: odd +3=18
    // i=10..14: +4 each = 18+20=38
    // i=15: +5=43; i=16: +5=48; i=17: +10=58; i=18: +5=63; i=19: +5=68
    // EXPECT: classify: 68
    printf("classify: %d\n", classify);

    // Test 7: loop with switch containing loops
    int sw_loop = 0;
    for (i = 0; i < 4; i = i + 1) {
        switch (i) {
        case 0:
            j = 0;
            while (j < 3) {
                sw_loop = sw_loop + 1;
                j = j + 1;
            }
            break;
        case 1:
            sw_loop = sw_loop + 10;
            break;
        case 2:
            for (j = 0; j < 2; j = j + 1) {
                sw_loop = sw_loop + 5;
            }
            break;
        case 3:
            sw_loop = sw_loop + 100;
            break;
        }
    }
    // i=0: while 3 times -> +3 (sw_loop=3)
    // i=1: +10 (13)
    // i=2: for 2 times -> +10 (23)
    // i=3: +100 (123)
    // EXPECT: sw_loop: 123
    printf("sw_loop: %d\n", sw_loop);

    // Test 8: break inside nested if inside loop
    int break_val = 0;
    for (i = 0; i < 100; i = i + 1) {
        if (i > 5) {
            if (i % 7 == 0) {
                break_val = i;
                break;
            }
        }
    }
    // First i>5 and i%7==0 is i=7
    // EXPECT: nested_break: 7
    printf("nested_break: %d\n", break_val);

    // Test 9: classify_and_count function
    int data[8];
    data[0] = 3; data[1] = 7; data[2] = -2; data[3] = 9;
    data[4] = 0; data[5] = 4; data[6] = -1; data[7] = 6;
    // 3: %3=0 -> +3
    // 7: %3=1 -> +1
    // -2: -> -1
    // 9: %3=0 -> +3
    // 0: neither
    // 4: %3=1 -> +1
    // -1: -> -1
    // 6: %3=0 -> +3
    // total = 3+1-1+3+0+1-1+3 = 9
    // EXPECT: classify_count: 9
    printf("classify_count: %d\n", classify_and_count(data, 8));

    // Test 10: while with inner for and switch
    int total = 0;
    int phase = 0;
    while (phase < 3) {
        for (i = 0; i < 4; i = i + 1) {
            switch (phase) {
            case 0: total = total + 1; break;
            case 1: total = total + 2; break;
            case 2: total = total + 3; break;
            }
        }
        phase = phase + 1;
    }
    // phase 0: 4*1=4, phase 1: 4*2=8, phase 2: 4*3=12 -> 24
    // EXPECT: phase_total: 24
    printf("phase_total: %d\n", total);

    // Test 11: triangular nested loops
    int tri = 0;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j <= i; j = j + 1) {
            tri = tri + 1;
        }
    }
    // 1+2+3+4+5 = 15
    // EXPECT: triangular: 15
    printf("triangular: %d\n", tri);

    // Test 12: conditional accumulation in nested loops
    int cond_sum = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (i + j < 5) {
                cond_sum = cond_sum + 1;
            }
        }
    }
    // i=0: j=0,1,2,3,4 -> 5
    // i=1: j=0,1,2,3 -> 4
    // i=2: j=0,1,2 -> 3
    // i=3: j=0,1 -> 2
    // i=4: j=0 -> 1
    // i=5: none -> 0
    // Total = 15
    // EXPECT: cond_nested: 15
    printf("cond_nested: %d\n", cond_sum);

    // Test 13: loop with multiple conditions and paths
    int multi = 0;
    for (i = 0; i < 30; i = i + 1) {
        if (i % 15 == 0) {
            multi = multi + 15;
        } else if (i % 3 == 0) {
            multi = multi + 3;
        } else if (i % 5 == 0) {
            multi = multi + 5;
        } else {
            multi = multi + 1;
        }
    }
    // i=0: +15(15), i=1:+1(16), i=2:+1(17), i=3:+3(20), i=4:+1(21),
    // i=5:+5(26), i=6:+3(29), i=7:+1(30), i=8:+1(31), i=9:+3(34),
    // i=10:+5(39), i=11:+1(40), i=12:+3(43), i=13:+1(44), i=14:+1(45),
    // i=15:+15(60), i=16:+1(61), i=17:+1(62), i=18:+3(65), i=19:+1(66),
    // i=20:+5(71), i=21:+3(74), i=22:+1(75), i=23:+1(76), i=24:+3(79),
    // i=25:+5(84), i=26:+1(85), i=27:+3(88), i=28:+1(89), i=29:+1(90)
    // EXPECT: fizzbuzz_sum: 90
    printf("fizzbuzz_sum: %d\n", multi);

    // Test 14: while inside if-else branches
    int branch_sum = 0;
    for (i = 0; i < 4; i = i + 1) {
        if (i < 2) {
            j = 0;
            while (j < 3) {
                branch_sum = branch_sum + 1;
                j = j + 1;
            }
        } else {
            j = 0;
            while (j < 2) {
                branch_sum = branch_sum + 10;
                j = j + 1;
            }
        }
    }
    // i=0: +3 (3); i=1: +3 (6); i=2: +20 (26); i=3: +20 (46)
    // EXPECT: branch_while: 46
    printf("branch_while: %d\n", branch_sum);

    // Test 15: do-while with nested for and conditionals
    int dw_complex = 0;
    int round = 0;
    do {
        for (i = 0; i < 5; i = i + 1) {
            if ((round + i) % 2 == 0) {
                dw_complex = dw_complex + 1;
            }
        }
        round = round + 1;
    } while (round < 4);
    // round=0: i=0(even+1),1(odd),2(even+1),3(odd),4(even+1) -> +3 (3)
    // round=1: i=0(odd),1(even+1),2(odd),3(even+1),4(odd) -> +2 (5)
    // round=2: i=0(even+1),1(odd),2(even+1),3(odd),4(even+1) -> +3 (8)
    // round=3: i=0(odd),1(even+1),2(odd),3(even+1),4(odd) -> +2 (10)
    // EXPECT: dw_complex: 10
    printf("dw_complex: %d\n", dw_complex);

    return 0;
}
