int printf(const char *fmt, ...);
// EXPECT: add(3,4)=7\nsub(10,3)=7\nmul(6,7)=42\ndiv(20,4)=5\nmod(17,5)=2\nsq(9)=81\ncube(3)=27\nabs(-7)=7\nmax(5,9)=9\nmin(5,9)=5\nclamp(15,0,10)=10\nclamp(-5,0,10)=0\nclamp(5,0,10)=5\nsum_sq_1_10=385\nnested=12
// Test 201: Multiple function definitions and calls

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) return 0;
    return a / b;
}

int modulo(int a, int b) {
    if (b == 0) return 0;
    return a - (a / b) * b;
}

int square(int x) {
    return multiply(x, x);
}

int cube(int x) {
    return multiply(multiply(x, x), x);
}

int abs_val(int x) {
    if (x < 0) return subtract(0, x);
    return x;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int clamp(int val, int lo, int hi) {
    return min(max(val, lo), hi);
}

int main(void) {
    printf("add(3,4)=%d\n", add(3, 4));
    printf("sub(10,3)=%d\n", subtract(10, 3));
    printf("mul(6,7)=%d\n", multiply(6, 7));
    printf("div(20,4)=%d\n", divide(20, 4));
    printf("mod(17,5)=%d\n", modulo(17, 5));
    printf("sq(9)=%d\n", square(9));
    printf("cube(3)=%d\n", cube(3));
    printf("abs(-7)=%d\n", abs_val(-7));
    printf("max(5,9)=%d\n", max(5, 9));
    printf("min(5,9)=%d\n", min(5, 9));
    printf("clamp(15,0,10)=%d\n", clamp(15, 0, 10));
    printf("clamp(-5,0,10)=%d\n", clamp(-5, 0, 10));
    printf("clamp(5,0,10)=%d\n", clamp(5, 0, 10));

    int sum = 0;
    int i = 1;
    while (i <= 10) {
        sum = add(sum, square(i));
        i = i + 1;
    }
    printf("sum_sq_1_10=%d\n", sum);

    printf("nested=%d\n", add(multiply(2, 3), subtract(10, 4)));
    return 0;
}
