int printf(const char *fmt, ...);
// EXPECT: FCFS1_W=0,5,12,18 FCFS1_T=6,13,19,21 FCFS1_C=6,14,21,24 FCFS1_AW=8 FCFS1_AT=14 FCFS1_SW=35 SJF1_W=0,15,7,3 SJF1_T=6,23,14,6 SJF1_C=6,24,16,9 SJF1_AW=6 SJF1_AT=12 RR1_W=9,14,15,6 RR1_T=15,22,22,9 RR1_C=15,23,24,12 RR1_AW=11 RR1_AT=17 PRI1_W=0,5,15,11 PRI1_T=6,13,22,14 PRI1_C=6,14,24,17 PRI1_AW=7 FCFS2_W=0,10,11,13,14 FCFS2_AW=9 FCFS2_AT=13 SJF2_W=9,0,2,1,4 SJF2_AW=3 RR2_W=9,2,3,5,10 RR2_AW=5 FCFS3_W=0 FCFS3_T=5 SJF3_W=0 RR3_W=0 RR3_T=5 FCFS4_W=0,1,5 FCFS4_C=3,9,13 SJF4_W=0,1,5 RR4_W=4,5,3 RR4_C=7,13,11 UTIL1=100 THRU1=16 MW1=18 NW1=0 RR_Q1_AW=12 RR_Q4_AW=11 RR_Q10_AW=8 
/* ====================================================================== */
/* Test 049: CPU Scheduling Algorithms Simulation                         */
/* FCFS, SJF, Round Robin, priority scheduling                            */
/* Calculate wait times, turnaround times                                 */
/* ====================================================================== */

/* Process representation:
   pid[i]     = process ID
   arrival[i] = arrival time
   burst[i]   = burst time (CPU time needed)
   priority[i]= priority (lower number = higher priority)
*/

/* ---- FCFS (First Come First Served) ---- */
/* Processes sorted by arrival time */
/* Wait time = start time - arrival time */
/* Turnaround time = completion time - arrival time */
void fcfs(int *arrival, int *burst, int n,
          int *wait, int *turnaround, int *completion) {
    int time;
    int i;

    time = 0;
    i = 0;
    while (i < n) {
        if (time < arrival[i]) {
            time = arrival[i];
        }
        wait[i] = time - arrival[i];
        time = time + burst[i];
        completion[i] = time;
        turnaround[i] = completion[i] - arrival[i];
        i = i + 1;
    }
}

/* Average (integer division) */
int average(int *arr, int n) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < n) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum / n;
}

/* Sum */
int sum_array(int *arr, int n) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < n) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

/* ---- SJF (Shortest Job First) - Non-preemptive ---- */
/* At each point, pick the arrived process with shortest burst */
void sjf(int *arrival, int *burst, int n,
         int *wait, int *turnaround, int *completion) {
    int done[20];
    int time;
    int i;
    int j;
    int min_burst;
    int min_idx;
    int completed;

    i = 0;
    while (i < n) {
        done[i] = 0;
        i = i + 1;
    }

    time = 0;
    completed = 0;

    while (completed < n) {
        /* Find shortest burst among arrived, not-done processes */
        min_burst = 99999;
        min_idx = -1;
        i = 0;
        while (i < n) {
            if (done[i] == 0 && arrival[i] <= time) {
                if (burst[i] < min_burst) {
                    min_burst = burst[i];
                    min_idx = i;
                }
            }
            i = i + 1;
        }

        if (min_idx == -1) {
            /* No process available, advance time */
            time = time + 1;
        } else {
            wait[min_idx] = time - arrival[min_idx];
            time = time + burst[min_idx];
            completion[min_idx] = time;
            turnaround[min_idx] = completion[min_idx] - arrival[min_idx];
            done[min_idx] = 1;
            completed = completed + 1;
        }
    }
}

/* ---- Round Robin ---- */
void round_robin(int *arrival, int *burst, int n, int quantum,
                 int *wait, int *turnaround, int *completion) {
    int remaining[20];
    int time;
    int i;
    int done;
    int all_done;
    int found;

    i = 0;
    while (i < n) {
        remaining[i] = burst[i];
        wait[i] = 0;
        i = i + 1;
    }

    time = 0;
    all_done = 0;

    while (all_done == 0) {
        all_done = 1;
        found = 0;
        i = 0;
        while (i < n) {
            if (remaining[i] > 0 && arrival[i] <= time) {
                found = 1;
                all_done = 0;
                if (remaining[i] > quantum) {
                    time = time + quantum;
                    remaining[i] = remaining[i] - quantum;
                } else {
                    time = time + remaining[i];
                    remaining[i] = 0;
                    completion[i] = time;
                    turnaround[i] = completion[i] - arrival[i];
                    wait[i] = turnaround[i] - burst[i];
                }
            }
            i = i + 1;
        }
        if (found == 0 && all_done == 0) {
            /* Check if there are remaining processes */
            i = 0;
            while (i < n) {
                if (remaining[i] > 0) {
                    all_done = 0;
                    i = n;
                }
                i = i + 1;
            }
            time = time + 1;
        }
    }
}

