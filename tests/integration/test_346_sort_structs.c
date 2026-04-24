int printf(const char *fmt, ...);
// EXPECT: by salary:\n  id=3 sal=55000 dept=1\n  id=6 sal=60000 dept=3\n  id=4 sal=65000 dept=3\n  id=5 sal=70000 dept=2\n  id=1 sal=80000 dept=2\n  id=2 sal=90000 dept=1\nby id:\n  id=1 sal=80000 dept=2\n  id=2 sal=90000 dept=1\n  id=3 sal=55000 dept=1\n  id=4 sal=65000 dept=3\n  id=5 sal=70000 dept=2\n  id=6 sal=60000 dept=3\nby dept then salary:\n  id=3 sal=55000 dept=1\n  id=2 sal=90000 dept=1\n  id=5 sal=70000 dept=2\n  id=1 sal=80000 dept=2\n  id=6 sal=60000 dept=3\n  id=4 sal=65000 dept=3
// Test: sort structs by field

struct Employee {
    int id;
    int salary;
    int dept;
};

void swap_employees(struct Employee *a, struct Employee *b) {
    struct Employee tmp;
    tmp.id = a->id;
    tmp.salary = a->salary;
    tmp.dept = a->dept;
    a->id = b->id;
    a->salary = b->salary;
    a->dept = b->dept;
    b->id = tmp.id;
    b->salary = tmp.salary;
    b->dept = tmp.dept;
}

void sort_by_salary(struct Employee arr[], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].salary > arr[j + 1].salary) {
                swap_employees(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_by_id(struct Employee arr[], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].id > arr[j + 1].id) {
                swap_employees(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_by_dept_then_salary(struct Employee arr[], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            int swap = 0;
            if (arr[j].dept > arr[j + 1].dept) {
                swap = 1;
            } else if (arr[j].dept == arr[j + 1].dept && arr[j].salary > arr[j + 1].salary) {
                swap = 1;
            }
            if (swap) {
                swap_employees(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void print_employees(struct Employee arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("  id=%d sal=%d dept=%d\n", arr[i].id, arr[i].salary, arr[i].dept);
    }
}

int main(void) {
    struct Employee emps[6];
    emps[0].id = 5; emps[0].salary = 70000; emps[0].dept = 2;
    emps[1].id = 3; emps[1].salary = 55000; emps[1].dept = 1;
    emps[2].id = 1; emps[2].salary = 80000; emps[2].dept = 2;
    emps[3].id = 4; emps[3].salary = 65000; emps[3].dept = 3;
    emps[4].id = 2; emps[4].salary = 90000; emps[4].dept = 1;
    emps[5].id = 6; emps[5].salary = 60000; emps[5].dept = 3;

    printf("by salary:\n");
    sort_by_salary(emps, 6);
    print_employees(emps, 6);

    printf("by id:\n");
    sort_by_id(emps, 6);
    print_employees(emps, 6);

    printf("by dept then salary:\n");
    sort_by_dept_then_salary(emps, 6);
    print_employees(emps, 6);

    return 0;
}
