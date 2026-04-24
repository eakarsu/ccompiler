int printf(const char *fmt, ...);

/* Fuzzy logic controller (integer membership functions)
 * Controls a heater based on temperature error and error rate.
 * All membership values 0-1000 (representing 0.0 to 1.0).
 * Output: heater power 0-1000.
 */

/* Triangular membership function
 * Returns membership value 0-1000
 * a: left foot, b: peak, c: right foot
 */
int triangle_mf(int x, int a, int b, int c) {
    if (x <= a) return 0;
    if (x >= c) return 0;
    if (x <= b) {
        if (b == a) return 1000;
        return (x - a) * 1000 / (b - a);
    }
    /* x > b */
    if (c == b) return 1000;
    return (c - x) * 1000 / (c - b);
}

/* Trapezoidal membership function
 * a: left foot, b: left shoulder, c: right shoulder, d: right foot
 */
int trapezoid_mf(int x, int a, int b, int c, int d) {
    if (x <= a) return 0;
    if (x >= d) return 0;
    if (x >= b && x <= c) return 1000;
    if (x < b) {
        if (b == a) return 1000;
        return (x - a) * 1000 / (b - a);
    }
    /* x > c */
    if (d == c) return 1000;
    return (d - x) * 1000 / (d - c);
}

/* Min of two values (fuzzy AND) */
int fuzzy_min(int a, int b) {
    if (a < b) return a;
    return b;
}

/* Max of two values (fuzzy OR) */
int fuzzy_max(int a, int b) {
    if (a > b) return a;
    return b;
}

/* Fuzzy temperature error membership functions
 * Error range: -100 to 100 (scaled: degrees * 10)
 * NL: Negative Large
 * NS: Negative Small
 * ZE: Zero
 * PS: Positive Small
 * PL: Positive Large
 */
int error_NL(int e) { return trapezoid_mf(e, -200, -200, -80, -40); }
int error_NS(int e) { return triangle_mf(e, -60, -30, 0); }
int error_ZE(int e) { return triangle_mf(e, -20, 0, 20); }
int error_PS(int e) { return triangle_mf(e, 0, 30, 60); }
int error_PL(int e) { return trapezoid_mf(e, 40, 80, 200, 200); }

/* Fuzzy error rate membership functions
 * Rate range: -50 to 50
 */
int rate_NEG(int r) { return trapezoid_mf(r, -100, -100, -20, 0); }
int rate_ZER(int r) { return triangle_mf(r, -15, 0, 15); }
int rate_POS(int r) { return trapezoid_mf(r, 0, 20, 100, 100); }

/* Defuzzify using weighted average method
 * output_levels: centroid of each output fuzzy set * 1000
 * strengths: firing strength of each rule * 1000
 * n: number of rules
 * Returns output * 1000
 */
int defuzzify(int *levels, int *strengths, int n) {
    int num;
    int den;
    int i;
    num = 0;
    den = 0;
    i = 0;
    while (i < n) {
        num = num + levels[i] * strengths[i] / 1000;
        den = den + strengths[i];
        i = i + 1;
    }
    if (den == 0) return 500;  /* default to medium */
    return num * 1000 / den;
}

