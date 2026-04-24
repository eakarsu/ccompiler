int printf(const char *fmt, ...);
// EXPECT: E1=1 E2=6 E3=17 E4=34 E5=2 E6=9 VN1=1 VN2=1 E7=2 E8=7 E9=1 ADD=3,3,3 ADEG=2 AE=9 SUB=-1,1,3 SDEG=2 MUL=2,5,8,3 MDEG=3 ME1=18 ME2=68 SC=2,4,6 SC0=1 DER=2,6 DDEG=1 DE1=2 DE2=8 DE3=14 DA1=2 DA2=8 TC1=3 TC2=2 CS1=6 CS2=3 CP1=6 CP2=2 IZ1=1 IZ2=0 CO1=34 CO2=17 EQ1=1 EQ2=0 SH=0,0,2,1 SHDEG=3 RI1=7 E10=5 E11=3 E12=7 E13=7 E14=23 MUL2=10,-1,-3,2,1 DER2=-3,0,3 D2=0,6 LC1=3 CT1=1 LC2=1 CT2=5 NEG=-1,-2,-3 ZRO=1 MES=115 CR1=2 CR2=1 INT=0,1,1,1 
/* ====================================================================== */
/* Test 045: Polynomial Operations                                        */
/* Evaluation (Horner), addition, multiplication using coefficient arrays */
/* ====================================================================== */

/* Evaluate polynomial at x using Horner's method */
/* poly[0] + poly[1]*x + poly[2]*x^2 + ... + poly[deg]*x^deg */
int poly_eval(int *poly, int deg, int x) {
    int result;
    int i;
    result = poly[deg];
    i = deg - 1;
    while (i >= 0) {
        result = result * x + poly[i];
        i = i - 1;
    }
    return result;
}

/* Evaluate polynomial using naive method */
int poly_eval_naive(int *poly, int deg, int x) {
    int result;
    int i;
    int pw;
    int j;
    result = 0;
    i = 0;
    while (i <= deg) {
        pw = 1;
        j = 0;
        while (j < i) {
            pw = pw * x;
            j = j + 1;
        }
        result = result + poly[i] * pw;
        i = i + 1;
    }
    return result;
}

/* Add two polynomials: result = a + b, returns degree */
int poly_add(int *a, int da, int *b, int db, int *result) {
    int md;
    int i;
    if (da > db) md = da; else md = db;
    i = 0;
    while (i <= md) {
        result[i] = 0;
        if (i <= da) result[i] = result[i] + a[i];
        if (i <= db) result[i] = result[i] + b[i];
        i = i + 1;
    }
    while (md > 0 && result[md] == 0) md = md - 1;
    return md;
}

/* Subtract: result = a - b */
int poly_sub(int *a, int da, int *b, int db, int *result) {
    int md;
    int i;
    if (da > db) md = da; else md = db;
    i = 0;
    while (i <= md) {
        result[i] = 0;
        if (i <= da) result[i] = result[i] + a[i];
        if (i <= db) result[i] = result[i] - b[i];
        i = i + 1;
    }
    while (md > 0 && result[md] == 0) md = md - 1;
    return md;
}

/* Multiply: result = a * b, returns degree */
int poly_mul(int *a, int da, int *b, int db, int *result) {
    int rd;
    int i;
    int j;
    rd = da + db;
    i = 0;
    while (i <= rd) { result[i] = 0; i = i + 1; }
    i = 0;
    while (i <= da) {
        j = 0;
        while (j <= db) {
            result[i + j] = result[i + j] + a[i] * b[j];
            j = j + 1;
        }
        i = i + 1;
    }
    while (rd > 0 && result[rd] == 0) rd = rd - 1;
    return rd;
}

/* Scale: result = c * poly */
int poly_scale(int *poly, int deg, int c, int *result) {
    int i;
    int rd;
    i = 0;
    while (i <= deg) { result[i] = poly[i] * c; i = i + 1; }
    rd = deg;
    while (rd > 0 && result[rd] == 0) rd = rd - 1;
    return rd;
}

