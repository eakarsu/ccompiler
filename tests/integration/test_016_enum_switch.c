int printf(const char *fmt, ...);
// EXPECT: === Basic Enum Values ===\nRED=0\nGREEN=1\nBLUE=2\n=== Custom Enum Values ===\nHTTP_OK=200\nHTTP_NOT_FOUND=404\nHTTP_SERVER_ERR=500\n=== Switch on Enum ===\ncolor is red\ncolor is green\ncolor is blue\n=== Enum Arithmetic ===\nRED+1=1\nGREEN+1=2\nBLUE-1=1\nGREEN*2=2\n=== Day Enum ===\nMON=0\nTUE=1\nWED=2\nTHU=3\nFRI=4\nSAT=5\nSUN=6\n=== Weekday Check ===\nday 0: weekday\nday 1: weekday\nday 2: weekday\nday 3: weekday\nday 4: weekday\nday 5: weekend\nday 6: weekend\n=== Season Switch ===\nSPRING: mild\nSUMMER: hot\nAUTUMN: cool\nWINTER: cold\n=== Nested Switch ===\ncategory=1 item=0: apple\ncategory=1 item=1: banana\ncategory=2 item=0: carrot\ncategory=2 item=1: daikon\ncategory=0 item=0: unknown\n=== Fall-through Behavior ===\ncase 1 hit\ncase 2 hit\ncase 3 hit\n=== Fall-through Counting ===\nfall-through from 0\nfall-through from 1\nfall-through from 2\nend\n=== Switch Default ===\nval=0: zero\nval=5: other\nval=10: ten\nval=99: other\n=== State Machine ===\nstate: IDLE\nevent: START -> state: RUNNING\nevent: PAUSE -> state: PAUSED\nevent: RESUME -> state: RUNNING\nevent: STOP -> state: STOPPED\nevent: RESET -> state: IDLE\n=== State Machine Extended ===\ninit: IDLE\nSTART -> RUNNING\nPAUSE -> PAUSED\nRESUME -> RUNNING\nPAUSE -> PAUSED\nRESUME -> RUNNING\nSTOP -> STOPPED\nRESET -> IDLE\nSTART -> RUNNING\nSTOP -> STOPPED\nfinal: STOPPED\n=== Enum Comparison ===\nRED < GREEN: 1\nBLUE > RED: 1\nGREEN == GREEN: 1\nRED != BLUE: 1\n=== Enum in Array Index ===\nscores[RED]=85\nscores[GREEN]=92\nscores[BLUE]=78\n=== Direction Enum ===\nNORTH=0\nEAST=1\nSOUTH=2\nWEST=3\n=== Turn Right ===\nfacing NORTH, turn right -> EAST\nfacing EAST, turn right -> SOUTH\nfacing SOUTH, turn right -> WEST\nfacing WEST, turn right -> NORTH\n=== Turn Left ===\nfacing NORTH, turn left -> WEST\nfacing WEST, turn left -> SOUTH\nfacing SOUTH, turn left -> EAST\nfacing EAST, turn left -> NORTH\n=== Opposite Direction ===\nopposite of NORTH is SOUTH\nopposite of EAST is WEST\nopposite of SOUTH is NORTH\nopposite of WEST is EAST\n=== Movement Simulation ===\nstart: x=0 y=0 facing=0\nmove NORTH: x=0 y=1\nturn right, move EAST: x=1 y=1\nturn right, move SOUTH: x=1 y=0\nturn right, move WEST: x=0 y=0\nback to start: 1\n=== Priority Enum ===\nLOW=0 MEDIUM=1 HIGH=2 CRITICAL=3\n=== Priority Sort ===\ntask 0: priority=2\ntask 1: priority=0\ntask 2: priority=3\ntask 3: priority=1\nsorted by priority:\ntask 1: priority=0\ntask 3: priority=1\ntask 0: priority=2\ntask 2: priority=3\n=== Enum Bit Patterns ===\nPERM_READ=1\nPERM_WRITE=2\nPERM_EXEC=4\nrw=3\nrwx=7\nrx=5\nhas_read(rw)=1\nhas_write(rw)=1\nhas_exec(rw)=0\nhas_read(rwx)=1\nhas_exec(rx)=1\n=== Switch with Expressions ===\n10/3=3 -> case 3\n7%4=3 -> case 3\n2*2=4 -> case 4\n15%10=5 -> default\n=== Multi-case Equivalent ===\ngrade 5: excellent\ngrade 4: good\ngrade 3: average\ngrade 2: below average\ngrade 1: poor\ngrade 0: fail\n=== All enum/switch tests done ===
// === Custom Enum Values ===
// HTTP_OK=200
// HTTP_NOT_FOUND=404
// HTTP_SERVER_ERR=500
// === Switch on Enum ===
// color is red
// color is green
// color is blue
// === Enum Arithmetic ===
// RED+1=1
// GREEN+1=2
// BLUE-1=1
// GREEN*2=2
// === Day Enum ===
// MON=0
// TUE=1
// WED=2
// THU=3
// FRI=4
// SAT=5
// SUN=6
// === Weekday Check ===
// day 0: weekday
// day 1: weekday
// day 2: weekday
// day 3: weekday
// day 4: weekday
// day 5: weekend
// day 6: weekend
// === Season Switch ===
// SPRING: mild
// SUMMER: hot
// AUTUMN: cool
// WINTER: cold
// === Nested Switch ===
// category=1 item=0: apple
// category=1 item=1: banana
// category=2 item=0: carrot
// category=2 item=1: daikon
// category=0 item=0: unknown
// === Fall-through Behavior ===
// case 1 hit
// case 2 hit
// case 3 hit
// === Fall-through Counting ===
// fall-through from 0
// fall-through from 1
// fall-through from 2
// end
// === Switch Default ===
// val=0: zero
// val=5: other
// val=10: ten
// val=99: other
// === State Machine ===
// state: IDLE
// event: START -> state: RUNNING
// event: PAUSE -> state: PAUSED
// event: RESUME -> state: RUNNING
// event: STOP -> state: STOPPED
// event: RESET -> state: IDLE
// === State Machine Extended ===
// init: IDLE
// START -> RUNNING
// PAUSE -> PAUSED
// RESUME -> RUNNING
// PAUSE -> PAUSED
// RESUME -> RUNNING
// STOP -> STOPPED
// RESET -> IDLE
// START -> RUNNING
// STOP -> STOPPED
// final: STOPPED
// === Enum Comparison ===
// RED < GREEN: 1
// BLUE > RED: 1
// GREEN == GREEN: 1
// RED != BLUE: 1
// === Enum in Array Index ===
// scores[RED]=85
// scores[GREEN]=92
// scores[BLUE]=78
// === Direction Enum ===
// NORTH=0
// EAST=1
// SOUTH=2
// WEST=3
// === Turn Right ===
// facing NORTH, turn right -> EAST
// facing EAST, turn right -> SOUTH
// facing SOUTH, turn right -> WEST
// facing WEST, turn right -> NORTH
// === Turn Left ===
// facing NORTH, turn left -> WEST
// facing WEST, turn left -> SOUTH
// facing SOUTH, turn left -> EAST
// facing EAST, turn left -> NORTH
// === Opposite Direction ===
// opposite of NORTH is SOUTH
// opposite of EAST is WEST
// opposite of SOUTH is NORTH
// opposite of WEST is EAST
// === Movement Simulation ===
// start: x=0 y=0 facing=0
// move NORTH: x=0 y=1
// turn right, move EAST: x=1 y=1
// turn right, move SOUTH: x=1 y=0
// turn right, move WEST: x=0 y=0
// back to start: 1
// === Priority Enum ===
// LOW=0 MEDIUM=1 HIGH=2 CRITICAL=3
// === Priority Sort ===
// task 0: priority=2
// task 1: priority=0
// task 2: priority=3
// task 3: priority=1
// sorted by priority:
// task 1: priority=0
// task 3: priority=1
// task 0: priority=2
// task 2: priority=3
// === Enum Bit Patterns ===
// PERM_READ=1
// PERM_WRITE=2
// PERM_EXEC=4
// rw=3
// rwx=7
// rx=5
// has_read(rw)=1
// has_write(rw)=1
// has_exec(rw)=0
// has_read(rwx)=1
// has_exec(rx)=1
// === Switch with Expressions ===
// 10/3=3 -> case 3
// 7%4=3 -> case 3
// 2*2=4 -> case 4
// 15%10=5 -> default
// === Multi-case Equivalent ===
// grade 5: excellent
// grade 4: good
// grade 3: average
// grade 2: below average
// grade 1: poor
// grade 0: fail
// === All enum/switch tests done ===

