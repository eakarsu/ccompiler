int printf(const char *fmt, ...);
// EXPECT: === If/Else Chain Tests ===
// EXPECT: basic_if: five is positive
// EXPECT: basic_else: neg is not positive
// EXPECT: if_else: 10 is even
// EXPECT: if_else2: 7 is odd
// EXPECT: elif1: grade A
// EXPECT: elif2: grade B
// EXPECT: elif3: grade C
// EXPECT: elif4: grade D
// EXPECT: elif5: grade F
// EXPECT: nested1: x>0 and y>0
// EXPECT: nested2: x>0 and y<=0
// EXPECT: nested3: x<=0 and y>0
// EXPECT: nested4: x<=0 and y<=0
// EXPECT: deep1: all positive: 1 2 3 4 5
// EXPECT: deep2: found non-positive at level 3
// EXPECT: side1: a=10, b=20
// EXPECT: side2: a=10, b=5
// EXPECT: complex_cond1: in range [10,20]
// EXPECT: complex_cond2: out of range
// EXPECT: multi_and: all conditions met
// EXPECT: multi_or: at least one condition met
// EXPECT: classify: -5 is negative
// EXPECT: classify: 0 is zero
// EXPECT: classify: 7 is positive
// EXPECT: abs_val: abs(-42)=42
// EXPECT: abs_val: abs(42)=42
// EXPECT: abs_val: abs(0)=0
// EXPECT: max3: max(3,7,5)=7
// EXPECT: max3: max(9,2,4)=9
// EXPECT: max3: max(1,6,8)=8
// EXPECT: clamp: clamp(-5,0,10)=0
// EXPECT: clamp: clamp(5,0,10)=5
// EXPECT: clamp: clamp(15,0,10)=10
// EXPECT: fizzbuzz: 1
// EXPECT: fizzbuzz: 2
// EXPECT: fizzbuzz: fizz
// EXPECT: fizzbuzz: 4
// EXPECT: fizzbuzz: buzz
// EXPECT: fizzbuzz: fizz
// EXPECT: fizzbuzz: 7
// EXPECT: fizzbuzz: 8
// EXPECT: fizzbuzz: fizz
// EXPECT: fizzbuzz: buzz
// EXPECT: fizzbuzz: 11
// EXPECT: fizzbuzz: fizz
// EXPECT: fizzbuzz: 13
// EXPECT: fizzbuzz: 14
// EXPECT: fizzbuzz: fizzbuzz
// EXPECT: deep_nest: result=42
// basic_if: five is positive
// basic_else: neg is not positive
// if_else: 10 is even
// if_else2: 7 is odd
// elif1: grade A
// elif2: grade B
// elif3: grade C
// elif4: grade D
// elif5: grade F
// nested1: x>0 and y>0
// nested2: x>0 and y<=0
// nested3: x<=0 and y>0
// nested4: x<=0 and y<=0
// deep1: all positive: 1 2 3 4 5
// deep2: found non-positive at level 3
// side1: a=10, b=20
// side2: a=10, b=5
// complex_cond1: in range [10,20]
// complex_cond2: out of range
// multi_and: all conditions met
// multi_or: at least one condition met
// classify: -5 is negative
// classify: 0 is zero
// classify: 7 is positive
// abs_val: abs(-42)=42
// abs_val: abs(42)=42
// abs_val: abs(0)=0
// max3: max(3,7,5)=7
// max3: max(9,2,4)=9
// max3: max(1,6,8)=8
// clamp: clamp(-5,0,10)=0
// clamp: clamp(5,0,10)=5
// clamp: clamp(15,0,10)=10
// fizzbuzz: 1
// fizzbuzz: 2
// fizzbuzz: fizz
// fizzbuzz: 4
// fizzbuzz: buzz
// fizzbuzz: fizz
// fizzbuzz: 7
// fizzbuzz: 8
// fizzbuzz: fizz
// fizzbuzz: buzz
// fizzbuzz: 11
// fizzbuzz: fizz
// fizzbuzz: 13
// fizzbuzz: 14
// fizzbuzz: fizzbuzz
// deep_nest: result=42

