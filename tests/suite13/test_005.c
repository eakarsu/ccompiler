int printf(const char *fmt, ...);

// Queue-based BFS on a grid
// Find shortest path in a grid from top-left to bottom-right
// 0 = passable, 1 = wall

int grid[8][8];
int visited[8][8];
int dist[8][8];

struct Queue {
    int data[256];
    int head;
    int tail;
    int count;
};

void queue_init(struct Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_empty(struct Queue *q) {
    return q->count == 0;
}

void queue_enqueue(struct Queue *q, int val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % 256;
    q->count = q->count + 1;
}

int queue_dequeue(struct Queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % 256;
    q->count = q->count - 1;
    return val;
}

// Encode row,col as single int
int encode(int r, int c) {
    return r * 8 + c;
}

int decode_row(int val) {
    return val / 8;
}

int decode_col(int val) {
    return val % 8;
}

// BFS shortest path, returns distance or -1
int bfs_shortest(int rows, int cols, int sr, int sc, int er, int ec) {
    struct Queue q;
    queue_init(&q);

    int r = 0;
    while (r < rows) {
        int c = 0;
        while (c < cols) {
            visited[r][c] = 0;
            dist[r][c] = -1;
            c = c + 1;
        }
        r = r + 1;
    }

    visited[sr][sc] = 1;
    dist[sr][sc] = 0;
    queue_enqueue(&q, encode(sr, sc));

    int dr[4];
    int dc[4];
    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0; dc[1] = 0; dc[2] = -1; dc[3] = 1;

    while (!queue_empty(&q)) {
        int cur = queue_dequeue(&q);
        int cr = decode_row(cur);
        int cc = decode_col(cur);

        if (cr == er && cc == ec) {
            return dist[cr][cc];
        }

        int i = 0;
        while (i < 4) {
            int nr = cr + dr[i];
            int nc = cc + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (grid[nr][nc] == 0 && visited[nr][nc] == 0) {
                    visited[nr][nc] = 1;
                    dist[nr][nc] = dist[cr][cc] + 1;
                    queue_enqueue(&q, encode(nr, nc));
                }
            }
            i = i + 1;
        }
    }
    return -1;
}

// Count reachable cells from a start point
int count_reachable(int rows, int cols, int sr, int sc) {
    struct Queue q;
    queue_init(&q);

    int r = 0;
    while (r < rows) {
        int c = 0;
        while (c < cols) {
            visited[r][c] = 0;
            c = c + 1;
        }
        r = r + 1;
    }

    visited[sr][sc] = 1;
    queue_enqueue(&q, encode(sr, sc));
    int count = 1;

    int dr[4];
    int dc[4];
    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0; dc[1] = 0; dc[2] = -1; dc[3] = 1;

    while (!queue_empty(&q)) {
        int cur = queue_dequeue(&q);
        int cr = decode_row(cur);
        int cc = decode_col(cur);
        int i = 0;
        while (i < 4) {
            int nr = cr + dr[i];
            int nc = cc + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (grid[nr][nc] == 0 && visited[nr][nc] == 0) {
                    visited[nr][nc] = 1;
                    count = count + 1;
                    queue_enqueue(&q, encode(nr, nc));
                }
            }
            i = i + 1;
        }
    }
    return count;
}

void clear_grid(int rows, int cols) {
    int r = 0;
    while (r < rows) {
        int c = 0;
        while (c < cols) {
            grid[r][c] = 0;
            c = c + 1;
        }
        r = r + 1;
    }
}

int main() {
    // Test 1: Simple 5x5 grid with no walls
    clear_grid(5, 5);
    int d = bfs_shortest(5, 5, 0, 0, 4, 4);
    // Manhattan distance = 8
    printf("%d\n", d);
    // EXPECT: 8

    // Test 2: 5x5 with walls forcing detour
    // Grid:
    // 0 0 0 0 0
    // 1 1 1 0 0
    // 0 0 0 0 0
    // 0 0 1 1 1
    // 0 0 0 0 0
    clear_grid(5, 5);
    grid[1][0] = 1; grid[1][1] = 1; grid[1][2] = 1;
    grid[3][2] = 1; grid[3][3] = 1; grid[3][4] = 1;
    d = bfs_shortest(5, 5, 0, 0, 4, 4);
    printf("%d\n", d);
    // EXPECT: 12

    // Test 3: Unreachable destination
    clear_grid(4, 4);
    // Wall completely blocking row 2
    grid[2][0] = 1; grid[2][1] = 1; grid[2][2] = 1; grid[2][3] = 1;
    d = bfs_shortest(4, 4, 0, 0, 3, 3);
    printf("%d\n", d);
    // EXPECT: -1

    // Test 4: Start equals end
    clear_grid(3, 3);
    d = bfs_shortest(3, 3, 1, 1, 1, 1);
    printf("%d\n", d);
    // EXPECT: 0

    // Test 5: Count reachable cells in open grid
    clear_grid(4, 4);
    int r = count_reachable(4, 4, 0, 0);
    printf("%d\n", r);
    // EXPECT: 16

    // Test 6: Count reachable with walls creating isolated areas
    clear_grid(4, 4);
    // Create vertical wall at col 2
    grid[0][2] = 1; grid[1][2] = 1; grid[2][2] = 1; grid[3][2] = 1;
    r = count_reachable(4, 4, 0, 0);
    // Only left part reachable: columns 0,1 = 8 cells
    printf("%d\n", r);
    // EXPECT: 8

    // Test 7: Maze-like grid
    // 0 0 1 0 0
    // 0 1 0 0 1
    // 0 0 0 1 0
    // 1 1 0 0 0
    // 0 0 0 1 0
    clear_grid(5, 5);
    grid[0][2] = 1;
    grid[1][1] = 1; grid[1][4] = 1;
    grid[2][3] = 1;
    grid[3][0] = 1; grid[3][1] = 1;
    grid[4][3] = 1;
    d = bfs_shortest(5, 5, 0, 0, 4, 4);
    printf("%d\n", d);
    // EXPECT: 8

    // Test 8: Narrow corridor
    // 0 1 1 1
    // 0 1 1 1
    // 0 0 0 0
    // 1 1 1 0
    clear_grid(4, 4);
    grid[0][1] = 1; grid[0][2] = 1; grid[0][3] = 1;
    grid[1][1] = 1; grid[1][2] = 1; grid[1][3] = 1;
    grid[3][0] = 1; grid[3][1] = 1; grid[3][2] = 1;
    d = bfs_shortest(4, 4, 0, 0, 3, 3);
    printf("%d\n", d);
    // EXPECT: 6

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