enum Color {
    RED,
    GREEN,
    BLUE
};

enum HttpStatus {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERR = 500
};

enum Day {
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN
};

enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

enum State {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED
};

enum Event {
    EVT_START,
    EVT_STOP,
    EVT_PAUSE,
    EVT_RESUME,
    EVT_RESET
};

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum Priority {
    PRI_LOW,
    PRI_MEDIUM,
    PRI_HIGH,
    PRI_CRITICAL
};

enum Permission {
    PERM_READ = 1,
    PERM_WRITE = 2,
    PERM_EXEC = 4
};

void print_color_name(int c) {
    switch (c) {
        case 0:
            printf("color is red\n");
            break;
        case 1:
            printf("color is green\n");
            break;
        case 2:
            printf("color is blue\n");
            break;
    }
}

int is_weekday(int day) {
    switch (day) {
        case 5:
            return 0;
        case 6:
            return 0;
    }
    return 1;
}

void print_season(int s) {
    switch (s) {
        case 0:
            printf("SPRING: mild\n");
            break;
        case 1:
            printf("SUMMER: hot\n");
            break;
        case 2:
            printf("AUTUMN: cool\n");
            break;
        case 3:
            printf("WINTER: cold\n");
            break;
    }
}

void nested_switch(int category, int item) {
    printf("category=%d item=%d: ", category, item);
    switch (category) {
        case 1:
            switch (item) {
                case 0:
                    printf("apple\n");
                    break;
                case 1:
                    printf("banana\n");
                    break;
                default:
                    printf("unknown fruit\n");
                    break;
            }
            break;
        case 2:
            switch (item) {
                case 0:
                    printf("carrot\n");
                    break;
                case 1:
                    printf("daikon\n");
                    break;
                default:
                    printf("unknown veggie\n");
                    break;
            }
            break;
        default:
            printf("unknown\n");
            break;
    }
}

