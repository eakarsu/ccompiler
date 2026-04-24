int printf(const char *fmt, ...);
// EXPECT: p1: (3, 4)\np2: (7, 1)\ndistance^2: 25\nrect: 10x5\narea: 50\nperimeter: 30\npoints[0]: (0, 0)\npoints[1]: (2, 3)\npoints[2]: (4, 6)\npoints[3]: (6, 9)
// Test: basic struct definition and access

struct Point {
    int x;
    int y;
};

struct Rectangle {
    int width;
    int height;
};

int area(struct Rectangle r) {
    return r.width * r.height;
}

int perimeter(struct Rectangle r) {
    return 2 * (r.width + r.height);
}

int distance_squared(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int main(void) {
    struct Point p1;
    p1.x = 3;
    p1.y = 4;
    printf("p1: (%d, %d)\n", p1.x, p1.y);

    struct Point p2;
    p2.x = 7;
    p2.y = 1;
    printf("p2: (%d, %d)\n", p2.x, p2.y);

    int dist2 = distance_squared(p1, p2);
    printf("distance^2: %d\n", dist2);

    struct Rectangle rect;
    rect.width = 10;
    rect.height = 5;
    printf("rect: %dx%d\n", rect.width, rect.height);
    printf("area: %d\n", area(rect));
    printf("perimeter: %d\n", perimeter(rect));

    struct Point points[4];
    int i;
    for (i = 0; i < 4; i++) {
        points[i].x = i * 2;
        points[i].y = i * 3;
    }
    for (i = 0; i < 4; i++) {
        printf("points[%d]: (%d, %d)\n", i, points[i].x, points[i].y);
    }

    return 0;
}
