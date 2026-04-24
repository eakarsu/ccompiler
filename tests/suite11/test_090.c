int printf(const char *fmt, ...);

// Real-Time Clock Simulation
// Tracks hours, minutes, seconds with alarm and calendar

enum RtcState { RTC_STOPPED, RTC_RUNNING };

struct RtcTime {
    int hours;
    int minutes;
    int seconds;
};

struct RtcDate {
    int year;
    int month;
    int day;
    int day_of_week;   // 0=Sunday, 6=Saturday
};

struct RtcAlarm {
    int enabled;
    int hours;
    int minutes;
    int seconds;
    int triggered;
    int trigger_count;
};

struct Rtc {
    struct RtcTime time;
    struct RtcDate date;
    struct RtcAlarm alarm;
    int state;
    int tick_counter;
    int ticks_per_second;
    int total_seconds;
};

void rtc_init(struct Rtc *rtc, int ticks_per_sec) {
    rtc->time.hours = 0;
    rtc->time.minutes = 0;
    rtc->time.seconds = 0;
    rtc->date.year = 2026;
    rtc->date.month = 1;
    rtc->date.day = 1;
    rtc->date.day_of_week = 3;  // Thursday
    rtc->alarm.enabled = 0;
    rtc->alarm.hours = 0;
    rtc->alarm.minutes = 0;
    rtc->alarm.seconds = 0;
    rtc->alarm.triggered = 0;
    rtc->alarm.trigger_count = 0;
    rtc->state = RTC_STOPPED;
    rtc->tick_counter = 0;
    rtc->ticks_per_second = ticks_per_sec;
    rtc->total_seconds = 0;
}

void rtc_set_time(struct Rtc *rtc, int h, int m, int s) {
    rtc->time.hours = h;
    rtc->time.minutes = m;
    rtc->time.seconds = s;
}

void rtc_set_date(struct Rtc *rtc, int year, int month, int day, int dow) {
    rtc->date.year = year;
    rtc->date.month = month;
    rtc->date.day = day;
    rtc->date.day_of_week = dow;
}

void rtc_set_alarm(struct Rtc *rtc, int h, int m, int s) {
    rtc->alarm.hours = h;
    rtc->alarm.minutes = m;
    rtc->alarm.seconds = s;
    rtc->alarm.enabled = 1;
    rtc->alarm.triggered = 0;
}

void rtc_start(struct Rtc *rtc) {
    rtc->state = RTC_RUNNING;
}

void rtc_stop(struct Rtc *rtc) {
    rtc->state = RTC_STOPPED;
}

