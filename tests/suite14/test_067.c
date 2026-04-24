int printf(const char *fmt, ...);

// Disk scheduling algorithms: FCFS, SSTF, SCAN
// Simulates head movement on a disk with tracks 0..199

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Sort array in place (insertion sort)
void sort_arr(int *arr, int n) {
    int i;
    int j;
    int key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// FCFS disk scheduling
int fcfs(int *requests, int n, int head) {
    int total = 0;
    int cur = head;
    int i;
    for (i = 0; i < n; i++) {
        total = total + abs_val(requests[i] - cur);
        cur = requests[i];
    }
    return total;
}

// SSTF: always go to nearest request
int sstf(int *requests, int n, int head) {
    int visited[16];
    int i;
    for (i = 0; i < n; i++) visited[i] = 0;

    int total = 0;
    int cur = head;
    int done = 0;

    while (done < n) {
        int best = -1;
        int best_dist = 999999;
        for (i = 0; i < n; i++) {
            if (!visited[i]) {
                int d = abs_val(requests[i] - cur);
                if (d < best_dist) {
                    best_dist = d;
                    best = i;
                }
            }
        }
        visited[best] = 1;
        total = total + best_dist;
        cur = requests[best];
        done++;
    }
    return total;
}

// SCAN (elevator): move in one direction, then reverse
// direction: 1 = moving up (towards higher tracks)
int scan(int *requests, int n, int head, int direction, int max_track) {
    int sorted[16];
    int i;
    for (i = 0; i < n; i++) sorted[i] = requests[i];
    sort_arr(sorted, n);

    int total = 0;
    int cur = head;

    if (direction == 1) {
        // Go up first
        // Service requests >= head
        for (i = 0; i < n; i++) {
            if (sorted[i] >= head) {
                total = total + abs_val(sorted[i] - cur);
                cur = sorted[i];
            }
        }
        // Go to end if any request was above
        if (cur != head) {
            // Already at highest request; reverse
        }
        // Now go down: service requests < head in reverse
        int j;
        for (j = n - 1; j >= 0; j--) {
            if (sorted[j] < head) {
                total = total + abs_val(sorted[j] - cur);
                cur = sorted[j];
            }
        }
    } else {
        // Go down first
        for (i = n - 1; i >= 0; i--) {
            if (sorted[i] <= head) {
                total = total + abs_val(sorted[i] - cur);
                cur = sorted[i];
            }
        }
        // Reverse: go up
        for (i = 0; i < n; i++) {
            if (sorted[i] > head) {
                total = total + abs_val(sorted[i] - cur);
                cur = sorted[i];
            }
        }
    }

    return total;
}

int main(void) {
    // Test case 1: Classic example
    int req1[8];
    req1[0] = 98;
    req1[1] = 183;
    req1[2] = 37;
    req1[3] = 122;
    req1[4] = 14;
    req1[5] = 124;
    req1[6] = 65;
    req1[7] = 67;
    int head1 = 53;
    int n1 = 8;

    // FCFS: |98-53| + |183-98| + |37-183| + |122-37| + |14-122| + |124-14| + |65-124| + |67-65|
    //     = 45 + 85 + 146 + 85 + 108 + 110 + 59 + 2 = 640
    int f1 = fcfs(req1, n1, head1);
    printf("FCFS total movement: %d\n", f1);
    // EXPECT: FCFS total movement: 640

    // SSTF from head=53:
    // Nearest: 65 (dist 12), cur=65
    // Nearest: 67 (dist 2), cur=67
    // Nearest: 37 (dist 30), cur=37
    // Nearest: 14 (dist 23), cur=14
    // Nearest: 98 (dist 84), cur=98
    // Nearest: 122 (dist 24), cur=122
    // Nearest: 124 (dist 2), cur=124
    // Nearest: 183 (dist 59), cur=183
    // Total: 12+2+30+23+84+24+2+59 = 236
    int s1 = sstf(req1, n1, head1);
    printf("SSTF total movement: %d\n", s1);
    // EXPECT: SSTF total movement: 236

    // SCAN going up from head=53:
    // Sorted: 14 37 65 67 98 122 124 183
    // Go up: 65(+12) 67(+2) 98(+31) 122(+24) 124(+2) 183(+59) cur=183
    // Then down: 37(+146) 14(+23)
    // Total: 12+2+31+24+2+59+146+23 = 299
    int sc1 = scan(req1, n1, head1, 1, 199);
    printf("SCAN-up total movement: %d\n", sc1);
    // EXPECT: SCAN-up total movement: 299

    // SCAN going down from head=53:
    // Sorted: 14 37 65 67 98 122 124 183
    // Go down: 37(+16) 14(+23) cur=14
    // Then up: 65(+51) 67(+2) 98(+31) 122(+24) 124(+2) 183(+59)
    // Total: 16+23+51+2+31+24+2+59 = 208
    int sc2 = scan(req1, n1, head1, 0, 199);
    printf("SCAN-down total movement: %d\n", sc2);
    // EXPECT: SCAN-down total movement: 208

    // Test case 2: Requests in one direction
    int req2[4];
    req2[0] = 100;
    req2[1] = 110;
    req2[2] = 120;
    req2[3] = 130;
    int head2 = 90;

    int f2 = fcfs(req2, 4, head2);
    printf("One-dir FCFS: %d\n", f2);
    // EXPECT: One-dir FCFS: 40

    int s2 = sstf(req2, 4, head2);
    printf("One-dir SSTF: %d\n", s2);
    // EXPECT: One-dir SSTF: 40

    int sc3 = scan(req2, 4, head2, 1, 199);
    printf("One-dir SCAN-up: %d\n", sc3);
    // EXPECT: One-dir SCAN-up: 40

    // Test case 3: Head in middle of cluster
    int req3[4];
    req3[0] = 10;
    req3[1] = 20;
    req3[2] = 180;
    req3[3] = 190;
    int head3 = 100;

    // FCFS: |10-100|+|20-10|+|180-20|+|190-180| = 90+10+160+10 = 270
    int f3 = fcfs(req3, 4, head3);
    printf("Split FCFS: %d\n", f3);
    // EXPECT: Split FCFS: 270

    // SSTF: nearest from 100: 20(dist80) vs 10(dist90) vs 180(dist80) vs 190(dist90)
    // Tie between 20(80) and 180(80) -> 20 comes first by index? No, 10 has dist 90.
    // Actually: 10->90, 20->80, 180->80, 190->90. Ties: 20 and 180 both at dist 80
    // Index 1 (val 20) checked before index 2 (val 180) -> pick 20
    // From 20: 10(10) 180(160) 190(170) -> pick 10 (dist 10)
    // From 10: 180(170) 190(180) -> pick 180 (dist 170)
    // From 180: 190(10) -> pick 190 (dist 10)
    // Total: 80+10+170+10 = 270
    int s3 = sstf(req3, 4, head3);
    printf("Split SSTF: %d\n", s3);
    // EXPECT: Split SSTF: 270

    // SCAN up from 100: 180(+80) 190(+10) then down: 20(+170) 10(+10)
    // Total: 80+10+170+10 = 270
    int sc4 = scan(req3, 4, head3, 1, 199);
    printf("Split SCAN-up: %d\n", sc4);
    // EXPECT: Split SCAN-up: 270

    // All algorithms same when requests split equally!
    printf("All equal: %d\n", (f3 == s3 && s3 == sc4) ? 1 : 0);
    // EXPECT: All equal: 1

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
