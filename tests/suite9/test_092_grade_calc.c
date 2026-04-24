int printf(const char *fmt, ...);

struct Student {
    char name[32];
    int scores[10];
    int num_scores;
    int total;
    int average;
};

struct Classroom {
    struct Student students[10];
    int count;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void add_student(struct Classroom *cls, const char *name) {
    int idx;
    idx = cls->count;
    str_copy(cls->students[idx].name, name);
    cls->students[idx].num_scores = 0;
    cls->students[idx].total = 0;
    cls->students[idx].average = 0;
    cls->count = cls->count + 1;
}

void add_score(struct Student *s, int score) {
    s->scores[s->num_scores] = score;
    s->num_scores = s->num_scores + 1;
    s->total = s->total + score;
    if (s->num_scores > 0) {
        s->average = s->total / s->num_scores;
    }
}

const char *get_grade(int avg) {
    if (avg >= 90) return "A";
    if (avg >= 80) return "B";
    if (avg >= 70) return "C";
    if (avg >= 60) return "D";
    return "F";
}

int find_highest(struct Classroom *cls) {
    int best;
    int best_idx;
    int i;
    best = -1;
    best_idx = 0;
    for (i = 0; i < cls->count; i = i + 1) {
        if (cls->students[i].average > best) {
            best = cls->students[i].average;
            best_idx = i;
        }
    }
    return best_idx;
}

int find_lowest(struct Classroom *cls) {
    int worst;
    int worst_idx;
    int i;
    worst = 9999;
    worst_idx = 0;
    for (i = 0; i < cls->count; i = i + 1) {
        if (cls->students[i].average < worst) {
            worst = cls->students[i].average;
            worst_idx = i;
        }
    }
    return worst_idx;
}

int class_average(struct Classroom *cls) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < cls->count; i = i + 1) {
        sum = sum + cls->students[i].average;
    }
    if (cls->count > 0) return sum / cls->count;
    return 0;
}

void print_report(struct Classroom *cls) {
    int i;
    for (i = 0; i < cls->count; i = i + 1) {
        printf("  %s: avg=%d grade=%s\n",
               cls->students[i].name,
               cls->students[i].average,
               get_grade(cls->students[i].average));
    }
}

int count_passing(struct Classroom *cls) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < cls->count; i = i + 1) {
        if (cls->students[i].average >= 60) c = c + 1;
    }
    return c;
}

int main() {
    struct Classroom cls;
    int hi;
    int lo;
    cls.count = 0;

    // EXPECT: === Grade Calculator ===
    printf("=== Grade Calculator ===\n");

    add_student(&cls, "Alice");
    add_student(&cls, "Bob");
    add_student(&cls, "Charlie");
    add_student(&cls, "Diana");
    add_student(&cls, "Eve");

    add_score(&cls.students[0], 95);
    add_score(&cls.students[0], 88);
    add_score(&cls.students[0], 92);

    add_score(&cls.students[1], 72);
    add_score(&cls.students[1], 68);
    add_score(&cls.students[1], 75);

    add_score(&cls.students[2], 85);
    add_score(&cls.students[2], 90);
    add_score(&cls.students[2], 88);

    add_score(&cls.students[3], 55);
    add_score(&cls.students[3], 60);
    add_score(&cls.students[3], 58);

    add_score(&cls.students[4], 98);
    add_score(&cls.students[4], 95);
    add_score(&cls.students[4], 100);

    // EXPECT: Student Report:
    printf("Student Report:\n");
    // EXPECT:   Alice: avg=91 grade=A
    // EXPECT:   Bob: avg=71 grade=C
    // EXPECT:   Charlie: avg=87 grade=B
    // EXPECT:   Diana: avg=57 grade=F
    // EXPECT:   Eve: avg=97 grade=A
    print_report(&cls);

    hi = find_highest(&cls);
    lo = find_lowest(&cls);
    // EXPECT: Highest: Eve with avg 97
    printf("Highest: %s with avg %d\n", cls.students[hi].name, cls.students[hi].average);
    // EXPECT: Lowest: Diana with avg 57
    printf("Lowest: %s with avg %d\n", cls.students[lo].name, cls.students[lo].average);
    // EXPECT: Class average: 80
    printf("Class average: %d\n", class_average(&cls));
    // EXPECT: Passing students: 4
    printf("Passing students: %d\n", count_passing(&cls));

    add_score(&cls.students[3], 80);
    // EXPECT: Diana after extra score: avg=63 grade=D
    printf("Diana after extra score: avg=%d grade=%s\n",
           cls.students[3].average, get_grade(cls.students[3].average));

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
