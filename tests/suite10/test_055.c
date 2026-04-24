int printf(const char *fmt, ...);

// Point-in-polygon test using ray casting algorithm (integer coordinates)
// Cast a horizontal ray to the right and count crossings

struct Point {
    int x;
    int y;
};

// Ray casting: count how many polygon edges the ray from (px, py) to (+inf, py) crosses
// Returns 1 if inside, 0 if outside
int point_in_polygon(struct Point *poly, int n, struct Point p) {
    int inside;
    int i;
    int j;
    int yi;
    int yj;
    int xi;
    int xj;
    int cross_x_num;
    int cross_x_den;

    inside = 0;
    j = n - 1;
    for (i = 0; i < n; i = i + 1) {
        yi = poly[i].y;
        yj = poly[j].y;
        xi = poly[i].x;
        xj = poly[j].x;

        // Check if the edge crosses the horizontal line at p.y
        if ((yi > p.y) != (yj > p.y)) {
            // Compute x-coordinate of crossing using integer math
            // cross_x = xi + (xj - xi) * (p.y - yi) / (yj - yi)
            // We compare: p.x < cross_x
            // i.e. p.x * (yj - yi) < (xi * (yj - yi) + (xj - xi) * (p.y - yi))
            cross_x_num = xi * (yj - yi) + (xj - xi) * (p.y - yi);
            cross_x_den = yj - yi;
            // Handle sign: if den < 0, flip both
            if (cross_x_den < 0) {
                cross_x_num = -cross_x_num;
                cross_x_den = -cross_x_den;
            }
            if (p.x * cross_x_den < cross_x_num) {
                inside = 1 - inside;
            }
        }
        j = i;
    }
    return inside;
}

// Winding number method (simplified for convex polygons)
// Counts the number of times the polygon winds around the point
int winding_number(struct Point *poly, int n, struct Point p) {
    int wn;
    int i;
    int j;
    int cross_val;
    wn = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        if (poly[i].y <= p.y) {
            if (poly[j].y > p.y) {
                // Upward crossing
                cross_val = (poly[j].x - poly[i].x) * (p.y - poly[i].y)
                          - (p.x - poly[i].x) * (poly[j].y - poly[i].y);
                if (cross_val > 0) wn = wn + 1;
            }
        } else {
            if (poly[j].y <= p.y) {
                // Downward crossing
                cross_val = (poly[j].x - poly[i].x) * (p.y - poly[i].y)
                          - (p.x - poly[i].x) * (poly[j].y - poly[i].y);
                if (cross_val < 0) wn = wn - 1;
            }
        }
    }
    return wn;
}

int main() {
    struct Point square[4];
    struct Point tri[3];
    struct Point lshape[6];
    struct Point p;
    int result;
    int wn;

    // Square (0,0)-(10,0)-(10,10)-(0,10)
    square[0].x = 0;  square[0].y = 0;
    square[1].x = 10; square[1].y = 0;
    square[2].x = 10; square[2].y = 10;
    square[3].x = 0;  square[3].y = 10;

    // Inside
    p.x = 5; p.y = 5;
    result = point_in_polygon(square, 4, p);
    printf("sq (5,5) inside = %d\n", result); // EXPECT: sq (5,5) inside = 1

    // Outside
    p.x = 15; p.y = 5;
    result = point_in_polygon(square, 4, p);
    printf("sq (15,5) inside = %d\n", result); // EXPECT: sq (15,5) inside = 0

    // Just outside left
    p.x = -1; p.y = 5;
    result = point_in_polygon(square, 4, p);
    printf("sq (-1,5) inside = %d\n", result); // EXPECT: sq (-1,5) inside = 0

    // Inside near corner
    p.x = 1; p.y = 1;
    result = point_in_polygon(square, 4, p);
    printf("sq (1,1) inside = %d\n", result); // EXPECT: sq (1,1) inside = 1

    // Triangle (0,0)-(8,0)-(4,6)
    tri[0].x = 0; tri[0].y = 0;
    tri[1].x = 8; tri[1].y = 0;
    tri[2].x = 4; tri[2].y = 6;

    p.x = 4; p.y = 2;
    result = point_in_polygon(tri, 3, p);
    printf("tri (4,2) inside = %d\n", result); // EXPECT: tri (4,2) inside = 1

    p.x = 1; p.y = 5;
    result = point_in_polygon(tri, 3, p);
    printf("tri (1,5) inside = %d\n", result); // EXPECT: tri (1,5) inside = 0

    p.x = 7; p.y = 1;
    result = point_in_polygon(tri, 3, p);
    printf("tri (7,1) inside = %d\n", result); // EXPECT: tri (7,1) inside = 1

    // L-shaped polygon
    lshape[0].x = 0; lshape[0].y = 0;
    lshape[1].x = 6; lshape[1].y = 0;
    lshape[2].x = 6; lshape[2].y = 3;
    lshape[3].x = 3; lshape[3].y = 3;
    lshape[4].x = 3; lshape[4].y = 6;
    lshape[5].x = 0; lshape[5].y = 6;

    // In lower part of L
    p.x = 4; p.y = 1;
    result = point_in_polygon(lshape, 6, p);
    printf("L (4,1) inside = %d\n", result); // EXPECT: L (4,1) inside = 1

    // In upper part of L
    p.x = 1; p.y = 4;
    result = point_in_polygon(lshape, 6, p);
    printf("L (1,4) inside = %d\n", result); // EXPECT: L (1,4) inside = 1

    // In concave region (outside)
    p.x = 5; p.y = 5;
    result = point_in_polygon(lshape, 6, p);
    printf("L (5,5) inside = %d\n", result); // EXPECT: L (5,5) inside = 0

    // Winding number tests
    wn = winding_number(square, 4, square[0]);
    // corner point - result is 0 for on-boundary
    p.x = 5; p.y = 5;
    wn = winding_number(square, 4, p);
    printf("winding sq (5,5) = %d\n", wn); // EXPECT: winding sq (5,5) = 1

    p.x = 15; p.y = 5;
    wn = winding_number(square, 4, p);
    printf("winding sq (15,5) = %d\n", wn); // EXPECT: winding sq (15,5) = 0

    p.x = 4; p.y = 2;
    wn = winding_number(tri, 3, p);
    printf("winding tri (4,2) = %d\n", wn); // EXPECT: winding tri (4,2) = 1

    return 0;
}
