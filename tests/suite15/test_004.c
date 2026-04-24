int printf(const char *fmt, ...);

/* test_004: GROUP BY with COUNT/SUM aggregation */

typedef struct {
    int id;
    int dept;
    int salary;
    int years;
} Worker;

typedef struct {
    Worker rows[20];
    int count;
} WorkerTable;

typedef struct {
    int key;
    int cnt;
    int sum_salary;
    int sum_years;
    int min_salary;
    int max_salary;
} GroupResult;

typedef struct {
    GroupResult groups[10];
    int count;
} GroupSet;

void wt_init(WorkerTable *t) {
    t->count = 0;
}

void wt_insert(WorkerTable *t, int id, int dept, int salary, int years) {
    if (t->count >= 20) return;
    Worker *w = &t->rows[t->count];
    w->id = id;
    w->dept = dept;
    w->salary = salary;
    w->years = years;
    t->count = t->count + 1;
}

int find_group(GroupSet *gs, int key) {
    int i;
    for (i = 0; i < gs->count; i = i + 1) {
        if (gs->groups[i].key == key) return i;
    }
    return -1;
}

void group_by_dept(WorkerTable *t, GroupSet *gs) {
    gs->count = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        int dept = t->rows[i].dept;
        int idx = find_group(gs, dept);
        if (idx < 0) {
            idx = gs->count;
            gs->groups[idx].key = dept;
            gs->groups[idx].cnt = 0;
            gs->groups[idx].sum_salary = 0;
            gs->groups[idx].sum_years = 0;
            gs->groups[idx].min_salary = t->rows[i].salary;
            gs->groups[idx].max_salary = t->rows[i].salary;
            gs->count = gs->count + 1;
        }
        gs->groups[idx].cnt = gs->groups[idx].cnt + 1;
        gs->groups[idx].sum_salary = gs->groups[idx].sum_salary + t->rows[i].salary;
        gs->groups[idx].sum_years = gs->groups[idx].sum_years + t->rows[i].years;
        if (t->rows[i].salary < gs->groups[idx].min_salary) {
            gs->groups[idx].min_salary = t->rows[i].salary;
        }
        if (t->rows[i].salary > gs->groups[idx].max_salary) {
            gs->groups[idx].max_salary = t->rows[i].salary;
        }
    }
}

/* Sort groups by key ascending */
void sort_groups(GroupSet *gs) {
    int i;
    int j;
    for (i = 0; i < gs->count - 1; i = i + 1) {
        for (j = 0; j < gs->count - 1 - i; j = j + 1) {
            if (gs->groups[j].key > gs->groups[j + 1].key) {
                GroupResult tmp = gs->groups[j];
                gs->groups[j] = gs->groups[j + 1];
                gs->groups[j + 1] = tmp;
            }
        }
    }
}

int main() {
    WorkerTable t;
    wt_init(&t);

    wt_insert(&t, 1, 10, 50000, 3);
    wt_insert(&t, 2, 20, 60000, 5);
    wt_insert(&t, 3, 10, 55000, 2);
    wt_insert(&t, 4, 30, 70000, 7);
    wt_insert(&t, 5, 20, 65000, 4);
    wt_insert(&t, 6, 10, 48000, 1);
    wt_insert(&t, 7, 30, 72000, 6);
    wt_insert(&t, 8, 20, 58000, 3);
    wt_insert(&t, 9, 10, 52000, 4);

    printf("count=%d\n", t.count);
    // EXPECT: count=9

    GroupSet gs;
    group_by_dept(&t, &gs);
    sort_groups(&gs);

    printf("GROUP BY dept:\n");
    // EXPECT: GROUP BY dept:

    int i;
    for (i = 0; i < gs.count; i = i + 1) {
        printf("  dept=%d count=%d sum_sal=%d\n",
               gs.groups[i].key, gs.groups[i].cnt, gs.groups[i].sum_salary);
    }
    // EXPECT:   dept=10 count=4 sum_sal=205000
    // EXPECT:   dept=20 count=3 sum_sal=183000
    // EXPECT:   dept=30 count=2 sum_sal=142000

    printf("Aggregates:\n");
    // EXPECT: Aggregates:
    for (i = 0; i < gs.count; i = i + 1) {
        int avg = gs.groups[i].sum_salary / gs.groups[i].cnt;
        printf("  dept=%d avg=%d min=%d max=%d sum_yrs=%d\n",
               gs.groups[i].key, avg,
               gs.groups[i].min_salary, gs.groups[i].max_salary,
               gs.groups[i].sum_years);
    }
    // EXPECT:   dept=10 avg=51250 min=48000 max=55000 sum_yrs=10
    // EXPECT:   dept=20 avg=61000 min=58000 max=65000 sum_yrs=12
    // EXPECT:   dept=30 avg=71000 min=70000 max=72000 sum_yrs=13

    /* HAVING count > 2 */
    printf("HAVING count > 2:\n");
    // EXPECT: HAVING count > 2:
    for (i = 0; i < gs.count; i = i + 1) {
        if (gs.groups[i].cnt > 2) {
            printf("  dept=%d count=%d\n", gs.groups[i].key, gs.groups[i].cnt);
        }
    }
    // EXPECT:   dept=10 count=4
    // EXPECT:   dept=20 count=3

    printf("total_groups=%d\n", gs.count);
    // EXPECT: total_groups=3

    printf("done\n");
    // EXPECT: done

    return 0;
}
