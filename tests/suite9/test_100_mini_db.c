int printf(const char *fmt, ...);

struct Record {
    int id;
    char name[32];
    int age;
    char dept[16];
    int salary;
    int active;
};

struct Table {
    struct Record rows[30];
    int count;
    int next_id;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void init_table(struct Table *t) {
    t->count = 0;
    t->next_id = 1;
}

int insert_record(struct Table *t, const char *name, int age, const char *dept, int salary) {
    int idx;
    idx = t->count;
    t->rows[idx].id = t->next_id;
    str_copy(t->rows[idx].name, name);
    t->rows[idx].age = age;
    str_copy(t->rows[idx].dept, dept);
    t->rows[idx].salary = salary;
    t->rows[idx].active = 1;
    t->count = t->count + 1;
    t->next_id = t->next_id + 1;
    return t->rows[idx].id;
}

int delete_by_id(struct Table *t, int id) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].id == id && t->rows[i].active) {
            t->rows[i].active = 0;
            return 1;
        }
    }
    return 0;
}

int update_salary(struct Table *t, int id, int new_salary) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].id == id && t->rows[i].active) {
            t->rows[i].salary = new_salary;
            return 1;
        }
    }
    return 0;
}

void select_all(struct Table *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active) {
            printf("  [%d] %s age=%d dept=%s sal=%d\n",
                   t->rows[i].id, t->rows[i].name, t->rows[i].age,
                   t->rows[i].dept, t->rows[i].salary);
        }
    }
}

void select_by_dept(struct Table *t, const char *dept) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && str_eq(t->rows[i].dept, dept)) {
            printf("  [%d] %s sal=%d\n", t->rows[i].id,
                   t->rows[i].name, t->rows[i].salary);
        }
    }
}

int count_by_dept(struct Table *t, const char *dept) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && str_eq(t->rows[i].dept, dept)) {
            c = c + 1;
        }
    }
    return c;
}

int sum_salary_by_dept(struct Table *t, const char *dept) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && str_eq(t->rows[i].dept, dept)) {
            sum = sum + t->rows[i].salary;
        }
    }
    return sum;
}

int avg_salary_by_dept(struct Table *t, const char *dept) {
    int cnt;
    int sum;
    cnt = count_by_dept(t, dept);
    if (cnt == 0) return 0;
    sum = sum_salary_by_dept(t, dept);
    return sum / cnt;
}

int select_where_salary_gt(struct Table *t, int threshold) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && t->rows[i].salary > threshold) {
            printf("  [%d] %s sal=%d\n", t->rows[i].id,
                   t->rows[i].name, t->rows[i].salary);
            c = c + 1;
        }
    }
    return c;
}

int select_where_age_between(struct Table *t, int lo, int hi) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && t->rows[i].age >= lo && t->rows[i].age <= hi) {
            printf("  [%d] %s age=%d\n", t->rows[i].id,
                   t->rows[i].name, t->rows[i].age);
            c = c + 1;
        }
    }
    return c;
}

int count_active(struct Table *t) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active) c = c + 1;
    }
    return c;
}

int max_salary(struct Table *t) {
    int i;
    int mx;
    mx = 0;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && t->rows[i].salary > mx) {
            mx = t->rows[i].salary;
        }
    }
    return mx;
}

int min_salary(struct Table *t) {
    int i;
    int mn;
    mn = 999999;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].active && t->rows[i].salary < mn) {
            mn = t->rows[i].salary;
        }
    }
    return mn;
}

int main() {
    struct Table db;
    int n;

    // EXPECT: === Mini Database Test ===
    printf("=== Mini Database Test ===\n");

    init_table(&db);

    insert_record(&db, "Alice", 30, "Eng", 90000);
    insert_record(&db, "Bob", 25, "Eng", 75000);
    insert_record(&db, "Charlie", 35, "Sales", 65000);
    insert_record(&db, "Diana", 28, "Eng", 85000);
    insert_record(&db, "Eve", 40, "Sales", 70000);
    insert_record(&db, "Frank", 32, "HR", 60000);
    insert_record(&db, "Grace", 27, "HR", 55000);
    insert_record(&db, "Hank", 45, "Sales", 80000);

    // EXPECT: SELECT * (all records):
    printf("SELECT * (all records):\n");
    // EXPECT:   [1] Alice age=30 dept=Eng sal=90000
    // EXPECT:   [2] Bob age=25 dept=Eng sal=75000
    // EXPECT:   [3] Charlie age=35 dept=Sales sal=65000
    // EXPECT:   [4] Diana age=28 dept=Eng sal=85000
    // EXPECT:   [5] Eve age=40 dept=Sales sal=70000
    // EXPECT:   [6] Frank age=32 dept=HR sal=60000
    // EXPECT:   [7] Grace age=27 dept=HR sal=55000
    // EXPECT:   [8] Hank age=45 dept=Sales sal=80000
    select_all(&db);

    // EXPECT: SELECT WHERE dept=Eng:
    printf("SELECT WHERE dept=Eng:\n");
    // EXPECT:   [1] Alice sal=90000
    // EXPECT:   [2] Bob sal=75000
    // EXPECT:   [4] Diana sal=85000
    select_by_dept(&db, "Eng");

    // EXPECT: COUNT(dept=Eng): 3
    printf("COUNT(dept=Eng): %d\n", count_by_dept(&db, "Eng"));
    // EXPECT: SUM(salary) WHERE dept=Eng: 250000
    printf("SUM(salary) WHERE dept=Eng: %d\n", sum_salary_by_dept(&db, "Eng"));
    // EXPECT: AVG(salary) WHERE dept=Eng: 83333
    printf("AVG(salary) WHERE dept=Eng: %d\n", avg_salary_by_dept(&db, "Eng"));

    // EXPECT: SELECT WHERE salary>75000:
    printf("SELECT WHERE salary>75000:\n");
    // EXPECT:   [1] Alice sal=90000
    // EXPECT:   [4] Diana sal=85000
    // EXPECT:   [8] Hank sal=80000
    n = select_where_salary_gt(&db, 75000);
    // EXPECT: Found: 3
    printf("Found: %d\n", n);

    delete_by_id(&db, 3);
    // EXPECT: After DELETE id=3, active: 7
    printf("After DELETE id=3, active: %d\n", count_active(&db));

    update_salary(&db, 2, 80000);
    // EXPECT: After UPDATE id=2 salary: SUM(Eng)=255000
    printf("After UPDATE id=2 salary: SUM(Eng)=%d\n", sum_salary_by_dept(&db, "Eng"));

    // EXPECT: SELECT WHERE age BETWEEN 25 AND 30:
    printf("SELECT WHERE age BETWEEN 25 AND 30:\n");
    // EXPECT:   [1] Alice age=30
    // EXPECT:   [2] Bob age=25
    // EXPECT:   [4] Diana age=28
    // EXPECT:   [7] Grace age=27
    n = select_where_age_between(&db, 25, 30);
    // EXPECT: Found: 4
    printf("Found: %d\n", n);

    // EXPECT: MAX(salary): 90000
    printf("MAX(salary): %d\n", max_salary(&db));
    // EXPECT: MIN(salary): 55000
    printf("MIN(salary): %d\n", min_salary(&db));

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
