int printf(const char *fmt, ...);

int count_grid_points_in_circle(int r) {
    int count = 0;
    int r_sq = r * r;
    int x;
    int y;
    for (y = -r; y <= r; y++) {
        for (x = -r; x <= r; x++) {
            if (x * x + y * y <= r_sq) {
                count++;
            }
        }
    }
    return count;
}

int count_boundary_points(int r) {
    int count = 0;
    int r_sq = r * r;
    int x;
    int y;
    for (y = -r; y <= r; y++) {
        for (x = -r; x <= r; x++) {
            int d = x * x + y * y;
            if (d <= r_sq && (d > (r - 1) * (r - 1))) {
                count++;
            }
        }
    }
    return count;
}

int count_interior_points(int r) {
    int total = count_grid_points_in_circle(r);
    int boundary = count_boundary_points(r);
    return total - boundary;
}

int count_points_in_annulus(int r_inner, int r_outer) {
    int count = 0;
    int ri_sq = r_inner * r_inner;
    int ro_sq = r_outer * r_outer;
    int x;
    int y;
    for (y = -r_outer; y <= r_outer; y++) {
        for (x = -r_outer; x <= r_outer; x++) {
            int d = x * x + y * y;
            if (d <= ro_sq && d > ri_sq) {
                count++;
            }
        }
    }
    return count;
}

int count_quadrant_points(int r) {
    int count = 0;
    int r_sq = r * r;
    int x;
    int y;
    for (y = 0; y <= r; y++) {
        for (x = 0; x <= r; x++) {
            if (x * x + y * y <= r_sq) {
                count++;
            }
        }
    }
    return count;
}

int count_lattice_on_circle(int r_sq) {
    int count = 0;
    int r;
    for (r = 0; r * r <= r_sq; r++) {}
    r = r - 1;
    int x;
    int y;
    for (y = -r - 1; y <= r + 1; y++) {
        for (x = -r - 1; x <= r + 1; x++) {
            if (x * x + y * y == r_sq) {
                count++;
            }
        }
    }
    return count;
}

int gauss_circle_approx_times4(int r) {
    int n = count_grid_points_in_circle(r);
    return n;
}

int main(void) {
    int c0 = count_grid_points_in_circle(0);
    printf("%d\n", c0);
    // EXPECT: 1

    int c1 = count_grid_points_in_circle(1);
    printf("%d\n", c1);
    // EXPECT: 5

    int c2 = count_grid_points_in_circle(2);
    printf("%d\n", c2);
    // EXPECT: 13

    int c3 = count_grid_points_in_circle(3);
    printf("%d\n", c3);
    // EXPECT: 29

    int c4 = count_grid_points_in_circle(4);
    printf("%d\n", c4);
    // EXPECT: 49

    int c5 = count_grid_points_in_circle(5);
    printf("%d\n", c5);
    // EXPECT: 81

    int q1 = count_quadrant_points(1);
    printf("%d\n", q1);
    // EXPECT: 3

    int q3 = count_quadrant_points(3);
    printf("%d\n", q3);
    // EXPECT: 11

    int q5 = count_quadrant_points(5);
    printf("%d\n", q5);
    // EXPECT: 26

    int b2 = count_boundary_points(2);
    printf("%d\n", b2);
    // EXPECT: 8

    int b3 = count_boundary_points(3);
    printf("%d\n", b3);
    // EXPECT: 16

    int i3 = count_interior_points(3);
    printf("%d\n", i3);
    // EXPECT: 13

    int ann = count_points_in_annulus(1, 3);
    printf("%d\n", ann);
    // EXPECT: 24

    int ann2 = count_points_in_annulus(2, 5);
    printf("%d\n", ann2);
    // EXPECT: 68

    int ann3 = count_points_in_annulus(0, 2);
    printf("%d\n", ann3);
    // EXPECT: 12

    int lat1 = count_lattice_on_circle(1);
    printf("%d\n", lat1);
    // EXPECT: 4

    int lat5 = count_lattice_on_circle(5);
    printf("%d\n", lat5);
    // EXPECT: 8

    int lat25 = count_lattice_on_circle(25);
    printf("%d\n", lat25);
    // EXPECT: 12

    int c10 = count_grid_points_in_circle(10);
    printf("%d\n", c10);
    // EXPECT: 317

    int q10 = count_quadrant_points(10);
    printf("%d\n", q10);
    // EXPECT: 90

    printf("done\n");
    // EXPECT: done

    return 0;
}
