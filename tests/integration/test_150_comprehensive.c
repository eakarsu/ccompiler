int printf(const char *fmt, ...);
// EXPECT: set: 1 4 9 16 25 \nsum = 55\nproduct = 14400\nr1=74 r2=36 r3=5\npowers: 1 2 4 8 16 32 64 128 256 \nmatrix:\n11 12 13 14 \n21 22 23 24 \n31 32 33 34 \n41 42 43 44 \ntrace = 110\ndet2x2 = -10\ngcd chain: 12\nsum=345 prod_odd=555\nDONE
// Test: comprehensive arithmetic and expression test

typedef struct {
    int vals[5];
    int count;
} intset;

void intset_init(intset *s) {
    s->count = 0;
}

void intset_add(intset *s, int v) {
    if (s->count < 5) {
        s->vals[s->count] = v;
        s->count++;
    }
}

int intset_sum(intset *s) {
    int i, sum;
    sum = 0;
    for (i = 0; i < s->count; i++) {
        sum += s->vals[i];
    }
    return sum;
}

int intset_product(intset *s) {
    int i, prod;
    prod = 1;
    for (i = 0; i < s->count; i++) {
        prod *= s->vals[i];
    }
    return prod;
}

int power(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

int gcd(int a, int b) {
    while (b) { int t; t = b; b = a % b; a = t; }
    return a;
}

int main(void) {
    intset s;
    int i;
    int matrix[4][4];
    int trace, det2x2;

    /* IntSet operations */
    intset_init(&s);
    for (i = 1; i <= 5; i++) {
        intset_add(&s, i * i);
    }
    printf("set: ");
    for (i = 0; i < s.count; i++) {
        printf("%d ", s.vals[i]);
    }
    printf("\n");
    printf("sum = %d\n", intset_sum(&s));
    printf("product = %d\n", intset_product(&s));

    /* Complex expression combining everything */
    {
        int a, b, c;
        int r1, r2, r3;
        a = 17;
        b = 5;
        c = 3;
        r1 = (a + b) * c - a % b + (b << 1);
        r2 = (a > b) ? (a - b) * c : (b - a) * c;
        r3 = gcd(a * b, b * c);
        printf("r1=%d r2=%d r3=%d\n", r1, r2, r3);
    }

    /* Power computations */
    printf("powers: ");
    for (i = 0; i <= 8; i++) {
        printf("%d ", power(2, i));
    }
    printf("\n");

    /* Matrix operations */
    for (i = 0; i < 4; i++) {
        int j;
        for (j = 0; j < 4; j++) {
            matrix[i][j] = (i + 1) * 10 + (j + 1);
        }
    }
    printf("matrix:\n");
    for (i = 0; i < 4; i++) {
        int j;
        for (j = 0; j < 4; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    trace = 0;
    for (i = 0; i < 4; i++) {
        trace += matrix[i][i];
    }
    printf("trace = %d\n", trace);

    /* 2x2 determinant of top-left corner */
    det2x2 = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    printf("det2x2 = %d\n", det2x2);

    /* Chained GCD */
    printf("gcd chain: %d\n", gcd(gcd(120, 84), gcd(36, 60)));

    /* Final comprehensive computation */
    {
        int sum, prod_odd;
        int arr[10];
        for (i = 0; i < 10; i++) {
            arr[i] = i * 7 + 3;
        }
        sum = 0;
        prod_odd = 1;
        for (i = 0; i < 10; i++) {
            sum += arr[i];
            if (arr[i] % 2 == 1) {
                prod_odd = (prod_odd * arr[i]) % 1000;
            }
        }
        printf("sum=%d prod_odd=%d\n", sum, prod_odd);
    }

    printf("DONE\n");

    return 0;
}
