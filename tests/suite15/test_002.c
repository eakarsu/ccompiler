int printf(const char *fmt, ...);

/* test_002: WHERE clause filtering on integer columns */

typedef struct {
    int id;
    int salary;
    int dept;
    int active;
} Employee;

typedef struct {
    Employee rows[20];
    int count;
} EmpTable;

void emp_init(EmpTable *t) {
    t->count = 0;
}

void emp_insert(EmpTable *t, int id, int salary, int dept, int active) {
    if (t->count >= 20) return;
    Employee *e = &t->rows[t->count];
    e->id = id;
    e->salary = salary;
    e->dept = dept;
    e->active = active;
    t->count = t->count + 1;
}

/* WHERE salary > threshold */
int where_salary_gt(EmpTable *t, int threshold) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].salary > threshold) {
            printf("  id=%d salary=%d dept=%d\n", t->rows[i].id, t->rows[i].salary, t->rows[i].dept);
            found = found + 1;
        }
    }
    return found;
}

/* WHERE dept = d AND active = 1 */
int where_dept_and_active(EmpTable *t, int d) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].dept == d && t->rows[i].active == 1) {
            printf("  id=%d salary=%d\n", t->rows[i].id, t->rows[i].salary);
            found = found + 1;
        }
    }
    return found;
}

/* WHERE salary BETWEEN lo AND hi */
int where_salary_between(EmpTable *t, int lo, int hi) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].salary >= lo && t->rows[i].salary <= hi) {
            printf("  id=%d salary=%d\n", t->rows[i].id, t->rows[i].salary);
            found = found + 1;
        }
    }
    return found;
}

/* WHERE dept IN (d1, d2) */
int where_dept_in(EmpTable *t, int d1, int d2) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].dept == d1 || t->rows[i].dept == d2) {
            printf("  id=%d dept=%d\n", t->rows[i].id, t->rows[i].dept);
            found = found + 1;
        }
    }
    return found;
}

/* WHERE NOT active */
int where_not_active(EmpTable *t) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active == 0) {
            printf("  id=%d salary=%d\n", t->rows[i].id, t->rows[i].salary);
            found = found + 1;
        }
    }
    return found;
}

int main() {
    EmpTable t;
    emp_init(&t);

    emp_insert(&t, 1, 50000, 10, 1);
    emp_insert(&t, 2, 75000, 20, 1);
    emp_insert(&t, 3, 60000, 10, 0);
    emp_insert(&t, 4, 90000, 30, 1);
    emp_insert(&t, 5, 45000, 20, 1);
    emp_insert(&t, 6, 80000, 10, 1);
    emp_insert(&t, 7, 55000, 30, 0);
    emp_insert(&t, 8, 70000, 20, 1);

    printf("WHERE salary > 65000:\n");
    // EXPECT: WHERE salary > 65000:
    int n = where_salary_gt(&t, 65000);
    // EXPECT:   id=2 salary=75000 dept=20
    // EXPECT:   id=4 salary=90000 dept=30
    // EXPECT:   id=6 salary=80000 dept=10
    // EXPECT:   id=8 salary=70000 dept=20
    printf("found=%d\n", n);
    // EXPECT: found=4

    printf("WHERE dept=10 AND active=1:\n");
    // EXPECT: WHERE dept=10 AND active=1:
    n = where_dept_and_active(&t, 10);
    // EXPECT:   id=1 salary=50000
    // EXPECT:   id=6 salary=80000
    printf("found=%d\n", n);
    // EXPECT: found=2

    printf("WHERE salary BETWEEN 50000 AND 70000:\n");
    // EXPECT: WHERE salary BETWEEN 50000 AND 70000:
    n = where_salary_between(&t, 50000, 70000);
    // EXPECT:   id=1 salary=50000
    // EXPECT:   id=3 salary=60000
    // EXPECT:   id=7 salary=55000
    // EXPECT:   id=8 salary=70000
    printf("found=%d\n", n);
    // EXPECT: found=4

    printf("WHERE dept IN (20, 30):\n");
    // EXPECT: WHERE dept IN (20, 30):
    n = where_dept_in(&t, 20, 30);
    // EXPECT:   id=2 dept=20
    // EXPECT:   id=4 dept=30
    // EXPECT:   id=5 dept=20
    // EXPECT:   id=7 dept=30
    // EXPECT:   id=8 dept=20
    printf("found=%d\n", n);
    // EXPECT: found=5

    printf("WHERE NOT active:\n");
    // EXPECT: WHERE NOT active:
    n = where_not_active(&t);
    // EXPECT:   id=3 salary=60000
    // EXPECT:   id=7 salary=55000
    printf("found=%d\n", n);
    // EXPECT: found=2

    printf("done\n");
    // EXPECT: done

    return 0;
}
