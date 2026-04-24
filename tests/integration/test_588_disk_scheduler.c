int printf(const char *fmt, ...);
// EXPECT: === FCFS Disk Scheduling ===\nHead starts at: 50\n  Move 50 -> 98 (seek=48)\n  Move 98 -> 183 (seek=85)\n  Move 183 -> 37 (seek=146)\n  Move 37 -> 122 (seek=85)\n  Move 122 -> 14 (seek=108)\n  Move 14 -> 65 (seek=51)\n  Move 65 -> 67 (seek=2)\nTotal seek: 525, Avg: 75\n\n=== SSTF Disk Scheduling ===\nHead starts at: 50\n  Move 50 -> 37 (seek=13)\n  Move 37 -> 14 (seek=23)\n  Move 14 -> 65 (seek=51)\n  Move 65 -> 67 (seek=2)\n  Move 67 -> 98 (seek=31)\n  Move 98 -> 122 (seek=24)\n  Move 122 -> 183 (seek=61)\nTotal seek: 205, Avg: 29\n\n=== SCAN Disk Scheduling ===\nHead starts at: 50\n  Move 50 -> 65 (seek=15)\n  Move 65 -> 67 (seek=2)\n  Move 67 -> 98 (seek=31)\n  Move 98 -> 122 (seek=24)\n  Move 122 -> 183 (seek=61)\n  Move 183 -> 37 (seek=146)\n  Move 37 -> 14 (seek=23)\nTotal seek: 302, Avg: 43
#define MAX_REQUESTS 10
#define DISK_SIZE 200

struct DiskRequest {
    int track;
    int served;
    int order;
};

struct DiskScheduler {
    struct DiskRequest requests[MAX_REQUESTS];
    int count;
    int head_pos;
    int total_seek;
    int served_count;
};

void ds_init(struct DiskScheduler *ds, int head) {
    ds->count = 0;
    ds->head_pos = head;
    ds->total_seek = 0;
    ds->served_count = 0;
}

void ds_add(struct DiskScheduler *ds, int track) {
    if (ds->count < MAX_REQUESTS) {
        ds->requests[ds->count].track = track;
        ds->requests[ds->count].served = 0;
        ds->requests[ds->count].order = 0;
        ds->count++;
    }
}

int abs_val(int x) { return x < 0 ? -x : x; }

void run_fcfs(struct DiskScheduler *ds) {
    printf("=== FCFS Disk Scheduling ===\n");
    printf("Head starts at: %d\n", ds->head_pos);
    ds->total_seek = 0;
    int pos = ds->head_pos;
    int i;
    for (i = 0; i < ds->count; i++) {
        int seek = abs_val(ds->requests[i].track - pos);
        ds->total_seek = ds->total_seek + seek;
        printf("  Move %d -> %d (seek=%d)\n", pos, ds->requests[i].track, seek);
        pos = ds->requests[i].track;
    }
    printf("Total seek: %d, Avg: %d\n", ds->total_seek, ds->total_seek / ds->count);
}

void run_sstf(struct DiskScheduler *ds) {
    printf("=== SSTF Disk Scheduling ===\n");
    printf("Head starts at: %d\n", ds->head_pos);
    int served[MAX_REQUESTS];
    int i;
    for (i = 0; i < ds->count; i++) served[i] = 0;

    int total = 0;
    int pos = ds->head_pos;
    int done = 0;
    while (done < ds->count) {
        int best = -1;
        int best_dist = 999999;
        for (i = 0; i < ds->count; i++) {
            if (!served[i]) {
                int dist = abs_val(ds->requests[i].track - pos);
                if (dist < best_dist) {
                    best_dist = dist;
                    best = i;
                }
            }
        }
        if (best == -1) break;
        served[best] = 1;
        total = total + best_dist;
        printf("  Move %d -> %d (seek=%d)\n", pos, ds->requests[best].track, best_dist);
        pos = ds->requests[best].track;
        done++;
    }
    printf("Total seek: %d, Avg: %d\n", total, total / ds->count);
}

void run_scan(struct DiskScheduler *ds) {
    printf("=== SCAN Disk Scheduling ===\n");
    printf("Head starts at: %d\n", ds->head_pos);
    int tracks[MAX_REQUESTS];
    int i;
    int j;
    for (i = 0; i < ds->count; i++) tracks[i] = ds->requests[i].track;
    for (i = 0; i < ds->count - 1; i++) {
        for (j = 0; j < ds->count - 1 - i; j++) {
            if (tracks[j] > tracks[j + 1]) {
                int tmp = tracks[j];
                tracks[j] = tracks[j + 1];
                tracks[j + 1] = tmp;
            }
        }
    }

    int total = 0;
    int pos = ds->head_pos;
    /* go right first */
    for (i = 0; i < ds->count; i++) {
        if (tracks[i] >= pos) {
            int seek = abs_val(tracks[i] - pos);
            total = total + seek;
            printf("  Move %d -> %d (seek=%d)\n", pos, tracks[i], seek);
            pos = tracks[i];
        }
    }
    /* then go left */
    for (i = ds->count - 1; i >= 0; i--) {
        if (tracks[i] < ds->head_pos) {
            int seek = abs_val(tracks[i] - pos);
            total = total + seek;
            printf("  Move %d -> %d (seek=%d)\n", pos, tracks[i], seek);
            pos = tracks[i];
        }
    }
    printf("Total seek: %d, Avg: %d\n", total, total / ds->count);
}

int main(void) {
    struct DiskScheduler ds;
    ds_init(&ds, 50);
    ds_add(&ds, 98); ds_add(&ds, 183); ds_add(&ds, 37);
    ds_add(&ds, 122); ds_add(&ds, 14); ds_add(&ds, 65);
    ds_add(&ds, 67);

    run_fcfs(&ds);
    printf("\n");
    run_sstf(&ds);
    printf("\n");
    run_scan(&ds);

    return 0;
}
