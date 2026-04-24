int printf(const char *fmt, ...);

// Circle approximation via integer grid
// Count grid points within a radius, compute area approximation
// Use squared radius comparisons to stay fully integer

struct Point {
    int x;
    int y;
};

// Count grid points strictly inside circle of radius r centered at origin
// (x^2 + y^2 < r^2)
int count_interior_grid(int r) {
    int count;
    int x;
    int y;
    int r2;
    r2 = r * r;
    count = 0;
    for (x = -r + 1; x < r; x = x + 1) {
        for (y = -r + 1; y < r; y = y + 1) {
            if (x * x + y * y < r2) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Count grid points on or inside circle (x^2 + y^2 <= r^2)
int count_grid_leq(int r) {
    int count;
    int x;
    int y;
    int r2;
    r2 = r * r;
    count = 0;
    for (x = -r; x <= r; x = x + 1) {
        for (y = -r; y <= r; y = y + 1) {
            if (x * x + y * y <= r2) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Count grid points exactly on the circle boundary (x^2 + y^2 = r^2)
int count_boundary(int r) {
    int count;
    int x;
    int y;
    int r2;
    r2 = r * r;
    count = 0;
    for (x = -r; x <= r; x = x + 1) {
        for (y = -r; y <= r; y = y + 1) {
            if (x * x + y * y == r2) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Approximate pi * 100 using grid counting
// Area of circle = pi * r^2, grid count ~ area
// So pi ~ count / r^2, pi*100 ~ count*100 / r^2
int approx_pi100(int r) {
    int count;
    count = count_grid_leq(r);
    return (count * 100) / (r * r);
}

// Count grid points in annulus (r_inner < dist <= r_outer)
int count_annulus(int r_inner, int r_outer) {
    int count;
    int x;
    int y;
    int d2;
    int ri2;
    int ro2;
    ri2 = r_inner * r_inner;
    ro2 = r_outer * r_outer;
    count = 0;
    for (x = -r_outer; x <= r_outer; x = x + 1) {
        for (y = -r_outer; y <= r_outer; y = y + 1) {
            d2 = x * x + y * y;
            if (d2 > ri2 && d2 <= ro2) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Count grid points in first quadrant only (x>0, y>0) inside circle
int count_quadrant(int r) {
    int count;
    int x;
    int y;
    int r2;
    r2 = r * r;
    count = 0;
    for (x = 1; x < r; x = x + 1) {
        for (y = 1; y < r; y = y + 1) {
            if (x * x + y * y < r2) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Compute the diameter of a circle in grid steps along x-axis
// Counts how many grid points along y=0 are inside [-(r), r]
int grid_diameter(int r) {
    int count;
    int x;
    count = 0;
    for (x = -r; x <= r; x = x + 1) {
        if (x * x <= r * r) count = count + 1;
    }
    return count;
}

// Draw a small circle to a grid and count filled cells
// Uses a character array as a grid
void draw_circle_grid(int r, int grid_size) {
    char grid[400]; // 20x20 max
    int x;
    int y;
    int r2;
    int idx;
    int row_count;

    r2 = r * r;
    // Initialize grid
    for (idx = 0; idx < grid_size * grid_size; idx = idx + 1) {
        grid[idx] = '.';
    }

    // Fill circle
    for (y = 0; y < grid_size; y = y + 1) {
        for (x = 0; x < grid_size; x = x + 1) {
            int dx;
            int dy;
            dx = x - r;
            dy = y - r;
            if (dx * dx + dy * dy <= r2) {
                grid[y * grid_size + x] = '*';
            }
        }
    }

    // Count stars per row and print row info
    for (y = 0; y < grid_size; y = y + 1) {
        row_count = 0;
        for (x = 0; x < grid_size; x = x + 1) {
            if (grid[y * grid_size + x] == '*') row_count = row_count + 1;
        }
        if (row_count > 0) {
            printf("row %d: %d cells\n", y, row_count);
        }
    }
}

int main() {
    int count;
    int pi100;

    // Interior points for small radii
    count = count_interior_grid(1);
    printf("interior r=1: %d\n", count); // EXPECT: interior r=1: 1

    count = count_interior_grid(2);
    printf("interior r=2: %d\n", count); // EXPECT: interior r=2: 9

    count = count_interior_grid(3);
    printf("interior r=3: %d\n", count); // EXPECT: interior r=3: 25

    count = count_interior_grid(5);
    printf("interior r=5: %d\n", count); // EXPECT: interior r=5: 69

    // Points on or inside
    count = count_grid_leq(1);
    printf("leq r=1: %d\n", count); // EXPECT: leq r=1: 5

    count = count_grid_leq(2);
    printf("leq r=2: %d\n", count); // EXPECT: leq r=2: 13

    count = count_grid_leq(3);
    printf("leq r=3: %d\n", count); // EXPECT: leq r=3: 29

    count = count_grid_leq(5);
    printf("leq r=5: %d\n", count); // EXPECT: leq r=5: 81

    // Boundary points
    count = count_boundary(1);
    printf("boundary r=1: %d\n", count); // EXPECT: boundary r=1: 4

    count = count_boundary(5);
    printf("boundary r=5: %d\n", count); // EXPECT: boundary r=5: 12

    // Pi approximation (pi*100 ~ 314)
    pi100 = approx_pi100(5);
    printf("pi*100 approx r=5: %d\n", pi100); // EXPECT: pi*100 approx r=5: 324

    pi100 = approx_pi100(10);
    printf("pi*100 approx r=10: %d\n", pi100); // EXPECT: pi*100 approx r=10: 317

    // Annulus
    count = count_annulus(2, 5);
    printf("annulus 2<r<=5: %d\n", count); // EXPECT: annulus 2<r<=5: 68

    // Quadrant count
    count = count_quadrant(5);
    printf("quadrant r=5: %d\n", count); // EXPECT: quadrant r=5: 13

    // Grid diameter
    count = grid_diameter(5);
    printf("diameter r=5: %d\n", count); // EXPECT: diameter r=5: 11

    // Draw circle r=3 on 7x7 grid
    printf("circle r=3:\n"); // EXPECT: circle r=3:
    draw_circle_grid(3, 7);
    // EXPECT: row 0: 1 cells
    // EXPECT: row 1: 5 cells
    // EXPECT: row 2: 5 cells
    // EXPECT: row 3: 7 cells
    // EXPECT: row 4: 5 cells
    // EXPECT: row 5: 5 cells
    // EXPECT: row 6: 1 cells

    return 0;
}
