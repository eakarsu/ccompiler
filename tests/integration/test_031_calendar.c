int printf(const char *fmt, ...);
// EXPECT: --- Leap Year ---\n2000 leap: 1\n1900 leap: 0\n2024 leap: 1\n2023 leap: 0\n2100 leap: 0\n1600 leap: 1\n2004 leap: 1\n1999 leap: 0\n--- Days in Month ---\n2024-01: 31\n2024-02: 29\n2023-02: 28\n2024-03: 31\n2024-04: 30\n2024-05: 31\n2024-06: 30\n2024-07: 31\n2024-08: 31\n2024-09: 30\n2024-10: 31\n2024-11: 30\n2024-12: 31\n--- Day of Year ---\n2024-01-01: day 1\n2024-02-01: day 32\n2024-03-01: day 61\n2023-03-01: day 60\n2024-12-31: day 366\n2023-12-31: day 365\n2024-06-15: day 167\n--- Day of Week (Zeller) ---\n2024-01-01 dow: 1\n2024-07-04 dow: 4\n2023-12-25 dow: 1\n2000-01-01 dow: 6\n2024-02-29 dow: 4\n1970-01-01 dow: 4\n--- Day Name ---\n2024-01-01: Monday\n2024-07-04: Thursday\n2023-12-25: Monday\n2000-01-01: Saturday\n2024-02-29: Thursday\n1970-01-01: Thursday\n--- Julian Day Number ---\nJDN 2000-01-01: 2451545\nJDN 2024-01-01: 2460311\nJDN 1970-01-01: 2440588\n--- Date Difference ---\n2024-01-01 to 2024-01-31: 30 days\n2024-01-01 to 2024-12-31: 365 days\n2023-01-01 to 2024-01-01: 365 days\n2024-02-28 to 2024-03-01: 2 days\n2023-02-28 to 2023-03-01: 1 days\n2020-01-01 to 2024-01-01: 1461 days\n--- Calendar Grid Feb 2024 ---\nFebruary 2024\nMo Tu We Th Fr Sa Su\n          1  2  3  4\n 5  6  7  8  9 10 11\n12 13 14 15 16 17 18\n19 20 21 22 23 24 25\n26 27 28 29\n--- Calendar Grid Jan 2024 ---\nJanuary 2024\nMo Tu We Th Fr Sa Su\n 1  2  3  4  5  6  7\n 8  9 10 11 12 13 14\n15 16 17 18 19 20 21\n22 23 24 25 26 27 28\n29 30 31\n--- Valid Date Check ---\n2024-02-29 valid: 1\n2023-02-29 valid: 0\n2024-13-01 valid: 0\n2024-00-01 valid: 0\n2024-01-00 valid: 0\n2024-01-32 valid: 0\n2024-04-31 valid: 0\n2024-12-31 valid: 1\n--- Add Days ---\n2024-01-01 + 30 = 2024-1-31\n2024-01-01 + 31 = 2024-2-1\n2024-02-28 + 1 = 2024-2-29\n2023-02-28 + 1 = 2023-3-1\n2024-12-31 + 1 = 2025-1-1\n2024-01-01 + 366 = 2025-1-1\n--- Days Until End of Year ---\n2024-01-01: 365 days left\n2024-12-31: 0 days left\n2024-06-15: 199 days left\n2023-06-15: 199 days left\n--- All calendar tests passed ---

// ========== Leap year ==========

