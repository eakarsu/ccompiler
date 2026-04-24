int printf(const char *fmt, ...);

int positive_mod(int a, int m) {
    int r = a % m;
    if (r < 0) r = r + m;
    return r;
}

int clock_add(int hour, int delta) {
    return positive_mod(hour + delta, 12);
}

int cyclic_index(int idx, int size) {
    return positive_mod(idx, size);
}

int wrap_around(int val, int lo, int hi) {
    int range = hi - lo;
    return lo + positive_mod(val - lo, range);
}

int is_even(int n) {
    int r = n % 2;
    if (r < 0) r = -r;
    return r == 0;
}

int is_divisible(int n, int d) {
    if (d == 0) return 0;
    return (n % d) == 0;
}

int day_of_week(int day_number) {
    return positive_mod(day_number, 7);
}

int rotate_index(int current, int offset, int size) {
    return positive_mod(current + offset, size);
}

int ring_buffer_next(int head, int capacity) {
    return (head + 1) % capacity;
}

int circular_distance(int a, int b, int modulus) {
    int d1 = positive_mod(a - b, modulus);
    int d2 = positive_mod(b - a, modulus);
    if (d1 < d2) return d1;
    return d2;
}

int fizzbuzz_type(int n) {
    if (n % 15 == 0) return 3;
    if (n % 3 == 0) return 1;
    if (n % 5 == 0) return 2;
    return 0;
}

int sum_multiples(int limit, int divisor) {
    int sum = 0;
    int i;
    for (i = divisor; i < limit; i = i + divisor) {
        sum = sum + i;
    }
    return sum;
}

int count_remainders(int n, int mod) {
    int counts[10];
    int i;
    for (i = 0; i < 10; i++) counts[i] = 0;
    for (i = 0; i < n; i++) {
        int r = i % mod;
        if (r < 10) counts[r] = counts[r] + 1;
    }
    return counts[0];
}

int main(void) {
    // EXPECT: positive_mod(7,3)=1
    printf("positive_mod(7,3)=%d\n", positive_mod(7, 3));

    // EXPECT: positive_mod(-7,3)=2
    printf("positive_mod(-7,3)=%d\n", positive_mod(-7, 3));

    // EXPECT: positive_mod(-1,12)=11
    printf("positive_mod(-1,12)=%d\n", positive_mod(-1, 12));

    // EXPECT: clock_add(10,5)=3
    printf("clock_add(10,5)=%d\n", clock_add(10, 5));

    // EXPECT: clock_add(3,-5)=10
    printf("clock_add(3,-5)=%d\n", clock_add(3, -5));

    // EXPECT: cyclic_index(-1,5)=4
    printf("cyclic_index(-1,5)=%d\n", cyclic_index(-1, 5));

    // EXPECT: cyclic_index(7,5)=2
    printf("cyclic_index(7,5)=%d\n", cyclic_index(7, 5));

    // EXPECT: wrap_around(15,0,10)=5
    printf("wrap_around(15,0,10)=%d\n", wrap_around(15, 0, 10));

    // EXPECT: wrap_around(-1,0,10)=9
    printf("wrap_around(-1,0,10)=%d\n", wrap_around(-1, 0, 10));

    // EXPECT: is_even(4)=1
    printf("is_even(4)=%d\n", is_even(4));

    // EXPECT: is_even(-3)=0
    printf("is_even(-3)=%d\n", is_even(-3));

    // EXPECT: is_divisible(15,5)=1
    printf("is_divisible(15,5)=%d\n", is_divisible(15, 5));

    // EXPECT: is_divisible(16,5)=0
    printf("is_divisible(16,5)=%d\n", is_divisible(16, 5));

    // EXPECT: day_of_week(0)=0
    printf("day_of_week(0)=%d\n", day_of_week(0));

    // EXPECT: day_of_week(-1)=6
    printf("day_of_week(-1)=%d\n", day_of_week(-1));

    // EXPECT: rotate_index(3,4,5)=2
    printf("rotate_index(3,4,5)=%d\n", rotate_index(3, 4, 5));

    // EXPECT: ring_next(7,8)=0
    printf("ring_next(7,8)=%d\n", ring_buffer_next(7, 8));

    // EXPECT: ring_next(3,8)=4
    printf("ring_next(3,8)=%d\n", ring_buffer_next(3, 8));

    // EXPECT: circ_dist(2,9,12)=5
    printf("circ_dist(2,9,12)=%d\n", circular_distance(2, 9, 12));

    // EXPECT: circ_dist(1,11,12)=2
    printf("circ_dist(1,11,12)=%d\n", circular_distance(1, 11, 12));

    // EXPECT: fizzbuzz(15)=3
    printf("fizzbuzz(15)=%d\n", fizzbuzz_type(15));

    // EXPECT: fizzbuzz(9)=1
    printf("fizzbuzz(9)=%d\n", fizzbuzz_type(9));

    // EXPECT: fizzbuzz(10)=2
    printf("fizzbuzz(10)=%d\n", fizzbuzz_type(10));

    // EXPECT: fizzbuzz(7)=0
    printf("fizzbuzz(7)=%d\n", fizzbuzz_type(7));

    // EXPECT: sum_multiples(20,3)=63
    printf("sum_multiples(20,3)=%d\n", sum_multiples(20, 3));

    // EXPECT: count_remainders(20,3)=7
    printf("count_remainders(20,3)=%d\n", count_remainders(20, 3));

    return 0;
}
