int printf(const char *fmt, ...);
// EXPECT: enum:R=  0,G=1,B=2,Y=3,W=4|bits: 137,t3=1,t2=0,clr=129,tog=161,cnt=3,rot=16|swap:99,42,inc=100,add=50|psum=165|pt:d2=25|rect:area=100,100,contains=1,0,overlap=1,0|game:ent=4,alive=4,health=360,moved= 15,25,aftdmg:alive=3,score=10,data0=  148,data1=220,type0=  RED,type1=BLU,aftptr:sc=110,lvl= 2,h0= 150|flood:  9|hanoi: 1,7,31,1023|sm=803|dir:NE=1,0,trn=1,opp=2|walk: 1,1|math:  abs=42,max=42,min=17,clam=30|digits:1,1,2,4,5|find=5,8,-1|global: 4,flags=17|nested=  40|final_score=134|end

/* THE FINAL BOSS: Combines ALL compiler features */
/* structs with arrays, nested structs, pointers, recursion, enums, */
/* complex control flow, bitwise ops, multiple functions */

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE
};

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct Point {
    int x;
    int y;
};

struct Size {
    int width;
    int height;
};

struct Rect {
    struct Point origin;
    struct Size size;
};

struct Entity {
    int id;
    int type; /* enum Color used as type */
    struct Point pos;
    int health;
    int alive;
    int data[8]; /* array inside struct */
};

struct GameState {
    struct Entity entities[8];
    int entity_count;
    int score;
    int level;
    int turn;
};

/* Global state */
struct GameState game;
int global_counter;
int global_flags;

/* Function declarations */
int abs_val(int x);
int max_val(int a, int b);
int min_val(int a, int b);
int clamp(int val, int lo, int hi);

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

/* Bitwise operations */
int set_bit(int val, int bit) {
    return val | (1 << bit);
}

int clear_bit(int val, int bit) {
    return val & ~(1 << bit);
}

int test_bit(int val, int bit) {
    return (val >> bit) & 1;
}

int toggle_bit(int val, int bit) {
    return val ^ (1 << bit);
}

int count_bits(int val) {
    int cnt;
    cnt = 0;
    while (val > 0) {
        cnt = cnt + (val & 1);
        val = val >> 1;
    }
    return cnt;
}

int rotate_left(int val, int n) {
    int i;
    int bit;
    i = 0;
    while (i < n) {
        bit = (val >> 15) & 1;
        val = ((val << 1) & 0xFFFF) | bit;
        i = i + 1;
    }
    return val;
}

/* Pointer manipulation */
void swap_ptr(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void increment_ptr(int *p) {
    *p = *p + 1;
}

void add_to_ptr(int *p, int val) {
    *p = *p + val;
}

int sum_through_ptr(int *arr, int size) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < size) {
        s = s + *(arr + i);
        i = i + 1;
    }
    return s;
}

/* Struct functions */
struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

int point_dist_sq(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return dx * dx + dy * dy;
}

struct Rect make_rect(int x, int y, int w, int h) {
    struct Rect r;
    r.origin.x = x;
    r.origin.y = y;
    r.size.width = w;
    r.size.height = h;
    return r;
}

int rect_area(struct Rect r) {
    return r.size.width * r.size.height;
}

int rect_contains(struct Rect r, struct Point p) {
    if (p.x < r.origin.x) return 0;
    if (p.y < r.origin.y) return 0;
    if (p.x >= r.origin.x + r.size.width) return 0;
    if (p.y >= r.origin.y + r.size.height) return 0;
    return 1;
}

int rects_overlap(struct Rect a, struct Rect b) {
    if (a.origin.x >= b.origin.x + b.size.width) return 0;
    if (b.origin.x >= a.origin.x + a.size.width) return 0;
    if (a.origin.y >= b.origin.y + b.size.height) return 0;
    if (b.origin.y >= a.origin.y + a.size.height) return 0;
    return 1;
}

