int printf(const char *fmt, ...);
// EXPECT: Closest: (2,3)-(3,4), dist^2=2\nFarthest: (2,3)-(40,50), dist^2=3653\nPairs within dist^2=100: 3\nUnit square closest: dist^2=1\nUnit square farthest: dist^2=2\nCollinear closest: dist^2=9\nCollinear farthest: dist^2=81
// Test: Closest pair of points (brute force for small N)

typedef struct {
    int x;
    int y;
} Point;

int dist_sq(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int closest_pair(Point pts[], int n, int *pi, int *pj) {
    int min_dist = 999999999;
    int i, j;
    *pi = 0;
    *pj = 1;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = dist_sq(pts[i], pts[j]);
            if (d < min_dist) {
                min_dist = d;
                *pi = i;
                *pj = j;
            }
        }
    }
    return min_dist;
}

int farthest_pair(Point pts[], int n, int *pi, int *pj) {
    int max_dist = 0;
    int i, j;
    *pi = 0;
    *pj = 1;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = dist_sq(pts[i], pts[j]);
            if (d > max_dist) {
                max_dist = d;
                *pi = i;
                *pj = j;
            }
        }
    }
    return max_dist;
}

int count_pairs_within(Point pts[], int n, int max_dist_sq) {
    int count = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (dist_sq(pts[i], pts[j]) <= max_dist_sq) {
                count++;
            }
        }
    }
    return count;
}

int main(void) {
    Point pts[] = {{2,3}, {12,30}, {40,50}, {5,1}, {12,10}, {3,4}};
    int n = 6;
    int pi, pj;

    int d = closest_pair(pts, n, &pi, &pj);
    printf("Closest: (%d,%d)-(%d,%d), dist^2=%d\n",
           pts[pi].x, pts[pi].y, pts[pj].x, pts[pj].y, d);

    d = farthest_pair(pts, n, &pi, &pj);
    printf("Farthest: (%d,%d)-(%d,%d), dist^2=%d\n",
           pts[pi].x, pts[pi].y, pts[pj].x, pts[pj].y, d);

    printf("Pairs within dist^2=100: %d\n", count_pairs_within(pts, n, 100));

    Point pts2[] = {{0,0}, {1,0}, {0,1}, {1,1}};
    d = closest_pair(pts2, 4, &pi, &pj);
    printf("Unit square closest: dist^2=%d\n", d);
    d = farthest_pair(pts2, 4, &pi, &pj);
    printf("Unit square farthest: dist^2=%d\n", d);

    Point pts3[] = {{0,0}, {3,0}, {6,0}, {9,0}};
    d = closest_pair(pts3, 4, &pi, &pj);
    printf("Collinear closest: dist^2=%d\n", d);
    d = farthest_pair(pts3, 4, &pi, &pj);
    printf("Collinear farthest: dist^2=%d\n", d);

    return 0;
}
