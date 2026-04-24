int printf(const char *fmt, ...);

// SIR Epidemic Model (integer population, rates scaled by 1000)
// new_infected = beta * S / 1000 * I / N
// recovered = gamma * I / 1000
// Population = 1000, beta = 300 (0.3), gamma = 100 (0.1)

typedef struct {
    int s;
    int i;
    int r;
} SIRState;

void sir_init(SIRState *st, int pop, int initial_infected) {
    st->s = pop - initial_infected;
    st->i = initial_infected;
    st->r = 0;
}

void sir_step(SIRState *st, int beta, int gamma, int pop) {
    int new_inf = (beta * st->s / 1000) * st->i / pop;
    int recovered = gamma * st->i / 1000;

    if (new_inf > st->s) new_inf = st->s;
    if (recovered > st->i) recovered = st->i;

    st->s = st->s - new_inf;
    st->i = st->i + new_inf - recovered;
    st->r = st->r + recovered;

    if (st->s < 0) st->s = 0;
    if (st->i < 0) st->i = 0;
}

void run_sir(int *s_hist, int *i_hist, int *r_hist, int steps,
             int pop, int init_inf, int beta, int gamma) {
    SIRState st;
    sir_init(&st, pop, init_inf);
    int t;
    s_hist[0] = st.s;
    i_hist[0] = st.i;
    r_hist[0] = st.r;
    for (t = 1; t <= steps; t++) {
        sir_step(&st, beta, gamma, pop);
        s_hist[t] = st.s;
        i_hist[t] = st.i;
        r_hist[t] = st.r;
    }
}

int find_max(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

int find_peak_time(int *arr, int n) {
    int m = arr[0];
    int idx = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) {
            m = arr[i];
            idx = i;
        }
    }
    return idx;
}

int compute_r0_x10(int beta, int gamma) {
    return beta * 10 / gamma;
}

int main(void) {
    int pop = 1000;
    int init_inf = 10;
    int beta = 300;
    int gamma = 100;
    int steps = 50;

    int s_hist[51];
    int i_hist[51];
    int r_hist[51];

    run_sir(s_hist, i_hist, r_hist, steps, pop, init_inf, beta, gamma);

    printf("S0=%d I0=%d R0=%d\n", s_hist[0], i_hist[0], r_hist[0]);
    // EXPECT: S0=990 I0=10 R0=0

    printf("S1=%d I1=%d R1=%d\n", s_hist[1], i_hist[1], r_hist[1]);
    // EXPECT: S1=988 I1=11 R1=1

    printf("S10=%d I10=%d R10=%d\n", s_hist[10], i_hist[10], r_hist[10]);
    // EXPECT: S10=930 I10=51 R10=19

    printf("S20=%d I20=%d R20=%d\n", s_hist[20], i_hist[20], r_hist[20]);
    // EXPECT: S20=673 I20=203 R20=124

    printf("S30=%d I30=%d R30=%d\n", s_hist[30], i_hist[30], r_hist[30]);
    // EXPECT: S30=288 I30=317 R30=395

    printf("S40=%d I40=%d R40=%d\n", s_hist[40], i_hist[40], r_hist[40]);
    // EXPECT: S40=126 I40=208 R40=666

    printf("S50=%d I50=%d R50=%d\n", s_hist[50], i_hist[50], r_hist[50]);
    // EXPECT: S50=83 I50=101 R50=816

    int peak = find_max(i_hist, steps + 1);
    printf("Peak infected: %d\n", peak);
    // EXPECT: Peak infected: 318

    int pt = find_peak_time(i_hist, steps + 1);
    printf("Peak time: %d\n", pt);
    // EXPECT: Peak time: 29

    int r0 = compute_r0_x10(beta, gamma);
    printf("R0 x10: %d\n", r0);
    // EXPECT: R0 x10: 30

    int total = s_hist[50] + i_hist[50] + r_hist[50];
    printf("Total: %d\n", total);
    // EXPECT: Total: 1000

    // Scenario 2: more aggressive epidemic (beta=400, R0=4)
    int beta2 = 400;
    int s2[51];
    int i2[51];
    int r2[51];
    run_sir(s2, i2, r2, steps, pop, init_inf, beta2, gamma);

    int peak2 = find_max(i2, steps + 1);
    printf("Agg peak: %d\n", peak2);
    // EXPECT: Agg peak: 428

    int pt2 = find_peak_time(i2, steps + 1);
    printf("Agg peak time: %d\n", pt2);
    // EXPECT: Agg peak time: 21

    printf("Agg final R: %d\n", r2[50]);
    // EXPECT: Agg final R: 927

    // More aggressive = earlier, higher peak
    int earlier = (pt2 < pt) ? 1 : 0;
    printf("Earlier peak: %d\n", earlier);
    // EXPECT: Earlier peak: 1

    int higher = (peak2 > peak) ? 1 : 0;
    printf("Higher peak: %d\n", higher);
    // EXPECT: Higher peak: 1

    return 0;
}
