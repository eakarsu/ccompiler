int printf(const char *fmt, ...);
// EXPECT: 1 0 1 1 0 0 0 1 0 3 0 0 0 0 1 0 1 1 0 2 6 12 24 43 5 2 3 6 9 1 15 5 1 0 8 111 1 0 1 1 1 1 1 3 4321 1 42 14 3 2 15 55 35

// Number games: Nim game analysis, number guessing logic, mathematical games,
// Sprague-Grundy values

// Nim game: given piles, determine if first player wins
// In standard Nim, first player wins iff XOR of all piles != 0
int nim_winner(int piles[], int n) {
    int xor_val;
    int i;
    xor_val = 0;
    for (i = 0; i < n; i = i + 1) {
        xor_val = xor_val ^ piles[i];
    }
    if (xor_val != 0) return 1; // first player wins
    return 0; // second player wins
}

// Single pile Nim: take 1..k stones, who wins?
// First player wins if pile % (k+1) != 0
int nim_single(int pile, int k) {
    if (pile % (k + 1) != 0) return 1;
    return 0;
}

// Sprague-Grundy value for single pile game where moves are in set
int sg_value(int n, int moves[], int nmoves) {
    int grundy[100];
    int i;
    int j;
    int mex_set[100];
    int mex;
    grundy[0] = 0;
    for (i = 1; i <= n; i = i + 1) {
        for (j = 0; j < 100; j = j + 1) {
            mex_set[j] = 0;
        }
        for (j = 0; j < nmoves; j = j + 1) {
            if (moves[j] <= i) {
                mex_set[grundy[i - moves[j]]] = 1;
            }
        }
        mex = 0;
        while (mex_set[mex]) {
            mex = mex + 1;
        }
        grundy[i] = mex;
    }
    return grundy[n];
}

// Multi-pile game with Sprague-Grundy: XOR of SG values
int multi_pile_sg(int piles[], int npiles, int moves[], int nmoves) {
    int xor_val;
    int i;
    xor_val = 0;
    for (i = 0; i < npiles; i = i + 1) {
        xor_val = xor_val ^ sg_value(piles[i], moves, nmoves);
    }
    if (xor_val != 0) return 1;
    return 0;
}

// Wythoff's game: two piles, can take any from one pile, or equal from both
// Losing positions are (floor(k*phi), floor(k*phi^2)) for k=0,1,2...
// Approximate: cold positions where diff * 1618 / 1000 ~ smaller pile
int wythoff_winner(int a, int b) {
    int mn;
    int mx;
    int diff;
    int target;
    if (a > b) {
        mx = a; mn = b;
    } else {
        mx = b; mn = a;
    }
    diff = mx - mn;
    // golden ratio ~1.618: target = diff * 1618 / 1000
    target = diff * 1618 / 1000;
    if (target == mn) return 0; // P-position (second player wins)
    return 1; // N-position (first player wins)
}

// Subtraction game: remove 1..k from pile n
// Who wins? First player wins if n % (k+1) != 0
int subtraction_game(int n, int k) {
    if (n % (k + 1) != 0) return 1;
    return 0;
}

// Last player loses game (Misere Nim):
// First player wins if XOR!=0 and (not all piles==1), or XOR==0 and all piles==1
int misere_nim(int piles[], int n) {
    int xor_val;
    int all_one;
    int i;
    xor_val = 0;
    all_one = 1;
    for (i = 0; i < n; i = i + 1) {
        xor_val = xor_val ^ piles[i];
        if (piles[i] > 1) all_one = 0;
    }
    if (all_one) {
        // Misere with all piles = 1: first wins if odd number of piles (xor=1)
        // Wait: if all 1s, n piles. Last to take loses.
        // If n is even, first player wins (each takes 1 alternately, second takes last)
        // Actually: if n even, first player can't avoid second winning
        // n odd: first player takes, n-1 remain, second loses
        // Misere all-1: first wins if n is odd (xor=1)
        if (n % 2 == 1) return 1;
        return 0;
    }
    if (xor_val != 0) return 1;
    return 0;
}

