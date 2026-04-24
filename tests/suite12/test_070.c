int printf(const char *fmt, ...);

// Map Building: Occupancy Grid
// Each cell has a value: 0=unknown, 1=free, 2=occupied
// Grid is 10x10 for simplicity, coordinates in grid cells

int grid[100];  // 10x10 flat array

int grid_width;
int grid_height;

void grid_init(int w, int h) {
    grid_width = w;
    grid_height = h;
    int i;
    for (i = 0; i < w * h; i++) {
        grid[i] = 0;  // unknown
    }
}

int grid_get(int x, int y) {
    if (x < 0 || x >= grid_width || y < 0 || y >= grid_height) return -1;
    return grid[y * grid_width + x];
}

void grid_set(int x, int y, int val) {
    if (x < 0 || x >= grid_width || y < 0 || y >= grid_height) return;
    grid[y * grid_width + x] = val;
}

// Mark a line of cells from (x0,y0) toward direction as free, final cell as occupied
// dir: 0=east, 1=north, 2=west, 3=south
// range: number of cells the sensor sees (free cells before obstacle)
void mark_ray(int x0, int y0, int dir, int range) {
    int dx = 0;
    int dy = 0;
    if (dir == 0) dx = 1;
    if (dir == 1) dy = 1;
    if (dir == 2) dx = -1;
    if (dir == 3) dy = -1;

    int cx = x0;
    int cy = y0;
    int i;
    for (i = 0; i < range; i++) {
        cx = cx + dx;
        cy = cy + dy;
        if (cx < 0 || cx >= grid_width || cy < 0 || cy >= grid_height) return;
        grid_set(cx, cy, 1);  // free
    }
    // Mark the cell at range+1 as occupied (the obstacle)
    cx = cx + dx;
    cy = cy + dy;
    if (cx >= 0 && cx < grid_width && cy >= 0 && cy < grid_height) {
        grid_set(cx, cy, 2);  // occupied
    }
}

// Count cells of each type
struct CellCounts {
    int unknown;
    int free;
    int occupied;
};

struct CellCounts count_cells(void) {
    struct CellCounts c;
    c.unknown = 0;
    c.free = 0;
    c.occupied = 0;
    int i;
    for (i = 0; i < grid_width * grid_height; i++) {
        if (grid[i] == 0) c.unknown = c.unknown + 1;
        if (grid[i] == 1) c.free = c.free + 1;
        if (grid[i] == 2) c.occupied = c.occupied + 1;
    }
    return c;
}

// Get cell type as string
int cell_name(int val) {
    if (val == 0) return 0; // unknown
    if (val == 1) return 1; // free
    if (val == 2) return 2; // occupied
    return -1;
}

// Check if position is navigable (free cell)
int is_navigable(int x, int y) {
    int v = grid_get(x, y);
    return (v == 1) ? 1 : 0;
}

// Simple flood fill to count connected free cells from a start point
// Uses a stack-based approach with arrays
int visited[100];

int flood_fill_count(int start_x, int start_y) {
    if (grid_get(start_x, start_y) != 1) return 0;

    int stack_x[100];
    int stack_y[100];
    int sp = 0;
    int count = 0;
    int i;

    for (i = 0; i < 100; i++) visited[i] = 0;

    stack_x[sp] = start_x;
    stack_y[sp] = start_y;
    sp = sp + 1;
    visited[start_y * grid_width + start_x] = 1;

    while (sp > 0) {
        sp = sp - 1;
        int cx = stack_x[sp];
        int cy = stack_y[sp];
        count = count + 1;

        // Check 4 neighbors
        int dirs_x[4];
        int dirs_y[4];
        dirs_x[0] = 1;  dirs_y[0] = 0;
        dirs_x[1] = -1; dirs_y[1] = 0;
        dirs_x[2] = 0;  dirs_y[2] = 1;
        dirs_x[3] = 0;  dirs_y[3] = -1;

        int d;
        for (d = 0; d < 4; d++) {
            int nx = cx + dirs_x[d];
            int ny = cy + dirs_y[d];
            if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height) {
                int idx = ny * grid_width + nx;
                if (grid[idx] == 1 && visited[idx] == 0) {
                    visited[idx] = 1;
                    stack_x[sp] = nx;
                    stack_y[sp] = ny;
                    sp = sp + 1;
                }
            }
        }
    }
    return count;
}

