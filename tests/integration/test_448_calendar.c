int printf(const char *fmt, ...);
// EXPECT: Leap years:\n  2000: 1\n  1900: 0\n  2024: 1\n  2023: 0\n\nDays in month:\n  Feb 2024: 29\n  Feb 2023: 28\n  Jan: 31\n  Apr: 30\n\nDay of year:\n  Jan 1: 1\n  Mar 1 (leap): 61\n  Dec 31 (leap): 366\n  Dec 31 (non-leap): 365\n\nDay of week:\n  Jan 1, 2024: Mon\n  Jul 4, 1776: Thu\n  Dec 25, 2025: Thu\n\nDays between:\n  Jan 1 - Dec 31, 2024: 365\n  Jan 1 - Jan 1, 2025: 366
// Test: Calendar computations

int is_leap_year(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

int days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) return 29;
    return days[month - 1];
}

int day_of_year(int day, int month, int year) {
    int total = 0;
    int m;
    for (m = 1; m < month; m++) {
        total = total + days_in_month(m, year);
    }
    return total + day;
}

int day_of_week(int day, int month, int year) {
    int y = year;
    int m = month;
    if (m < 3) {
        m = m + 12;
        y = y - 1;
    }
    int k = y % 100;
    int j = y / 100;
    int h = (day + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    if (h < 0) h = h + 7;
    return h;
}

int days_between(int d1, int m1, int y1, int d2, int m2, int y2) {
    int total1 = 0;
    int total2 = 0;
    int y;

    for (y = 1; y < y1; y++) {
        total1 = total1 + (is_leap_year(y) ? 366 : 365);
    }
    total1 = total1 + day_of_year(d1, m1, y1);

    for (y = 1; y < y2; y++) {
        total2 = total2 + (is_leap_year(y) ? 366 : 365);
    }
    total2 = total2 + day_of_year(d2, m2, y2);

    int diff = total2 - total1;
    if (diff < 0) diff = -diff;
    return diff;
}

int main(void) {
    char *day_names[] = {"Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"};

    printf("Leap years:\n");
    printf("  2000: %d\n", is_leap_year(2000));
    printf("  1900: %d\n", is_leap_year(1900));
    printf("  2024: %d\n", is_leap_year(2024));
    printf("  2023: %d\n", is_leap_year(2023));

    printf("\nDays in month:\n");
    printf("  Feb 2024: %d\n", days_in_month(2, 2024));
    printf("  Feb 2023: %d\n", days_in_month(2, 2023));
    printf("  Jan: %d\n", days_in_month(1, 2024));
    printf("  Apr: %d\n", days_in_month(4, 2024));

    printf("\nDay of year:\n");
    printf("  Jan 1: %d\n", day_of_year(1, 1, 2024));
    printf("  Mar 1 (leap): %d\n", day_of_year(1, 3, 2024));
    printf("  Dec 31 (leap): %d\n", day_of_year(31, 12, 2024));
    printf("  Dec 31 (non-leap): %d\n", day_of_year(31, 12, 2023));

    printf("\nDay of week:\n");
    int dow = day_of_week(1, 1, 2024);
    printf("  Jan 1, 2024: %s\n", day_names[dow]);
    dow = day_of_week(4, 7, 1776);
    printf("  Jul 4, 1776: %s\n", day_names[dow]);
    dow = day_of_week(25, 12, 2025);
    printf("  Dec 25, 2025: %s\n", day_names[dow]);

    printf("\nDays between:\n");
    printf("  Jan 1 - Dec 31, 2024: %d\n", days_between(1,1,2024, 31,12,2024));
    printf("  Jan 1 - Jan 1, 2025: %d\n", days_between(1,1,2024, 1,1,2025));

    return 0;
}
