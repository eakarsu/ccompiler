int printf(const char *fmt, ...);

// Inverse Kinematics: 2-link planar arm using integer trig lookup tables
// Angles in degrees (0-360), trig values scaled by 1000 (sin(30)=500, cos(0)=1000)

// Sine lookup table for 0..90 degrees, scaled by 1000
int sin_table[91];
int cos_table[91];

void init_trig_tables(void) {
    // Approximate sine values for 0-90 degrees, scaled by 1000
    // Using a polynomial approximation: sin(x) ~ x * (1000 - x*x/600) / 57
    // But we'll just hardcode key values and interpolate
    int key_sins[10];
    key_sins[0] = 0;     // sin(0)
    key_sins[1] = 174;   // sin(10)
    key_sins[2] = 342;   // sin(20)
    key_sins[3] = 500;   // sin(30)
    key_sins[4] = 643;   // sin(40)
    key_sins[5] = 766;   // sin(50)
    key_sins[6] = 866;   // sin(60)
    key_sins[7] = 940;   // sin(70)
    key_sins[8] = 985;   // sin(80)
    key_sins[9] = 1000;  // sin(90)

    int i;
    for (i = 0; i <= 90; i++) {
        int bucket = i / 10;
        int frac = i - bucket * 10;
        int s0 = key_sins[bucket];
        int s1;
        if (bucket < 9) {
            s1 = key_sins[bucket + 1];
        } else {
            s1 = 1000;
        }
        sin_table[i] = s0 + (s1 - s0) * frac / 10;
    }

    for (i = 0; i <= 90; i++) {
        cos_table[i] = sin_table[90 - i];
    }
}

// Get sin(angle) * 1000, angle in degrees (0-359)
int isin(int angle) {
    while (angle < 0) angle = angle + 360;
    while (angle >= 360) angle = angle - 360;

    if (angle <= 90) return sin_table[angle];
    if (angle <= 180) return sin_table[180 - angle];
    if (angle <= 270) return -sin_table[angle - 180];
    return -sin_table[360 - angle];
}

// Get cos(angle) * 1000
int icos(int angle) {
    return isin(angle + 90);
}

// Integer square root (Newton's method)
int isqrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

// Integer atan2 approximation, returns degrees
// Uses octant-based approximation
int iatan2(int y, int x) {
    if (x == 0 && y == 0) return 0;
    if (x == 0) {
        if (y > 0) return 90;
        return 270;
    }
    if (y == 0) {
        if (x > 0) return 0;
        return 180;
    }

    int ax = x; if (ax < 0) ax = -ax;
    int ay = y; if (ay < 0) ay = -ay;

    // Approximate atan(ay/ax) in degrees when ay <= ax
    int angle;
    if (ay <= ax) {
        angle = 45 * ay / ax;
    } else {
        angle = 90 - 45 * ax / ay;
    }

    // Map to correct quadrant
    if (x > 0 && y >= 0) return angle;
    if (x < 0 && y >= 0) return 180 - angle;
    if (x < 0 && y < 0) return 180 + angle;
    return 360 - angle;
}

struct ArmResult {
    int theta1;  // shoulder angle in degrees
    int theta2;  // elbow angle in degrees
    int valid;   // 1 if solution found
};

// Solve IK for 2-link arm
// link1, link2: link lengths (scaled by 100)
// tx, ty: target position (scaled by 100)
// Result angles in degrees
struct ArmResult solve_ik(int link1, int link2, int tx, int ty) {
    struct ArmResult res;
    res.theta1 = 0;
    res.theta2 = 0;
    res.valid = 0;

    // Distance to target squared (scaled by 100*100 = 10000)
    int dist_sq = tx * tx + ty * ty;
    int dist = isqrt(dist_sq);

    // Check reachability
    int max_reach = link1 + link2;
    int min_reach = link1 - link2;
    if (min_reach < 0) min_reach = -min_reach;