/* ---- Priority Scheduling (non-preemptive) ---- */
void priority_schedule(int *arrival, int *burst, int *priority, int n,
                       int *wait, int *turnaround, int *completion) {
    int done[20];
    int time;
    int i;
    int min_pri;
    int min_idx;
    int completed;

    i = 0;
    while (i < n) {
        done[i] = 0;
        i = i + 1;
    }

    time = 0;
    completed = 0;

    while (completed < n) {
        min_pri = 99999;
        min_idx = -1;
        i = 0;
        while (i < n) {
            if (done[i] == 0 && arrival[i] <= time) {
                if (priority[i] < min_pri) {
                    min_pri = priority[i];
                    min_idx = i;
                }
            }
            i = i + 1;
        }

        if (min_idx == -1) {
            time = time + 1;
        } else {
            wait[min_idx] = time - arrival[min_idx];
            time = time + burst[min_idx];
            completion[min_idx] = time;
            turnaround[min_idx] = completion[min_idx] - arrival[min_idx];
            done[min_idx] = 1;
            completed = completed + 1;
        }
    }
}

/* Print array */
void print_arr(int *arr, int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(",");
        i = i + 1;
    }
}

/* Find max in array */
int find_max(int *arr, int n) {
    int m;
    int i;
    m = arr[0];
    i = 1;
    while (i < n) {
        if (arr[i] > m) m = arr[i];
        i = i + 1;
    }
    return m;
}

/* Find min in array */
int find_min(int *arr, int n) {
    int m;
    int i;
    m = arr[0];
    i = 1;
    while (i < n) {
        if (arr[i] < m) m = arr[i];
        i = i + 1;
    }
    return m;
}

/* CPU utilization: total burst / total time * 100 */
int cpu_utilization(int *burst, int n, int *completion) {
    int total_burst;
    int max_completion;
    int i;

    total_burst = sum_array(burst, n);
    max_completion = find_max(completion, n);
    if (max_completion == 0) return 0;
    return total_burst * 100 / max_completion;
}

/* Throughput * 100: number of processes completed / total time */
int throughput_100(int n, int *completion) {
    int max_completion;
    max_completion = find_max(completion, n);
    if (max_completion == 0) return 0;
    return n * 100 / max_completion;
}

