int printf(const char *fmt, ...);

// Consistent Hashing
// Simulates a hash ring with virtual nodes for load balancing

struct VNode {
    int hash_val;
    int real_node;
    int active;
};

struct HashRing {
    struct VNode vnodes[30];
    int count;
};

// Simple hash function
int simple_hash(int key, int mod) {
    int h;
    h = key * 2654435761;
    if (h < 0) h = -h;
    h = h % mod;
    return h;
}

int node_hash(int node_id, int vnode_idx, int mod) {
    int h;
    h = (node_id * 1000 + vnode_idx * 137) * 31;
    if (h < 0) h = -h;
    h = h % mod;
    return h;
}

void ring_init(struct HashRing *ring) {
    ring->count = 0;
}

void ring_add_node(struct HashRing *ring, int node_id, int vnodes_per) {
    int i;
    int h;
    int idx;
    for (i = 0; i < vnodes_per; i = i + 1) {
        idx = ring->count;
        h = node_hash(node_id, i, 1000);
        ring->vnodes[idx].hash_val = h;
        ring->vnodes[idx].real_node = node_id;
        ring->vnodes[idx].active = 1;
        ring->count = ring->count + 1;
    }
}

// Sort vnodes by hash value (bubble sort)
void ring_sort(struct HashRing *ring) {
    int i;
    int j;
    int tmp_h;
    int tmp_n;
    int tmp_a;
    for (i = 0; i < ring->count - 1; i = i + 1) {
        for (j = 0; j < ring->count - 1 - i; j = j + 1) {
            if (ring->vnodes[j].hash_val > ring->vnodes[j + 1].hash_val) {
                tmp_h = ring->vnodes[j].hash_val;
                tmp_n = ring->vnodes[j].real_node;
                tmp_a = ring->vnodes[j].active;
                ring->vnodes[j].hash_val = ring->vnodes[j + 1].hash_val;
                ring->vnodes[j].real_node = ring->vnodes[j + 1].real_node;
                ring->vnodes[j].active = ring->vnodes[j + 1].active;
                ring->vnodes[j + 1].hash_val = tmp_h;
                ring->vnodes[j + 1].real_node = tmp_n;
                ring->vnodes[j + 1].active = tmp_a;
            }
        }
    }
}

int ring_lookup(struct HashRing *ring, int key_hash) {
    int i;
    for (i = 0; i < ring->count; i = i + 1) {
        if (ring->vnodes[i].active && ring->vnodes[i].hash_val >= key_hash) {
            return ring->vnodes[i].real_node;
        }
    }
    // Wrap around to first active node
    for (i = 0; i < ring->count; i = i + 1) {
        if (ring->vnodes[i].active) {
            return ring->vnodes[i].real_node;
        }
    }
    return -1;
}

void ring_remove_node(struct HashRing *ring, int node_id) {
    int i;
    for (i = 0; i < ring->count; i = i + 1) {
        if (ring->vnodes[i].real_node == node_id) {
            ring->vnodes[i].active = 0;
        }
    }
}

void count_distribution(struct HashRing *ring, int num_keys, int num_nodes) {
    int counts[5];
    int i;
    int kh;
    int node;
    for (i = 0; i < 5; i = i + 1) {
        counts[i] = 0;
    }
    for (i = 0; i < num_keys; i = i + 1) {
        kh = simple_hash(i * 7 + 3, 1000);
        node = ring_lookup(ring, kh);
        if (node >= 0 && node < 5) {
            counts[node] = counts[node] + 1;
        }
    }
    printf("Distribution:");
    for (i = 0; i < num_nodes; i = i + 1) {
        printf(" N%d=%d", i, counts[i]);
    }
    printf("\n");
}

int main() {
    struct HashRing ring;
    int kh;
    int node;

    ring_init(&ring);

    // Add 3 nodes with 3 virtual nodes each
    ring_add_node(&ring, 0, 3);
    ring_add_node(&ring, 1, 3);
    ring_add_node(&ring, 2, 3);
    ring_sort(&ring);

    printf("Ring has %d vnodes\n", ring.count);
    // EXPECT: Ring has 9 vnodes

    // Look up some keys
    kh = simple_hash(42, 1000);
    node = ring_lookup(&ring, kh);
    printf("Key 42 -> Node %d\n", node);
    // EXPECT: Key 42 -> Node 0

    kh = simple_hash(100, 1000);
    node = ring_lookup(&ring, kh);
    printf("Key 100 -> Node %d\n", node);
    // EXPECT: Key 100 -> Node 0

    kh = simple_hash(500, 1000);
    node = ring_lookup(&ring, kh);
    printf("Key 500 -> Node %d\n", node);
    // EXPECT: Key 500 -> Node 0

    kh = simple_hash(999, 1000);
    node = ring_lookup(&ring, kh);
    printf("Key 999 -> Node %d\n", node);
    // EXPECT: Key 999 -> Node 0

    // Show distribution
    count_distribution(&ring, 20, 3);
    // EXPECT: Distribution: N0=20 N1=0 N2=0

    // Remove node 1
    printf("Removing Node 1\n");
    // EXPECT: Removing Node 1
    ring_remove_node(&ring, 1);

    kh = simple_hash(500, 1000);
    node = ring_lookup(&ring, kh);
    printf("Key 500 after removal -> Node %d\n", node);
    // EXPECT: Key 500 after removal -> Node 0

    count_distribution(&ring, 20, 3);
    // EXPECT: Distribution: N0=20 N1=0 N2=0

    // Add node 3
    ring_add_node(&ring, 3, 3);
    ring_sort(&ring);
    printf("Added Node 3, total vnodes: %d\n", ring.count);
    // EXPECT: Added Node 3, total vnodes: 12

    count_distribution(&ring, 20, 4);
    // EXPECT: Distribution: N0=20 N1=0 N2=0 N3=0

    printf("Consistent hashing done\n");
    // EXPECT: Consistent hashing done

    return 0;
}