    if (dist > max_reach || dist < min_reach) {
        return res;
    }

    // cos(theta2) = (dist^2 - L1^2 - L2^2) / (2 * L1 * L2)
    // scaled: numerator and denominator both scaled by 10000
    int num = dist_sq - link1 * link1 - link2 * link2;
    int den = 2 * link1 * link2;

    // cos_t2 scaled by 1000
    int cos_t2;
    if (den != 0) {
        cos_t2 = num * 1000 / den;
    } else {
        return res;
    }

    // Clamp cos_t2 to [-1000, 1000]
    if (cos_t2 > 1000) cos_t2 = 1000;
    if (cos_t2 < -1000) cos_t2 = -1000;

    // sin_t2 from cos_t2: sin = sqrt(1 - cos^2), scaled by 1000
    int sin_t2_sq = 1000000 - cos_t2 * cos_t2 / 1;
    if (sin_t2_sq < 0) sin_t2_sq = 0;
    int sin_t2 = isqrt(sin_t2_sq);

    // theta2 from atan2
    res.theta2 = iatan2(sin_t2, cos_t2);

    // theta1 = atan2(ty, tx) - atan2(L2*sin(t2), L1 + L2*cos(t2))
    int alpha = iatan2(ty, tx);
    int k1 = link1 + link2 * cos_t2 / 1000;
    int k2 = link2 * sin_t2 / 1000;
    int beta = iatan2(k2, k1);
    res.theta1 = alpha - beta;
    if (res.theta1 < 0) res.theta1 = res.theta1 + 360;

    res.valid = 1;
    return res;
}

// Forward kinematics: compute end effector position
struct Point {
    int x;
    int y;
};

struct Point forward_kin(int link1, int link2, int theta1, int theta2) {
    struct Point p;
    // End effector x = L1*cos(t1) + L2*cos(t1+t2)
    // End effector y = L1*sin(t1) + L2*sin(t1+t2)
    int t1_cos = icos(theta1);
    int t1_sin = isin(theta1);
    int t12 = theta1 + theta2;
    int t12_cos = icos(t12);
    int t12_sin = isin(t12);

    p.x = link1 * t1_cos / 1000 + link2 * t12_cos / 1000;
    p.y = link1 * t1_sin / 1000 + link2 * t12_sin / 1000;
    return p;
}

