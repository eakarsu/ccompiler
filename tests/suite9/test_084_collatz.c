int printf(const char *fmt, ...);

// Collatz conjecture (3n+1 problem)

int collatz_next(int n) {
    if (n % 2 == 0) {
        return n / 2;
    } else {
        return 3 * n + 1;
    }
}

int collatz_length(int n) {
    int count;
    count = 0;
    while (n != 1) {
        n = collatz_next(n);
        count = count + 1;
    }
    return count;
}

int collatz_max(int n) {
    int max_val;
    max_val = n;
    while (n != 1) {
        n = collatz_next(n);
        if (n > max_val) {
            max_val = n;
        }
    }
    return max_val;
}

void print_collatz_sequence(int n) {
    printf("%d", n);
    while (n != 1) {
        n = collatz_next(n);
        printf(" %d", n);
    }
    printf("\n");
}

int collatz_even_steps(int n) {
    int count;
    count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            count = count + 1;
        }
        n = collatz_next(n);
    }
    return count;
}

int collatz_odd_steps(int n) {
    int count;
    count = 0;
    while (n != 1) {
        if (n % 2 != 0) {
            count = count + 1;
        }
        n = collatz_next(n);
    }
    return count;
}

int longest_collatz_under(int limit) {
    int best_n;
    int best_len;
    int i;
    int len;
    best_n = 1;
    best_len = 0;
    for (i = 1; i < limit; i++) {
        len = collatz_length(i);
        if (len > best_len) {
            best_len = len;
            best_n = i;
        }
    }
    return best_n;
}

int longest_collatz_length_under(int limit) {
    int best_len;
    int i;
    int len;
    best_len = 0;
    for (i = 1; i < limit; i++) {
        len = collatz_length(i);
        if (len > best_len) {
            best_len = len;
        }
    }
    return best_len;
}

int count_collatz_above_threshold(int start, int threshold) {
    int count;
    int n;
    count = 0;
    n = start;
    while (n != 1) {
        if (n > threshold) {
            count = count + 1;
        }
        n = collatz_next(n);
    }
    return count;
}

int main() {
    int len;
    int max_v;
    int ev;
    int od;
    int best_n;
    int best_len;
    int above;

    // EXPECT: Collatz sequence of 6:
    printf("Collatz sequence of 6:\n");
    // EXPECT: 6 3 10 5 16 8 4 2 1
    print_collatz_sequence(6);

    // EXPECT: Collatz sequence of 27:
    printf("Collatz sequence of 27:\n");
    // EXPECT: 27 82 41 124 62 31 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 137 412 206 103 310 155 466 233 700 350 175 526 263 790 395 1186 593 1780 890 445 1336 668 334 167 502 251 754 377 1132 566 283 850 425 1276 638 319 958 479 1438 719 2158 1079 3238 1619 4858 2429 7288 3644 1822 911 2734 1367 4102 2051 6154 3077 9232 4616 2308 1154 577 1732 866 433 1300 650 325 976 488 244 122 61 184 92 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    print_collatz_sequence(27);

    len = collatz_length(6);
    // EXPECT: Collatz length of 6: 8
    printf("Collatz length of 6: %d\n", len);

    len = collatz_length(27);
    // EXPECT: Collatz length of 27: 111
    printf("Collatz length of 27: %d\n", len);

    max_v = collatz_max(27);
    // EXPECT: Collatz max of 27: 9232
    printf("Collatz max of 27: %d\n", max_v);

    ev = collatz_even_steps(27);
    // EXPECT: Even steps for 27: 70
    printf("Even steps for 27: %d\n", ev);

    od = collatz_odd_steps(27);
    // EXPECT: Odd steps for 27: 41
    printf("Odd steps for 27: %d\n", od);

    best_n = longest_collatz_under(100);
    // EXPECT: Longest Collatz under 100 starts at: 97
    printf("Longest Collatz under 100 starts at: %d\n", best_n);

    best_len = longest_collatz_length_under(100);
    // EXPECT: Longest Collatz length under 100: 118
    printf("Longest Collatz length under 100: %d\n", best_len);

    above = count_collatz_above_threshold(27, 1000);
    // EXPECT: Steps above 1000 for 27: 27
    printf("Steps above 1000 for 27: %d\n", above);

    return 0;
}