int is_leap_year(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

void test_leap_year() {
    printf("--- Leap Year ---\n");
    printf("2000 leap: %d\n", is_leap_year(2000));
    printf("1900 leap: %d\n", is_leap_year(1900));
    printf("2024 leap: %d\n", is_leap_year(2024));
    printf("2023 leap: %d\n", is_leap_year(2023));
    printf("2100 leap: %d\n", is_leap_year(2100));
    printf("1600 leap: %d\n", is_leap_year(1600));
    printf("2004 leap: %d\n", is_leap_year(2004));
    printf("1999 leap: %d\n", is_leap_year(1999));
}

// ========== Days in month ==========

int days_in_month(int year, int month) {
    if (month == 2) {
        if (is_leap_year(year)) return 29;
        return 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

void test_days_in_month() {
    int m;
    printf("--- Days in Month ---\n");
    for (m = 1; m <= 12; m = m + 1) {
        if (m == 2) {
            printf("2024-02: %d\n", days_in_month(2024, 2));
            printf("2023-02: %d\n", days_in_month(2023, 2));
        } else {
            printf("2024-%c%c: %d\n", '0' + m / 10, '0' + m % 10,
                   days_in_month(2024, m));
        }
    }
}

// ========== Day of year ==========

int day_of_year(int year, int month, int day) {
    int m;
    int total = 0;
    for (m = 1; m < month; m = m + 1) {
        total = total + days_in_month(year, m);
    }
    total = total + day;
    return total;
}

void test_day_of_year() {
    printf("--- Day of Year ---\n");
    printf("2024-01-01: day %d\n", day_of_year(2024, 1, 1));
    printf("2024-02-01: day %d\n", day_of_year(2024, 2, 1));
    printf("2024-03-01: day %d\n", day_of_year(2024, 3, 1));
    printf("2023-03-01: day %d\n", day_of_year(2023, 3, 1));
    printf("2024-12-31: day %d\n", day_of_year(2024, 12, 31));
    printf("2023-12-31: day %d\n", day_of_year(2023, 12, 31));
    printf("2024-06-15: day %d\n", day_of_year(2024, 6, 15));
}

// ========== Day of week (Zeller's congruence, modified) ==========
// Returns 0=Sunday, 1=Monday, ..., 6=Saturday

int day_of_week(int year, int month, int day) {
    int q;
    int m;
    int k;
    int j;
    int h;

    q = day;
    // Zeller: treat Jan and Feb as months 13,14 of previous year
    if (month < 3) {
        month = month + 12;
        year = year - 1;
    }
    m = month;
    k = year % 100;
    j = year / 100;

    h = (q + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    // h: 0=Saturday, 1=Sunday, 2=Monday, ..., 6=Friday
    // Convert to: 0=Sunday, 1=Monday, ..., 6=Saturday
    h = ((h + 6) % 7);
    // Now: 0=Saturday, 1=Sunday, ... no wait, let me be more careful.
    // Original Zeller: h=0 means Saturday, h=1 means Sunday, etc.
    // So (h+6)%7: if h=0 -> 6 (Saturday), h=1 -> 0 (Sunday), h=2 -> 1 (Monday)
    // That gives us: 0=Sunday, 1=Monday, ..., 6=Saturday
    return h;
}

void test_day_of_week() {
    printf("--- Day of Week (Zeller) ---\n");
    // 2024-01-01 is Monday (1)
    printf("2024-01-01 dow: %d\n", day_of_week(2024, 1, 1));
    // 2024-07-04 is Thursday (4)
    printf("2024-07-04 dow: %d\n", day_of_week(2024, 7, 4));
    // 2023-12-25 is Monday (1)
    printf("2023-12-25 dow: %d\n", day_of_week(2023, 12, 25));
    // 2000-01-01 is Saturday (6)
    printf("2000-01-01 dow: %d\n", day_of_week(2000, 1, 1));
    // 2024-02-29 is Thursday (4)
    printf("2024-02-29 dow: %d\n", day_of_week(2024, 2, 29));
    // 1970-01-01 is Thursday (4)
    printf("1970-01-01 dow: %d\n", day_of_week(1970, 1, 1));
}

// ========== Day name ==========

void print_day_name(int dow) {
    if (dow == 0) printf("Sunday");
    else if (dow == 1) printf("Monday");
    else if (dow == 2) printf("Tuesday");
    else if (dow == 3) printf("Wednesday");
    else if (dow == 4) printf("Thursday");
    else if (dow == 5) printf("Friday");
    else if (dow == 6) printf("Saturday");
}

void test_day_name() {
    printf("--- Day Name ---\n");

    printf("2024-01-01: ");
    print_day_name(day_of_week(2024, 1, 1));
    printf("\n");

    printf("2024-07-04: ");
    print_day_name(day_of_week(2024, 7, 4));
    printf("\n");

    printf("2023-12-25: ");
    print_day_name(day_of_week(2023, 12, 25));
    printf("\n");

    printf("2000-01-01: ");
    print_day_name(day_of_week(2000, 1, 1));
    printf("\n");

    printf("2024-02-29: ");
    print_day_name(day_of_week(2024, 2, 29));
    printf("\n");

    printf("1970-01-01: ");
    print_day_name(day_of_week(1970, 1, 1));
    printf("\n");
}

// ========== Julian Day Number ==========

int julian_day_number(int year, int month, int day) {
    int a;
    int y;
    int m;
    int jdn;

    a = (14 - month) / 12;
    y = year + 4800 - a;
    m = month + 12 * a - 3;
    jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    return jdn;
}

void test_julian_day() {
    printf("--- Julian Day Number ---\n");
    printf("JDN 2000-01-01: %d\n", julian_day_number(2000, 1, 1));
    printf("JDN 2024-01-01: %d\n", julian_day_number(2024, 1, 1));
    printf("JDN 1970-01-01: %d\n", julian_day_number(1970, 1, 1));
}

// ========== Date difference ==========

int date_diff(int y1, int m1, int d1, int y2, int m2, int d2) {
    int jdn1 = julian_day_number(y1, m1, d1);
    int jdn2 = julian_day_number(y2, m2, d2);
    int diff = jdn2 - jdn1;
    if (diff < 0) diff = -diff;
    return diff;
}

void test_date_diff() {
    printf("--- Date Difference ---\n");
    printf("2024-01-01 to 2024-01-31: %d days\n",
           date_diff(2024, 1, 1, 2024, 1, 31));
    printf("2024-01-01 to 2024-12-31: %d days\n",
           date_diff(2024, 1, 1, 2024, 12, 31));
    printf("2023-01-01 to 2024-01-01: %d days\n",
           date_diff(2023, 1, 1, 2024, 1, 1));
    printf("2024-02-28 to 2024-03-01: %d days\n",
           date_diff(2024, 2, 28, 2024, 3, 1));
    printf("2023-02-28 to 2023-03-01: %d days\n",
           date_diff(2023, 2, 28, 2023, 3, 1));
    printf("2020-01-01 to 2024-01-01: %d days\n",
           date_diff(2020, 1, 1, 2024, 1, 1));
}

// ========== Calendar grid ==========

void print_month_name(int month) {
    if (month == 1) printf("January");
    else if (month == 2) printf("February");
    else if (month == 3) printf("March");
    else if (month == 4) printf("April");
    else if (month == 5) printf("May");
    else if (month == 6) printf("June");
    else if (month == 7) printf("July");
    else if (month == 8) printf("August");
    else if (month == 9) printf("September");
    else if (month == 10) printf("October");
    else if (month == 11) printf("November");
    else if (month == 12) printf("December");
}

void print_calendar(int year, int month) {
    int dow;
    int dim;
    int d;
    int col;

    print_month_name(month);
    printf(" %d\n", year);
    printf("Mo Tu We Th Fr Sa Su\n");

    // day_of_week returns 0=Sun, 1=Mon, ..., 6=Sat
    // We want Monday=column 0, Sunday=column 6
    dow = day_of_week(year, month, 1);
    // Convert: 0(Sun)->6, 1(Mon)->0, 2(Tue)->1, ..., 6(Sat)->5
    if (dow == 0) col = 6;
    else col = dow - 1;

    dim = days_in_month(year, month);

    // Print leading spaces
    {
        int i;
        for (i = 0; i < col; i = i + 1) {
            printf("   ");
        }
    }

    for (d = 1; d <= dim; d = d + 1) {
        printf("%2d", d);
        col = col + 1;
        if (col == 7) {
            printf("\n");
            col = 0;
        } else if (d < dim) {
            printf(" ");
        }
    }
    if (col != 0) printf("\n");
}

void test_calendar_grid() {
    printf("--- Calendar Grid Feb 2024 ---\n");
    print_calendar(2024, 2);

    printf("--- Calendar Grid Jan 2024 ---\n");
    print_calendar(2024, 1);
}

// ========== Valid date check ==========

int is_valid_date(int year, int month, int day) {
    int dim;
    if (month < 1 || month > 12) return 0;
    if (day < 1) return 0;
    dim = days_in_month(year, month);
    if (day > dim) return 0;
    return 1;
}

void test_valid_date() {
    printf("--- Valid Date Check ---\n");
    printf("2024-02-29 valid: %d\n", is_valid_date(2024, 2, 29));
    printf("2023-02-29 valid: %d\n", is_valid_date(2023, 2, 29));
    printf("2024-13-01 valid: %d\n", is_valid_date(2024, 13, 1));
    printf("2024-00-01 valid: %d\n", is_valid_date(2024, 0, 1));
    printf("2024-01-00 valid: %d\n", is_valid_date(2024, 1, 0));
    printf("2024-01-32 valid: %d\n", is_valid_date(2024, 1, 32));
    printf("2024-04-31 valid: %d\n", is_valid_date(2024, 4, 31));
    printf("2024-12-31 valid: %d\n", is_valid_date(2024, 12, 31));
}

// ========== Add days to date ==========

void add_days(int year, int month, int day, int n,
              int *ry, int *rm, int *rd) {
    int dim;
    day = day + n;
    while (1) {
        dim = days_in_month(year, month);
        if (day <= dim) break;
        day = day - dim;
        month = month + 1;
        if (month > 12) {
            month = 1;
            year = year + 1;
        }
    }
    *ry = year;
    *rm = month;
    *rd = day;
}

void test_add_days() {
    int ry;
    int rm;
    int rd;

    printf("--- Add Days ---\n");

    add_days(2024, 1, 1, 30, &ry, &rm, &rd);
    printf("2024-01-01 + 30 = %d-%d-%d\n", ry, rm, rd);

    add_days(2024, 1, 1, 31, &ry, &rm, &rd);
    printf("2024-01-01 + 31 = %d-%d-%d\n", ry, rm, rd);

    add_days(2024, 2, 28, 1, &ry, &rm, &rd);
    printf("2024-02-28 + 1 = %d-%d-%d\n", ry, rm, rd);

    add_days(2023, 2, 28, 1, &ry, &rm, &rd);
    printf("2023-02-28 + 1 = %d-%d-%d\n", ry, rm, rd);

    add_days(2024, 12, 31, 1, &ry, &rm, &rd);
    printf("2024-12-31 + 1 = %d-%d-%d\n", ry, rm, rd);

    add_days(2024, 1, 1, 366, &ry, &rm, &rd);
    printf("2024-01-01 + 366 = %d-%d-%d\n", ry, rm, rd);
}

// ========== Days until end of year ==========

int days_until_eoy(int year, int month, int day) {
    int total_days;
    if (is_leap_year(year)) {
        total_days = 366;
    } else {
        total_days = 365;
    }
    return total_days - day_of_year(year, month, day);
}

void test_days_until_eoy() {
    printf("--- Days Until End of Year ---\n");
    printf("2024-01-01: %d days left\n", days_until_eoy(2024, 1, 1));
    printf("2024-12-31: %d days left\n", days_until_eoy(2024, 12, 31));
    printf("2024-06-15: %d days left\n", days_until_eoy(2024, 6, 15));
    printf("2023-06-15: %d days left\n", days_until_eoy(2023, 6, 15));
}

int main() {
    test_leap_year();
    test_days_in_month();
    test_day_of_year();
    test_day_of_week();
    test_day_name();
    test_julian_day();
    test_date_diff();
    test_calendar_grid();
    test_valid_date();
    test_add_days();
    test_days_until_eoy();
    printf("--- All calendar tests passed ---\n");
    return 0;
}
