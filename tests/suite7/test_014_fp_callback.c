int printf(const char *fmt, ...);

typedef void (*Callback)(int event_id, int data);

int g_log_count;
int g_alert_count;
int g_total_data;
int g_last_event;
int g_max_data;

void logger_cb(int event_id, int data) {
    g_log_count = g_log_count + 1;
    g_last_event = event_id;
}

void alert_cb(int event_id, int data) {
    g_alert_count = g_alert_count + 1;
    if (data > 100) {
        g_alert_count = g_alert_count + 1;
    }
}

void accumulator_cb(int event_id, int data) {
    g_total_data = g_total_data + data;
}

void max_tracker_cb(int event_id, int data) {
    if (data > g_max_data) {
        g_max_data = data;
    }
}

struct EventSystem {
    Callback callbacks[8];
    int count;
};

void es_init(struct EventSystem *es) {
    es->count = 0;
}

void es_register(struct EventSystem *es, Callback cb) {
    if (es->count < 8) {
        es->callbacks[es->count] = cb;
        es->count = es->count + 1;
    }
}

void es_fire(struct EventSystem *es, int event_id, int data) {
    int i;
    for (i = 0; i < es->count; i++) {
        es->callbacks[i](event_id, data);
    }
}

void es_fire_batch(struct EventSystem *es, int ids[], int datas[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        es_fire(es, ids[i], datas[i]);
    }
}

int es_fire_filtered(struct EventSystem *es, int event_id, int data, int min_data) {
    int i;
    int fired;
    fired = 0;
    if (data >= min_data) {
        for (i = 0; i < es->count; i++) {
            es->callbacks[i](event_id, data);
            fired = fired + 1;
        }
    }
    return fired;
}

void reset_globals(void) {
    g_log_count = 0;
    g_alert_count = 0;
    g_total_data = 0;
    g_last_event = -1;
    g_max_data = 0;
}

int main(void) {
    struct EventSystem es;
    int ids[5];
    int datas[5];
    int fired;

    reset_globals();
    es_init(&es);

    es_register(&es, logger_cb);
    es_register(&es, alert_cb);
    es_register(&es, accumulator_cb);
    // EXPECT: registered: 3
    printf("registered: %d\n", es.count);

    es_fire(&es, 1, 50);
    // EXPECT: after event 1: log=1 alert=1 total=50
    printf("after event 1: log=%d alert=%d total=%d\n", g_log_count, g_alert_count, g_total_data);

    es_fire(&es, 2, 150);
    // EXPECT: after event 2: log=2 alert=3 total=200
    printf("after event 2: log=%d alert=%d total=%d\n", g_log_count, g_alert_count, g_total_data);

    // EXPECT: last event: 2
    printf("last event: %d\n", g_last_event);

    es_fire(&es, 3, 30);
    es_fire(&es, 4, 200);
    // EXPECT: after 4 events: log=4 alert=6 total=430
    printf("after 4 events: log=%d alert=%d total=%d\n", g_log_count, g_alert_count, g_total_data);

    reset_globals();
    es_register(&es, max_tracker_cb);
    // EXPECT: registered now: 4
    printf("registered now: %d\n", es.count);

    ids[0] = 10; ids[1] = 11; ids[2] = 12; ids[3] = 13; ids[4] = 14;
    datas[0] = 5; datas[1] = 80; datas[2] = 200; datas[3] = 40; datas[4] = 150;
    es_fire_batch(&es, ids, datas, 5);
    // EXPECT: batch: log=5 total=475 max=200
    printf("batch: log=%d total=%d max=%d\n", g_log_count, g_total_data, g_max_data);
    // EXPECT: batch last: 14
    printf("batch last: %d\n", g_last_event);

    reset_globals();
    fired = es_fire_filtered(&es, 99, 50, 100);
    // EXPECT: filtered low: fired=0 log=0
    printf("filtered low: fired=%d log=%d\n", fired, g_log_count);

    fired = es_fire_filtered(&es, 99, 150, 100);
    // EXPECT: filtered high: fired=4 log=1
    printf("filtered high: fired=%d log=%d\n", fired, g_log_count);

    reset_globals();
    es_fire(&es, 1, 10);
    es_fire(&es, 2, 20);
    es_fire(&es, 3, 30);
    // EXPECT: three events: total=60 max=30
    printf("three events: total=%d max=%d\n", g_total_data, g_max_data);

    // EXPECT: alert count: 3
    printf("alert count: %d\n", g_alert_count);

    es_fire(&es, 4, 500);
    // EXPECT: big event: alert=5 max=500
    printf("big event: alert=%d max=%d\n", g_alert_count, g_max_data);

    // EXPECT: total after big: 560
    printf("total after big: %d\n", g_total_data);

    // EXPECT: log after big: 4
    printf("log after big: %d\n", g_log_count);

    return 0;
}
