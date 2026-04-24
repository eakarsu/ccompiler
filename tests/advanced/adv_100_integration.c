
// EXPECT: === Mini Database System ===
// EXPECT: Initialized database: employees (cap=16)
// EXPECT: === Insert Records ===
// EXPECT: Inserted: Alice (Engineering, salary=95000)
// EXPECT: Inserted: Bob (Marketing, salary=72000)
// EXPECT: Inserted: Charlie (Engineering, salary=105000)
// EXPECT: Inserted: Diana (Marketing, salary=68000)
// EXPECT: Inserted: Eve (Engineering, salary=88000)
// EXPECT: Inserted: Frank (Sales, salary=77000)
// EXPECT: Inserted: Grace (Sales, salary=82000)
// EXPECT: Inserted: Hank (Engineering, salary=110000)
// EXPECT: Total records: 8
// EXPECT: === Query: All Engineers ===
// EXPECT:   Alice: salary=95000
// EXPECT:   Charlie: salary=105000
// EXPECT:   Eve: salary=88000
// EXPECT:   Hank: salary=110000
// EXPECT: Engineers found: 4
// EXPECT: === Query: Salary > 80000 ===
// EXPECT:   Alice: salary=95000
// EXPECT:   Charlie: salary=105000
// EXPECT:   Eve: salary=88000
// EXPECT:   Grace: salary=82000
// EXPECT:   Hank: salary=110000
// EXPECT: High earners: 5
// EXPECT: === Sort by Salary ===
// EXPECT:   Diana: 68000
// EXPECT:   Bob: 72000
// EXPECT:   Frank: 77000
// EXPECT:   Grace: 82000
// EXPECT:   Eve: 88000
// EXPECT:   Alice: 95000
// EXPECT:   Charlie: 105000
// EXPECT:   Hank: 110000
// EXPECT: === Aggregate Stats ===
// EXPECT: Total salary: 697000
// EXPECT: Average salary: 87125
// EXPECT: Min salary: 68000 (Diana)
// EXPECT: Max salary: 110000 (Hank)
// EXPECT: === Department Summary ===
// EXPECT: Engineering: 4 employees, total=398000, avg=99500
// EXPECT: Marketing: 2 employees, total=140000, avg=70000
// EXPECT: Sales: 2 employees, total=159000, avg=79500
// EXPECT: === Search by Name ===
// EXPECT: Found: Charlie (Engineering, salary=105000)
// EXPECT: Not found: Zara
// EXPECT: === Update Record ===
// EXPECT: Updated Bob salary: 72000 -> 80000
// EXPECT: === Delete Record ===
// EXPECT: Deleted: Diana
// EXPECT: Records after delete: 7
// EXPECT: === Function Pointer Operations ===
// EXPECT: Sort by name:
// EXPECT:   Alice Bob Charlie Eve Frank Grace Hank
// EXPECT: All integration tests passed!
int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// Department enum
enum Department {
    DEPT_ENGINEERING,
    DEPT_MARKETING,
    DEPT_SALES,
    DEPT_COUNT
};

const char *dept_name(int dept) {
    if (dept == DEPT_ENGINEERING) return "Engineering";
    if (dept == DEPT_MARKETING) return "Marketing";
    if (dept == DEPT_SALES) return "Sales";
    return "Unknown";
}

// String utilities
int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void str_copy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_compare(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

// Employee record
struct Employee {
    char name[32];
    int department;
    int salary;
    int active;
};

// Database
struct Database {
    struct Employee records[16];
    int count;
    int capacity;
};

// Comparison function pointer type
typedef int (*CompareFunc)(struct Employee *a, struct Employee *b);

void db_init(struct Database *db) {
    db->count = 0;
    db->capacity = 16;
    int i;
    for (i = 0; i < 16; i++) {
        db->records[i].active = 0;
    }
}

int db_insert(struct Database *db, const char *name, int dept, int salary) {
    if (db->count >= db->capacity) return -1;
    int idx = db->count;
    str_copy(db->records[idx].name, name);
    db->records[idx].department = dept;
    db->records[idx].salary = salary;
    db->records[idx].active = 1;
    db->count = db->count + 1;
    return idx;
}

// Find by name, return index or -1
int db_find(struct Database *db, const char *name) {
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && str_equal(db->records[i].name, name)) {
            return i;
        }
    }
    return -1;
}

