int printf(const char *fmt, ...);

/* ---- Enums for grade categories ---- */
enum Grade {
    GRADE_A,
    GRADE_B,
    GRADE_C,
    GRADE_D,
    GRADE_F
};

/* ---- Structs ---- */
struct Student {
    int id;
    int scores[5];    /* 5 subject scores */
    int total;
    int average;
    int grade;        /* enum Grade */
};

struct ClassRoom {
    struct Student students[10];
    int count;
    int class_total;
    int class_average;
    int highest_avg;
    int lowest_avg;
};

/* ---- Helper functions ---- */

int compute_total(int *scores, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + scores[i];
    }
    return sum;
}

int compute_average(int total, int n) {
    if (n == 0) return 0;
    return total / n;
}

int assign_grade(int avg) {
    if (avg >= 90) return GRADE_A;
    if (avg >= 80) return GRADE_B;
    if (avg >= 70) return GRADE_C;
    if (avg >= 60) return GRADE_D;
    return GRADE_F;
}

char *grade_to_string(int g) {
    switch (g) {
        case GRADE_A: return "A";
        case GRADE_B: return "B";
        case GRADE_C: return "C";
        case GRADE_D: return "D";
        case GRADE_F: return "F";
    }
    return "?";
}

void init_student(struct Student *s, int id, int s0, int s1, int s2, int s3, int s4) {
    s->id = id;
    s->scores[0] = s0;
    s->scores[1] = s1;
    s->scores[2] = s2;
    s->scores[3] = s3;
    s->scores[4] = s4;
    s->total = compute_total(s->scores, 5);
    s->average = compute_average(s->total, 5);
    s->grade = assign_grade(s->average);
}

void classroom_init(struct ClassRoom *cr) {
    cr->count = 0;
    cr->class_total = 0;
    cr->class_average = 0;
    cr->highest_avg = 0;
    cr->lowest_avg = 100;
}

void classroom_add(struct ClassRoom *cr, int id, int s0, int s1, int s2, int s3, int s4) {
    if (cr->count >= 10) return;
    init_student(&cr->students[cr->count], id, s0, s1, s2, s3, s4);
    cr->count = cr->count + 1;
}

void classroom_compute_stats(struct ClassRoom *cr) {
    int i;
    cr->class_total = 0;
    cr->highest_avg = 0;
    cr->lowest_avg = 100;
    for (i = 0; i < cr->count; i = i + 1) {
        cr->class_total = cr->class_total + cr->students[i].total;
        if (cr->students[i].average > cr->highest_avg) {
            cr->highest_avg = cr->students[i].average;
        }
        if (cr->students[i].average < cr->lowest_avg) {
            cr->lowest_avg = cr->students[i].average;
        }
    }
    if (cr->count > 0) {
        cr->class_average = cr->class_total / (cr->count * 5);
    }
}

int classroom_count_grade(struct ClassRoom *cr, int grade) {
    int count = 0;
    int i;
    for (i = 0; i < cr->count; i = i + 1) {
        if (cr->students[i].grade == grade) {
            count = count + 1;
        }
    }
    return count;
}

int classroom_find_top_student(struct ClassRoom *cr) {
    if (cr->count == 0) return -1;
    int best = 0;
    int i;
    for (i = 1; i < cr->count; i = i + 1) {
        if (cr->students[i].average > cr->students[best].average) {
            best = i;
        }
    }
    return cr->students[best].id;
}

/* Sort students by average (descending) using bubble sort */
void classroom_sort_by_avg(struct ClassRoom *cr) {
    int i;
    int j;
    for (i = 0; i < cr->count - 1; i = i + 1) {
        for (j = 0; j < cr->count - 1 - i; j = j + 1) {
            if (cr->students[j].average < cr->students[j + 1].average) {
                struct Student temp = cr->students[j];
                cr->students[j] = cr->students[j + 1];
                cr->students[j + 1] = temp;
            }
        }
    }
}

/* Count students above a threshold */
int classroom_count_above(struct ClassRoom *cr, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < cr->count; i = i + 1) {
        if (cr->students[i].average >= threshold) {
            count = count + 1;
        }
    }
    return count;
}

/* Find subject with highest class average */
int classroom_best_subject(struct ClassRoom *cr) {
    if (cr->count == 0) return -1;
    int subject_totals[5];
    int s;
    int i;
    for (s = 0; s < 5; s = s + 1) {
        subject_totals[s] = 0;
        for (i = 0; i < cr->count; i = i + 1) {
            subject_totals[s] = subject_totals[s] + cr->students[i].scores[s];
        }
    }
    int best = 0;
    for (s = 1; s < 5; s = s + 1) {
        if (subject_totals[s] > subject_totals[best]) {
            best = s;
        }
    }
    return best;
}