/* Derivative: result = d/dx(poly) */
int poly_deriv(int *poly, int deg, int *result) {
    int i;
    if (deg == 0) { result[0] = 0; return 0; }
    i = 1;
    while (i <= deg) { result[i - 1] = i * poly[i]; i = i + 1; }
    return deg - 1;
}

/* Evaluate derivative at point */
int poly_deriv_at(int *poly, int deg, int x) {
    int d[20];
    int dd;
    dd = poly_deriv(poly, deg, d);
    return poly_eval(d, dd, x);
}

/* Print polynomial coefficients */
void print_poly(int *poly, int deg) {
    int i;
    i = 0;
    while (i <= deg) {
        printf("%d", poly[i]);
        if (i < deg) printf(",");
        i = i + 1;
    }
}

/* Check if polynomial is zero */
int poly_is_zero(int *poly, int deg) {
    int i;
    i = 0;
    while (i <= deg) {
        if (poly[i] != 0) return 0;
        i = i + 1;
    }
    return 1;
}

/* Sum of coefficients = poly(1) */
int poly_coeff_sum(int *poly, int deg) {
    return poly_eval(poly, deg, 1);
}

/* Product of non-zero coefficients */
int poly_coeff_prod(int *poly, int deg) {
    int prod;
    int i;
    prod = 1;
    i = 0;
    while (i <= deg) {
        if (poly[i] != 0) prod = prod * poly[i];
        i = i + 1;
    }
    return prod;
}

/* Count non-zero terms */
int poly_terms(int *poly, int deg) {
    int c;
    int i;
    c = 0;
    i = 0;
    while (i <= deg) { if (poly[i] != 0) c = c + 1; i = i + 1; }
    return c;
}

/* Compose: evaluate a(b(x)) at a given x */
int poly_compose_at(int *a, int da, int *b, int db, int x) {
    int bx;
    bx = poly_eval(b, db, x);
    return poly_eval(a, da, bx);
}

/* Check equality */
int poly_equal(int *a, int da, int *b, int db) {
    int md;
    int i;
    int va;
    int vb;
    if (da > db) md = da; else md = db;
    i = 0;
    while (i <= md) {
        va = 0; vb = 0;
        if (i <= da) va = a[i];
        if (i <= db) vb = b[i];
        if (va != vb) return 0;
        i = i + 1;
    }
    return 1;
}

/* Shift left by n (multiply by x^n) */
int poly_shift(int *poly, int deg, int n, int *result) {
    int i;
    i = 0;
    while (i < n) { result[i] = 0; i = i + 1; }
    i = 0;
    while (i <= deg) { result[i + n] = poly[i]; i = i + 1; }
    return deg + n;
}

/* Integral (integer division for coefficients) */
int poly_integral(int *poly, int deg, int *result) {
    int i;
    result[0] = 0;
    i = 0;
    while (i <= deg) {
        result[i + 1] = poly[i] / (i + 1);
        i = i + 1;
    }
    return deg + 1;
}

/* Riemann sum approximation */
int poly_riemann(int *poly, int deg, int a, int b, int n) {
    int sum;
    int i;
    int x;
    sum = 0;
    i = 0;
    while (i < n) {
        x = a + i * (b - a) / n;
        sum = sum + poly_eval(poly, deg, x);
        i = i + 1;
    }
    return sum * (b - a) / n;
}

/* Second derivative */
int poly_deriv2(int *poly, int deg, int *result) {
    int d1[20];
    int dd;
    dd = poly_deriv(poly, deg, d1);
    return poly_deriv(d1, dd, result);
}

/* Evaluate at multiple points, return sum of evaluations */
int poly_multi_eval_sum(int *poly, int deg, int lo, int hi) {
    int sum;
    int x;
    sum = 0;
    x = lo;
    while (x <= hi) {
        sum = sum + poly_eval(poly, deg, x);
        x = x + 1;
    }
    return sum;
}

