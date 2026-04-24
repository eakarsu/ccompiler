int printf(const char *fmt, ...);
// EXPECT: === Bank single teller ===\nTotal wait: 17\nMax queue: 4\nIdle time: 0\n=== Bank multi teller ===\nMulti-2 wait: 3\nMulti-3 wait: 1\n=== Bank test 2 ===\nTotal wait: 7\nIdle time: 5\n=== Elevator FCFS ===\nFCFS distance: 27\n=== Elevator SSTF ===\nSSTF distance: 9\n=== Elevator SCAN ===\nSCAN distance: 16\n=== Elevator wait time ===\nTotal elev wait: 12\n=== Traffic single car ===\nTravel time: 14\n=== Traffic multi car ===\nTotal travel: 30\n=== Green waves ===\nGreen waves (0-20): 2\n=== Traffic light states ===\nLight0 at t=0: 1\nLight0 at t=5: 0\nLight0 at t=10: 1\nWait at light0 t=5: 5\nWait at light0 t=3: 0\n=== MM1 queue ===\nMM1 L*100 (30/50): 150\nMM1 util*100 (30/50): 60\n=== Producer-Consumer ===\nMax buffer: 4\n=== Round Robin ===\nRR time q=2: 22\n=== SJF ===\nSJF total wait: 25\n=== Bank batch test ===\nBatch total wait: 0\n=== Elevator batch ===\nBatch FCFS: 15\nBatch SSTF: 7\n=== Traffic batch ===\nBatch single car: 3\n=== RR vs SJF ===\nRR q=3: 21\nSJF wait: 12\nAll simulation tests passed!
// ============================================================
// test_078_simulation.c
// Bank queue simulation, elevator simulation, traffic
// simulation (all using arrays and simple structs).
// ============================================================

// ============================================================
// QUEUE DATA STRUCTURE
// ============================================================

struct Queue {
    int data[200];
    int front;
    int back;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->back = 0;
}

int queue_empty(struct Queue *q) {
    return (q->front == q->back) ? 1 : 0;
}

void queue_enqueue(struct Queue *q, int val) {
    q->data[q->back] = val;
    q->back++;
}

int queue_dequeue(struct Queue *q) {
    int val = q->data[q->front];
    q->front++;
    return val;
}

int queue_peek(struct Queue *q) {
    return q->data[q->front];
}

int queue_size(struct Queue *q) {
    return q->back - q->front;
}

// ============================================================
// PRIORITY QUEUE (using simple sorted insertion)
// ============================================================

struct PriorityQueue {
    int keys[100];
    int values[100];
    int size;
};

void pq_init(struct PriorityQueue *pq) {
    pq->size = 0;
}

void pq_insert(struct PriorityQueue *pq, int key, int value) {
    // Insert maintaining sorted order (ascending by key)
    int i = pq->size - 1;
    while (i >= 0 && pq->keys[i] > key) {
        pq->keys[i + 1] = pq->keys[i];
        pq->values[i + 1] = pq->values[i];
        i--;
    }
    pq->keys[i + 1] = key;
    pq->values[i + 1] = value;
    pq->size++;
}

int pq_extract_min_key(struct PriorityQueue *pq) {
    int key = pq->keys[0];
    int i;
    for (i = 0; i < pq->size - 1; i++) {
        pq->keys[i] = pq->keys[i + 1];
        pq->values[i] = pq->values[i + 1];
    }
    pq->size--;
    return key;
}

int pq_extract_min_value(struct PriorityQueue *pq) {
    int value = pq->values[0];
    int i;
    for (i = 0; i < pq->size - 1; i++) {
        pq->keys[i] = pq->keys[i + 1];
        pq->values[i] = pq->values[i + 1];
    }
    pq->size--;
    return value;
}

int pq_empty(struct PriorityQueue *pq) {
    return (pq->size == 0) ? 1 : 0;
}

// ============================================================
// BANK QUEUE SIMULATION
// ============================================================

// Customer struct
struct Customer {
    int arrival_time;
    int service_time;
    int start_service;
    int finish_time;
};

