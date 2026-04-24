int printf(const char *fmt, ...);

// Banker's algorithm for deadlock avoidance
// Simplified: 3 resource types, up to 5 processes

struct BankerState {
    int n_proc;
    int n_res;
    int available[3];
    int maximum[5][3];
    int allocation[5][3];
    int need[5][3];       // need = maximum - allocation
};

void banker_init(struct BankerState *b, int np, int nr) {
    b->n_proc = np;
    b->n_res = nr;
    int i;
    int j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            b->maximum[i][j] = 0;
            b->allocation[i][j] = 0;
            b->need[i][j] = 0;
        }
    }
    for (j = 0; j < 3; j++) {
        b->available[j] = 0;
    }
}

void set_available(struct BankerState *b, int r0, int r1, int r2) {
    b->available[0] = r0;
    b->available[1] = r1;
    b->available[2] = r2;
}

void set_max(struct BankerState *b, int proc, int r0, int r1, int r2) {
    b->maximum[proc][0] = r0;
    b->maximum[proc][1] = r1;
    b->maximum[proc][2] = r2;
    b->need[proc][0] = r0 - b->allocation[proc][0];
    b->need[proc][1] = r1 - b->allocation[proc][1];
    b->need[proc][2] = r2 - b->allocation[proc][2];
}

void set_alloc(struct BankerState *b, int proc, int r0, int r1, int r2) {
    b->allocation[proc][0] = r0;
    b->allocation[proc][1] = r1;
    b->allocation[proc][2] = r2;
    b->need[proc][0] = b->maximum[proc][0] - r0;
    b->need[proc][1] = b->maximum[proc][1] - r1;
    b->need[proc][2] = b->maximum[proc][2] - r2;
}

// Check if process p can be satisfied with work vector
int can_finish(struct BankerState *b, int p, int *work) {
    int j;
    for (j = 0; j < b->n_res; j++) {
        if (b->need[p][j] > work[j]) return 0;
    }
    return 1;
}

// Safety algorithm: returns 1 if safe, 0 if unsafe
// Also fills safe_seq with the safe sequence
int is_safe(struct BankerState *b, int *safe_seq) {
    int work[3];
    int finish[5];
    int i;
    int j;

    for (j = 0; j < b->n_res; j++) {
        work[j] = b->available[j];
    }
    for (i = 0; i < b->n_proc; i++) {
        finish[i] = 0;
    }

    int count = 0;
    int found;
    while (count < b->n_proc) {
        found = 0;
        for (i = 0; i < b->n_proc; i++) {
            if (!finish[i] && can_finish(b, i, work)) {
                // This process can finish
                for (j = 0; j < b->n_res; j++) {
                    work[j] = work[j] + b->allocation[i][j];
                }
                finish[i] = 1;
                safe_seq[count] = i;
                count++;
                found = 1;
            }
        }
        if (!found) break; // deadlock
    }

    return (count == b->n_proc) ? 1 : 0;
}

// Request resources for process p
// Returns 1 if granted, 0 if denied (would be unsafe)
int request_resources(struct BankerState *b, int p, int r0, int r1, int r2) {
    int req[3];
    req[0] = r0;
    req[1] = r1;
    req[2] = r2;

    int j;
    // Check request <= need
    if (req[0] > b->need[p][0] || req[1] > b->need[p][1] || req[2] > b->need[p][2]) {
        return 0;
    }
    // Check request <= available
    if (req[0] > b->available[0] || req[1] > b->available[1] || req[2] > b->available[2]) {
        return 0;
    }

    // Try allocation
    for (j = 0; j < b->n_res; j++) {
        b->available[j] = b->available[j] - req[j];
        b->allocation[p][j] = b->allocation[p][j] + req[j];
        b->need[p][j] = b->need[p][j] - req[j];
    }

    // Check safety
    int seq[5];
    if (is_safe(b, seq)) {
        return 1; // granted
    }

    // Rollback
    for (j = 0; j < b->n_res; j++) {
        b->available[j] = b->available[j] + req[j];
        b->allocation[p][j] = b->allocation[p][j] - req[j];
        b->need[p][j] = b->need[p][j] + req[j];
    }

    return 0; // denied
}

