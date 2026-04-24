int printf(const char *fmt, ...);

// Nested loop join of two tables

struct Employee {
    int id;
    int name_code;
    int dept_id;
    int salary;
};

struct Department {
    int id;
    int dept_name_code;
    int budget;
};

struct EmpTable {
    struct Employee rows[16];
    int nrows;
};

struct DeptTable {
    struct Department rows[8];
    int nrows;
};

struct JoinRow {
    int emp_id;
    int emp_name;
    int dept_name;
    int salary;
    int budget;
};

struct JoinResult {
    struct JoinRow rows[16];
    int nrows;
};

void emp_add(struct EmpTable *t, int id, int nc, int did, int sal) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].name_code = nc;
    t->rows[t->nrows].dept_id = did;
    t->rows[t->nrows].salary = sal;
    t->nrows++;
}

void dept_add(struct DeptTable *t, int id, int dnc, int budget) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].dept_name_code = dnc;
    t->rows[t->nrows].budget = budget;
    t->nrows++;
}

// INNER JOIN: employees JOIN departments ON employees.dept_id = departments.id
void nested_loop_join(struct EmpTable *emp, struct DeptTable *dept, struct JoinResult *jr) {
    jr->nrows = 0;
    int i;
    int j;
    for (i = 0; i < emp->nrows; i++) {
        for (j = 0; j < dept->nrows; j++) {
            if (emp->rows[i].dept_id == dept->rows[j].id) {
                jr->rows[jr->nrows].emp_id = emp->rows[i].id;
                jr->rows[jr->nrows].emp_name = emp->rows[i].name_code;
                jr->rows[jr->nrows].dept_name = dept->rows[j].dept_name_code;
                jr->rows[jr->nrows].salary = emp->rows[i].salary;
                jr->rows[jr->nrows].budget = dept->rows[j].budget;
                jr->nrows++;
            }
        }
    }
}

// LEFT JOIN: all employees, null dept fields if no match (using -1 as null)
void nested_loop_left_join(struct EmpTable *emp, struct DeptTable *dept, struct JoinResult *jr) {
    jr->nrows = 0;
    int i;
    int j;
    for (i = 0; i < emp->nrows; i++) {
        int matched = 0;
        for (j = 0; j < dept->nrows; j++) {
            if (emp->rows[i].dept_id == dept->rows[j].id) {
                jr->rows[jr->nrows].emp_id = emp->rows[i].id;
                jr->rows[jr->nrows].emp_name = emp->rows[i].name_code;
                jr->rows[jr->nrows].dept_name = dept->rows[j].dept_name_code;
                jr->rows[jr->nrows].salary = emp->rows[i].salary;
                jr->rows[jr->nrows].budget = dept->rows[j].budget;
                jr->nrows++;
                matched = 1;
            }
        }
        if (!matched) {
            jr->rows[jr->nrows].emp_id = emp->rows[i].id;
            jr->rows[jr->nrows].emp_name = emp->rows[i].name_code;
            jr->rows[jr->nrows].dept_name = -1;
            jr->rows[jr->nrows].salary = emp->rows[i].salary;
            jr->rows[jr->nrows].budget = -1;
            jr->nrows++;
        }
    }
}

char *emp_name(int code) {
    if (code == 1) return "Alice";
    if (code == 2) return "Bob";
    if (code == 3) return "Charlie";
    if (code == 4) return "Dave";
    if (code == 5) return "Eve";
    if (code == 6) return "Frank";
    return "Unknown";
}

char *dept_str(int code) {
    if (code == 10) return "Engineering";
    if (code == 20) return "Sales";
    if (code == 30) return "Marketing";
    if (code == -1) return "NULL";
    return "Unknown";
}

int main() {
    struct EmpTable emps;
    emps.nrows = 0;
    emp_add(&emps, 1, 1, 100, 70000);  // Alice, dept 100
    emp_add(&emps, 2, 2, 200, 55000);  // Bob, dept 200
    emp_add(&emps, 3, 3, 100, 82000);  // Charlie, dept 100
    emp_add(&emps, 4, 4, 300, 45000);  // Dave, dept 300
    emp_add(&emps, 5, 5, 200, 60000);  // Eve, dept 200
    emp_add(&emps, 6, 6, 999, 50000);  // Frank, dept 999 (no match)

    struct DeptTable depts;
    depts.nrows = 0;
    dept_add(&depts, 100, 10, 500000);  // Engineering
    dept_add(&depts, 200, 20, 300000);  // Sales
    dept_add(&depts, 300, 30, 200000);  // Marketing

    printf("Employees: %d, Departments: %d\n", emps.nrows, depts.nrows);
    // EXPECT: Employees: 6, Departments: 3

    // Inner join
    struct JoinResult jr;
    nested_loop_join(&emps, &depts, &jr);
    printf("Inner join rows: %d\n", jr.nrows); // EXPECT: Inner join rows: 5

    int i;
    for (i = 0; i < jr.nrows; i++) {
        printf("  %s in %s sal=%d\n", emp_name(jr.rows[i].emp_name),
               dept_str(jr.rows[i].dept_name), jr.rows[i].salary);
    }
    // EXPECT:   Alice in Engineering sal=70000
    // EXPECT:   Bob in Sales sal=55000
    // EXPECT:   Charlie in Engineering sal=82000
    // EXPECT:   Dave in Marketing sal=45000
    // EXPECT:   Eve in Sales sal=60000

    // Left join
    struct JoinResult ljr;
    nested_loop_left_join(&emps, &depts, &ljr);
    printf("Left join rows: %d\n", ljr.nrows); // EXPECT: Left join rows: 6

    for (i = 0; i < ljr.nrows; i++) {
        printf("  %s in %s\n", emp_name(ljr.rows[i].emp_name),
               dept_str(ljr.rows[i].dept_name));
    }
    // EXPECT:   Alice in Engineering
    // EXPECT:   Bob in Sales
    // EXPECT:   Charlie in Engineering
    // EXPECT:   Dave in Marketing
    // EXPECT:   Eve in Sales
    // EXPECT:   Frank in NULL

    // Sum salaries from inner join
    int total_sal = 0;
    for (i = 0; i < jr.nrows; i++) {
        total_sal = total_sal + jr.rows[i].salary;
    }
    printf("Total salary (inner join): %d\n", total_sal);
    // EXPECT: Total salary (inner join): 312000

    printf("Join test passed\n"); // EXPECT: Join test passed
    return 0;
}
