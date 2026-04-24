int printf(const char *fmt, ...);
// EXPECT: Size: 10/16\nMax displacement: 3\nAvg displacement x10: 10\nGet 1: 10 (found=1)\nGet 17: 170 (found=1)\nGet 33: 330 (found=1)\nGet 49: 490 (found=1)\nGet 99: found=0\nDisplacement at slot 1: 0\nUpdated 1: 999\nSize after update: 10\nDone

// Robin Hood hashing - open addressing with displacement tracking

struct RHEntry {
    int key;
    int value;
    int dist;   // probe distance from ideal position
    int occupied;
};

struct RobinHood {
    struct RHEntry table[16];
    int capacity;
    int size;
};

void rh_init(struct RobinHood *rh) {
    rh->capacity = 16;
    rh->size = 0;
    int i = 0;
    while (i < 16) {
        rh->table[i].occupied = 0;
        rh->table[i].dist = 0;
        i = i + 1;
    }
}

int rh_hash(int key, int cap) {
    int h = key;
    if (h < 0) h = -h;
    return h % cap;
}

void rh_put(struct RobinHood *rh, int key, int value) {
    int h = rh_hash(key, rh->capacity);
    int dist = 0;
    int cur_key = key;
    int cur_val = value;
    int cur_dist = dist;

    int i = 0;
    while (i < rh->capacity) {
        int idx = (h + i) % rh->capacity;
        if (!rh->table[idx].occupied) {
            rh->table[idx].key = cur_key;
            rh->table[idx].value = cur_val;
            rh->table[idx].dist = cur_dist;
            rh->table[idx].occupied = 1;
            rh->size = rh->size + 1;
            return;
        }
        if (rh->table[idx].key == cur_key) {
            rh->table[idx].value = cur_val;
            return;
        }
        // Robin Hood: steal from rich (low dist), give to poor (high dist)
        if (cur_dist > rh->table[idx].dist) {
            int tmp_key = rh->table[idx].key;
            int tmp_val = rh->table[idx].value;
            int tmp_dist = rh->table[idx].dist;
            rh->table[idx].key = cur_key;
            rh->table[idx].value = cur_val;
            rh->table[idx].dist = cur_dist;
            cur_key = tmp_key;
            cur_val = tmp_val;
            cur_dist = tmp_dist;
        }
        cur_dist = cur_dist + 1;
        i = i + 1;
    }
}

int rh_get(struct RobinHood *rh, int key, int *found) {
    int h = rh_hash(key, rh->capacity);
    int i = 0;
    while (i < rh->capacity) {
        int idx = (h + i) % rh->capacity;
        if (!rh->table[idx].occupied) {
            *found = 0;
            return 0;
        }
        if (rh->table[idx].key == key) {
            *found = 1;
            return rh->table[idx].value;
        }
        if (i > rh->table[idx].dist) {
            // Can stop early - key would have been here
            *found = 0;
            return 0;
        }
        i = i + 1;
    }
    *found = 0;
    return 0;
}

int rh_max_displacement(struct RobinHood *rh) {
    int max_d = 0;
    int i = 0;
    while (i < rh->capacity) {
        if (rh->table[i].occupied && rh->table[i].dist > max_d) {
            max_d = rh->table[i].dist;
        }
        i = i + 1;
    }
    return max_d;
}

int rh_avg_displacement_x10(struct RobinHood *rh) {
    int total = 0;
    int i = 0;
    while (i < rh->capacity) {
        if (rh->table[i].occupied) {
            total = total + rh->table[i].dist;
        }
        i = i + 1;
    }
    if (rh->size == 0) return 0;
    return (total * 10) / rh->size;
}

void rh_print_stats(struct RobinHood *rh) {
    printf("Size: %d/%d\n", rh->size, rh->capacity);
    printf("Max displacement: %d\n", rh_max_displacement(rh));
    printf("Avg displacement x10: %d\n", rh_avg_displacement_x10(rh));
}

int main(void) {
    struct RobinHood rh;
    rh_init(&rh);

    // Insert keys that will cause collisions
    rh_put(&rh, 1, 10);
    rh_put(&rh, 17, 170);
    rh_put(&rh, 33, 330);
    rh_put(&rh, 49, 490);
    rh_put(&rh, 5, 50);
    rh_put(&rh, 21, 210);
    rh_put(&rh, 37, 370);
    rh_put(&rh, 8, 80);
    rh_put(&rh, 24, 240);
    rh_put(&rh, 11, 110);

    rh_print_stats(&rh);

    int found;
    int v;
    v = rh_get(&rh, 1, &found);
    printf("Get 1: %d (found=%d)\n", v, found);
    v = rh_get(&rh, 17, &found);
    printf("Get 17: %d (found=%d)\n", v, found);
    v = rh_get(&rh, 33, &found);
    printf("Get 33: %d (found=%d)\n", v, found);
    v = rh_get(&rh, 49, &found);
    printf("Get 49: %d (found=%d)\n", v, found);
    v = rh_get(&rh, 99, &found);
    printf("Get 99: found=%d\n", found);

    // Verify displacements are bounded
    printf("Displacement at slot 1: %d\n", rh.table[1].dist);

    // Update existing
    rh_put(&rh, 1, 999);
    v = rh_get(&rh, 1, &found);
    printf("Updated 1: %d\n", v);
    printf("Size after update: %d\n", rh.size);

    printf("Done\n");
    return 0;
}
