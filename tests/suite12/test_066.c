int printf(const char *fmt, ...);

// Servo Position Tracking
// Simulates servo motor with position feedback and PD control

struct Servo {
    int target_angle;    // target in tenths of degree (0-1800 = 0-180 deg)
    int current_angle;   // current in tenths of degree
    int speed;           // tenths of degree per tick
    int max_speed;
    int kp;              // proportional gain (x100)
    int kd;              // derivative gain (x100)
    int prev_error;
    int min_angle;
    int max_angle;
    int settled;
};

void servo_init(struct Servo *s, int min_a, int max_a, int max_spd) {
    s->target_angle = 0;
    s->current_angle = 0;
    s->speed = 0;
    s->max_speed = max_spd;
    s->kp = 200;
    s->kd = 50;
    s->prev_error = 0;
    s->min_angle = min_a;
    s->max_angle = max_a;
    s->settled = 1;
}

int servo_clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

void servo_set_target(struct Servo *s, int angle_tenths) {
    s->target_angle = servo_clamp(angle_tenths, s->min_angle, s->max_angle);
    s->settled = 0;
}

void servo_tick(struct Servo *s) {
    int error;
    int derivative;
    int control;
    int abs_err;

    error = s->target_angle - s->current_angle;
    derivative = error - s->prev_error;

    control = (s->kp * error + s->kd * derivative) / 100;
    control = servo_clamp(control, -s->max_speed, s->max_speed);

    s->current_angle = s->current_angle + control;
    s->current_angle = servo_clamp(s->current_angle, s->min_angle, s->max_angle);

    s->speed = control;
    s->prev_error = error;

    abs_err = error;
    if (abs_err < 0) abs_err = -abs_err;
    if (abs_err < 5) {
        s->settled = 1;
    }
}

int servo_degrees(struct Servo *s) {
    return s->current_angle / 10;
}

int servo_tenths(struct Servo *s) {
    return s->current_angle % 10;
}

// Multi-servo arm
struct ServoArm {
    struct Servo joint0;  // base rotation
    struct Servo joint1;  // shoulder
    struct Servo joint2;  // elbow
};

void arm_init(struct ServoArm *arm) {
    servo_init(&arm->joint0, 0, 3600, 100);  // 0-360 degrees
    servo_init(&arm->joint1, 0, 1800, 80);   // 0-180 degrees
    servo_init(&arm->joint2, 0, 1350, 60);   // 0-135 degrees
}

void arm_set_pose(struct ServoArm *arm, int a0, int a1, int a2) {
    servo_set_target(&arm->joint0, a0);
    servo_set_target(&arm->joint1, a1);
    servo_set_target(&arm->joint2, a2);
}

int arm_settled(struct ServoArm *arm) {
    return arm->joint0.settled && arm->joint1.settled && arm->joint2.settled;
}

void arm_tick(struct ServoArm *arm) {
    servo_tick(&arm->joint0);
    servo_tick(&arm->joint1);
    servo_tick(&arm->joint2);
}

int main() {
    struct Servo srv;
    struct ServoArm arm;
    int i;
    int ticks;

    printf("=== Servo Position Tracking Test ===\n");
    // EXPECT: === Servo Position Tracking Test ===

    // Test 1: Basic servo init
    servo_init(&srv, 0, 1800, 50);
    printf("Init: angle=%d, target=%d, settled=%d\n", srv.current_angle, srv.target_angle, srv.settled);
    // EXPECT: Init: angle=0, target=0, settled=1

    // Test 2: Move to 90 degrees (900 tenths)
    servo_set_target(&srv, 900);
    printf("Target set: %d tenths (%d deg)\n", srv.target_angle, srv.target_angle / 10);
    // EXPECT: Target set: 900 tenths (90 deg)

    for (i = 0; i < 20; i = i + 1) {
        servo_tick(&srv);
    }
    printf("After 20 ticks: angle=%d (deg=%d), settled=%d\n", srv.current_angle, servo_degrees(&srv), srv.settled);
    // EXPECT: After 20 ticks: angle=925 (deg=92), settled=1

    // Test 3: Clamping
    servo_init(&srv, 0, 1800, 50);
    servo_set_target(&srv, 2000);
    printf("Clamped target: %d (max=1800)\n", srv.target_angle);
    // EXPECT: Clamped target: 1800 (max=1800)

    servo_set_target(&srv, -100);
    printf("Clamped target: %d (min=0)\n", srv.target_angle);
    // EXPECT: Clamped target: 0 (min=0)

    // Test 4: Fast convergence from 0 to 450 (45 degrees)
    servo_init(&srv, 0, 1800, 200);
    servo_set_target(&srv, 450);
    ticks = 0;
    while (!srv.settled && ticks < 50) {
        servo_tick(&srv);
        ticks = ticks + 1;
    }
    printf("Converged to %d in %d ticks\n", srv.current_angle, ticks);
    // EXPECT: Converged to 550 in 50 ticks

    // Test 5: Servo arm
    arm_init(&arm);
    printf("Arm init: j0=%d, j1=%d, j2=%d\n",
           arm.joint0.current_angle, arm.joint1.current_angle, arm.joint2.current_angle);
    // EXPECT: Arm init: j0=0, j1=0, j2=0

    arm_set_pose(&arm, 900, 450, 300);
    printf("Arm target: j0=%d, j1=%d, j2=%d\n",
           arm.joint0.target_angle, arm.joint1.target_angle, arm.joint2.target_angle);
    // EXPECT: Arm target: j0=900, j1=450, j2=300

    for (i = 0; i < 30; i = i + 1) {
        arm_tick(&arm);
    }
    printf("Arm settled: %d\n", arm_settled(&arm));
    // EXPECT: Arm settled: 0

    printf("Arm pos: j0=%d, j1=%d, j2=%d\n",
           arm.joint0.current_angle, arm.joint1.current_angle, arm.joint2.current_angle);
    // EXPECT: Arm pos: j0=850, j1=490, j2=270

    // Test 6: Reverse movement
    servo_init(&srv, 0, 1800, 100);
    srv.current_angle = 900;
    servo_set_target(&srv, 200);
    for (i = 0; i < 15; i = i + 1) {
        servo_tick(&srv);
    }
    printf("Reverse: angle=%d (target=200)\n", srv.current_angle);
    // EXPECT: Reverse: angle=150 (target=200)

    printf("Servo tracking done\n");
    // EXPECT: Servo tracking done

    return 0;
}
