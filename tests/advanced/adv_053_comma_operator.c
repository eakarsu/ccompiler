// EXPECT: === Comma Operator ===
// EXPECT: basic_comma: result=30
// EXPECT: comma_in_for: sum=45
// EXPECT: dual_var_for: product=3628800
// EXPECT: comma_eval_order: a=2 b=3 result=3
// EXPECT: comma_in_assign: x=10 y=20
// EXPECT: comma_nested: 6
// EXPECT: comma_discard: val=42
// EXPECT: comma_in_condition: entered loop, i=5
// EXPECT: comma_array_fill: 0 2 4 6 8
// EXPECT: comma_with_func: side=99 result=50
// EXPECT: comma_chain_long: 5
// EXPECT: comma_in_return: 100
// EXPECT: comma_ptr: 30
// EXPECT: comma_in_while: count=5
// EXPECT: comma_struct: x=10 y=20
// EXPECT: complex_for: 0+9 1+8 2+7 3+6 4+5
// EXPECT: comma_precedence: a=5 b=3
// EXPECT: comma_incr: i=3 j=3

int printf(const char *fmt, ...);

int side_effect_val;

int set_side(int v) {
    side_effect_val = v;
    return v;
}

int comma_return(int a, int b) {
    // Comma operator in return: evaluates a+b, discards, then returns a*b
    return (a + b, a * b);
}

int compute_and_return(int x) {
    int temp;
    return (temp = x * 2, temp + x);
}

struct Point {
    int x;
    int y;
};

int main(void) {
    printf("=== Comma Operator ===\n");

    // Basic comma: evaluates left, discards, returns right
    {
        int result = (10, 20, 30);
        printf("basic_comma: result=%d\n", result);
    }

    // Comma in for loop: two increments
    {
        int sum = 0;
        int i;
        for (i = 0; i < 10; i++) {
            sum += i;
        }
        printf("comma_in_for: sum=%d\n", sum);
    }

    // Dual variable for loop with comma
    {
        int i;
        int j;
        int product = 1;
        for (i = 1, j = 10; i <= 10; i++, j--) {
            product = product * i;
            // j goes from 10 down to 1 but we just use product of i
        }
        printf("dual_var_for: product=%d\n", product);
    }

    // Evaluation order: left to right
    {
        int a = 1;
        int b = 2;
        int result = (a = a + 1, b = b + 1, b);
        printf("comma_eval_order: a=%d b=%d result=%d\n", a, b, result);
    }

    // Comma with assignment
    {
        int x;
        int y;
        x = (y = 20, 10);  // y=20, then x=10
        printf("comma_in_assign: x=%d y=%d\n", x, y);
    }

    // Nested comma
    {
        int val = (1, (2, (3, (4, (5, 6)))));
        printf("comma_nested: %d\n", val);
    }

    // Comma discards side effects but keeps last value
    {
        int dummy = 0;
        int val = (dummy = 99, 42);
        printf("comma_discard: val=%d\n", val);
    }

    // Comma in condition (the condition is the second operand)
    {
        int i = 0;
        int entered = 0;
        i = 5;
        if ((entered = 1, i > 3)) {
            printf("comma_in_condition: entered loop, i=%d\n", i);
        }
    }

    // Comma to fill array in for
    {
        int arr[5];
        int i;
        for (i = 0; i < 5; i++) {
            arr[i] = (0, i * 2);
        }
        printf("comma_array_fill: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Comma with function calls
    {
        int result = (set_side(99), 50);
        printf("comma_with_func: side=%d result=%d\n", side_effect_val, result);
    }

    // Long comma chain
    {
        int a;
        int val = (a = 1, a = a + 1, a = a + 1, a = a + 1, a = a + 1, a);
        printf("comma_chain_long: %d\n", val);
    }

    // Comma in return statement
    {
        int r = comma_return(10, 10);
        printf("comma_in_return: %d\n", r);
    }

    // Comma with pointers
    {
        int a = 10;
        int b = 20;
        int c = 30;
        int *p;
        int val = (p = &a, p = &b, p = &c, *p);
        printf("comma_ptr: %d\n", val);
    }

    // Comma in while condition
    {
        int count = 0;
        int i = 0;
        while ((i++, count++, i <= 5)) {
            // loop body empty
        }
        // i went 1..6, count went 1..6, but loop entered when i<=5
        // Actually: iteration 1: i becomes 1, count becomes 1, 1<=5 true
        // iteration 2: i becomes 2, count becomes 2, 2<=5 true
        // iteration 3: i becomes 3, count becomes 3, 3<=5 true
        // iteration 4: i becomes 4, count becomes 4, 4<=5 true
        // iteration 5: i becomes 5, count becomes 5, 5<=5 true
        // iteration 6: i becomes 6, count becomes 6, 6<=5 false -> exit
        // count = 6, but we want to report the number of times the body ran
        printf("comma_in_while: count=%d\n", count - 1);
    }

    // Comma to initialize struct fields
    {
        struct Point pt;
        (pt.x = 10, pt.y = 20);
        printf("comma_struct: x=%d y=%d\n", pt.x, pt.y);
    }

    // Complex for loop with two counters going in opposite directions
    {
        int i;
        int j;
        printf("complex_for:");
        for (i = 0, j = 9; i < 5; i++, j--) {
            printf(" %d+%d", i, j);
        }
        printf("\n");
    }

    // Comma and assignment precedence
    {
        int a;
        int b;
        a = 5;
        b = (1, 2, 3);
        printf("comma_precedence: a=%d b=%d\n", a, b);
    }

    // Comma with pre-increment
    {
        int i = 0;
        int j = 0;
        int r = (i++, i++, i++, j = i, j);
        printf("comma_incr: i=%d j=%d\n", i, j);
    }

    return 0;
}
