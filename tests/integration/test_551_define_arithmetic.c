int printf(const char *fmt, ...);
// EXPECT: SQUARE(5) = 25\nCUBE(3) = 27\nADD(5,3) = 8\nSUB(5,3) = 2\nMUL(5,3) = 15\nDIV(5,3) = 1\nMOD(5,3) = 2\nABS(-7) = 7\nDOUBLE(5) = 10\nHALF(5) = 2\nIS_EVEN(5) = 0\nIS_ODD(5) = 1\nCLAMP(5,0,4) = 4\nCLAMP(-7,0,10) = 0\nBETWEEN(5,1,10) = 1\nBETWEEN(-7,0,10) = 0\nSum of squares 1..10 = 385\n5! = 120\nAbsSum = 46
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define ADD(a, b) ((a) + (b))
#define SUB(a, b) ((a) - (b))
#define MUL(a, b) ((a) * (b))
#define DIV(a, b) ((a) / (b))
#define MOD(a, b) ((a) % (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define DOUBLE(x) ((x) << 1)
#define HALF(x) ((x) >> 1)
#define IS_EVEN(x) (((x) & 1) == 0)
#define IS_ODD(x) (((x) & 1) == 1)
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define BETWEEN(x, lo, hi) ((x) >= (lo) && (x) <= (hi))

int main(void) {
    int a = 5;
    int b = 3;
    int c = -7;

    printf("SQUARE(5) = %d\n", SQUARE(a));
    printf("CUBE(3) = %d\n", CUBE(b));
    printf("ADD(5,3) = %d\n", ADD(a, b));
    printf("SUB(5,3) = %d\n", SUB(a, b));
    printf("MUL(5,3) = %d\n", MUL(a, b));
    printf("DIV(5,3) = %d\n", DIV(a, b));
    printf("MOD(5,3) = %d\n", MOD(a, b));
    printf("ABS(-7) = %d\n", ABS(c));
    printf("DOUBLE(5) = %d\n", DOUBLE(a));
    printf("HALF(5) = %d\n", HALF(a));
    printf("IS_EVEN(5) = %d\n", IS_EVEN(a));
    printf("IS_ODD(5) = %d\n", IS_ODD(a));
    printf("CLAMP(5,0,4) = %d\n", CLAMP(a, 0, 4));
    printf("CLAMP(-7,0,10) = %d\n", CLAMP(c, 0, 10));
    printf("BETWEEN(5,1,10) = %d\n", BETWEEN(a, 1, 10));
    printf("BETWEEN(-7,0,10) = %d\n", BETWEEN(c, 0, 10));

    int sum = 0;
    int i;
    for (i = 1; i <= 10; i++) {
        sum = ADD(sum, SQUARE(i));
    }
    printf("Sum of squares 1..10 = %d\n", sum);

    int product = 1;
    for (i = 1; i <= 5; i++) {
        product = MUL(product, i);
    }
    printf("5! = %d\n", product);

    int vals[5];
    vals[0] = 12; vals[1] = -3; vals[2] = 7; vals[3] = -15; vals[4] = 9;
    int abs_sum = 0;
    for (i = 0; i < 5; i++) {
        abs_sum = ADD(abs_sum, ABS(vals[i]));
    }
    printf("AbsSum = %d\n", abs_sum);

    return 0;
}
