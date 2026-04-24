int printf(const char *fmt, ...);
// EXPECT: Table: employees (5 rows)\nname       | age        | salary     | dept      \n----------------------------------------------------\nAlice      | 30         | 85000      | Eng       \nBob        | 25         | 65000      | Sales     \nCharlie    | 35         | 92000      | Eng       \nDiana      | 22         | 55000      | HR        \nEve        | 28         | 78000      | Eng       \n\nQueries:\nTotal records: 5\nAge > 25: 3\nSalary > 70k: 3\nTotal salary (all): 375000\nTotal salary (age>25): 255000\nAvg salary: 75000
#define MAX_RECORDS 10
#define MAX_COLS 5

enum ColType { COL_INT = 0, COL_STR = 1 };

struct Column {
    char *name;
    int type;
};

struct Record {
    int ints[MAX_COLS];
    char *strs[MAX_COLS];
};

struct Table {
    char *name;
    struct Column cols[MAX_COLS];
    int num_cols;
    struct Record rows[MAX_RECORDS];
    int num_rows;
};

typedef int (*WhereClause)(struct Record *, struct Table *);

void table_init(struct Table *t, char *name) {
    t->name = name;
    t->num_cols = 0;
    t->num_rows = 0;
}

void table_add_col(struct Table *t, char *name, int type) {
    if (t->num_cols < MAX_COLS) {
        t->cols[t->num_cols].name = name;
        t->cols[t->num_cols].type = type;
        t->num_cols++;
    }
}

int table_insert(struct Table *t) {
    if (t->num_rows >= MAX_RECORDS) return -1;
    int r = t->num_rows;
    int i;
    for (i = 0; i < MAX_COLS; i++) {
        t->rows[r].ints[i] = 0;
        t->rows[r].strs[i] = "";
    }
    t->num_rows++;
    return r;
}

void table_set_int(struct Table *t, int row, int col, int val) {
    t->rows[row].ints[col] = val;
}

void table_set_str(struct Table *t, int row, int col, char *val) {
    t->rows[row].strs[col] = val;
}

void table_print(struct Table *t) {
    printf("Table: %s (%d rows)\n", t->name, t->num_rows);
    int i;
    int j;
    for (i = 0; i < t->num_cols; i++) {
        if (i > 0) printf(" | ");
        printf("%-10s", t->cols[i].name);
    }
    printf("\n");
    for (i = 0; i < t->num_cols * 13; i++) printf("-");
    printf("\n");
    for (j = 0; j < t->num_rows; j++) {
        for (i = 0; i < t->num_cols; i++) {
            if (i > 0) printf(" | ");
            if (t->cols[i].type == COL_INT) {
                printf("%-10d", t->rows[j].ints[i]);
            } else {
                printf("%-10s", t->rows[j].strs[i]);
            }
        }
        printf("\n");
    }
}

int select_count(struct Table *t, WhereClause where) {
    int count = 0;
    int i;
    for (i = 0; i < t->num_rows; i++) {
        if (where(&t->rows[i], t)) count++;
    }
    return count;
}

int sum_col(struct Table *t, int col, WhereClause where) {
    int total = 0;
    int i;
    for (i = 0; i < t->num_rows; i++) {
        if (where(&t->rows[i], t)) {
            total = total + t->rows[i].ints[col];
        }
    }
    return total;
}

int where_all(struct Record *r, struct Table *t) { return 1; }
int where_age_gt_25(struct Record *r, struct Table *t) { return r->ints[1] > 25; }
int where_salary_gt_70k(struct Record *r, struct Table *t) { return r->ints[2] > 70000; }

int main(void) {
    struct Table employees;
    table_init(&employees, "employees");
    table_add_col(&employees, "name", COL_STR);
    table_add_col(&employees, "age", COL_INT);
    table_add_col(&employees, "salary", COL_INT);
    table_add_col(&employees, "dept", COL_STR);

    int r;
    r = table_insert(&employees);
    table_set_str(&employees, r, 0, "Alice");
    table_set_int(&employees, r, 1, 30);
    table_set_int(&employees, r, 2, 85000);
    table_set_str(&employees, r, 3, "Eng");

    r = table_insert(&employees);
    table_set_str(&employees, r, 0, "Bob");
    table_set_int(&employees, r, 1, 25);
    table_set_int(&employees, r, 2, 65000);
    table_set_str(&employees, r, 3, "Sales");

    r = table_insert(&employees);
    table_set_str(&employees, r, 0, "Charlie");
    table_set_int(&employees, r, 1, 35);
    table_set_int(&employees, r, 2, 92000);
    table_set_str(&employees, r, 3, "Eng");

    r = table_insert(&employees);
    table_set_str(&employees, r, 0, "Diana");
    table_set_int(&employees, r, 1, 22);
    table_set_int(&employees, r, 2, 55000);
    table_set_str(&employees, r, 3, "HR");

    r = table_insert(&employees);
    table_set_str(&employees, r, 0, "Eve");
    table_set_int(&employees, r, 1, 28);
    table_set_int(&employees, r, 2, 78000);
    table_set_str(&employees, r, 3, "Eng");

    table_print(&employees);

    printf("\nQueries:\n");
    printf("Total records: %d\n", select_count(&employees, where_all));
    printf("Age > 25: %d\n", select_count(&employees, where_age_gt_25));
    printf("Salary > 70k: %d\n", select_count(&employees, where_salary_gt_70k));
    printf("Total salary (all): %d\n", sum_col(&employees, 2, where_all));
    printf("Total salary (age>25): %d\n", sum_col(&employees, 2, where_age_gt_25));
    printf("Avg salary: %d\n", sum_col(&employees, 2, where_all) / select_count(&employees, where_all));

    return 0;
}
