int printf(const char *fmt, ...);

/* Message system with different payload types via tagged union */
enum MsgType {
    MSG_TEXT = 0,
    MSG_NUMBER = 1,
    MSG_COORD = 2,
    MSG_CMD = 3
};

struct Coord {
    int x;
    int y;
};

struct Command {
    int opcode;
    int arg;
};

union MsgPayload {
    char text[16];
    int number;
    struct Coord coord;
    struct Command cmd;
};

struct Message {
    int type;
    int priority;
    union MsgPayload payload;
};

/* Message constructors */
struct Message make_text_msg(int prio, char c0, char c1, char c2, char c3) {
    struct Message m;
    m.type = MSG_TEXT;
    m.priority = prio;
    m.payload.text[0] = c0;
    m.payload.text[1] = c1;
    m.payload.text[2] = c2;
    m.payload.text[3] = c3;
    m.payload.text[4] = 0;
    return m;
}

struct Message make_num_msg(int prio, int val) {
    struct Message m;
    m.type = MSG_NUMBER;
    m.priority = prio;
    m.payload.number = val;
    return m;
}

struct Message make_coord_msg(int prio, int x, int y) {
    struct Message m;
    m.type = MSG_COORD;
    m.priority = prio;
    m.payload.coord.x = x;
    m.payload.coord.y = y;
    return m;
}

struct Message make_cmd_msg(int prio, int opcode, int arg) {
    struct Message m;
    m.type = MSG_CMD;
    m.priority = prio;
    m.payload.cmd.opcode = opcode;
    m.payload.cmd.arg = arg;
    return m;
}

/* Type name */
char *msg_type_name(int type) {
    if (type == MSG_TEXT) return "TEXT";
    if (type == MSG_NUMBER) return "NUMBER";
    if (type == MSG_COORD) return "COORD";
    if (type == MSG_CMD) return "CMD";
    return "UNKNOWN";
}

/* Print message */
void print_msg(struct Message *m) {
    if (m->type == MSG_TEXT) {
        printf("[%s p=%d] %s\n", msg_type_name(m->type), m->priority, m->payload.text);
    } else if (m->type == MSG_NUMBER) {
        printf("[%s p=%d] %d\n", msg_type_name(m->type), m->priority, m->payload.number);
    } else if (m->type == MSG_COORD) {
        printf("[%s p=%d] (%d,%d)\n", msg_type_name(m->type), m->priority,
               m->payload.coord.x, m->payload.coord.y);
    } else if (m->type == MSG_CMD) {
        printf("[%s p=%d] op=%d arg=%d\n", msg_type_name(m->type), m->priority,
               m->payload.cmd.opcode, m->payload.cmd.arg);
    }
}

/* Message queue (simple ring buffer) */
struct MsgQueue {
    struct Message msgs[8];
    int head;
    int tail;
    int count;
};

void queue_init(struct MsgQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_push(struct MsgQueue *q, struct Message m) {
    if (q->count >= 8) return 0;
    q->msgs[q->tail] = m;
    q->tail = (q->tail + 1) % 8;
    q->count++;
    return 1;
}

struct Message queue_pop(struct MsgQueue *q) {
    struct Message m = q->msgs[q->head];
    q->head = (q->head + 1) % 8;
    q->count--;
    return m;
}

int queue_empty(struct MsgQueue *q) {
    return q->count == 0;
}

/* Count messages of a given type in queue */
int count_msg_type(struct MsgQueue *q, int type) {
    int count = 0;
    int i;
    for (i = 0; i < q->count; i++) {
        int idx = (q->head + i) % 8;
        if (q->msgs[idx].type == type) count++;
    }
    return count;
}

/* Find highest priority in queue */
int max_priority(struct MsgQueue *q) {
    if (q->count == 0) return -1;
    int maxp = 0;
    int i;
    for (i = 0; i < q->count; i++) {
        int idx = (q->head + i) % 8;
        if (q->msgs[idx].priority > maxp) {
            maxp = q->msgs[idx].priority;
        }
    }
    return maxp;
}

int main(void) {
    /* Test message constructors and printing */
    struct Message m1 = make_text_msg(1, 'H', 'i', '!', 0);
    // EXPECT: [TEXT p=1] Hi!
    print_msg(&m1);

    struct Message m2 = make_num_msg(2, 42);
    // EXPECT: [NUMBER p=2] 42
    print_msg(&m2);

    struct Message m3 = make_coord_msg(3, 10, 20);
    // EXPECT: [COORD p=3] (10,20)
    print_msg(&m3);

    struct Message m4 = make_cmd_msg(5, 100, 200);
    // EXPECT: [CMD p=5] op=100 arg=200
    print_msg(&m4);

    /* Test type names */
    // EXPECT: Type 0: TEXT
    printf("Type 0: %s\n", msg_type_name(0));
    // EXPECT: Type 1: NUMBER
    printf("Type 1: %s\n", msg_type_name(1));
    // EXPECT: Type 2: COORD
    printf("Type 2: %s\n", msg_type_name(2));
    // EXPECT: Type 3: CMD
    printf("Type 3: %s\n", msg_type_name(3));

    /* Test message queue */
    struct MsgQueue q;
    queue_init(&q);
    // EXPECT: Queue empty: 1
    printf("Queue empty: %d\n", queue_empty(&q));

    queue_push(&q, make_num_msg(1, 100));
    queue_push(&q, make_num_msg(3, 200));
    queue_push(&q, make_coord_msg(2, 5, 6));
    queue_push(&q, make_text_msg(4, 'O', 'K', 0, 0));
    queue_push(&q, make_cmd_msg(1, 1, 2));

    // EXPECT: Queue count: 5
    printf("Queue count: %d\n", q.count);
    // EXPECT: Queue empty: 0
    printf("Queue empty: %d\n", queue_empty(&q));

    /* Count by type */
    // EXPECT: NUM msgs: 2
    printf("NUM msgs: %d\n", count_msg_type(&q, MSG_NUMBER));
    // EXPECT: COORD msgs: 1
    printf("COORD msgs: %d\n", count_msg_type(&q, MSG_COORD));
    // EXPECT: TEXT msgs: 1
    printf("TEXT msgs: %d\n", count_msg_type(&q, MSG_TEXT));
    // EXPECT: CMD msgs: 1
    printf("CMD msgs: %d\n", count_msg_type(&q, MSG_CMD));

    /* Max priority */
    // EXPECT: Max priority: 4
    printf("Max priority: %d\n", max_priority(&q));

    /* Pop and process messages */
    struct Message popped = queue_pop(&q);
    // EXPECT: Popped type: NUMBER
    printf("Popped type: %s\n", msg_type_name(popped.type));
    // EXPECT: Popped val: 100
    printf("Popped val: %d\n", popped.payload.number);

    // EXPECT: After pop count: 4
    printf("After pop count: %d\n", q.count);

    /* Pop remaining and sum all number payloads */
    int total_nums = 0;
    int msg_count = 0;
    while (!queue_empty(&q)) {
        struct Message msg = queue_pop(&q);
        msg_count++;
        if (msg.type == MSG_NUMBER) {
            total_nums += msg.payload.number;
        }
    }
    // EXPECT: Processed: 4
    printf("Processed: %d\n", msg_count);
    // EXPECT: Sum of numbers: 200
    printf("Sum of numbers: %d\n", total_nums);

    return 0;
}
