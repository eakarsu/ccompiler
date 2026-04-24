int printf(const char *fmt, ...);

typedef void (*CallbackFn)(int event_id, int data);

struct Observer {
    int id;
    int call_count;
    int last_event;
    int last_data;
    int total;
    CallbackFn on_notify;
};

struct Subject {
    struct Observer *observers[8];
    int num_observers;
    int event_count;
};

void observer_callback_add(int event_id, int data) {
    (void)event_id;
    (void)data;
}

void subject_init(struct Subject *s) {
    s->num_observers = 0;
    s->event_count = 0;
    int i = 0;
    while (i < 8) {
        s->observers[i] = 0;
        i = i + 1;
    }
}

void observer_init(struct Observer *o, int id) {
    o->id = id;
    o->call_count = 0;
    o->last_event = -1;
    o->last_data = 0;
    o->total = 0;
    o->on_notify = observer_callback_add;
}

int subject_add(struct Subject *s, struct Observer *o) {
    if (s->num_observers >= 8) return -1;
    s->observers[s->num_observers] = o;
    s->num_observers = s->num_observers + 1;
    return 0;
}

int subject_remove(struct Subject *s, int obs_id) {
    int i = 0;
    while (i < s->num_observers) {
        if (s->observers[i]->id == obs_id) {
            int j = i;
            while (j < s->num_observers - 1) {
                s->observers[j] = s->observers[j + 1];
                j = j + 1;
            }
            s->num_observers = s->num_observers - 1;
            s->observers[s->num_observers] = 0;
            return 0;
        }
        i = i + 1;
    }
    return -1;
}

void subject_notify(struct Subject *s, int event_id, int data) {
    s->event_count = s->event_count + 1;
    int i = 0;
    while (i < s->num_observers) {
        struct Observer *o = s->observers[i];
        o->call_count = o->call_count + 1;
        o->last_event = event_id;
        o->last_data = data;
        o->total = o->total + data;
        i = i + 1;
    }
}

int subject_total_notifications(struct Subject *s) {
    int total = 0;
    int i = 0;
    while (i < s->num_observers) {
        total = total + s->observers[i]->call_count;
        i = i + 1;
    }
    return total;
}

int main(void) {
    struct Subject subj;
    subject_init(&subj);

    struct Observer obs[5];
    int i = 0;
    while (i < 5) {
        observer_init(&obs[i], i + 1);
        subject_add(&subj, &obs[i]);
        i = i + 1;
    }

    // EXPECT: num_obs=5
    printf("num_obs=%d\n", subj.num_observers);

    subject_notify(&subj, 10, 100);
    // EXPECT: event_count=1
    printf("event_count=%d\n", subj.event_count);

    // EXPECT: obs0_calls=1
    printf("obs0_calls=%d\n", obs[0].call_count);
    // EXPECT: obs0_last=10
    printf("obs0_last=%d\n", obs[0].last_event);
    // EXPECT: obs0_data=100
    printf("obs0_data=%d\n", obs[0].last_data);

    // EXPECT: obs4_calls=1
    printf("obs4_calls=%d\n", obs[4].call_count);
    // EXPECT: obs4_total=100
    printf("obs4_total=%d\n", obs[4].total);

    subject_notify(&subj, 20, 50);
    subject_notify(&subj, 30, 75);

    // EXPECT: event_count2=3
    printf("event_count2=%d\n", subj.event_count);
    // EXPECT: obs2_calls=3
    printf("obs2_calls=%d\n", obs[2].call_count);
    // EXPECT: obs2_total=225
    printf("obs2_total=%d\n", obs[2].total);
    // EXPECT: obs2_last_event=30
    printf("obs2_last_event=%d\n", obs[2].last_event);

    // EXPECT: total_notifs=15
    printf("total_notifs=%d\n", subject_total_notifications(&subj));

    int ret = subject_remove(&subj, 3);
    // EXPECT: remove_ret=0
    printf("remove_ret=%d\n", ret);
    // EXPECT: after_remove=4
    printf("after_remove=%d\n", subj.num_observers);

    subject_notify(&subj, 40, 10);
    // EXPECT: obs0_calls2=4
    printf("obs0_calls2=%d\n", obs[0].call_count);
    // EXPECT: obs2_calls_unchanged=3
    printf("obs2_calls_unchanged=%d\n", obs[2].call_count);

    // EXPECT: obs3_calls=4
    printf("obs3_calls=%d\n", obs[3].call_count);

    ret = subject_remove(&subj, 99);
    // EXPECT: bad_remove=-1
    printf("bad_remove=%d\n", ret);

    int grand_total = 0;
    i = 0;
    while (i < 5) {
        grand_total = grand_total + obs[i].total;
        i = i + 1;
    }
    // EXPECT: grand_total=1165
    printf("grand_total=%d\n", grand_total);

    return 0;
}
