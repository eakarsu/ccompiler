int printf(const char *fmt, ...);
// EXPECT: Points:\n  (3, 4)\n  (1, 2)\n  (5, 1)\n  (2, 5)\n  (4, 3)\ndist_sq(0,1) = 8\nbefore swap: (3,4) (1,2)\nafter swap: (1,2) (3,4)\nSorted by x:\n  (1, 2)\n  (2, 5)\n  (3, 4)\n  (4, 3)\n  (5, 1)\nclosest pair: (2,5) and (3,4) dist_sq=2\ncentroid: (3, 3)
// Test: pointer manipulation with structs and arrays

struct Point {
    int x;
    int y;
};

int distance_sq(struct Point *a, struct Point *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

void swap_points(struct Point *a, struct Point *b) {
    struct Point tmp;
    tmp.x = a->x; tmp.y = a->y;
    a->x = b->x; a->y = b->y;
    b->x = tmp.x; b->y = tmp.y;
}

// Sort points by x coordinate using pointer array
void sort_points_by_x(struct Point *pts, int n) {
    int i; int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (pts[j].x > pts[j + 1].x) {
                swap_points(&pts[j], &pts[j + 1]);
            }
        }
    }
}

// Find closest pair (brute force)
void closest_pair(struct Point *pts, int n, int *i_out, int *j_out) {
    int min_d = 999999;
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = distance_sq(&pts[i], &pts[j]);
            if (d < min_d) {
                min_d = d;
                *i_out = i;
                *j_out = j;
            }
        }
    }
}

int main(void) {
    struct Point pts[5];
    pts[0].x = 3; pts[0].y = 4;
    pts[1].x = 1; pts[1].y = 2;
    pts[2].x = 5; pts[2].y = 1;
    pts[3].x = 2; pts[3].y = 5;
    pts[4].x = 4; pts[4].y = 3;

    printf("Points:\n");
    int i;
    for (i = 0; i < 5; i++) {
        printf("  (%d, %d)\n", pts[i].x, pts[i].y);
    }

    // Distance between first two
    printf("dist_sq(0,1) = %d\n", distance_sq(&pts[0], &pts[1]));

    // Swap
    printf("before swap: (%d,%d) (%d,%d)\n", pts[0].x, pts[0].y, pts[1].x, pts[1].y);
    swap_points(&pts[0], &pts[1]);
    printf("after swap: (%d,%d) (%d,%d)\n", pts[0].x, pts[0].y, pts[1].x, pts[1].y);
    swap_points(&pts[0], &pts[1]); // swap back

    // Sort by x
    sort_points_by_x(pts, 5);
    printf("Sorted by x:\n");
    for (i = 0; i < 5; i++) {
        printf("  (%d, %d)\n", pts[i].x, pts[i].y);
    }

    // Closest pair
    int ci; int cj;
    closest_pair(pts, 5, &ci, &cj);
    printf("closest pair: (%d,%d) and (%d,%d) dist_sq=%d\n",
           pts[ci].x, pts[ci].y, pts[cj].x, pts[cj].y,
           distance_sq(&pts[ci], &pts[cj]));

    // Centroid via pointer walk
    int sx = 0; int sy = 0;
    struct Point *p = pts;
    for (i = 0; i < 5; i++) {
        sx = sx + p->x;
        sy = sy + p->y;
        p = p + 1;
    }
    printf("centroid: (%d, %d)\n", sx / 5, sy / 5);

    return 0;
}
