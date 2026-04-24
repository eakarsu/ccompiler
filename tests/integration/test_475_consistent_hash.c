int printf(const char *fmt, ...);
// EXPECT: Servers: 3\nRing nodes: 12\nKey mappings:\n  Key 3 -> Server 2\n  Key 10 -> Server 1\n  Key 17 -> Server 2\n  Key 24 -> Server 3\n  Key 31 -> Server 1\n  Key 38 -> Server 3\nDistribution: S1=5 S2=3 S3=4\n\nAfter adding server 4:\nRing nodes: 16\nKey mappings:\n  Key 3 -> Server 2\n  Key 10 -> Server 1\n  Key 17 -> Server 2\n  Key 24 -> Server 3\n  Key 31 -> Server 1\n  Key 38 -> Server 3\nDistribution: S1=3 S2=3 S3=4 S4=2\n\nAfter removing server 2:\nRing nodes: 12\nDistribution: S1=6 S3=4 S4=2\nDone

// Consistent hashing ring simulation

struct VNode {
    int position;
    int server_id;
};

struct ConsistentHash {
    struct VNode ring[64];
    int ring_size;
    int num_servers;
    int vnodes_per_server;
};

int ch_hash_pos(int key) {
    // Simple hash to 0-999 range
    int h = key * 2654435761;  // Knuth multiplicative hash
    if (h < 0) h = -h;
    return h % 1000;
}

void ch_sort_ring(struct ConsistentHash *ch) {
    // Insertion sort by position
    int i = 1;
    while (i < ch->ring_size) {
        struct VNode tmp;
        tmp.position = ch->ring[i].position;
        tmp.server_id = ch->ring[i].server_id;
        int j = i - 1;
        while (j >= 0 && ch->ring[j].position > tmp.position) {
            ch->ring[j + 1].position = ch->ring[j].position;
            ch->ring[j + 1].server_id = ch->ring[j].server_id;
            j = j - 1;
        }
        ch->ring[j + 1].position = tmp.position;
        ch->ring[j + 1].server_id = tmp.server_id;
        i = i + 1;
    }
}

void ch_init(struct ConsistentHash *ch, int vnodes_per_server) {
    ch->ring_size = 0;
    ch->num_servers = 0;
    ch->vnodes_per_server = vnodes_per_server;
}

void ch_add_server(struct ConsistentHash *ch, int server_id) {
    int i = 0;
    while (i < ch->vnodes_per_server) {
        int pos = ch_hash_pos(server_id * 100 + i);
        ch->ring[ch->ring_size].position = pos;
        ch->ring[ch->ring_size].server_id = server_id;
        ch->ring_size = ch->ring_size + 1;
        i = i + 1;
    }
    ch->num_servers = ch->num_servers + 1;
    ch_sort_ring(ch);
}

void ch_remove_server(struct ConsistentHash *ch, int server_id) {
    int write = 0;
    int read = 0;
    while (read < ch->ring_size) {
        if (ch->ring[read].server_id != server_id) {
            if (write != read) {
                ch->ring[write].position = ch->ring[read].position;
                ch->ring[write].server_id = ch->ring[read].server_id;
            }
            write = write + 1;
        }
        read = read + 1;
    }
    ch->ring_size = write;
    ch->num_servers = ch->num_servers - 1;
}

int ch_get_server(struct ConsistentHash *ch, int key) {
    if (ch->ring_size == 0) return -1;
    int pos = ch_hash_pos(key);
    // Binary search for first vnode >= pos
    int lo = 0;
    int hi = ch->ring_size - 1;
    int result = 0;  // wrap around to first if none found
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (ch->ring[mid].position >= pos) {
            result = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    if (lo > ch->ring_size - 1) result = 0;  // wrap
    return ch->ring[result].server_id;
}

void ch_distribution(struct ConsistentHash *ch, int *keys, int num_keys) {
    int counts[10];
    int i = 0;
    while (i < 10) {
        counts[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < num_keys) {
        int srv = ch_get_server(ch, keys[i]);
        if (srv >= 0 && srv < 10) {
            counts[srv] = counts[srv] + 1;
        }
        i = i + 1;
    }
    printf("Distribution:");
    i = 0;
    while (i < 10) {
        if (counts[i] > 0) {
            printf(" S%d=%d", i, counts[i]);
        }
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    struct ConsistentHash ch;
    ch_init(&ch, 4);

    ch_add_server(&ch, 1);
    ch_add_server(&ch, 2);
    ch_add_server(&ch, 3);

    printf("Servers: %d\n", ch.num_servers);
    printf("Ring nodes: %d\n", ch.ring_size);

    // Map some keys
    int keys[12];
    int i = 0;
    while (i < 12) {
        keys[i] = i * 7 + 3;
        i = i + 1;
    }

    printf("Key mappings:\n");
    i = 0;
    while (i < 6) {
        printf("  Key %d -> Server %d\n", keys[i], ch_get_server(&ch, keys[i]));
        i = i + 1;
    }

    ch_distribution(&ch, keys, 12);

    // Add a server
    ch_add_server(&ch, 4);
    printf("\nAfter adding server 4:\n");
    printf("Ring nodes: %d\n", ch.ring_size);
    printf("Key mappings:\n");
    i = 0;
    while (i < 6) {
        printf("  Key %d -> Server %d\n", keys[i], ch_get_server(&ch, keys[i]));
        i = i + 1;
    }
    ch_distribution(&ch, keys, 12);

    // Remove a server
    ch_remove_server(&ch, 2);
    printf("\nAfter removing server 2:\n");
    printf("Ring nodes: %d\n", ch.ring_size);
    ch_distribution(&ch, keys, 12);

    printf("Done\n");
    return 0;
}
