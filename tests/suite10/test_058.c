int printf(const char *fmt, ...);

// Closest pair of points using brute force O(n^2)
// Returns squared distance to avoid floating point

struct Point {
    int x;
    int y;
};

int dist_sq(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Find the closest pair of points, return the squared distance
// Also output the indices via pointers
int closest_pair(struct Point *pts, int n, int *idx1, int *idx2) {
    int best;
    int d;
    int i;
    int j;
    best = 2000000000; // large sentinel
    *idx1 = 0;
    *idx2 = 1;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            d = dist_sq(pts[i], pts[j]);
            if (d < best) {
                best = d;
                *idx1 = i;
                *idx2 = j;
            }
        }
    }
    return best;
}

// Find the farthest pair (diameter of point set)
int farthest_pair(struct Point *pts, int n, int *idx1, int *idx2) {
    int best;
    int d;
    int i;
    int j;
    best = 0;
    *idx1 = 0;
    *idx2 = 1;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            d = dist_sq(pts[i], pts[j]);
            if (d > best) {
                best = d;
                *idx1 = i;
                *idx2 = j;
            }
        }
    }
    return best;
}

// Find the k-th closest pair (0-indexed)
// Simple approach: find closest, mark, find next closest, etc.
int kth_closest_pair(struct Point *pts, int n, int k) {
    int pairs[100]; // store all pairwise distances squared (max 100)
    int count;
    int i;
    int j;
    int temp;
    int swapped;

    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            pairs[count] = dist_sq(pts[i], pts[j]);
            count = count + 1;
        }
    }

    // Bubble sort the distances
    swapped = 1;
    while (swapped) {
        swapped = 0;
        for (i = 0; i < count - 1; i = i + 1) {
            if (pairs[i] > pairs[i + 1]) {
                temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                swapped = 1;
            }
        }
    }

    if (k < count) return pairs[k];
    return -1;
}

// Count how many pairs have distance squared <= threshold
int count_close_pairs(struct Point *pts, int n, int threshold) {
    int count;
    int i;
    int j;
    int d;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            d = dist_sq(pts[i], pts[j]);
            if (d <= threshold) count = count + 1;
        }
    }
    return count;
}

// Compute average squared distance (integer division)
int avg_dist_sq(struct Point *pts, int n) {
    int sum;
    int count;
    int i;
    int j;
    sum = 0;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            sum = sum + dist_sq(pts[i], pts[j]);
            count = count + 1;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

int main() {
    struct Point pts[8];
    int d;
    int i1;
    int i2;
    int cnt;
    int avg;

    // Test 1: simple points
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 3; pts[1].y = 4;
    pts[2].x = 1; pts[2].y = 1;
    pts[3].x = 6; pts[3].y = 7;
    pts[4].x = 10; pts[4].y = 0;

    d = closest_pair(pts, 5, &i1, &i2);
    printf("closest dist_sq = %d\n", d); // EXPECT: closest dist_sq = 2
    printf("closest pair: (%d,%d) (%d,%d)\n", pts[i1].x, pts[i1].y, pts[i2].x, pts[i2].y); // EXPECT: closest pair: (0,0) (1,1)

    d = farthest_pair(pts, 5, &i1, &i2);
    printf("farthest dist_sq = %d\n", d); // EXPECT: farthest dist_sq = 100
    printf("farthest pair: (%d,%d) (%d,%d)\n", pts[i1].x, pts[i1].y, pts[i2].x, pts[i2].y); // EXPECT: farthest pair: (0,0) (10,0)

    // kth closest
    printf("0th closest = %d\n", kth_closest_pair(pts, 5, 0)); // EXPECT: 0th closest = 2
    printf("1st closest = %d\n", kth_closest_pair(pts, 5, 1)); // EXPECT: 1st closest = 13
    printf("2nd closest = %d\n", kth_closest_pair(pts, 5, 2)); // EXPECT: 2nd closest = 18

    // Count close pairs
    cnt = count_close_pairs(pts, 5, 20);
    printf("pairs within 20 sq = %d\n", cnt); // EXPECT: pairs within 20 sq = 3

    cnt = count_close_pairs(pts, 5, 2);
    printf("pairs within 2 sq = %d\n", cnt); // EXPECT: pairs within 2 sq = 1

    // Average
    avg = avg_dist_sq(pts, 5);
    printf("avg dist_sq = %d\n", avg); // EXPECT: avg dist_sq = 51

    // Test 2: clustered points
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 1; pts[1].y = 0;
    pts[2].x = 0; pts[2].y = 1;
    pts[3].x = 100; pts[3].y = 100;
    pts[4].x = 101; pts[4].y = 100;
    pts[5].x = 100; pts[5].y = 101;

    d = closest_pair(pts, 6, &i1, &i2);
    printf("cluster closest = %d\n", d); // EXPECT: cluster closest = 1
    printf("cluster pair: (%d,%d) (%d,%d)\n", pts[i1].x, pts[i1].y, pts[i2].x, pts[i2].y); // EXPECT: cluster pair: (0,0) (1,0)

    d = farthest_pair(pts, 6, &i1, &i2);
    printf("cluster farthest = %d\n", d); // EXPECT: cluster farthest = 20201
    printf("cluster far pair: (%d,%d) (%d,%d)\n", pts[i1].x, pts[i1].y, pts[i2].x, pts[i2].y); // EXPECT: cluster far pair: (0,0) (101,100)

    // All same point
    pts[0].x = 5; pts[0].y = 5;
    pts[1].x = 5; pts[1].y = 5;
    pts[2].x = 5; pts[2].y = 5;
    d = closest_pair(pts, 3, &i1, &i2);
    printf("same pts closest = %d\n", d); // EXPECT: same pts closest = 0

    return 0;
}