void print_switch_default(int val) {
    printf("val=%d: ", val);
    switch (val) {
        case 0:
            printf("zero\n");
            break;
        case 10:
            printf("ten\n");
            break;
        default:
            printf("other\n");
            break;
    }
}

int state_machine_step(int state, int event) {
    int next;
    next = state;
    switch (state) {
        case 0: // IDLE
            switch (event) {
                case 0: // START
                    next = 1; // RUNNING
                    break;
            }
            break;
        case 1: // RUNNING
            switch (event) {
                case 1: // STOP
                    next = 3; // STOPPED
                    break;
                case 2: // PAUSE
                    next = 2; // PAUSED
                    break;
            }
            break;
        case 2: // PAUSED
            switch (event) {
                case 3: // RESUME
                    next = 1; // RUNNING
                    break;
                case 1: // STOP
                    next = 3; // STOPPED
                    break;
            }
            break;
        case 3: // STOPPED
            switch (event) {
                case 4: // RESET
                    next = 0; // IDLE
                    break;
            }
            break;
    }
    return next;
}

void print_state_name(int s) {
    switch (s) {
        case 0:
            printf("IDLE");
            break;
        case 1:
            printf("RUNNING");
            break;
        case 2:
            printf("PAUSED");
            break;
        case 3:
            printf("STOPPED");
            break;
    }
}

void print_event_name(int e) {
    switch (e) {
        case 0:
            printf("START");
            break;
        case 1:
            printf("STOP");
            break;
        case 2:
            printf("PAUSE");
            break;
        case 3:
            printf("RESUME");
            break;
        case 4:
            printf("RESET");
            break;
    }
}

int turn_right(int dir) {
    return (dir + 1) % 4;
}

int turn_left(int dir) {
    return (dir + 3) % 4;
}

int opposite_dir(int dir) {
    return (dir + 2) % 4;
}

void print_dir_name(int d) {
    switch (d) {
        case 0:
            printf("NORTH");
            break;
        case 1:
            printf("EAST");
            break;
        case 2:
            printf("SOUTH");
            break;
        case 3:
            printf("WEST");
            break;
    }
}

