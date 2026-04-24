int printf(const char *fmt, ...);

// Point distance squared - no floating point needed
// Compute squared Euclidean distance between 2D points

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

// Manhattan distance between two points
int manhattan(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

// Chebyshev distance (max of abs differences)
int chebyshev(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx > dy) return dx;
    return dy;
}

// Check if point is within squared radius of origin
int within_radius_sq(struct Point p, int rsq) {
    int d;
    d = p.x * p.x + p.y * p.y;
    if (d <= rsq) return 1;
    return 0;
}

// Find the closest point in an array to a given target
int closest_index(struct Point *pts, int n, struct Point target) {
    int best_idx;
    int best_dist;
    int i;
    int d;
    best_idx = 0;
    best_dist = dist_sq(pts[0], target);
    for (i = 1; i < n; i = i + 1) {
        d = dist_sq(pts[i], target);
        if (d < best_dist) {
            best_dist = d;
            best_idx = i;
        }
    }
    return best_idx;
}

// Find the farthest point in an array from a given target
int farthest_index(struct Point *pts, int n, struct Point target) {
    int best_idx;
    int best_dist;
    int i;
    int d;
    best_idx = 0;
    best_dist = dist_sq(pts[0], target);
    for (i = 1; i < n; i = i + 1) {
        d = dist_sq(pts[i], target);
        if (d > best_dist) {
            best_dist = d;
            best_idx = i;
        }
    }
    return best_idx;
}

// Sum of all pairwise squared distances
int total_pairwise_dist_sq(struct Point *pts, int n) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            sum = sum + dist_sq(pts[i], pts[j]);
        }
    }
    return sum;
}

int main() {
    struct Point a;
    struct Point b;
    struct Point c;
    struct Point origin;
    struct Point pts[5];
    struct Point target;
    int d;
    int idx;
    int total;

    a.x = 0; a.y = 0;
    b.x = 3; b.y = 4;
    d = dist_sq(a, b);
    printf("dist_sq(0,0)-(3,4) = %d\n", d); // EXPECT: dist_sq(0,0)-(3,4) = 25

    c.x = -1; c.y = -1;
    d = dist_sq(b, c);
    printf("dist_sq(3,4)-(-1,-1) = %d\n", d); // EXPECT: dist_sq(3,4)-(-1,-1) = 41

    d = manhattan(a, b);
    printf("manhattan(0,0)-(3,4) = %d\n", d); // EXPECT: manhattan(0,0)-(3,4) = 7

    d = manhattan(c, b);
    printf("manhattan(-1,-1)-(3,4) = %d\n", d); // EXPECT: manhattan(-1,-1)-(3,4) = 9

    d = chebyshev(a, b);
    printf("chebyshev(0,0)-(3,4) = %d\n", d); // EXPECT: chebyshev(0,0)-(3,4) = 4

    d = chebyshev(c, b);
    printf("chebyshev(-1,-1)-(3,4) = %d\n", d); // EXPECT: chebyshev(-1,-1)-(3,4) = 5

    origin.x = 0; origin.y = 0;
    b.x = 2; b.y = 2;
    printf("within_r2(2,2,10) = %d\n", within_radius_sq(b, 10)); // EXPECT: within_r2(2,2,10) = 1
    printf("within_r2(2,2,7) = %d\n", within_radius_sq(b, 7));   // EXPECT: within_r2(2,2,7) = 0

    pts[0].x = 1;  pts[0].y = 1;
    pts[1].x = 5;  pts[1].y = 5;
    pts[2].x = -3; pts[2].y = 2;
    pts[3].x = 7;  pts[3].y = -1;
    pts[4].x = 0;  pts[4].y = 4;

    target.x = 0; target.y = 0;
    idx = closest_index(pts, 5, target);
    printf("closest to origin: idx=%d (%d,%d)\n", idx, pts[idx].x, pts[idx].y); // EXPECT: closest to origin: idx=0 (1,1)

    idx = farthest_index(pts, 5, target);
    printf("farthest from origin: idx=%d (%d,%d)\n", idx, pts[idx].x, pts[idx].y); // EXPECT: farthest from origin: idx=1 (5,5)

    target.x = 4; target.y = 4;
    idx = closest_index(pts, 5, target);
    printf("closest to (4,4): idx=%d (%d,%d)\n", idx, pts[idx].x, pts[idx].y); // EXPECT: closest to (4,4): idx=1 (5,5)

    total = total_pairwise_dist_sq(pts, 5);
    printf("total pairwise dist_sq = %d\n", total); // EXPECT: total pairwise dist_sq = 434

    return 0;
}
