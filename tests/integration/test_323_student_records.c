int printf(const char *fmt, ...);
// EXPECT: Student 1001: [85, 90, 78] avg=84 high=90 low=78 pass=1\nStudent 1002: [55, 62, 48] avg=55 high=62 low=48 pass=0\nStudent 1003: [95, 88, 92] avg=91 high=95 low=88 pass=1\nStudent 1004: [70, 65, 72] avg=69 high=72 low=65 pass=1\nclass average: 74\npassing: 3/4
// Test: struct for student records

struct Student {
    int id;
    int scores[5];
    int num_scores;
};

void student_init(struct Student *s, int id) {
    s->id = id;
    s->num_scores = 0;
}

void student_add_score(struct Student *s, int score) {
    if (s->num_scores < 5) {
        s->scores[s->num_scores] = score;
        s->num_scores = s->num_scores + 1;
    }
}

int student_average(struct Student *s) {
    if (s->num_scores == 0) return 0;
    int sum = 0;
    int i;
    for (i = 0; i < s->num_scores; i++) {
        sum = sum + s->scores[i];
    }
    return sum / s->num_scores;
}

int student_highest(struct Student *s) {
    if (s->num_scores == 0) return 0;
    int max = s->scores[0];
    int i;
    for (i = 1; i < s->num_scores; i++) {
        if (s->scores[i] > max) max = s->scores[i];
    }
    return max;
}

int student_lowest(struct Student *s) {
    if (s->num_scores == 0) return 0;
    int min = s->scores[0];
    int i;
    for (i = 1; i < s->num_scores; i++) {
        if (s->scores[i] < min) min = s->scores[i];
    }
    return min;
}

int student_pass(struct Student *s) {
    return student_average(s) >= 60;
}

void student_print(struct Student *s) {
    printf("Student %d: [", s->id);
    int i;
    for (i = 0; i < s->num_scores; i++) {
        if (i > 0) printf(", ");
        printf("%d", s->scores[i]);
    }
    printf("] avg=%d high=%d low=%d pass=%d\n",
           student_average(s), student_highest(s),
           student_lowest(s), student_pass(s));
}

int class_average(struct Student students[], int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + student_average(&students[i]);
    }
    return total / n;
}

int count_passing(struct Student students[], int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (student_pass(&students[i])) count = count + 1;
    }
    return count;
}

int main(void) {
    struct Student students[4];
    student_init(&students[0], 1001);
    student_add_score(&students[0], 85);
    student_add_score(&students[0], 90);
    student_add_score(&students[0], 78);

    student_init(&students[1], 1002);
    student_add_score(&students[1], 55);
    student_add_score(&students[1], 62);
    student_add_score(&students[1], 48);

    student_init(&students[2], 1003);
    student_add_score(&students[2], 95);
    student_add_score(&students[2], 88);
    student_add_score(&students[2], 92);

    student_init(&students[3], 1004);
    student_add_score(&students[3], 70);
    student_add_score(&students[3], 65);
    student_add_score(&students[3], 72);

    int i;
    for (i = 0; i < 4; i++) {
        student_print(&students[i]);
    }

    printf("class average: %d\n", class_average(students, 4));
    printf("passing: %d/%d\n", count_passing(students, 4), 4);

    return 0;
}
