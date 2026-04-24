int printf(const char *fmt, ...);

struct Student {
    int id;
    int grade;
    int age;
};

struct Classroom {
    struct Student students[10];
    int count;
    int room_number;
};

void add_student(struct Classroom *c, int id, int grade, int age) {
    int idx = c->count;
    c->students[idx].id = id;
    c->students[idx].grade = grade;
    c->students[idx].age = age;
    c->count = c->count + 1;
}

int avg_grade(struct Classroom *c) {
    int total = 0;
    int i;
    for (i = 0; i < c->count; i = i + 1) {
        total = total + c->students[i].grade;
    }
    if (c->count == 0) return 0;
    return total / c->count;
}

int max_grade(struct Classroom *c) {
    int mx = 0;
    int i;
    for (i = 0; i < c->count; i = i + 1) {
        if (c->students[i].grade > mx) {
            mx = c->students[i].grade;
        }
    }
    return mx;
}

int min_grade(struct Classroom *c) {
    if (c->count == 0) return 0;
    int mn = c->students[0].grade;
    int i;
    for (i = 1; i < c->count; i = i + 1) {
        if (c->students[i].grade < mn) {
            mn = c->students[i].grade;
        }
    }
    return mn;
}

int count_above(struct Classroom *c, int threshold) {
    int cnt = 0;
    int i;
    for (i = 0; i < c->count; i = i + 1) {
        if (c->students[i].grade > threshold) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

void sort_by_grade(struct Classroom *c) {
    int i;
    int j;
    for (i = 0; i < c->count - 1; i = i + 1) {
        for (j = 0; j < c->count - 1 - i; j = j + 1) {
            if (c->students[j].grade > c->students[j + 1].grade) {
                struct Student tmp;
                tmp.id = c->students[j].id;
                tmp.grade = c->students[j].grade;
                tmp.age = c->students[j].age;
                c->students[j].id = c->students[j + 1].id;
                c->students[j].grade = c->students[j + 1].grade;
                c->students[j].age = c->students[j + 1].age;
                c->students[j + 1].id = tmp.id;
                c->students[j + 1].grade = tmp.grade;
                c->students[j + 1].age = tmp.age;
            }
        }
    }
}

void print_classroom(struct Classroom *c) {
    int i;
    for (i = 0; i < c->count; i = i + 1) {
        printf("  Student %d: grade=%d age=%d\n",
               c->students[i].id, c->students[i].grade, c->students[i].age);
    }
}

int total_age(struct Classroom *c) {
    int sum = 0;
    int i;
    for (i = 0; i < c->count; i = i + 1) {
        sum = sum + c->students[i].age;
    }
    return sum;
}

int main() {
    struct Classroom room;
    room.count = 0;
    room.room_number = 101;

    add_student(&room, 1, 85, 15);
    add_student(&room, 2, 92, 16);
    add_student(&room, 3, 78, 15);
    add_student(&room, 4, 95, 17);
    add_student(&room, 5, 88, 16);

    // EXPECT: Room 101, Students: 5
    printf("Room %d, Students: %d\n", room.room_number, room.count);

    // EXPECT: Average grade: 87
    printf("Average grade: %d\n", avg_grade(&room));

    // EXPECT: Max grade: 95
    printf("Max grade: %d\n", max_grade(&room));

    // EXPECT: Min grade: 78
    printf("Min grade: %d\n", min_grade(&room));

    // EXPECT: Above 85: 3
    printf("Above 85: %d\n", count_above(&room, 85));

    // EXPECT: Total age: 79
    printf("Total age: %d\n", total_age(&room));

    sort_by_grade(&room);
    // EXPECT: Sorted by grade:
    printf("Sorted by grade:\n");
    // EXPECT:   Student 3: grade=78 age=15
    // EXPECT:   Student 1: grade=85 age=15
    // EXPECT:   Student 5: grade=88 age=16
    // EXPECT:   Student 2: grade=92 age=16
    // EXPECT:   Student 4: grade=95 age=17
    print_classroom(&room);

    // EXPECT: First after sort: id=3 grade=78
    printf("First after sort: id=%d grade=%d\n", room.students[0].id, room.students[0].grade);

    // EXPECT: Last after sort: id=4 grade=95
    printf("Last after sort: id=%d grade=%d\n",
           room.students[room.count - 1].id, room.students[room.count - 1].grade);

    return 0;
}