// GCD game: given two numbers, repeatedly subtract smaller from larger
// Number of steps
int gcd_steps(int a, int b) {
    int steps;
    steps = 0;
    while (a != b) {
        if (a > b) {
            a = a - b;
        } else {
            b = b - a;
        }
        steps = steps + 1;
    }
    return steps;
}

// GCD value
int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// LCM
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

// Power mod
int power_mod(int base, int exp, int mod) {
    int result;
    result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

// Fibonacci game: take 1..F(k) stones from pile of F(n)
// Zeckendorf representation: express n as sum of non-consecutive Fibonacci numbers
// Count Fibonacci numbers needed
int zeckendorf_count(int n) {
    int fibs[30];
    int nf;
    int count;
    int i;
    fibs[0] = 1;
    fibs[1] = 2;
    nf = 2;
    while (fibs[nf - 1] <= n) {
        fibs[nf] = fibs[nf - 1] + fibs[nf - 2];
        nf = nf + 1;
    }
    count = 0;
    for (i = nf - 1; i >= 0; i = i - 1) {
        if (fibs[i] <= n) {
            n = n - fibs[i];
            count = count + 1;
        }
    }
    return count;
}

// Digital root: repeatedly sum digits until single digit
int digital_root(int n) {
    int sum;
    while (n >= 10) {
        sum = 0;
        while (n > 0) {
            sum = sum + n % 10;
            n = n / 10;
        }
        n = sum;
    }
    return n;
}

// Sum of digits
int digit_sum(int n) {
    int sum;
    sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

// Number of digits
int digit_count(int n) {
    int count;
    if (n == 0) return 1;
    count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 10;
    }
    return count;
}

// Collatz sequence length
int collatz_length(int n) {
    int count;
    count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        count = count + 1;
    }
    return count;
}

// Happy number check: repeatedly sum squares of digits, reaches 1?
int is_happy(int n) {
    int slow;
    int fast;
    int sum;
    int tmp;
    slow = n;
    fast = n;
    while (1) {
        // One step for slow
        sum = 0;
        tmp = slow;
        while (tmp > 0) {
            sum = sum + (tmp % 10) * (tmp % 10);
            tmp = tmp / 10;
        }
        slow = sum;
        // Two steps for fast
        sum = 0;
        tmp = fast;
        while (tmp > 0) {
            sum = sum + (tmp % 10) * (tmp % 10);
            tmp = tmp / 10;
        }
        fast = sum;
        sum = 0;
        tmp = fast;
        while (tmp > 0) {
            sum = sum + (tmp % 10) * (tmp % 10);
            tmp = tmp / 10;
        }
        fast = sum;
        if (fast == 1) return 1;
        if (slow == fast) return 0;
    }
    return 0;
}

// Perfect number check
int is_perfect(int n) {
    int sum;
    int i;
    if (n <= 1) return 0;
    sum = 1;
    for (i = 2; i * i <= n; i = i + 1) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    if (sum == n) return 1;
    return 0;
}

// Armstrong number (narcissistic): sum of cubes of digits == number (for 3-digit)
int is_armstrong_3(int n) {
    int d1;
    int d2;
    int d3;
    d1 = n / 100;
    d2 = (n / 10) % 10;
    d3 = n % 10;
    if (d1 * d1 * d1 + d2 * d2 * d2 + d3 * d3 * d3 == n) return 1;
    return 0;
}

