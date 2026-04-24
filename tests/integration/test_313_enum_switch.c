int printf(const char *fmt, ...);
// EXPECT: After NORTH 5: (0, 5)\nAfter EAST 3: (3, 5)\nAfter SOUTH 2: (3, 3)\nAfter WEST 1: (2, 3)\n[DEBUG] code=100\n[INFO] code=200\n[WARN] code=300\n[ERROR] code=400\n[FATAL] code=500\nseverity DEBUG=1\nseverity ERROR=10\nseverity FATAL=100\nopposite NORTH=1\nopposite EAST=3
// Test: enum in switch statement

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARN = 2,
    LOG_ERROR = 3,
    LOG_FATAL = 4
};

struct Position {
    int x;
    int y;
};

void move(struct Position *pos, enum Direction dir, int steps) {
    switch (dir) {
        case NORTH: pos->y = pos->y + steps; break;
        case SOUTH: pos->y = pos->y - steps; break;
        case EAST:  pos->x = pos->x + steps; break;
        case WEST:  pos->x = pos->x - steps; break;
    }
}

void log_message(enum LogLevel level, int code) {
    switch (level) {
        case LOG_DEBUG: printf("[DEBUG] code=%d\n", code); break;
        case LOG_INFO:  printf("[INFO] code=%d\n", code); break;
        case LOG_WARN:  printf("[WARN] code=%d\n", code); break;
        case LOG_ERROR: printf("[ERROR] code=%d\n", code); break;
        case LOG_FATAL: printf("[FATAL] code=%d\n", code); break;
        default: printf("[UNKNOWN] code=%d\n", code); break;
    }
}

int severity_score(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return 1;
        case LOG_INFO:  return 2;
        case LOG_WARN:  return 5;
        case LOG_ERROR: return 10;
        case LOG_FATAL: return 100;
        default: return 0;
    }
}

int opposite_dir(enum Direction d) {
    switch (d) {
        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        default: return -1;
    }
}

int main(void) {
    struct Position pos;
    pos.x = 0;
    pos.y = 0;

    move(&pos, NORTH, 5);
    printf("After NORTH 5: (%d, %d)\n", pos.x, pos.y);
    move(&pos, EAST, 3);
    printf("After EAST 3: (%d, %d)\n", pos.x, pos.y);
    move(&pos, SOUTH, 2);
    printf("After SOUTH 2: (%d, %d)\n", pos.x, pos.y);
    move(&pos, WEST, 1);
    printf("After WEST 1: (%d, %d)\n", pos.x, pos.y);

    log_message(LOG_DEBUG, 100);
    log_message(LOG_INFO, 200);
    log_message(LOG_WARN, 300);
    log_message(LOG_ERROR, 400);
    log_message(LOG_FATAL, 500);

    printf("severity DEBUG=%d\n", severity_score(LOG_DEBUG));
    printf("severity ERROR=%d\n", severity_score(LOG_ERROR));
    printf("severity FATAL=%d\n", severity_score(LOG_FATAL));

    printf("opposite NORTH=%d\n", opposite_dir(NORTH));
    printf("opposite EAST=%d\n", opposite_dir(EAST));

    return 0;
}
