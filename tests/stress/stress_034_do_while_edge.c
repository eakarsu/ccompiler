// stress_034_do_while_edge.c - do-while edge cases
// EXPECT: wrapper_val=42
// EXPECT: continue_sum=25
// EXPECT: break_at=3
// EXPECT: goto_escaped=1
// EXPECT: nested_outer=3
// EXPECT: nested_inner_total=6
// EXPECT: side_effect_count=5
// EXPECT: early_return=99
// EXPECT: single_iter=1
// EXPECT: zero_body=executed
// EXPECT: do_while_chain=15
// EXPECT: modify_in_cond=10
// EXPECT: complex_cond=45
// EXPECT: decrement_cond=0
// EXPECT: continue_skip=2 4 6 8 10
// EXPECT: triple_nest=8
// EXPECT: do_for_mix=35
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// do-while(0) as statement wrapper macro simulation
int wrapper_test(void) {
    int result = 0;
    do {
        result = 40;
        result = result + 2;
        if (result == 42) break;
        result = -1; // should not reach
    } while (0);
    return result;
}

// do-while with continue
int continue_test(void) {
    int sum = 0;
    int i = 0;
    do {
        i++;
        if (i % 2 == 0) continue; // skip even
        sum = sum + i;
    } while (i < 10);
    // sum of odd 1..9 = 1+3+5+7+9 = 25
    return sum;
}

// do-while with break
int break_test(void) {
    int i = 0;
    do {
        i++;
        if (i == 3) break;
    } while (i < 100);
    return i;
}

// do-while with goto
int goto_test(void) {
    int escaped = 0;
    do {
        goto out;
        escaped = -1;
    } while (1);
out:
    escaped = 1;
    return escaped;
}

// Nested do-while
void nested_test(int *outer_count, int *inner_total) {
    int oc = 0;
    int it = 0;
    int i = 0;
    do {
        i++;
        oc++;
        int j = 0;
        do {
            j++;
            it++;
        } while (j < 2);
    } while (i < 3);
    *outer_count = oc;
    *inner_total = it;
}

// Side effects in condition
int side_effect_cond(void) {
    int count = 0;
    int x = 0;
    do {
        count++;
    } while (++x < 5);
    return count;
}

// Early return from do-while
int early_return_test(int flag) {
    do {
        if (flag) return 99;
        return -1;
    } while (0);
    return -2; // unreachable
}

// Single iteration do-while(0)
int single_iter_test(void) {
    int x = 0;
    do { x++; } while (0);
    return x;
}

// do-while with empty-ish body (just a side effect)
void zero_body_test(void) {
    int done = 0;
    do {
        done = 1;
    } while (0);
    if (done) printf("zero_body=executed\n");
}

// Chained do-while
int chain_test(void) {
    int sum = 0;
    int i = 0;
    do { sum = sum + (++i); } while (i < 5);
    // 1+2+3+4+5=15
    return sum;
}

// Modify variable in condition
int modify_in_cond_test(void) {
    int x = 0;
    do {
        x++;
    } while (x < 10);
    return x;
}

// Complex condition with multiple sub-expressions
int complex_cond_test(void) {
    int sum = 0;
    int i = 0;
    do {
        i++;
        sum = sum + i;
    } while (i < 9 && sum < 100);
    // sum = 1+2+3+4+5+6+7+8+9 = 45
    return sum;
}

// Decrement in condition until zero
int decrement_cond_test(void) {
    int n = 5;
    do {
        n--;
    } while (n > 0);
    return n;
}

// Continue skipping odd, printing even
void continue_skip_test(void) {
    int i = 0;
    int first = 1;
    printf("continue_skip=");
    do {
        i++;
        if (i % 2 != 0) continue;
        if (!first) printf(" ");
        printf("%d", i);
        first = 0;
    } while (i < 10);
    printf("\n");
}

// Triple nested do-while
int triple_nest(void) {
    int count = 0;
    int a = 0;
    do {
        a++;
        int b = 0;
        do {
            b++;
            int c = 0;
            do {
                c++;
                count++;
            } while (c < 2);
        } while (b < 2);
    } while (a < 2);
    return count; // 2*2*2=8
}

// do-while mixed with for loop
int do_for_mix(void) {
    int total = 0;
    int round = 0;
    do {
        round++;
        int i;
        for (i = 1; i <= round; i++) {
            total = total + i;
        }
    } while (round < 5);
    // round1: 1, round2: 1+2=3, round3: 1+2+3=6, round4: 1+2+3+4=10, round5: 1+2+3+4+5=15
    // total = 1+3+6+10+15 = 35... wait let me recalculate
    // Actually total accumulates: +1=1, +1+2=4, +1+2+3=10, +1+2+3+4=20, +1+2+3+4+5=35
    // Hmm, let me recount: 1, 1+3=4, 4+6=10, 10+10=20, 20+15=35
    // Wait: round1 adds sum(1..1)=1, round2 adds sum(1..2)=3, round3 adds sum(1..3)=6,
    // round4 adds sum(1..4)=10, round5 adds sum(1..5)=15
    // 1+3+6+10+15=35... but I wrote 55 in EXPECT. Let me fix.
    // Actually sum(1..n) = n*(n+1)/2. Total = sum of sum(1..k) for k=1..5
    // = 1+3+6+10+15 = 35. Let me adjust EXPECT.
    return total;
}

int main(void) {
    printf("wrapper_val=%d\n", wrapper_test());
    printf("continue_sum=%d\n", continue_test());
    printf("break_at=%d\n", break_test());
    printf("goto_escaped=%d\n", goto_test());

    int oc, it;
    nested_test(&oc, &it);
    printf("nested_outer=%d\n", oc);
    printf("nested_inner_total=%d\n", it);

    printf("side_effect_count=%d\n", side_effect_cond());
    printf("early_return=%d\n", early_return_test(1));
    printf("single_iter=%d\n", single_iter_test());
    zero_body_test();
    printf("do_while_chain=%d\n", chain_test());
    printf("modify_in_cond=%d\n", modify_in_cond_test());
    printf("complex_cond=%d\n", complex_cond_test());
    printf("decrement_cond=%d\n", decrement_cond_test());
    continue_skip_test();
    printf("triple_nest=%d\n", triple_nest());
    printf("do_for_mix=%d\n", do_for_mix());

    printf("pass_done=1\n");
    return 0;
}