// Delete by name (mark inactive)
int db_delete(struct Database *db, const char *name) {
    int idx = db_find(db, name);
    if (idx < 0) return -1;
    db->records[idx].active = 0;
    return 0;
}

// Count active records
int db_active_count(struct Database *db) {
    int count = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active) count = count + 1;
    }
    return count;
}

// Query by department
int db_query_dept(struct Database *db, int dept) {
    int found = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && db->records[i].department == dept) {
            printf("  %s: salary=%d\n", db->records[i].name, db->records[i].salary);
            found = found + 1;
        }
    }
    return found;
}

// Query by salary threshold
int db_query_salary_above(struct Database *db, int threshold) {
    int found = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && db->records[i].salary > threshold) {
            printf("  %s: salary=%d\n", db->records[i].name, db->records[i].salary);
            found = found + 1;
        }
    }
    return found;
}

// Sort active records by comparison function (bubble sort on indices)
void db_sort(struct Database *db, CompareFunc cmp) {
    int i;
    int j;
    struct Employee temp;
    // Simple bubble sort
    for (i = 0; i < db->count - 1; i++) {
        for (j = 0; j < db->count - 1 - i; j++) {
            if (db->records[j].active && db->records[j + 1].active) {
                if (cmp(&db->records[j], &db->records[j + 1]) > 0) {
                    // Swap
                    str_copy(temp.name, db->records[j].name);
                    temp.department = db->records[j].department;
                    temp.salary = db->records[j].salary;
                    temp.active = db->records[j].active;

                    str_copy(db->records[j].name, db->records[j + 1].name);
                    db->records[j].department = db->records[j + 1].department;
                    db->records[j].salary = db->records[j + 1].salary;
                    db->records[j].active = db->records[j + 1].active;

                    str_copy(db->records[j + 1].name, temp.name);
                    db->records[j + 1].department = temp.department;
                    db->records[j + 1].salary = temp.salary;
                    db->records[j + 1].active = temp.active;
                }
            }
        }
    }
}

// Comparison functions
int cmp_by_salary(struct Employee *a, struct Employee *b) {
    return a->salary - b->salary;
}

int cmp_by_name(struct Employee *a, struct Employee *b) {
    return str_compare(a->name, b->name);
}

// Aggregate: total salary
int db_total_salary(struct Database *db) {
    int total = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active) {
            total = total + db->records[i].salary;
        }
    }
    return total;
}

// Find min/max salary, store index
int db_min_salary_idx(struct Database *db) {
    int min_sal = 2147483647;
    int min_idx = -1;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && db->records[i].salary < min_sal) {
            min_sal = db->records[i].salary;
            min_idx = i;
        }
    }
    return min_idx;
}

int db_max_salary_idx(struct Database *db) {
    int max_sal = -1;
    int max_idx = -1;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && db->records[i].salary > max_sal) {
            max_sal = db->records[i].salary;
            max_idx = i;
        }
    }
    return max_idx;
}

// Department stats
void db_dept_summary(struct Database *db, int dept) {
    int count = 0;
    int total = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active && db->records[i].department == dept) {
            count = count + 1;
            total = total + db->records[i].salary;
        }
    }
    if (count > 0) {
        printf("%s: %d employees, total=%d, avg=%d\n",
               dept_name(dept), count, total, total / count);
    }
}

