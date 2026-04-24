int printf(const char *fmt, ...);

/* PID controller simulation (integer-scaled)
 * All values scaled by 1000 to avoid floating point.
 */

typedef struct {
    int kp;       /* proportional gain * 1000 */
    int ki;       /* integral gain * 1000 */
    int kd;       /* derivative gain * 1000 */
    int setpoint; /* target * 1000 */
    int integral; /* accumulated error */
    int prev_err; /* previous error */
} PIDController;

void pid_init(PIDController *pid, int kp, int ki, int kd, int sp) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = sp;
    pid->integral = 0;
    pid->prev_err = 0;
}

/* Returns control output scaled by 1000 */
int pid_update(PIDController *pid, int measured) {
    int error;
    int derivative;
    int output;

    error = pid->setpoint - measured;
    pid->integral = pid->integral + error;

    /* Clamp integral to prevent windup */
    if (pid->integral > 50000) pid->integral = 50000;
    if (pid->integral < -50000) pid->integral = -50000;

    derivative = error - pid->prev_err;
    pid->prev_err = error;

    output = (pid->kp * error) / 1000
           + (pid->ki * pid->integral) / 1000
           + (pid->kd * derivative) / 1000;

    return output;
}

/* Simple plant: new = old + control / 2 */
int plant_step(int current, int control) {
    return current + control / 2;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int main(void) {
    PIDController pid;
    int plant_val;
    int control;
    int i;
    int step_vals[20];
    int max_val;

    pid_init(&pid, 500, 100, 200, 10000);
    plant_val = 0;

    printf("PID Controller Simulation\n");
    // EXPECT: PID Controller Simulation

    /* Run 20 steps */
    i = 0;
    while (i < 20) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        step_vals[i] = plant_val;
        i = i + 1;
    }

    printf("Step 1: %d\n", step_vals[0]);
    // EXPECT: Step 1: 4000
    printf("Step 5: %d\n", step_vals[4]);
    // EXPECT: Step 5: 10435
    printf("Step 10: %d\n", step_vals[9]);
    // EXPECT: Step 10: 12647
    printf("Step 15: %d\n", step_vals[14]);
    // EXPECT: Step 15: 11566
    printf("Step 20: %d\n", step_vals[19]);
    // EXPECT: Step 20: 10265

    /* Test with different gains */
    pid_init(&pid, 800, 50, 300, 5000);
    plant_val = 0;

    i = 0;
    while (i < 10) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        i = i + 1;
    }

    printf("New setpoint result: %d\n", plant_val);
    // EXPECT: New setpoint result: 5475

    /* Anti-windup test */
    pid_init(&pid, 100, 500, 50, 100000);
    plant_val = 0;

    i = 0;
    while (i < 30) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        i = i + 1;
    }

    printf("Windup integral: %d\n", pid.integral);
    // EXPECT: Windup integral: 13325

    /* Negative setpoint test */
    pid_init(&pid, 500, 100, 200, -5000);
    plant_val = 0;

    i = 0;
    while (i < 15) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        i = i + 1;
    }

    printf("Negative setpoint: %d\n", plant_val);
    // EXPECT: Negative setpoint: -5783

    /* Steady state: run many steps with strong integral */
    pid_init(&pid, 1000, 200, 100, 8000);
    plant_val = 0;

    i = 0;
    while (i < 50) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        i = i + 1;
    }

    printf("Steady state: %d\n", plant_val);
    // EXPECT: Steady state: 8000

    /* Overshoot check: high P, no I, low D */
    pid_init(&pid, 900, 0, 100, 3000);
    plant_val = 0;

    max_val = 0;
    i = 0;
    while (i < 20) {
        control = pid_update(&pid, plant_val);
        plant_val = plant_step(plant_val, control);
        if (plant_val > max_val) max_val = plant_val;
        i = i + 1;
    }

    printf("Overshoot max: %d\n", max_val);
    // EXPECT: Overshoot max: 2998

    printf("Final value: %d\n", plant_val);
    // EXPECT: Final value: 2998

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