int main(void) {
    int a, b, x, y, r;
    int i;

    printf("=== If/Else Chain Tests ===\n");

    // Basic if
    a = 5;
    if (a > 0) {
        printf("basic_if: five is positive\n");
    }

    // Basic if/else
    a = -3;
    if (a > 0) {
        printf("basic_else: neg is positive\n");
    } else {
        printf("basic_else: neg is not positive\n");
    }

    // If/else for even/odd
    a = 10;
    if (a % 2 == 0) {
        printf("if_else: 10 is even\n");
    } else {
        printf("if_else: 10 is odd\n");
    }
    a = 7;
    if (a % 2 == 0) {
        printf("if_else2: 7 is even\n");
    } else {
        printf("if_else2: 7 is odd\n");
    }

    // Else-if ladder: grade classification
    int scores[5];
    scores[0] = 95;
    scores[1] = 85;
    scores[2] = 75;
    scores[3] = 65;
    scores[4] = 45;
    for (i = 0; i < 5; i++) {
        int s = scores[i];
        if (s >= 90) {
            printf("elif%d: grade A\n", i + 1);
        } else if (s >= 80) {
            printf("elif%d: grade B\n", i + 1);
        } else if (s >= 70) {
            printf("elif%d: grade C\n", i + 1);
        } else if (s >= 60) {
            printf("elif%d: grade D\n", i + 1);
        } else {
            printf("elif%d: grade F\n", i + 1);
        }
    }

    // Nested if/else (2 levels)
    int xs[2];
    int ys[2];
    xs[0] = 5; ys[0] = 3;
    xs[1] = 5; ys[1] = -1;

    x = xs[0]; y = ys[0];
    if (x > 0) {
        if (y > 0) {
            printf("nested1: x>0 and y>0\n");
        } else {
            printf("nested1: x>0 and y<=0\n");
        }
    } else {
        if (y > 0) {
            printf("nested1: x<=0 and y>0\n");
        } else {
            printf("nested1: x<=0 and y<=0\n");
        }
    }

    x = xs[1]; y = ys[1];
    if (x > 0) {
        if (y > 0) {
            printf("nested2: x>0 and y>0\n");
        } else {
            printf("nested2: x>0 and y<=0\n");
        }
    } else {
        if (y > 0) {
            printf("nested2: x<=0 and y>0\n");
        } else {
            printf("nested2: x<=0 and y<=0\n");
        }
    }

    x = -2; y = 4;
    if (x > 0) {
        if (y > 0) {
            printf("nested3: x>0 and y>0\n");
        } else {
            printf("nested3: x>0 and y<=0\n");
        }
    } else {
        if (y > 0) {
            printf("nested3: x<=0 and y>0\n");
        } else {
            printf("nested3: x<=0 and y<=0\n");
        }
    }

    x = -2; y = -4;
    if (x > 0) {
        if (y > 0) {
            printf("nested4: x>0 and y>0\n");
        } else {
            printf("nested4: x>0 and y<=0\n");
        }
    } else {
        if (y > 0) {
            printf("nested4: x<=0 and y>0\n");
        } else {
            printf("nested4: x<=0 and y<=0\n");
        }
    }

    // Deeply nested (5 levels)
    int vals[5];
    vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4; vals[4] = 5;
    if (vals[0] > 0) {
        if (vals[1] > 0) {
            if (vals[2] > 0) {
                if (vals[3] > 0) {
                    if (vals[4] > 0) {
                        printf("deep1: all positive: %d %d %d %d %d\n",
                               vals[0], vals[1], vals[2], vals[3], vals[4]);
                    }
                }
            }
        }
    }

    vals[2] = -1;
    if (vals[0] > 0) {
        if (vals[1] > 0) {
            if (vals[2] > 0) {
                if (vals[3] > 0) {
                    if (vals[4] > 0) {
                        printf("deep2: all positive\n");
                    }
                }
            } else {
                printf("deep2: found non-positive at level 3\n");
            }
        }
    }

    // Conditions with side effects (assignment in condition)
    a = 0; b = 0;
    if ((a = 10) > 5) {
        b = 20;
    }
    printf("side1: a=%d, b=%d\n", a, b);

    a = 0; b = 0;
    if ((a = 10) > 15) {
        b = 20;
    } else {
        b = 5;
    }
    printf("side2: a=%d, b=%d\n", a, b);

    // Complex conditions
    a = 15;
    if (a >= 10 && a <= 20) {
        printf("complex_cond1: in range [10,20]\n");
    }
    a = 25;
    if (a >= 10 && a <= 20) {
        printf("complex_cond2: in range [10,20]\n");
    } else {
        printf("complex_cond2: out of range\n");
    }

    // Multiple AND conditions
    a = 5; b = 10;
    if (a > 0 && b > 0 && a < b && b < 100) {
        printf("multi_and: all conditions met\n");
    }

    // Multiple OR conditions
    a = -5;
    if (a > 100 || a < 0 || a == 42) {
        printf("multi_or: at least one condition met\n");
    }

    // Classify numbers
    int nums[3];
    nums[0] = -5; nums[1] = 0; nums[2] = 7;
    for (i = 0; i < 3; i++) {
        if (nums[i] < 0) {
            printf("classify: %d is negative\n", nums[i]);
        } else if (nums[i] == 0) {
            printf("classify: %d is zero\n", nums[i]);
        } else {
            printf("classify: %d is positive\n", nums[i]);
        }
    }

    // Absolute value via if
    int abs_inputs[3];
    abs_inputs[0] = -42; abs_inputs[1] = 42; abs_inputs[2] = 0;
    for (i = 0; i < 3; i++) {
        a = abs_inputs[i];
        if (a < 0) a = -a;
        printf("abs_val: abs(%d)=%d\n", abs_inputs[i], a);
    }

    // Max of 3 numbers
    int m3a[3];
    int m3b[3];
    int m3c[3];
    m3a[0] = 3; m3b[0] = 7; m3c[0] = 5;
    m3a[1] = 9; m3b[1] = 2; m3c[1] = 4;
    m3a[2] = 1; m3b[2] = 6; m3c[2] = 8;
    for (i = 0; i < 3; i++) {
        r = m3a[i];
        if (m3b[i] > r) r = m3b[i];
        if (m3c[i] > r) r = m3c[i];
        printf("max3: max(%d,%d,%d)=%d\n", m3a[i], m3b[i], m3c[i], r);
    }

    // Clamp function via if/else
    int cvals[3];
    cvals[0] = -5; cvals[1] = 5; cvals[2] = 15;
    for (i = 0; i < 3; i++) {
        a = cvals[i];
        if (a < 0) a = 0;
        else if (a > 10) a = 10;
        printf("clamp: clamp(%d,0,10)=%d\n", cvals[i], a);
    }

    // FizzBuzz 1-15
    for (i = 1; i <= 15; i++) {
        if (i % 15 == 0) {
            printf("fizzbuzz: fizzbuzz\n");
        } else if (i % 3 == 0) {
            printf("fizzbuzz: fizz\n");
        } else if (i % 5 == 0) {
            printf("fizzbuzz: buzz\n");
        } else {
            printf("fizzbuzz: %d\n", i);
        }
    }

    // Deeply nested with computed result (6 levels)
    // r accumulates: 0+1+2+4+8+16=31, then +11=42
    a = 10;
    r = 0;
    if (a > 5) {
        r = r + 1;
        if (a > 6) {
            r = r + 2;
            if (a > 7) {
                r = r + 4;
                if (a > 8) {
                    r = r + 8;
                    if (a > 9) {
                        r = r + 16;
                        if (a >= 10) {
                            r = r + 11;
                        }
                    }
                }
            }
        }
    }
    printf("deep_nest: result=%d\n", r);

    return 0;
}
