int printf(const char *fmt, ...);

// Consistent hashing ring (simplified)
// Maps virtual nodes on a ring [0, RING_SIZE) to physical servers
// Uses sorted array of ring positions

enum { RING_SIZE = 1000, MAX_NODES = 32 };

struct RingNode {
    int position;
    int server_id;
};

struct ConsistentRing {
    struct RingNode nodes[32];
    int node_count;
};

void ring_init(struct ConsistentRing *r) {
    r->node_count = 0;
}

int simple_hash(int val) {
    int h = val * 2654435761;
    if (h < 0) h = -h;
    return h % RING_SIZE;
}

void ring_sort(struct ConsistentRing *r) {
    int i;
    int j;
    struct RingNode tmp;
    for (i = 0; i < r->node_count - 1; i++) {
        for (j = i + 1; j < r->node_count; j++) {
            if (r->nodes[j].position < r->nodes[i].position) {
                tmp = r->nodes[i];
                r->nodes[i] = r->nodes[j];
                r->nodes[j] = tmp;
            }
        }
    }
}

void ring_add_server(struct ConsistentRing *r, int server_id, int num_vnodes) {
    int i;
    int pos;
    for (i = 0; i < num_vnodes; i++) {
        if (r->node_count >= MAX_NODES) return;
        pos = simple_hash(server_id * 100 + i);
        r->nodes[r->node_count].position = pos;
        r->nodes[r->node_count].server_id = server_id;
        r->node_count = r->node_count + 1;
    }
    ring_sort(r);
}

int ring_lookup(struct ConsistentRing *r, int key) {
    int pos;
    int i;
    if (r->node_count == 0) return -1;
    pos = simple_hash(key);
    for (i = 0; i < r->node_count; i++) {
        if (r->nodes[i].position >= pos) {
            return r->nodes[i].server_id;
        }
    }
    return r->nodes[0].server_id;
}

void ring_remove_server(struct ConsistentRing *r, int server_id) {
    int write;
    int read;
    write = 0;
    for (read = 0; read < r->node_count; read++) {
        if (r->nodes[read].server_id != server_id) {
            r->nodes[write] = r->nodes[read];
            write = write + 1;
        }
    }
    r->node_count = write;
}

void count_distribution(struct ConsistentRing *r, int *keys, int nkeys, int *counts, int nservers) {
    int i;
    int server;
    for (i = 0; i < nservers; i++) {
        counts[i] = 0;
    }
    for (i = 0; i < nkeys; i++) {
        server = ring_lookup(r, keys[i]);
        if (server >= 0 && server < nservers) {
            counts[server] = counts[server] + 1;
        }
    }
}

int main() {
    struct ConsistentRing ring;
    int keys[20];
    int counts[5];
    int i;
    int server;
    int total;

    ring_init(&ring);

    ring_add_server(&ring, 0, 3);
    ring_add_server(&ring, 1, 3);
    ring_add_server(&ring, 2, 3);

    printf("%d\n", ring.node_count);
    // EXPECT: 9

    for (i = 0; i < ring.node_count - 1; i++) {
        if (ring.nodes[i].position > ring.nodes[i + 1].position) {
            printf("ERROR: not sorted\n");
        }
    }
    printf("%s\n", "sorted_ok");
    // EXPECT: sorted_ok

    for (i = 0; i < 20; i++) {
        keys[i] = i * 50 + 7;
    }

    count_distribution(&ring, keys, 20, counts, 3);

    total = counts[0] + counts[1] + counts[2];
    printf("%d\n", total);
    // EXPECT: 20

    printf("%d\n", counts[0] > 0);
    // EXPECT: 1

    printf("%d\n", counts[1] > 0);
    // EXPECT: 1

    printf("%d\n", counts[2] > 0);
    // EXPECT: 1

    server = ring_lookup(&ring, 0);
    printf("%d\n", server >= 0);
    // EXPECT: 1

    server = ring_lookup(&ring, 500);
    printf("%d\n", server >= 0);
    // EXPECT: 1

    int s1 = ring_lookup(&ring, 42);
    int s2 = ring_lookup(&ring, 42);
    printf("%d\n", s1 == s2);
    // EXPECT: 1

    int before_counts[3];
    count_distribution(&ring, keys, 20, before_counts, 3);

    ring_remove_server(&ring, 1);
    printf("%d\n", ring.node_count);
    // EXPECT: 6

    int after_counts[3];
    count_distribution(&ring, keys, 20, after_counts, 3);

    total = after_counts[0] + after_counts[2];
    printf("%d\n", total);
    // EXPECT: 20

    printf("%d\n", after_counts[0] >= before_counts[0]);
    // EXPECT: 1

    printf("%d\n", after_counts[2] >= before_counts[2]);
    // EXPECT: 1

    ring_add_server(&ring, 3, 3);
    printf("%d\n", ring.node_count);
    // EXPECT: 9

    int new_counts[4];
    count_distribution(&ring, keys, 20, new_counts, 4);
    total = new_counts[0] + new_counts[2] + new_counts[3];
    printf("%d\n", total);
    // EXPECT: 20

    printf("%d\n", new_counts[3] > 0);
    // EXPECT: 0

    int stable = ring_lookup(&ring, 777);
    int stable2 = ring_lookup(&ring, 777);
    printf("%d\n", stable == stable2);
    // EXPECT: 1

    return 0;
}