// Find nearest occupied cell from a given position (Manhattan distance)
struct NearestResult {
    int x;
    int y;
    int dist;
};

struct NearestResult find_nearest_obstacle(int fx, int fy) {
    struct NearestResult res;
    res.x = -1;
    res.y = -1;
    res.dist = 9999;

    int gx;
    int gy;
    for (gy = 0; gy < grid_height; gy++) {
        for (gx = 0; gx < grid_width; gx++) {
            if (grid_get(gx, gy) == 2) {
                int dx = gx - fx; if (dx < 0) dx = -dx;
                int dy = gy - fy; if (dy < 0) dy = -dy;
                int d = dx + dy;
                if (d < res.dist) {
                    res.dist = d;
                    res.x = gx;
                    res.y = gy;
                }
            }
        }
    }
    return res;
}

// Print a row of the grid as text
void print_row(int y) {
    int x;
    for (x = 0; x < grid_width; x++) {
        int v = grid_get(x, y);
        if (v == 0) printf(".");
        else if (v == 1) printf(" ");
        else if (v == 2) printf("#");
    }
    printf("\n");
}

int main(void) {
    // Initialize 10x10 grid
    grid_init(10, 10);

    printf("=== Grid Init ===\n");                                // EXPECT: === Grid Init ===
    struct CellCounts c0 = count_cells();
    printf("unknown=%d free=%d occ=%d\n", c0.unknown, c0.free, c0.occupied); // EXPECT: unknown=100 free=0 occ=0

    // Test basic cell operations
    printf("=== Cell Operations ===\n");                          // EXPECT: === Cell Operations ===
    grid_set(5, 5, 1);
    printf("get(5,5)=%d\n", grid_get(5, 5));                     // EXPECT: get(5,5)=1
    grid_set(5, 5, 2);
    printf("get(5,5)=%d\n", grid_get(5, 5));                     // EXPECT: get(5,5)=2
    grid_set(5, 5, 0);
    printf("out_of_bounds=%d\n", grid_get(-1, 0));                // EXPECT: out_of_bounds=-1
    printf("out_of_bounds2=%d\n", grid_get(10, 5));               // EXPECT: out_of_bounds2=-1

    // Reset grid
    grid_init(10, 10);

    // Simulate sensor readings from robot at position (2, 5)
    printf("=== Sensor Readings ===\n");                          // EXPECT: === Sensor Readings ===
    // Robot at (2,5), facing east, sees obstacle at range 4
    mark_ray(2, 5, 0, 4);  // east, 4 free cells then obstacle
    // Robot at (2,5), facing north, sees obstacle at range 2
    mark_ray(2, 5, 1, 2);  // north, 2 free cells then obstacle
    // Robot at (2,5), facing south, sees obstacle at range 3
    mark_ray(2, 5, 3, 3);  // south, 3 free cells then obstacle
    // Robot at (2,5), facing west, sees obstacle at range 1
    mark_ray(2, 5, 2, 1);  // west, 1 free cell then obstacle

    struct CellCounts c1 = count_cells();
    printf("after scan: unknown=%d free=%d occ=%d\n", c1.unknown, c1.free, c1.occupied); // EXPECT: after scan: unknown=86 free=10 occ=4

    // Check specific cells
    printf("robot_pos=%d\n", grid_get(2, 5));                     // EXPECT: robot_pos=0
    printf("east_free=%d\n", grid_get(3, 5));                     // EXPECT: east_free=1
    printf("east_free2=%d\n", grid_get(4, 5));                    // EXPECT: east_free2=1
    printf("east_occ=%d\n", grid_get(7, 5));                      // EXPECT: east_occ=2
    printf("west_free=%d\n", grid_get(1, 5));                     // EXPECT: west_free=1
    printf("west_occ=%d\n", grid_get(0, 5));                      // EXPECT: west_occ=2
    printf("north_occ=%d\n", grid_get(2, 8));                     // EXPECT: north_occ=2
    printf("south_occ=%d\n", grid_get(2, 2));                     // EXPECT: south_occ=1

    // Navigability test
    printf("=== Navigability ===\n");                             // EXPECT: === Navigability ===
    printf("nav(3,5)=%d\n", is_navigable(3, 5));                  // EXPECT: nav(3,5)=1
    printf("nav(7,5)=%d\n", is_navigable(7, 5));                  // EXPECT: nav(7,5)=0
    printf("nav(0,0)=%d\n", is_navigable(0, 0));                  // EXPECT: nav(0,0)=0

    // Mark robot position as free too
    grid_set(2, 5, 1);

    // Flood fill from robot position
    printf("=== Flood Fill ===\n");                               // EXPECT: === Flood Fill ===
    int connected = flood_fill_count(2, 5);
    printf("connected_free=%d\n", connected);                     // EXPECT: connected_free=11

    // Try flood fill from occupied cell
    int occ_fill = flood_fill_count(7, 5);
    printf("occ_fill=%d\n", occ_fill);                            // EXPECT: occ_fill=0

    // Try flood fill from unknown cell
    int unk_fill = flood_fill_count(9, 9);
    printf("unk_fill=%d\n", unk_fill);                            // EXPECT: unk_fill=0

    // Find nearest obstacle from robot position
    printf("=== Nearest Obstacle ===\n");                         // EXPECT: === Nearest Obstacle ===
    struct NearestResult nr = find_nearest_obstacle(2, 5);
    printf("nearest: dist=%d\n", nr.dist);                        // EXPECT: nearest: dist=2

    // Add more sensor data - second scan from (5, 5)
    printf("=== Second Scan ===\n");                              // EXPECT: === Second Scan ===
    grid_set(5, 5, 1);  // mark new robot position as free
    mark_ray(5, 5, 1, 3);  // north, 3 free then obstacle
    mark_ray(5, 5, 3, 4);  // south, 4 free then obstacle

    struct CellCounts c2 = count_cells();
    printf("after scan2: unknown=%d free=%d occ=%d\n", c2.unknown, c2.free, c2.occupied); // EXPECT: after scan2: unknown=76 free=18 occ=6

    // Test grid boundary handling
    printf("=== Boundary Test ===\n");                            // EXPECT: === Boundary Test ===
    grid_init(10, 10);
    // Shoot ray from corner
    mark_ray(0, 0, 0, 8);  // east from (0,0), 8 free + 1 occupied
    struct CellCounts c3 = count_cells();
    printf("boundary: free=%d occ=%d\n", c3.free, c3.occupied);  // EXPECT: boundary: free=8 occ=1

    // Shoot ray that goes off edge
    grid_init(10, 10);
    mark_ray(0, 0, 0, 20);  // would go past grid
    struct CellCounts c4 = count_cells();
    printf("offedge: free=%d occ=%d\n", c4.free, c4.occupied);   // EXPECT: offedge: free=9 occ=0

    // Build a room (walls around perimeter)
    printf("=== Room Building ===\n");                            // EXPECT: === Room Building ===
    grid_init(10, 10);
    int wx;
    int wy;
    // Top and bottom walls
    for (wx = 0; wx < 10; wx++) {
        grid_set(wx, 0, 2);   // bottom wall
        grid_set(wx, 9, 2);   // top wall
    }
    // Left and right walls
    for (wy = 0; wy < 10; wy++) {
        grid_set(0, wy, 2);   // left wall
        grid_set(9, wy, 2);   // right wall
    }
    // Interior is free
    int ix;
    int iy;
    for (iy = 1; iy < 9; iy++) {
        for (ix = 1; ix < 9; ix++) {
            grid_set(ix, iy, 1);
        }
    }
    struct CellCounts c5 = count_cells();
    printf("room: unknown=%d free=%d occ=%d\n", c5.unknown, c5.free, c5.occupied); // EXPECT: room: unknown=0 free=64 occ=36

    // Add an obstacle inside the room
    grid_set(5, 5, 2);
    grid_set(5, 6, 2);
    grid_set(6, 5, 2);
    grid_set(6, 6, 2);

    // Check connected regions
    int region1 = flood_fill_count(1, 1);
    printf("region_size=%d\n", region1);                          // EXPECT: region_size=60

    // Nearest obstacle from center of room
    struct NearestResult nr2 = find_nearest_obstacle(3, 3);
    printf("nearest_from_center: dist=%d\n", nr2.dist);           // EXPECT: nearest_from_center: dist=3

    printf("PASS\n");                                             // EXPECT: PASS
    return 0;
}
