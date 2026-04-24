int printf(const char *fmt, ...);

struct Date {
    int year;
    int month;
    int day;
};

struct Time {
    int hour;
    int minute;
    int second;
};

struct DateTime {
    struct Date date;
    struct Time time;
};

struct Version {
    int major;
    int minor;
    int patch;
};

int date_cmp(struct Date *a, struct Date *b) {
    if (a->year != b->year) return a->year - b->year;
    if (a->month != b->month) return a->month - b->month;
    return a->day - b->day;
}

int time_cmp(struct Time *a, struct Time *b) {
    if (a->hour != b->hour) return a->hour - b->hour;
    if (a->minute != b->minute) return a->minute - b->minute;
    return a->second - b->second;
}

int datetime_cmp(struct DateTime *a, struct DateTime *b) {
    int dc = date_cmp(&a->date, &b->date);
    if (dc != 0) return dc;
    return time_cmp(&a->time, &b->time);
}

int version_cmp(struct Version *a, struct Version *b) {
    if (a->major != b->major) return a->major - b->major;
    if (a->minor != b->minor) return a->minor - b->minor;
    return a->patch - b->patch;
}

int version_compatible(struct Version *a, struct Version *b) {
    return (a->major == b->major) && (a->minor >= b->minor);
}

int date_to_days(struct Date *d) {
    return d->year * 365 + d->month * 30 + d->day;
}

int days_between(struct Date *a, struct Date *b) {
    int da = date_to_days(a);
    int db = date_to_days(b);
    int diff = da - db;
    if (diff < 0) diff = -diff;
    return diff;
}

int time_to_seconds(struct Time *t) {
    return t->hour * 3600 + t->minute * 60 + t->second;
}

int seconds_between(struct Time *a, struct Time *b) {
    int sa = time_to_seconds(a);
    int sb = time_to_seconds(b);
    int diff = sa - sb;
    if (diff < 0) diff = -diff;
    return diff;
}

void init_date(struct Date *d, int y, int m, int day) {
    d->year = y;
    d->month = m;
    d->day = day;
}

void init_time(struct Time *t, int h, int m, int s) {
    t->hour = h;
    t->minute = m;
    t->second = s;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void sort_dates(struct Date *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (date_cmp(&arr[j], &arr[j + 1]) > 0) {
                struct Date tmp;
                tmp.year = arr[j].year;
                tmp.month = arr[j].month;
                tmp.day = arr[j].day;
                arr[j].year = arr[j + 1].year;
                arr[j].month = arr[j + 1].month;
                arr[j].day = arr[j + 1].day;
                arr[j + 1].year = tmp.year;
                arr[j + 1].month = tmp.month;
                arr[j + 1].day = tmp.day;
            }
        }
    }
}

int main() {
    struct Date d1;
    init_date(&d1, 2024, 6, 15);
    struct Date d2;
    init_date(&d2, 2024, 6, 15);
    struct Date d3;
    init_date(&d3, 2024, 7, 1);

    // EXPECT: d1 vs d2: 0
    printf("d1 vs d2: %d\n", sign(date_cmp(&d1, &d2)));
    // EXPECT: d1 vs d3: -1
    printf("d1 vs d3: %d\n", sign(date_cmp(&d1, &d3)));
    // EXPECT: d3 vs d1: 1
    printf("d3 vs d1: %d\n", sign(date_cmp(&d3, &d1)));

    // EXPECT: Days between d1 and d3: 16
    printf("Days between d1 and d3: %d\n", days_between(&d1, &d3));

    struct Time t1;
    init_time(&t1, 10, 30, 0);
    struct Time t2;
    init_time(&t2, 14, 45, 30);

    // EXPECT: t1 vs t2: -1
    printf("t1 vs t2: %d\n", sign(time_cmp(&t1, &t2)));

    // EXPECT: Seconds between: 15330
    printf("Seconds between: %d\n", seconds_between(&t1, &t2));

    // EXPECT: t1 in seconds: 37800
    printf("t1 in seconds: %d\n", time_to_seconds(&t1));

    struct DateTime dt1;
    init_date(&dt1.date, 2024, 1, 1);
    init_time(&dt1.time, 12, 0, 0);
    struct DateTime dt2;
    init_date(&dt2.date, 2024, 1, 1);
    init_time(&dt2.time, 13, 0, 0);
    struct DateTime dt3;
    init_date(&dt3.date, 2023, 12, 31);
    init_time(&dt3.time, 23, 59, 59);

    // EXPECT: dt1 vs dt2 (same day, diff time): -1
    printf("dt1 vs dt2 (same day, diff time): %d\n", sign(datetime_cmp(&dt1, &dt2)));
    // EXPECT: dt1 vs dt3 (diff day): 1
    printf("dt1 vs dt3 (diff day): %d\n", sign(datetime_cmp(&dt1, &dt3)));

    struct Version v1;
    v1.major = 2; v1.minor = 3; v1.patch = 1;
    struct Version v2;
    v2.major = 2; v2.minor = 3; v2.patch = 5;
    struct Version v3;
    v3.major = 3; v3.minor = 0; v3.patch = 0;

    // EXPECT: v1 vs v2: -1
    printf("v1 vs v2: %d\n", sign(version_cmp(&v1, &v2)));
    // EXPECT: v2 vs v3: -1
    printf("v2 vs v3: %d\n", sign(version_cmp(&v2, &v3)));
    // EXPECT: v1 compatible with v2: 1
    printf("v1 compatible with v2: %d\n", version_compatible(&v1, &v2));
    // EXPECT: v1 compatible with v3: 0
    printf("v1 compatible with v3: %d\n", version_compatible(&v1, &v3));

    struct Date dates[4];
    init_date(&dates[0], 2024, 12, 25);
    init_date(&dates[1], 2024, 1, 1);
    init_date(&dates[2], 2024, 7, 4);
    init_date(&dates[3], 2024, 3, 14);

    sort_dates(dates, 4);
    // EXPECT: Sorted dates:
    printf("Sorted dates:\n");
    int i;
    for (i = 0; i < 4; i = i + 1) {
        // EXPECT:   2024-1-1
        // EXPECT:   2024-3-14
        // EXPECT:   2024-7-4
        // EXPECT:   2024-12-25
        printf("  %d-%d-%d\n", dates[i].year, dates[i].month, dates[i].day);
    }

    return 0;
}
