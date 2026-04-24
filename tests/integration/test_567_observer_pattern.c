int printf(const char *fmt, ...);
// EXPECT: Subscribers: 4\nEvent 1 (data=30):\n  [LOG] event=1 data=30\n  [COUNT] Total events seen: 1\nEvent 2 (data=75):\n  [LOG] event=2 data=75\n  [ALERT] High value: 75\n  [COUNT] Total events seen: 2\nEvent 3 (data=10):\n  [LOG] event=3 data=10\n  [COUNT] Total events seen: 3\nStats sum: 115\nLogger calls: 3\nAlerts: 1\nEvents fired: 3\nAfter unsubscribe, subscribers: 3\nEvent 4 (data=80):\n  [ALERT] High value: 80\n  [COUNT] Total events seen: 4\nStats sum: 195

#define MAX_OBSERVERS 8

typedef void (*ObserverFn)(int event_id, int data);

struct EventSystem {
    ObserverFn observers[MAX_OBSERVERS];
    char *names[MAX_OBSERVERS];
    int count;
    int events_fired;
};

void es_init(struct EventSystem *es) {
    es->count = 0;
    es->events_fired = 0;
}

int es_subscribe(struct EventSystem *es, ObserverFn fn, char *name) {
    if (es->count >= MAX_OBSERVERS) return -1;
    es->observers[es->count] = fn;
    es->names[es->count] = name;
    es->count++;
    return es->count - 1;
}

void es_notify(struct EventSystem *es, int event_id, int data) {
    int i;
    es->events_fired++;
    for (i = 0; i < es->count; i++) {
        es->observers[i](event_id, data);
    }
}

void es_unsubscribe(struct EventSystem *es, int handle) {
    if (handle < 0 || handle >= es->count) return;
    int i;
    for (i = handle; i < es->count - 1; i++) {
        es->observers[i] = es->observers[i + 1];
        es->names[i] = es->names[i + 1];
    }
    es->count--;
}

int g_logger_count = 0;
int g_stats_sum = 0;
int g_alert_count = 0;

void logger_observer(int event_id, int data) {
    printf("  [LOG] event=%d data=%d\n", event_id, data);
    g_logger_count++;
}

void stats_observer(int event_id, int data) {
    g_stats_sum = g_stats_sum + data;
}

void alert_observer(int event_id, int data) {
    if (data > 50) {
        printf("  [ALERT] High value: %d\n", data);
        g_alert_count++;
    }
}

void counter_observer(int event_id, int data) {
    static int count = 0;
    count++;
    printf("  [COUNT] Total events seen: %d\n", count);
}

int main(void) {
    struct EventSystem es;
    es_init(&es);

    es_subscribe(&es, logger_observer, "logger");
    es_subscribe(&es, stats_observer, "stats");
    es_subscribe(&es, alert_observer, "alert");
    es_subscribe(&es, counter_observer, "counter");
    printf("Subscribers: %d\n", es.count);

    printf("Event 1 (data=30):\n");
    es_notify(&es, 1, 30);

    printf("Event 2 (data=75):\n");
    es_notify(&es, 2, 75);

    printf("Event 3 (data=10):\n");
    es_notify(&es, 3, 10);

    printf("Stats sum: %d\n", g_stats_sum);
    printf("Logger calls: %d\n", g_logger_count);
    printf("Alerts: %d\n", g_alert_count);
    printf("Events fired: %d\n", es.events_fired);

    es_unsubscribe(&es, 0);
    printf("After unsubscribe, subscribers: %d\n", es.count);

    printf("Event 4 (data=80):\n");
    es_notify(&es, 4, 80);
    printf("Stats sum: %d\n", g_stats_sum);

    return 0;
}
