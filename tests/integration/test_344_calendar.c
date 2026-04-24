int printf(const char *fmt, ...);
// EXPECT: total events: 7\nevents on 3/15: 2\nevents in march: 4\nevents in april: 3\nhours in march: 10\nbusiest march day: 15\nhigh priority: 4\nconflict 3/15 10-12: 1\nconflict 3/15 12-14: 0\nconflict 3/15 11-12: 0
// Test: calendar with structs

struct Event {
    int month;
    int day;
    int hour;
    int duration;
    int priority;
};

struct Calendar {
    struct Event events[16];
    int count;
};

void cal_init(struct Calendar *c) {
    c->count = 0;
}

int cal_add(struct Calendar *c, int month, int day, int hour, int duration, int priority) {
    if (c->count >= 16) return 0;
    int idx = c->count;
    c->events[idx].month = month;
    c->events[idx].day = day;
    c->events[idx].hour = hour;
    c->events[idx].duration = duration;
    c->events[idx].priority = priority;
    c->count = c->count + 1;
    return 1;
}

int cal_events_on_day(struct Calendar *c, int month, int day) {
    int count = 0;
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->events[i].month == month && c->events[i].day == day) {
            count = count + 1;
        }
    }
    return count;
}

int cal_events_in_month(struct Calendar *c, int month) {
    int count = 0;
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->events[i].month == month) count = count + 1;
    }
    return count;
}

int cal_total_hours_month(struct Calendar *c, int month) {
    int total = 0;
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->events[i].month == month) {
            total = total + c->events[i].duration;
        }
    }
    return total;
}

int cal_has_conflict(struct Calendar *c, int month, int day, int hour, int duration) {
    int i;
    int end = hour + duration;
    for (i = 0; i < c->count; i++) {
        if (c->events[i].month == month && c->events[i].day == day) {
            int evt_end = c->events[i].hour + c->events[i].duration;
            if (hour < evt_end && end > c->events[i].hour) {
                return 1;
            }
        }
    }
    return 0;
}

int cal_busiest_day(struct Calendar *c, int month) {
    int max_count = 0;
    int max_day = 0;
    int day;
    for (day = 1; day <= 31; day++) {
        int count = cal_events_on_day(c, month, day);
        if (count > max_count) {
            max_count = count;
            max_day = day;
        }
    }
    return max_day;
}

int cal_high_priority(struct Calendar *c) {
    int count = 0;
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->events[i].priority >= 3) count = count + 1;
    }
    return count;
}

int main(void) {
    struct Calendar cal;
    cal_init(&cal);

    cal_add(&cal, 3, 15, 9, 2, 3);
    cal_add(&cal, 3, 15, 14, 1, 2);
    cal_add(&cal, 3, 16, 10, 3, 4);
    cal_add(&cal, 3, 20, 8, 4, 5);
    cal_add(&cal, 4, 1, 9, 2, 3);
    cal_add(&cal, 4, 1, 13, 2, 2);
    cal_add(&cal, 4, 5, 11, 1, 1);

    printf("total events: %d\n", cal.count);
    printf("events on 3/15: %d\n", cal_events_on_day(&cal, 3, 15));
    printf("events in march: %d\n", cal_events_in_month(&cal, 3));
    printf("events in april: %d\n", cal_events_in_month(&cal, 4));
    printf("hours in march: %d\n", cal_total_hours_month(&cal, 3));
    printf("busiest march day: %d\n", cal_busiest_day(&cal, 3));
    printf("high priority: %d\n", cal_high_priority(&cal));

    printf("conflict 3/15 10-12: %d\n", cal_has_conflict(&cal, 3, 15, 10, 2));
    printf("conflict 3/15 12-14: %d\n", cal_has_conflict(&cal, 3, 15, 12, 2));
    printf("conflict 3/15 11-12: %d\n", cal_has_conflict(&cal, 3, 15, 11, 1));

    return 0;
}
