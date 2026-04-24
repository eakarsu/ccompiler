int printf(const char *fmt, ...);

// PID Controller (integer-based)
// Simulates a PID controller tracking a setpoint

struct PIDController {
    int kp;          // proportional gain (x100)
    int ki;          // integral gain (x100)
    int kd;          // derivative gain (x100)
    int setpoint;
    int integral;
    int prev_error;
    int output;
    int min_output;
    int max_output;
};

void pid_init(struct PIDController *pid, int kp, int ki, int kd, int setpoint) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output = 0;
    pid->min_output = -1000;
    pid->max_output = 1000;
}

int pid_clamp(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

int pid_update(struct PIDController *pid, int current_value) {
    int error;
    int p_term;
    int i_term;
    int d_term;
    int derivative;
    int output;

    error = pid->setpoint - current_value;
    pid->integral = pid->integral + error;
    pid->integral = pid_clamp(pid->integral, -5000, 5000);

    derivative = error - pid->prev_error;

    p_term = (pid->kp * error) / 100;
    i_term = (pid->ki * pid->integral) / 100;
    d_term = (pid->kd * derivative) / 100;

    output = p_term + i_term + d_term;
    output = pid_clamp(output, pid->min_output, pid->max_output);

    pid->prev_error = error;
    pid->output = output;
    return output;
}

int simulate_plant(int current, int control_input) {
    return current + control_input / 10;
}

void run_pid_simulation(struct PIDController *pid, int initial, int steps) {
    int current;
    int output;
    int i;

    current = initial;
    printf("PID Simulation: setpoint=%d, start=%d\n", pid->setpoint, initial);

    for (i = 0; i < steps; i = i + 1) {
        output = pid_update(pid, current);
        current = simulate_plant(current, output);
        if (i == 0 || i == 2 || i == 4 || i == 9) {
            printf("  Step %d: current=%d, output=%d, error=%d\n", i, current, output, pid->prev_error);
        }
    }
}

int main() {
    struct PIDController pid;
    int val;
    int out;

    printf("=== PID Basic Test ===\n");
    // EXPECT: === PID Basic Test ===

    pid_init(&pid, 50, 10, 20, 100);

    printf("Initial: kp=%d, ki=%d, kd=%d\n", pid.kp, pid.ki, pid.kd);
    // EXPECT: Initial: kp=50, ki=10, kd=20

    val = 0;
    out = pid_update(&pid, val);
    printf("Update 1: error=100, output=%d\n", out);
    // EXPECT: Update 1: error=100, output=80

    val = 70;
    out = pid_update(&pid, val);
    printf("Update 2: error=30, output=%d\n", out);
    // EXPECT: Update 2: error=30, output=14

    // --- Convergence test ---
    {
        struct PIDController pid2;
        int current;
        int i;
        int output;
        int settled;

        printf("=== PID Convergence Test ===\n");
        // EXPECT: === PID Convergence Test ===

        pid_init(&pid2, 80, 5, 30, 50);
        current = 0;
        settled = 0;

        for (i = 0; i < 30; i = i + 1) {
            output = pid_update(&pid2, current);
            current = simulate_plant(current, output);
            if (current >= 45 && current <= 55 && settled == 0) {
                printf("Settled near setpoint at step %d, value=%d\n", i, current);
                // EXPECT: Settled near setpoint at step 15, value=46
                settled = 1;
            }
        }
        printf("Final value: %d (setpoint=50)\n", current);
        // EXPECT: Final value: 62 (setpoint=50)
    }

    // --- Full simulation ---
    pid_init(&pid, 60, 15, 25, 100);
    run_pid_simulation(&pid, 0, 10);
    // EXPECT: PID Simulation: setpoint=100, start=0
    // EXPECT:   Step 0: current=10, output=100, error=100
    // EXPECT:   Step 2: current=26, output=87, error=82
    // EXPECT:   Step 4: current=44, output=98, error=65
    // EXPECT:   Step 9: current=92, output=96, error=17

    printf("Clamp test: %d\n", pid_clamp(500, -100, 100));
    // EXPECT: Clamp test: 100
    printf("Clamp test: %d\n", pid_clamp(-500, -100, 100));
    // EXPECT: Clamp test: -100
    printf("Clamp test: %d\n", pid_clamp(50, -100, 100));
    // EXPECT: Clamp test: 50

    printf("PID test done\n");
    // EXPECT: PID test done

    return 0;
}
