int printf(const char *fmt, ...);

// Path Planning (Grid-based BFS)
// Finds shortest path in a grid with obstacles

struct Point {
    int x;
    int y;
};

struct Queue {
    int data_x[200];
    int data_y[200];
    int front;
    int rear;
    int size;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

void queue_push(struct Queue *q, int x, int y) {
    q->data_x[q->rear] = x;
    q->data_y[q->rear] = y;
    q->rear = q->rear + 1;
    q->size = q->size + 1;
}

int queue_empty(struct Queue *q) {
    return q->size == 0;
}

struct Point queue_pop(struct Queue *q) {
    struct Point p;
    p.x = q->data_x[q->front];
    p.y = q->data_y[q->front];
    q->front = q->front + 1;
    q->size = q->size - 1;
    return p;
}

int grid[8][8];
int visited[8][8];
int dist[8][8];
int parent_x[8][8];
int parent_y[8][8];

void init_grid() {
    int i;
    int j;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            grid[i][j] = 0;
            visited[i][j] = 0;
            dist[i][j] = -1;
            parent_x[i][j] = -1;
            parent_y[i][j] = -1;
        }
    }
}

void add_obstacle(int x, int y) {
    grid[x][y] = 1;
}

int is_valid(int x, int y) {
    if (x < 0 || x >= 8) return 0;
    if (y < 0 || y >= 8) return 0;
    if (grid[x][y] == 1) return 0;
    if (visited[x][y] == 1) return 0;
    return 1;
}

int bfs(int sx, int sy, int ex, int ey) {
    struct Queue q;
    struct Point cur;
    int dx[4];
    int dy[4];
    int i;
    int nx;
    int ny;

    dx[0] = 0; dx[1] = 0; dx[2] = 1; dx[3] = -1;
    dy[0] = 1; dy[1] = -1; dy[2] = 0; dy[3] = 0;

    queue_init(&q);
    queue_push(&q, sx, sy);
    visited[sx][sy] = 1;
    dist[sx][sy] = 0;

    while (!queue_empty(&q)) {
        cur = queue_pop(&q);
        if (cur.x == ex && cur.y == ey) {
            return dist[ex][ey];
        }
        for (i = 0; i < 4; i = i + 1) {
            nx = cur.x + dx[i];
            ny = cur.y + dy[i];
            if (is_valid(nx, ny)) {
                visited[nx][ny] = 1;
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                parent_x[nx][ny] = cur.x;
                parent_y[nx][ny] = cur.y;
                queue_push(&q, nx, ny);
            }
        }
    }
    return -1;
}

void print_path(int sx, int sy, int ex, int ey) {
    int path_x[64];
    int path_y[64];
    int len;
    int cx;
    int cy;
    int temp;
    int i;

    len = 0;
    cx = ex;
    cy = ey;
    while (cx != sx || cy != sy) {
        path_x[len] = cx;
        path_y[len] = cy;
        len = len + 1;
        temp = parent_x[cx][cy];
        cy = parent_y[cx][cy];
        cx = temp;
    }
    path_x[len] = sx;
    path_y[len] = sy;
    len = len + 1;

    printf("Path (%d steps): ", len - 1);
    for (i = len - 1; i >= 0; i = i - 1) {
        printf("(%d,%d)", path_x[i], path_y[i]);
        if (i > 0) printf("->");
    }
    printf("\n");
}

void print_grid() {
    int i;
    int j;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            if (grid[i][j] == 1) printf("X");
            else printf(".");
        }
        printf("\n");
    }
}

int main() {
    int d;

    printf("=== Path Planning Test ===\n");
    // EXPECT: === Path Planning Test ===

    // Test 1: Simple path
    init_grid();
    d = bfs(0, 0, 3, 3);
    printf("Distance (0,0)->(3,3) no obstacles: %d\n", d);
    // EXPECT: Distance (0,0)->(3,3) no obstacles: 6
    print_path(0, 0, 3, 3);
    // EXPECT: Path (6 steps): (0,0)->(0,1)->(0,2)->(0,3)->(1,3)->(2,3)->(3,3)

    // Test 2: With wall
    init_grid();
    add_obstacle(1, 0);
    add_obstacle(1, 1);
    add_obstacle(1, 2);
    d = bfs(0, 0, 2, 0);
    printf("Distance with wall: %d\n", d);
    // EXPECT: Distance with wall: 8

    // Test 3: Blocked
    init_grid();
    add_obstacle(1, 0);
    add_obstacle(1, 1);
    add_obstacle(1, 2);
    add_obstacle(1, 3);
    add_obstacle(1, 4);
    add_obstacle(1, 5);
    add_obstacle(1, 6);
    add_obstacle(1, 7);
    d = bfs(0, 0, 2, 0);
    printf("Blocked path: %d\n", d);
    // EXPECT: Blocked path: -1

    // Test 4: Adjacent
    init_grid();
    d = bfs(0, 0, 0, 1);
    printf("Adjacent: %d\n", d);
    // EXPECT: Adjacent: 1

    // Test 5: Same cell
    init_grid();
    d = bfs(3, 3, 3, 3);
    printf("Same cell: %d\n", d);
    // EXPECT: Same cell: 0

    // Test 6: Grid print
    init_grid();
    add_obstacle(2, 2);
    add_obstacle(2, 3);
    add_obstacle(3, 2);
    printf("Grid with obstacles:\n");
    // EXPECT: Grid with obstacles:
    print_grid();
    // EXPECT: ........
    // EXPECT: ........
    // EXPECT: ..XX....
    // EXPECT: ..X.....
    // EXPECT: ........
    // EXPECT: ........
    // EXPECT: ........
    // EXPECT: ........

    printf("Path planning done\n");
    // EXPECT: Path planning done

    return 0;
}
