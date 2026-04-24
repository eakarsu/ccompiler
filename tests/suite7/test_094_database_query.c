int printf(const char *fmt, ...);

// Database query simulation: table scan, filter, project, join, aggregate

struct Row {
    int id;
    int col_a;
    int col_b;
    int col_c;
};

struct Table {
    struct Row rows[16];
    int count;
};

void table_init(struct Table *t) {
    t->count = 0;
}

void table_insert(struct Table *t, int id, int a, int b, int c) {
    int idx = t->count;
    t->rows[idx].id = id;
    t->rows[idx].col_a = a;
    t->rows[idx].col_b = b;
    t->rows[idx].col_c = c;
    t->count = t->count + 1;
}

// Filter: select rows where col_a > threshold
int filter_gt(struct Table *src, struct Table *dst, int threshold) {
    table_init(dst);
    int i;
    for (i = 0; i < src->count; i++) {
        if (src->rows[i].col_a > threshold) {
            table_insert(dst, src->rows[i].id, src->rows[i].col_a,
                         src->rows[i].col_b, src->rows[i].col_c);
        }
    }
    return dst->count;
}

// Filter: select rows where col_b == value
int filter_eq(struct Table *src, struct Table *dst, int value) {
    table_init(dst);
    int i;
    for (i = 0; i < src->count; i++) {
        if (src->rows[i].col_b == value) {
            table_insert(dst, src->rows[i].id, src->rows[i].col_a,
                         src->rows[i].col_b, src->rows[i].col_c);
        }
    }
    return dst->count;
}

// Project: extract col_a and col_b only (col_c set to 0)
void project_ab(struct Table *src, struct Table *dst) {
    table_init(dst);
    int i;
    for (i = 0; i < src->count; i++) {
        table_insert(dst, src->rows[i].id, src->rows[i].col_a,
                     src->rows[i].col_b, 0);
    }
}

// Nested-loop join on col_a == col_b
int join_tables(struct Table *left, struct Table *right, struct Table *result) {
    table_init(result);
    int i;
    int j;
    for (i = 0; i < left->count; i++) {
        for (j = 0; j < right->count; j++) {
            if (left->rows[i].col_a == right->rows[j].col_b) {
                table_insert(result, left->rows[i].id,
                             left->rows[i].col_a,
                             right->rows[j].col_c,
                             right->rows[j].id);
            }
        }
    }
    return result->count;
}

// Aggregate functions
int agg_sum(struct Table *t, int col) {
    int sum = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        if (col == 0) sum = sum + t->rows[i].col_a;
        else if (col == 1) sum = sum + t->rows[i].col_b;
        else sum = sum + t->rows[i].col_c;
    }
    return sum;
}

int agg_count(struct Table *t) {
    return t->count;
}

int agg_max(struct Table *t, int col) {
    int mx;
    int i;
    if (t->count == 0) return 0;
    if (col == 0) mx = t->rows[0].col_a;
    else if (col == 1) mx = t->rows[0].col_b;
    else mx = t->rows[0].col_c;
    for (i = 1; i < t->count; i++) {
        int val;
        if (col == 0) val = t->rows[i].col_a;
        else if (col == 1) val = t->rows[i].col_b;
        else val = t->rows[i].col_c;
        if (val > mx) mx = val;
    }
    return mx;
}

int agg_min(struct Table *t, int col) {
    int mn;
    int i;
    if (t->count == 0) return 0;
    if (col == 0) mn = t->rows[0].col_a;
    else if (col == 1) mn = t->rows[0].col_b;
    else mn = t->rows[0].col_c;
    for (i = 1; i < t->count; i++) {
        int val;
        if (col == 0) val = t->rows[i].col_a;
        else if (col == 1) val = t->rows[i].col_b;
        else val = t->rows[i].col_c;
        if (val < mn) mn = val;
    }
    return mn;
}

