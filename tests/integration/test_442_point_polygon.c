int printf(const char *fmt, ...);
// EXPECT: (5,5) in square: 1\n(15,5) in square: 0\n(0,0) in square: 1\n(10,5) in square: 0\n(5,3) in triangle: 1\n(0,10) in triangle: 0\nSquare 2x area: 200\nTriangle 2x area: 100\nSquare perimeter^2 sum: 400\nSquare convex: 1\nConcave polygon convex: 0
// Test: Point in polygon (ray casting)

typedef struct {
    int x;
    int y;
} Point;

int point_in_polygon(Point poly[], int n, Point p) {
    int inside = 0;
    int i, j;
    j = n - 1;
    for (i = 0; i < n; i++) {
        if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
            (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) /
                    (poly[j].y - poly[i].y) + poly[i].x)) {
            inside = !inside;
        }
        j = i;
    }
    return inside;
}

int polygon_perimeter_sq(Point poly[], int n) {
    int peri = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int dx = poly[j].x - poly[i].x;
        int dy = poly[j].y - poly[i].y;
        peri = peri + dx * dx + dy * dy;
    }
    return peri;
}

int polygon_area_2x(Point poly[], int n) {
    int area = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area = area + poly[i].x * poly[j].y;
        area = area - poly[j].x * poly[i].y;
    }
    if (area < 0) area = -area;
    return area;
}

int is_convex(Point poly[], int n) {
    int sign = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;
        int cross = (poly[j].x - poly[i].x) * (poly[k].y - poly[j].y) -
                    (poly[j].y - poly[i].y) * (poly[k].x - poly[j].x);
        if (cross != 0) {
            if (sign == 0) {
                sign = cross > 0 ? 1 : -1;
            } else {
                int cur_sign = cross > 0 ? 1 : -1;
                if (cur_sign != sign) return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    Point square[] = {{0,0}, {10,0}, {10,10}, {0,10}};

    Point p1 = {5, 5};
    Point p2 = {15, 5};
    Point p3 = {0, 0};
    Point p4 = {10, 5};

    printf("(5,5) in square: %d\n", point_in_polygon(square, 4, p1));
    printf("(15,5) in square: %d\n", point_in_polygon(square, 4, p2));
    printf("(0,0) in square: %d\n", point_in_polygon(square, 4, p3));
    printf("(10,5) in square: %d\n", point_in_polygon(square, 4, p4));

    Point triangle[] = {{0,0}, {10,0}, {5,10}};
    Point tp1 = {5, 3};
    Point tp2 = {0, 10};
    printf("(5,3) in triangle: %d\n", point_in_polygon(triangle, 3, tp1));
    printf("(0,10) in triangle: %d\n", point_in_polygon(triangle, 3, tp2));

    printf("Square 2x area: %d\n", polygon_area_2x(square, 4));
    printf("Triangle 2x area: %d\n", polygon_area_2x(triangle, 3));
    printf("Square perimeter^2 sum: %d\n", polygon_perimeter_sq(square, 4));

    printf("Square convex: %d\n", is_convex(square, 4));
    Point concave[] = {{0,0}, {10,0}, {5,5}, {10,10}, {0,10}};
    printf("Concave polygon convex: %d\n", is_convex(concave, 5));

    return 0;
}
