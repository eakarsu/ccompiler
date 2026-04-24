int printf(const char *fmt, ...);

struct Student {
    char name[16];
    int grades[8];
    int grade_count;
    int id;
};

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int add_student(struct Student *students, int *count, char *name, int id) {
    int idx;
    idx = *count;
    if (idx >= 20) return -1;
    str_copy(students[idx].name, name);
    students[idx].id = id;
    students[idx].grade_count = 0;
    *count = *count + 1;
    return idx;
}

int add_grade(struct Student *s, int grade) {
    if (s->grade_count >= 8) return -1;
    s->grades[s->grade_count] = grade;
    s->grade_count = s->grade_count + 1;
    return s->grade_count;
}

/* Returns average * 100 for integer precision */
int calc_average_x100(struct Student *s) {
    int i;
    int sum;
    if (s->grade_count == 0) return 0;
    sum = 0;
    i = 0;
    while (i < s->grade_count) {
        sum = sum + s->grades[i];
        i = i + 1;
    }
    return (sum * 100) / s->grade_count;
}

int calc_gpa_x100(struct Student *s) {
    int avg;
    avg = calc_average_x100(s);
    /* A: >=90 -> 400, B: >=80 -> 300, C: >=70 -> 200, D: >=60 -> 100, F: <60 -> 0 */
    /* Using avg*100: >=9000 -> 400, etc */
    if (avg >= 9000) return 400;
    if (avg >= 8000) return 300;
    if (avg >= 7000) return 200;
    if (avg >= 6000) return 100;
    return 0;
}

int highest_grade(struct Student *s) {
    int i;
    int best;
    if (s->grade_count == 0) return -1;
    best = s->grades[0];
    i = 1;
    while (i < s->grade_count) {
        if (s->grades[i] > best) best = s->grades[i];
        i = i + 1;
    }
    return best;
}

int lowest_grade(struct Student *s) {
    int i;
    int worst;
    if (s->grade_count == 0) return -1;
    worst = s->grades[0];
    i = 1;
    while (i < s->grade_count) {
        if (s->grades[i] < worst) worst = s->grades[i];
        i = i + 1;
    }
    return worst;
}

int is_honors(struct Student *s) {
    int avg;
    avg = calc_average_x100(s);
    return avg >= 9000;
}

int count_passing(struct Student *s) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < s->grade_count) {
        if (s->grades[i] >= 60) c = c + 1;
        i = i + 1;
    }
    return c;
}

/* Rank students by average (return index of rank-th student, 0-based rank) */
int rank_student(struct Student *students, int count, int rank) {
    int avgs[20];
    int indices[20];
    int i;
    int j;
    int tmp;
    i = 0;
    while (i < count) {
        avgs[i] = calc_average_x100(&students[i]);
        indices[i] = i;
        i = i + 1;
    }
    /* Sort descending by average */
    i = 0;
    while (i < count - 1) {
        j = 0;
        while (j < count - 1 - i) {
            if (avgs[j] < avgs[j + 1]) {
                tmp = avgs[j]; avgs[j] = avgs[j + 1]; avgs[j + 1] = tmp;
                tmp = indices[j]; indices[j] = indices[j + 1]; indices[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (rank < count) return indices[rank];
    return -1;
}

int class_average_x100(struct Student *students, int count) {
    int i;
    int total;
    if (count == 0) return 0;
    total = 0;
    i = 0;
    while (i < count) {
        total = total + calc_average_x100(&students[i]);
        i = i + 1;
    }
    return total / count;
}

int count_honors(struct Student *students, int count) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (is_honors(&students[i])) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main(void) {
    struct Student students[20];
    int count;
    int idx;
    int result;

    count = 0;

    idx = add_student(students, &count, "Alice", 101);
    add_grade(&students[idx], 95);
    add_grade(&students[idx], 88);
    add_grade(&students[idx], 92);
    add_grade(&students[idx], 90);

    idx = add_student(students, &count, "Bob", 102);
    add_grade(&students[idx], 75);
    add_grade(&students[idx], 80);
    add_grade(&students[idx], 70);
    add_grade(&students[idx], 65);

    idx = add_student(students, &count, "Charlie", 103);
    add_grade(&students[idx], 55);
    add_grade(&students[idx], 60);
    add_grade(&students[idx], 50);
    add_grade(&students[idx], 45);

    idx = add_student(students, &count, "Diana", 104);
    add_grade(&students[idx], 98);
    add_grade(&students[idx], 95);
    add_grade(&students[idx], 97);
    add_grade(&students[idx], 100);

    // EXPECT: 4
    printf("%d\n", count);

    /* Alice average: (95+88+92+90)/4 = 365/4 = 91.25 -> 9125 */
    // EXPECT: 9125
    printf("%d\n", calc_average_x100(&students[0]));

    /* Bob average: (75+80+70+65)/4 = 290/4 = 72.50 -> 7250 */
    // EXPECT: 7250
    printf("%d\n", calc_average_x100(&students[1]));

    /* Charlie average: (55+60+50+45)/4 = 210/4 = 52.50 -> 5250 */
    // EXPECT: 5250
    printf("%d\n", calc_average_x100(&students[2]));

    /* Diana average: (98+95+97+100)/4 = 390/4 = 97.50 -> 9750 */
    // EXPECT: 9750
    printf("%d\n", calc_average_x100(&students[3]));

    /* GPA: Alice 400, Bob 200, Charlie 0, Diana 400 */
    // EXPECT: 400
    printf("%d\n", calc_gpa_x100(&students[0]));
    // EXPECT: 200
    printf("%d\n", calc_gpa_x100(&students[1]));
    // EXPECT: 0
    printf("%d\n", calc_gpa_x100(&students[2]));
    // EXPECT: 400
    printf("%d\n", calc_gpa_x100(&students[3]));

    /* Highest/lowest for Alice */
    // EXPECT: 95
    printf("%d\n", highest_grade(&students[0]));
    // EXPECT: 88
    printf("%d\n", lowest_grade(&students[0]));

    /* Honors: Alice yes, Bob no */
    // EXPECT: 1
    printf("%d\n", is_honors(&students[0]));
    // EXPECT: 0
    printf("%d\n", is_honors(&students[1]));

    /* Passing grades for Charlie: 55(no), 60(yes), 50(no), 45(no) = 1 */
    // EXPECT: 1
    printf("%d\n", count_passing(&students[2]));

    /* Rank 0 (top) = Diana (index 3), Rank 1 = Alice (index 0) */
    // EXPECT: 3
    printf("%d\n", rank_student(students, count, 0));
    // EXPECT: 0
    printf("%d\n", rank_student(students, count, 1));

    /* Class average: (9125+7250+5250+9750)/4 = 31375/4 = 7843 */
    // EXPECT: 7843
    printf("%d\n", class_average_x100(students, count));

    /* Count honors: Alice + Diana = 2 */
    // EXPECT: 2
    printf("%d\n", count_honors(students, count));

    /* Diana highest */
    // EXPECT: 100
    printf("%d\n", highest_grade(&students[3]));

    return 0;
}
