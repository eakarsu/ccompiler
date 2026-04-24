int printf(const char *fmt, ...);

// Bounding box computation for sets of points and polygons

struct Point {
    int x;
    int y;
};

struct BBox {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

// Compute bounding box for an array of points
struct BBox compute_bbox(struct Point *pts, int n) {
    struct BBox bb;
    int i;
    bb.min_x = pts[0].x;
    bb.max_x = pts[0].x;
    bb.min_y = pts[0].y;
    bb.max_y = pts[0].y;
    for (i = 1; i < n; i = i + 1) {
        if (pts[i].x < bb.min_x) bb.min_x = pts[i].x;
        if (pts[i].x > bb.max_x) bb.max_x = pts[i].x;
        if (pts[i].y < bb.min_y) bb.min_y = pts[i].y;
        if (pts[i].y > bb.max_y) bb.max_y = pts[i].y;
    }
    return bb;
}

// Area of bounding box
int bbox_area(struct BBox bb) {
    return (bb.max_x - bb.min_x) * (bb.max_y - bb.min_y);
}

// Perimeter of bounding box
int bbox_perimeter(struct BBox bb) {
    return 2 * ((bb.max_x - bb.min_x) + (bb.max_y - bb.min_y));
}

// Check if a point is inside a bounding box
int point_in_bbox(struct BBox bb, struct Point p) {
    if (p.x >= bb.min_x && p.x <= bb.max_x &&
        p.y >= bb.min_y && p.y <= bb.max_y)
        return 1;
    return 0;
}

// Check if two bounding boxes overlap
int bbox_overlap(struct BBox a, struct BBox b) {
    if (a.max_x < b.min_x || b.max_x < a.min_x)
        return 0;
    if (a.max_y < b.min_y || b.max_y < a.min_y)
        return 0;
    return 1;
}

// Compute intersection of two bounding boxes
// Returns a bbox; if no overlap, area will be <= 0
struct BBox bbox_intersection(struct BBox a, struct BBox b) {
    struct BBox r;
    r.min_x = a.min_x; if (b.min_x > r.min_x) r.min_x = b.min_x;
    r.min_y = a.min_y; if (b.min_y > r.min_y) r.min_y = b.min_y;
    r.max_x = a.max_x; if (b.max_x < r.max_x) r.max_x = b.max_x;
    r.max_y = a.max_y; if (b.max_y < r.max_y) r.max_y = b.max_y;
    return r;
}

// Compute union bounding box of two bounding boxes
struct BBox bbox_union(struct BBox a, struct BBox b) {
    struct BBox r;
    r.min_x = a.min_x; if (b.min_x < r.min_x) r.min_x = b.min_x;
    r.min_y = a.min_y; if (b.min_y < r.min_y) r.min_y = b.min_y;
    r.max_x = a.max_x; if (b.max_x > r.max_x) r.max_x = b.max_x;
    r.max_y = a.max_y; if (b.max_y > r.max_y) r.max_y = b.max_y;
    return r;
}

// Width and height
int bbox_width(struct BBox bb) {
    return bb.max_x - bb.min_x;
}

int bbox_height(struct BBox bb) {
    return bb.max_y - bb.min_y;
}

// Center of bounding box (integer division)
struct Point bbox_center(struct BBox bb) {
    struct Point c;
    c.x = (bb.min_x + bb.max_x) / 2;
    c.y = (bb.min_y + bb.max_y) / 2;
    return c;
}

// Expand bounding box by margin
struct BBox bbox_expand(struct BBox bb, int margin) {
    struct BBox r;
    r.min_x = bb.min_x - margin;
    r.min_y = bb.min_y - margin;
    r.max_x = bb.max_x + margin;
    r.max_y = bb.max_y + margin;
    return r;
}

int main() {
    struct Point pts[6];
    struct BBox bb;
    struct BBox bb2;
    struct BBox inter;
    struct BBox uni;
    struct BBox expanded;
    struct Point p;
    struct Point center;

    // Test 1: basic bounding box
    pts[0].x = 1; pts[0].y = 2;
    pts[1].x = 5; pts[1].y = 8;
    pts[2].x = 3; pts[2].y = -1;
    pts[3].x = -2; pts[3].y = 4;

    bb = compute_bbox(pts, 4);
    printf("bb = (%d,%d)-(%d,%d)\n", bb.min_x, bb.min_y, bb.max_x, bb.max_y); // EXPECT: bb = (-2,-1)-(5,8)
    printf("bb area = %d\n", bbox_area(bb)); // EXPECT: bb area = 63
    printf("bb perimeter = %d\n", bbox_perimeter(bb)); // EXPECT: bb perimeter = 32
    printf("bb width = %d\n", bbox_width(bb)); // EXPECT: bb width = 7
    printf("bb height = %d\n", bbox_height(bb)); // EXPECT: bb height = 9

    center = bbox_center(bb);
    printf("bb center = (%d,%d)\n", center.x, center.y); // EXPECT: bb center = (1,3)

    // Point in bbox tests
    p.x = 3; p.y = 4;
    printf("(3,4) in bb = %d\n", point_in_bbox(bb, p)); // EXPECT: (3,4) in bb = 1
    p.x = 6; p.y = 4;
    printf("(6,4) in bb = %d\n", point_in_bbox(bb, p)); // EXPECT: (6,4) in bb = 0
    p.x = -2; p.y = -1;
    printf("(-2,-1) in bb = %d\n", point_in_bbox(bb, p)); // EXPECT: (-2,-1) in bb = 1

    // Test 2: second bbox
    pts[0].x = 3; pts[0].y = 3;
    pts[1].x = 10; pts[1].y = 6;
    bb2 = compute_bbox(pts, 2);
    printf("bb2 = (%d,%d)-(%d,%d)\n", bb2.min_x, bb2.min_y, bb2.max_x, bb2.max_y); // EXPECT: bb2 = (3,3)-(10,6)

    // Overlap test
    printf("bb overlap bb2 = %d\n", bbox_overlap(bb, bb2)); // EXPECT: bb overlap bb2 = 1

    // Intersection
    inter = bbox_intersection(bb, bb2);
    printf("intersection = (%d,%d)-(%d,%d)\n", inter.min_x, inter.min_y, inter.max_x, inter.max_y); // EXPECT: intersection = (3,3)-(5,6)
    printf("inter area = %d\n", bbox_area(inter)); // EXPECT: inter area = 6

    // Union
    uni = bbox_union(bb, bb2);
    printf("union = (%d,%d)-(%d,%d)\n", uni.min_x, uni.min_y, uni.max_x, uni.max_y); // EXPECT: union = (-2,-1)-(10,8)
    printf("union area = %d\n", bbox_area(uni)); // EXPECT: union area = 108

    // Non-overlapping boxes
    bb2.min_x = 20; bb2.min_y = 20; bb2.max_x = 30; bb2.max_y = 30;
    printf("bb overlap far = %d\n", bbox_overlap(bb, bb2)); // EXPECT: bb overlap far = 0

    // Expand test
    expanded = bbox_expand(bb, 2);
    printf("expanded = (%d,%d)-(%d,%d)\n", expanded.min_x, expanded.min_y, expanded.max_x, expanded.max_y); // EXPECT: expanded = (-4,-3)-(7,10)
    printf("expanded area = %d\n", bbox_area(expanded)); // EXPECT: expanded area = 143

    // Single point bbox
    pts[0].x = 7; pts[0].y = 7;
    bb = compute_bbox(pts, 1);
    printf("single pt bb = (%d,%d)-(%d,%d)\n", bb.min_x, bb.min_y, bb.max_x, bb.max_y); // EXPECT: single pt bb = (7,7)-(7,7)
    printf("single pt area = %d\n", bbox_area(bb)); // EXPECT: single pt area = 0

    return 0;
}
