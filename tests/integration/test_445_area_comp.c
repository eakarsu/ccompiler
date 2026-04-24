int printf(const char *fmt, ...);
// EXPECT: Triangle (0,0)(10,0)(0,10) 2x area: 100\nTriangle (1,1)(4,1)(4,5) 2x area: 12\nPentagon 2x area: 42\nHexagon 2x area: 24\nRect (0,0)-(5,3): 15\nRect (1,1)-(4,6): 15\nIntersection (0,0)-(5,5) and (3,3)-(8,8): 4\nIntersection (0,0)-(2,2) and (3,3)-(5,5): 0\nPentagon boundary pts: 8\nPentagon interior pts: 18
// Test: Area computation (various shapes, integer arithmetic)

typedef struct {
    int x;
    int y;
} Point;

int triangle_area_2x(Point a, Point b, Point c) {
    int area = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (area < 0) area = -area;
    return area;
}

int polygon_area_2x(Point pts[], int n) {
    int area = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area = area + pts[i].x * pts[j].y;
        area = area - pts[j].x * pts[i].y;
    }
    if (area < 0) area = -area;
    return area;
}

int rectangle_area(int x1, int y1, int x2, int y2) {
    int w = x2 - x1;
    int h = y2 - y1;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return w * h;
}

int rect_intersection_area(int ax1, int ay1, int ax2, int ay2,
                           int bx1, int by1, int bx2, int by2) {
    int x1 = ax1 > bx1 ? ax1 : bx1;
    int y1 = ay1 > by1 ? ay1 : by1;
    int x2 = ax2 < bx2 ? ax2 : bx2;
    int y2 = ay2 < by2 ? ay2 : by2;
    if (x1 >= x2 || y1 >= y2) return 0;
    return (x2 - x1) * (y2 - y1);
}

int picks_theorem_interior(int area_2x, int boundary) {
    return (area_2x - boundary + 2) / 2;
}

int lattice_points_on_segment(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    int a = dx, b = dy;
    while (b != 0) { int t = b; b = a % b; a = t; }
    return a;
}

int main(void) {
    Point t1a = {0, 0}, t1b = {10, 0}, t1c = {0, 10};
    printf("Triangle (0,0)(10,0)(0,10) 2x area: %d\n",
           triangle_area_2x(t1a, t1b, t1c));

    Point t2a = {1, 1}, t2b = {4, 1}, t2c = {4, 5};
    printf("Triangle (1,1)(4,1)(4,5) 2x area: %d\n",
           triangle_area_2x(t2a, t2b, t2c));

    Point pent[] = {{0,0}, {4,0}, {5,3}, {2,5}, {-1,3}};
    printf("Pentagon 2x area: %d\n", polygon_area_2x(pent, 5));

    Point hex[] = {{2,0}, {4,1}, {4,3}, {2,4}, {0,3}, {0,1}};
    printf("Hexagon 2x area: %d\n", polygon_area_2x(hex, 6));

    printf("Rect (0,0)-(5,3): %d\n", rectangle_area(0, 0, 5, 3));
    printf("Rect (1,1)-(4,6): %d\n", rectangle_area(1, 1, 4, 6));

    printf("Intersection (0,0)-(5,5) and (3,3)-(8,8): %d\n",
           rect_intersection_area(0, 0, 5, 5, 3, 3, 8, 8));
    printf("Intersection (0,0)-(2,2) and (3,3)-(5,5): %d\n",
           rect_intersection_area(0, 0, 2, 2, 3, 3, 5, 5));

    int area2 = polygon_area_2x(pent, 5);
    int boundary = lattice_points_on_segment(0,0,4,0) +
                   lattice_points_on_segment(4,0,5,3) +
                   lattice_points_on_segment(5,3,2,5) +
                   lattice_points_on_segment(2,5,-1,3) +
                   lattice_points_on_segment(-1,3,0,0);
    printf("Pentagon boundary pts: %d\n", boundary);
    printf("Pentagon interior pts: %d\n", picks_theorem_interior(area2, boundary));

    return 0;
}
