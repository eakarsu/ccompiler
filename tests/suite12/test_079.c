int printf(const char *fmt, ...);

// Quorum System Simulation
// Simulates read/write quorums for replicated data with versioned reads

struct Replica {
    int id;
    int is_alive;
    int value;
    int version;
};

struct QuorumConfig {
    int n;             // total replicas
    int write_quorum;  // W
    int read_quorum;   // R
};

void replica_init(struct Replica *r, int id) {
    r->id = id;
    r->is_alive = 1;
    r->value = 0;
    r->version = 0;
}

void config_init(struct QuorumConfig *qc, int n, int w, int r) {
    qc->n = n;
    qc->write_quorum = w;
    qc->read_quorum = r;
}

int is_valid_quorum(struct QuorumConfig *qc) {
    // R + W > N ensures overlap
    if (qc->read_quorum + qc->write_quorum > qc->n) {
        return 1;
    }
    return 0;
}

// Write to quorum: returns number of successful writes
int quorum_write(struct Replica *replicas, struct QuorumConfig *qc, int value, int version) {
    int i;
    int writes;
    writes = 0;

    for (i = 0; i < qc->n; i = i + 1) {
        if (replicas[i].is_alive == 0) continue;
        if (writes >= qc->write_quorum) break;

        if (version > replicas[i].version) {
            replicas[i].value = value;
            replicas[i].version = version;
            writes = writes + 1;
            printf("  Write to R%d: val=%d ver=%d\n", i, value, version);
        }
    }
    return writes;
}

// Read from quorum: returns the value with highest version
int quorum_read(struct Replica *replicas, struct QuorumConfig *qc, int *out_version) {
    int i;
    int reads;
    int best_val;
    int best_ver;

    reads = 0;
    best_val = -1;
    best_ver = -1;

    for (i = 0; i < qc->n; i = i + 1) {
        if (replicas[i].is_alive == 0) continue;
        if (reads >= qc->read_quorum) break;

        reads = reads + 1;
        printf("  Read from R%d: val=%d ver=%d\n", i, replicas[i].value, replicas[i].version);

        if (replicas[i].version > best_ver) {
            best_ver = replicas[i].version;
            best_val = replicas[i].value;
        }
    }

    *out_version = best_ver;
    return best_val;
}

int count_alive(struct Replica *replicas, int n) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (replicas[i].is_alive) count = count + 1;
    }
    return count;
}

int can_write(struct Replica *replicas, struct QuorumConfig *qc) {
    int alive;
    alive = count_alive(replicas, qc->n);
    return alive >= qc->write_quorum;
}

int can_read(struct Replica *replicas, struct QuorumConfig *qc) {
    int alive;
    alive = count_alive(replicas, qc->n);
    return alive >= qc->read_quorum;
}

void print_replicas(struct Replica *replicas, int n) {
    int i;
    printf("Replicas:");
    for (i = 0; i < n; i = i + 1) {
        if (replicas[i].is_alive) {
            printf(" R%d(v%d=%d)", i, replicas[i].version, replicas[i].value);
        } else {
            printf(" R%d(DOWN)", i);
        }
    }
    printf("\n");
}

int main() {
    struct Replica replicas[5];
    struct QuorumConfig qc;
    int i;
    int val;
    int ver;
    int writes;
    int valid;

    for (i = 0; i < 5; i = i + 1) {
        replica_init(&replicas[i], i);
    }

    // N=5, W=3, R=3 (strict quorum)
    config_init(&qc, 5, 3, 3);
    valid = is_valid_quorum(&qc);
    printf("Quorum N=%d W=%d R=%d valid=%d\n", qc.n, qc.write_quorum, qc.read_quorum, valid);
    // EXPECT: Quorum N=5 W=3 R=3 valid=1

    // Write value 42
    printf("Write 42 (version 1):\n");
    // EXPECT: Write 42 (version 1):
    writes = quorum_write(replicas, &qc, 42, 1);
    // EXPECT:   Write to R0: val=42 ver=1
    // EXPECT:   Write to R1: val=42 ver=1
    // EXPECT:   Write to R2: val=42 ver=1
    printf("Writes successful: %d\n", writes);
    // EXPECT: Writes successful: 3

    print_replicas(replicas, 5);
    // EXPECT: Replicas: R0(v1=42) R1(v1=42) R2(v1=42) R3(v0=0) R4(v0=0)

    // Read - should get 42
    printf("Read:\n");
    // EXPECT: Read:
    val = quorum_read(replicas, &qc, &ver);
    // EXPECT:   Read from R0: val=42 ver=1
    // EXPECT:   Read from R1: val=42 ver=1
    // EXPECT:   Read from R2: val=42 ver=1
    printf("Read result: val=%d ver=%d\n", val, ver);
    // EXPECT: Read result: val=42 ver=1

    // Write new value
    printf("Write 99 (version 2):\n");
    // EXPECT: Write 99 (version 2):
    writes = quorum_write(replicas, &qc, 99, 2);
    // EXPECT:   Write to R0: val=99 ver=2
    // EXPECT:   Write to R1: val=99 ver=2
    // EXPECT:   Write to R2: val=99 ver=2
    printf("Writes successful: %d\n", writes);
    // EXPECT: Writes successful: 3

    // Take nodes down
    printf("--- Taking R0, R1 down ---\n");
    // EXPECT: --- Taking R0, R1 down ---
    replicas[0].is_alive = 0;
    replicas[1].is_alive = 0;

    printf("Can write: %d\n", can_write(replicas, &qc));
    // EXPECT: Can write: 1
    printf("Can read: %d\n", can_read(replicas, &qc));
    // EXPECT: Can read: 1

    // Take one more down
    replicas[2].is_alive = 0;
    printf("After R2 down - Can write: %d, Can read: %d\n",
           can_write(replicas, &qc), can_read(replicas, &qc));
    // EXPECT: After R2 down - Can write: 0, Can read: 0

    // Restore
    replicas[0].is_alive = 1;
    replicas[1].is_alive = 1;
    replicas[2].is_alive = 1;

    // Sloppy quorum: N=5, W=2, R=2 (not strict)
    printf("--- Sloppy quorum ---\n");
    // EXPECT: --- Sloppy quorum ---
    config_init(&qc, 5, 2, 2);
    valid = is_valid_quorum(&qc);
    printf("Quorum N=%d W=%d R=%d valid=%d\n", qc.n, qc.write_quorum, qc.read_quorum, valid);
    // EXPECT: Quorum N=5 W=2 R=2 valid=0

    config_init(&qc, 5, 3, 3);
    print_replicas(replicas, 5);
    // EXPECT: Replicas: R0(v2=99) R1(v2=99) R2(v2=99) R3(v0=0) R4(v0=0)

    printf("Quorum system simulation done\n");
    // EXPECT: Quorum system simulation done

    return 0;
}
