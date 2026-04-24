int printf(const char *fmt, ...);

/* test_015_goto_labels.c
   Tests: forward goto, backward goto, goto out of loops,
   goto into different blocks, multiple labels, goto in nested if.
*/

/* Forward goto: skip a block */
int forward_skip(int flag) {
    int x = 1;
    if (flag) goto skip;
    x = 2;
skip:
    return x;
}
/* forward_skip(1) = 1 (skipped x=2) */
/* forward_skip(0) = 2 */

/* Backward goto: simple loop without for/while */
int backward_loop(int n) {
    int sum = 0, i = 0;
loop_top:
    if (i >= n) goto loop_done;
    sum += i;
    i++;
    goto loop_top;
loop_done:
    return sum;
}
/* backward_loop(5) = 0+1+2+3+4 = 10 */

/* Goto out of nested loop */
int goto_out_nested(int limit) {
    int found = -1;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i * j > limit) {
                found = i * j;
                goto done;
            }
        }
    }
done:
    return found;
}
/* goto_out_nested(20): i=3,j=7: 21>20 => found=21 */

/* Multiple labels in sequence */
int multi_label(int n) {
    int x = 0;
    if (n == 1) goto label1;
    if (n == 2) goto label2;
    if (n == 3) goto label3;
    goto label_end;
label1:
    x += 100;
    goto label_end;
label2:
    x += 200;
    goto label_end;
label3:
    x += 300;
label_end:
    return x;
}

/* Goto in nested if: jump to common exit path */
int nested_if_goto(int a, int b, int c) {
    int result = 0;
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                result = 1;
                goto done;
            }
            result = 2;
            goto done;
        }
        result = 3;
        goto done;
    }
    result = 4;
done:
    return result;
}

/* Goto used to retry a block */
int retry_block(int *counter, int max_tries) {
    int attempts = 0;
retry:
    attempts++;
    if (attempts < max_tries) goto retry;
    *counter = attempts;
    return attempts;
}
/* retry_block(&c, 5) => returns 5 */

/* Forward goto over variable initialization */
int goto_over_init(int skip) {
    int x = 10;
    if (skip) goto after;
    x = x * 2;
after:
    return x;
}
/* goto_over_init(1) = 10 */
/* goto_over_init(0) = 20 */

/* Goto exit pattern: early exit from deep computation */
int deep_search(int target) {
    int result = -1;
    for (int a = 1; a <= 5; a++) {
        for (int b = 1; b <= 5; b++) {
            for (int c = 1; c <= 5; c++) {
                if (a + b + c == target && a < b && b < c) {
                    result = a * 100 + b * 10 + c;
                    goto found;
                }
            }
        }
    }
found:
    return result;
}
/* deep_search(6): smallest a+b+c=6, a<b<c: a=1,b=2,c=3 => 123 */
/* deep_search(9): a=1,b=3,c=5: 1+3+5=9, 1<3<5 => check: a=1,b=2,c=6>5 no; a=1,b=3,c=5 yes => 135 */
/* deep_search(100): -1 (no solution) */

int main(void) {
    printf("%d\n", forward_skip(1)); // EXPECT: 1
    printf("%d\n", forward_skip(0)); // EXPECT: 2

    printf("%d\n", backward_loop(5));  // EXPECT: 10
    printf("%d\n", backward_loop(0));  // EXPECT: 0
    printf("%d\n", backward_loop(10)); // EXPECT: 45

    printf("%d\n", goto_out_nested(20)); // EXPECT: 21
    printf("%d\n", goto_out_nested(80)); // EXPECT: 81

    printf("%d\n", multi_label(1)); // EXPECT: 100
    printf("%d\n", multi_label(2)); // EXPECT: 200
    printf("%d\n", multi_label(3)); // EXPECT: 300
    printf("%d\n", multi_label(4)); // EXPECT: 0

    printf("%d\n", nested_if_goto(1, 1, 1));  // EXPECT: 1
    printf("%d\n", nested_if_goto(1, 1, -1)); // EXPECT: 2
    printf("%d\n", nested_if_goto(1, -1, 0)); // EXPECT: 3
    printf("%d\n", nested_if_goto(-1, 0, 0)); // EXPECT: 4

    int c = 0;
    printf("%d\n", retry_block(&c, 5)); // EXPECT: 5
    printf("%d\n", c);                  // EXPECT: 5

    printf("%d\n", goto_over_init(1)); // EXPECT: 10
    printf("%d\n", goto_over_init(0)); // EXPECT: 20

    printf("%d\n", deep_search(6));   // EXPECT: 123
    printf("%d\n", deep_search(9));   // EXPECT: 135
    printf("%d\n", deep_search(100)); // EXPECT: -1

    return 0;
}
