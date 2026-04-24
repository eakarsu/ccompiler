int printf(const char *fmt, ...);
// EXPECT: === Employee Report ===\nName       Dept     Salary Years Rating\n----------------------------------------\nAlice      Eng       85000     5      9\nBob        Eng       92000     8      8\nCharlie    Sales     65000     3      7\nDiana      Sales     70000     6      9\nEve        Eng       78000     2      8\nFrank      HR        60000     4      7\n----------------------------------------\n\n=== Summary ===\nTotal employees: 6\nAvg salary: 75000\nMax salary: 92000\nMin salary: 60000\nAvg years: 4\nAvg rating: 8\n\n=== High Performers (rating >= 9) ===\n  Alice (Eng)\n  Diana (Sales)\n\n=== Senior (years >= 5) ===\n  Alice: 5 years\n  Bob: 8 years\n  Diana: 6 years
#define MAX_EMPLOYEES 6

struct Employee {
    char *name;
    char *dept;
    int salary;
    int years;
    int rating;
};

void print_separator(int width) {
    int i;
    for (i = 0; i < width; i++) printf("-");
    printf("\n");
}

void print_header(void) {
    printf("%-10s %-8s %6s %5s %6s\n", "Name", "Dept", "Salary", "Years", "Rating");
    print_separator(40);
}

void print_employee(struct Employee *e) {
    printf("%-10s %-8s %6d %5d %6d\n",
           e->name, e->dept, e->salary, e->years, e->rating);
}

int main(void) {
    struct Employee emps[MAX_EMPLOYEES];
    emps[0].name = "Alice";   emps[0].dept = "Eng";   emps[0].salary = 85000; emps[0].years = 5; emps[0].rating = 9;
    emps[1].name = "Bob";     emps[1].dept = "Eng";   emps[1].salary = 92000; emps[1].years = 8; emps[1].rating = 8;
    emps[2].name = "Charlie"; emps[2].dept = "Sales"; emps[2].salary = 65000; emps[2].years = 3; emps[2].rating = 7;
    emps[3].name = "Diana";   emps[3].dept = "Sales"; emps[3].salary = 70000; emps[3].years = 6; emps[3].rating = 9;
    emps[4].name = "Eve";     emps[4].dept = "Eng";   emps[4].salary = 78000; emps[4].years = 2; emps[4].rating = 8;
    emps[5].name = "Frank";   emps[5].dept = "HR";    emps[5].salary = 60000; emps[5].years = 4; emps[5].rating = 7;

    int n = MAX_EMPLOYEES;

    printf("=== Employee Report ===\n");
    print_header();
    int i;
    for (i = 0; i < n; i++) {
        print_employee(&emps[i]);
    }
    print_separator(40);

    int total_salary = 0;
    int total_years = 0;
    int total_rating = 0;
    int max_salary = 0;
    int min_salary = 999999;
    for (i = 0; i < n; i++) {
        total_salary = total_salary + emps[i].salary;
        total_years = total_years + emps[i].years;
        total_rating = total_rating + emps[i].rating;
        if (emps[i].salary > max_salary) max_salary = emps[i].salary;
        if (emps[i].salary < min_salary) min_salary = emps[i].salary;
    }

    printf("\n=== Summary ===\n");
    printf("Total employees: %d\n", n);
    printf("Avg salary: %d\n", total_salary / n);
    printf("Max salary: %d\n", max_salary);
    printf("Min salary: %d\n", min_salary);
    printf("Avg years: %d\n", total_years / n);
    printf("Avg rating: %d\n", total_rating / n);

    printf("\n=== High Performers (rating >= 9) ===\n");
    for (i = 0; i < n; i++) {
        if (emps[i].rating >= 9) {
            printf("  %s (%s)\n", emps[i].name, emps[i].dept);
        }
    }

    printf("\n=== Senior (years >= 5) ===\n");
    for (i = 0; i < n; i++) {
        if (emps[i].years >= 5) {
            printf("  %s: %d years\n", emps[i].name, emps[i].years);
        }
    }

    return 0;
}