int main(void) {
    struct Table employees;
    struct Table filtered;
    struct Table projected;
    struct Table departments;
    struct Table joined;

    // Build employees table
    table_init(&employees);
    table_insert(&employees, 1, 50, 10, 100);  // id=1, salary=50, dept=10, bonus=100
    table_insert(&employees, 2, 80, 20, 200);  // id=2, salary=80, dept=20, bonus=200
    table_insert(&employees, 3, 30, 10, 50);   // id=3, salary=30, dept=10, bonus=50
    table_insert(&employees, 4, 90, 30, 300);  // id=4, salary=90, dept=30, bonus=300
    table_insert(&employees, 5, 60, 20, 150);  // id=5, salary=60, dept=20, bonus=150
    table_insert(&employees, 6, 45, 10, 80);   // id=6, salary=45, dept=10, bonus=80

    // EXPECT: Employees: 6
    printf("Employees: %d\n", employees.count);

    // Filter: salary > 50
    filter_gt(&employees, &filtered, 50);
    // EXPECT: Salary > 50: 3
    printf("Salary > 50: %d\n", filtered.count);

    // EXPECT: Filtered ids: 2 4 5
    printf("Filtered ids: %d %d %d\n",
           filtered.rows[0].id, filtered.rows[1].id, filtered.rows[2].id);

    // Filter: dept == 10
    filter_eq(&employees, &filtered, 10);
    // EXPECT: Dept 10: 3
    printf("Dept 10: %d\n", filtered.count);

    // EXPECT: Dept 10 ids: 1 3 6
    printf("Dept 10 ids: %d %d %d\n",
           filtered.rows[0].id, filtered.rows[1].id, filtered.rows[2].id);

    // Project salary and dept
    project_ab(&employees, &projected);
    // EXPECT: Projected count: 6 first_c=0
    printf("Projected count: %d first_c=%d\n", projected.count, projected.rows[0].col_c);

    // Aggregates on full table
    // EXPECT: Sum salary: 355
    printf("Sum salary: %d\n", agg_sum(&employees, 0));

    // EXPECT: Max salary: 90
    printf("Max salary: %d\n", agg_max(&employees, 0));

    // EXPECT: Min salary: 30
    printf("Min salary: %d\n", agg_min(&employees, 0));

    // EXPECT: Count: 6
    printf("Count: %d\n", agg_count(&employees));

    // EXPECT: Sum bonus: 880
    printf("Sum bonus: %d\n", agg_sum(&employees, 2));

    // Aggregates on filtered (dept 10)
    filter_eq(&employees, &filtered, 10);
    // EXPECT: Dept 10 sum salary: 125
    printf("Dept 10 sum salary: %d\n", agg_sum(&filtered, 0));

    // EXPECT: Dept 10 max bonus: 100
    printf("Dept 10 max bonus: %d\n", agg_max(&filtered, 2));

    // Join: employees.salary == departments.dept_code
    table_init(&departments);
    table_insert(&departments, 101, 0, 50, 500);  // dept has col_b=50
    table_insert(&departments, 102, 0, 80, 600);  // dept has col_b=80
    table_insert(&departments, 103, 0, 90, 700);  // dept has col_b=90

    join_tables(&employees, &departments, &joined);
    // EXPECT: Join result: 3
    printf("Join result: %d\n", joined.count);

    // Employee id=1(salary=50) joins with dept 101(col_b=50)
    // EXPECT: Join row 0: emp=1 match_dept=101
    printf("Join row 0: emp=%d match_dept=%d\n", joined.rows[0].id, joined.rows[0].col_c);

    // EXPECT: Join row 1: emp=2 match_dept=102
    printf("Join row 1: emp=%d match_dept=%d\n", joined.rows[1].id, joined.rows[1].col_c);

    // Min/max bonus
    // EXPECT: Min bonus: 50
    printf("Min bonus: %d\n", agg_min(&employees, 2));

    // EXPECT: DB query complete
    printf("DB query complete\n");

    return 0;
}
