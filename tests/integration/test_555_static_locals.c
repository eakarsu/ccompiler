int printf(const char *fmt, ...);
// EXPECT: Counter: 1 2 3 4 5 \nAccumulator: 10 30 60 100 150 210 \nRunning min: 42 17 17 8 8 3 3 3 \nRunning max: 42 42 93 93 93 93 93 93 \nFibonacci: 0 1 1 2 3 5 8 13 21 34 \nToggle: 1 0 1 0 1 0 

int counter(void) {
    static int count = 0;
    count++;
    return count;
}

int accumulator(int val) {
    static int total = 0;
    total = total + val;
    return total;
}

int running_min(int val) {
    static int first = 1;
    static int min_val = 0;
    if (first) {
        min_val = val;
        first = 0;
    } else if (val < min_val) {
        min_val = val;
    }
    return min_val;
}

int running_max(int val) {
    static int first = 1;
    static int max_val = 0;
    if (first) {
        max_val = val;
        first = 0;
    } else if (val > max_val) {
        max_val = val;
    }
    return max_val;
}

int fibonacci_next(void) {
    static int a = 0;
    static int b = 1;
    int result = a;
    int temp = a + b;
    a = b;
    b = temp;
    return result;
}

int toggle(void) {
    static int state = 0;
    state = 1 - state;
    return state;
}

int main(void) {
    int i;
    printf("Counter: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", counter());
    }
    printf("\n");

    printf("Accumulator: ");
    int vals[6];
    vals[0] = 10; vals[1] = 20; vals[2] = 30; vals[3] = 40; vals[4] = 50; vals[5] = 60;
    for (i = 0; i < 6; i++) {
        printf("%d ", accumulator(vals[i]));
    }
    printf("\n");

    int data[8];
    data[0] = 42; data[1] = 17; data[2] = 93; data[3] = 8;
    data[4] = 55; data[5] = 3; data[6] = 71; data[7] = 29;
    printf("Running min: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", running_min(data[i]));
    }
    printf("\n");

    printf("Running max: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", running_max(data[i]));
    }
    printf("\n");

    printf("Fibonacci: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", fibonacci_next());
    }
    printf("\n");

    printf("Toggle: ");
    for (i = 0; i < 6; i++) {
        printf("%d ", toggle());
    }
    printf("\n");

    return 0;
}
