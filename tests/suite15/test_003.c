int printf(const char *fmt, ...);

/* test_003: ORDER BY sorting on a table */

typedef struct {
    int id;
    int score;
    char name[12];
} Student;

typedef struct {
    Student rows[16];
    int count;
} StudentTable;

void st_init(StudentTable *t) {
    t->count = 0;
}

void copy_str(char *dst, char *src, int max) {
    int i = 0;
    while (src[i] && i < max - 1) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void st_insert(StudentTable *t, int id, int score, char *name) {
    if (t->count >= 16) return;
    Student *s = &t->rows[t->count];
    s->id = id;
    s->score = score;
    copy_str(s->name, name, 12);
    t->count = t->count + 1;
}

void swap_students(Student *a, Student *b) {
    Student tmp;
    tmp.id = a->id;
    tmp.score = a->score;
    copy_str(tmp.name, a->name, 12);

    a->id = b->id;
    a->score = b->score;
    copy_str(a->name, b->name, 12);

    b->id = tmp.id;
    b->score = tmp.score;
    copy_str(b->name, tmp.name, 12);
}

/* Bubble sort by score ascending */
void order_by_score_asc(StudentTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            if (t->rows[j].score > t->rows[j + 1].score) {
                swap_students(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

/* Bubble sort by score descending */
void order_by_score_desc(StudentTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            if (t->rows[j].score < t->rows[j + 1].score) {
                swap_students(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

/* Bubble sort by id ascending */
void order_by_id_asc(StudentTable *t) {
    int i;
    int j;
    for (i = 0; i < t->count - 1; i = i + 1) {
        for (j = 0; j < t->count - 1 - i; j = j + 1) {
            if (t->rows[j].id > t->rows[j + 1].id) {
                swap_students(&t->rows[j], &t->rows[j + 1]);
            }
        }
    }
}

void print_table(StudentTable *t) {
    int i;
    for (i = 0; i < t->count; i = i + 1) {
        printf("  id=%d score=%d name=%s\n", t->rows[i].id, t->rows[i].score, t->rows[i].name);
    }
}

int main() {
    StudentTable t;
    st_init(&t);

    st_insert(&t, 5, 88, "Eve");
    st_insert(&t, 2, 72, "Bob");
    st_insert(&t, 4, 95, "Dave");
    st_insert(&t, 1, 85, "Alice");
    st_insert(&t, 3, 60, "Carol");
    st_insert(&t, 6, 91, "Frank");

    printf("Original:\n");
    // EXPECT: Original:
    print_table(&t);
    // EXPECT:   id=5 score=88 name=Eve
    // EXPECT:   id=2 score=72 name=Bob
    // EXPECT:   id=4 score=95 name=Dave
    // EXPECT:   id=1 score=85 name=Alice
    // EXPECT:   id=3 score=60 name=Carol
    // EXPECT:   id=6 score=91 name=Frank

    printf("ORDER BY score ASC:\n");
    // EXPECT: ORDER BY score ASC:
    order_by_score_asc(&t);
    print_table(&t);
    // EXPECT:   id=3 score=60 name=Carol
    // EXPECT:   id=2 score=72 name=Bob
    // EXPECT:   id=1 score=85 name=Alice
    // EXPECT:   id=5 score=88 name=Eve
    // EXPECT:   id=6 score=91 name=Frank
    // EXPECT:   id=4 score=95 name=Dave

    printf("ORDER BY score DESC:\n");
    // EXPECT: ORDER BY score DESC:
    order_by_score_desc(&t);
    print_table(&t);
    // EXPECT:   id=4 score=95 name=Dave
    // EXPECT:   id=6 score=91 name=Frank
    // EXPECT:   id=5 score=88 name=Eve
    // EXPECT:   id=1 score=85 name=Alice
    // EXPECT:   id=2 score=72 name=Bob
    // EXPECT:   id=3 score=60 name=Carol

    printf("ORDER BY id ASC:\n");
    // EXPECT: ORDER BY id ASC:
    order_by_id_asc(&t);
    print_table(&t);
    // EXPECT:   id=1 score=85 name=Alice
    // EXPECT:   id=2 score=72 name=Bob
    // EXPECT:   id=3 score=60 name=Carol
    // EXPECT:   id=4 score=95 name=Dave
    // EXPECT:   id=5 score=88 name=Eve
    // EXPECT:   id=6 score=91 name=Frank

    printf("done\n");
    // EXPECT: done

    return 0;
}