// Kaprekar routine for 4-digit numbers
// Sort digits desc - sort digits asc, repeat until reaching 6174 or 0
int kaprekar_steps(int n) {
    int digits[4];
    int i;
    int j;
    int t;
    int desc;
    int asc;
    int steps;
    steps = 0;
    while (n != 6174 && n != 0 && steps < 10) {
        // Extract digits
        digits[0] = n / 1000;
        digits[1] = (n / 100) % 10;
        digits[2] = (n / 10) % 10;
        digits[3] = n % 10;
        // Sort ascending
        for (i = 0; i < 3; i = i + 1) {
            for (j = i + 1; j < 4; j = j + 1) {
                if (digits[i] > digits[j]) {
                    t = digits[i];
                    digits[i] = digits[j];
                    digits[j] = t;
                }
            }
        }
        asc = digits[0]*1000 + digits[1]*100 + digits[2]*10 + digits[3];
        desc = digits[3]*1000 + digits[2]*100 + digits[1]*10 + digits[0];
        n = desc - asc;
        steps = steps + 1;
    }
    return steps;
}

// Number reversal
int reverse_number(int n) {
    int rev;
    rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }
    return rev;
}

// Check palindrome number
int is_palindrome_num(int n) {
    if (n == reverse_number(n)) return 1;
    return 0;
}

// Catalan number
int catalan(int n) {
    int dp[20];
    int i;
    int j;
    dp[0] = 1;
    dp[1] = 1;
    for (i = 2; i <= n; i = i + 1) {
        dp[i] = 0;
        for (j = 0; j < i; j = j + 1) {
            dp[i] = dp[i] + dp[j] * dp[i - 1 - j];
        }
    }
    return dp[n];
}

// Josephus problem: n people in circle, every k-th eliminated
// Return 0-indexed position of survivor
int josephus(int n, int k) {
    int result;
    int i;
    result = 0;
    for (i = 2; i <= n; i = i + 1) {
        result = (result + k) % i;
    }
    return result;
}

// Tower of Hanoi move count
int hanoi_moves(int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i = i + 1) {
        result = result * 2;
    }
    return result - 1;
}

// Triangular number
int triangular(int n) {
    return n * (n + 1) / 2;
}

// Pentagonal number
int pentagonal(int n) {
    return n * (3 * n - 1) / 2;
}

