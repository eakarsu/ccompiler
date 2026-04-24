int printf(const char *fmt, ...);
// EXPECT: Student 1: grade=85 credits=30\nStudent 2: grade=92 credits=28\nStudent 3: grade=78 credits=32\nStudent 4: grade=95 credits=29\nStudent 5: grade=88 credits=31\nHighest grade: 95\nAverage grade: 87\nTotal credits: 150\nAbove 85: 4\nAbove 90: 2
// Test: array of structs

struct Student {
    int id;
    int grade;
    int credits;
};

int find_highest_grade(struct Student students[], int n) {
    int max = students[0].grade;
    int i;
    for (i = 1; i < n; i++) {
        if (students[i].grade > max) {
            max = students[i].grade;
        }
    }
    return max;
}

int average_grade(struct Student students[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + students[i].grade;
    }
    return sum / n;
}

int total_credits(struct Student students[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + students[i].credits;
    }
    return sum;
}

int count_above(struct Student students[], int n, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (students[i].grade >= threshold) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    struct Student students[5];
    students[0].id = 1; students[0].grade = 85; students[0].credits = 30;
    students[1].id = 2; students[1].grade = 92; students[1].credits = 28;
    students[2].id = 3; students[2].grade = 78; students[2].credits = 32;
    students[3].id = 4; students[3].grade = 95; students[3].credits = 29;
    students[4].id = 5; students[4].grade = 88; students[4].credits = 31;

    int i;
    for (i = 0; i < 5; i++) {
        printf("Student %d: grade=%d credits=%d\n",
               students[i].id, students[i].grade, students[i].credits);
    }

    printf("Highest grade: %d\n", find_highest_grade(students, 5));
    printf("Average grade: %d\n", average_grade(students, 5));
    printf("Total credits: %d\n", total_credits(students, 5));
    printf("Above 85: %d\n", count_above(students, 5, 85));
    printf("Above 90: %d\n", count_above(students, 5, 90));

    return 0;
}
