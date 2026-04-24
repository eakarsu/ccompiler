int printf(const char *fmt, ...);

typedef enum {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_COUNT
} Color;

typedef enum {
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST
} Direction;

typedef enum {
    PRIO_LOW = 1,
    PRIO_MEDIUM = 5,
    PRIO_HIGH = 10,
    PRIO_CRITICAL = 20
} Priority;

typedef enum {
    DAY_MON,
    DAY_TUE,
    DAY_WED,
    DAY_THU,
    DAY_FRI,
    DAY_SAT,
    DAY_SUN
} Day;

int color_to_int(Color c) {
    return c;
}

Color next_color(Color c) {
    int next = c + 1;
    if (next >= COLOR_COUNT) return COLOR_RED;
    return next;
}

Direction opposite(Direction d) {
    if (d == DIR_NORTH) return DIR_SOUTH;
    if (d == DIR_SOUTH) return DIR_NORTH;
    if (d == DIR_EAST) return DIR_WEST;
    return DIR_EAST;
}

Direction turn_right(Direction d) {
    if (d == DIR_NORTH) return DIR_EAST;
    if (d == DIR_EAST) return DIR_SOUTH;
    if (d == DIR_SOUTH) return DIR_WEST;
    return DIR_NORTH;
}

int priority_value(Priority p) {
    return p;
}

int is_urgent(Priority p) {
    return p >= PRIO_HIGH;
}

Priority max_priority(Priority a, Priority b) {
    if (a > b) return a;
    return b;
}

int is_weekend(Day d) {
    return (d == DAY_SAT) || (d == DAY_SUN);
}

int is_workday(Day d) {
    return !is_weekend(d);
}

Day next_day(Day d) {
    if (d == DAY_SUN) return DAY_MON;
    return d + 1;
}

int days_until_weekend(Day d) {
    int count = 0;
    Day cur = d;
    while (!is_weekend(cur)) {
        count = count + 1;
        cur = next_day(cur);
    }
    return count;
}

int count_by_color(Color arr[], int len, Color target) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] == target) count = count + 1;
    }
    return count;
}

int sum_priorities(Priority arr[], int len) {
    int total = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

int main(void) {
    // EXPECT: red: 0
    printf("red: %d\n", color_to_int(COLOR_RED));

    // EXPECT: blue: 2
    printf("blue: %d\n", color_to_int(COLOR_BLUE));

    // EXPECT: color count: 4
    printf("color count: %d\n", COLOR_COUNT);

    // EXPECT: next red: 1
    printf("next red: %d\n", next_color(COLOR_RED));

    // EXPECT: next yellow: 0
    printf("next yellow: %d\n", next_color(COLOR_YELLOW));

    // EXPECT: opp north: 2
    printf("opp north: %d\n", opposite(DIR_NORTH));

    // EXPECT: opp east: 3
    printf("opp east: %d\n", opposite(DIR_EAST));

    // EXPECT: turn right north: 1
    printf("turn right north: %d\n", turn_right(DIR_NORTH));

    // EXPECT: turn right west: 0
    printf("turn right west: %d\n", turn_right(DIR_WEST));

    // EXPECT: prio low: 1
    printf("prio low: %d\n", priority_value(PRIO_LOW));

    // EXPECT: prio critical: 20
    printf("prio critical: %d\n", priority_value(PRIO_CRITICAL));

    // EXPECT: urgent low: 0
    printf("urgent low: %d\n", is_urgent(PRIO_LOW));

    // EXPECT: urgent high: 1
    printf("urgent high: %d\n", is_urgent(PRIO_HIGH));

    // EXPECT: max prio: 20
    printf("max prio: %d\n", max_priority(PRIO_MEDIUM, PRIO_CRITICAL));

    // EXPECT: weekend sat: 1
    printf("weekend sat: %d\n", is_weekend(DAY_SAT));

    // EXPECT: weekend wed: 0
    printf("weekend wed: %d\n", is_weekend(DAY_WED));

    // EXPECT: workday mon: 1
    printf("workday mon: %d\n", is_workday(DAY_MON));

    // EXPECT: next sun: 0
    printf("next sun: %d\n", next_day(DAY_SUN));

    // EXPECT: next fri: 5
    printf("next fri: %d\n", next_day(DAY_FRI));

    // EXPECT: days to weekend from mon: 5
    printf("days to weekend from mon: %d\n", days_until_weekend(DAY_MON));

    // EXPECT: days to weekend from fri: 1
    printf("days to weekend from fri: %d\n", days_until_weekend(DAY_FRI));

    Color colors[6];
    colors[0] = COLOR_RED; colors[1] = COLOR_BLUE; colors[2] = COLOR_RED;
    colors[3] = COLOR_GREEN; colors[4] = COLOR_RED; colors[5] = COLOR_BLUE;
    // EXPECT: count red: 3
    printf("count red: %d\n", count_by_color(colors, 6, COLOR_RED));

    // EXPECT: count blue: 2
    printf("count blue: %d\n", count_by_color(colors, 6, COLOR_BLUE));

    Priority tasks[4];
    tasks[0] = PRIO_LOW; tasks[1] = PRIO_HIGH;
    tasks[2] = PRIO_MEDIUM; tasks[3] = PRIO_CRITICAL;
    // EXPECT: sum prios: 36
    printf("sum prios: %d\n", sum_priorities(tasks, 4));

    Direction d = DIR_NORTH;
    d = turn_right(d);
    d = turn_right(d);
    // EXPECT: 2 right turns: 2
    printf("2 right turns: %d\n", d);

    return 0;
}