/* Game entity functions */
void init_game(void) {
    int i;
    int j;
    game.entity_count = 0;
    game.score = 0;
    game.level = 1;
    game.turn = 0;
    i = 0;
    while (i < 8) {
        game.entities[i].id = -1;
        game.entities[i].type = RED;
        game.entities[i].pos.x = 0;
        game.entities[i].pos.y = 0;
        game.entities[i].health = 0;
        game.entities[i].alive = 0;
        j = 0;
        while (j < 8) {
            game.entities[i].data[j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    global_counter = 0;
    global_flags = 0;
}

int spawn_entity(int type, int x, int y, int health) {
    int idx;
    idx = game.entity_count;
    if (idx >= 8) return -1;
    game.entities[idx].id = idx + 100;
    game.entities[idx].type = type;
    game.entities[idx].pos.x = x;
    game.entities[idx].pos.y = y;
    game.entities[idx].health = health;
    game.entities[idx].alive = 1;
    game.entity_count = game.entity_count + 1;
    global_counter = global_counter + 1;
    return idx;
}

void move_entity(int idx, int dx, int dy) {
    if (!game.entities[idx].alive) return;
    game.entities[idx].pos.x = game.entities[idx].pos.x + dx;
    game.entities[idx].pos.y = game.entities[idx].pos.y + dy;
}

void damage_entity(int idx, int amount) {
    if (!game.entities[idx].alive) return;
    game.entities[idx].health = game.entities[idx].health - amount;
    if (game.entities[idx].health <= 0) {
        game.entities[idx].health = 0;
        game.entities[idx].alive = 0;
        game.score = game.score + 10;
    }
}

int count_alive(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < game.entity_count) {
        if (game.entities[i].alive) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int total_health(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < game.entity_count) {
        if (game.entities[i].alive) s = s + game.entities[i].health;
        i = i + 1;
    }
    return s;
}

/* Recursion: flood fill count on a grid */
int grid[64]; /* 8x8 grid */
int visited[64];

void init_grid(void) {
    int i;
    i = 0;
    while (i < 64) {
        grid[i] = 0;
        visited[i] = 0;
        i = i + 1;
    }
}

int flood_fill_count(int x, int y, int target) {
    int idx;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;
    idx = y * 8 + x;
    if (visited[idx]) return 0;
    if (grid[idx] != target) return 0;
    visited[idx] = 1;
    return 1 + flood_fill_count(x + 1, y, target)
             + flood_fill_count(x - 1, y, target)
             + flood_fill_count(x, y + 1, target)
             + flood_fill_count(x, y - 1, target);
}

/* Recursive tower of hanoi move count */
int hanoi_moves(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return 2 * hanoi_moves(n - 1) + 1;
}

/* Complex control flow: state machine */
int run_state_machine(int *input_seq, int len) {
    int state;
    int i;
    int val;
    int transitions;
    state = 0;
    transitions = 0;
    i = 0;
    while (i < len) {
        val = input_seq[i];
        switch (state) {
            case 0:
                if (val == 1) { state = 1; transitions = transitions + 1; }
                else if (val == 2) { state = 2; transitions = transitions + 1; }
                break;
            case 1:
                if (val == 0) { state = 0; transitions = transitions + 1; }
                else if (val == 3) { state = 3; transitions = transitions + 1; }
                break;
            case 2:
                if (val == 1) { state = 1; transitions = transitions + 1; }
                else if (val == 0) { state = 0; transitions = transitions + 1; }
                break;
            case 3:
                if (val == 0) { state = 0; transitions = transitions + 1; }
                break;
        }
        i = i + 1;
    }
    return transitions * 100 + state;
}

/* Direction functions with enum */
int dx_for_dir(int dir) {
    if (dir == NORTH) return 0;
    if (dir == EAST) return 1;
    if (dir == SOUTH) return 0;
    if (dir == WEST) return -1;
    return 0;
}

int dy_for_dir(int dir) {
    if (dir == NORTH) return -1;
    if (dir == EAST) return 0;
    if (dir == SOUTH) return 1;
    if (dir == WEST) return 0;
    return 0;
}

int turn_right(int dir) {
    return (dir + 1) % 4;
}

int turn_left(int dir) {
    return (dir + 3) % 4;
}

int opposite(int dir) {
    return (dir + 2) % 4;
}

/* Walk a path: given directions, compute final position */
void walk_path(int *dirs, int len, int *out_x, int *out_y) {
    int i;
    int x;
    int y;
    x = 0;
    y = 0;
    i = 0;
    while (i < len) {
        x = x + dx_for_dir(dirs[i]);
        y = y + dy_for_dir(dirs[i]);
        i = i + 1;
    }
    *out_x = x;
    *out_y = y;
}

/* Color name printer */
void print_color(int c) {
    switch (c) {
        case RED: printf("%c%c%c", 'R', 'E', 'D'); break;
        case GREEN: printf("%c%c%c", 'G', 'R', 'N'); break;
        case BLUE: printf("%c%c%c", 'B', 'L', 'U'); break;
        case YELLOW: printf("%c%c%c", 'Y', 'L', 'W'); break;
        case WHITE: printf("%c%c%c", 'W', 'H', 'T'); break;
    }
}

/* Array inside struct operations */
void fill_entity_data(int idx, int start) {
    int i;
    i = 0;
    while (i < 8) {
        game.entities[idx].data[i] = start + i * i;
        i = i + 1;
    }
}

int sum_entity_data(int idx) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < 8) {
        s = s + game.entities[idx].data[i];
        i = i + 1;
    }
    return s;
}

/* Do-while loop test */
int count_digits(int n) {
    int cnt;
    if (n < 0) n = 0 - n;
    if (n == 0) return 1;
    cnt = 0;
    do {
        cnt = cnt + 1;
        n = n / 10;
    } while (n > 0);
    return cnt;
}

/* For loop test (using while) with break/continue */
int find_first_above(int *arr, int size, int threshold) {
    int i;
    i = 0;
    while (i < size) {
        if (arr[i] > threshold) return i;
        i = i + 1;
    }
    return -1;
}

/* Complex nested struct access via pointer */
void modify_game_ptr(struct GameState *g) {
    g->score = g->score + 100;
    g->level = g->level + 1;
    g->entities[0].health = g->entities[0].health + 50;
}

int main(void) {
    int i;
    int a;
    int b;
    int v;
    int x;
    int y;
    struct Point p1;
    struct Point p2;
    struct Rect r1;
    struct Rect r2;
    int sm_input[10];
    int dirs[8];
    int arr[10];

    /* ===== Enum tests ===== */
    printf("%c%c%c%c%c%c%c%c%c", 'e', 'n', 'u', 'm', ':', 'R', '=', ' ', ' ');
    printf("%d", RED);
    printf("%c%c%c", ',', 'G', '=');
    printf("%d", GREEN);
    printf("%c%c%c", ',', 'B', '=');
    printf("%d", BLUE);
    printf("%c%c%c", ',', 'Y', '=');
    printf("%d", YELLOW);
    printf("%c%c%c", ',', 'W', '=');
    printf("%d", WHITE);

    /* ===== Bitwise ops ===== */
    v = 0;
    v = set_bit(v, 0);
    v = set_bit(v, 3);
    v = set_bit(v, 7);
    printf("%c%c%c%c%c%c%c", '|', 'b', 'i', 't', 's', ':', ' ');
    printf("%d", v);
    printf("%c%c%c", ',', 't', '3');
    printf("%c", '=');
    printf("%d", test_bit(v, 3));
    printf("%c%c%c", ',', 't', '2');
    printf("%c", '=');
    printf("%d", test_bit(v, 2));
    v = clear_bit(v, 3);
    printf("%c%c%c%c%c", ',', 'c', 'l', 'r', '=');
    printf("%d", v);
    v = toggle_bit(v, 5);
    printf("%c%c%c%c%c", ',', 't', 'o', 'g', '=');
    printf("%d", v);
    printf("%c%c%c%c%c", ',', 'c', 'n', 't', '=');
    printf("%d", count_bits(v));
    printf("%c%c%c%c%c", ',', 'r', 'o', 't', '=');
    printf("%d", rotate_left(0x0001, 4));

    /* ===== Pointer manipulation ===== */
    a = 42;
    b = 99;
    swap_ptr(&a, &b);
    printf("%c%c%c%c%c%c", '|', 's', 'w', 'a', 'p', ':');
    printf("%d", a);
    printf("%c", ',');
    printf("%d", b);
    increment_ptr(&a);
    printf("%c%c%c%c%c", ',', 'i', 'n', 'c', '=');
    printf("%d", a);
    add_to_ptr(&b, 8);
    printf("%c%c%c%c%c", ',', 'a', 'd', 'd', '=');
    printf("%d", b);

    /* Array through pointer */
    i = 0;
    while (i < 10) {
        arr[i] = (i + 1) * 3;
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", '|', 'p', 's', 'u', 'm', '=');
    printf("%d", sum_through_ptr(arr, 10));

    /* ===== Struct Point and Rect ===== */
    p1 = make_point(3, 4);
    p2 = make_point(6, 8);
    printf("%c%c%c%c%c%c%c", '|', 'p', 't', ':', 'd', '2', '=');
    printf("%d", point_dist_sq(p1, p2));

    r1 = make_rect(0, 0, 10, 10);
    r2 = make_rect(5, 5, 10, 10);
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'r', 'e', 'c', 't', ':', 'a', 'r', 'e', 'a');
    printf("%c", '=');
    printf("%d", rect_area(r1));
    printf("%c", ',');
    printf("%d", rect_area(r2));

    p1 = make_point(5, 5);
    printf("%c%c%c%c%c%c%c%c%c%c", ',', 'c', 'o', 'n', 't', 'a', 'i', 'n', 's', '=');
    printf("%d", rect_contains(r1, p1));

    p1 = make_point(15, 15);
    printf("%c", ',');
    printf("%d", rect_contains(r1, p1));

    printf("%c%c%c%c%c%c%c%c%c", ',', 'o', 'v', 'e', 'r', 'l', 'a', 'p', '=');
    printf("%d", rects_overlap(r1, r2));

    r2 = make_rect(20, 20, 5, 5);
    printf("%c", ',');
    printf("%d", rects_overlap(r1, r2));

    /* ===== Game entity system ===== */
    init_game();
    spawn_entity(RED, 10, 20, 100);
    spawn_entity(BLUE, 30, 40, 80);
    spawn_entity(GREEN, 50, 60, 120);
    spawn_entity(YELLOW, 70, 80, 60);

    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'g', 'a', 'm', 'e', ':', 'e', 'n', 't', '=');
    printf("%d", game.entity_count);
    printf("%c%c%c%c%c%c%c", ',', 'a', 'l', 'i', 'v', 'e', '=');
    printf("%d", count_alive());
    printf("%c%c%c%c%c%c%c", ',', 'h', 'e', 'a', 'l', 't', 'h');
    printf("%c", '=');
    printf("%d", total_health());

    /* Move and damage */
    move_entity(0, 5, 5);
    printf("%c%c%c%c%c%c%c%c", ',', 'm', 'o', 'v', 'e', 'd', '=', ' ');
    printf("%d", game.entities[0].pos.x);
    printf("%c", ',');
    printf("%d", game.entities[0].pos.y);

    damage_entity(3, 70); /* kills entity 3 */
    printf("%c%c%c%c%c%c%c", ',', 'a', 'f', 't', 'd', 'm', 'g');
    printf("%c%c%c%c%c%c%c", ':', 'a', 'l', 'i', 'v', 'e', '=');
    printf("%d", count_alive());
    printf("%c%c%c%c%c%c", ',', 's', 'c', 'o', 'r', 'e');
    printf("%c", '=');
    printf("%d", game.score);

    /* Entity data arrays */
    fill_entity_data(0, 1);
    fill_entity_data(1, 10);
    printf("%c%c%c%c%c%c%c%c%c", ',', 'd', 'a', 't', 'a', '0', '=', ' ', ' ');
    printf("%d", sum_entity_data(0));
    printf("%c%c%c%c%c%c%c", ',', 'd', 'a', 't', 'a', '1', '=');
    printf("%d", sum_entity_data(1));

    /* Entity type colors */
    printf("%c%c%c%c%c%c%c%c%c", ',', 't', 'y', 'p', 'e', '0', '=', ' ', ' ');
    print_color(game.entities[0].type);
    printf("%c%c%c%c%c%c", ',', 't', 'y', 'p', 'e', '1');
    printf("%c", '=');
    print_color(game.entities[1].type);

    /* Modify through pointer */
    modify_game_ptr(&game);
    printf("%c%c%c%c%c%c%c%c%c%c%c", ',', 'a', 'f', 't', 'p', 't', 'r', ':', 's', 'c', '=');
    printf("%d", game.score);
    printf("%c%c%c%c%c%c", ',', 'l', 'v', 'l', '=', ' ');
    printf("%d", game.level);
    printf("%c%c%c%c%c", ',', 'h', '0', '=', ' ');
    printf("%d", game.entities[0].health);

    /* ===== Flood fill ===== */
    init_grid();
    /* Create a 3x3 block of 1s at (2,2) */
    grid[2 * 8 + 2] = 1; grid[2 * 8 + 3] = 1; grid[2 * 8 + 4] = 1;
    grid[3 * 8 + 2] = 1; grid[3 * 8 + 3] = 1; grid[3 * 8 + 4] = 1;
    grid[4 * 8 + 2] = 1; grid[4 * 8 + 3] = 1; grid[4 * 8 + 4] = 1;
    printf("%c%c%c%c%c%c%c%c%c", '|', 'f', 'l', 'o', 'o', 'd', ':', ' ', ' ');
    printf("%d", flood_fill_count(3, 3, 1));

    /* ===== Hanoi ===== */
    printf("%c%c%c%c%c%c%c%c", '|', 'h', 'a', 'n', 'o', 'i', ':', ' ');
    printf("%d", hanoi_moves(1));
    printf("%c", ',');
    printf("%d", hanoi_moves(3));
    printf("%c", ',');
    printf("%d", hanoi_moves(5));
    printf("%c", ',');
    printf("%d", hanoi_moves(10));

    /* ===== State machine ===== */
    sm_input[0] = 1; sm_input[1] = 3; sm_input[2] = 0; sm_input[3] = 2;
    sm_input[4] = 1; sm_input[5] = 0; sm_input[6] = 1; sm_input[7] = 3;
    printf("%c%c%c%c", '|', 's', 'm', '=');
    printf("%d", run_state_machine(sm_input, 8));

    /* ===== Directions ===== */
    printf("%c%c%c%c%c", '|', 'd', 'i', 'r', ':');
    printf("%c%c", 'N', 'E');
    printf("%c", '=');
    printf("%d", dx_for_dir(EAST));
    printf("%c", ',');
    printf("%d", dy_for_dir(EAST));
    printf("%c%c%c%c%c", ',', 't', 'r', 'n', '=');
    printf("%d", turn_right(NORTH));
    printf("%c%c%c%c%c", ',', 'o', 'p', 'p', '=');
    printf("%d", opposite(NORTH));

    /* Walk path */
    dirs[0] = EAST; dirs[1] = EAST; dirs[2] = SOUTH; dirs[3] = SOUTH;
    dirs[4] = WEST; dirs[5] = NORTH;
    walk_path(dirs, 6, &x, &y);
    printf("%c%c%c%c%c%c%c", '|', 'w', 'a', 'l', 'k', ':', ' ');
    printf("%d", x);
    printf("%c", ',');
    printf("%d", y);

    /* ===== Math helpers ===== */
    printf("%c%c%c%c%c%c%c%c", '|', 'm', 'a', 't', 'h', ':', ' ', ' ');
    printf("%c%c%c%c", 'a', 'b', 's', '=');
    printf("%d", abs_val(-42));
    printf("%c%c%c%c", ',', 'm', 'a', 'x');
    printf("%c", '=');
    printf("%d", max_val(17, 42));
    printf("%c%c%c%c", ',', 'm', 'i', 'n');
    printf("%c", '=');
    printf("%d", min_val(17, 42));
    printf("%c%c%c%c%c", ',', 'c', 'l', 'a', 'm');
    printf("%c", '=');
    printf("%d", clamp(50, 0, 30));

    /* ===== Do-while / digit count ===== */
    printf("%c%c%c%c%c%c%c%c", '|', 'd', 'i', 'g', 'i', 't', 's', ':');
    printf("%d", count_digits(0));
    printf("%c", ',');
    printf("%d", count_digits(5));
    printf("%c", ',');
    printf("%d", count_digits(99));
    printf("%c", ',');
    printf("%d", count_digits(1000));
    printf("%c", ',');
    printf("%d", count_digits(-12345));

    /* ===== Find first above ===== */
    i = 0;
    while (i < 10) {
        arr[i] = i * i;
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", '|', 'f', 'i', 'n', 'd', '=');
    printf("%d", find_first_above(arr, 10, 20));
    printf("%c", ',');
    printf("%d", find_first_above(arr, 10, 50));
    printf("%c", ',');
    printf("%d", find_first_above(arr, 10, 100));

    /* ===== Global state ===== */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'g', 'l', 'o', 'b', 'a', 'l', ':', ' ');
    printf("%d", global_counter);
    global_flags = set_bit(global_flags, 0);
    global_flags = set_bit(global_flags, 4);
    printf("%c%c%c%c%c%c%c", ',', 'f', 'l', 'a', 'g', 's', '=');
    printf("%d", global_flags);

    /* ===== Complex nested for */
    {
        int total;
        int r;
        int c;
        total = 0;
        r = 0;
        while (r < 5) {
            c = 0;
            while (c < 5) {
                if (r == c) {
                    total = total + r * c;
                } else if (r > c) {
                    total = total + 1;
                }
                c = c + 1;
            }
            r = r + 1;
        }
        printf("%c%c%c%c%c%c%c%c%c%c", '|', 'n', 'e', 's', 't', 'e', 'd', '=', ' ', ' ');
        printf("%d", total);
    }

    /* ===== Final combined score ===== */
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'f', 'i', 'n', 'a', 'l', '_', 's', 'c', 'o', 'r', 'e', '=');
    printf("%d", game.score + game.level * 10 + global_counter);

    printf("%c%c%c%c", '|', 'e', 'n', 'd');
    printf("%c", '\n');

    return 0;
}