/* Fuzzy controller: given error and error rate, return heater power (0-1000) */
int fuzzy_control(int error, int rate) {
    int rule_strength[9];
    int rule_output[9];
    int e_nl;
    int e_ns;
    int e_ze;
    int e_ps;
    int e_pl;
    int r_neg;
    int r_zer;
    int r_pos;

    /* Fuzzify inputs */
    e_nl = error_NL(error);
    e_ns = error_NS(error);
    e_ze = error_ZE(error);
    e_ps = error_PS(error);
    e_pl = error_PL(error);

    r_neg = rate_NEG(rate);
    r_zer = rate_ZER(rate);
    r_pos = rate_POS(rate);

    /* Rule base (error, rate) -> output
     * Rule 0: IF error is NL AND rate is any THEN heat HIGH (900)
     * Rule 1: IF error is NS AND rate is NEG THEN heat HIGH (800)
     * Rule 2: IF error is NS AND rate is ZER THEN heat MED-HIGH (700)
     * Rule 3: IF error is ZE AND rate is NEG THEN heat MED-HIGH (650)
     * Rule 4: IF error is ZE AND rate is ZER THEN heat MED (500)
     * Rule 5: IF error is ZE AND rate is POS THEN heat MED-LOW (350)
     * Rule 6: IF error is PS AND rate is ZER THEN heat LOW (300)
     * Rule 7: IF error is PS AND rate is POS THEN heat LOW (200)
     * Rule 8: IF error is PL AND rate is any THEN heat OFF (100)
     */
    rule_output[0] = 900;
    rule_strength[0] = e_nl;  /* any rate for NL */

    rule_output[1] = 800;
    rule_strength[1] = fuzzy_min(e_ns, r_neg);

    rule_output[2] = 700;
    rule_strength[2] = fuzzy_min(e_ns, r_zer);

    rule_output[3] = 650;
    rule_strength[3] = fuzzy_min(e_ze, r_neg);

    rule_output[4] = 500;
    rule_strength[4] = fuzzy_min(e_ze, r_zer);

    rule_output[5] = 350;
    rule_strength[5] = fuzzy_min(e_ze, r_pos);

    rule_output[6] = 300;
    rule_strength[6] = fuzzy_min(e_ps, r_zer);

    rule_output[7] = 200;
    rule_strength[7] = fuzzy_min(e_ps, r_pos);

    rule_output[8] = 100;
    rule_strength[8] = e_pl;  /* any rate for PL */

    return defuzzify(rule_output, rule_strength, 9);
}

int main(void) {
    int output;

    printf("Fuzzy Logic Controller\n");
    // EXPECT: Fuzzy Logic Controller

    /* Test membership functions */
    printf("tri(-50,-60,-30,0): %d\n", triangle_mf(-50, -60, -30, 0));
    // EXPECT: tri(-50,-60,-30,0): 333
    printf("tri(0,-20,0,20): %d\n", triangle_mf(0, -20, 0, 20));
    // EXPECT: tri(0,-20,0,20): 1000
    printf("tri(10,-20,0,20): %d\n", triangle_mf(10, -20, 0, 20));
    // EXPECT: tri(10,-20,0,20): 500
    printf("trap(-100): %d\n", trapezoid_mf(-100, -200, -200, -80, -40));
    // EXPECT: trap(-100): 1000
    printf("trap(-60): %d\n", trapezoid_mf(-60, -200, -200, -80, -40));
    // EXPECT: trap(-60): 500

    /* Test controller: large negative error (too cold) -> high heat */
    output = fuzzy_control(-80, 0);
    printf("Cold (-80,0): %d\n", output);
    // EXPECT: Cold (-80,0): 900

    /* Test: slightly cold */
    output = fuzzy_control(-30, 0);
    printf("Slightly cold (-30,0): %d\n", output);
    // EXPECT: Slightly cold (-30,0): 700

    /* Test: at setpoint, stable */
    output = fuzzy_control(0, 0);
    printf("At setpoint (0,0): %d\n", output);
    // EXPECT: At setpoint (0,0): 500

    /* Test: slightly hot */
    output = fuzzy_control(30, 0);
    printf("Slightly hot (30,0): %d\n", output);
    // EXPECT: Slightly hot (30,0): 300

    /* Test: very hot -> low heat */
    output = fuzzy_control(80, 0);
    printf("Hot (80,0): %d\n", output);
    // EXPECT: Hot (80,0): 100

    /* Test: at setpoint but cooling fast -> increase heat */
    output = fuzzy_control(0, -30);
    printf("Cooling (0,-30): %d\n", output);
    // EXPECT: Cooling (0,-30): 650

    /* Test: at setpoint but warming fast -> decrease heat */
    output = fuzzy_control(0, 30);
    printf("Warming (0,30): %d\n", output);
    // EXPECT: Warming (0,30): 350

    /* Test: slightly cold and cooling -> more heat */
    output = fuzzy_control(-30, -30);
    printf("Cold+cooling (-30,-30): %d\n", output);
    // EXPECT: Cold+cooling (-30,-30): 800

    /* Test: slightly hot and warming -> less heat */
    output = fuzzy_control(30, 30);
    printf("Hot+warming (30,30): %d\n", output);
    // EXPECT: Hot+warming (30,30): 200

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