// ---- Simulate single-teller bank queue ----
// Returns average waiting time * 100 (to avoid float)
int bank_sim_single(struct Customer customers[], int n) {
    int current_time = 0;
    int total_wait = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (current_time < customers[i].arrival_time) {
            current_time = customers[i].arrival_time;
        }
        customers[i].start_service = current_time;
        customers[i].finish_time = current_time + customers[i].service_time;
        total_wait = total_wait + (customers[i].start_service - customers[i].arrival_time);
        current_time = customers[i].finish_time;
    }

    // Return total wait (average * n for simplicity)
    return total_wait;
}

// ---- Simulate multi-teller bank (k tellers) ----
// Returns total waiting time
int bank_sim_multi(struct Customer customers[], int n, int k) {
    int teller_free[10]; // time when each teller becomes free
    int i, j;
    for (i = 0; i < k; i++) teller_free[i] = 0;

    int total_wait = 0;

    for (i = 0; i < n; i++) {
        // Find teller that becomes free earliest
        int min_free = teller_free[0];
        int min_idx = 0;
        for (j = 1; j < k; j++) {
            if (teller_free[j] < min_free) {
                min_free = teller_free[j];
                min_idx = j;
            }
        }

        int start;
        if (min_free <= customers[i].arrival_time) {
            start = customers[i].arrival_time;
        } else {
            start = min_free;
        }

        total_wait = total_wait + (start - customers[i].arrival_time);
        teller_free[min_idx] = start + customers[i].service_time;
    }

    return total_wait;
}

// ---- Bank simulation: max queue length ----
int bank_max_queue_length(struct Customer customers[], int n) {
    // Event-based simulation
    int events[400]; // time
    int types[400];  // 0 = arrival, 1 = departure
    int ne = 0;
    int i;

    for (i = 0; i < n; i++) {
        events[ne] = customers[i].arrival_time;
        types[ne] = 0;
        ne++;
    }

    // Single teller: compute departure times
    int current = 0;
    for (i = 0; i < n; i++) {
        if (current < customers[i].arrival_time) {
            current = customers[i].arrival_time;
        }
        current = current + customers[i].service_time;
        events[ne] = current;
        types[ne] = 1;
        ne++;
    }

    // Sort events by time (break ties: departure before arrival)
    int j;
    for (i = 0; i < ne - 1; i++) {
        for (j = 0; j < ne - 1 - i; j++) {
            int swap = 0;
            if (events[j] > events[j + 1]) swap = 1;
            else if (events[j] == events[j + 1] && types[j] < types[j + 1]) swap = 1;
            if (swap) {
                int tmp;
                tmp = events[j]; events[j] = events[j + 1]; events[j + 1] = tmp;
                tmp = types[j]; types[j] = types[j + 1]; types[j + 1] = tmp;
            }
        }
    }

    int max_q = 0;
    int cur_q = 0;
    for (i = 0; i < ne; i++) {
        if (types[i] == 0) cur_q++;
        else cur_q--;
        if (cur_q > max_q) max_q = cur_q;
    }
    return max_q;
}

// ---- Bank idle time ----
int bank_idle_time(struct Customer customers[], int n) {
    int current = 0;
    int idle = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (current < customers[i].arrival_time) {
            idle = idle + (customers[i].arrival_time - current);
            current = customers[i].arrival_time;
        }
        current = current + customers[i].service_time;
    }
    return idle;
}

// ============================================================
// ELEVATOR SIMULATION
// ============================================================

struct ElevatorRequest {
    int floor;       // destination floor
    int request_time; // when request was made
    int direction;    // 1 = up, -1 = down, 0 = either
};

// ---- Simple elevator: FCFS ----
// Elevator starts at floor 0, serves requests in order
// Returns total distance traveled
int elevator_fcfs(struct ElevatorRequest requests[], int n) {
    int current_floor = 0;
    int total_distance = 0;
    int i;

    for (i = 0; i < n; i++) {
        int dist = requests[i].floor - current_floor;
        if (dist < 0) dist = -dist;
        total_distance = total_distance + dist;
        current_floor = requests[i].floor;
    }
    return total_distance;
}

