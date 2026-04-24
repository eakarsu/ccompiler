int printf(const char *fmt, ...);

// Polygon area using the Shoelace formula (doubled to stay integer)
// Area*2 = |sum of (x_i * y_{i+1} - x_{i+1} * y_i)|

struct Point {
    int x;
    int y;
};

// Signed area * 2 of polygon (positive if CCW)
int signed_area2(struct Point *poly, int n) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        sum = sum + (poly[i].x * poly[j].y - poly[j].x * poly[i].y);
    }
    return sum;
}

// Absolute area * 2
int area2(struct Point *poly, int n) {
    int s;
    s = signed_area2(poly, n);
    if (s < 0) s = -s;
    return s;
}

// Check if polygon is given in CCW order
int is_ccw(struct Point *poly, int n) {
    int s;
    s = signed_area2(poly, n);
    if (s > 0) return 1;
    return 0;
}

// Triangle area * 2 using cross product
int triangle_area2(struct Point a, struct Point b, struct Point c) {
    int val;
    val = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (val < 0) val = -val;
    return val;
}

// Centroid of polygon (integer approximation: sum/n)
struct Point centroid_approx(struct Point *poly, int n) {
    struct Point c;
    int i;
    c.x = 0;
    c.y = 0;
    for (i = 0; i < n; i = i + 1) {
        c.x = c.x + poly[i].x;
        c.y = c.y + poly[i].y;
    }
    c.x = c.x / n;
    c.y = c.y / n;
    return c;
}

// Perimeter * 1 using Manhattan-like segments
// (sum of max(|dx|,|dy|) per edge - rough approx)
int perimeter_approx(struct Point *poly, int n) {
    int sum;
    int i;
    int j;
    int dx;
    int dy;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        j = i + 1;
        if (j >= n) j = 0;
        dx = poly[j].x - poly[i].x;
        dy = poly[j].y - poly[i].y;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        sum = sum + dx + dy;
    }
    return sum;
}

int main() {
    struct Point tri[3];
    struct Point sq[4];
    struct Point pent[5];
    struct Point hex[6];
    struct Point c;
    int a2;
    int sa2;

    // Triangle: (0,0), (4,0), (0,3)
    tri[0].x = 0; tri[0].y = 0;
    tri[1].x = 4; tri[1].y = 0;
    tri[2].x = 0; tri[2].y = 3;
    a2 = area2(tri, 3);
    printf("triangle area*2 = %d\n", a2); // EXPECT: triangle area*2 = 12

    sa2 = signed_area2(tri, 3);
    printf("triangle signed_area*2 = %d\n", sa2); // EXPECT: triangle signed_area*2 = 12

    printf("triangle is_ccw = %d\n", is_ccw(tri, 3)); // EXPECT: triangle is_ccw = 1

    a2 = triangle_area2(tri[0], tri[1], tri[2]);
    printf("triangle_area2 direct = %d\n", a2); // EXPECT: triangle_area2 direct = 12

    // Square: (0,0), (4,0), (4,4), (0,4)
    sq[0].x = 0; sq[0].y = 0;
    sq[1].x = 4; sq[1].y = 0;
    sq[2].x = 4; sq[2].y = 4;
    sq[3].x = 0; sq[3].y = 4;
    a2 = area2(sq, 4);
    printf("square area*2 = %d\n", a2); // EXPECT: square area*2 = 32

    printf("square is_ccw = %d\n", is_ccw(sq, 4)); // EXPECT: square is_ccw = 1

    c = centroid_approx(sq, 4);
    printf("square centroid = (%d,%d)\n", c.x, c.y); // EXPECT: square centroid = (2,2)

    // Clockwise triangle: (0,0), (0,3), (4,0)
    tri[0].x = 0; tri[0].y = 0;
    tri[1].x = 0; tri[1].y = 3;
    tri[2].x = 4; tri[2].y = 0;
    sa2 = signed_area2(tri, 3);
    printf("CW triangle signed_area*2 = %d\n", sa2); // EXPECT: CW triangle signed_area*2 = -12
    printf("CW triangle is_ccw = %d\n", is_ccw(tri, 3)); // EXPECT: CW triangle is_ccw = 0
    a2 = area2(tri, 3);
    printf("CW triangle area*2 = %d\n", a2); // EXPECT: CW triangle area*2 = 12

    // Pentagon: (0,2), (2,0), (4,1), (4,4), (1,4)
    pent[0].x = 0; pent[0].y = 2;
    pent[1].x = 2; pent[1].y = 0;
    pent[2].x = 4; pent[2].y = 1;
    pent[3].x = 4; pent[3].y = 4;
    pent[4].x = 1; pent[4].y = 4;
    a2 = area2(pent, 5);
    printf("pentagon area*2 = %d\n", a2); // EXPECT: pentagon area*2 = 24

    c = centroid_approx(pent, 5);
    printf("pentagon centroid = (%d,%d)\n", c.x, c.y); // EXPECT: pentagon centroid = (2,2)

    // Perimeter tests
    printf("square perimeter_approx = %d\n", perimeter_approx(sq, 4)); // EXPECT: square perimeter_approx = 16
    printf("triangle perimeter_approx = %d\n", perimeter_approx(tri, 3)); // EXPECT: triangle perimeter_approx = 14

    // Hexagon: (2,0),(4,1),(4,3),(2,4),(0,3),(0,1)
    hex[0].x = 2; hex[0].y = 0;
    hex[1].x = 4; hex[1].y = 1;
    hex[2].x = 4; hex[2].y = 3;
    hex[3].x = 2; hex[3].y = 4;
    hex[4].x = 0; hex[4].y = 3;
    hex[5].x = 0; hex[5].y = 1;
    a2 = area2(hex, 6);
    printf("hexagon area*2 = %d\n", a2); // EXPECT: hexagon area*2 = 24

    printf("hexagon perimeter_approx = %d\n", perimeter_approx(hex, 6)); // EXPECT: hexagon perimeter_approx = 16

    return 0;
}