int main(void) {
    struct BankerState b;
    int seq[5];

    // Classic example: 5 processes, 3 resource types
    // Available: (3, 3, 2)
    // Max matrix:
    //   P0: (7, 5, 3)
    //   P1: (3, 2, 2)
    //   P2: (9, 0, 2)
    //   P3: (2, 2, 2)
    //   P4: (4, 3, 3)
    // Allocation:
    //   P0: (0, 1, 0)
    //   P1: (2, 0, 0)
    //   P2: (3, 0, 2)
    //   P3: (2, 1, 1)
    //   P4: (0, 0, 2)

    banker_init(&b, 5, 3);
    set_available(&b, 3, 3, 2);

    set_max(&b, 0, 7, 5, 3);
    set_max(&b, 1, 3, 2, 2);
    set_max(&b, 2, 9, 0, 2);
    set_max(&b, 3, 2, 2, 2);
    set_max(&b, 4, 4, 3, 3);

    set_alloc(&b, 0, 0, 1, 0);
    set_alloc(&b, 1, 2, 0, 0);
    set_alloc(&b, 2, 3, 0, 2);
    set_alloc(&b, 3, 2, 1, 1);
    set_alloc(&b, 4, 0, 0, 2);

    // Need matrix:
    //   P0: (7,4,3)
    //   P1: (1,2,2)
    //   P2: (6,0,0)
    //   P3: (0,1,1)
    //   P4: (4,3,1)

    printf("Need P0: %d %d %d\n", b.need[0][0], b.need[0][1], b.need[0][2]);
    // EXPECT: Need P0: 7 4 3
    printf("Need P1: %d %d %d\n", b.need[1][0], b.need[1][1], b.need[1][2]);
    // EXPECT: Need P1: 1 2 2
    printf("Need P3: %d %d %d\n", b.need[3][0], b.need[3][1], b.need[3][2]);
    // EXPECT: Need P3: 0 1 1

    // Check safety
    // Available: (3,3,2)
    // P1 need (1,2,2) <= (3,3,2) -> finish P1, work=(5,3,2)
    // P3 need (0,1,1) <= (5,3,2) -> finish P3, work=(7,4,3)
    // P4 need (4,3,1) <= (7,4,3) -> finish P4, work=(7,4,5)
    // P0 need (7,4,3) <= (7,4,5) -> finish P0, work=(7,5,5)
    // P2 need (6,0,0) <= (7,5,5) -> finish P2, work=(10,5,7)
    int safe = is_safe(&b, seq);
    printf("Safe: %d\n", safe);
    // EXPECT: Safe: 1

    printf("Safe sequence: P%d P%d P%d P%d P%d\n",
           seq[0], seq[1], seq[2], seq[3], seq[4]);
    // EXPECT: Safe sequence: P1 P3 P4 P0 P2

    // Request: P1 requests (1,0,2)
    // P1 need is (1,2,2), request (1,0,2) <= need? 1<=1, 0<=2, 2<=2 yes
    // Available (3,3,2), request (1,0,2) <= avail? yes
    // New: avail=(2,3,0), alloc P1=(3,0,2), need P1=(0,2,0)
    // Safety check with new state:
    // P1 need (0,2,0) <= (2,3,0) -> finish P1, work=(5,3,2)
    // P3 need (0,1,1) <= (5,3,2) -> finish P3, work=(7,4,3)
    // P4 need (4,3,1) <= (7,4,3) -> finish P4, work=(7,4,5)
    // P0 need (7,4,3) <= (7,4,5) -> finish P0, work=(7,5,5)
    // P2 need (6,0,0) <= (7,5,5) -> finish P2 -> SAFE
    int granted = request_resources(&b, 1, 1, 0, 2);
    printf("P1 request (1,0,2): %s\n", granted ? "granted" : "denied");
    // EXPECT: P1 request (1,0,2): granted

    printf("Available after: %d %d %d\n", b.available[0], b.available[1], b.available[2]);
    // EXPECT: Available after: 2 3 0

    // Request: P4 requests (3,3,0) -- this would be unsafe
    // P4 need is (4,3,1), request (3,3,0) <= need? 3<=4, 3<=3, 0<=1 yes
    // Available (2,3,0), request (3,3,0) <= avail? 3>2 NO -> denied
    int g2 = request_resources(&b, 4, 3, 3, 0);
    printf("P4 request (3,3,0): %s\n", g2 ? "granted" : "denied");
    // EXPECT: P4 request (3,3,0): denied

    // Request: P0 requests (0,2,0)
    // P0 need is (7,4,3), request (0,2,0) <= need? yes
    // Available (2,3,0), request (0,2,0) <= avail? 0<=2, 2<=3, 0<=0 yes
    // New: avail=(2,1,0), alloc P0=(0,3,0), need P0=(7,2,3)
    // Safety: work=(2,1,0)
    // P1 need (0,2,0) -> 2>1 NO
    // P2 need (6,0,0) -> 6>2 NO
    // P3 need (0,1,1) -> 1>0 NO
    // P4 need (4,3,1) -> 4>2 NO
    // P0 need (7,2,3) -> 7>2 NO
    // Nobody can finish -> UNSAFE -> deny
    int g3 = request_resources(&b, 0, 0, 2, 0);
    printf("P0 request (0,2,0): %s\n", g3 ? "granted" : "denied");
    // EXPECT: P0 request (0,2,0): denied

    // Verify state unchanged after denial
    printf("Available still: %d %d %d\n", b.available[0], b.available[1], b.available[2]);
    // EXPECT: Available still: 2 3 0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
