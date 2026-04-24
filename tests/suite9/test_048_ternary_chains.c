int printf(const char *fmt, ...);

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int clamp(int x, int lo, int hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

int max3(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

int min3(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int median3(int a, int b, int c) {
    int mx = max3(a, b, c);
    int mn = min3(a, b, c);
    return a + b + c - mx - mn;
}

int sign(int x) {
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

int classify_bmi(int bmi_x10) {
    return bmi_x10 < 185 ? 0
         : bmi_x10 < 250 ? 1
         : bmi_x10 < 300 ? 2
         : 3;
}

int grade(int score) {
    return score >= 90 ? 4
         : score >= 80 ? 3
         : score >= 70 ? 2
         : score >= 60 ? 1
         : 0;
}

int ternary_side_effect_count;

int inc_and_return(int val) {
    ternary_side_effect_count = ternary_side_effect_count + 1;
    return val;
}

int nested_ternary_complex(int a, int b, int c, int d) {
    return (a > b)
        ? ((c > d)
            ? (a > c ? a * 10 + c : c * 10 + a)
            : (a > d ? a * 10 + d : d * 10 + a))
        : ((c > d)
            ? (b > c ? b * 10 + c : c * 10 + b)
            : (b > d ? b * 10 + d : d * 10 + b));
}

int fizzbuzz_class(int n) {
    return (n % 15 == 0) ? 3
         : (n % 3 == 0) ? 1
         : (n % 5 == 0) ? 2
         : 0;
}

int main() {
    // EXPECT: abs -7: 7
    printf("abs -7: %d\n", abs_val(-7));
    // EXPECT: abs 5: 5
    printf("abs 5: %d\n", abs_val(5));
    // EXPECT: abs 0: 0
    printf("abs 0: %d\n", abs_val(0));

    // EXPECT: clamp -5: 0
    printf("clamp -5: %d\n", clamp(-5, 0, 10));
    // EXPECT: clamp 5: 5
    printf("clamp 5: %d\n", clamp(5, 0, 10));
    // EXPECT: clamp 15: 10
    printf("clamp 15: %d\n", clamp(15, 0, 10));

    // EXPECT: max3 3 7 5: 7
    printf("max3 3 7 5: %d\n", max3(3, 7, 5));
    // EXPECT: min3 3 7 5: 3
    printf("min3 3 7 5: %d\n", min3(3, 7, 5));
    // EXPECT: median3 3 7 5: 5
    printf("median3 3 7 5: %d\n", median3(3, 7, 5));

    // EXPECT: sign -42: -1
    printf("sign -42: %d\n", sign(-42));
    // EXPECT: sign 0: 0
    printf("sign 0: %d\n", sign(0));
    // EXPECT: sign 99: 1
    printf("sign 99: %d\n", sign(99));

    // EXPECT: bmi 170: 0
    printf("bmi 170: %d\n", classify_bmi(170));
    // EXPECT: bmi 220: 1
    printf("bmi 220: %d\n", classify_bmi(220));
    // EXPECT: bmi 275: 2
    printf("bmi 275: %d\n", classify_bmi(275));
    // EXPECT: bmi 350: 3
    printf("bmi 350: %d\n", classify_bmi(350));

    int gpa = 0;
    int scores[] = {95, 82, 67, 73, 88, 91, 55, 78};
    int i;
    for (i = 0; i < 8; i = i + 1) {
        gpa = gpa + grade(scores[i]);
    }
    // EXPECT: total gpa: 19
    printf("total gpa: %d\n", gpa);

    ternary_side_effect_count = 0;
    int r = 1 ? inc_and_return(10) : inc_and_return(20);
    // EXPECT: ternary selected: 10
    printf("ternary selected: %d\n", r);
    // EXPECT: side effects: 1
    printf("side effects: %d\n", ternary_side_effect_count);

    // EXPECT: complex 8 3 6 2: 86
    printf("complex 8 3 6 2: %d\n", nested_ternary_complex(8, 3, 6, 2));
    // EXPECT: complex 2 7 5 9: 97
    printf("complex 2 7 5 9: %d\n", nested_ternary_complex(2, 7, 5, 9));

    int fb_sum = 0;
    for (i = 1; i <= 30; i = i + 1) {
        fb_sum = fb_sum + fizzbuzz_class(i);
    }
    // EXPECT: fizzbuzz class sum: 22
    printf("fizzbuzz class sum: %d\n", fb_sum);

    return 0;
}
