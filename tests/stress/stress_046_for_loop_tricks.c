// stress_046_for_loop_tricks.c - For loop edge cases torture test
// EXPECT: empty_for=10
// EXPECT: decl_init=55
// EXPECT: comma_all=45
// EXPECT: ptr_iter=165
// EXPECT: countdown=55
// EXPECT: odd_step=25
// EXPECT: skip_even=25
// EXPECT: break_cond=6
// EXPECT: linked_sum=150
// EXPECT: nested_break=12
// EXPECT: multi_var=100
// EXPECT: no_body=0
// EXPECT: post_inc=45

int printf(const char *fmt, ...);

struct Node {
    int val;
    struct Node *next;
};

int main(void) {
    int i, j;

    // --- Empty for(;;) with break ---
    {
        int count = 0;
        for (;;) {
            if (count >= 10) break;
            count = count + 1;
        }
        printf("empty_for=%d\n", count);
    }

    // --- For with declaration in init (C99) ---
    {
        int sum = 0;
        for (int x = 1; x <= 10; x++) {
            sum = sum + x;
        }
        printf("decl_init=%d\n", sum);
    }

    // --- For with comma operator in all 3 parts ---
    {
        int sum = 0;
        for (i = 0, j = 9; i < 10; i++, j--) {
            sum = sum + i;
        }
        printf("comma_all=%d\n", sum);
    }

    // --- For with pointer iterator ---
    {
        int arr[10];
        for (i = 0; i < 10; i++) arr[i] = (i + 1) * 3;
        int sum = 0;
        int *p;
        for (p = arr; p < arr + 10; p++) {
            sum = sum + *p;
        }
        printf("ptr_iter=%d\n", sum);
    }

    // --- For counting down ---
    {
        int sum = 0;
        for (i = 10; i >= 1; i--) {
            sum = sum + i;
        }
        printf("countdown=%d\n", sum);
    }

    // --- For with odd step (i += 2) ---
    {
        int sum = 0;
        for (i = 1; i < 10; i = i + 2) {
            sum = sum + i;
        }
        printf("odd_step=%d\n", sum);
    }

    // --- Skip even numbers with continue ---
    {
        int sum = 0;
        for (i = 1; i <= 10; i++) {
            if (i % 2 == 0) continue;
            sum = sum + i;
        }
        printf("skip_even=%d\n", sum);
    }

    // --- Break on condition ---
    {
        int sum = 0;
        for (i = 1; i <= 100; i++) {
            if (i > 3) break;
            sum = sum + i;
        }
        printf("break_cond=%d\n", sum);
    }

    // --- Iterate linked list ---
    {
        struct Node n5; n5.val = 50; n5.next = 0;
        struct Node n4; n4.val = 40; n4.next = &n5;
        struct Node n3; n3.val = 30; n3.next = &n4;
        struct Node n2; n2.val = 20; n2.next = &n3;
        struct Node n1; n1.val = 10; n1.next = &n2;
        struct Node *cur;
        int sum = 0;
        for (cur = &n1; cur != 0; cur = cur->next) {
            sum = sum + cur->val;
        }
        printf("linked_sum=%d\n", sum);
    }

    // --- Nested for with break from inner only ---
    {
        int sum = 0;
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 5; j++) {
                if (j >= 2) break;
                sum = sum + 1;
            }
        }
        printf("nested_break=%d\n", sum);
    }

    // --- Multiple variables converging ---
    {
        int sum = 0;
        for (i = 0, j = 100; i < j; i++, j--) {
            sum = sum + 2;
        }
        printf("multi_var=%d\n", sum);
    }

    // --- For with empty body (just increment) ---
    {
        for (i = 0; i < 100; i++) {
        }
        printf("no_body=%d\n", i - 100);
    }

    // --- Post-increment as iterator ---
    {
        int arr[10];
        for (i = 0; i < 10; i++) arr[i] = i;
        int sum = 0;
        i = 0;
        for (; i < 10; ) {
            sum = sum + arr[i++];
        }
        printf("post_inc=%d\n", sum);
    }

    return 0;
}