int is_leap_year(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

int days_in_month(int month, int year) {
    int days[12];
    days[0] = 31; days[1] = 28; days[2] = 31; days[3] = 30;
    days[4] = 31; days[5] = 30; days[6] = 31; days[7] = 31;
    days[8] = 30; days[9] = 31; days[10] = 30; days[11] = 31;
    if (month < 1 || month > 12) return 30;
    if (month == 2 && is_leap_year(year)) return 29;
    return days[month - 1];
}

void rtc_advance_date(struct Rtc *rtc) {
    int dim = days_in_month(rtc->date.month, rtc->date.year);
    rtc->date.day = rtc->date.day + 1;
    rtc->date.day_of_week = (rtc->date.day_of_week + 1) % 7;
    if (rtc->date.day > dim) {
        rtc->date.day = 1;
        rtc->date.month = rtc->date.month + 1;
        if (rtc->date.month > 12) {
            rtc->date.month = 1;
            rtc->date.year = rtc->date.year + 1;
        }
    }
}

// Check alarm match
void rtc_check_alarm(struct Rtc *rtc) {
    if (!rtc->alarm.enabled) return;
    if (rtc->time.hours == rtc->alarm.hours &&
        rtc->time.minutes == rtc->alarm.minutes &&
        rtc->time.seconds == rtc->alarm.seconds) {
        if (!rtc->alarm.triggered) {
            rtc->alarm.triggered = 1;
            rtc->alarm.trigger_count = rtc->alarm.trigger_count + 1;
        }
    } else {
        rtc->alarm.triggered = 0;
    }
}

// Advance by one second
void rtc_tick_second(struct Rtc *rtc) {
    if (rtc->state != RTC_RUNNING) return;

    rtc->total_seconds = rtc->total_seconds + 1;
    rtc->time.seconds = rtc->time.seconds + 1;

    if (rtc->time.seconds >= 60) {
        rtc->time.seconds = 0;
        rtc->time.minutes = rtc->time.minutes + 1;
        if (rtc->time.minutes >= 60) {
            rtc->time.minutes = 0;
            rtc->time.hours = rtc->time.hours + 1;
            if (rtc->time.hours >= 24) {
                rtc->time.hours = 0;
                rtc_advance_date(rtc);
            }
        }
    }

    rtc_check_alarm(rtc);
}

// Advance by N seconds
void rtc_advance_seconds(struct Rtc *rtc, int n) {
    int i;
    for (i = 0; i < n; i++) {
        rtc_tick_second(rtc);
    }
}

// Convert time to total seconds since midnight
int rtc_time_to_seconds(struct Rtc *rtc) {
    return rtc->time.hours * 3600 + rtc->time.minutes * 60 + rtc->time.seconds;
}

// Seconds until alarm
int rtc_seconds_until_alarm(struct Rtc *rtc) {
    int now = rtc_time_to_seconds(rtc);
    int alarm_sec = rtc->alarm.hours * 3600 + rtc->alarm.minutes * 60 + rtc->alarm.seconds;
    if (alarm_sec > now) return alarm_sec - now;
    return (86400 - now) + alarm_sec;  // wrap around midnight
}

char *day_name(int dow) {
    if (dow == 0) return "Sun";
    if (dow == 1) return "Mon";
    if (dow == 2) return "Tue";
    if (dow == 3) return "Wed";
    if (dow == 4) return "Thu";
    if (dow == 5) return "Fri";
    if (dow == 6) return "Sat";
    return "???";
}

int main() {
    struct Rtc rtc;
    int secs;

    rtc_init(&rtc, 32768);
    printf("RTC initialized\n");
    // EXPECT: RTC initialized

    // Set initial time and date
    rtc_set_time(&rtc, 10, 30, 0);
    rtc_set_date(&rtc, 2026, 2, 28, 6);  // Saturday
    rtc_start(&rtc);

    printf("Time: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: Time: 10:30:0
    printf("Date: %d-%d-%d %s\n", rtc.date.year, rtc.date.month, rtc.date.day, day_name(rtc.date.day_of_week));
    // EXPECT: Date: 2026-2-28 Sat

    // Advance 30 seconds
    rtc_advance_seconds(&rtc, 30);
    printf("After 30s: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: After 30s: 10:30:30

    // Advance to next minute
    rtc_advance_seconds(&rtc, 30);
    printf("After 1min: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: After 1min: 10:31:0

    // Advance 29 minutes to 11:00:00
    rtc_advance_seconds(&rtc, 29 * 60);
    printf("After 30min: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: After 30min: 11:0:0

    // Test alarm
    rtc_set_alarm(&rtc, 11, 0, 5);
    printf("Alarm set for %d:%d:%d\n", rtc.alarm.hours, rtc.alarm.minutes, rtc.alarm.seconds);
    // EXPECT: Alarm set for 11:0:5

    secs = rtc_seconds_until_alarm(&rtc);
    printf("Seconds until alarm: %d\n", secs);
    // EXPECT: Seconds until alarm: 5

    rtc_advance_seconds(&rtc, 5);
    printf("Alarm triggered: %d\n", rtc.alarm.triggered);
    // EXPECT: Alarm triggered: 1
    printf("Alarm count: %d\n", rtc.alarm.trigger_count);
    // EXPECT: Alarm count: 1

    // Advance past alarm
    rtc_advance_seconds(&rtc, 1);
    printf("Alarm after pass: %d\n", rtc.alarm.triggered);
    // EXPECT: Alarm after pass: 0

    // Advance to midnight (cross day boundary)
    // Currently at 11:00:06, need 12h 59m 54s = 46794 seconds
    rtc_advance_seconds(&rtc, 46794);
    printf("Midnight: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: Midnight: 0:0:0
    printf("Next day: %d-%d-%d %s\n", rtc.date.year, rtc.date.month, rtc.date.day, day_name(rtc.date.day_of_week));
    // EXPECT: Next day: 2026-3-1 Sun

    // 2026 is not a leap year, so Feb has 28 days
    printf("Feb 2026 days: %d\n", days_in_month(2, 2026));
    // EXPECT: Feb 2026 days: 28

    // Test leap year
    printf("Feb 2024 days: %d\n", days_in_month(2, 2024));
    // EXPECT: Feb 2024 days: 29
    printf("Feb 2000 days: %d\n", days_in_month(2, 2000));
    // EXPECT: Feb 2000 days: 29
    printf("Feb 1900 days: %d\n", days_in_month(2, 1900));
    // EXPECT: Feb 1900 days: 28

    // Advance to end of year
    // Currently March 1, advance through the rest of the year
    // March has 31 days, need to go to Dec 31
    // Let's just advance a few days
    rtc_advance_seconds(&rtc, 86400 * 3);  // 3 days
    printf("After 3 days: %d-%d-%d %s\n", rtc.date.year, rtc.date.month, rtc.date.day, day_name(rtc.date.day_of_week));
    // EXPECT: After 3 days: 2026-3-4 Wed

    // Time to seconds
    rtc_set_time(&rtc, 1, 30, 45);
    secs = rtc_time_to_seconds(&rtc);
    printf("1:30:45 in seconds: %d\n", secs);
    // EXPECT: 1:30:45 in seconds: 5445

    // Test alarm wrap around midnight
    rtc_set_time(&rtc, 23, 59, 50);
    rtc_set_alarm(&rtc, 0, 0, 5);
    secs = rtc_seconds_until_alarm(&rtc);
    printf("Seconds until midnight alarm: %d\n", secs);
    // EXPECT: Seconds until midnight alarm: 15

    // Total seconds tracked
    printf("Total seconds elapsed: %d\n", rtc.total_seconds);
    // EXPECT: Total seconds elapsed: 307800

    // Test RTC stop
    rtc_stop(&rtc);
    rtc_advance_seconds(&rtc, 100);
    printf("Stopped time: %d:%d:%d\n", rtc.time.hours, rtc.time.minutes, rtc.time.seconds);
    // EXPECT: Stopped time: 23:59:50

    printf("RTC test complete\n");
    // EXPECT: RTC test complete

    return 0;
}
