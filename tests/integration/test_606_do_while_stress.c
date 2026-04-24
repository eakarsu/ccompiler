int printf(const char *fmt, ...);
// EXPECT: T1:1\nT2:42\nT3:00 01 02 10 11 12\nT4:3\nT5:25\nT6:36\nT7:20\nT8:3\nT9:1\nT10:10\nT11:1 2\nT12:5 5\nT13:8\nT14:3 7\nT15:8\nT16:89 144 11\nT17:6\nT18:3\nT19:5\nT20:8\nT21:9\nT22:-1\nT23:1 1\nT24:1024

// Helper function for Test 22
int is_positive(int x) {
    return x > 0;
}

int main() {
    int i, j, k, n, t;

    // Test 1: Basic do-while that runs exactly once (condition false from start)
    {
        int x = 0;
        do {
            x++;
        } while (0);
        printf("T1:%d\n", x);
    }

    // Test 2: do-while(0) idiom (macro wrapper pattern)
    {
        int val = 0;
        do {
            val = 42;
        } while (0);
        printf("T2:%d\n", val);
    }

    // Test 3: Nested do-while loops (2 levels, outer 2x, inner 3x)
    {
        int first = 1;
        printf("T3:");
        i = 0;
        do {
            j = 0;
            do {
                if (!first) printf(" ");
                printf("%d%d", i, j);
                first = 0;
                j++;
            } while (j < 3);
            i++;
        } while (i < 2);
        printf("\n");
    }

    // Test 4: do-while with break statement (stop at i==3)
    {
        i = 0;
        do {
            if (i == 3) break;
            i++;
        } while (i < 10);
        printf("T4:%d\n", i);
    }

    // Test 5: do-while with continue (sum odd values 1..10)
    {
        i = 0;
        int sum = 0;
        do {
            i++;
            if (i % 2 == 0) continue;
            sum += i;
        } while (i < 10);
        // odd values: 1,3,5,7,9 = 25
        printf("T5:%d\n", sum);
    }

    // Test 6: do-while inside a for loop (sum of i*j products)
    {
        int sum = 0;
        for (i = 1; i <= 3; i++) {
            j = 1;
            do {
                sum += i * j;
                j++;
            } while (j <= 3);
        }
        // i=1: 1+2+3=6, i=2: 2+4+6=12, i=3: 3+6+9=18 -> 36
        printf("T6:%d\n", sum);
    }

    // Test 7: for loop inside a do-while (triangular sums)
    {
        int sum = 0;
        i = 1;
        do {
            for (j = 1; j <= i; j++) {
                sum += j;
            }
            i++;
        } while (i <= 4);
        // i=1:1, i=2:1+2=3, i=3:1+2+3=6, i=4:1+2+3+4=10 -> 20
        printf("T7:%d\n", sum);
    }

    // Test 8: do-while with complex condition (&& short-circuit)
    {
        i = 0;
        do {
            i++;
        } while (i < 10 && i % 3 != 0);
        // i=1: T&&T, i=2: T&&T, i=3: T&&F -> stop
        printf("T8:%d\n", i);
    }

    // Test 9: do-while where body modifies condition variable (divide by 3)
    {
        int x = 100;
        do {
            x = x / 3;
        } while (x > 2);
        // 100->33->11->3->1
        printf("T9:%d\n", x);
    }

    // Test 10: do-while inside switch inside for
    {
        int result = 0;
        for (i = 0; i < 3; i++) {
            switch (i) {
                case 0:
                    n = 1;
                    do {
                        result += 1;
                        n++;
                    } while (n <= 3);
                    // adds 1 three times = 3
                    break;
                case 1:
                    n = 1;
                    do {
                        result += 2;
                        n++;
                    } while (n <= 2);
                    // adds 2 twice = 4
                    break;
                case 2:
                    n = 1;
                    do {
                        result += 3;
                        n++;
                    } while (n <= 1);
                    // adds 3 once = 3
                    break;
            }
        }
        // 3 + 4 + 3 = 10
        printf("T10:%d\n", result);
    }

    // Test 11: do-while with multiple break points (first at i==2)
    {
        int flag = 0;
        i = 0;
        do {
            i++;
            if (i == 2) {
                flag = 1;
                break;
            }
            if (i == 5) {
                flag = 2;
                break;
            }
        } while (i < 10);
        printf("T11:%d %d\n", flag, i);
    }

    // Test 12: Counter patterns - counting up and counting down
    {
        int up = 0;
        do {
            up++;
        } while (up < 5);

        int down = 10;
        do {
            down--;
        } while (down > 5);

        printf("T12:%d %d\n", up, down);
    }

    // Test 13: 3-level nested do-while (2*2*2 = 8 iterations)
    {
        int count = 0;
        i = 0;
        do {
            j = 0;
            do {
                k = 0;
                do {
                    count++;
                    k++;
                } while (k < 2);
                j++;
            } while (j < 2);
            i++;
        } while (i < 2);
        printf("T13:%d\n", count);
    }

    // Test 14: do-while with || condition (loop while a<3 OR b>8)
    {
        int a = 0;
        int b = 10;
        do {
            a++;
            b--;
        } while (a < 3 || b > 8);
        // a=1,b=9: T||T->T. a=2,b=8: T||F->T. a=3,b=7: F||F->F
        printf("T14:%d %d\n", a, b);
    }

    // Test 15: do-while with array traversal (find max)
    {
        int arr[5];
        arr[0] = 5;
        arr[1] = 3;
        arr[2] = 8;
        arr[3] = 1;
        arr[4] = 4;
        int max = arr[0];
        i = 1;
        do {
            if (arr[i] > max) max = arr[i];
            i++;
        } while (i < 5);
        printf("T15:%d\n", max);
    }

    // Test 16: do-while computing Fibonacci until b >= 100
    {
        int a = 0;
        int b = 1;
        n = 0;
        do {
            t = a + b;
            a = b;
            b = t;
            n++;
        } while (b < 100);
        // Sequence: (0,1)->(1,1)->(1,2)->(2,3)->(3,5)->(5,8)->
        //   (8,13)->(13,21)->(21,34)->(34,55)->(55,89)->(89,144)
        // n=11, a=89, b=144
        printf("T16:%d %d %d\n", a, b, n);
    }

    // Test 17: do-while computing GCD of 48 and 18
    {
        int a = 48;
        int b = 18;
        do {
            t = a % b;
            a = b;
            b = t;
        } while (b != 0);
        // 48%18=12, 18%12=6, 12%6=0 -> GCD=6
        printf("T17:%d\n", a);
    }

    // Test 18: do-while state machine (3-state cycle, count steps)
    {
        int state = 0;
        int steps = 0;
        do {
            steps++;
            if (state == 0) state = 1;
            else if (state == 1) state = 2;
            else if (state == 2) state = 0;
        } while (state != 0);
        // step1: 0->1, step2: 1->2, step3: 2->0 -> 3 steps
        printf("T18:%d\n", steps);
    }

    // Test 19: do-while with continue counting even iterations
    {
        i = 0;
        int evens = 0;
        do {
            i++;
            if (i % 2 != 0) continue;
            evens++;
        } while (i < 10);
        // even values of i: 2,4,6,8,10 -> 5
        printf("T19:%d\n", evens);
    }

    // Test 20: do-while counting set bits (popcount of 255)
    {
        int val = 255;
        int count = 0;
        do {
            if (val & 1) count++;
            val = val >> 1;
        } while (val > 0);
        // 255 = 0xFF = 8 bits set
        printf("T20:%d\n", count);
    }

    // Test 21: Nested do-while with break in inner only (j==2)
    {
        int sum = 0;
        i = 0;
        do {
            j = 0;
            do {
                if (j == 2) break;
                sum += i + j;
                j++;
            } while (j < 5);
            i++;
        } while (i < 3);
        // i=0: (0+0)+(0+1)=1. i=1: (1+0)+(1+1)=3. i=2: (2+0)+(2+1)=5
        // sum = 1+3+5 = 9
        printf("T21:%d\n", sum);
    }

    // Test 22: do-while with function call in condition
    {
        int x = 5;
        do {
            x -= 2;
        } while (is_positive(x));
        // x=5->3: pos(3)=T. x=3->1: pos(1)=T. x=1->-1: pos(-1)=F
        printf("T22:%d\n", x);
    }

    // Test 23: Nested do-while with flag-based outer break (labeled break sim)
    {
        int done = 0;
        i = 0;
        do {
            j = 0;
            do {
                if (i == 1 && j == 1) {
                    done = 1;
                    break;
                }
                j++;
            } while (j < 3 && !done);
            if (done) break;
            i++;
        } while (i < 3);
        printf("T23:%d %d\n", i, j);
    }

    // Test 24: do-while computing 2^10 via repeated multiply
    {
        int base = 2;
        int exp = 10;
        int result = 1;
        do {
            result *= base;
            exp--;
        } while (exp > 0);
        printf("T24:%d\n", result);
    }

    return 0;
}