int main(void) {
    struct Database db;
    int idx;
    int found;
    int i;

    printf("=== Mini Database System ===\n");
    db_init(&db);
    printf("Initialized database: employees (cap=%d)\n", db.capacity);

    // Insert records
    printf("=== Insert Records ===\n");
    db_insert(&db, "Alice", DEPT_ENGINEERING, 95000);
    printf("Inserted: Alice (Engineering, salary=95000)\n");
    db_insert(&db, "Bob", DEPT_MARKETING, 72000);
    printf("Inserted: Bob (Marketing, salary=72000)\n");
    db_insert(&db, "Charlie", DEPT_ENGINEERING, 105000);
    printf("Inserted: Charlie (Engineering, salary=105000)\n");
    db_insert(&db, "Diana", DEPT_MARKETING, 68000);
    printf("Inserted: Diana (Marketing, salary=68000)\n");
    db_insert(&db, "Eve", DEPT_ENGINEERING, 88000);
    printf("Inserted: Eve (Engineering, salary=88000)\n");
    db_insert(&db, "Frank", DEPT_SALES, 77000);
    printf("Inserted: Frank (Sales, salary=77000)\n");
    db_insert(&db, "Grace", DEPT_SALES, 82000);
    printf("Inserted: Grace (Sales, salary=82000)\n");
    db_insert(&db, "Hank", DEPT_ENGINEERING, 110000);
    printf("Inserted: Hank (Engineering, salary=110000)\n");
    printf("Total records: %d\n", db.count);

    // Query by department
    printf("=== Query: All Engineers ===\n");
    found = db_query_dept(&db, DEPT_ENGINEERING);
    printf("Engineers found: %d\n", found);

    // Query by salary
    printf("=== Query: Salary > 80000 ===\n");
    found = db_query_salary_above(&db, 80000);
    printf("High earners: %d\n", found);

    // Sort by salary
    printf("=== Sort by Salary ===\n");
    db_sort(&db, cmp_by_salary);
    for (i = 0; i < db.count; i++) {
        if (db.records[i].active) {
            printf("  %s: %d\n", db.records[i].name, db.records[i].salary);
        }
    }

    // Aggregate stats
    printf("=== Aggregate Stats ===\n");
    int total = db_total_salary(&db);
    int active = db_active_count(&db);
    printf("Total salary: %d\n", total);
    printf("Average salary: %d\n", total / active);
    idx = db_min_salary_idx(&db);
    printf("Min salary: %d (%s)\n", db.records[idx].salary, db.records[idx].name);
    idx = db_max_salary_idx(&db);
    printf("Max salary: %d (%s)\n", db.records[idx].salary, db.records[idx].name);

    // Department summary
    printf("=== Department Summary ===\n");
    db_dept_summary(&db, DEPT_ENGINEERING);
    db_dept_summary(&db, DEPT_MARKETING);
    db_dept_summary(&db, DEPT_SALES);

    // Search
    printf("=== Search by Name ===\n");
    idx = db_find(&db, "Charlie");
    if (idx >= 0) {
        printf("Found: %s (%s, salary=%d)\n",
               db.records[idx].name, dept_name(db.records[idx].department),
               db.records[idx].salary);
    }
    idx = db_find(&db, "Zara");
    if (idx < 0) {
        printf("Not found: Zara\n");
    }

    // Update
    printf("=== Update Record ===\n");
    idx = db_find(&db, "Bob");
    if (idx >= 0) {
        int old_sal = db.records[idx].salary;
        db.records[idx].salary = 80000;
        printf("Updated Bob salary: %d -> %d\n", old_sal, db.records[idx].salary);
    }

    // Delete
    printf("=== Delete Record ===\n");
    db_delete(&db, "Diana");
    printf("Deleted: Diana\n");
    printf("Records after delete: %d\n", db_active_count(&db));

    // Function pointer sort by name
    printf("=== Function Pointer Operations ===\n");
    db_sort(&db, cmp_by_name);
    printf("Sort by name:\n");
    printf(" ");
    for (i = 0; i < db.count; i++) {
        if (db.records[i].active) {
            printf(" %s", db.records[i].name);
        }
    }
    printf("\n");

    printf("All integration tests passed!\n");
    return 0;
}