int main(void) {
    int arrival[10];
    int burst[10];
    int priority[10];
    int wait[10];
    int turnaround[10];
    int completion[10];
    int n;

    /* ---- Test Set 1: 4 processes ---- */
    /* P0: arrival=0, burst=6 */
    /* P1: arrival=1, burst=8 */
    /* P2: arrival=2, burst=7 */
    /* P3: arrival=3, burst=3 */
    n = 4;
    arrival[0]=0; burst[0]=6;
    arrival[1]=1; burst[1]=8;
    arrival[2]=2; burst[2]=7;
    arrival[3]=3; burst[3]=3;

    /* FCFS */
    fcfs(arrival, burst, n, wait, turnaround, completion);
    printf("FCFS1_W="); print_arr(wait, n); printf(" ");
    printf("FCFS1_T="); print_arr(turnaround, n); printf(" ");
    printf("FCFS1_C="); print_arr(completion, n); printf(" ");
    printf("FCFS1_AW=%d ", average(wait, n));
    printf("FCFS1_AT=%d ", average(turnaround, n));
    printf("FCFS1_SW=%d ", sum_array(wait, n));

    /* SJF */
    sjf(arrival, burst, n, wait, turnaround, completion);
    printf("SJF1_W="); print_arr(wait, n); printf(" ");
    printf("SJF1_T="); print_arr(turnaround, n); printf(" ");
    printf("SJF1_C="); print_arr(completion, n); printf(" ");
    printf("SJF1_AW=%d ", average(wait, n));
    printf("SJF1_AT=%d ", average(turnaround, n));

    /* Round Robin with quantum=3 */
    round_robin(arrival, burst, n, 3, wait, turnaround, completion);
    printf("RR1_W="); print_arr(wait, n); printf(" ");
    printf("RR1_T="); print_arr(turnaround, n); printf(" ");
    printf("RR1_C="); print_arr(completion, n); printf(" ");
    printf("RR1_AW=%d ", average(wait, n));
    printf("RR1_AT=%d ", average(turnaround, n));

    /* Priority scheduling */
    priority[0]=3; priority[1]=1; priority[2]=4; priority[3]=2;
    priority_schedule(arrival, burst, priority, n, wait, turnaround, completion);
    printf("PRI1_W="); print_arr(wait, n); printf(" ");
    printf("PRI1_T="); print_arr(turnaround, n); printf(" ");
    printf("PRI1_C="); print_arr(completion, n); printf(" ");
    printf("PRI1_AW=%d ", average(wait, n));

    /* ---- Test Set 2: All arrive at time 0 ---- */
    n = 5;
    arrival[0]=0; burst[0]=10;
    arrival[1]=0; burst[1]=1;
    arrival[2]=0; burst[2]=2;
    arrival[3]=0; burst[3]=1;
    arrival[4]=0; burst[4]=5;

    /* FCFS */
    fcfs(arrival, burst, n, wait, turnaround, completion);
    printf("FCFS2_W="); print_arr(wait, n); printf(" ");
    printf("FCFS2_AW=%d ", average(wait, n));
    printf("FCFS2_AT=%d ", average(turnaround, n));

    /* SJF */
    sjf(arrival, burst, n, wait, turnaround, completion);
    printf("SJF2_W="); print_arr(wait, n); printf(" ");
    printf("SJF2_AW=%d ", average(wait, n));

    /* Round Robin q=2 */
    round_robin(arrival, burst, n, 2, wait, turnaround, completion);
    printf("RR2_W="); print_arr(wait, n); printf(" ");
    printf("RR2_AW=%d ", average(wait, n));

    /* ---- Test Set 3: Single process ---- */
    n = 1;
    arrival[0]=0; burst[0]=5;
    fcfs(arrival, burst, n, wait, turnaround, completion);
    printf("FCFS3_W=%d ", wait[0]);
    printf("FCFS3_T=%d ", turnaround[0]);

    sjf(arrival, burst, n, wait, turnaround, completion);
    printf("SJF3_W=%d ", wait[0]);

    round_robin(arrival, burst, n, 2, wait, turnaround, completion);
    printf("RR3_W=%d ", wait[0]);
    printf("RR3_T=%d ", turnaround[0]);

    /* ---- Test Set 4: Staggered arrivals ---- */
    n = 3;
    arrival[0]=0; burst[0]=3;
    arrival[1]=2; burst[1]=6;
    arrival[2]=4; burst[2]=4;

    fcfs(arrival, burst, n, wait, turnaround, completion);
    printf("FCFS4_W="); print_arr(wait, n); printf(" ");
    printf("FCFS4_C="); print_arr(completion, n); printf(" ");

    sjf(arrival, burst, n, wait, turnaround, completion);
    printf("SJF4_W="); print_arr(wait, n); printf(" ");

    round_robin(arrival, burst, n, 2, wait, turnaround, completion);
    printf("RR4_W="); print_arr(wait, n); printf(" ");
    printf("RR4_C="); print_arr(completion, n); printf(" ");

    /* CPU utilization and throughput for test set 1 */
    n = 4;
    arrival[0]=0; burst[0]=6;
    arrival[1]=1; burst[1]=8;
    arrival[2]=2; burst[2]=7;
    arrival[3]=3; burst[3]=3;

    fcfs(arrival, burst, n, wait, turnaround, completion);
    printf("UTIL1=%d ", cpu_utilization(burst, n, completion));
    printf("THRU1=%d ", throughput_100(n, completion));

    /* Max and min wait times */
    printf("MW1=%d ", find_max(wait, n));
    printf("NW1=%d ", find_min(wait, n));

    /* Round Robin with different quanta */
    round_robin(arrival, burst, n, 1, wait, turnaround, completion);
    printf("RR_Q1_AW=%d ", average(wait, n));

    round_robin(arrival, burst, n, 4, wait, turnaround, completion);
    printf("RR_Q4_AW=%d ", average(wait, n));

    round_robin(arrival, burst, n, 10, wait, turnaround, completion);
    printf("RR_Q10_AW=%d ", average(wait, n));

    printf("\n");
    return 0;
}
