int printf(const char *fmt, ...);

// Line Follower Simulation
// Simulates a robot following a line on a grid using sensor array

int track[20][20];

void track_init() {
    int i;
    int j;
    for (i = 0; i < 20; i = i + 1) {
        for (j = 0; j < 20; j = j + 1) {
            track[i][j] = 0;
        }
    }
}

void track_draw_line(int x1, int y1, int x2, int y2) {
    int dx;
    int dy;
    int steps;
    int i;
    int cx;
    int cy;

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    steps = dx;
    if (dy > steps) steps = dy;
    if (steps == 0) {
        track[x1][y1] = 1;
        return;
    }

    for (i = 0; i <= steps; i = i + 1) {
        cx = x1 + ((x2 - x1) * i) / steps;
        cy = y1 + ((y2 - y1) * i) / steps;
        if (cx >= 0 && cx < 20 && cy >= 0 && cy < 20) {
            track[cx][cy] = 1;
        }
    }
}

struct LineBot {
    int x;           // position (x100)
    int y;           // position (x100)
    int heading;     // 0-359 degrees
    int speed;       // units per tick (x100)
    int sensors[5];  // 5 ground sensors
    int error;       // line position error
    int total_dist;
    int on_line;
};

void bot_init(struct LineBot *b, int x, int y, int heading) {
    int i;
    b->x = x * 100;
    b->y = y * 100;
    b->heading = heading;
    b->speed = 50;
    b->error = 0;
    b->total_dist = 0;
    b->on_line = 0;
    for (i = 0; i < 5; i = i + 1) {
        b->sensors[i] = 0;
    }
}

// Integer sine approximation (x1000), angle in degrees
int isin(int deg) {
    int a;
    a = deg % 360;
    if (a < 0) a = a + 360;
    if (a == 0) return 0;
    if (a == 90) return 1000;
    if (a == 180) return 0;
    if (a == 270) return -1000;
    if (a < 90) return (a * 1000) / 90;
    if (a < 180) return ((180 - a) * 1000) / 90;
    if (a < 270) return -((a - 180) * 1000) / 90;
    return -((360 - a) * 1000) / 90;
}

int icos(int deg) {
    return isin(deg + 90);
}

void bot_read_sensors(struct LineBot *b) {
    int i;
    int sx;
    int sy;
    int offsets[5];
    int gx;
    int gy;

    offsets[0] = -2;
    offsets[1] = -1;
    offsets[2] = 0;
    offsets[3] = 1;
    offsets[4] = 2;

    b->on_line = 0;
    for (i = 0; i < 5; i = i + 1) {
        gx = b->x / 100 + (offsets[i] * icos(b->heading + 90)) / 1000;
        gy = b->y / 100 + (offsets[i] * isin(b->heading + 90)) / 1000;
        if (gx >= 0 && gx < 20 && gy >= 0 && gy < 20) {
            b->sensors[i] = track[gx][gy];
        } else {
            b->sensors[i] = 0;
        }
        if (b->sensors[i]) b->on_line = 1;
    }
}

int bot_compute_error(struct LineBot *b) {
    int weighted;
    int total;
    int i;
    int weights[5];

    weights[0] = -2;
    weights[1] = -1;
    weights[2] = 0;
    weights[3] = 1;
    weights[4] = 2;

    weighted = 0;
    total = 0;
    for (i = 0; i < 5; i = i + 1) {
        if (b->sensors[i]) {
            weighted = weighted + weights[i];
            total = total + 1;
        }
    }
    if (total == 0) return b->error;  // keep last error
    return (weighted * 100) / total;
}

void bot_move(struct LineBot *b) {
    int steer;

    b->error = bot_compute_error(b);
    steer = b->error / 5;

    b->heading = b->heading + steer;
    if (b->heading < 0) b->heading = b->heading + 360;
    if (b->heading >= 360) b->heading = b->heading - 360;

    b->x = b->x + (b->speed * icos(b->heading)) / 1000;
    b->y = b->y + (b->speed * isin(b->heading)) / 1000;
    b->total_dist = b->total_dist + b->speed;
}

void bot_step(struct LineBot *b) {
    bot_read_sensors(b);
    bot_move(b);
}

int main() {
    struct LineBot bot;
    int i;
    int s;
    int c;

    printf("=== Line Follower Test ===\n");
    // EXPECT: === Line Follower Test ===

    // Test 1: trig functions
    printf("sin(0)=%d, sin(90)=%d, sin(180)=%d, sin(270)=%d\n",
           isin(0), isin(90), isin(180), isin(270));
    // EXPECT: sin(0)=0, sin(90)=1000, sin(180)=0, sin(270)=-1000

    printf("cos(0)=%d, cos(90)=%d, cos(180)=%d\n", icos(0), icos(90), icos(180));
    // EXPECT: cos(0)=1000, cos(90)=0, cos(180)=-1000

    printf("sin(45)=%d, cos(45)=%d\n", isin(45), icos(45));
    // EXPECT: sin(45)=500, cos(45)=500

    // Test 2: Track creation
    track_init();
    track_draw_line(5, 5, 5, 15);
    s = 0;
    for (i = 0; i < 20; i = i + 1) {
        s = s + track[5][i];
    }
    printf("Line cells: %d\n", s);
    // EXPECT: Line cells: 11

    // Test 3: Bot init
    bot_init(&bot, 5, 5, 0);
    printf("Bot init: x=%d, y=%d, heading=%d\n", bot.x / 100, bot.y / 100, bot.heading);
    // EXPECT: Bot init: x=5, y=5, heading=0

    // Test 4: Sensor reading on line
    bot_read_sensors(&bot);
    printf("On line: %d, sensors: %d%d%d%d%d\n",
           bot.on_line, bot.sensors[0], bot.sensors[1], bot.sensors[2], bot.sensors[3], bot.sensors[4]);
    // EXPECT: On line: 1, sensors: 00111

    // Test 5: Straight line tracking
    track_init();
    track_draw_line(0, 10, 19, 10);
    bot_init(&bot, 0, 10, 0);
    for (i = 0; i < 10; i = i + 1) {
        bot_step(&bot);
    }
    printf("After 10 steps: x=%d, y=%d, dist=%d\n", bot.x / 100, bot.y / 100, bot.total_dist);
    // EXPECT: After 10 steps: x=5, y=10, dist=500

    // Test 6: Error computation
    bot_init(&bot, 5, 5, 0);
    bot.sensors[0] = 0;
    bot.sensors[1] = 0;
    bot.sensors[2] = 1;
    bot.sensors[3] = 0;
    bot.sensors[4] = 0;
    printf("Center error: %d\n", bot_compute_error(&bot));
    // EXPECT: Center error: 0

    bot.sensors[0] = 0;
    bot.sensors[1] = 0;
    bot.sensors[2] = 0;
    bot.sensors[3] = 1;
    bot.sensors[4] = 1;
    printf("Right error: %d\n", bot_compute_error(&bot));
    // EXPECT: Right error: 150

    bot.sensors[0] = 1;
    bot.sensors[1] = 1;
    bot.sensors[2] = 0;
    bot.sensors[3] = 0;
    bot.sensors[4] = 0;
    printf("Left error: %d\n", bot_compute_error(&bot));
    // EXPECT: Left error: -150

    printf("Line follower done\n");
    // EXPECT: Line follower done

    return 0;
}
