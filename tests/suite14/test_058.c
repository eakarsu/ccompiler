int printf(const char *fmt, ...);

// Snake game simulation (step-based)
// Grid 10x10. Snake moves by direction. Eats food to grow.

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[50]; // body[0] = head
    int length;
    int dir; // 0=right, 1=down, 2=left, 3=up
    int alive;
    int score;
} Snake;

typedef struct {
    int grid[10][10]; // 0=empty, 1=snake, 2=food
    Snake snake;
    int food_x;
    int food_y;
} Game;

void game_init(Game *g) {
    int r, c;
    for (r = 0; r < 10; r++)
        for (c = 0; c < 10; c++)
            g->grid[r][c] = 0;

    g->snake.length = 3;
    g->snake.body[0].x = 5;
    g->snake.body[0].y = 4;
    g->snake.body[1].x = 4;
    g->snake.body[1].y = 4;
    g->snake.body[2].x = 3;
    g->snake.body[2].y = 4;
    g->snake.dir = 0; // right
    g->snake.alive = 1;
    g->snake.score = 0;

    int i;
    for (i = 0; i < g->snake.length; i++) {
        g->grid[g->snake.body[i].y][g->snake.body[i].x] = 1;
    }

    // Place food
    g->food_x = 7;
    g->food_y = 4;
    g->grid[g->food_y][g->food_x] = 2;
}

void place_food(Game *g, int fx, int fy) {
    g->food_x = fx;
    g->food_y = fy;
    g->grid[fy][fx] = 2;
}

// dir: 0=right, 1=down, 2=left, 3=up
int dx_arr[4];
int dy_arr[4];

void init_dirs() {
    dx_arr[0] = 1;  dy_arr[0] = 0;  // right
    dx_arr[1] = 0;  dy_arr[1] = 1;  // down
    dx_arr[2] = -1; dy_arr[2] = 0;  // left
    dx_arr[3] = 0;  dy_arr[3] = -1; // up
}

void set_direction(Game *g, int dir) {
    // Prevent 180-degree turn
    if ((g->snake.dir + 2) % 4 == dir) return;
    g->snake.dir = dir;
}

int step(Game *g) {
    if (!g->snake.alive) return 0;

    int head_x = g->snake.body[0].x + dx_arr[g->snake.dir];
    int head_y = g->snake.body[0].y + dy_arr[g->snake.dir];

    // Check wall collision
    if (head_x < 0 || head_x >= 10 || head_y < 0 || head_y >= 10) {
        g->snake.alive = 0;
        return -1;
    }

    // Check self collision (before moving tail)
    int ate_food = (head_x == g->food_x && head_y == g->food_y);

    // Check body collision (check all segments except tail if not eating)
    int check_len = g->snake.length;
    if (!ate_food) check_len--; // tail will move away
    int i;
    for (i = 0; i < check_len; i++) {
        if (g->snake.body[i].x == head_x && g->snake.body[i].y == head_y) {
            g->snake.alive = 0;
            return -1;
        }
    }

    // Move tail (clear last segment unless eating)
    if (!ate_food) {
        int tail_idx = g->snake.length - 1;
        g->grid[g->snake.body[tail_idx].y][g->snake.body[tail_idx].x] = 0;
    }

    // Shift body segments backward
    if (ate_food) {
        // Grow: shift all segments back by 1, inserting new head
        for (i = g->snake.length; i > 0; i--) {
            g->snake.body[i].x = g->snake.body[i-1].x;
            g->snake.body[i].y = g->snake.body[i-1].y;
        }
        g->snake.length++;
        g->snake.score++;
    } else {
        for (i = g->snake.length - 1; i > 0; i--) {
            g->snake.body[i].x = g->snake.body[i-1].x;
            g->snake.body[i].y = g->snake.body[i-1].y;
        }
    }

    // Set new head
    g->snake.body[0].x = head_x;
    g->snake.body[0].y = head_y;
    g->grid[head_y][head_x] = 1;

    if (ate_food) return 2; // ate food
    return 1; // normal move
}

int main() {
    init_dirs();

    Game g;
    game_init(&g);

    printf("init_len: %d\n", g.snake.length);
    // EXPECT: init_len: 3
    printf("head: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head: 5 4
    printf("alive: %d\n", g.snake.alive);
    // EXPECT: alive: 1

    // Move right towards food at (7,4)
    int r1 = step(&g);
    printf("step1: %d\n", r1);
    // EXPECT: step1: 1
    printf("head1: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head1: 6 4

    int r2 = step(&g);
    printf("step2: %d\n", r2);
    // EXPECT: step2: 2
    printf("head2: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head2: 7 4
    printf("len_after_eat: %d\n", g.snake.length);
    // EXPECT: len_after_eat: 4
    printf("score: %d\n", g.snake.score);
    // EXPECT: score: 1

    // Place new food and continue
    place_food(&g, 7, 2);

    // Turn up
    set_direction(&g, 3);
    int r3 = step(&g);
    printf("step3: %d\n", r3);
    // EXPECT: step3: 1
    printf("head3: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head3: 7 3

    int r4 = step(&g);
    printf("step4: %d\n", r4);
    // EXPECT: step4: 2
    printf("head4: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head4: 7 2
    printf("len_after_eat2: %d\n", g.snake.length);
    // EXPECT: len_after_eat2: 5

    // Turn left and go to wall
    set_direction(&g, 2);
    // Try to reverse direction (should be ignored since we're going up... wait, we set dir to 3=up, now setting 2=left is fine)
    // Actually we just set 3=up, then stepped twice. Now we set 2=left.
    int r5 = step(&g);
    printf("step5: %d\n", r5);
    // EXPECT: step5: 1
    printf("head5: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head5: 6 2

    // Test 180-degree prevention: currently going left(2), try to go right(0)
    set_direction(&g, 0);
    // Should be ignored, still going left
    int r6 = step(&g);
    printf("step6: %d\n", r6);
    // EXPECT: step6: 1
    printf("head6: %d %d\n", g.snake.body[0].x, g.snake.body[0].y);
    // EXPECT: head6: 5 2

    // Drive into wall: keep going left
    int steps_to_wall = 0;
    int result = 1;
    while (result > 0) {
        result = step(&g);
        steps_to_wall++;
    }
    // Head was at (5,2), going left. Steps:
    // (4,2)ok, (3,2)ok, (2,2)ok, (1,2)ok, (0,2)ok, (-1,2)=wall
    // That's 6 iterations, last one hits wall
    printf("steps_to_wall: %d\n", steps_to_wall);
    // EXPECT: steps_to_wall: 6
    printf("dead: %d\n", g.snake.alive);
    // EXPECT: dead: 0
    printf("final_score: %d\n", g.snake.score);
    // EXPECT: final_score: 2

    printf("done\n");
    // EXPECT: done
    return 0;
}
