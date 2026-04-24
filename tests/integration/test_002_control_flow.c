int printf(const char *fmt, ...);
// EXPECT: if_pos=yes
// if_neg=no
// if_zero=zero
// if_big=big
// if_medium=medium
// nested_even=4
// nested_inner=inner
// while_sum=55
// for_sum=55
// dowhile_sum=55
// countdown=10 9 8 7 6 5 4 3 2 1
// break_at=5
// continue_odd_sum=25
// nested_pairs=5
// fizz1=1
// fizz2=2
// fizz3=F
// fizz4=4
// fizz5=B
// fizz6=F
// fizz7=7
// fizz8=8
// fizz9=F
// fizz10=B
// fizz11=11
// fizz12=F
// fizz13=13
// fizz14=14
// fizz15=FB
// prime2=1
// prime3=1
// prime4=0
// prime5=1
// prime6=0
// prime7=1
// prime8=0
// prime9=0
// prime10=0
// prime11=1
// sw0=zero
// sw1=one
// sw2=two
// sw5=other
// deep_found=1
// mul=1 2 3 4 5
// mul=2 4 6 8 10
// mul=3 6 9 12 15
// mul=4 8 12 16 20
// mul=5 10 15 20 25
// early_ret=6
// tri=equilateral
// tri=isoceles
// tri=scalene
// max3=9
// min3=1
// collatz6=8
// digits=4
// pow2_10=1024

// ---- Helper functions ----

