int printf(const char *fmt, ...);

/* test_001: Table storage with insert/select operations */

typedef struct {
    int id;
    int age;
    char name[16];
} Row;

typedef struct {
    Row rows[32];
    int count;
} Table;

void table_init(Table *t) {
    t->count = 0;
}

int table_insert(Table *t, int id, int age, char *name) {
    if (t->count >= 32) return -1;
    Row *r = &t->rows[t->count];
    r->id = id;
    r->age = age;
    int i = 0;
    while (name[i] && i < 15) {
        r->name[i] = name[i];
        i = i + 1;
    }
    r->name[i] = 0;
    t->count = t->count + 1;
    return 0;
}

Row *table_select_by_id(Table *t, int id) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].id == id) {
            return &t->rows[i];
        }
    }
    return (Row *)0;
}

int table_select_all(Table *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        printf("row: id=%d age=%d name=%s\n", t->rows[i].id, t->rows[i].age, t->rows[i].name);
    }
    return t->count;
}

int table_count(Table *t) {
    return t->count;
}

int table_select_by_age_range(Table *t, int min_age, int max_age) {
    int found = 0;
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        if (t->rows[i].age >= min_age && t->rows[i].age <= max_age) {
            printf("match: id=%d age=%d name=%s\n", t->rows[i].id, t->rows[i].age, t->rows[i].name);
            found = found + 1;
        }
    }
    return found;
}

int main() {
    Table t;
    table_init(&t);

    printf("count=%d\n", table_count(&t));
    // EXPECT: count=0

    table_insert(&t, 1, 25, "Alice");
    table_insert(&t, 2, 30, "Bob");
    table_insert(&t, 3, 22, "Carol");
    table_insert(&t, 4, 35, "Dave");
    table_insert(&t, 5, 28, "Eve");

    printf("count=%d\n", table_count(&t));
    // EXPECT: count=5

    printf("--- select all ---\n");
    // EXPECT: --- select all ---

    int n = table_select_all(&t);
    // EXPECT: row: id=1 age=25 name=Alice
    // EXPECT: row: id=2 age=30 name=Bob
    // EXPECT: row: id=3 age=22 name=Carol
    // EXPECT: row: id=4 age=35 name=Dave
    // EXPECT: row: id=5 age=28 name=Eve

    printf("printed=%d\n", n);
    // EXPECT: printed=5

    printf("--- select by id ---\n");
    // EXPECT: --- select by id ---

    Row *r = table_select_by_id(&t, 3);
    if (r) {
        printf("found: id=%d name=%s\n", r->id, r->name);
        // EXPECT: found: id=3 name=Carol
    }

    r = table_select_by_id(&t, 99);
    if (r) {
        printf("found unexpected\n");
    } else {
        printf("not found: id=99\n");
        // EXPECT: not found: id=99
    }

    printf("--- age range 25-30 ---\n");
    // EXPECT: --- age range 25-30 ---

    int matches = table_select_by_age_range(&t, 25, 30);
    // EXPECT: match: id=1 age=25 name=Alice
    // EXPECT: match: id=2 age=30 name=Bob
    // EXPECT: match: id=5 age=28 name=Eve

    printf("matches=%d\n", matches);
    // EXPECT: matches=3

    table_insert(&t, 6, 40, "Frank");
    table_insert(&t, 7, 19, "Grace");

    printf("count=%d\n", table_count(&t));
    // EXPECT: count=7

    r = table_select_by_id(&t, 7);
    if (r) {
        printf("found: id=%d age=%d name=%s\n", r->id, r->age, r->name);
        // EXPECT: found: id=7 age=19 name=Grace
    }

    printf("done\n");
    // EXPECT: done

    return 0;
}
