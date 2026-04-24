
// EXPECT: === Pre/Post Increment/Decrement ===
// EXPECT: pre_incr: 6 6
// EXPECT: post_incr: 5 6
// EXPECT: pre_decr: 4 4
// EXPECT: post_decr: 5 4
// EXPECT: in_expr: a=11 b=21
// EXPECT: post_in_add: result=15 x=11 y=6
// EXPECT: pre_in_add: result=17 x=11 y=6
// EXPECT: array_post_idx: 10 20 30
// EXPECT: array_pre_idx: 20 30
// EXPECT: ptr_incr: 10 20 30
// EXPECT: ptr_decr: 30 20 10
// EXPECT: loop_post: 0 1 2 3 4
// EXPECT: loop_pre: 0 1 2 3 4
// EXPECT: countdown: 5 4 3 2 1
// EXPECT: nested_loop_incr: count=9
// EXPECT: incr_in_func_arg: printed=5 after=6
// EXPECT: incr_in_condition: ran 5 times
// EXPECT: decr_to_zero: 10 9 8 7 6 5 4 3 2 1
// EXPECT: incr_char: A B C D E
// EXPECT: mixed_incr_decr: a=6 b=4
// EXPECT: post_incr_assign: old=5 new=6
// EXPECT: pre_incr_assign: val=6 x=6
// EXPECT: incr_struct_member: x=11 y=19
int printf(const char *fmt, ...);

struct Counter {
    int x;
    int y;
};

int identity(int x) {
    return x;
}

int main(void) {
    printf("=== Pre/Post Increment/Decrement ===\n");

    // Pre-increment: increment then use
    {
        int a = 5;
        int b = ++a;
        printf("pre_incr: %d %d\n", a, b);
    }

    // Post-increment: use then increment
    {
        int a = 5;
        int b = a++;
        printf("post_incr: %d %d\n", b, a);
    }

    // Pre-decrement
    {
        int a = 5;
        int b = --a;
        printf("pre_decr: %d %d\n", a, b);
    }

    // Post-decrement
    {
        int a = 5;
        int b = a--;
        printf("post_decr: %d %d\n", b, a);
    }

    // In expressions
    {
        int a = 10;
        int b = 20;
        a++;
        b++;
        printf("in_expr: a=%d b=%d\n", a, b);
    }

    // Post-increment in addition
    {
        int x = 10;
        int y = 5;
        int result = x++ + y++;
        printf("post_in_add: result=%d x=%d y=%d\n", result, x, y);
    }

    // Pre-increment in addition
    {
        int x = 10;
        int y = 5;
        int result = ++x + ++y;
        printf("pre_in_add: result=%d x=%d y=%d\n", result, x, y);
    }

    // Post-increment as array index
    {
        int arr[5];
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;
        arr[4] = 50;
        int i = 0;
        int a = arr[i++];
        int b = arr[i++];
        int c = arr[i++];
        printf("array_post_idx: %d %d %d\n", a, b, c);
    }

    // Pre-increment as array index
    {
        int arr[5];
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;
        arr[4] = 50;
        int i = 0;
        int a = arr[++i];  // i=1, arr[1]=20
        int b = arr[++i];  // i=2, arr[2]=30
        printf("array_pre_idx: %d %d\n", a, b);
    }

    // Pointer increment
    {
        int arr[3];
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        int *p = arr;
        printf("ptr_incr: %d %d %d\n", *p, *(p + 1), *(p + 2));
    }

    // Pointer decrement
    {
        int arr[3];
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        int *p = &arr[2];
        int c = *p--;
        int b = *p--;
        int a = *p;
        printf("ptr_decr: %d %d %d\n", c, b, a);
    }

    // Post-increment in for loop
    {
        int i;
        printf("loop_post:");
        for (i = 0; i < 5; i++) {
            printf(" %d", i);
        }
        printf("\n");
    }

    // Pre-increment in for loop (same behavior due to position)
    {
        int i;
        printf("loop_pre:");
        for (i = 0; i < 5; ++i) {
            printf(" %d", i);
        }
        printf("\n");
    }

    // Countdown with post-decrement
    {
        int i = 5;
        printf("countdown:");
        while (i > 0) {
            printf(" %d", i);
            i--;
        }
        printf("\n");
    }

    // Nested loop with increment
    {
        int count = 0;
        int i;
        int j;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                count++;
            }
        }
        printf("nested_loop_incr: count=%d\n", count);
    }

    // Increment in function argument
    {
        int x = 5;
        int printed = x;
        printf("incr_in_func_arg: printed=%d", printed);
        x++;
        printf(" after=%d\n", x);
    }

    // Increment in loop condition
    {
        int i = 0;
        int count = 0;
        while (i++ < 5) {
            count++;
        }
        printf("incr_in_condition: ran %d times\n", count);
    }

    // Decrement to zero
    {
        int n = 10;
        printf("decr_to_zero:");
        while (n > 0) {
            printf(" %d", n);
            n--;
        }
        printf("\n");
    }

    // Increment on char
    {
        char c = 'A';
        printf("incr_char:");
        int i;
        for (i = 0; i < 5; i++) {
            printf(" %c", c);
            c++;
        }
        printf("\n");
    }

    // Mixed increment and decrement
    {
        int a = 5;
        int b = 5;
        a++;
        b--;
        printf("mixed_incr_decr: a=%d b=%d\n", a, b);
    }

    // Post-increment captures old value
    {
        int x = 5;
        int old = x++;
        printf("post_incr_assign: old=%d new=%d\n", old, x);
    }

    // Pre-increment returns new value
    {
        int x = 5;
        int val = ++x;
        printf("pre_incr_assign: val=%d x=%d\n", val, x);
    }

    // Struct member increment
    {
        struct Counter cnt;
        cnt.x = 10;
        cnt.y = 20;
        cnt.x++;
        cnt.y--;
        printf("incr_struct_member: x=%d y=%d\n", cnt.x, cnt.y);
    }

    return 0;
}
