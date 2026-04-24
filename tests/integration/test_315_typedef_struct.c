int printf(const char *fmt, ...);
// EXPECT: p1=(3, 4)\np2=(7, 2)\nsum=(10, 6)\nRect((0, 0), 10x8)\narea=80\nperimeter=36\ncontains(5,3)=1\ncontains(15,3)=0\nRect((5, 5), 20x10)\narea=200
// Test: typedef for struct

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point origin;
    int width;
    int height;
} Rect;

Point point_new(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

void point_print(Point p) {
    printf("(%d, %d)", p.x, p.y);
}

Point point_add(Point a, Point b) {
    Point result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

void rect_init(Rect *r, int x, int y, int w, int h) {
    r->origin.x = x;
    r->origin.y = y;
    r->width = w;
    r->height = h;
}

int rect_area(Rect *r) {
    return r->width * r->height;
}

int rect_perimeter(Rect *r) {
    return 2 * (r->width + r->height);
}

int rect_contains(Rect *r, Point p) {
    return p.x >= r->origin.x &&
           p.x < r->origin.x + r->width &&
           p.y >= r->origin.y &&
           p.y < r->origin.y + r->height;
}

void rect_print(Rect *r) {
    printf("Rect(");
    point_print(r->origin);
    printf(", %dx%d)", r->width, r->height);
}

int main(void) {
    Point p1 = point_new(3, 4);
    Point p2 = point_new(7, 2);
    printf("p1="); point_print(p1); printf("\n");
    printf("p2="); point_print(p2); printf("\n");

    Point sum = point_add(p1, p2);
    printf("sum="); point_print(sum); printf("\n");

    Rect r;
    rect_init(&r, 0, 0, 10, 8);
    rect_print(&r); printf("\n");
    printf("area=%d\n", rect_area(&r));
    printf("perimeter=%d\n", rect_perimeter(&r));

    Point test1 = point_new(5, 3);
    Point test2 = point_new(15, 3);
    printf("contains(5,3)=%d\n", rect_contains(&r, test1));
    printf("contains(15,3)=%d\n", rect_contains(&r, test2));

    Rect r2;
    rect_init(&r2, 5, 5, 20, 10);
    rect_print(&r2); printf("\n");
    printf("area=%d\n", rect_area(&r2));

    return 0;
}