int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int max2(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min2(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int max3(int a, int b, int c) {
    int m;
    m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

int min3(int a, int b, int c) {
    int m;
    m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int is_prime(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int classify_triangle(int a, int b, int c) {
    // 0 = invalid, 1 = equilateral, 2 = isoceles, 3 = scalene
    if (a + b <= c) return 0;
    if (a + c <= b) return 0;
    if (b + c <= a) return 0;
    if (a == b && b == c) return 1;
    if (a == b || b == c || a == c) return 2;
    return 3;
}

int early_return_test(int n) {
    int i;
    i = 0;
    while (i < n) {
        if (i * i > 30) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int collatz_count(int n) {
    int steps;
    steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
    }
    return steps;
}

int count_digits(int n) {
    int c;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    c = 0;
    while (n > 0) {
        c = c + 1;
        n = n / 10;
    }
    return c;
}

int power_of_2(int exp) {
    int result;
    int i;
    result = 1;
    i = 0;
    while (i < exp) {
        result = result * 2;
        i = i + 1;
    }
    return result;
}

int sum_while(int n) {
    int s;
    int i;
    s = 0;
    i = 1;
    while (i <= n) {
        s = s + i;
        i = i + 1;
    }
    return s;
}

int sum_for_loop(int n) {
    int s;
    int i;
    s = 0;
    for (i = 1; i <= n; i = i + 1) {
        s = s + i;
    }
    return s;
}

int sum_dowhile(int n) {
    int s;
    int i;
    s = 0;
    i = 1;
    do {
        s = s + i;
        i = i + 1;
    } while (i <= n);
    return s;
}

int find_break_point(int limit) {
    int i;
    i = 0;
    while (i < limit) {
        if (i * i >= 25) {
            break;
        }
        i = i + 1;
    }
    return i;
}

int sum_odd_continue(int n) {
    int s;
    int i;
    s = 0;
    for (i = 1; i <= n; i = i + 1) {
        if (i % 2 == 0) {
            continue;
        }
        s = s + i;
    }
    return s;
}

int deep_search(int target) {
    int i;
    int j;
    int k;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            for (k = 0; k < 10; k = k + 1) {
                if (i * 100 + j * 10 + k == target) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int grade(int score) {
    if (score >= 90) return 4;
    if (score >= 80) return 3;
    if (score >= 70) return 2;
    if (score >= 60) return 1;
    return 0;
}

int sum_even_range(int n) {
    int s;
    int i;
    s = 0;
    i = 2;
    while (i <= n) {
        s = s + i;
        i = i + 2;
    }
    return s;
}

int count_down_steps(int start) {
    int count;
    int i;
    count = 0;
    i = start;
    while (i > 0) {
        count = count + 1;
        i = i - 1;
    }
    return count;
}

int nested_break_count(int n) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (j >= 3) break;
            count = count + 1;
        }
    }
    return count;
}

int do_while_countdown(int start) {
    int val;
    val = start;
    do {
        val = val - 1;
    } while (val > 0);
    return val;
}

int complex_condition(int a, int b, int c) {
    if (a > 0 && b > 0 && c > 0) {
        if (a + b > c && a + c > b && b + c > a) {
            return 1;
        }
    }
    return 0;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int binary_search_iterative(int target, int low, int high) {
    int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (mid == target) return mid;
        if (mid < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    int i;
    int j;
    int tri;

    // Basic if/else
    if (5 > 3) {
        printf("if_pos=yes\n");
    } else {
        printf("if_pos=no\n");
    }

    if (2 > 7) {
        printf("if_neg=yes\n");
    } else {
        printf("if_neg=no\n");
    }

    // if/else chain
    i = 0;
    if (i > 0) {
        printf("if_zero=pos\n");
    } else if (i < 0) {
        printf("if_zero=neg\n");
    } else {
        printf("if_zero=zero\n");
    }

    i = 75;
    if (i >= 90) {
        printf("if_big=huge\n");
    } else if (i >= 50) {
        printf("if_big=big\n");
    } else if (i >= 20) {
        printf("if_big=medium\n");
    } else {
        printf("if_big=small\n");
    }

    i = 30;
    if (i >= 90) {
        printf("if_medium=huge\n");
    } else if (i >= 50) {
        printf("if_medium=big\n");
    } else if (i >= 20) {
        printf("if_medium=medium\n");
    } else {
        printf("if_medium=small\n");
    }

    // Nested ifs
    i = 4;
    if (i > 0) {
        if (i < 10) {
            if (i % 2 == 0) {
                printf("nested_even=%d\n", i);
            }
        }
    }

    i = 3;
    j = 5;
    if (i < j) {
        if (i + j > 6) {
            printf("nested_inner=inner\n");
        } else {
            printf("nested_inner=outer\n");
        }
    }

    // While loop sum 1..10
    printf("while_sum=%d\n", sum_while(10));

    // For loop sum 1..10
    printf("for_sum=%d\n", sum_for_loop(10));

    // Do-while sum 1..10
    printf("dowhile_sum=%d\n", sum_dowhile(10));

    // Countdown
    printf("countdown=");
    for (i = 10; i >= 1; i = i - 1) {
        printf("%d", i);
        if (i > 1) printf(" ");
    }
    printf("\n");

    // Break test: first i where i*i >= 25 => i=5
    printf("break_at=%d\n", find_break_point(100));

    // Continue test: sum of odd numbers from 1 to 9 = 1+3+5+7+9=25
    printf("continue_odd_sum=%d\n", sum_odd_continue(9));

    // Nested loop: count pairs (i,j) with 1<=i<=5, 1<=j<=5, i+j==6
    // (1,5),(2,4),(3,3),(4,2),(5,1) = 5
    {
        int cnt;
        cnt = 0;
        for (i = 1; i <= 5; i = i + 1) {
            for (j = 1; j <= 5; j = j + 1) {
                if (i + j == 6) {
                    cnt = cnt + 1;
                }
            }
        }
        printf("nested_pairs=%d\n", cnt);
    }

    // FizzBuzz 1-15
    for (i = 1; i <= 15; i = i + 1) {
        printf("fizz%d=", i);
        if (i % 15 == 0) {
            printf("FB\n");
        } else if (i % 3 == 0) {
            printf("F\n");
        } else if (i % 5 == 0) {
            printf("B\n");
        } else {
            printf("%d\n", i);
        }
    }

    // Primality tests 2..11
    for (i = 2; i <= 11; i = i + 1) {
        printf("prime%d=%d\n", i, is_prime(i));
    }

    // Switch tests
    printf("sw0=");
    switch (0) {
        case 0: printf("zero\n"); break;
        case 1: printf("one\n"); break;
        default: printf("other\n"); break;
    }
    printf("sw1=");
    switch (1) {
        case 0: printf("zero\n"); break;
        case 1: printf("one\n"); break;
        default: printf("other\n"); break;
    }
    printf("sw2=");
    switch (2) {
        case 0: printf("zero\n"); break;
        case 1: printf("one\n"); break;
        case 2: printf("two\n"); break;
        default: printf("other\n"); break;
    }
    printf("sw5=");
    switch (5) {
        case 0: printf("zero\n"); break;
        case 1: printf("one\n"); break;
        case 2: printf("two\n"); break;
        default: printf("other\n"); break;
    }

    // Deep nested search for 345
    printf("deep_found=%d\n", deep_search(345));

    // Multiplication table 5x5
    for (i = 1; i <= 5; i = i + 1) {
        printf("mul=");
        for (j = 1; j <= 5; j = j + 1) {
            printf("%d", i * j);
            if (j < 5) printf(" ");
        }
        printf("\n");
    }

    // Early return: first i where i*i > 30 => i=6 (6*6=36>30)
    printf("early_ret=%d\n", early_return_test(100));

    // Triangle classification
    tri = classify_triangle(5, 5, 5);
    if (tri == 1) printf("tri=equilateral\n");
    else if (tri == 2) printf("tri=isoceles\n");
    else if (tri == 3) printf("tri=scalene\n");
    else printf("tri=invalid\n");

    tri = classify_triangle(5, 5, 3);
    if (tri == 1) printf("tri=equilateral\n");
    else if (tri == 2) printf("tri=isoceles\n");
    else if (tri == 3) printf("tri=scalene\n");
    else printf("tri=invalid\n");

    tri = classify_triangle(3, 4, 5);
    if (tri == 1) printf("tri=equilateral\n");
    else if (tri == 2) printf("tri=isoceles\n");
    else if (tri == 3) printf("tri=scalene\n");
    else printf("tri=invalid\n");

    // Max/Min of 3
    printf("max3=%d\n", max3(3, 9, 5));
    printf("min3=%d\n", min3(3, 9, 1));

    // Collatz(6): 6->3->10->5->16->8->4->2->1 = 8 steps
    printf("collatz6=%d\n", collatz_count(6));

    // Digit count of 1234
    printf("digits=%d\n", count_digits(1234));

    // 2^10
    printf("pow2_10=%d\n", power_of_2(10));

    return 0;
}
