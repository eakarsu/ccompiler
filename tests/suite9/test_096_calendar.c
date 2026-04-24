int printf(const char *fmt, ...);

struct Date {
    int year;
    int month;
    int day;
};

int is_leap_year(int y) {
    if (y % 400 == 0) return 1;
    if (y % 100 == 0) return 0;
    if (y % 4 == 0) return 1;
    return 0;
}

int days_in_month(int m, int y) {
    int days[12];
    days[0] = 31; days[1] = 28; days[2] = 31; days[3] = 30;
    days[4] = 31; days[5] = 30; days[6] = 31; days[7] = 31;
    days[8] = 30; days[9] = 31; days[10] = 30; days[11] = 31;
    if (m == 2 && is_leap_year(y)) return 29;
    return days[m - 1];
}

int days_in_year(int y) {
    if (is_leap_year(y)) return 366;
    return 365;
}

int day_of_year(struct Date d) {
    int i;
    int total;
    total = 0;
    for (i = 1; i < d.month; i = i + 1) {
        total = total + days_in_month(i, d.year);
    }
    total = total + d.day;
    return total;
}

int days_remaining(struct Date d) {
    return days_in_year(d.year) - day_of_year(d);
}

struct Date add_days(struct Date d, int n) {
    struct Date result;
    result = d;
    while (n > 0) {
        int dim;
        dim = days_in_month(result.month, result.year);
        if (result.day + n <= dim) {
            result.day = result.day + n;
            n = 0;
        } else {
            n = n - (dim - result.day + 1);
            result.day = 1;
            result.month = result.month + 1;
            if (result.month > 12) {
                result.month = 1;
                result.year = result.year + 1;
            }
        }
    }
    return result;
}

int diff_days(struct Date a, struct Date b) {
    int count;
    struct Date cur;
    count = 0;
    cur = a;
    while (cur.year < b.year || cur.month < b.month || cur.day < b.day) {
        cur = add_days(cur, 1);
        count = count + 1;
    }
    return count;
}

int day_of_week(struct Date d) {
    int y;
    int m;
    int q;
    int k;
    int j;
    int h;
    y = d.year;
    m = d.month;
    q = d.day;
    if (m < 3) {
        m = m + 12;
        y = y - 1;
    }
    k = y % 100;
    j = y / 100;
    h = (q + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    if (h < 0) h = h + 7;
    return h;
}

const char *weekday_name(int h) {
    if (h == 0) return "Saturday";
    if (h == 1) return "Sunday";
    if (h == 2) return "Monday";
    if (h == 3) return "Tuesday";
    if (h == 4) return "Wednesday";
    if (h == 5) return "Thursday";
    if (h == 6) return "Friday";
    return "Unknown";
}

int is_valid_date(struct Date d) {
    if (d.year < 1 || d.month < 1 || d.month > 12) return 0;
    if (d.day < 1 || d.day > days_in_month(d.month, d.year)) return 0;
    return 1;
}

void print_date(struct Date d) {
    printf("%d-%d-%d", d.year, d.month, d.day);
}

int main() {
    struct Date d1;
    struct Date d2;
    struct Date d3;
    struct Date added;
    int dow;
    int diff;

    // EXPECT: === Calendar Date Operations ===
    printf("=== Calendar Date Operations ===\n");

    // EXPECT: Leap year 2000: 1
    printf("Leap year 2000: %d\n", is_leap_year(2000));
    // EXPECT: Leap year 1900: 0
    printf("Leap year 1900: %d\n", is_leap_year(1900));
    // EXPECT: Leap year 2024: 1
    printf("Leap year 2024: %d\n", is_leap_year(2024));
    // EXPECT: Leap year 2023: 0
    printf("Leap year 2023: %d\n", is_leap_year(2023));

    // EXPECT: Feb 2024 days: 29
    printf("Feb 2024 days: %d\n", days_in_month(2, 2024));
    // EXPECT: Feb 2023 days: 28
    printf("Feb 2023 days: %d\n", days_in_month(2, 2023));

    d1.year = 2024; d1.month = 3; d1.day = 15;
    // EXPECT: Day of year 2024-3-15: 75
    printf("Day of year ");
    print_date(d1);
    printf(": %d\n", day_of_year(d1));

    // EXPECT: Days remaining: 291
    printf("Days remaining: %d\n", days_remaining(d1));

    dow = day_of_week(d1);
    // EXPECT: Weekday: Friday
    printf("Weekday: %s\n", weekday_name(dow));

    added = add_days(d1, 30);
    // EXPECT: +30 days: 2024-4-14
    printf("+30 days: ");
    print_date(added);
    printf("\n");

    d2.year = 2024; d2.month = 1; d2.day = 1;
    d3.year = 2024; d3.month = 1; d3.day = 31;
    diff = diff_days(d2, d3);
    // EXPECT: Days between Jan 1 and Jan 31: 30
    printf("Days between Jan 1 and Jan 31: %d\n", diff);

    d2.year = 2024; d2.month = 12; d2.day = 25;
    dow = day_of_week(d2);
    // EXPECT: Christmas 2024: Wednesday
    printf("Christmas 2024: %s\n", weekday_name(dow));

    // EXPECT: Valid 2024-2-29: 1
    d2.year = 2024; d2.month = 2; d2.day = 29;
    printf("Valid 2024-2-29: %d\n", is_valid_date(d2));

    // EXPECT: Valid 2023-2-29: 0
    d2.year = 2023; d2.month = 2; d2.day = 29;
    printf("Valid 2023-2-29: %d\n", is_valid_date(d2));

    added = add_days(d1, 365);
    // EXPECT: +365 days: 2025-3-15
    printf("+365 days: ");
    print_date(added);
    printf("\n");

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
