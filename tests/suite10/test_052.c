int printf(const char *fmt, ...);

// Line segment intersection detection using cross products (integer only)
// Uses the orientation method: cross product sign determines turn direction

struct Point {
    int x;
    int y;
};

struct Segment {
    struct Point p;
    struct Point q;
};

// Cross product of vectors (b-a) x (c-a)
int cross(struct Point a, struct Point b, struct Point c) {
    int v1x;
    int v1y;
    int v2x;
    int v2y;
    v1x = b.x - a.x;
    v1y = b.y - a.y;
    v2x = c.x - a.x;
    v2y = c.y - a.y;
    return v1x * v2y - v1y * v2x;
}

// Sign function: returns -1, 0, or 1
int sign(int val) {
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}

// Orientation: 0=collinear, 1=clockwise, 2=counterclockwise
int orientation(struct Point p, struct Point q, struct Point r) {
    int val;
    val = cross(p, q, r);
    if (val == 0) return 0;
    if (val > 0) return 2;
    return 1;
}

// Check if point q lies on segment pr (assuming collinear)
int on_segment(struct Point p, struct Point r, struct Point q) {
    int minx;
    int maxx;
    int miny;
    int maxy;
    minx = p.x; if (r.x < minx) minx = r.x;
    maxx = p.x; if (r.x > maxx) maxx = r.x;
    miny = p.y; if (r.y < miny) miny = r.y;
    maxy = p.y; if (r.y > maxy) maxy = r.y;
    if (q.x >= minx && q.x <= maxx && q.y >= miny && q.y <= maxy)
        return 1;
    return 0;
}

// Check if two segments intersect
int segments_intersect(struct Segment s1, struct Segment s2) {
    int o1;
    int o2;
    int o3;
    int o4;

    o1 = orientation(s1.p, s1.q, s2.p);
    o2 = orientation(s1.p, s1.q, s2.q);
    o3 = orientation(s2.p, s2.q, s1.p);
    o4 = orientation(s2.p, s2.q, s1.q);

    // General case
    if (o1 != o2 && o3 != o4) return 1;

    // Collinear special cases
    if (o1 == 0 && on_segment(s1.p, s1.q, s2.p)) return 1;
    if (o2 == 0 && on_segment(s1.p, s1.q, s2.q)) return 1;
    if (o3 == 0 && on_segment(s2.p, s2.q, s1.p)) return 1;
    if (o4 == 0 && on_segment(s2.p, s2.q, s1.q)) return 1;

    return 0;
}

// Check if a point is strictly left of the directed line from a to b
int is_left(struct Point a, struct Point b, struct Point p) {
    int cp;
    cp = cross(a, b, p);
    if (cp > 0) return 1;
    return 0;
}

int main() {
    struct Segment s1;
    struct Segment s2;
    struct Point p;
    struct Point q;
    struct Point r;
    int result;

    // Test cross product
    p.x = 0; p.y = 0;
    q.x = 1; q.y = 0;
    r.x = 0; r.y = 1;
    printf("cross(0,0)(1,0)(0,1) = %d\n", cross(p, q, r)); // EXPECT: cross(0,0)(1,0)(0,1) = 1

    r.x = 0; r.y = -1;
    printf("cross(0,0)(1,0)(0,-1) = %d\n", cross(p, q, r)); // EXPECT: cross(0,0)(1,0)(0,-1) = -1

    r.x = 2; r.y = 0;
    printf("cross(0,0)(1,0)(2,0) = %d\n", cross(p, q, r)); // EXPECT: cross(0,0)(1,0)(2,0) = 0

    // Test orientation
    p.x = 0; p.y = 0;
    q.x = 4; q.y = 4;
    r.x = 1; r.y = 2;
    printf("orient(0,0)(4,4)(1,2) = %d\n", orientation(p, q, r)); // EXPECT: orient(0,0)(4,4)(1,2) = 2

    r.x = 2; r.y = 1;
    printf("orient(0,0)(4,4)(2,1) = %d\n", orientation(p, q, r)); // EXPECT: orient(0,0)(4,4)(2,1) = 1

    r.x = 2; r.y = 2;
    printf("orient(0,0)(4,4)(2,2) = %d\n", orientation(p, q, r)); // EXPECT: orient(0,0)(4,4)(2,2) = 0

    // Test segment intersection: crossing segments
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 4; s1.q.y = 4;
    s2.p.x = 0; s2.p.y = 4; s2.q.x = 4; s2.q.y = 0;
    result = segments_intersect(s1, s2);
    printf("cross segs: %d\n", result); // EXPECT: cross segs: 1

    // Parallel non-overlapping
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 4; s1.q.y = 0;
    s2.p.x = 0; s2.p.y = 2; s2.q.x = 4; s2.q.y = 2;
    result = segments_intersect(s1, s2);
    printf("parallel segs: %d\n", result); // EXPECT: parallel segs: 0

    // Touching at endpoint
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 2; s1.q.y = 2;
    s2.p.x = 2; s2.p.y = 2; s2.q.x = 4; s2.q.y = 0;
    result = segments_intersect(s1, s2);
    printf("touch endpoint: %d\n", result); // EXPECT: touch endpoint: 1

    // T-intersection
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 4; s1.q.y = 0;
    s2.p.x = 2; s2.p.y = -2; s2.q.x = 2; s2.q.y = 0;
    result = segments_intersect(s1, s2);
    printf("T-intersect: %d\n", result); // EXPECT: T-intersect: 1

    // Non-intersecting
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 1; s1.q.y = 1;
    s2.p.x = 3; s2.p.y = 3; s2.q.x = 4; s2.q.y = 4;
    result = segments_intersect(s1, s2);
    printf("no intersect: %d\n", result); // EXPECT: no intersect: 0

    // is_left tests
    p.x = 0; p.y = 0;
    q.x = 4; q.y = 0;
    r.x = 2; r.y = 3;
    printf("is_left (2,3): %d\n", is_left(p, q, r)); // EXPECT: is_left (2,3): 1
    r.x = 2; r.y = -3;
    printf("is_left (2,-3): %d\n", is_left(p, q, r)); // EXPECT: is_left (2,-3): 0

    // Collinear overlapping segments
    s1.p.x = 0; s1.p.y = 0; s1.q.x = 4; s1.q.y = 0;
    s2.p.x = 2; s2.p.y = 0; s2.q.x = 6; s2.q.y = 0;
    result = segments_intersect(s1, s2);
    printf("collinear overlap: %d\n", result); // EXPECT: collinear overlap: 1

    return 0;
}
