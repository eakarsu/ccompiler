int printf(const char *fmt, ...);

// Queuing theory: M/M/1 queue simulation
// Deterministic interarrival and service times via LCG
// Track queue length, wait times, utilization

typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

// Generate interarrival time: 1 to max_val
int gen_interarrival(RNG *r, int max_val) {
    return 1 + rng_next(r) % max_val;
}

// Generate service time: 1 to max_val
int gen_service(RNG *r, int max_val) {
    return 1 + rng_next(r) % max_val;
}

typedef struct {
    int arrival_time;
    int service_start;
    int service_end;
    int wait_time;
} Customer;

// Simulate queue and fill customer records
int simulate_queue(RNG *arr_rng, RNG *svc_rng, Customer *custs, int n) {
    int i;
    int clock = 0;
    int server_free_at = 0;

    for (i = 0; i < n; i++) {
        int inter = gen_interarrival(arr_rng, 5);
        clock = clock + inter;
        custs[i].arrival_time = clock;

        if (clock >= server_free_at) {
            custs[i].service_start = clock;
        } else {
            custs[i].service_start = server_free_at;
        }
        custs[i].wait_time = custs[i].service_start - custs[i].arrival_time;

        int svc = gen_service(svc_rng, 4);
        custs[i].service_end = custs[i].service_start + svc;
        server_free_at = custs[i].service_end;
    }
    return clock; // total simulation time
}

int compute_avg_wait(Customer *custs, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + custs[i].wait_time;
    }
    return total / n;
}

int compute_max_wait(Customer *custs, int n) {
    int max = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (custs[i].wait_time > max) max = custs[i].wait_time;
    }
    return max;
}

// Count customers who waited > threshold
int count_waited(Customer *custs, int n, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (custs[i].wait_time > threshold) count++;
    }
    return count;
}

// Compute utilization: total service time / total sim time (x100 for percent)
int compute_utilization(Customer *custs, int n, int total_time) {
    int busy = 0;
    int i;
    for (i = 0; i < n; i++) {
        busy = busy + (custs[i].service_end - custs[i].service_start);
    }
    return busy * 100 / total_time;
}

// Find max queue length (count how many in system at each arrival)
int max_queue_length(Customer *custs, int n) {
    int max_q = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        int q = 0;
        for (j = 0; j < i; j++) {
            if (custs[j].service_end > custs[i].arrival_time) q++;
        }
        if (q > max_q) max_q = q;
    }
    return max_q;
}

int main(void) {
    RNG arr_rng;
    RNG svc_rng;
    Customer custs[20];

    rng_init(&arr_rng, 42);
    rng_init(&svc_rng, 99);

    int total_time = simulate_queue(&arr_rng, &svc_rng, custs, 20);

    printf("Customer 0: arr=%d start=%d end=%d wait=%d\n",
           custs[0].arrival_time, custs[0].service_start,
           custs[0].service_end, custs[0].wait_time);
    // EXPECT: Customer 0: arr=2 start=2 end=4 wait=0

    printf("Customer 1: arr=%d start=%d end=%d wait=%d\n",
           custs[1].arrival_time, custs[1].service_start,
           custs[1].service_end, custs[1].wait_time);
    // EXPECT: Customer 1: arr=3 start=4 end=7 wait=1

    printf("Customer 2: arr=%d start=%d end=%d wait=%d\n",
           custs[2].arrival_time, custs[2].service_start,
           custs[2].service_end, custs[2].wait_time);
    // EXPECT: Customer 2: arr=6 start=7 end=11 wait=1

    printf("Customer 5: arr=%d start=%d end=%d wait=%d\n",
           custs[5].arrival_time, custs[5].service_start,
           custs[5].service_end, custs[5].wait_time);
    // EXPECT: Customer 5: arr=16 start=16 end=20 wait=0

    printf("Total sim time: %d\n", total_time);
    // EXPECT: Total sim time: 59

    int avg_wait = compute_avg_wait(custs, 20);
    printf("Avg wait: %d\n", avg_wait);
    // EXPECT: Avg wait: 0

    int max_wait = compute_max_wait(custs, 20);
    printf("Max wait: %d\n", max_wait);
    // EXPECT: Max wait: 2

    int waited = count_waited(custs, 20, 0);
    printf("Customers who waited: %d\n", waited);
    // EXPECT: Customers who waited: 5

    int util = compute_utilization(custs, 20, total_time);
    printf("Utilization: %d\n", util);
    // EXPECT: Utilization: 64

    int maxq = max_queue_length(custs, 20);
    printf("Max queue length: %d\n", maxq);
    // EXPECT: Max queue length: 1

    // Second simulation with different seeds
    RNG arr2;
    RNG svc2;
    Customer custs2[20];
    rng_init(&arr2, 7);
    rng_init(&svc2, 13);

    int total2 = simulate_queue(&arr2, &svc2, custs2, 20);
    printf("Sim2 total time: %d\n", total2);
    // EXPECT: Sim2 total time: 63

    int avg2 = compute_avg_wait(custs2, 20);
    printf("Sim2 avg wait: %d\n", avg2);
    // EXPECT: Sim2 avg wait: 0

    int max2 = compute_max_wait(custs2, 20);
    printf("Sim2 max wait: %d\n", max2);
    // EXPECT: Sim2 max wait: 5

    int util2 = compute_utilization(custs2, 20, total2);
    printf("Sim2 utilization: %d\n", util2);
    // EXPECT: Sim2 utilization: 82

    return 0;
}
