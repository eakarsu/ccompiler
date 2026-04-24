int printf(const char *fmt, ...);
// EXPECT: Snake Game Simulation\nTick 0: head=(4,4) dir=RIGHT len=3 score=0 food=(6,4) alive=1\n\nMoving right to food:\nTick 1: head=(5,4) dir=RIGHT len=3 score=0 food=(6,4) alive=1\n  Ate food! Score: 10\nTick 2: head=(6,4) dir=RIGHT len=4 score=10 food=(1,1) alive=1\n\nTurn down:\nTick 3: head=(6,5) dir=DOWN len=4 score=10 food=(1,1) alive=1\n\nTurn left:\nTick 4: head=(5,5) dir=LEFT len=4 score=10 food=(1,1) alive=1\n\nContinue:\nTick 5: head=(4,5) dir=LEFT len=4 score=10 food=(1,1) alive=1\nTick 6: head=(3,5) dir=LEFT len=4 score=10 food=(1,1) alive=1\n\nSnake body: (3,5) (4,5) (5,5) (6,5) \nDistance to food: 6\nFinal score: 10 in 6 ticks
#define MAX_SIZE 8

enum Direction { DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3 };

struct Point {
    int x;
    int y;
};

struct Snake {
    struct Point body[MAX_SIZE * MAX_SIZE];
    int length;
    int direction;
    int alive;
    int score;
};

struct Game {
    int grid[MAX_SIZE][MAX_SIZE];
    struct Snake snake;
    struct Point food;
    int ticks;
};

typedef void (*UpdateFn)(struct Game *);

int abs_val(int x) { return x < 0 ? -x : x; }

void point_print(struct Point *p) {
    printf("(%d,%d)", p->x, p->y);
}

char *dir_name(int d) {
    switch (d) {
        case DIR_UP: return "UP";
        case DIR_RIGHT: return "RIGHT";
        case DIR_DOWN: return "DOWN";
        case DIR_LEFT: return "LEFT";
    }
    return "?";
}

void game_init(struct Game *g) {
    int i;
    int j;
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            g->grid[i][j] = 0;
        }
    }
    g->snake.length = 3;
    g->snake.body[0].x = 4; g->snake.body[0].y = 4;
    g->snake.body[1].x = 3; g->snake.body[1].y = 4;
    g->snake.body[2].x = 2; g->snake.body[2].y = 4;
    g->snake.direction = DIR_RIGHT;
    g->snake.alive = 1;
    g->snake.score = 0;
    g->food.x = 6; g->food.y = 4;
    g->ticks = 0;
}

int dx_for_dir(int d) {
    if (d == DIR_RIGHT) return 1;
    if (d == DIR_LEFT) return -1;
    return 0;
}

int dy_for_dir(int d) {
    if (d == DIR_DOWN) return 1;
    if (d == DIR_UP) return -1;
    return 0;
}

int check_collision(struct Snake *s, int x, int y) {
    int i;
    for (i = 0; i < s->length; i++) {
        if (s->body[i].x == x && s->body[i].y == y) return 1;
    }
    return 0;
}

void game_update(struct Game *g) {
    if (!g->snake.alive) return;
    g->ticks++;

    int new_x = g->snake.body[0].x + dx_for_dir(g->snake.direction);
    int new_y = g->snake.body[0].y + dy_for_dir(g->snake.direction);

    if (new_x < 0 || new_x >= MAX_SIZE || new_y < 0 || new_y >= MAX_SIZE) {
        g->snake.alive = 0;
        printf("  Wall collision at (%d,%d)!\n", new_x, new_y);
        return;
    }

    if (check_collision(&g->snake, new_x, new_y)) {
        g->snake.alive = 0;
        printf("  Self collision at (%d,%d)!\n", new_x, new_y);
        return;
    }

    int ate_food = (new_x == g->food.x && new_y == g->food.y);
    int i;

    if (!ate_food) {
        for (i = g->snake.length - 1; i > 0; i--) {
            g->snake.body[i] = g->snake.body[i - 1];
        }
    } else {
        for (i = g->snake.length; i > 0; i--) {
            g->snake.body[i] = g->snake.body[i - 1];
        }
        g->snake.length++;
        g->snake.score = g->snake.score + 10;
        printf("  Ate food! Score: %d\n", g->snake.score);
        /* new food position */
        g->food.x = (g->food.x + 3) % MAX_SIZE;
        g->food.y = (g->food.y + 5) % MAX_SIZE;
    }
    g->snake.body[0].x = new_x;
    g->snake.body[0].y = new_y;
}

void game_set_dir(struct Game *g, int dir) {
    int opposite = (g->snake.direction + 2) % 4;
    if (dir != opposite) {
        g->snake.direction = dir;
    }
}

void game_print_state(struct Game *g) {
    printf("Tick %d: head=", g->ticks);
    point_print(&g->snake.body[0]);
    printf(" dir=%s len=%d score=%d food=", dir_name(g->snake.direction),
           g->snake.length, g->snake.score);
    point_print(&g->food);
    printf(" alive=%d\n", g->snake.alive);
}

int compute_distance(struct Point *a, struct Point *b) {
    return abs_val(a->x - b->x) + abs_val(a->y - b->y);
}

int main(void) {
    struct Game game;
    game_init(&game);
    printf("Snake Game Simulation\n");
    game_print_state(&game);

    UpdateFn update = game_update;

    printf("\nMoving right to food:\n");
    update(&game);
    game_print_state(&game);
    update(&game);
    game_print_state(&game);

    printf("\nTurn down:\n");
    game_set_dir(&game, DIR_DOWN);
    update(&game);
    game_print_state(&game);

    printf("\nTurn left:\n");
    game_set_dir(&game, DIR_LEFT);
    update(&game);
    game_print_state(&game);

    printf("\nContinue:\n");
    update(&game);
    game_print_state(&game);
    update(&game);
    game_print_state(&game);

    printf("\nSnake body: ");
    int i;
    for (i = 0; i < game.snake.length; i++) {
        point_print(&game.snake.body[i]);
        printf(" ");
    }
    printf("\n");

    printf("Distance to food: %d\n",
           compute_distance(&game.snake.body[0], &game.food));

    printf("Final score: %d in %d ticks\n", game.snake.score, game.ticks);

    return 0;
}
