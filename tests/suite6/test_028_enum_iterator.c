int printf(const char *fmt, ...);

/* Days of the week */
enum Day {
    MON = 0,
    TUE = 1,
    WED = 2,
    THU = 3,
    FRI = 4,
    SAT = 5,
    SUN = 6,
    DAY_COUNT = 7
};

/* Seasons */
enum Season {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3,
    SEASON_COUNT = 4
};

/* Month enum */
enum Month {
    JAN = 0, FEB = 1, MAR = 2, APR = 3, MAY = 4, JUN = 5,
    JUL = 6, AUG = 7, SEP = 8, OCT = 9, NOV = 10, DEC = 11,
    MONTH_COUNT = 12
};

char *day_name(int d) {
    if (d == MON) return "MON";
    if (d == TUE) return "TUE";
    if (d == WED) return "WED";
    if (d == THU) return "THU";
    if (d == FRI) return "FRI";
    if (d == SAT) return "SAT";
    if (d == SUN) return "SUN";
    return "???";
}

char *season_name(int s) {
    if (s == SPRING) return "SPRING";
    if (s == SUMMER) return "SUMMER";
    if (s == AUTUMN) return "AUTUMN";
    if (s == WINTER) return "WINTER";
    return "???";
}

char *month_name(int m) {
    if (m == JAN) return "JAN";
    if (m == FEB) return "FEB";
    if (m == MAR) return "MAR";
    if (m == APR) return "APR";
    if (m == MAY) return "MAY";
    if (m == JUN) return "JUN";
    if (m == JUL) return "JUL";
    if (m == AUG) return "AUG";
    if (m == SEP) return "SEP";
    if (m == OCT) return "OCT";
    if (m == NOV) return "NOV";
    if (m == DEC) return "DEC";
    return "???";
}

/* Category checks */
int is_weekday(int d) {
    return d >= MON && d <= FRI;
}

int is_weekend(int d) {
    return d == SAT || d == SUN;
}

/* Get season for a month */
int get_season(int month) {
    if (month >= MAR && month <= MAY) return SPRING;
    if (month >= JUN && month <= AUG) return SUMMER;
    if (month >= SEP && month <= NOV) return AUTUMN;
    return WINTER;  /* DEC, JAN, FEB */
}

/* Days in month (non-leap year) */
int days_in_month(int m) {
    if (m == FEB) return 28;
    if (m == APR || m == JUN || m == SEP || m == NOV) return 30;
    return 31;
}

/* Count matching days in a range */
int count_weekdays(int start, int end) {
    int count = 0;
    int d;
    for (d = start; d <= end; d++) {
        if (is_weekday(d)) count++;
    }
    return count;
}

/* Count months in a season */
int count_months_in_season(int season) {
    int count = 0;
    int m;
    for (m = 0; m < MONTH_COUNT; m++) {
        if (get_season(m) == season) count++;
    }
    return count;
}

/* Total days in a season */
int total_days_in_season(int season) {
    int total = 0;
    int m;
    for (m = 0; m < MONTH_COUNT; m++) {
        if (get_season(m) == season) {
            total += days_in_month(m);
        }
    }
    return total;
}

/* Find next day */
int next_day(int d) {
    return (d + 1) % DAY_COUNT;
}

/* Find day N days from now */
int day_after(int d, int n) {
    return (d + n) % DAY_COUNT;
}

int main(void) {
    /* Iterate over all days */
    int d;
    for (d = 0; d < DAY_COUNT; d++) {
        if (d > 0) printf(" ");
        printf("%s", day_name(d));
    }
    printf("\n");
    // EXPECT: MON TUE WED THU FRI SAT SUN

    /* Count weekdays and weekends */
    int weekdays = 0;
    int weekends = 0;
    for (d = 0; d < DAY_COUNT; d++) {
        if (is_weekday(d)) weekdays++;
        if (is_weekend(d)) weekends++;
    }
    // EXPECT: Weekdays: 5
    printf("Weekdays: %d\n", weekdays);
    // EXPECT: Weekends: 2
    printf("Weekends: %d\n", weekends);

    /* Count weekdays in range MON..THU */
    // EXPECT: Weekdays MON-THU: 4
    printf("Weekdays MON-THU: %d\n", count_weekdays(MON, THU));

    /* Iterate seasons */
    int s;
    for (s = 0; s < SEASON_COUNT; s++) {
        printf("Season: %s\n", season_name(s));
    }
    // EXPECT: Season: SPRING
    // EXPECT: Season: SUMMER
    // EXPECT: Season: AUTUMN
    // EXPECT: Season: WINTER

    /* Map months to seasons */
    // EXPECT: JAN -> WINTER
    printf("JAN -> %s\n", season_name(get_season(JAN)));
    // EXPECT: APR -> SPRING
    printf("APR -> %s\n", season_name(get_season(APR)));
    // EXPECT: JUL -> SUMMER
    printf("JUL -> %s\n", season_name(get_season(JUL)));
    // EXPECT: OCT -> AUTUMN
    printf("OCT -> %s\n", season_name(get_season(OCT)));

    /* Months per season */
    // EXPECT: Spring months: 3
    printf("Spring months: %d\n", count_months_in_season(SPRING));
    // EXPECT: Winter months: 3
    printf("Winter months: %d\n", count_months_in_season(WINTER));

    /* Total days per season */
    // EXPECT: Spring days: 92
    printf("Spring days: %d\n", total_days_in_season(SPRING));
    // EXPECT: Summer days: 92
    printf("Summer days: %d\n", total_days_in_season(SUMMER));

    /* Days in specific months */
    // EXPECT: Feb days: 28
    printf("Feb days: %d\n", days_in_month(FEB));
    // EXPECT: Jul days: 31
    printf("Jul days: %d\n", days_in_month(JUL));

    /* Total days in year */
    int total = 0;
    int m;
    for (m = 0; m < MONTH_COUNT; m++) {
        total += days_in_month(m);
    }
    // EXPECT: Year days: 365
    printf("Year days: %d\n", total);

    /* Next day */
    // EXPECT: After FRI: SAT
    printf("After FRI: %s\n", day_name(next_day(FRI)));
    // EXPECT: After SUN: MON
    printf("After SUN: %s\n", day_name(next_day(SUN)));

    /* Day after N days */
    // EXPECT: MON+3: THU
    printf("MON+3: %s\n", day_name(day_after(MON, 3)));
    // EXPECT: WED+7: WED
    printf("WED+7: %s\n", day_name(day_after(WED, 7)));

    return 0;
}
