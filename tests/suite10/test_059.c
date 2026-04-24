int printf(const char *fmt, ...);

// Polygon perimeter using Manhattan distance between consecutive vertices
// Also: edge lengths, longest/shortest edge detection

struct Point {
    int x;
    int y;
};

int abs_val(int v) {
    if (v < 0) return -v;
    return v;
}

// Manhattan distance between two points
int manhattan_dist(struct Point a, struct Point b) {
    return abs_val(a.x - b.x) + abs_val(a.y - b.y);
}

// Squared Euclidean distance
int dist_sq(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Manhattan perimeter of polygon
int manhattan_perimeter(struct Point *poly, int n) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        sum = sum + manhattan_dist(poly[i], poly[j]);
    }
    return sum;
}

// Sum of squared edge lengths (avoids sqrt)
int sq_perimeter(struct Point *poly, int n) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        sum = sum + dist_sq(poly[i], poly[j]);
    }
    return sum;
}

// Index of longest edge (by squared distance)
int longest_edge(struct Point *poly, int n) {
    int best;
    int best_idx;
    int i;
    int j;
    int d;
    best = 0;
    best_idx = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        d = dist_sq(poly[i], poly[j]);
        if (d > best) {
            best = d;
            best_idx = i;
        }
    }
    return best_idx;
}

// Index of shortest edge (by squared distance)
int shortest_edge(struct Point *poly, int n) {
    int best;
    int best_idx;
    int i;
    int j;
    int d;
    best = 2000000000;
    best_idx = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        d = dist_sq(poly[i], poly[j]);
        if (d < best) {
            best = d;
            best_idx = i;
        }
    }
    return best_idx;
}

// Number of edges with manhattan distance > threshold
int count_long_edges(struct Point *poly, int n, int threshold) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        if (manhattan_dist(poly[i], poly[j]) > threshold) count = count + 1;
    }
    return count;
}

// Check if polygon is axis-aligned rectangle
int is_axis_aligned_rect(struct Point *poly, int n) {
    int i;
    int j;
    int dx;
    int dy;
    if (n != 4) return 0;
    for (i = 0; i < 4; i = i + 1) {
        j = i + 1;
        if (j >= 4) j = 0;
        dx = poly[j].x - poly[i].x;
        dy = poly[j].y - poly[i].y;
        // Each edge must be axis-aligned (dx=0 or dy=0)
        if (dx != 0 && dy != 0) return 0;
    }
    return 1;
}

int main() {
    struct Point square[4];
    struct Point tri[3];
    struct Point pent[5];
    struct Point diamond[4];
    int perim;
    int sq_perim;
    int idx;
    int cnt;

    // Square: (0,0)-(4,0)-(4,4)-(0,4)
    square[0].x = 0; square[0].y = 0;
    square[1].x = 4; square[1].y = 0;
    square[2].x = 4; square[2].y = 4;
    square[3].x = 0; square[3].y = 4;

    perim = manhattan_perimeter(square, 4);
    printf("square manhattan perim = %d\n", perim); // EXPECT: square manhattan perim = 16

    sq_perim = sq_perimeter(square, 4);
    printf("square sq perim = %d\n", sq_perim); // EXPECT: square sq perim = 64

    idx = longest_edge(square, 4);
    printf("square longest edge idx = %d\n", idx); // EXPECT: square longest edge idx = 0

    idx = shortest_edge(square, 4);
    printf("square shortest edge idx = %d\n", idx); // EXPECT: square shortest edge idx = 0

    printf("square is_rect = %d\n", is_axis_aligned_rect(square, 4)); // EXPECT: square is_rect = 1

    // Triangle: (0,0)-(6,0)-(3,4)
    tri[0].x = 0; tri[0].y = 0;
    tri[1].x = 6; tri[1].y = 0;
    tri[2].x = 3; tri[2].y = 4;

    perim = manhattan_perimeter(tri, 3);
    printf("tri manhattan perim = %d\n", perim); // EXPECT: tri manhattan perim = 20

    sq_perim = sq_perimeter(tri, 3);
    printf("tri sq perim = %d\n", sq_perim); // EXPECT: tri sq perim = 86

    idx = longest_edge(tri, 3);
    printf("tri longest edge idx = %d\n", idx); // EXPECT: tri longest edge idx = 0

    idx = shortest_edge(tri, 3);
    printf("tri shortest edge idx = %d\n", idx); // EXPECT: tri shortest edge idx = 1

    // Pentagon: (0,2)-(2,0)-(5,1)-(5,4)-(1,5)
    pent[0].x = 0; pent[0].y = 2;
    pent[1].x = 2; pent[1].y = 0;
    pent[2].x = 5; pent[2].y = 1;
    pent[3].x = 5; pent[3].y = 4;
    pent[4].x = 1; pent[4].y = 5;

    perim = manhattan_perimeter(pent, 5);
    printf("pent manhattan perim = %d\n", perim); // EXPECT: pent manhattan perim = 20

    cnt = count_long_edges(pent, 5, 4);
    printf("pent edges > 4 manhattan = %d\n", cnt); // EXPECT: pent edges > 4 manhattan = 1

    idx = longest_edge(pent, 5);
    printf("pent longest edge idx = %d\n", idx); // EXPECT: pent longest edge idx = 3

    idx = shortest_edge(pent, 5);
    printf("pent shortest edge idx = %d\n", idx); // EXPECT: pent shortest edge idx = 0

    // Diamond (tilted square): (3,0)-(6,3)-(3,6)-(0,3)
    diamond[0].x = 3; diamond[0].y = 0;
    diamond[1].x = 6; diamond[1].y = 3;
    diamond[2].x = 3; diamond[2].y = 6;
    diamond[3].x = 0; diamond[3].y = 3;

    perim = manhattan_perimeter(diamond, 4);
    printf("diamond manhattan perim = %d\n", perim); // EXPECT: diamond manhattan perim = 24

    sq_perim = sq_perimeter(diamond, 4);
    printf("diamond sq perim = %d\n", sq_perim); // EXPECT: diamond sq perim = 72

    printf("diamond is_rect = %d\n", is_axis_aligned_rect(diamond, 4)); // EXPECT: diamond is_rect = 0

    // Edge distance tests
    printf("manhattan (0,0)-(3,4) = %d\n", manhattan_dist(square[0], tri[2])); // EXPECT: manhattan (0,0)-(3,4) = 7
    printf("dist_sq (0,0)-(3,4) = %d\n", dist_sq(square[0], tri[2])); // EXPECT: dist_sq (0,0)-(3,4) = 25

    return 0;
}
