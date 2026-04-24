int printf(const char *fmt, ...);
// EXPECT: date: 2024-03-15\nvalid: 1\nday of year: 75\nleap year 2024: 1\nleap year 2023: 0\nleap year 2000: 1\nleap year 1900: 0\ndays in feb 2024: 29\ndays in feb 2023: 28\nafter 2024-12-31: 2025-01-01\nafter 2024-02-28: 2024-02-29\ndays between: 74\ncompare: 1
// Test: struct for date/time operations

struct Date {
    int year;
    int month;
    int day;
};

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
    if (month == 2 && is_leap_year(year)) return 29;
    return days[month - 1];
}

int is_valid_date(struct Date *d) {
    if (d->month < 1 || d->month > 12) return 0;
    if (d->day < 1 || d->day > days_in_month(d->month, d->year)) return 0;
    return 1;
}

int day_of_year(struct Date *d) {
    int total = 0;
    int m;
    for (m = 1; m < d->month; m++) {
        total = total + days_in_month(m, d->year);
    }
    total = total + d->day;
    return total;
}

int date_compare(struct Date *a, struct Date *b) {
    if (a->year != b->year) return a->year - b->year;
    if (a->month != b->month) return a->month - b->month;
    return a->day - b->day;
}

void date_next_day(struct Date *d) {
    d->day = d->day + 1;
    if (d->day > days_in_month(d->month, d->year)) {
        d->day = 1;
        d->month = d->month + 1;
        if (d->month > 12) {
            d->month = 1;
            d->year = d->year + 1;
        }
    }
}

void date_print(struct Date *d) {
    printf("%d-%02d-%02d", d->year, d->month, d->day);
}

int days_between(struct Date *a, struct Date *b) {
    int da = day_of_year(a);
    int db = day_of_year(b);
    if (a->year == b->year) {
        int diff = db - da;
        if (diff < 0) diff = -diff;
        return diff;
    }
    return -1;
}

int main(void) {
    struct Date d1;
    d1.year = 2024; d1.month = 3; d1.day = 15;
    printf("date: "); date_print(&d1); printf("\n");
    printf("valid: %d\n", is_valid_date(&d1));
    printf("day of year: %d\n", day_of_year(&d1));
    printf("leap year 2024: %d\n", is_leap_year(2024));
    printf("leap year 2023: %d\n", is_leap_year(2023));
    printf("leap year 2000: %d\n", is_leap_year(2000));
    printf("leap year 1900: %d\n", is_leap_year(1900));

    printf("days in feb 2024: %d\n", days_in_month(2, 2024));
    printf("days in feb 2023: %d\n", days_in_month(2, 2023));

    struct Date d2;
    d2.year = 2024; d2.month = 12; d2.day = 31;
    date_next_day(&d2);
    printf("after 2024-12-31: "); date_print(&d2); printf("\n");

    struct Date d3;
    d3.year = 2024; d3.month = 2; d3.day = 28;
    date_next_day(&d3);
    printf("after 2024-02-28: "); date_print(&d3); printf("\n");

    struct Date d4;
    d4.year = 2024; d4.month = 1; d4.day = 1;
    printf("days between: %d\n", days_between(&d4, &d1));

    int cmp = date_compare(&d1, &d4);
    printf("compare: %d\n", cmp > 0 ? 1 : (cmp < 0 ? -1 : 0));

    return 0;
}
