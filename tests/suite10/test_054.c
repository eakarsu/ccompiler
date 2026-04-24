int printf(const char *fmt, ...);

// Convex hull using Gift Wrapping (Jarvis March) on integer coords

struct Point {
    int x;
    int y;
};

// Cross product of vectors (b-a) x (c-a)
int cross(struct Point a, struct Point b, struct Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Squared distance
int dist_sq(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Find leftmost point index
int find_leftmost(struct Point *pts, int n) {
    int idx;
    int i;
    idx = 0;
    for (i = 1; i < n; i = i + 1) {
        if (pts[i].x < pts[idx].x) {
            idx = i;
        } else if (pts[i].x == pts[idx].x && pts[i].y < pts[idx].y) {
            idx = i;
        }
    }
    return idx;
}

// Convex hull using Jarvis march
// Writes hull points into hull array, returns number of hull points
int convex_hull(struct Point *pts, int n, struct Point *hull) {
    int start;
    int current;
    int next;
    int hull_size;
    int i;
    int cp;

    if (n < 3) {
        for (i = 0; i < n; i = i + 1) {
            hull[i] = pts[i];
        }
        return n;
    }

    start = find_leftmost(pts, n);
    current = start;
    hull_size = 0;

    while (1) {
        hull[hull_size] = pts[current];
        hull_size = hull_size + 1;

        next = 0;
        if (next == current) next = 1;

        for (i = 0; i < n; i = i + 1) {
            if (i == current) continue;
            cp = cross(pts[current], pts[next], pts[i]);
            if (cp > 0) {
                next = i;
            } else if (cp == 0) {
                // Collinear: pick the farther one
                if (dist_sq(pts[current], pts[i]) > dist_sq(pts[current], pts[next])) {
                    next = i;
                }
            }
        }

        current = next;
        if (current == start) break;
        if (hull_size >= n) break; // safety
    }

    return hull_size;
}

// Check if a point is inside the convex hull (using cross products)
int point_in_convex(struct Point *hull, int h, struct Point p) {
    int i;
    int j;
    int cp;
    int all_pos;
    int all_neg;
    all_pos = 1;
    all_neg = 1;
    for (i = 0; i < h; i = i + 1) {
        j = i + 1;
        if (j >= h) j = 0;
        cp = cross(hull[i], hull[j], p);
        if (cp < 0) all_pos = 0;
        if (cp > 0) all_neg = 0;
    }
    if (all_pos || all_neg) return 1;
    return 0;
}

int main() {
    struct Point pts[10];
    struct Point hull[10];
    int h;
    int i;
    struct Point test_pt;

    // Test 1: Simple square with interior point
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 4; pts[1].y = 0;
    pts[2].x = 4; pts[2].y = 4;
    pts[3].x = 0; pts[3].y = 4;
    pts[4].x = 2; pts[4].y = 2;  // interior

    h = convex_hull(pts, 5, hull);
    printf("hull size = %d\n", h); // EXPECT: hull size = 4
    for (i = 0; i < h; i = i + 1) {
        printf("hull[%d] = (%d,%d)\n", i, hull[i].x, hull[i].y);
    }
    // EXPECT: hull[0] = (0,0)
    // EXPECT: hull[1] = (0,4)
    // EXPECT: hull[2] = (4,4)
    // EXPECT: hull[3] = (4,0)

    // Test point in convex hull
    test_pt.x = 2; test_pt.y = 2;
    printf("(2,2) in hull = %d\n", point_in_convex(hull, h, test_pt)); // EXPECT: (2,2) in hull = 1

    test_pt.x = 5; test_pt.y = 5;
    printf("(5,5) in hull = %d\n", point_in_convex(hull, h, test_pt)); // EXPECT: (5,5) in hull = 0

    // Test 2: Triangle points + interior
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 6; pts[1].y = 0;
    pts[2].x = 3; pts[2].y = 6;
    pts[3].x = 3; pts[3].y = 2;  // interior
    pts[4].x = 2; pts[4].y = 1;  // interior

    h = convex_hull(pts, 5, hull);
    printf("tri hull size = %d\n", h); // EXPECT: tri hull size = 3
    for (i = 0; i < h; i = i + 1) {
        printf("tri hull[%d] = (%d,%d)\n", i, hull[i].x, hull[i].y);
    }
    // EXPECT: tri hull[0] = (0,0)
    // EXPECT: tri hull[1] = (3,6)
    // EXPECT: tri hull[2] = (6,0)

    // Test 3: L-shaped set of points
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 3; pts[1].y = 0;
    pts[2].x = 3; pts[2].y = 1;
    pts[3].x = 1; pts[3].y = 1;
    pts[4].x = 1; pts[4].y = 3;
    pts[5].x = 0; pts[5].y = 3;

    h = convex_hull(pts, 6, hull);
    printf("L hull size = %d\n", h); // EXPECT: L hull size = 5
    for (i = 0; i < h; i = i + 1) {
        printf("L hull[%d] = (%d,%d)\n", i, hull[i].x, hull[i].y);
    }
    // EXPECT: L hull[0] = (0,0)
    // EXPECT: L hull[1] = (0,3)
    // EXPECT: L hull[2] = (1,3)
    // EXPECT: L hull[3] = (3,1)
    // EXPECT: L hull[4] = (3,0)

    // Test leftmost
    printf("leftmost idx = %d\n", find_leftmost(pts, 6)); // EXPECT: leftmost idx = 0

    return 0;
}
