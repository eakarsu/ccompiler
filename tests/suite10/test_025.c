int printf(const char *fmt, ...);

// Disk Scheduler Simulation: SCAN and SSTF algorithms

struct DiskRequest {
    int cylinder;
    int served;
};

void sort_requests(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

void scan_schedule(int *requests, int n, int head, int max_cyl) {
    int sorted[20];
    int total_seek;
    int current;
    int i;
    int direction; // 1 = up, 0 = down

    printf("=== SCAN Disk Scheduling ===\n"); // EXPECT: === SCAN Disk Scheduling ===
    printf("Head starts at %d, direction=UP\n", head); // EXPECT: Head starts at 53, direction=UP

    for (i = 0; i < n; i = i + 1) {
        sorted[i] = requests[i];
    }
    sort_requests(sorted, n);

    total_seek = 0;
    current = head;
    direction = 1;

    printf("Order:");
    // SCAN going up first
    // Serve requests >= head in ascending order
    for (i = 0; i < n; i = i + 1) {
        if (sorted[i] >= current) {
            total_seek = total_seek + abs_val(sorted[i] - current);
            current = sorted[i];
            printf(" %d", sorted[i]);
        }
    }
    // Go to max cylinder
    total_seek = total_seek + abs_val(max_cyl - current);
    current = max_cyl;

    // Now serve remaining requests going down
    for (i = n - 1; i >= 0; i = i - 1) {
        if (sorted[i] < head) {
            total_seek = total_seek + abs_val(sorted[i] - current);
            current = sorted[i];
            printf(" %d", sorted[i]);
        }
    }
    printf("\n"); // EXPECT: Order: 65 67 98 122 124 183 37 14

    printf("SCAN total seek: %d\n", total_seek); // EXPECT: SCAN total seek: 331
}

void sstf_schedule(int *requests, int n, int head) {
    struct DiskRequest reqs[20];
    int total_seek;
    int current;
    int served_count;
    int i;
    int min_dist;
    int min_idx;
    int dist;

    printf("=== SSTF Disk Scheduling ===\n"); // EXPECT: === SSTF Disk Scheduling ===
    printf("Head starts at %d\n", head); // EXPECT: Head starts at 53

    for (i = 0; i < n; i = i + 1) {
        reqs[i].cylinder = requests[i];
        reqs[i].served = 0;
    }

    total_seek = 0;
    current = head;
    served_count = 0;

    printf("Order:");
    while (served_count < n) {
        min_dist = 99999;
        min_idx = -1;
        for (i = 0; i < n; i = i + 1) {
            if (reqs[i].served == 0) {
                dist = abs_val(reqs[i].cylinder - current);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_idx = i;
                }
            }
        }
        if (min_idx >= 0) {
            total_seek = total_seek + min_dist;
            current = reqs[min_idx].cylinder;
            reqs[min_idx].served = 1;
            served_count = served_count + 1;
            printf(" %d", current);
        }
    }
    printf("\n"); // EXPECT: Order: 65 67 37 14 98 122 124 183
    printf("SSTF total seek: %d\n", total_seek); // EXPECT: SSTF total seek: 236
}

void compare_algorithms(int scan_seek, int sstf_seek) {
    printf("=== Comparison ===\n"); // EXPECT: === Comparison ===
    printf("SCAN: %d\n", scan_seek); // EXPECT: SCAN: 331
    printf("SSTF: %d\n", sstf_seek); // EXPECT: SSTF: 236
    if (sstf_seek < scan_seek) {
        printf("SSTF is better by %d cylinders\n", scan_seek - sstf_seek); // EXPECT: SSTF is better by 95 cylinders
    } else {
        printf("SCAN is better by %d cylinders\n", sstf_seek - scan_seek);
    }
}

int main() {
    int requests[8];
    int n;
    int head;
    int max_cyl;

    // Cylinder requests
    requests[0] = 98;
    requests[1] = 183;
    requests[2] = 37;
    requests[3] = 122;
    requests[4] = 14;
    requests[5] = 124;
    requests[6] = 65;
    requests[7] = 67;
    n = 8;
    head = 53;
    max_cyl = 199;

    scan_schedule(requests, n, head, max_cyl);
    sstf_schedule(requests, n, head);
    compare_algorithms(331, 236);

    printf("Disk scheduler simulation complete\n"); // EXPECT: Disk scheduler simulation complete
    return 0;
}
