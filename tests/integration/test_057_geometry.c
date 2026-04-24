int printf(const char *fmt, ...);
// EXPECT: T1:d00=0 d01=1 d34=25 d_neg=25 d_big=200 T2:s0=0 s1=1 s4=2 s9=3 s25=5 s26=5 s100=10 s200=14 T3:o1=0 o2=2 o3=1 T4:c1=1 c2=1 c3=0 c4=1 T5:i1=1 i2=0 i3=1 i4=0 i5=1 i6=1 T6:a1=12 a2=80 a3=0 T7:sq=32 rect=36 T8:pent=20 T9:sq=1 tri=1 conc=0 T10:in1=1 in2=1 in3=0 in4=1 in5=0 T11:m1=7 m2=7 m3=10 m4=0 T12:c1=1 c2=-1 c3=4 T13:perim=12 perim2=14 T14:minD=2 sqrt=1 T15:bb:0,1,6,7 area=36 DONE

/* =========================================================================
 * Test 057: Integer Geometry Operations
 * =========================================================================
 * Point distance squared, line intersection checks, polygon area
 * (shoelace formula), convex hull check, orientation, collinearity.
 * All computations use integer arithmetic only.
 * ========================================================================= */

/* ---- Integer square root (floor) ---- */
int isqrt(int n) {
    int x;
    int y;
    if (n <= 0) {
        return 0;
    }
    x = n;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* ---- Distance squared between two points ---- */
int dist_sq(int x1, int y1, int x2, int y2) {
    int dx;
    int dy;
    dx = x2 - x1;
    dy = y2 - y1;
    return dx * dx + dy * dy;
}

/* ---- Orientation of triplet (p, q, r) ----
   Returns: 0 = collinear, 1 = clockwise, 2 = counterclockwise */
int orientation(int px, int py, int qx, int qy, int rx, int ry) {
    int val;
    val = (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
    if (val == 0) {
        return 0;
    }
    if (val > 0) {
        return 1;
    }
    return 2;
}

/* ---- Check if point q lies on segment pr ---- */
int on_segment(int px, int py, int qx, int qy, int rx, int ry) {
    int minx;
    int maxx;
    int miny;
    int maxy;

    minx = px;
    if (rx < minx) { minx = rx; }
    maxx = px;
    if (rx > maxx) { maxx = rx; }
    miny = py;
    if (ry < miny) { miny = ry; }
    maxy = py;
    if (ry > maxy) { maxy = ry; }

    if (qx >= minx && qx <= maxx && qy >= miny && qy <= maxy) {
        return 1;
    }
    return 0;
}

/* ---- Check if two line segments intersect ----
   Segment 1: (p1x,p1y)-(q1x,q1y)
   Segment 2: (p2x,p2y)-(q2x,q2y) */
int segments_intersect(int p1x, int p1y, int q1x, int q1y,
                       int p2x, int p2y, int q2x, int q2y) {
    int o1;
    int o2;
    int o3;
    int o4;

    o1 = orientation(p1x, p1y, q1x, q1y, p2x, p2y);
    o2 = orientation(p1x, p1y, q1x, q1y, q2x, q2y);
    o3 = orientation(p2x, p2y, q2x, q2y, p1x, p1y);
    o4 = orientation(p2x, p2y, q2x, q2y, q1x, q1y);

    /* General case */
    if (o1 != o2 && o3 != o4) {
        return 1;
    }

    /* Collinear special cases */
    if (o1 == 0 && on_segment(p1x, p1y, p2x, p2y, q1x, q1y)) { return 1; }
    if (o2 == 0 && on_segment(p1x, p1y, q2x, q2y, q1x, q1y)) { return 1; }
    if (o3 == 0 && on_segment(p2x, p2y, p1x, p1y, q2x, q2y)) { return 1; }
    if (o4 == 0 && on_segment(p2x, p2y, q1x, q1y, q2x, q2y)) { return 1; }

    return 0;
}

/* ---- Polygon area * 2 using Shoelace formula ----
   Returns positive value (absolute area * 2) */
int polygon_area2(int *px, int *py, int n) {
    int area;
    int i;
    int j;
    area = 0;
    j = n - 1;
    for (i = 0; i < n; i = i + 1) {
        area = area + (px[j] + px[i]) * (py[j] - py[i]);
        j = i;
    }
    if (area < 0) {
        area = -area;
    }
    return area;
}

/* ---- Cross product of vectors (o->a) x (o->b) ---- */
int cross(int ox, int oy, int ax, int ay, int bx, int by) {
    return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

/* ---- Check if polygon (given as arrays) is convex ----
   Assumes at least 3 points. */
int is_convex(int *px, int *py, int n) {
    int i;
    int c;
    int sign;
    int a;
    int b;

    sign = 0;
    for (i = 0; i < n; i = i + 1) {
        a = (i + 1) % n;
        b = (i + 2) % n;
        c = cross(px[i], py[i], px[a], py[a], px[b], py[b]);
        if (c != 0) {
            if (sign == 0) {
                if (c > 0) { sign = 1; }
                else { sign = -1; }
            } else {
                if (c > 0 && sign == -1) { return 0; }
                if (c < 0 && sign == 1) { return 0; }
            }
        }
    }
    return 1;
}

/* ---- Manhattan distance ---- */
int manhattan(int x1, int y1, int x2, int y2) {
    int dx;
    int dy;
    dx = x2 - x1;
    if (dx < 0) { dx = -dx; }
    dy = y2 - y1;
    if (dy < 0) { dy = -dy; }
    return dx + dy;
}

/* ---- Point in triangle test using area method ----
   Returns 1 if point (px,py) is inside triangle (x1,y1),(x2,y2),(x3,y3) */
int point_in_triangle(int px, int py,
                      int x1, int y1, int x2, int y2, int x3, int y3) {
    int d1;
    int d2;
    int d3;
    int has_neg;
    int has_pos;

    d1 = cross(x1, y1, x2, y2, px, py);
    d2 = cross(x2, y2, x3, y3, px, py);
    d3 = cross(x3, y3, x1, y1, px, py);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    if (has_neg && has_pos) {
        return 0;
    }
    return 1;
}

/* ---- Check collinearity of 3 points ---- */
int collinear(int x1, int y1, int x2, int y2, int x3, int y3) {
    return orientation(x1, y1, x2, y2, x3, y3) == 0;
}

/* ---- Perimeter approximation: sum of isqrt(dist_sq) ---- */
int polygon_perimeter_approx(int *px, int *py, int n) {
    int perim;
    int i;
    int j;
    perim = 0;
    for (i = 0; i < n; i = i + 1) {
        j = (i + 1) % n;
        perim = perim + isqrt(dist_sq(px[i], py[i], px[j], py[j]));
    }
    return perim;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_dist_squared(void) {
    printf("T1:");
    printf("d00=%d", dist_sq(0, 0, 0, 0));
    printf(" d01=%d", dist_sq(0, 0, 1, 0));
    printf(" d34=%d", dist_sq(0, 0, 3, 4));
    printf(" d_neg=%d", dist_sq(-1, -1, 2, 3));
    printf(" d_big=%d", dist_sq(0, 0, 10, 10));
}

void test_isqrt(void) {
    printf(" T2:");
    printf("s0=%d", isqrt(0));
    printf(" s1=%d", isqrt(1));
    printf(" s4=%d", isqrt(4));
    printf(" s9=%d", isqrt(9));
    printf(" s25=%d", isqrt(25));
    printf(" s26=%d", isqrt(26));
    printf(" s100=%d", isqrt(100));
    printf(" s200=%d", isqrt(200));
}

void test_orientation(void) {
    printf(" T3:");
    /* collinear: (0,0),(1,1),(2,2) */
    printf("o1=%d", orientation(0, 0, 1, 1, 2, 2));
    /* clockwise: (0,0),(4,0),(2,2) - actually let me compute:
       val = (0-0)*(2-4) - (4-0)*(2-0) = 0*(-2) - 4*2 = -8 => CW would be val>0
       Actually: val = (qy-py)*(rx-qx) - (qx-px)*(ry-qy)
       = (0-0)*(2-4) - (4-0)*(2-0) = 0 - 8 = -8 => negative => CCW=2 */
    printf(" o2=%d", orientation(0, 0, 4, 0, 2, 2));
    /* (0,0),(0,4),(4,0) => val = (4-0)*(4-0)-(0-0)*(0-4) = 16-0 = 16 => CW=1 */
    printf(" o3=%d", orientation(0, 0, 0, 4, 4, 0));
}

void test_collinear(void) {
    printf(" T4:");
    printf("c1=%d", collinear(0, 0, 1, 1, 2, 2));
    printf(" c2=%d", collinear(0, 0, 1, 0, 2, 0));
    printf(" c3=%d", collinear(0, 0, 1, 1, 2, 3));
    printf(" c4=%d", collinear(1, 1, 3, 3, 5, 5));
}

void test_segments_intersect(void) {
    printf(" T5:");
    /* Crossing X: (0,0)-(4,4) and (0,4)-(4,0) */
    printf("i1=%d", segments_intersect(0, 0, 4, 4, 0, 4, 4, 0));
    /* Parallel: (0,0)-(4,0) and (0,1)-(4,1) */
    printf(" i2=%d", segments_intersect(0, 0, 4, 0, 0, 1, 4, 1));
    /* T-shape: (0,0)-(4,0) and (2,-2)-(2,2) */
    printf(" i3=%d", segments_intersect(0, 0, 4, 0, 2, -2, 2, 2));
    /* Non-intersecting: (0,0)-(1,1) and (2,0)-(3,0) */
    printf(" i4=%d", segments_intersect(0, 0, 1, 1, 2, 0, 3, 0));
    /* Touching at endpoint: (0,0)-(2,2) and (2,2)-(4,0) */
    printf(" i5=%d", segments_intersect(0, 0, 2, 2, 2, 2, 4, 0));
    /* Collinear overlapping: (0,0)-(3,0) and (2,0)-(5,0) */
    printf(" i6=%d", segments_intersect(0, 0, 3, 0, 2, 0, 5, 0));
}

void test_triangle_area(void) {
    int px[3];
    int py[3];
    int a;

    printf(" T6:");

    /* Triangle (0,0),(4,0),(0,3) -> area = 6, area2 = 12 */
    px[0] = 0; py[0] = 0;
    px[1] = 4; py[1] = 0;
    px[2] = 0; py[2] = 3;
    a = polygon_area2(px, py, 3);
    printf("a1=%d", a);

    /* Triangle (0,0),(10,0),(5,8) -> area2 = |10*8| = 80 */
    px[0] = 0; py[0] = 0;
    px[1] = 10; py[1] = 0;
    px[2] = 5; py[2] = 8;
    a = polygon_area2(px, py, 3);
    printf(" a2=%d", a);

    /* Degenerate triangle (all on line) -> area = 0 */
    px[0] = 0; py[0] = 0;
    px[1] = 1; py[1] = 1;
    px[2] = 2; py[2] = 2;
    a = polygon_area2(px, py, 3);
    printf(" a3=%d", a);
}

void test_square_area(void) {
    int px[4];
    int py[4];

    printf(" T7:");

    /* Square (0,0),(4,0),(4,4),(0,4) -> area = 16, area2 = 32 */
    px[0] = 0; py[0] = 0;
    px[1] = 4; py[1] = 0;
    px[2] = 4; py[2] = 4;
    px[3] = 0; py[3] = 4;
    printf("sq=%d", polygon_area2(px, py, 4));

    /* Rectangle (0,0),(6,0),(6,3),(0,3) -> area = 18, area2 = 36 */
    px[0] = 0; py[0] = 0;
    px[1] = 6; py[1] = 0;
    px[2] = 6; py[2] = 3;
    px[3] = 0; py[3] = 3;
    printf(" rect=%d", polygon_area2(px, py, 4));
}

void test_pentagon_area(void) {
    int px[5];
    int py[5];

    printf(" T8:");

    /* Pentagon: (1,0),(3,0),(4,2),(2,4),(0,2) */
    px[0] = 1; py[0] = 0;
    px[1] = 3; py[1] = 0;
    px[2] = 4; py[2] = 2;
    px[3] = 2; py[3] = 4;
    px[4] = 0; py[4] = 2;
    printf("pent=%d", polygon_area2(px, py, 5));
}

void test_convex_check(void) {
    int px[6];
    int py[6];

    printf(" T9:");

    /* Convex square */
    px[0] = 0; py[0] = 0;
    px[1] = 4; py[1] = 0;
    px[2] = 4; py[2] = 4;
    px[3] = 0; py[3] = 4;
    printf("sq=%d", is_convex(px, py, 4));

    /* Convex triangle */
    px[0] = 0; py[0] = 0;
    px[1] = 5; py[1] = 0;
    px[2] = 2; py[2] = 4;
    printf(" tri=%d", is_convex(px, py, 3));

    /* Non-convex (concave) quadrilateral: arrow shape */
    px[0] = 0; py[0] = 0;
    px[1] = 4; py[1] = 0;
    px[2] = 2; py[2] = 1;  /* indented point */
    px[3] = 2; py[3] = 4;
    printf(" conc=%d", is_convex(px, py, 4));
}

void test_point_in_triangle(void) {
    printf(" T10:");

    /* Triangle: (0,0),(10,0),(0,10) */
    /* Point (2,2) -> inside */
    printf("in1=%d", point_in_triangle(2, 2, 0, 0, 10, 0, 0, 10));
    /* Point (5,5) -> on edge */
    printf(" in2=%d", point_in_triangle(5, 5, 0, 0, 10, 0, 0, 10));
    /* Point (8,8) -> outside */
    printf(" in3=%d", point_in_triangle(8, 8, 0, 0, 10, 0, 0, 10));
    /* Point (0,0) -> vertex */
    printf(" in4=%d", point_in_triangle(0, 0, 0, 0, 10, 0, 0, 10));
    /* Point (-1,-1) -> outside */
    printf(" in5=%d", point_in_triangle(-1, -1, 0, 0, 10, 0, 0, 10));
}

void test_manhattan(void) {
    printf(" T11:");
    printf("m1=%d", manhattan(0, 0, 3, 4));
    printf(" m2=%d", manhattan(1, 1, 4, 5));
    printf(" m3=%d", manhattan(-2, -3, 2, 3));
    printf(" m4=%d", manhattan(0, 0, 0, 0));
}

void test_cross_product(void) {
    printf(" T12:");
    /* cross((0,0),(1,0),(0,1)) = 1*1 - 0*0 = 1 */
    printf("c1=%d", cross(0, 0, 1, 0, 0, 1));
    /* cross((0,0),(0,1),(1,0)) = 0*0 - 1*1 = -1 */
    printf(" c2=%d", cross(0, 0, 0, 1, 1, 0));
    /* cross((1,1),(3,1),(2,3)) = (3-1)*(3-1)-(1-1)*(2-1) = 2*2-0 = 4 */
    printf(" c3=%d", cross(1, 1, 3, 1, 2, 3));
}

void test_perimeter(void) {
    int px[4];
    int py[4];

    printf(" T13:");

    /* Square side 3: perimeter approx = 4*3 = 12 */
    px[0] = 0; py[0] = 0;
    px[1] = 3; py[1] = 0;
    px[2] = 3; py[2] = 3;
    px[3] = 0; py[3] = 3;
    printf("perim=%d", polygon_perimeter_approx(px, py, 4));

    /* Rectangle 4x3: perimeter = 2*(4+3) = 14 */
    px[0] = 0; py[0] = 0;
    px[1] = 4; py[1] = 0;
    px[2] = 4; py[2] = 3;
    px[3] = 0; py[3] = 3;
    printf(" perim2=%d", polygon_perimeter_approx(px, py, 4));
}

void test_closest_pair_brute(void) {
    /* Find minimum distance squared among a set of points */
    int px[6];
    int py[6];
    int n;
    int i;
    int j;
    int min_d;
    int d;

    printf(" T14:");

    px[0]=0; py[0]=0;
    px[1]=3; py[1]=4;
    px[2]=1; py[2]=1;
    px[3]=5; py[3]=2;
    px[4]=7; py[4]=0;
    px[5]=2; py[5]=3;
    n = 6;

    min_d = 999999;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            d = dist_sq(px[i], py[i], px[j], py[j]);
            if (d < min_d) {
                min_d = d;
            }
        }
    }
    printf("minD=%d", min_d);
    printf(" sqrt=%d", isqrt(min_d));
}

/* ---- Compute centroid (integer approximation) of polygon ---- */
int centroid_x(int *px, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + px[i];
    }
    return sum / n;
}

int centroid_y(int *py, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + py[i];
    }
    return sum / n;
}