int main(void) {
    init_trig_tables();

    printf("=== Trig Table Tests ===\n");                         // EXPECT: === Trig Table Tests ===
    printf("sin(0)=%d\n", isin(0));                               // EXPECT: sin(0)=0
    printf("sin(30)=%d\n", isin(30));                             // EXPECT: sin(30)=500
    printf("sin(90)=%d\n", isin(90));                             // EXPECT: sin(90)=1000
    printf("cos(0)=%d\n", icos(0));                               // EXPECT: cos(0)=1000
    printf("cos(90)=%d\n", icos(90));                             // EXPECT: cos(90)=0
    printf("sin(180)=%d\n", isin(180));                           // EXPECT: sin(180)=0
    printf("sin(270)=%d\n", isin(270));                           // EXPECT: sin(270)=-1000

    printf("=== Integer Sqrt ===\n");                             // EXPECT: === Integer Sqrt ===
    printf("sqrt(0)=%d\n", isqrt(0));                             // EXPECT: sqrt(0)=0
    printf("sqrt(100)=%d\n", isqrt(100));                         // EXPECT: sqrt(100)=10
    printf("sqrt(144)=%d\n", isqrt(144));                         // EXPECT: sqrt(144)=12
    printf("sqrt(10000)=%d\n", isqrt(10000));                     // EXPECT: sqrt(10000)=100

    printf("=== Integer Atan2 ===\n");                            // EXPECT: === Integer Atan2 ===
    printf("atan2(0,100)=%d\n", iatan2(0, 100));                  // EXPECT: atan2(0,100)=0
    printf("atan2(100,0)=%d\n", iatan2(100, 0));                  // EXPECT: atan2(100,0)=90
    printf("atan2(0,-100)=%d\n", iatan2(0, -100));                // EXPECT: atan2(0,-100)=180
    printf("atan2(-100,0)=%d\n", iatan2(-100, 0));                // EXPECT: atan2(-100,0)=270
    printf("atan2(100,100)=%d\n", iatan2(100, 100));              // EXPECT: atan2(100,100)=45

    printf("=== Forward Kinematics ===\n");                       // EXPECT: === Forward Kinematics ===
    // Arm with L1=100, L2=100, both at 0 degrees -> end at (200, 0)
    struct Point p1 = forward_kin(100, 100, 0, 0);
    printf("FK(0,0): x=%d y=%d\n", p1.x, p1.y);                  // EXPECT: FK(0,0): x=200 y=0

    // L1=100, L2=100, theta1=90, theta2=0 -> end at (0, 200)
    struct Point p2 = forward_kin(100, 100, 90, 0);
    printf("FK(90,0): x=%d y=%d\n", p2.x, p2.y);                 // EXPECT: FK(90,0): x=0 y=200

    // L1=100, L2=100, theta1=0, theta2=90 -> end at (100, 100)
    struct Point p3 = forward_kin(100, 100, 0, 90);
    printf("FK(0,90): x=%d y=%d\n", p3.x, p3.y);                 // EXPECT: FK(0,90): x=100 y=100

    printf("=== Inverse Kinematics ===\n");                       // EXPECT: === Inverse Kinematics ===

    // Target at (200, 0): arm fully extended
    struct ArmResult r1 = solve_ik(100, 100, 200, 0);
    printf("IK(200,0): valid=%d t1=%d t2=%d\n", r1.valid, r1.theta1, r1.theta2); // EXPECT: IK(200,0): valid=1 t1=0 t2=0

    // Unreachable target
    struct ArmResult r2 = solve_ik(100, 100, 300, 0);
    printf("IK(300,0): valid=%d\n", r2.valid);                    // EXPECT: IK(300,0): valid=0

    // Target at (0, 200): arm fully extended upward
    struct ArmResult r3 = solve_ik(100, 100, 0, 200);
    printf("IK(0,200): valid=%d t1=%d t2=%d\n", r3.valid, r3.theta1, r3.theta2); // EXPECT: IK(0,200): valid=1 t1=90 t2=0

    // Target at (100, 100): L1=100 L2=100
    struct ArmResult r4 = solve_ik(100, 100, 100, 100);
    printf("IK(100,100): valid=%d\n", r4.valid);                  // EXPECT: IK(100,100): valid=1
    // Verify with forward kinematics
    struct Point p4 = forward_kin(100, 100, r4.theta1, r4.theta2);
    int err_x = p4.x - 100; if (err_x < 0) err_x = -err_x;
    int err_y = p4.y - 100; if (err_y < 0) err_y = -err_y;
    int fk_ok = (err_x <= 15 && err_y <= 15) ? 1 : 0;
    printf("FK verify: ok=%d\n", fk_ok);                          // EXPECT: FK verify: ok=1

    // Test multiple targets in workspace
    printf("=== Workspace Scan ===\n");                           // EXPECT: === Workspace Scan ===
    int reachable = 0;
    int unreachable = 0;
    int test_targets[5];
    test_targets[0] = 50;
    test_targets[1] = 100;
    test_targets[2] = 150;
    test_targets[3] = 180;
    test_targets[4] = 210;
    int j;
    for (j = 0; j < 5; j++) {
        struct ArmResult rt = solve_ik(100, 100, test_targets[j], 0);
        if (rt.valid) {
            reachable = reachable + 1;
        } else {
            unreachable = unreachable + 1;
        }
    }
    printf("reachable=%d unreachable=%d\n", reachable, unreachable); // EXPECT: reachable=4 unreachable=1

    printf("PASS\n");                                             // EXPECT: PASS
    return 0;
}