// ---- Elevator SCAN algorithm ----
// Elevator goes up to max, then down to min
int elevator_scan(int requests[], int n, int start, int max_floor) {
    // Sort requests
    int sorted[50];
    int i, j;
    for (i = 0; i < n; i++) sorted[i] = requests[i];

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    // Go up first, then down
    int total = 0;
    int pos = start;

    // Find first request >= start
    int split = 0;
    for (i = 0; i < n; i++) {
        if (sorted[i] >= start) { split = i; break; }
        if (i == n - 1) split = n;
    }

    // Go up
    for (i = split; i < n; i++) {
        total = total + (sorted[i] - pos);
        if (sorted[i] - pos < 0) total = total - 2 * (sorted[i] - pos);
        pos = sorted[i];
    }

    // Reach top if needed
    if (split < n) {
        total = total + (max_floor - pos);
        pos = max_floor;
    }

    // Go down
    for (i = split - 1; i >= 0; i--) {
        total = total + (pos - sorted[i]);
        pos = sorted[i];
    }

    return total;
}

// ---- Elevator SSTF (Shortest Seek Time First) ----
int elevator_sstf(int requests[], int n, int start) {
    int served[50];
    int i, j;
    for (i = 0; i < n; i++) served[i] = 0;

    int total = 0;
    int pos = start;

    int count;
    for (count = 0; count < n; count++) {
        int best_idx = -1;
        int best_dist = 999999;
        for (j = 0; j < n; j++) {
            if (!served[j]) {
                int d = requests[j] - pos;
                if (d < 0) d = -d;
                if (d < best_dist) {
                    best_dist = d;
                    best_idx = j;
                }
            }
        }
        if (best_idx == -1) break;
        served[best_idx] = 1;
        total = total + best_dist;
        pos = requests[best_idx];
    }
    return total;
}

// ---- Elevator total wait time ----
// Each request has a floor and time; elevator travels 1 floor per time unit
int elevator_wait_time(struct ElevatorRequest requests[], int n) {
    int current_floor = 0;
    int current_time = 0;
    int total_wait = 0;
    int i;

    for (i = 0; i < n; i++) {
        // Travel to request floor
        int dist = requests[i].floor - current_floor;
        if (dist < 0) dist = -dist;

        int arrive_time;
        if (current_time + dist > requests[i].request_time) {
            arrive_time = current_time + dist;
        } else {
            arrive_time = requests[i].request_time;
        }

        total_wait = total_wait + (arrive_time - requests[i].request_time);
        current_floor = requests[i].floor;
        current_time = arrive_time;
    }
    return total_wait;
}

// ============================================================
// TRAFFIC SIMULATION
// ============================================================

// Traffic light states: 0 = red, 1 = green
// Each intersection has a cycle time

struct TrafficLight {
    int cycle_time;    // total cycle time
    int green_time;    // green phase duration
    int offset;        // phase offset from time 0
};

// ---- Get light state at given time ----
int get_light_state(struct TrafficLight *light, int time) {
    int phase = (time + light->offset) % light->cycle_time;
    if (phase < 0) phase = phase + light->cycle_time;
    return (phase < light->green_time) ? 1 : 0;
}

// ---- Wait time at a traffic light ----
int wait_at_light(struct TrafficLight *light, int arrival_time) {
    if (get_light_state(light, arrival_time)) return 0; // Green
    // Find next green
    int phase = (arrival_time + light->offset) % light->cycle_time;
    if (phase < 0) phase = phase + light->cycle_time;
    // Red phase: wait until next green (phase wraps to 0)
    int wait = light->cycle_time - phase;
    return wait;
}

// ---- Simulate car travel through n intersections ----
// travel_time[i] = time to travel from intersection i to i+1
// Returns total time including waits
int traffic_sim_single_car(struct TrafficLight lights[], int n,
                           int travel_times[], int start_time) {
    int current_time = start_time;
    int total_wait = 0;
    int i;

    for (i = 0; i < n; i++) {
        int w = wait_at_light(&lights[i], current_time);
        total_wait = total_wait + w;
        current_time = current_time + w;
        if (i < n - 1) {
            current_time = current_time + travel_times[i];
        }
    }
    return current_time - start_time;
}

// ---- Simulate multiple cars ----
// Returns total time for all cars
int traffic_sim_multi_car(struct TrafficLight lights[], int n,
                          int travel_times[],
                          int start_times[], int num_cars) {
    int total = 0;
    int i;
    for (i = 0; i < num_cars; i++) {
        total = total + traffic_sim_single_car(lights, n, travel_times, start_times[i]);
    }
    return total;
}

