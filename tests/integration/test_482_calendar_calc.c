int printf(const char *fmt, ...);
// EXPECT: === Calendar Calculations ===\nLeap years:\n  1900: no\n  2000: yes\n  2024: yes\n  1996: yes\n  1800: no\n  2100: no\nDay of year: Jul 4, 2024 = 186\nDay of year: Dec 31, 2024 = 366\nDays between Jan 1, 2024 and Dec 31, 2024 = 365\nDays between Jan 1, 2020 and Jan 1, 2024 = 1461\nDay of week:\n  Jan 1, 2024 = Mon\n  Jul 4, 2024 = Thu\n  Dec 25, 2024 = Wed\nMonths and days in 2024:\n  Jan: 31 days\n  Feb: 29 days\n  Mar: 31 days\n  Apr: 30 days\n  May: 31 days\n  Jun: 30 days\n  Jul: 31 days\n  Aug: 31 days\n  Sep: 30 days\n  Oct: 31 days\n  Nov: 30 days\n  Dec: 31 days

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
    if (month >= 1 && month <= 12) return days[month - 1];
    return 0;
}

int day_of_year(int day, int month, int year) {
    int total = 0;
    int m;
    for (m = 1; m < month; m++) {
        total = total + days_in_month(m, year);
    }
    return total + day;
}

int days_between(int d1, int m1, int y1, int d2, int m2, int y2) {
    int total = 0;
    if (y1 == y2) {
        return day_of_year(d2, m2, y2) - day_of_year(d1, m1, y1);
    }
    int days_in_y1 = is_leap_year(y1) ? 366 : 365;
    total = days_in_y1 - day_of_year(d1, m1, y1);
    int y;
    for (y = y1 + 1; y < y2; y++) {
        total = total + (is_leap_year(y) ? 366 : 365);
    }
    total = total + day_of_year(d2, m2, y2);
    return total;
}

int day_of_week(int day, int month, int year) {
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12 * a - 2;
    return (day + (31 * m) / 12 + y + y / 4 - y / 100 + y / 400) % 7;
}

const char *dow_name(int dow) {
    if (dow == 0) return "Sun";
    if (dow == 1) return "Mon";
    if (dow == 2) return "Tue";
    if (dow == 3) return "Wed";
    if (dow == 4) return "Thu";
    if (dow == 5) return "Fri";
    return "Sat";
}

const char *month_name(int m) {
    if (m == 1) return "Jan";
    if (m == 2) return "Feb";
    if (m == 3) return "Mar";
    if (m == 4) return "Apr";
    if (m == 5) return "May";
    if (m == 6) return "Jun";
    if (m == 7) return "Jul";
    if (m == 8) return "Aug";
    if (m == 9) return "Sep";
    if (m == 10) return "Oct";
    if (m == 11) return "Nov";
    return "Dec";
}

int main(void) {
    printf("=== Calendar Calculations ===\n");

    int test_years[6];
    test_years[0] = 1900; test_years[1] = 2000; test_years[2] = 2024;
    test_years[3] = 1996; test_years[4] = 1800; test_years[5] = 2100;

    int i;
    printf("Leap years:\n");
    for (i = 0; i < 6; i++) {
        printf("  %d: %s\n", test_years[i], is_leap_year(test_years[i]) ? "yes" : "no");
    }

    printf("Day of year: Jul 4, 2024 = %d\n", day_of_year(4, 7, 2024));
    printf("Day of year: Dec 31, 2024 = %d\n", day_of_year(31, 12, 2024));

    printf("Days between Jan 1, 2024 and Dec 31, 2024 = %d\n",
           days_between(1, 1, 2024, 31, 12, 2024));
    printf("Days between Jan 1, 2020 and Jan 1, 2024 = %d\n",
           days_between(1, 1, 2020, 1, 1, 2024));

    printf("Day of week:\n");
    printf("  Jan 1, 2024 = %s\n", dow_name(day_of_week(1, 1, 2024)));
    printf("  Jul 4, 2024 = %s\n", dow_name(day_of_week(4, 7, 2024)));
    printf("  Dec 25, 2024 = %s\n", dow_name(day_of_week(25, 12, 2024)));

    printf("Months and days in 2024:\n");
    int m;
    for (m = 1; m <= 12; m++) {
        printf("  %s: %d days\n", month_name(m), days_in_month(m, 2024));
    }

    return 0;
}
