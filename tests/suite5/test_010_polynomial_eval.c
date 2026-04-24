int printf(const char *fmt, ...);

int horner(int coeffs[], int degree, int x) {
    int result = coeffs[0];
    int i = 1;
    while (i <= degree) {
        result = result * x + coeffs[i];
        i = i + 1;
    }
    return result;
}

int poly_eval_naive(int coeffs[], int degree, int x) {
    int result = 0;
    int i = degree;
    while (i >= 0) {
        int power = 1;
        int j = 0;
        while (j < degree - i) {
            power = power * x;
            j = j + 1;
        }
        result = result + coeffs[i] * power;
        i = i - 1;
    }
    return result;
}

void poly_add(int a[], int da, int b[], int db,
              int c[], int dc_out[]) {
    int max_d = da;
    if (db > max_d) max_d = db;
    dc_out[0] = max_d;
    int i = 0;
    while (i <= max_d) {
        c[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i <= da) {
        c[i + (max_d - da)] = c[i + (max_d - da)] + a[i];
        i = i + 1;
    }
    i = 0;
    while (i <= db) {
        c[i + (max_d - db)] = c[i + (max_d - db)] + b[i];
        i = i + 1;
    }
}

void poly_derivative(int coeffs[], int degree,
                     int deriv[], int new_deg[]) {
    if (degree == 0) {
        deriv[0] = 0;
        new_deg[0] = 0;
        return;
    }
    new_deg[0] = degree - 1;
    int i = 0;
    while (i < degree) {
        deriv[i] = coeffs[i] * (degree - i);
        i = i + 1;
    }
}

int poly_eval_at_zero(int coeffs[], int degree) {
    return coeffs[degree];
}

int poly_eval_at_one(int coeffs[], int degree) {
    int sum = 0;
    int i = 0;
    while (i <= degree) {
        sum = sum + coeffs[i];
        i = i + 1;
    }
    return sum;
}

int poly_eval_at_neg_one(int coeffs[], int degree) {
    int sum = 0;
    int sign = 1;
    if (degree % 2 == 1) sign = -1;
    int i = 0;
    while (i <= degree) {
        sum = sum + sign * coeffs[i];
        sign = 0 - sign;
        i = i + 1;
    }
    return sum;
}

int poly_leading_coeff(int coeffs[]) {
    return coeffs[0];
}

int poly_scale(int coeffs[], int degree, int s,
               int out[]) {
    int i = 0;
    while (i <= degree) {
        out[i] = coeffs[i] * s;
        i = i + 1;
    }
    return degree;
}

int poly_compose_linear(int coeffs[], int degree,
                        int a, int b) {
    return horner(coeffs, degree, a + b);
}

int second_derivative_at(int coeffs[], int degree, int x) {
    int d1[10];
    int nd1[1];
    poly_derivative(coeffs, degree, d1, nd1);
    int d2[10];
    int nd2[1];
    poly_derivative(d1, nd1[0], d2, nd2);
    return horner(d2, nd2[0], x);
}

int main(void) {
    int p1[4];
    p1[0] = 2; p1[1] = -3; p1[2] = 0; p1[3] = 5;

    // EXPECT: horner(p1,3) = 32
    printf("horner(p1,3) = %d\n", horner(p1, 3, 3));
    // EXPECT: horner(p1,0) = 5
    printf("horner(p1,0) = %d\n", horner(p1, 3, 0));
    // EXPECT: horner(p1,1) = 4
    printf("horner(p1,1) = %d\n", horner(p1, 3, 1));
    // EXPECT: horner(p1,-1) = 0
    printf("horner(p1,-1) = %d\n", horner(p1, 3, -1));

    // EXPECT: naive(p1,3) = 32
    printf("naive(p1,3) = %d\n", poly_eval_naive(p1, 3, 3));
    // EXPECT: naive(p1,2) = 9
    printf("naive(p1,2) = %d\n", poly_eval_naive(p1, 3, 2));

    int p2[3];
    p2[0] = 1; p2[1] = 1; p2[2] = 1;
    int sum_c[5];
    int dc[1];
    poly_add(p1, 3, p2, 2, sum_c, dc);
    // EXPECT: add_deg = 3
    printf("add_deg = %d\n", dc[0]);
    // EXPECT: add[0] = 2
    printf("add[0] = %d\n", sum_c[0]);
    // EXPECT: add[1] = -2
    printf("add[1] = %d\n", sum_c[1]);

    int deriv[10];
    int nd[1];
    poly_derivative(p1, 3, deriv, nd);
    // EXPECT: d_deg = 2
    printf("d_deg = %d\n", nd[0]);
    // EXPECT: d[0] = 6
    printf("d[0] = %d\n", deriv[0]);
    // EXPECT: d[1] = -6
    printf("d[1] = %d\n", deriv[1]);
    // EXPECT: d[2] = 0
    printf("d[2] = %d\n", deriv[2]);
    // EXPECT: d_eval(2) = 12
    printf("d_eval(2) = %d\n", horner(deriv, nd[0], 2));

    // EXPECT: at_zero = 5
    printf("at_zero = %d\n", poly_eval_at_zero(p1, 3));
    // EXPECT: at_one = 4
    printf("at_one = %d\n", poly_eval_at_one(p1, 3));
    // EXPECT: at_neg1 = 0
    printf("at_neg1 = %d\n", poly_eval_at_neg_one(p1, 3));

    int scaled[10];
    poly_scale(p1, 3, 3, scaled);
    // EXPECT: scale[0] = 6
    printf("scale[0] = %d\n", scaled[0]);
    // EXPECT: scale[3] = 15
    printf("scale[3] = %d\n", scaled[3]);

    // EXPECT: d2_at(1) = 6
    printf("d2_at(1) = %d\n", second_derivative_at(p1, 3, 1));

    return 0;
}
