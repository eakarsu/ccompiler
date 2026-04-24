int printf(const char *fmt, ...);
// EXPECT: === Gradebook System ===\nAlice: avg=90 grade=A\nBob: avg=78 grade=C\nCharlie: avg=67 grade=D\nDiana: avg=95 grade=A\nClass average: 82\nTop student: Diana (95)\nDistribution: A=2 B=0 C=1 D=1 F=0

struct Student {
    char name[20];
    int grades[5];
    int num_grades;
    int average;
};

struct Gradebook {
    struct Student students[6];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] && i < 19) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void add_student(struct Gradebook *gb, const char *name) {
    if (gb->count >= 6) return;
    copy_str(gb->students[gb->count].name, name);
    gb->students[gb->count].num_grades = 0;
    gb->students[gb->count].average = 0;
    gb->count++;
}

void add_grade(struct Gradebook *gb, int student_idx, int grade) {
    if (student_idx < 0 || student_idx >= gb->count) return;
    struct Student *s = &gb->students[student_idx];
    if (s->num_grades >= 5) return;
    s->grades[s->num_grades] = grade;
    s->num_grades++;
}

int calc_average(struct Student *s) {
    if (s->num_grades == 0) return 0;
    int sum = 0;
    int i;
    for (i = 0; i < s->num_grades; i++) {
        sum = sum + s->grades[i];
    }
    s->average = sum / s->num_grades;
    return s->average;
}

char get_letter_grade(int avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

int find_highest(struct Gradebook *gb) {
    int best = -1;
    int best_avg = -1;
    int i;
    for (i = 0; i < gb->count; i++) {
        int avg = calc_average(&gb->students[i]);
        if (avg > best_avg) {
            best_avg = avg;
            best = i;
        }
    }
    return best;
}

int main(void) {
    struct Gradebook gb;
    gb.count = 0;

    add_student(&gb, "Alice");
    add_student(&gb, "Bob");
    add_student(&gb, "Charlie");
    add_student(&gb, "Diana");

    add_grade(&gb, 0, 92); add_grade(&gb, 0, 88); add_grade(&gb, 0, 95);
    add_grade(&gb, 0, 90); add_grade(&gb, 0, 85);
    add_grade(&gb, 1, 78); add_grade(&gb, 1, 82); add_grade(&gb, 1, 75);
    add_grade(&gb, 1, 80); add_grade(&gb, 1, 77);
    add_grade(&gb, 2, 65); add_grade(&gb, 2, 70); add_grade(&gb, 2, 68);
    add_grade(&gb, 2, 72); add_grade(&gb, 2, 60);
    add_grade(&gb, 3, 95); add_grade(&gb, 3, 98); add_grade(&gb, 3, 92);
    add_grade(&gb, 3, 97); add_grade(&gb, 3, 94);

    printf("=== Gradebook System ===\n");

    int i;
    int class_total = 0;
    for (i = 0; i < gb.count; i++) {
        int avg = calc_average(&gb.students[i]);
        class_total = class_total + avg;
        printf("%s: avg=%d grade=%c\n",
               gb.students[i].name, avg, get_letter_grade(avg));
    }

    printf("Class average: %d\n", class_total / gb.count);

    int top = find_highest(&gb);
    printf("Top student: %s (%d)\n", gb.students[top].name, gb.students[top].average);

    int grade_dist[5];
    int j;
    for (j = 0; j < 5; j++) grade_dist[j] = 0;
    for (i = 0; i < gb.count; i++) {
        char g = get_letter_grade(gb.students[i].average);
        if (g == 'A') grade_dist[0]++;
        else if (g == 'B') grade_dist[1]++;
        else if (g == 'C') grade_dist[2]++;
        else if (g == 'D') grade_dist[3]++;
        else grade_dist[4]++;
    }
    printf("Distribution: A=%d B=%d C=%d D=%d F=%d\n",
           grade_dist[0], grade_dist[1], grade_dist[2], grade_dist[3], grade_dist[4]);

    return 0;
}