void test_bounding_box(void) {
    int px[5];
    int py[5];
    int n;
    int i;
    int minx;
    int maxx;
    int miny;
    int maxy;

    printf(" T15:");

    px[0]=1; py[0]=3;
    px[1]=5; py[1]=1;
    px[2]=3; py[2]=7;
    px[3]=0; py[3]=4;
    px[4]=6; py[4]=2;
    n = 5;

    minx = px[0]; maxx = px[0];
    miny = py[0]; maxy = py[0];
    for (i = 1; i < n; i = i + 1) {
        if (px[i] < minx) { minx = px[i]; }
        if (px[i] > maxx) { maxx = px[i]; }
        if (py[i] < miny) { miny = py[i]; }
        if (py[i] > maxy) { maxy = py[i]; }
    }
    printf("bb:%d,%d,%d,%d", minx, miny, maxx, maxy);
    printf(" area=%d", (maxx - minx) * (maxy - miny));
    printf(" DONE\n");
}

int main(void) {
    test_dist_squared();
    test_isqrt();
    test_orientation();
    test_collinear();
    test_segments_intersect();
    test_triangle_area();
    test_square_area();
    test_pentagon_area();
    test_convex_check();
    test_point_in_triangle();
    test_manhattan();
    test_cross_product();
    test_perimeter();
    test_closest_pair_brute();
    test_bounding_box();
    return 0;
}
