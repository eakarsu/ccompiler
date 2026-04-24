int printf(const char *fmt, ...);
// EXPECT: Student 1: scores=[92,88,95,90,87] total=452 avg=90 grade=A\nStudent 2: scores=[78,85,72,80,76] total=391 avg=78 grade=C\nStudent 3: scores=[65,70,68,72,60] total=335 avg=67 grade=D\nStudent 4: scores=[55,48,62,45,50] total=260 avg=52 grade=F\nStudent 5: scores=[98,95,97,99,96] total=485 avg=97 grade=A\nhighest: Student 5 (total=485)\nlowest: Student 4 (total=260)\nclass_avg=76\nA count=2\nB count=0\nC count=1\nD count=1\nF count=1
// Test 245: Complex multi-function program - student grade system

struct Student {
    int id;
    int scores[5];
    int total;
    int average;
    int grade;  // 0=F, 1=D, 2=C, 3=B, 4=A
};

int compute_total(int *scores, int n) {
    int sum = 0;
    int i = 0;
    while (i < n) { sum = sum + scores[i]; i = i + 1; }
    return sum;
}

int compute_average(int total, int count) {
    return total / count;
}

int compute_grade(int average) {
    if (average >= 90) return 4;
    if (average >= 80) return 3;
    if (average >= 70) return 2;
    if (average >= 60) return 1;
    return 0;
}

char grade_letter(int grade) {
    if (grade == 4) return 'A';
    if (grade == 3) return 'B';
    if (grade == 2) return 'C';
    if (grade == 1) return 'D';
    return 'F';
}

void process_student(struct Student *s) {
    s->total = compute_total(s->scores, 5);
    s->average = compute_average(s->total, 5);
    s->grade = compute_grade(s->average);
}

int find_highest_scorer(struct Student *students, int n) {
    int best_idx = 0;
    int i = 1;
    while (i < n) {
        if (students[i].total > students[best_idx].total) best_idx = i;
        i = i + 1;
    }
    return best_idx;
}

int find_lowest_scorer(struct Student *students, int n) {
    int worst_idx = 0;
    int i = 1;
    while (i < n) {
        if (students[i].total < students[worst_idx].total) worst_idx = i;
        i = i + 1;
    }
    return worst_idx;
}

int class_average(struct Student *students, int n) {
    int sum = 0;
    int i = 0;
    while (i < n) { sum = sum + students[i].average; i = i + 1; }
    return sum / n;
}

int count_grade(struct Student *students, int n, int grade) {
    int c = 0;
    int i = 0;
    while (i < n) {
        if (students[i].grade == grade) c = c + 1;
        i = i + 1;
    }
    return c;
}

void print_student(struct Student *s) {
    printf("Student %d: scores=[%d,%d,%d,%d,%d] total=%d avg=%d grade=%c\n",
           s->id, s->scores[0], s->scores[1], s->scores[2],
           s->scores[3], s->scores[4], s->total, s->average,
           grade_letter(s->grade));
}

int main(void) {
    struct Student students[5];
    students[0].id = 1;
    students[0].scores[0]=92; students[0].scores[1]=88; students[0].scores[2]=95;
    students[0].scores[3]=90; students[0].scores[4]=87;

    students[1].id = 2;
    students[1].scores[0]=78; students[1].scores[1]=85; students[1].scores[2]=72;
    students[1].scores[3]=80; students[1].scores[4]=76;

    students[2].id = 3;
    students[2].scores[0]=65; students[2].scores[1]=70; students[2].scores[2]=68;
    students[2].scores[3]=72; students[2].scores[4]=60;

    students[3].id = 4;
    students[3].scores[0]=55; students[3].scores[1]=48; students[3].scores[2]=62;
    students[3].scores[3]=45; students[3].scores[4]=50;

    students[4].id = 5;
    students[4].scores[0]=98; students[4].scores[1]=95; students[4].scores[2]=97;
    students[4].scores[3]=99; students[4].scores[4]=96;

    int i = 0;
    while (i < 5) {
        process_student(&students[i]);
        print_student(&students[i]);
        i = i + 1;
    }

    int best = find_highest_scorer(students, 5);
    int worst = find_lowest_scorer(students, 5);
    printf("highest: Student %d (total=%d)\n", students[best].id, students[best].total);
    printf("lowest: Student %d (total=%d)\n", students[worst].id, students[worst].total);
    printf("class_avg=%d\n", class_average(students, 5));

    printf("A count=%d\n", count_grade(students, 5, 4));
    printf("B count=%d\n", count_grade(students, 5, 3));
    printf("C count=%d\n", count_grade(students, 5, 2));
    printf("D count=%d\n", count_grade(students, 5, 1));
    printf("F count=%d\n", count_grade(students, 5, 0));

    return 0;
}
