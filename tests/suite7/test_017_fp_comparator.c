int printf(const char *fmt, ...);

struct Record {
    int id;
    int age;
    int score;
    int name_code;
};

typedef int (*RecordCmp)(struct Record *, struct Record *);

int cmp_by_id(struct Record *a, struct Record *b) {
    return a->id - b->id;
}

int cmp_by_age(struct Record *a, struct Record *b) {
    return a->age - b->age;
}

int cmp_by_score(struct Record *a, struct Record *b) {
    return a->score - b->score;
}

int cmp_by_score_desc(struct Record *a, struct Record *b) {
    return b->score - a->score;
}

int cmp_by_name_code(struct Record *a, struct Record *b) {
    return a->name_code - b->name_code;
}

int cmp_age_then_score(struct Record *a, struct Record *b) {
    int d;
    d = a->age - b->age;
    if (d != 0) return d;
    return a->score - b->score;
}

void sort_records(struct Record recs[], int n, RecordCmp cmp) {
    int i;
    int j;
    struct Record tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp(&recs[j], &recs[j + 1]) > 0) {
                tmp = recs[j];
                recs[j] = recs[j + 1];
                recs[j + 1] = tmp;
            }
        }
    }
}

int find_min_record(struct Record recs[], int n, RecordCmp cmp) {
    int best;
    int i;
    best = 0;
    for (i = 1; i < n; i++) {
        if (cmp(&recs[i], &recs[best]) < 0) {
            best = i;
        }
    }
    return best;
}

int find_max_record(struct Record recs[], int n, RecordCmp cmp) {
    int best;
    int i;
    best = 0;
    for (i = 1; i < n; i++) {
        if (cmp(&recs[i], &recs[best]) > 0) {
            best = i;
        }
    }
    return best;
}

int is_sorted_records(struct Record recs[], int n, RecordCmp cmp) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (cmp(&recs[i], &recs[i + 1]) > 0) return 0;
    }
    return 1;
}

void print_ids(struct Record recs[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", recs[i].id);
    }
    printf("\n");
}

void print_scores(struct Record recs[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", recs[i].score);
    }
    printf("\n");
}

void init_record(struct Record *r, int id, int age, int score, int nc) {
    r->id = id;
    r->age = age;
    r->score = score;
    r->name_code = nc;
}

int main(void) {
    struct Record recs[5];
    int idx;

    init_record(&recs[0], 3, 25, 88, 50);
    init_record(&recs[1], 1, 30, 95, 10);
    init_record(&recs[2], 5, 22, 72, 40);
    init_record(&recs[3], 2, 25, 91, 30);
    init_record(&recs[4], 4, 28, 85, 20);

    sort_records(recs, 5, cmp_by_id);
    // EXPECT: by id: 1 2 3 4 5
    printf("by id: ");
    print_ids(recs, 5);

    sort_records(recs, 5, cmp_by_score);
    // EXPECT: by score: 5 4 3 2 1
    printf("by score: ");
    print_ids(recs, 5);

    // EXPECT: scores asc: 72 85 88 91 95
    printf("scores asc: ");
    print_scores(recs, 5);

    sort_records(recs, 5, cmp_by_score_desc);
    // EXPECT: by score desc: 1 2 3 4 5
    printf("by score desc: ");
    print_ids(recs, 5);

    sort_records(recs, 5, cmp_by_age);
    // EXPECT: by age: 5 2 3 4 1
    printf("by age: ");
    print_ids(recs, 5);

    sort_records(recs, 5, cmp_by_name_code);
    // EXPECT: by name: 1 4 2 5 3
    printf("by name: ");
    print_ids(recs, 5);

    idx = find_min_record(recs, 5, cmp_by_score);
    // EXPECT: min score id: 5
    printf("min score id: %d\n", recs[idx].id);

    idx = find_max_record(recs, 5, cmp_by_score);
    // EXPECT: max score id: 1
    printf("max score id: %d\n", recs[idx].id);

    idx = find_min_record(recs, 5, cmp_by_age);
    // EXPECT: youngest id: 5
    printf("youngest id: %d\n", recs[idx].id);

    idx = find_max_record(recs, 5, cmp_by_age);
    // EXPECT: oldest id: 1
    printf("oldest id: %d\n", recs[idx].id);

    sort_records(recs, 5, cmp_by_id);
    // EXPECT: sorted by id: 1
    printf("sorted by id: %d\n", is_sorted_records(recs, 5, cmp_by_id));

    // EXPECT: sorted by score: 0
    printf("sorted by score: %d\n", is_sorted_records(recs, 5, cmp_by_score));

    sort_records(recs, 5, cmp_age_then_score);
    // EXPECT: age+score: 5 3 2 4 1
    printf("age+score: ");
    print_ids(recs, 5);

    // EXPECT: age+score sorted: 1
    printf("age+score sorted: %d\n", is_sorted_records(recs, 5, cmp_age_then_score));

    // EXPECT: rec0 age: 22
    printf("rec0 age: %d\n", recs[0].age);

    return 0;
}
