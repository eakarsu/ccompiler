int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

int dist_sq(Point a, Point b) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    return dx * dx + dy * dy;
}

int closest_pair_dist_sq(Point *pts, int n) {
    if (n < 2) return -1;
    int best = dist_sq(pts[0], pts[1]);
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = dist_sq(pts[i], pts[j]);
            if (d < best) {
                best = d;
            }
        }
    }
    return best;
}

void closest_pair_indices(Point *pts, int n, int *idx1, int *idx2) {
    *idx1 = 0;
    *idx2 = 1;
    int best = dist_sq(pts[0], pts[1]);
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = dist_sq(pts[i], pts[j]);
            if (d < best) {
                best = d;
                *idx1 = i;
                *idx2 = j;
            }
        }
    }
}

int farthest_pair_dist_sq(Point *pts, int n) {
    if (n < 2) return -1;
    int best = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = dist_sq(pts[i], pts[j]);
            if (d > best) {
                best = d;
            }
        }
    }
    return best;
}

int count_pairs_within(Point *pts, int n, int threshold_sq) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (dist_sq(pts[i], pts[j]) <= threshold_sq) {
                count++;
            }
        }
    }
    return count;
}

Point make_pt(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int main(void) {
    Point pts1[4];
    pts1[0] = make_pt(0, 0);
    pts1[1] = make_pt(10, 0);
    pts1[2] = make_pt(10, 10);
    pts1[3] = make_pt(0, 10);

    int d1 = closest_pair_dist_sq(pts1, 4);
    printf("%d\n", d1);
    // EXPECT: 100

    int f1 = farthest_pair_dist_sq(pts1, 4);
    printf("%d\n", f1);
    // EXPECT: 200

    Point pts2[5];
    pts2[0] = make_pt(0, 0);
    pts2[1] = make_pt(3, 4);
    pts2[2] = make_pt(1, 1);
    pts2[3] = make_pt(8, 2);
    pts2[4] = make_pt(6, 7);

    int d2 = closest_pair_dist_sq(pts2, 5);
    printf("%d\n", d2);
    // EXPECT: 2

    int i1;
    int i2;
    closest_pair_indices(pts2, 5, &i1, &i2);
    printf("%d %d\n", i1, i2);
    // EXPECT: 0 2

    int f2 = farthest_pair_dist_sq(pts2, 5);
    printf("%d\n", f2);
    // EXPECT: 85

    Point pts3[3];
    pts3[0] = make_pt(0, 0);
    pts3[1] = make_pt(1, 0);
    pts3[2] = make_pt(100, 100);

    int d3 = closest_pair_dist_sq(pts3, 3);
    printf("%d\n", d3);
    // EXPECT: 1

    int f3 = farthest_pair_dist_sq(pts3, 3);
    printf("%d\n", f3);
    // EXPECT: 20000

    Point pts4[6];
    pts4[0] = make_pt(0, 0);
    pts4[1] = make_pt(1, 0);
    pts4[2] = make_pt(2, 0);
    pts4[3] = make_pt(3, 0);
    pts4[4] = make_pt(4, 0);
    pts4[5] = make_pt(5, 0);

    int d4 = closest_pair_dist_sq(pts4, 6);
    printf("%d\n", d4);
    // EXPECT: 1

    int c1 = count_pairs_within(pts4, 6, 1);
    printf("%d\n", c1);
    // EXPECT: 5

    int c2 = count_pairs_within(pts4, 6, 4);
    printf("%d\n", c2);
    // EXPECT: 9

    int c3 = count_pairs_within(pts4, 6, 25);
    printf("%d\n", c3);
    // EXPECT: 15

    Point cluster[5];
    cluster[0] = make_pt(0, 0);
    cluster[1] = make_pt(0, 1);
    cluster[2] = make_pt(1, 0);
    cluster[3] = make_pt(1, 1);
    cluster[4] = make_pt(100, 100);

    int d5 = closest_pair_dist_sq(cluster, 5);
    printf("%d\n", d5);
    // EXPECT: 1

    int c4 = count_pairs_within(cluster, 5, 2);
    printf("%d\n", c4);
    // EXPECT: 6

    int c5 = count_pairs_within(cluster, 5, 0);
    printf("%d\n", c5);
    // EXPECT: 0

    Point two[2];
    two[0] = make_pt(3, 7);
    two[1] = make_pt(6, 11);
    int d6 = closest_pair_dist_sq(two, 2);
    printf("%d\n", d6);
    // EXPECT: 25

    printf("done\n");
    // EXPECT: done

    return 0;
}