int main(void) {
    int piles[10];
    int moves[10];
    int i;

    // Test 1-3: Standard Nim
    piles[0] = 3; piles[1] = 4; piles[2] = 5;
    printf("%d", nim_winner(piles, 3)); // 3^4^5 = 7^5 = 2, != 0, first wins: 1

    piles[0] = 1; piles[1] = 1;
    printf(" %d", nim_winner(piles, 2)); // 1^1 = 0, second wins: 0

    piles[0] = 7;
    printf(" %d", nim_winner(piles, 1)); // 7, first wins: 1

    // Test 4-6: Single pile Nim
    printf(" %d", nim_single(10, 3)); // 10 % 4 = 2 != 0: 1
    printf(" %d", nim_single(8, 3));  // 8 % 4 = 0: 0
    printf(" %d", nim_single(15, 4)); // 15 % 5 = 0: 0

    // Test 7-10: Sprague-Grundy values
    // Moves: {1, 2, 3}
    moves[0] = 1; moves[1] = 2; moves[2] = 3;
    printf(" %d", sg_value(0, moves, 3)); // 0
    printf(" %d", sg_value(1, moves, 3)); // 1
    printf(" %d", sg_value(4, moves, 3)); // SG(4): can reach SG(3)=3,SG(2)=2,SG(1)=1 -> mex{1,2,3}=0
    printf(" %d", sg_value(7, moves, 3)); // periodic: 0,1,2,3,0,1,2,3 -> SG(7)=3

    // Test 11-12: Moves {1, 3, 4}
    moves[0] = 1; moves[1] = 3; moves[2] = 4;
    printf(" %d", sg_value(0, moves, 3)); // 0
    printf(" %d", sg_value(7, moves, 3)); // Let me compute: SG(0)=0,SG(1)=mex{0}=1,SG(2)=mex{1}=0,SG(3)=mex{0,1}=2,SG(4)=mex{2,0,0}=1,SG(5)=mex{1,2,1}=0,SG(6)=mex{0,1,0}=2,SG(7)=mex{2,0,2}=1

    // Test 13-14: Wythoff's game
    printf(" %d", wythoff_winner(0, 0)); // P-position: 0
    printf(" %d", wythoff_winner(1, 2)); // P-position (1,2): diff=1, target=1*1618/1000=1=mn: 0

    // Test 15-16: Subtraction game
    printf(" %d", subtraction_game(10, 3)); // 10%4=2!=0: 1
    printf(" %d", subtraction_game(12, 3)); // 12%4=0: 0

    // Test 17-19: Misere Nim
    piles[0] = 1; piles[1] = 1; piles[2] = 1;
    printf(" %d", misere_nim(piles, 3)); // all 1s, n=3 odd: 1

    piles[0] = 3; piles[1] = 4; piles[2] = 5;
    printf(" %d", misere_nim(piles, 3)); // xor=2!=0, not all 1: 1

    piles[0] = 1; piles[1] = 1;
    printf(" %d", misere_nim(piles, 2)); // all 1s, n=2 even: 0

    // Test 20-22: GCD steps and value
    printf(" %d", gcd_steps(12, 8)); // 12-8=4, 8-4=4, done: 4->err
    // 12,8 -> 4,8 -> 4,4: 2 steps
    printf(" %d", gcd(48, 18)); // 6
    printf(" %d", lcm(4, 6));   // 12

    // Test 23-25: Power mod
    printf(" %d", power_mod(2, 10, 1000)); // 1024 % 1000 = 24
    printf(" %d", power_mod(3, 5, 100));   // 243 % 100 = 43
    printf(" %d", power_mod(7, 3, 13));    // 343 % 13 = 5

    // Test 26-27: Zeckendorf
    printf(" %d", zeckendorf_count(10)); // 10 = 8+2: 2
    printf(" %d", zeckendorf_count(20)); // 20 = 21-1 -> 20=13+5+2: 3

    // Test 28-30: Digital root
    printf(" %d", digital_root(123)); // 1+2+3=6
    printf(" %d", digital_root(999)); // 9+9+9=27->2+7=9
    printf(" %d", digital_root(1));   // 1

    // Test 31-33: Digit sum and count
    printf(" %d", digit_sum(12345)); // 15
    printf(" %d", digit_count(12345)); // 5
    printf(" %d", digit_count(0));     // 1

    // Test 34-36: Collatz
    printf(" %d", collatz_length(1));  // 0
    printf(" %d", collatz_length(6));  // 6->3->10->5->16->8->4->2->1: 8
    printf(" %d", collatz_length(27)); // known to be 111

    // Test 37-39: Happy numbers
    printf(" %d", is_happy(7));   // 7->49->97->130->10->1: 1
    printf(" %d", is_happy(4));   // cycle, not happy: 0
    printf(" %d", is_happy(100)); // 1+0+0=1: 1

    // Test 40-41: Perfect numbers
    printf(" %d", is_perfect(6));   // 1+2+3=6: 1
    printf(" %d", is_perfect(28));  // 1+2+4+7+14=28: 1

    // Test 42-43: Armstrong
    printf(" %d", is_armstrong_3(153)); // 1+125+27=153: 1
    printf(" %d", is_armstrong_3(370)); // 27+343+0=370: 1

    // Test 44: Kaprekar
    printf(" %d", kaprekar_steps(3524)); // 3524: 5432-2345=3087, 8730-378=8352, 8532-2358=6174: 3

    // Test 45-46: Reverse number, palindrome
    printf(" %d", reverse_number(1234)); // 4321
    printf(" %d", is_palindrome_num(12321)); // 1

    // Test 47-48: Catalan numbers
    printf(" %d", catalan(5));  // 42
    printf(" %d", catalan(4));  // 14

    // Test 49-50: Josephus
    printf(" %d", josephus(7, 3)); // 3 (0-indexed)
    printf(" %d", josephus(5, 2)); // 2

    // Additional tests to fill lines
    printf(" %d", hanoi_moves(4));    // 15
    printf(" %d", triangular(10));    // 55
    printf(" %d", pentagonal(5));     // 35

    printf("\n");
    return 0;
}
