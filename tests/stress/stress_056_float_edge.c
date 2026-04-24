// stress_056_float_edge.c - Float edge cases stress test
// Float precision, truncation, comparison, epsilon, structs with floats.
// EXPECT: tiny_add=0.002000
// EXPECT: large_add=2000000000000000.000000
// EXPECT: eps_eq=1
// EXPECT: eps_ne=0
// EXPECT: trunc_pos=3
// EXPECT: trunc_neg=-3
// EXPECT: float_chain=55.000000
// EXPECT: double_chain=55.000000
// EXPECT: negzero_eq=1
// EXPECT: struct_float=x=1.500000 y=2.500000 sum=4.000000
// EXPECT: float_array_sum=5.500000
// EXPECT: float_mul_chain=3628800.000000
// EXPECT: int_to_float=42.000000
// EXPECT: float_div_precision=0.333333
// EXPECT: float_tests=14

int printf(const char *fmt, ...);

double fabs_d(double x) {
    if (x < 0.0) return -x;
    return x;
}

int eps_equal(double a, double b, double eps) {
    return fabs_d(a - b) < eps;
}

struct Point2D {
    double x;
    double y;
};

int main(void) {
    int tests = 0;

    // Tiny float addition
    double tiny = 0.001 + 0.001;
    printf("tiny_add=%f\n", tiny);
    tests++;

    // Large float addition
    double large = 1000000000000000.0 + 1000000000000000.0;
    printf("large_add=%f\n", large);
    tests++;

    // Epsilon comparison (equal)
    double a = 1.0 / 3.0;
    double b = a * 3.0;
    // b should be very close to 1.0
    int eq = eps_equal(b, 1.0, 0.000001);
    printf("eps_eq=%d\n", eq);
    tests++;

    // Epsilon comparison (not equal)
    int ne = eps_equal(1.0, 1.1, 0.000001);
    printf("eps_ne=%d\n", ne);
    tests++;

    // Float to int truncation (positive)
    double fp = 3.999;
    int ip = (int)fp;
    printf("trunc_pos=%d\n", ip);
    tests++;

    // Float to int truncation (negative)
    double fn = -3.999;
    int in_val = (int)fn;
    printf("trunc_neg=%d\n", in_val);
    tests++;

    // Float arithmetic chain (sum of 1.0 to 10.0)
    float fsum = 0.0;
    int i;
    for (i = 1; i <= 10; i++) {
        fsum = fsum + (float)i;
    }
    printf("float_chain=%f\n", (double)fsum);
    tests++;

    // Double arithmetic chain
    double dsum = 0.0;
    for (i = 1; i <= 10; i++) {
        dsum += (double)i;
    }
    printf("double_chain=%f\n", dsum);
    tests++;

    // Negative zero
    double pz = 0.0;
    double nz = -0.0;
    printf("negzero_eq=%d\n", pz == nz ? 1 : 0);
    tests++;

    // Float in struct
    struct Point2D pt;
    pt.x = 1.5;
    pt.y = 2.5;
    double psum = pt.x + pt.y;
    printf("struct_float=x=%f y=%f sum=%f\n", pt.x, pt.y, psum);
    tests++;

    // Float array sum
    double arr[5];
    arr[0] = 1.1; arr[1] = 1.1; arr[2] = 1.1; arr[3] = 1.1; arr[4] = 1.1;
    double asum = 0.0;
    for (i = 0; i < 5; i++) {
        asum += arr[i];
    }
    printf("float_array_sum=%f\n", asum);
    tests++;

    // Float multiplication chain: 10!
    double prod = 1.0;
    for (i = 1; i <= 10; i++) {
        prod *= (double)i;
    }
    printf("float_mul_chain=%f\n", prod);
    tests++;

    // Int to float conversion
    int ival = 42;
    double dval = (double)ival;
    printf("int_to_float=%f\n", dval);
    tests++;

    // Float division precision
    double third = 1.0 / 3.0;
    printf("float_div_precision=%f\n", third);
    tests++;

    printf("float_tests=%d\n", tests);

    return 0;
}
