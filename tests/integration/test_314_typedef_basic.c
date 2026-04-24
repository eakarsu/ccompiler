int printf(const char *fmt, ...);
// EXPECT: add: 25\nmultiply: -714\nmax: 42\nmin: -17\nabs(-17): 17\nis_positive(42): 1\nis_positive(-17): 0\nis_even(42): 1\nis_even(-17): 0\nclamp(50,0,100): 50\nclamp(-5,0,100): 0\nclamp(200,0,100): 100\nto_upper('h'): H\nto_lower('Z'): z
// Test: typedef for basic types

typedef int Integer;
typedef int Bool;
typedef char Byte;

Integer add(Integer a, Integer b) {
    return a + b;
}

Integer multiply(Integer a, Integer b) {
    return a * b;
}

Integer max_val(Integer a, Integer b) {
    if (a > b) return a;
    return b;
}

Integer min_val(Integer a, Integer b) {
    if (a < b) return a;
    return b;
}

Integer abs_val(Integer x) {
    if (x < 0) return -x;
    return x;
}

Bool is_positive(Integer x) {
    return x > 0;
}

Bool is_even(Integer x) {
    return (x % 2) == 0;
}

Integer clamp(Integer val, Integer lo, Integer hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

Byte to_upper(Byte c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

Byte to_lower(Byte c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

int main(void) {
    Integer x = 42;
    Integer y = -17;

    printf("add: %d\n", add(x, y));
    printf("multiply: %d\n", multiply(x, y));
    printf("max: %d\n", max_val(x, y));
    printf("min: %d\n", min_val(x, y));
    printf("abs(%d): %d\n", y, abs_val(y));

    printf("is_positive(%d): %d\n", x, is_positive(x));
    printf("is_positive(%d): %d\n", y, is_positive(y));
    printf("is_even(%d): %d\n", x, is_even(x));
    printf("is_even(%d): %d\n", y, is_even(y));

    printf("clamp(50,0,100): %d\n", clamp(50, 0, 100));
    printf("clamp(-5,0,100): %d\n", clamp(-5, 0, 100));
    printf("clamp(200,0,100): %d\n", clamp(200, 0, 100));

    Byte ch = 'h';
    printf("to_upper('%c'): %c\n", ch, to_upper(ch));
    ch = 'Z';
    printf("to_lower('%c'): %c\n", ch, to_lower(ch));

    return 0;
}