int main(void) {
    struct ClassRoom cr;
    classroom_init(&cr);

    /* Add 8 students with 5 subject scores each */
    classroom_add(&cr, 101, 95, 88, 92, 85, 90);   /* avg = 450/5 = 90 -> A */
    classroom_add(&cr, 102, 78, 82, 75, 80, 85);   /* avg = 400/5 = 80 -> B */
    classroom_add(&cr, 103, 65, 70, 72, 68, 60);   /* avg = 335/5 = 67 -> D */
    classroom_add(&cr, 104, 92, 95, 98, 90, 100);  /* avg = 475/5 = 95 -> A */
    classroom_add(&cr, 105, 55, 60, 50, 58, 52);   /* avg = 275/5 = 55 -> F */
    classroom_add(&cr, 106, 88, 85, 90, 82, 80);   /* avg = 425/5 = 85 -> B */
    classroom_add(&cr, 107, 70, 75, 72, 78, 80);   /* avg = 375/5 = 75 -> C */
    classroom_add(&cr, 108, 45, 50, 55, 40, 35);   /* avg = 225/5 = 45 -> F */

    // EXPECT: Student count: 8
    printf("Student count: %d\n", cr.count);

    /* Check individual students */
    // EXPECT: Student 101 total: 450
    printf("Student 101 total: %d\n", cr.students[0].total);
    // EXPECT: Student 101 avg: 90
    printf("Student 101 avg: %d\n", cr.students[0].average);
    // EXPECT: Student 101 grade: A
    printf("Student 101 grade: %s\n", grade_to_string(cr.students[0].grade));

    // EXPECT: Student 104 total: 475
    printf("Student 104 total: %d\n", cr.students[3].total);
    // EXPECT: Student 104 avg: 95
    printf("Student 104 avg: %d\n", cr.students[3].average);
    // EXPECT: Student 104 grade: A
    printf("Student 104 grade: %s\n", grade_to_string(cr.students[3].grade));

    // EXPECT: Student 105 avg: 55
    printf("Student 105 avg: %d\n", cr.students[4].average);
    // EXPECT: Student 105 grade: F
    printf("Student 105 grade: %s\n", grade_to_string(cr.students[4].grade));

    // EXPECT: Student 107 avg: 75
    printf("Student 107 avg: %d\n", cr.students[6].average);
    // EXPECT: Student 107 grade: C
    printf("Student 107 grade: %s\n", grade_to_string(cr.students[6].grade));

    /* Compute class statistics */
    classroom_compute_stats(&cr);

    /* class_total = 450+400+335+475+275+425+375+225 = 2960 */
    // EXPECT: Class total: 2960
    printf("Class total: %d\n", cr.class_total);

    /* class_average = 2960 / (8*5) = 2960 / 40 = 74 */
    // EXPECT: Class average: 74
    printf("Class average: %d\n", cr.class_average);

    // EXPECT: Highest avg: 95
    printf("Highest avg: %d\n", cr.highest_avg);
    // EXPECT: Lowest avg: 45
    printf("Lowest avg: %d\n", cr.lowest_avg);

    /* Grade distribution */
    // EXPECT: Grade A count: 2
    printf("Grade A count: %d\n", classroom_count_grade(&cr, GRADE_A));
    // EXPECT: Grade B count: 2
    printf("Grade B count: %d\n", classroom_count_grade(&cr, GRADE_B));
    // EXPECT: Grade C count: 1
    printf("Grade C count: %d\n", classroom_count_grade(&cr, GRADE_C));
    // EXPECT: Grade D count: 1
    printf("Grade D count: %d\n", classroom_count_grade(&cr, GRADE_D));
    // EXPECT: Grade F count: 2
    printf("Grade F count: %d\n", classroom_count_grade(&cr, GRADE_F));

    /* Find top student */
    int top = classroom_find_top_student(&cr);
    // EXPECT: Top student: 104
    printf("Top student: %d\n", top);

    /* Count above threshold */
    // EXPECT: Above 80: 4
    printf("Above 80: %d\n", classroom_count_above(&cr, 80));
    // EXPECT: Above 70: 5
    printf("Above 70: %d\n", classroom_count_above(&cr, 70));
    // EXPECT: Above 90: 2
    printf("Above 90: %d\n", classroom_count_above(&cr, 90));

    /* Find best subject */
    /* Subject totals:
       S0: 95+78+65+92+55+88+70+45 = 588
       S1: 88+82+70+95+60+85+75+50 = 605
       S2: 92+75+72+98+50+90+72+55 = 604
       S3: 85+80+68+90+58+82+78+40 = 581
       S4: 90+85+60+100+52+80+80+35 = 582
       Best is S1 (605) -> index 1
    */
    int best_subj = classroom_best_subject(&cr);
    // EXPECT: Best subject index: 1
    printf("Best subject index: %d\n", best_subj);

    /* Sort by average descending and check order */
    classroom_sort_by_avg(&cr);
    // EXPECT: Rank 1 id: 104
    printf("Rank 1 id: %d\n", cr.students[0].id);
    // EXPECT: Rank 2 id: 101
    printf("Rank 2 id: %d\n", cr.students[1].id);
    // EXPECT: Rank 3 id: 106
    printf("Rank 3 id: %d\n", cr.students[2].id);
    // EXPECT: Last rank id: 108
    printf("Last rank id: %d\n", cr.students[7].id);

    /* Verify sorted order */
    int sorted = 1;
    int i;
    for (i = 0; i < cr.count - 1; i = i + 1) {
        if (cr.students[i].average < cr.students[i + 1].average) {
            sorted = 0;
        }
    }
    // EXPECT: Sorted correctly: 1
    printf("Sorted correctly: %d\n", sorted);

    /* Pass/fail summary using ternary */
    int pass = 0;
    int fail = 0;
    for (i = 0; i < cr.count; i = i + 1) {
        int is_pass = cr.students[i].average >= 60 ? 1 : 0;
        pass = pass + is_pass;
        fail = fail + (1 - is_pass);
    }
    // EXPECT: Pass: 6
    printf("Pass: %d\n", pass);
    // EXPECT: Fail: 2
    printf("Fail: %d\n", fail);

    return 0;
}
