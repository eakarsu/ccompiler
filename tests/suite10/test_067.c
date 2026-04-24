int printf(const char *fmt, ...);

// ORDER BY with comparator function pointers

struct Record {
    int id;
    int name_code;
    int score;
    int age;
};

struct Table {
    struct Record rows[16];
    int nrows;
};

typedef int (*Comparator)(struct Record *a, struct Record *b);

void add_rec(struct Table *t, int id, int nc, int score, int age) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].name_code = nc;
    t->rows[t->nrows].score = score;
    t->rows[t->nrows].age = age;
    t->nrows++;
}

// Comparators
int cmp_score_asc(struct Record *a, struct Record *b) {
    return a->score - b->score;
}

int cmp_score_desc(struct Record *a, struct Record *b) {
    return b->score - a->score;
}

int cmp_age_asc(struct Record *a, struct Record *b) {
    return a->age - b->age;
}

int cmp_name_asc(struct Record *a, struct Record *b) {
    return a->name_code - b->name_code;
}

// Composite: sort by score desc, then by age asc
int cmp_score_desc_age_asc(struct Record *a, struct Record *b) {
    int d = b->score - a->score;
    if (d != 0) return d;
    return a->age - b->age;
}

// Insertion sort using comparator
void order_by(struct Table *t, Comparator cmp) {
    int i;
    int j;
    for (i = 1; i < t->nrows; i++) {
        struct Record tmp = t->rows[i];
        j = i - 1;
        while (j >= 0 && cmp(&t->rows[j], &tmp) > 0) {
            t->rows[j + 1] = t->rows[j];
            j--;
        }
        t->rows[j + 1] = tmp;
    }
}

// Copy table for resorting
void copy_table(struct Table *dst, struct Table *src) {
    dst->nrows = src->nrows;
    int i;
    for (i = 0; i < src->nrows; i++) {
        dst->rows[i] = src->rows[i];
    }
}

char *name_str(int code) {
    if (code == 1) return "Alice";
    if (code == 2) return "Bob";
    if (code == 3) return "Charlie";
    if (code == 4) return "Dave";
    if (code == 5) return "Eve";
    if (code == 6) return "Frank";
    if (code == 7) return "Grace";
    if (code == 8) return "Hank";
    return "Unknown";
}

int main() {
    struct Table original;
    original.nrows = 0;

    add_rec(&original, 1, 3, 85, 25);  // Charlie, 85, 25
    add_rec(&original, 2, 1, 92, 30);  // Alice, 92, 30
    add_rec(&original, 3, 5, 78, 22);  // Eve, 78, 22
    add_rec(&original, 4, 2, 92, 28);  // Bob, 92, 28
    add_rec(&original, 5, 4, 65, 35);  // Dave, 65, 35
    add_rec(&original, 6, 6, 88, 27);  // Frank, 88, 27
    add_rec(&original, 7, 7, 78, 31);  // Grace, 78, 31
    add_rec(&original, 8, 8, 95, 24);  // Hank, 95, 24

    printf("Records: %d\n", original.nrows); // EXPECT: Records: 8

    // ORDER BY score ASC
    struct Table t1;
    copy_table(&t1, &original);
    order_by(&t1, cmp_score_asc);
    printf("ORDER BY score ASC:\n"); // EXPECT: ORDER BY score ASC:
    int i;
    for (i = 0; i < t1.nrows; i++) {
        printf("  %s score=%d\n", name_str(t1.rows[i].name_code), t1.rows[i].score);
    }
    // EXPECT:   Dave score=65
    // EXPECT:   Eve score=78
    // EXPECT:   Grace score=78
    // EXPECT:   Charlie score=85
    // EXPECT:   Frank score=88
    // EXPECT:   Alice score=92
    // EXPECT:   Bob score=92
    // EXPECT:   Hank score=95

    // ORDER BY score DESC
    struct Table t2;
    copy_table(&t2, &original);
    order_by(&t2, cmp_score_desc);
    printf("ORDER BY score DESC:\n"); // EXPECT: ORDER BY score DESC:
    printf("  Top: %s score=%d\n", name_str(t2.rows[0].name_code), t2.rows[0].score);
    // EXPECT:   Top: Hank score=95
    printf("  Bottom: %s score=%d\n", name_str(t2.rows[t2.nrows - 1].name_code),
           t2.rows[t2.nrows - 1].score);
    // EXPECT:   Bottom: Dave score=65

    // ORDER BY age ASC
    struct Table t3;
    copy_table(&t3, &original);
    order_by(&t3, cmp_age_asc);
    printf("ORDER BY age ASC:\n"); // EXPECT: ORDER BY age ASC:
    printf("  Youngest: %s age=%d\n", name_str(t3.rows[0].name_code), t3.rows[0].age);
    // EXPECT:   Youngest: Eve age=22
    printf("  Oldest: %s age=%d\n", name_str(t3.rows[t3.nrows - 1].name_code),
           t3.rows[t3.nrows - 1].age);
    // EXPECT:   Oldest: Dave age=35

    // ORDER BY score DESC, age ASC (composite)
    struct Table t4;
    copy_table(&t4, &original);
    order_by(&t4, cmp_score_desc_age_asc);
    printf("ORDER BY score DESC, age ASC:\n"); // EXPECT: ORDER BY score DESC, age ASC:
    for (i = 0; i < t4.nrows; i++) {
        printf("  %s score=%d age=%d\n", name_str(t4.rows[i].name_code),
               t4.rows[i].score, t4.rows[i].age);
    }
    // EXPECT:   Hank score=95 age=24
    // EXPECT:   Bob score=92 age=28
    // EXPECT:   Alice score=92 age=30
    // EXPECT:   Frank score=88 age=27
    // EXPECT:   Charlie score=85 age=25
    // EXPECT:   Eve score=78 age=22
    // EXPECT:   Grace score=78 age=31
    // EXPECT:   Dave score=65 age=35

    // ORDER BY name ASC
    struct Table t5;
    copy_table(&t5, &original);
    order_by(&t5, cmp_name_asc);
    printf("ORDER BY name ASC:\n"); // EXPECT: ORDER BY name ASC:
    printf("  First: %s\n", name_str(t5.rows[0].name_code)); // EXPECT:   First: Alice
    printf("  Last: %s\n", name_str(t5.rows[t5.nrows - 1].name_code)); // EXPECT:   Last: Hank

    printf("ORDER BY test passed\n"); // EXPECT: ORDER BY test passed
    return 0;
}
