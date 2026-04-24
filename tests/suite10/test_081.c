int printf(const char *fmt, ...);

// Observer Pattern: callback list with subscribe/notify

typedef void (*Callback)(int);

struct Observer {
    Callback cb;
    int id;
};

struct Subject {
    struct Observer observers[8];
    int count;
    int value;
};

void subject_init(struct Subject *s) {
    s->count = 0;
    s->value = 0;
}

void subject_subscribe(struct Subject *s, Callback cb, int id) {
    if (s->count < 8) {
        s->observers[s->count].cb = cb;
        s->observers[s->count].id = id;
        s->count = s->count + 1;
    }
}

void subject_unsubscribe(struct Subject *s, int id) {
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        if (s->observers[i].id == id) {
            int j;
            for (j = i; j < s->count - 1; j = j + 1) {
                s->observers[j] = s->observers[j + 1];
            }
            s->count = s->count - 1;
            return;
        }
    }
}

int notify_log[32];
int notify_log_count;

void subject_notify(struct Subject *s) {
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        s->observers[i].cb(s->value);
    }
}

void subject_set_value(struct Subject *s, int val) {
    s->value = val;
    subject_notify(s);
}

void observer_a(int val) {
    notify_log[notify_log_count] = val + 1000;
    notify_log_count = notify_log_count + 1;
}

void observer_b(int val) {
    notify_log[notify_log_count] = val + 2000;
    notify_log_count = notify_log_count + 1;
}

void observer_c(int val) {
    notify_log[notify_log_count] = val + 3000;
    notify_log_count = notify_log_count + 1;
}

void print_log(char *label) {
    int i;
    printf("%s:", label);
    for (i = 0; i < notify_log_count; i = i + 1) {
        printf(" %d", notify_log[i]);
    }
    printf("\n");
}

void clear_log() {
    notify_log_count = 0;
}

int main() {
    struct Subject subj;
    subject_init(&subj);
    notify_log_count = 0;

    subject_subscribe(&subj, observer_a, 1);
    subject_subscribe(&subj, observer_b, 2);
    subject_subscribe(&subj, observer_c, 3);

    printf("Subscribers: %d\n", subj.count); // EXPECT: Subscribers: 3

    clear_log();
    subject_set_value(&subj, 10);
    print_log("Notify(10)"); // EXPECT: Notify(10): 1010 2010 3010
    printf("Log entries: %d\n", notify_log_count); // EXPECT: Log entries: 3

    subject_unsubscribe(&subj, 2);
    printf("After unsub B: %d\n", subj.count); // EXPECT: After unsub B: 2

    clear_log();
    subject_set_value(&subj, 20);
    print_log("Notify(20)"); // EXPECT: Notify(20): 1020 3020
    printf("Log entries: %d\n", notify_log_count); // EXPECT: Log entries: 2

    subject_unsubscribe(&subj, 1);
    subject_unsubscribe(&subj, 3);
    printf("After unsub all: %d\n", subj.count); // EXPECT: After unsub all: 0

    clear_log();
    subject_set_value(&subj, 99);
    printf("Log entries (none): %d\n", notify_log_count); // EXPECT: Log entries (none): 0

    subject_subscribe(&subj, observer_a, 10);
    subject_subscribe(&subj, observer_b, 11);
    subject_subscribe(&subj, observer_c, 12);
    subject_subscribe(&subj, observer_a, 13);
    printf("Subscribers: %d\n", subj.count); // EXPECT: Subscribers: 4

    clear_log();
    subject_set_value(&subj, 5);
    print_log("Notify(5)"); // EXPECT: Notify(5): 1005 2005 3005 1005
    printf("Log entries: %d\n", notify_log_count); // EXPECT: Log entries: 4

    subject_unsubscribe(&subj, 12);
    printf("After unsub 12: %d\n", subj.count); // EXPECT: After unsub 12: 3

    clear_log();
    subject_set_value(&subj, 7);
    print_log("Notify(7)"); // EXPECT: Notify(7): 1007 2007 1007

    subject_unsubscribe(&subj, 10);
    subject_unsubscribe(&subj, 11);
    subject_unsubscribe(&subj, 13);
    printf("Final count: %d\n", subj.count); // EXPECT: Final count: 0

    printf("Done\n"); // EXPECT: Done
    return 0;
}