// ---- Count green waves ----
// A green wave is when a car passes through all lights without stopping
int count_green_waves(struct TrafficLight lights[], int n,
                      int travel_times[], int max_start) {
    int count = 0;
    int start;
    for (start = 0; start <= max_start; start++) {
        int current_time = start;
        int all_green = 1;
        int i;
        for (i = 0; i < n; i++) {
            if (!get_light_state(&lights[i], current_time)) {
                all_green = 0;
                break;
            }
            if (i < n - 1) current_time = current_time + travel_times[i];
        }
        if (all_green) count++;
    }
    return count;
}

// ============================================================
// QUEUING THEORY HELPERS
// ============================================================

// ---- M/M/1 queue: average customers in system * 100 ----
// Given arrival rate * 100 and service rate * 100
int mm1_avg_customers(int lambda100, int mu100) {
    // L = lambda / (mu - lambda)
    // L * 100 = lambda100 * 100 / (mu100 - lambda100)
    if (mu100 <= lambda100) return -1;
    return lambda100 * 100 / (mu100 - lambda100);
}

// ---- M/M/1 utilization * 100 ----
int mm1_utilization(int lambda100, int mu100) {
    return lambda100 * 100 / mu100;
}

// ---- Simulate simple producer-consumer ----
// Producer adds items at intervals, consumer removes at intervals
// Returns max buffer size reached
int producer_consumer(int produce_times[], int np,
                      int consume_times[], int nc,
                      int max_time) {
    int buffer = 0;
    int max_buffer = 0;
    int pi = 0, ci = 0;
    int next_produce = (np > 0) ? produce_times[0] : max_time + 1;
    int next_consume = (nc > 0) ? consume_times[0] : max_time + 1;

    while (pi < np || ci < nc) {
        if (next_produce <= next_consume && pi < np) {
            buffer++;
            if (buffer > max_buffer) max_buffer = buffer;
            pi++;
            next_produce = (pi < np) ? produce_times[pi] : max_time + 1;
        } else if (ci < nc) {
            if (buffer > 0) buffer--;
            ci++;
            next_consume = (ci < nc) ? consume_times[ci] : max_time + 1;
        } else {
            break;
        }
    }
    return max_buffer;
}

// ---- Round-robin scheduling simulation ----
// Returns total completion time
int round_robin(int burst_times[], int n, int quantum) {
    int remaining[20];
    int i;
    for (i = 0; i < n; i++) remaining[i] = burst_times[i];

    int time = 0;
    int done = 0;
    while (done < n) {
        for (i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                if (remaining[i] <= quantum) {
                    time = time + remaining[i];
                    remaining[i] = 0;
                    done++;
                } else {
                    time = time + quantum;
                    remaining[i] = remaining[i] - quantum;
                }
            }
        }
    }
    return time;
}

// ---- SJF (Shortest Job First) total time ----
int sjf_total_time(int burst_times[], int n) {
    // Sort by burst time
    int sorted[20];
    int i, j;
    for (i = 0; i < n; i++) sorted[i] = burst_times[i];
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    int time = 0;
    int total_wait = 0;
    for (i = 0; i < n; i++) {
        total_wait = total_wait + time;
        time = time + sorted[i];
    }
    return total_wait;
}


