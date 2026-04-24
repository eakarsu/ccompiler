// EXPECT: === Calendar Computation Tests ===
// EXPECT: Leap year 2000: yes
// EXPECT: Leap year 1900: no
// EXPECT: Leap year 2024: yes
// EXPECT: Leap year 2023: no
// EXPECT: Leap year 2100: no
// EXPECT: Leap year 1600: yes
// EXPECT: Days in Jan 2023: 31
// EXPECT: Days in Feb 2024: 29
// EXPECT: Days in Feb 2023: 28
// EXPECT: Days in Apr 2023: 30
// EXPECT: Days in Dec 2023: 31
// EXPECT: Day of week 2024-01-01: Monday
// EXPECT: Day of week 2024-07-04: Thursday
// EXPECT: Day of week 2000-01-01: Saturday
// EXPECT: Day of week 1999-12-31: Friday
// EXPECT: Day of week 2023-12-25: Monday
// EXPECT: Day of year 2024-01-01: 1
// EXPECT: Day of year 2024-03-01: 61
// EXPECT: Day of year 2023-03-01: 60
// EXPECT: Day of year 2024-12-31: 366
// EXPECT: Day of year 2023-12-31: 365
// EXPECT: Valid 2024-02-29: yes
// EXPECT: Valid 2023-02-29: no
// EXPECT: Valid 2023-13-01: no
// EXPECT: Valid 2023-00-15: no
// EXPECT: Valid 2023-06-31: no
// EXPECT: Valid 2023-06-30: yes
// EXPECT: All calendar tests passed!

int printf(const char *fmt, ...);

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
    if (month < 1 || month > 12) return 0;
    int d = days[month - 1];
    if (month == 2 && is_leap_year(year)) {
        d = 29;
    }
    return d;
}

// Zeller's congruence for day of week
// Returns 0=Sunday, 1=Monday, ... 6=Saturday
int day_of_week(int year, int month, int day) {
    int m = month;
    int y = year;
    if (m < 3) {
        m = m + 12;
        y = y - 1;
    }
    int k = y % 100;
    int j = y / 100;
    int h = (day + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    // h can be negative
    if (h < 0) h = h + 7;
    // Convert from Zeller (0=Sat) to 0=Sunday
    int dow = ((h + 6) % 7);
    return dow;
}

char *day_name(int dow) {
    if (dow == 0) return "Sunday";
    if (dow == 1) return "Monday";
    if (dow == 2) return "Tuesday";
    if (dow == 3) return "Wednesday";
    if (dow == 4) return "Thursday";
    if (dow == 5) return "Friday";
    if (dow == 6) return "Saturday";
    return "Unknown";
}

int day_of_year(int year, int month, int day) {
    int total = 0;
    int m;
    for (m = 1; m < month; m++) {
        total = total + days_in_month(m, year);
    }
    total = total + day;
    return total;
}

int is_valid_date(int year, int month, int day) {
    if (month < 1 || month > 12) return 0;
    if (day < 1) return 0;
    int max_day = days_in_month(month, year);
    if (day > max_day) return 0;
    return 1;
}

int main() {
    printf("=== Calendar Computation Tests ===\n");

    // Leap year tests
    printf("Leap year 2000: %s\n", is_leap_year(2000) ? "yes" : "no");
    printf("Leap year 1900: %s\n", is_leap_year(1900) ? "yes" : "no");
    printf("Leap year 2024: %s\n", is_leap_year(2024) ? "yes" : "no");
    printf("Leap year 2023: %s\n", is_leap_year(2023) ? "yes" : "no");
    printf("Leap year 2100: %s\n", is_leap_year(2100) ? "yes" : "no");
    printf("Leap year 1600: %s\n", is_leap_year(1600) ? "yes" : "no");

    // Days in month tests
    printf("Days in Jan 2023: %d\n", days_in_month(1, 2023));
    printf("Days in Feb 2024: %d\n", days_in_month(2, 2024));
    printf("Days in Feb 2023: %d\n", days_in_month(2, 2023));
    printf("Days in Apr 2023: %d\n", days_in_month(4, 2023));
    printf("Days in Dec 2023: %d\n", days_in_month(12, 2023));

    // Day of week tests
    printf("Day of week 2024-01-01: %s\n", day_name(day_of_week(2024, 1, 1)));
    printf("Day of week 2024-07-04: %s\n", day_name(day_of_week(2024, 7, 4)));
    printf("Day of week 2000-01-01: %s\n", day_name(day_of_week(2000, 1, 1)));
    printf("Day of week 1999-12-31: %s\n", day_name(day_of_week(1999, 12, 31)));
    printf("Day of week 2023-12-25: %s\n", day_name(day_of_week(2023, 12, 25)));

    // Day of year tests
    printf("Day of year 2024-01-01: %d\n", day_of_year(2024, 1, 1));
    printf("Day of year 2024-03-01: %d\n", day_of_year(2024, 3, 1));
    printf("Day of year 2023-03-01: %d\n", day_of_year(2023, 3, 1));
    printf("Day of year 2024-12-31: %d\n", day_of_year(2024, 12, 31));
    printf("Day of year 2023-12-31: %d\n", day_of_year(2023, 12, 31));

    // Date validation
    printf("Valid 2024-02-29: %s\n", is_valid_date(2024, 2, 29) ? "yes" : "no");
    printf("Valid 2023-02-29: %s\n", is_valid_date(2023, 2, 29) ? "yes" : "no");
    printf("Valid 2023-13-01: %s\n", is_valid_date(2023, 13, 1) ? "yes" : "no");
    printf("Valid 2023-00-15: %s\n", is_valid_date(2023, 0, 15) ? "yes" : "no");
    printf("Valid 2023-06-31: %s\n", is_valid_date(2023, 6, 31) ? "yes" : "no");
    printf("Valid 2023-06-30: %s\n", is_valid_date(2023, 6, 30) ? "yes" : "no");

    printf("All calendar tests passed!\n");
    return 0;
}
