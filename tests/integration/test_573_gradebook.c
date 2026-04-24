int printf(const char *fmt, ...);
// EXPECT: Grade Book Report:\n  Alice: grades=[95,88,92,90] avg=91 grade=A\n  Bob: grades=[78,82,75,80] avg=78 grade=C\n  Charlie: grades=[65,70,68,72] avg=68 grade=D\n  Diana: grades=[90,95,88,93] avg=91 grade=A\n  Eve: grades=[55,60,58,52] avg=56 grade=F\nClass average: 76\nTop student: Alice (91)

#define MAX_STUDENTS 5
#define MAX_GRADES 4

struct Student {
    char *name;
    int grades[MAX_GRADES];
    int num_grades;
    int total;
    int average;
    char letter;
};

void student_init(struct Student *s, char *name) {
    s->name = name;
    s->num_grades = 0;
    s->total = 0;
    s->average = 0;
    s->letter = 'F';
}

void student_add_grade(struct Student *s, int grade) {
    if (s->num_grades < MAX_GRADES) {
        s->grades[s->num_grades] = grade;
        s->num_grades++;
        s->total = s->total + grade;
        s->average = s->total / s->num_grades;
    }
}

char compute_letter(int avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

void student_finalize(struct Student *s) {
    s->letter = compute_letter(s->average);
}

void student_print(struct Student *s) {
    printf("  %s: grades=[", s->name);
    int i;
    for (i = 0; i < s->num_grades; i++) {
        if (i > 0) printf(",");
        printf("%d", s->grades[i]);
    }
    printf("] avg=%d grade=%c\n", s->average, s->letter);
}

struct GradeBook {
    struct Student students[MAX_STUDENTS];
    int count;
};

void gb_init(struct GradeBook *gb) {
    gb->count = 0;
}

int gb_add_student(struct GradeBook *gb, char *name) {
    if (gb->count >= MAX_STUDENTS) return -1;
    student_init(&gb->students[gb->count], name);
    gb->count++;
    return gb->count - 1;
}

void gb_class_average(struct GradeBook *gb) {
    int total = 0;
    int i;
    for (i = 0; i < gb->count; i++) {
        total = total + gb->students[i].average;
    }
    printf("Class average: %d\n", total / gb->count);
}

int gb_highest(struct GradeBook *gb) {
    int best = 0;
    int i;
    for (i = 1; i < gb->count; i++) {
        if (gb->students[i].average > gb->students[best].average) best = i;
    }
    return best;
}

int main(void) {
    struct GradeBook gb;
    gb_init(&gb);

    int s0 = gb_add_student(&gb, "Alice");
    int s1 = gb_add_student(&gb, "Bob");
    int s2 = gb_add_student(&gb, "Charlie");
    int s3 = gb_add_student(&gb, "Diana");
    int s4 = gb_add_student(&gb, "Eve");

    student_add_grade(&gb.students[s0], 95); student_add_grade(&gb.students[s0], 88);
    student_add_grade(&gb.students[s0], 92); student_add_grade(&gb.students[s0], 90);

    student_add_grade(&gb.students[s1], 78); student_add_grade(&gb.students[s1], 82);
    student_add_grade(&gb.students[s1], 75); student_add_grade(&gb.students[s1], 80);

    student_add_grade(&gb.students[s2], 65); student_add_grade(&gb.students[s2], 70);
    student_add_grade(&gb.students[s2], 68); student_add_grade(&gb.students[s2], 72);

    student_add_grade(&gb.students[s3], 90); student_add_grade(&gb.students[s3], 95);
    student_add_grade(&gb.students[s3], 88); student_add_grade(&gb.students[s3], 93);

    student_add_grade(&gb.students[s4], 55); student_add_grade(&gb.students[s4], 60);
    student_add_grade(&gb.students[s4], 58); student_add_grade(&gb.students[s4], 52);

    int i;
    for (i = 0; i < gb.count; i++) {
        student_finalize(&gb.students[i]);
    }

    printf("Grade Book Report:\n");
    for (i = 0; i < gb.count; i++) {
        student_print(&gb.students[i]);
    }

    gb_class_average(&gb);
    int top = gb_highest(&gb);
    printf("Top student: %s (%d)\n", gb.students[top].name, gb.students[top].average);

    return 0;
}
