int printf(const char *fmt, ...);

int goto_loop(int n) {
    int sum = 0;
    int i = 0;
loop:
    if (i >= n) goto done;
    sum = sum + i;
    i = i + 1;
    goto loop;
done:
    return sum;
}

int retry_pattern(int attempts) {
    int tries = 0;
retry:
    tries = tries + 1;
    if (tries < attempts) {
        goto retry;
    }
    return tries;
}

int cleanup_chain(int step) {
    int resource1 = 0;
    int resource2 = 0;
    int resource3 = 0;

    resource1 = 10;
    if (step == 1) goto cleanup3;

    resource2 = 20;
    if (step == 2) goto cleanup2;

    resource3 = 30;
    if (step == 3) goto cleanup1;

    return resource1 + resource2 + resource3;

cleanup1:
    resource3 = 0;
cleanup2:
    resource2 = 0;
cleanup3:
    resource1 = 0;
    return resource1 + resource2 + resource3;
}

int goto_countdown(int n) {
    int result = 1;
again:
    if (n <= 1) goto finish;
    result = result * n;
    n = n - 1;
    goto again;
finish:
    return result;
}

int goto_gcd(int a, int b) {
start:
    if (b == 0) return a;
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    goto start;
}

int goto_sum_evens(int n) {
    int sum = 0;
    int i = 0;
next:
    if (i >= n) goto end;
    if (i % 2 != 0) {
        i = i + 1;
        goto next;
    }
    sum = sum + i;
    i = i + 1;
    goto next;
end:
    return sum;
}

int goto_nested_cleanup(int a, int b) {
    int x = 0;
    int y = 0;

    x = a * 10;
    if (x < 0) goto fail;

    y = b * 10;
    if (y < 0) goto partial;

    return x + y;

partial:
    return x;
fail:
    return -1;
}

int goto_state_machine(int input) {
    int result = 0;
    int idx = 0;
    int data[4];
    data[0] = input;
    data[1] = input + 1;
    data[2] = input + 2;
    data[3] = -1;

state_a:
    if (data[idx] < 0) goto state_end;
    if (data[idx] % 2 == 0) {
        result = result + data[idx];
        idx = idx + 1;
        goto state_b;
    }
    result = result + 1;
    idx = idx + 1;
    goto state_a;

state_b:
    if (data[idx] < 0) goto state_end;
    result = result + data[idx] * 10;
    idx = idx + 1;
    goto state_a;

state_end:
    return result;
}

int goto_fibonacci(int n) {
    int a = 0;
    int b = 1;
    int temp;
    int i = 0;
loop:
    if (i >= n) goto done;
    temp = a + b;
    a = b;
    b = temp;
    i = i + 1;
    goto loop;
done:
    return a;
}

int main(void) {
    // EXPECT: gotoloop 5: 10
    printf("gotoloop 5: %d\n", goto_loop(5));
    // EXPECT: gotoloop 0: 0
    printf("gotoloop 0: %d\n", goto_loop(0));
    // EXPECT: retry 5: 5
    printf("retry 5: %d\n", retry_pattern(5));
    // EXPECT: retry 1: 1
    printf("retry 1: %d\n", retry_pattern(1));
    // EXPECT: cleanup 0: 60
    printf("cleanup 0: %d\n", cleanup_chain(0));
    // EXPECT: cleanup 1: 0
    printf("cleanup 1: %d\n", cleanup_chain(1));
    // EXPECT: cleanup 2: 0
    printf("cleanup 2: %d\n", cleanup_chain(2));
    // EXPECT: cleanup 3: 0
    printf("cleanup 3: %d\n", cleanup_chain(3));
    // EXPECT: countdown 5: 120
    printf("countdown 5: %d\n", goto_countdown(5));
    // EXPECT: gcd 12,8: 4
    printf("gcd 12,8: %d\n", goto_gcd(12, 8));
    // EXPECT: gcd 15,5: 5
    printf("gcd 15,5: %d\n", goto_gcd(15, 5));
    // EXPECT: evens 10: 20
    printf("evens 10: %d\n", goto_sum_evens(10));
    // EXPECT: nclean 5,3: 80
    printf("nclean 5,3: %d\n", goto_nested_cleanup(5, 3));
    // EXPECT: nclean -1,3: -1
    printf("nclean -1,3: %d\n", goto_nested_cleanup(-1, 3));
    // EXPECT: nclean 5,-1: 50
    printf("nclean 5,-1: %d\n", goto_nested_cleanup(5, -1));
    // EXPECT: state 2: 36
    printf("state 2: %d\n", goto_state_machine(2));
    // EXPECT: fib 0: 0
    printf("fib 0: %d\n", goto_fibonacci(0));
    // EXPECT: fib 7: 13
    printf("fib 7: %d\n", goto_fibonacci(7));
    // EXPECT: fib 10: 55
    printf("fib 10: %d\n", goto_fibonacci(10));

    return 0;
}
