int printf(const char *fmt, ...);

// Table scan with filter predicates

struct Row {
    int id;
    int name_code;  // encoded name (1=Alice, 2=Bob, 3=Charlie, 4=Dave, 5=Eve)
    int age;
    int salary;
    int dept;       // 1=Engineering, 2=Sales, 3=Marketing
};

struct Table {
    struct Row rows[20];
    int nrows;
};

void table_add(struct Table *t, int id, int nc, int age, int sal, int dept) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].name_code = nc;
    t->rows[t->nrows].age = age;
    t->rows[t->nrows].salary = sal;
    t->rows[t->nrows].dept = dept;
    t->nrows++;
}

// Filter predicates as function pointers
typedef int (*Predicate)(struct Row *r);

int filter_age_over_30(struct Row *r) {
    return r->age > 30;
}

int filter_salary_over_60k(struct Row *r) {
    return r->salary > 60000;
}

int filter_dept_engineering(struct Row *r) {
    return r->dept == 1;
}

int filter_dept_sales(struct Row *r) {
    return r->dept == 2;
}

struct ResultSet {
    int indices[20];
    int count;
};

// Full table scan with a single filter
void table_scan(struct Table *t, Predicate pred, struct ResultSet *rs) {
    rs->count = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (pred(&t->rows[i])) {
            rs->indices[rs->count] = i;
            rs->count++;
        }
    }
}

// Table scan with AND of two filters
void table_scan_and(struct Table *t, Predicate p1, Predicate p2, struct ResultSet *rs) {
    rs->count = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (p1(&t->rows[i]) && p2(&t->rows[i])) {
            rs->indices[rs->count] = i;
            rs->count++;
        }
    }
}

// Table scan with OR of two filters
void table_scan_or(struct Table *t, Predicate p1, Predicate p2, struct ResultSet *rs) {
    rs->count = 0;
    int i;
    for (i = 0; i < t->nrows; i++) {
        if (p1(&t->rows[i]) || p2(&t->rows[i])) {
            rs->indices[rs->count] = i;
            rs->count++;
        }
    }
}

char *name_of(int code) {
    if (code == 1) return "Alice";
    if (code == 2) return "Bob";
    if (code == 3) return "Charlie";
    if (code == 4) return "Dave";
    if (code == 5) return "Eve";
    return "Unknown";
}

char *dept_name(int d) {
    if (d == 1) return "Engineering";
    if (d == 2) return "Sales";
    if (d == 3) return "Marketing";
    return "Unknown";
}

int main() {
    struct Table t;
    t.nrows = 0;

    table_add(&t, 1, 1, 28, 55000, 1);  // Alice, 28, 55k, Engineering
    table_add(&t, 2, 2, 35, 72000, 2);  // Bob, 35, 72k, Sales
    table_add(&t, 3, 3, 42, 85000, 1);  // Charlie, 42, 85k, Engineering
    table_add(&t, 4, 4, 31, 48000, 3);  // Dave, 31, 48k, Marketing
    table_add(&t, 5, 5, 26, 62000, 2);  // Eve, 26, 62k, Sales
    table_add(&t, 6, 1, 38, 91000, 1);  // Alice2->Alice, 38, 91k, Eng
    table_add(&t, 7, 2, 29, 53000, 3);  // Bob2->Bob, 29, 53k, Marketing
    table_add(&t, 8, 3, 45, 78000, 2);  // Charlie2->Charlie, 45, 78k, Sales

    printf("Table loaded: %d rows\n", t.nrows); // EXPECT: Table loaded: 8 rows

    // Scan: age > 30
    struct ResultSet rs;
    table_scan(&t, filter_age_over_30, &rs);
    printf("Age > 30: %d rows\n", rs.count); // EXPECT: Age > 30: 5 rows
    int i;
    for (i = 0; i < rs.count; i++) {
        int idx = rs.indices[i];
        printf("  id=%d age=%d\n", t.rows[idx].id, t.rows[idx].age);
    }
    // EXPECT:   id=2 age=35
    // EXPECT:   id=3 age=42
    // EXPECT:   id=4 age=31
    // EXPECT:   id=6 age=38
    // EXPECT:   id=8 age=45

    // Scan: salary > 60000
    struct ResultSet rs2;
    table_scan(&t, filter_salary_over_60k, &rs2);
    printf("Salary > 60000: %d rows\n", rs2.count); // EXPECT: Salary > 60000: 5 rows

    // AND: Engineering AND age > 30
    struct ResultSet rs3;
    table_scan_and(&t, filter_dept_engineering, filter_age_over_30, &rs3);
    printf("Engineering AND age>30: %d rows\n", rs3.count); // EXPECT: Engineering AND age>30: 2 rows
    for (i = 0; i < rs3.count; i++) {
        int idx = rs3.indices[i];
        printf("  %s dept=%s\n", name_of(t.rows[idx].name_code), dept_name(t.rows[idx].dept));
    }
    // EXPECT:   Charlie dept=Engineering
    // EXPECT:   Alice dept=Engineering

    // OR: Engineering OR Sales
    struct ResultSet rs4;
    table_scan_or(&t, filter_dept_engineering, filter_dept_sales, &rs4);
    printf("Engineering OR Sales: %d rows\n", rs4.count); // EXPECT: Engineering OR Sales: 6 rows

    // Count by department manually
    int eng_count = 0;
    int sales_count = 0;
    int mkt_count = 0;
    for (i = 0; i < t.nrows; i++) {
        if (t.rows[i].dept == 1) eng_count++;
        if (t.rows[i].dept == 2) sales_count++;
        if (t.rows[i].dept == 3) mkt_count++;
    }
    printf("Engineering: %d, Sales: %d, Marketing: %d\n", eng_count, sales_count, mkt_count);
    // EXPECT: Engineering: 3, Sales: 3, Marketing: 2

    printf("Table scan test passed\n"); // EXPECT: Table scan test passed
    return 0;
}
