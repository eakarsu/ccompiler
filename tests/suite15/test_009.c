int printf(const char *fmt, ...);

/* test_009: Multi-column sorting (ORDER BY col1, col2) */

typedef struct {
    int id;
    int dept;
    int level;
    int salary;
} Staff;

typedef struct {
    Staff rows[20];
    int count;
} StaffTable;

void sf_init(StaffTable *t) {
    t->count = 0;
}

void sf_insert(StaffTable *t, int id, int dept, int level, int salary) {
    if (t->count >= 20) return;
    Staff *s = &t->rows[t->count];
    s->id = id;
    s->dept = dept;
    s->level = level;
    s->salary = salary;
    t->count = t->count + 1;
}

void swap_staff(Staff *a, Staff *b) {
    Staff tmp;
    tmp.id = a->id;
    tmp.dept = a->dept;
    tmp.level = a->level;
    tmp.salary = a->salary;
    a->id = b->id;
    a->dept = b->dept;
    a->level = b->level;
    a->salary = b->salary;
    b->id = tmp.id;
    b->dept = tmp.dept;
    b->level = tmp.level;
    b->salary = tmp.salary;
}

/* ORDER BY dept ASC, level ASC */
void order_by_dept_level(StaffTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            int swap = 0;
            if (t->rows[j].dept > t->rows[j + 1].dept) {
                swap = 1;
            } else if (t->rows[j].dept == t->rows[j + 1].dept) {
                if (t->rows[j].level > t->rows[j + 1].level) {
                    swap = 1;
                }
            }
            if (swap) {
                swap_staff(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

/* ORDER BY dept ASC, salary DESC */
void order_by_dept_salary_desc(StaffTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            int swap = 0;
            if (t->rows[j].dept > t->rows[j + 1].dept) {
                swap = 1;
            } else if (t->rows[j].dept == t->rows[j + 1].dept) {
                if (t->rows[j].salary < t->rows[j + 1].salary) {
                    swap = 1;
                }
            }
            if (swap) {
                swap_staff(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

/* ORDER BY level DESC, dept ASC, id ASC (3-column sort) */
void order_by_level_dept_id(StaffTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            int swap = 0;
            if (t->rows[j].level < t->rows[j + 1].level) {
                swap = 1;
            } else if (t->rows[j].level == t->rows[j + 1].level) {
                if (t->rows[j].dept > t->rows[j + 1].dept) {
                    swap = 1;
                } else if (t->rows[j].dept == t->rows[j + 1].dept) {
                    if (t->rows[j].id > t->rows[j + 1].id) {
                        swap = 1;
                    }
                }
            }
            if (swap) {
                swap_staff(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

void sf_print(StaffTable *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        printf("  id=%d dept=%d lvl=%d sal=%d\n",
               t->rows[i].id, t->rows[i].dept, t->rows[i].level, t->rows[i].salary);
    }
}

/* Make a copy so we can sort without destroying original */
void sf_copy(StaffTable *dst, StaffTable *src) {
    int i;
    dst->count = src->count;
    for (i = 0; i < src->count; i = i + 1) {
        dst->rows[i] = src->rows[i];
    }
}

int main() {
    StaffTable orig;
    sf_init(&orig);

    sf_insert(&orig, 1, 20, 3, 70000);
    sf_insert(&orig, 2, 10, 1, 45000);
    sf_insert(&orig, 3, 20, 1, 50000);
    sf_insert(&orig, 4, 10, 3, 80000);
    sf_insert(&orig, 5, 30, 2, 60000);
    sf_insert(&orig, 6, 10, 2, 55000);
    sf_insert(&orig, 7, 20, 2, 65000);
    sf_insert(&orig, 8, 30, 1, 48000);
    sf_insert(&orig, 9, 30, 3, 75000);
    sf_insert(&orig, 10, 10, 1, 42000);

    printf("count=%d\n", orig.count);
    // EXPECT: count=10

    StaffTable t;

    printf("ORDER BY dept ASC, level ASC:\n");
    // EXPECT: ORDER BY dept ASC, level ASC:
    sf_copy(&t, &orig);
    order_by_dept_level(&t);
    sf_print(&t);
    // EXPECT:   id=2 dept=10 lvl=1 sal=45000
    // EXPECT:   id=10 dept=10 lvl=1 sal=42000
    // EXPECT:   id=6 dept=10 lvl=2 sal=55000
    // EXPECT:   id=4 dept=10 lvl=3 sal=80000
    // EXPECT:   id=3 dept=20 lvl=1 sal=50000
    // EXPECT:   id=7 dept=20 lvl=2 sal=65000
    // EXPECT:   id=1 dept=20 lvl=3 sal=70000
    // EXPECT:   id=8 dept=30 lvl=1 sal=48000
    // EXPECT:   id=5 dept=30 lvl=2 sal=60000
    // EXPECT:   id=9 dept=30 lvl=3 sal=75000

    printf("ORDER BY dept ASC, salary DESC:\n");
    // EXPECT: ORDER BY dept ASC, salary DESC:
    sf_copy(&t, &orig);
    order_by_dept_salary_desc(&t);
    sf_print(&t);
    // EXPECT:   id=4 dept=10 lvl=3 sal=80000
    // EXPECT:   id=6 dept=10 lvl=2 sal=55000
    // EXPECT:   id=2 dept=10 lvl=1 sal=45000
    // EXPECT:   id=10 dept=10 lvl=1 sal=42000
    // EXPECT:   id=1 dept=20 lvl=3 sal=70000
    // EXPECT:   id=7 dept=20 lvl=2 sal=65000
    // EXPECT:   id=3 dept=20 lvl=1 sal=50000
    // EXPECT:   id=9 dept=30 lvl=3 sal=75000
    // EXPECT:   id=5 dept=30 lvl=2 sal=60000
    // EXPECT:   id=8 dept=30 lvl=1 sal=48000

    printf("ORDER BY level DESC, dept ASC, id ASC:\n");
    // EXPECT: ORDER BY level DESC, dept ASC, id ASC:
    sf_copy(&t, &orig);
    order_by_level_dept_id(&t);
    sf_print(&t);
    // EXPECT:   id=4 dept=10 lvl=3 sal=80000
    // EXPECT:   id=1 dept=20 lvl=3 sal=70000
    // EXPECT:   id=9 dept=30 lvl=3 sal=75000
    // EXPECT:   id=6 dept=10 lvl=2 sal=55000
    // EXPECT:   id=7 dept=20 lvl=2 sal=65000
    // EXPECT:   id=5 dept=30 lvl=2 sal=60000
    // EXPECT:   id=2 dept=10 lvl=1 sal=45000
    // EXPECT:   id=10 dept=10 lvl=1 sal=42000
    // EXPECT:   id=3 dept=20 lvl=1 sal=50000
    // EXPECT:   id=8 dept=30 lvl=1 sal=48000

    printf("done\n");
    // EXPECT: done

    return 0;
}
