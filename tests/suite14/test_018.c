int printf(const char *fmt, ...);

// Collatz conjecture sequence analysis

// One step of Collatz sequence
int collatz_step(int n) {
    if (n % 2 == 0) {
        return n / 2;
    } else {
        return 3 * n + 1;
    }
}

// Count steps to reach 1 (stopping time)
int collatz_length(int n) {
    int steps = 0;
    while (n != 1) {
        n = collatz_step(n);
        steps = steps + 1;
    }
    return steps;
}

// Find maximum value in Collatz sequence
int collatz_max(int n) {
    int max_val = n;
    while (n != 1) {
        n = collatz_step(n);
        if (n > max_val) {
            max_val = n;
        }
    }
    return max_val;
}

// Count even steps in Collatz sequence
int collatz_even_steps(int n) {
    int count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            count = count + 1;
        }
        n = collatz_step(n);
    }
    return count;
}

// Count odd steps in Collatz sequence
int collatz_odd_steps(int n) {
    int count = 0;
    while (n != 1) {
        if (n % 2 != 0) {
            count = count + 1;
        }
        n = collatz_step(n);
    }
    return count;
}

// Find the number with the longest Collatz sequence in [1..n]
int longest_collatz(int n) {
    int best = 1;
    int best_len = 0;
    int i;
    for (i = 1; i <= n; i++) {
        int len = collatz_length(i);
        if (len > best_len) {
            best_len = len;
            best = i;
        }
    }
    return best;
}

// Find the number with the highest peak in Collatz sequence in [1..n]
int highest_peak(int n) {
    int best = 1;
    int best_max = 1;
    int i;
    for (i = 1; i <= n; i++) {
        int m = collatz_max(i);
        if (m > best_max) {
            best_max = m;
            best = i;
        }
    }
    return best;
}

// Store first few elements of Collatz sequence
void collatz_seq(int n, int *buf, int max_len, int *actual_len) {
    int idx = 0;
    buf[idx] = n;
    idx = idx + 1;
    while (n != 1 && idx < max_len) {
        n = collatz_step(n);
        buf[idx] = n;
        idx = idx + 1;
    }
    *actual_len = idx;
}

// Total stopping time: sum of all Collatz lengths for 1..n
int total_stopping_time(int n) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        sum = sum + collatz_length(i);
    }
    return sum;
}

// Count numbers in [1..n] whose Collatz sequence exceeds a threshold length
int count_long_sequences(int n, int threshold) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (collatz_length(i) > threshold) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    // Basic sequences
    printf("collatz(1) steps: %d\n", collatz_length(1));
    // EXPECT: collatz(1) steps: 0

    printf("collatz(2) steps: %d\n", collatz_length(2));
    // EXPECT: collatz(2) steps: 1

    printf("collatz(3) steps: %d\n", collatz_length(3));
    // EXPECT: collatz(3) steps: 7

    printf("collatz(6) steps: %d\n", collatz_length(6));
    // EXPECT: collatz(6) steps: 8

    printf("collatz(27) steps: %d\n", collatz_length(27));
    // EXPECT: collatz(27) steps: 111

    // Maximum values
    printf("collatz_max(3): %d\n", collatz_max(3));
    // EXPECT: collatz_max(3): 16

    printf("collatz_max(27): %d\n", collatz_max(27));
    // EXPECT: collatz_max(27): 9232

    // Even/odd steps for 7
    // 7->22->11->34->17->52->26->13->40->20->10->5->16->8->4->2->1
    // odd: 7, 11, 17, 13, 5 = 5 odd steps
    // even: 22, 34, 52, 26, 40, 20, 10, 16, 8, 4, 2 = 11 even steps
    printf("collatz(7) even: %d\n", collatz_even_steps(7));
    // EXPECT: collatz(7) even: 11

    printf("collatz(7) odd: %d\n", collatz_odd_steps(7));
    // EXPECT: collatz(7) odd: 5

    // Longest in range
    // In 1..100, n=97 has length 118
    int l = longest_collatz(100);
    printf("longest in [1,100]: %d\n", l);
    // EXPECT: longest in [1,100]: 97

    printf("length of 97: %d\n", collatz_length(97));
    // EXPECT: length of 97: 118

    // Highest peak in [1..50]
    int hp = highest_peak(50);
    printf("highest peak in [1,50]: %d\n", hp);
    // EXPECT: highest peak in [1,50]: 27

    // Sequence of 6: 6,3,10,5,16,8,4,2,1
    int buf[20];
    int len;
    collatz_seq(6, buf, 20, &len);
    printf("seq(6) len: %d\n", len);
    // EXPECT: seq(6) len: 9

    printf("seq(6): %d,%d,%d,%d,%d,%d,%d,%d,%d\n",
           buf[0], buf[1], buf[2], buf[3], buf[4],
           buf[5], buf[6], buf[7], buf[8]);
    // EXPECT: seq(6): 6,3,10,5,16,8,4,2,1

    // Total stopping time for 1..20
    int tst = total_stopping_time(20);
    printf("total stopping time [1,20]: %d\n", tst);
    // EXPECT: total stopping time [1,20]: 196

    // Count numbers in [1,100] with Collatz length > 50
    int cl = count_long_sequences(100, 50);
    printf("sequences > 50 in [1,100]: %d\n", cl);
    // EXPECT: sequences > 50 in [1,100]: 16

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