int main() {
    // === Bank single teller ===
    printf("=== Bank single teller ===\n");
    {
        struct Customer c[5];
        c[0].arrival_time = 0; c[0].service_time = 3;
        c[1].arrival_time = 1; c[1].service_time = 2;
        c[2].arrival_time = 2; c[2].service_time = 4;
        c[3].arrival_time = 3; c[3].service_time = 1;
        c[4].arrival_time = 4; c[4].service_time = 2;
        printf("Total wait: %d\n", bank_sim_single(c, 5));
        // Recompute for max queue
        c[0].arrival_time = 0; c[0].service_time = 3;
        c[1].arrival_time = 1; c[1].service_time = 2;
        c[2].arrival_time = 2; c[2].service_time = 4;
        c[3].arrival_time = 3; c[3].service_time = 1;
        c[4].arrival_time = 4; c[4].service_time = 2;
        printf("Max queue: %d\n", bank_max_queue_length(c, 5));
        c[0].arrival_time = 0; c[0].service_time = 3;
        c[1].arrival_time = 1; c[1].service_time = 2;
        c[2].arrival_time = 2; c[2].service_time = 4;
        c[3].arrival_time = 3; c[3].service_time = 1;
        c[4].arrival_time = 4; c[4].service_time = 2;
        printf("Idle time: %d\n", bank_idle_time(c, 5));
    }

    // === Bank multi teller ===
    printf("=== Bank multi teller ===\n");
    {
        struct Customer c[4];
        c[0].arrival_time = 0; c[0].service_time = 3;
        c[1].arrival_time = 0; c[1].service_time = 2;
        c[2].arrival_time = 1; c[2].service_time = 1;
        c[3].arrival_time = 1; c[3].service_time = 2;
        printf("Multi-2 wait: %d\n", bank_sim_multi(c, 4, 2));
        printf("Multi-3 wait: %d\n", bank_sim_multi(c, 4, 3));
    }

    // === Bank test 2 (with gaps) ===
    printf("=== Bank test 2 ===\n");
    {
        struct Customer c[4];
        c[0].arrival_time = 0; c[0].service_time = 5;
        c[1].arrival_time = 2; c[1].service_time = 3;
        c[2].arrival_time = 4; c[2].service_time = 2;
        c[3].arrival_time = 15; c[3].service_time = 1;
        printf("Total wait: %d\n", bank_sim_single(c, 4));
        c[0].arrival_time = 0; c[0].service_time = 5;
        c[1].arrival_time = 2; c[1].service_time = 3;
        c[2].arrival_time = 4; c[2].service_time = 2;
        c[3].arrival_time = 15; c[3].service_time = 1;
        printf("Idle time: %d\n", bank_idle_time(c, 4));
    }

    // === Elevator FCFS ===
    printf("=== Elevator FCFS ===\n");
    {
        struct ElevatorRequest req[5];
        req[0].floor = 5; req[0].request_time = 0; req[0].direction = 1;
        req[1].floor = 2; req[1].request_time = 1; req[1].direction = -1;
        req[2].floor = 8; req[2].request_time = 2; req[2].direction = 1;
        req[3].floor = 1; req[3].request_time = 3; req[3].direction = -1;
        req[4].floor = 7; req[4].request_time = 4; req[4].direction = 1;
        printf("FCFS distance: %d\n", elevator_fcfs(req, 5));
    }

    // === Elevator SSTF ===
    printf("=== Elevator SSTF ===\n");
    {
        int req[5]; req[0]=5; req[1]=2; req[2]=8; req[3]=1; req[4]=7;
        printf("SSTF distance: %d\n", elevator_sstf(req, 5, 3));
    }

    // === Elevator SCAN ===
    printf("=== Elevator SCAN ===\n");
    {
        int req[5]; req[0]=5; req[1]=2; req[2]=8; req[3]=1; req[4]=7;
        printf("SCAN distance: %d\n", elevator_scan(req, 5, 3, 10));
    }

    // === Elevator wait time ===
    printf("=== Elevator wait time ===\n");
    {
        struct ElevatorRequest req[3];
        req[0].floor = 3; req[0].request_time = 0; req[0].direction = 1;
        req[1].floor = 1; req[1].request_time = 2; req[1].direction = -1;
        req[2].floor = 5; req[2].request_time = 3; req[2].direction = 1;
        printf("Total elev wait: %d\n", elevator_wait_time(req, 3));
    }

    // === Traffic single car ===
    printf("=== Traffic single car ===\n");
    {
        struct TrafficLight lights[3];
        lights[0].cycle_time = 10; lights[0].green_time = 5; lights[0].offset = 0;
        lights[1].cycle_time = 10; lights[1].green_time = 5; lights[1].offset = 3;
        lights[2].cycle_time = 10; lights[2].green_time = 5; lights[2].offset = 6;
        int travel[2]; travel[0] = 3; travel[1] = 3;
        printf("Travel time: %d\n", traffic_sim_single_car(lights, 3, travel, 0));
    }

    // === Traffic multi car ===
    printf("=== Traffic multi car ===\n");
    {
        struct TrafficLight lights[2];
        lights[0].cycle_time = 10; lights[0].green_time = 5; lights[0].offset = 0;
        lights[1].cycle_time = 10; lights[1].green_time = 5; lights[1].offset = 0;
        int travel[1]; travel[0] = 5;
        int starts[3]; starts[0] = 0; starts[1] = 3; starts[2] = 7;
        printf("Total travel: %d\n", traffic_sim_multi_car(lights, 2, travel, starts, 3));
    }

    // === Green waves ===
    printf("=== Green waves ===\n");
    {
        struct TrafficLight lights[2];
        lights[0].cycle_time = 10; lights[0].green_time = 5; lights[0].offset = 0;
        lights[1].cycle_time = 10; lights[1].green_time = 5; lights[1].offset = 3;
        int travel[1]; travel[0] = 3;
        printf("Green waves (0-20): %d\n", count_green_waves(lights, 2, travel, 20));
    }

    // === Traffic light states ===
    printf("=== Traffic light states ===\n");
    {
        struct TrafficLight l;
        l.cycle_time = 10; l.green_time = 5; l.offset = 0;
        printf("Light0 at t=0: %d\n", get_light_state(&l, 0));
        printf("Light0 at t=5: %d\n", get_light_state(&l, 5));
        printf("Light0 at t=10: %d\n", get_light_state(&l, 10));
        printf("Wait at light0 t=5: %d\n", wait_at_light(&l, 5));
        printf("Wait at light0 t=3: %d\n", wait_at_light(&l, 3));
    }

    // === MM1 queue ===
    printf("=== MM1 queue ===\n");
    printf("MM1 L*100 (30/50): %d\n", mm1_avg_customers(30, 50));
    printf("MM1 util*100 (30/50): %d\n", mm1_utilization(30, 50));

    // === Producer-Consumer ===
    printf("=== Producer-Consumer ===\n");
    {
        int pt[5]; pt[0]=0; pt[1]=1; pt[2]=2; pt[3]=3; pt[4]=4;
        int ct[3]; ct[0]=2; ct[1]=4; ct[2]=6;
        printf("Max buffer: %d\n", producer_consumer(pt, 5, ct, 3, 10));
    }

    // === Round Robin ===
    printf("=== Round Robin ===\n");
    {
        int bt[4]; bt[0]=5; bt[1]=3; bt[2]=8; bt[3]=6;
        printf("RR time q=2: %d\n", round_robin(bt, 4, 2));
    }

    // === SJF ===
    printf("=== SJF ===\n");
    {
        int bt[4]; bt[0]=5; bt[1]=3; bt[2]=8; bt[3]=6;
        printf("SJF total wait: %d\n", sjf_total_time(bt, 4));
    }

    // === Bank batch ===
    printf("=== Bank batch test ===\n");
    {
        struct Customer c[3];
        c[0].arrival_time = 0; c[0].service_time = 2;
        c[1].arrival_time = 5; c[1].service_time = 3;
        c[2].arrival_time = 10; c[2].service_time = 1;
        printf("Batch total wait: %d\n", bank_sim_single(c, 3));
    }

    // === Elevator batch ===
    printf("=== Elevator batch ===\n");
    {
        struct ElevatorRequest req[4];
        req[0].floor = 3; req[0].request_time = 0; req[0].direction = 1;
        req[1].floor = 7; req[1].request_time = 0; req[1].direction = 1;
        req[2].floor = 2; req[2].request_time = 0; req[2].direction = -1;
        req[3].floor = 5; req[3].request_time = 0; req[3].direction = 1;
        printf("Batch FCFS: %d\n", elevator_fcfs(req, 4));
    }
    {
        int req[4]; req[0]=3; req[1]=7; req[2]=2; req[3]=5;
        printf("Batch SSTF: %d\n", elevator_sstf(req, 4, 0));
    }

    // === Traffic batch ===
    printf("=== Traffic batch ===\n");
    {
        struct TrafficLight lights[2];
        lights[0].cycle_time = 10; lights[0].green_time = 5; lights[0].offset = 0;
        lights[1].cycle_time = 10; lights[1].green_time = 5; lights[1].offset = 0;
        int travel[1]; travel[0] = 3;
        printf("Batch single car: %d\n", traffic_sim_single_car(lights, 2, travel, 0));
    }

    // === RR vs SJF ===
    printf("=== RR vs SJF ===\n");
    {
        int bt[3]; bt[0]=6; bt[1]=3; bt[2]=12;
        printf("RR q=3: %d\n", round_robin(bt, 3, 3));
        printf("SJF wait: %d\n", sjf_total_time(bt, 3));
    }

    printf("All simulation tests passed!\n");
    return 0;
}