void move_forward(int *x, int *y, int dir) {
    switch (dir) {
        case 0: // NORTH
            *y = *y + 1;
            break;
        case 1: // EAST
            *x = *x + 1;
            break;
        case 2: // SOUTH
            *y = *y - 1;
            break;
        case 3: // WEST
            *x = *x - 1;
            break;
    }
}

void sort_by_priority(int *task_ids, int *priorities, int n) {
    int i;
    int j;
    int tmp;
    i = 0;
    while (i < n - 1) {
        j = 0;
        while (j < n - 1 - i) {
            if (priorities[j] > priorities[j + 1]) {
                tmp = priorities[j];
                priorities[j] = priorities[j + 1];
                priorities[j + 1] = tmp;
                tmp = task_ids[j];
                task_ids[j] = task_ids[j + 1];
                task_ids[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int has_permission(int mask, int perm) {
    if ((mask & perm) != 0) {
        return 1;
    }
    return 0;
}

void print_switch_expr(int val) {
    switch (val) {
        case 0:
            printf("case 0\n");
            break;
        case 1:
            printf("case 1\n");
            break;
        case 2:
            printf("case 2\n");
            break;
        case 3:
            printf("case 3\n");
            break;
        case 4:
            printf("case 4\n");
            break;
        default:
            printf("default\n");
            break;
    }
}

void print_grade(int g) {
    printf("grade %d: ", g);
    switch (g) {
        case 5:
            printf("excellent\n");
            break;
        case 4:
            printf("good\n");
            break;
        case 3:
            printf("average\n");
            break;
        case 2:
            printf("below average\n");
            break;
        case 1:
            printf("poor\n");
            break;
        default:
            printf("fail\n");
            break;
    }
}

int main() {
    int c;
    int d;
    int s;
    int state;
    int next;
    int x;
    int y;
    int dir;
    int scores[3];
    int task_ids[4];
    int priorities[4];
    int rw;
    int rwx;
    int rx;
    int i;
    int val;
    int events[9];

    printf("=== Basic Enum Values ===\n");
    printf("RED=%d\n", RED);
    printf("GREEN=%d\n", GREEN);
    printf("BLUE=%d\n", BLUE);

    printf("=== Custom Enum Values ===\n");
    printf("HTTP_OK=%d\n", HTTP_OK);
    printf("HTTP_NOT_FOUND=%d\n", HTTP_NOT_FOUND);
    printf("HTTP_SERVER_ERR=%d\n", HTTP_SERVER_ERR);

    printf("=== Switch on Enum ===\n");
    print_color_name(RED);
    print_color_name(GREEN);
    print_color_name(BLUE);

    printf("=== Enum Arithmetic ===\n");
    printf("RED+1=%d\n", RED + 1);
    printf("GREEN+1=%d\n", GREEN + 1);
    printf("BLUE-1=%d\n", BLUE - 1);
    printf("GREEN*2=%d\n", GREEN * 2);

    printf("=== Day Enum ===\n");
    printf("MON=%d\n", MON);
    printf("TUE=%d\n", TUE);
    printf("WED=%d\n", WED);
    printf("THU=%d\n", THU);
    printf("FRI=%d\n", FRI);
    printf("SAT=%d\n", SAT);
    printf("SUN=%d\n", SUN);

    printf("=== Weekday Check ===\n");
    d = 0;
    while (d <= 6) {
        printf("day %d: ", d);
        if (is_weekday(d)) {
            printf("weekday\n");
        } else {
            printf("weekend\n");
        }
        d = d + 1;
    }

    printf("=== Season Switch ===\n");
    print_season(SPRING);
    print_season(SUMMER);
    print_season(AUTUMN);
    print_season(WINTER);

    printf("=== Nested Switch ===\n");
    nested_switch(1, 0);
    nested_switch(1, 1);
    nested_switch(2, 0);
    nested_switch(2, 1);
    nested_switch(0, 0);

    printf("=== Fall-through Behavior ===\n");
    c = 1;
    switch (c) {
        case 0:
            printf("case 0 hit\n");
            break;
        case 1:
            printf("case 1 hit\n");
        case 2:
            printf("case 2 hit\n");
        case 3:
            printf("case 3 hit\n");
            break;
        case 4:
            printf("case 4 hit\n");
            break;
    }

    printf("=== Fall-through Counting ===\n");
    c = 0;
    switch (c) {
        case 0:
            printf("fall-through from 0\n");
        case 1:
            printf("fall-through from 1\n");
        case 2:
            printf("fall-through from 2\n");
            printf("end\n");
            break;
        case 3:
            printf("fall-through from 3\n");
            break;
    }

    printf("=== Switch Default ===\n");
    print_switch_default(0);
    print_switch_default(5);
    print_switch_default(10);
    print_switch_default(99);

    printf("=== State Machine ===\n");
    state = STATE_IDLE;
    printf("state: ");
    print_state_name(state);
    printf("\n");

    next = state_machine_step(state, EVT_START);
    printf("event: ");
    print_event_name(EVT_START);
    printf(" -> state: ");
    print_state_name(next);
    printf("\n");
    state = next;

    next = state_machine_step(state, EVT_PAUSE);
    printf("event: ");
    print_event_name(EVT_PAUSE);
    printf(" -> state: ");
    print_state_name(next);
    printf("\n");
    state = next;

    next = state_machine_step(state, EVT_RESUME);
    printf("event: ");
    print_event_name(EVT_RESUME);
    printf(" -> state: ");
    print_state_name(next);
    printf("\n");
    state = next;

    next = state_machine_step(state, EVT_STOP);
    printf("event: ");
    print_event_name(EVT_STOP);
    printf(" -> state: ");
    print_state_name(next);
    printf("\n");
    state = next;

    next = state_machine_step(state, EVT_RESET);
    printf("event: ");
    print_event_name(EVT_RESET);
    printf(" -> state: ");
    print_state_name(next);
    printf("\n");
    state = next;

    printf("=== State Machine Extended ===\n");
    state = STATE_IDLE;
    printf("init: ");
    print_state_name(state);
    printf("\n");

    events[0] = EVT_START;
    events[1] = EVT_PAUSE;
    events[2] = EVT_RESUME;
    events[3] = EVT_PAUSE;
    events[4] = EVT_RESUME;
    events[5] = EVT_STOP;
    events[6] = EVT_RESET;
    events[7] = EVT_START;
    events[8] = EVT_STOP;

    i = 0;
    while (i < 9) {
        next = state_machine_step(state, events[i]);
        print_event_name(events[i]);
        printf(" -> ");
        print_state_name(next);
        printf("\n");
        state = next;
        i = i + 1;
    }
    printf("final: ");
    print_state_name(state);
    printf("\n");

    printf("=== Enum Comparison ===\n");
    printf("RED < GREEN: %d\n", RED < GREEN);
    printf("BLUE > RED: %d\n", BLUE > RED);
    printf("GREEN == GREEN: %d\n", GREEN == GREEN);
    printf("RED != BLUE: %d\n", RED != BLUE);

    printf("=== Enum in Array Index ===\n");
    scores[RED] = 85;
    scores[GREEN] = 92;
    scores[BLUE] = 78;
    printf("scores[RED]=%d\n", scores[RED]);
    printf("scores[GREEN]=%d\n", scores[GREEN]);
    printf("scores[BLUE]=%d\n", scores[BLUE]);

    printf("=== Direction Enum ===\n");
    printf("NORTH=%d\n", NORTH);
    printf("EAST=%d\n", EAST);
    printf("SOUTH=%d\n", SOUTH);
    printf("WEST=%d\n", WEST);

    printf("=== Turn Right ===\n");
    dir = NORTH;
    printf("facing ");
    print_dir_name(dir);
    printf(", turn right -> ");
    dir = turn_right(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn right -> ");
    dir = turn_right(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn right -> ");
    dir = turn_right(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn right -> ");
    dir = turn_right(dir);
    print_dir_name(dir);
    printf("\n");

    printf("=== Turn Left ===\n");
    dir = NORTH;
    printf("facing ");
    print_dir_name(dir);
    printf(", turn left -> ");
    dir = turn_left(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn left -> ");
    dir = turn_left(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn left -> ");
    dir = turn_left(dir);
    print_dir_name(dir);
    printf("\n");

    printf("facing ");
    print_dir_name(dir);
    printf(", turn left -> ");
    dir = turn_left(dir);
    print_dir_name(dir);
    printf("\n");

    printf("=== Opposite Direction ===\n");
    printf("opposite of ");
    print_dir_name(NORTH);
    printf(" is ");
    print_dir_name(opposite_dir(NORTH));
    printf("\n");
    printf("opposite of ");
    print_dir_name(EAST);
    printf(" is ");
    print_dir_name(opposite_dir(EAST));
    printf("\n");
    printf("opposite of ");
    print_dir_name(SOUTH);
    printf(" is ");
    print_dir_name(opposite_dir(SOUTH));
    printf("\n");
    printf("opposite of ");
    print_dir_name(WEST);
    printf(" is ");
    print_dir_name(opposite_dir(WEST));
    printf("\n");

    printf("=== Movement Simulation ===\n");
    x = 0;
    y = 0;
    dir = NORTH;
    printf("start: x=%d y=%d facing=%d\n", x, y, dir);

    move_forward(&x, &y, dir);
    printf("move NORTH: x=%d y=%d\n", x, y);

    dir = turn_right(dir);
    move_forward(&x, &y, dir);
    printf("turn right, move EAST: x=%d y=%d\n", x, y);

    dir = turn_right(dir);
    move_forward(&x, &y, dir);
    printf("turn right, move SOUTH: x=%d y=%d\n", x, y);

    dir = turn_right(dir);
    move_forward(&x, &y, dir);
    printf("turn right, move WEST: x=%d y=%d\n", x, y);

    if (x == 0 && y == 0) {
        printf("back to start: 1\n");
    } else {
        printf("back to start: 0\n");
    }

    printf("=== Priority Enum ===\n");
    printf("LOW=%d MEDIUM=%d HIGH=%d CRITICAL=%d\n",
        PRI_LOW, PRI_MEDIUM, PRI_HIGH, PRI_CRITICAL);

    printf("=== Priority Sort ===\n");
    task_ids[0] = 0; priorities[0] = PRI_HIGH;
    task_ids[1] = 1; priorities[1] = PRI_LOW;
    task_ids[2] = 2; priorities[2] = PRI_CRITICAL;
    task_ids[3] = 3; priorities[3] = PRI_MEDIUM;

    i = 0;
    while (i < 4) {
        printf("task %d: priority=%d\n", task_ids[i], priorities[i]);
        i = i + 1;
    }

    sort_by_priority(task_ids, priorities, 4);
    printf("sorted by priority:\n");
    i = 0;
    while (i < 4) {
        printf("task %d: priority=%d\n", task_ids[i], priorities[i]);
        i = i + 1;
    }

    printf("=== Enum Bit Patterns ===\n");
    printf("PERM_READ=%d\n", PERM_READ);
    printf("PERM_WRITE=%d\n", PERM_WRITE);
    printf("PERM_EXEC=%d\n", PERM_EXEC);
    rw = PERM_READ | PERM_WRITE;
    rwx = PERM_READ | PERM_WRITE | PERM_EXEC;
    rx = PERM_READ | PERM_EXEC;
    printf("rw=%d\n", rw);
    printf("rwx=%d\n", rwx);
    printf("rx=%d\n", rx);
    printf("has_read(rw)=%d\n", has_permission(rw, PERM_READ));
    printf("has_write(rw)=%d\n", has_permission(rw, PERM_WRITE));
    printf("has_exec(rw)=%d\n", has_permission(rw, PERM_EXEC));
    printf("has_read(rwx)=%d\n", has_permission(rwx, PERM_READ));
    printf("has_exec(rx)=%d\n", has_permission(rx, PERM_EXEC));

    printf("=== Switch with Expressions ===\n");
    val = 10 / 3;
    printf("10/3=%d -> ", val);
    print_switch_expr(val);
    val = 7 % 4;
    printf("7%%4=%d -> ", val);
    print_switch_expr(val);
    val = 2 * 2;
    printf("2*2=%d -> ", val);
    print_switch_expr(val);
    val = 15 % 10;
    printf("15%%10=%d -> ", val);
    print_switch_expr(val);

    printf("=== Multi-case Equivalent ===\n");
    i = 5;
    while (i >= 0) {
        print_grade(i);
        i = i - 1;
    }

    printf("=== All enum/switch tests done ===\n");

    return 0;
}
