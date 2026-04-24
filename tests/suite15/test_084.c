int printf(const char *fmt, ...);

/* 1D Kalman filter (integer-scaled)
 * All values scaled by 1000.
 * State: position estimate
 * Measurement noise R, process noise Q
 */

typedef struct {
    int x;     /* state estimate * 1000 */
    int p;     /* error covariance * 1000 */
    int q;     /* process noise * 1000 */
    int r;     /* measurement noise * 1000 */
    int k;     /* kalman gain * 1000 */
} KalmanFilter;

void kf_init(KalmanFilter *kf, int x0, int p0, int q, int r) {
    kf->x = x0;
    kf->p = p0;
    kf->q = q;
    kf->r = r;
    kf->k = 0;
}

/* Predict step: x stays same (constant model), p increases by q */
void kf_predict(KalmanFilter *kf) {
    kf->p = kf->p + kf->q;
}

/* Update step with measurement z (scaled by 1000) */
void kf_update(KalmanFilter *kf, int z) {
    int denom;
    denom = kf->p + kf->r;
    if (denom == 0) {
        kf->k = 500;
    } else {
        kf->k = kf->p * 1000 / denom;
    }

    kf->x = kf->x + kf->k * (z - kf->x) / 1000;
    kf->p = (1000 - kf->k) * kf->p / 1000;
}

/* Run one full step: predict then update */
void kf_step(KalmanFilter *kf, int z) {
    kf_predict(kf);
    kf_update(kf, z);
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int main(void) {
    KalmanFilter kf;
    int measurements[10];
    int i;
    int estimates[10];
    int gains[10];

    printf("Kalman Filter Tests\n");
    // EXPECT: Kalman Filter Tests

    /* Test 1: Filter noisy measurements of constant value (true = 5000) */
    kf_init(&kf, 0, 1000, 100, 500);

    measurements[0] = 4800;
    measurements[1] = 5200;
    measurements[2] = 4900;
    measurements[3] = 5100;
    measurements[4] = 5000;
    measurements[5] = 4700;
    measurements[6] = 5300;
    measurements[7] = 5000;
    measurements[8] = 4900;
    measurements[9] = 5100;

    i = 0;
    while (i < 10) {
        kf_step(&kf, measurements[i]);
        estimates[i] = kf.x;
        gains[i] = kf.k;
        i = i + 1;
    }

    printf("Est 1: %d\n", estimates[0]);
    // EXPECT: Est 1: 3297
    printf("Est 3: %d\n", estimates[2]);
    // EXPECT: Est 3: 4475
    printf("Est 5: %d\n", estimates[4]);
    // EXPECT: Est 5: 4814
    printf("Est 10: %d\n", estimates[9]);
    // EXPECT: Est 10: 5002

    printf("Gain 1: %d\n", gains[0]);
    // EXPECT: Gain 1: 687
    printf("Gain 5: %d\n", gains[4]);
    // EXPECT: Gain 5: 364
    printf("Gain 10: %d\n", gains[9]);
    // EXPECT: Gain 10: 358

    printf("Final P: %d\n", kf.p);
    // EXPECT: Final P: 179

    /* Test 2: High measurement noise - should trust prediction more */
    kf_init(&kf, 3000, 500, 50, 5000);

    i = 0;
    while (i < 5) {
        kf_step(&kf, 6000);
        i = i + 1;
    }

    printf("High R est: %d\n", kf.x);
    // EXPECT: High R est: 4201
    printf("High R gain: %d\n", kf.k);
    // EXPECT: High R gain: 96

    /* Test 3: Low measurement noise - should trust measurements more */
    kf_init(&kf, 3000, 500, 50, 100);

    i = 0;
    while (i < 5) {
        kf_step(&kf, 6000);
        i = i + 1;
    }

    printf("Low R est: %d\n", kf.x);
    // EXPECT: Low R est: 5976
    printf("Low R gain: %d\n", kf.k);
    // EXPECT: Low R gain: 500

    /* Test 4: Step change - true value jumps from 1000 to 8000 */
    kf_init(&kf, 1000, 200, 100, 300);

    i = 0;
    while (i < 5) {
        kf_step(&kf, 1000);
        i = i + 1;
    }

    printf("Before jump: %d\n", kf.x);
    // EXPECT: Before jump: 1000

    i = 0;
    while (i < 5) {
        kf_step(&kf, 8000);
        i = i + 1;
    }

    printf("After jump: %d\n", kf.x);
    // EXPECT: After jump: 7588

    /* Test 5: Zero process noise */
    kf_init(&kf, 0, 10000, 0, 1000);

    i = 0;
    while (i < 8) {
        kf_step(&kf, 4000);
        i = i + 1;
    }

    printf("Zero Q est: %d\n", kf.x);
    // EXPECT: Zero Q est: 3949

    printf("Zero Q P: %d\n", kf.p);
    // EXPECT: Zero Q P: 122

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
