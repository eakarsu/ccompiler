int printf(const char *fmt, ...);
// EXPECT: Hull points (4): (0,0) (2,0) (2,2) (0,2)\n2x Area: 8\nSquare hull (4): (0,0) (4,0) (4,4) (0,4)\n2x Area: 32\nTriangle 2x area: 24
// Test: Convex hull (Graham scan simplified, small set)

typedef struct {
    int x;
    int y;
} Point;

int cross(Point O, Point A, Point B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

void swap_pt(Point *a, Point *b) {
    Point t = *a;
    *a = *b;
    *b = t;
}

void sort_points(Point pts[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (pts[j].x > pts[j + 1].x ||
                (pts[j].x == pts[j + 1].x && pts[j].y > pts[j + 1].y)) {
                swap_pt(&pts[j], &pts[j + 1]);
            }
        }
    }
}

int convex_hull(Point pts[], int n, Point hull[]) {
    sort_points(pts, n);
    int k = 0;
    int i;

    for (i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0)
            k--;
        hull[k] = pts[i];
        k++;
    }

    int lower = k + 1;
    for (i = n - 2; i >= 0; i--) {
        while (k >= lower && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0)
            k--;
        hull[k] = pts[i];
        k++;
    }
    return k - 1;
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

int main(void) {
    Point pts[] = {{0,0}, {1,1}, {2,2}, {0,2}, {2,0}, {1,0}, {0,1}};
    int n = 7;
    Point hull[16];
    int h = convex_hull(pts, n, hull);

    printf("Hull points (%d):", h);
    int i;
    for (i = 0; i < h; i++) {
        printf(" (%d,%d)", hull[i].x, hull[i].y);
    }
    printf("\n");
    printf("2x Area: %d\n", polygon_area_2x(hull, h));

    Point pts2[] = {{0,0}, {4,0}, {4,4}, {0,4}, {2,2}};
    h = convex_hull(pts2, 5, hull);
    printf("Square hull (%d):", h);
    for (i = 0; i < h; i++) {
        printf(" (%d,%d)", hull[i].x, hull[i].y);
    }
    printf("\n");
    printf("2x Area: %d\n", polygon_area_2x(hull, h));

    Point tri[] = {{0,0}, {6,0}, {3,4}};
    printf("Triangle 2x area: %d\n", polygon_area_2x(tri, 3));

    return 0;
}