/* Find integer roots in range [lo..hi] */
int poly_count_roots(int *poly, int deg, int lo, int hi) {
    int count;
    int x;
    count = 0;
    x = lo;
    while (x <= hi) {
        if (poly_eval(poly, deg, x) == 0) count = count + 1;
        x = x + 1;
    }
    return count;
}

/* Leading coefficient */
int poly_leading(int *poly, int deg) {
    return poly[deg];
}

/* Constant term */
int poly_constant(int *poly) {
    return poly[0];
}

/* Negate polynomial */
int poly_negate(int *poly, int deg, int *result) {
    int i;
    i = 0;
    while (i <= deg) { result[i] = -poly[i]; i = i + 1; }
    return deg;
}

int main(void) {
    int p1[10];
    int p2[10];
    int p3[10];
    int result[30];
    int deriv[10];
    int d2[10];
    int shifted[20];
    int integ[10];
    int neg[10];
    int deg;
    int d;

    /* p1 = 1 + 2x + 3x^2 */
    p1[0] = 1; p1[1] = 2; p1[2] = 3;

    /* p2 = 2 + x */
    p2[0] = 2; p2[1] = 1;

    /* Evaluate p1 at various x */
    printf("E1=%d ", poly_eval(p1, 2, 0));
    printf("E2=%d ", poly_eval(p1, 2, 1));
    printf("E3=%d ", poly_eval(p1, 2, 2));
    printf("E4=%d ", poly_eval(p1, 2, 3));
    printf("E5=%d ", poly_eval(p1, 2, -1));
    printf("E6=%d ", poly_eval(p1, 2, -2));

    /* Verify Horner vs naive */
    printf("VN1=%d ", poly_eval(p1, 2, 5) == poly_eval_naive(p1, 2, 5));
    printf("VN2=%d ", poly_eval(p1, 2, -3) == poly_eval_naive(p1, 2, -3));

    /* Evaluate p2 */
    printf("E7=%d ", poly_eval(p2, 1, 0));
    printf("E8=%d ", poly_eval(p2, 1, 5));
    printf("E9=%d ", poly_eval(p2, 1, -1));

    /* Addition */
    deg = poly_add(p1, 2, p2, 1, result);
    printf("ADD="); print_poly(result, deg); printf(" ");
    printf("ADEG=%d ", deg);
    printf("AE=%d ", poly_eval(result, deg, 1));

    /* Subtraction */
    deg = poly_sub(p1, 2, p2, 1, result);
    printf("SUB="); print_poly(result, deg); printf(" ");
    printf("SDEG=%d ", deg);

    /* Multiplication */
    deg = poly_mul(p1, 2, p2, 1, result);
    printf("MUL="); print_poly(result, deg); printf(" ");
    printf("MDEG=%d ", deg);
    printf("ME1=%d ", poly_eval(result, deg, 1));
    printf("ME2=%d ", poly_eval(result, deg, 2));

    /* Scale by 2 */
    deg = poly_scale(p1, 2, 2, result);
    printf("SC="); print_poly(result, deg); printf(" ");

    /* Scale by 0 */
    deg = poly_scale(p1, 2, 0, result);
    printf("SC0=%d ", poly_is_zero(result, deg));

    /* Derivative of p1: 2 + 6x */
    d = poly_deriv(p1, 2, deriv);
    printf("DER="); print_poly(deriv, d); printf(" ");
    printf("DDEG=%d ", d);
    printf("DE1=%d ", poly_eval(deriv, d, 0));
    printf("DE2=%d ", poly_eval(deriv, d, 1));
    printf("DE3=%d ", poly_eval(deriv, d, 2));

    /* Derivative at point */
    printf("DA1=%d ", poly_deriv_at(p1, 2, 0));
    printf("DA2=%d ", poly_deriv_at(p1, 2, 1));

    /* Properties */
    printf("TC1=%d ", poly_terms(p1, 2));
    printf("TC2=%d ", poly_terms(p2, 1));
    printf("CS1=%d ", poly_coeff_sum(p1, 2));
    printf("CS2=%d ", poly_coeff_sum(p2, 1));
    printf("CP1=%d ", poly_coeff_prod(p1, 2));
    printf("CP2=%d ", poly_coeff_prod(p2, 1));

    /* Zero check */
    p3[0] = 0; p3[1] = 0;
    printf("IZ1=%d ", poly_is_zero(p3, 1));
    printf("IZ2=%d ", poly_is_zero(p1, 2));

    /* Compose: p1(p2(x)) at x=1 => p2(1)=3, p1(3)=34 */
    printf("CO1=%d ", poly_compose_at(p1, 2, p2, 1, 1));
    printf("CO2=%d ", poly_compose_at(p1, 2, p2, 1, 0));

    /* Equality */
    printf("EQ1=%d ", poly_equal(p1, 2, p1, 2));
    printf("EQ2=%d ", poly_equal(p1, 2, p2, 1));

    /* Shift p2 by 2 */
    deg = poly_shift(p2, 1, 2, shifted);
    printf("SH="); print_poly(shifted, deg); printf(" ");
    printf("SHDEG=%d ", deg);

    /* Riemann sum */
    printf("RI1=%d ", poly_riemann(p1, 2, 0, 2, 100));

    /* p3 = 5 - 3x + x^3 */
    p3[0] = 5; p3[1] = -3; p3[2] = 0; p3[3] = 1;
    printf("E10=%d ", poly_eval(p3, 3, 0));
    printf("E11=%d ", poly_eval(p3, 3, 1));
    printf("E12=%d ", poly_eval(p3, 3, 2));
    printf("E13=%d ", poly_eval(p3, 3, -1));
    printf("E14=%d ", poly_eval(p3, 3, 3));

    /* p3 * p2 */
    deg = poly_mul(p3, 3, p2, 1, result);
    printf("MUL2="); print_poly(result, deg); printf(" ");

    /* Derivative of p3: -3 + 0*x + 3x^2 */
    d = poly_deriv(p3, 3, deriv);
    printf("DER2="); print_poly(deriv, d); printf(" ");

    /* Second derivative of p3: 0 + 6x -> [0, 6] */
    d = poly_deriv2(p3, 3, d2);
    printf("D2="); print_poly(d2, d); printf(" ");

    /* Leading and constant */
    printf("LC1=%d ", poly_leading(p1, 2));
    printf("CT1=%d ", poly_constant(p1));
    printf("LC2=%d ", poly_leading(p3, 3));
    printf("CT2=%d ", poly_constant(p3));

    /* Negate */
    d = poly_negate(p1, 2, neg);
    printf("NEG="); print_poly(neg, d); printf(" ");

    /* Add p1 + neg(p1) = 0 */
    deg = poly_add(p1, 2, neg, d, result);
    printf("ZRO=%d ", poly_is_zero(result, deg));

    /* Multi eval sum: p1 evaluated at 0..4 */
    printf("MES=%d ", poly_multi_eval_sum(p1, 2, 0, 4));

    /* Count roots: x^2 - 1 has roots at -1 and 1 */
    {
        int q[3];
        q[0] = -1; q[1] = 0; q[2] = 1;
        printf("CR1=%d ", poly_count_roots(q, 2, -5, 5));
    }

    /* Count roots: x^2 has root at 0 */
    {
        int q[3];
        q[0] = 0; q[1] = 0; q[2] = 1;
        printf("CR2=%d ", poly_count_roots(q, 2, -5, 5));
    }

    /* Integral of p1: C + x + x^2 + x^3 (integer division: 1/1=1, 2/2=1, 3/3=1) */
    d = poly_integral(p1, 2, integ);
    printf("INT="); print_poly(integ, d); printf(" ");

    printf("\n");
    return 0;
}
