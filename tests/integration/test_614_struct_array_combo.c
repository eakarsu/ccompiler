int printf(const char *fmt, ...);
// EXPECT: === Array of Structs Test ===\nGrades[1]: 90 88 75 65 92\n--- print_student ---\nAlice age 20 grades 85 90 78\nBob age 18 grades 92 88 95\n--- grade sums ---\nAlice total: 253 avg: 84\nBob total: 275 avg: 91\nCarol total: 225 avg: 75\nDave total: 195 avg: 65\nEve total: 278 avg: 92\n--- sorted by age ---\nBob 18\nDave 19\nAlice 20\nEve 21\nCarol 22\n--- max/min age ---\nMax age: 22\nMin age: 18\n--- Vec dot product ---\nv1: 1 2 3\nv2: 4 5 6\nDot product: 32\n--- Vec array sums ---\nSum col 0: 1\nSum col 1: 1\nSum col 2: 1\n--- Vec scale ---\nScaled: 3 6 9\n--- Nested Class ---\nClass count: 3\nMember 0: Tom age 15 grades 80 85 90\nMember 1: Sue age 16 grades 95 90 88\nMember 2: Pat age 14 grades 70 72 68\n--- Class grade avg ---\nTom class avg: 85\nSue class avg: 91\nPat class avg: 70\n--- Modify through pointer ---\nBefore: Bob 18\nAfter: Bob 25\n--- Modify grades through pointer ---\nBefore: 92 88 95\nAfter: 100 88 95\n--- Search by name ---\nFound Bob at index 0\nFound Dave at index 1\n--- Copy struct ---\nCopy: Bob age 25 grades 100 88 95\n--- Struct array element swap ---\nBefore swap: Bob Alice\nAfter swap: Alice Bob\n--- Final check ---\nAll tests passed!

struct Student {
    char name[20];
    int age;
    int grades[3];
};

struct Vec {
    int data[3];
};

struct Class {
    struct Student members[3];
    int count;
};

void print_student(struct Student *s) {
    printf("%s age %d grades %d %d %d\n",
           s->name, s->age, s->grades[0], s->grades[1], s->grades[2]);
}

int grade_sum(struct Student *s) {
    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + s->grades[i];
    }
    return sum;
}

int grade_avg(struct Student *s) {
    return grade_sum(s) / 3;
}

int dot_product(struct Vec *a, struct Vec *b) {
    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + a->data[i] * b->data[i];
    }
    return sum;
}

void scale_vec(struct Vec *v, int factor) {
    int i;
    for (i = 0; i < 3; i++) {
        v->data[i] = v->data[i] * factor;
    }
}

void copy_name(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void init_student(struct Student *s, char *name, int age, int g0, int g1, int g2) {
    copy_name(s->name, name);
    s->age = age;
    s->grades[0] = g0;
    s->grades[1] = g1;
    s->grades[2] = g2;
}

void swap_students(struct Student *a, struct Student *b) {
    struct Student tmp;
    int i;
    /* Copy a to tmp */
    copy_name(tmp.name, a->name);
    tmp.age = a->age;
    for (i = 0; i < 3; i++) tmp.grades[i] = a->grades[i];
    /* Copy b to a */
    copy_name(a->name, b->name);
    a->age = b->age;
    for (i = 0; i < 3; i++) a->grades[i] = b->grades[i];
    /* Copy tmp to b */
    copy_name(b->name, tmp.name);
    b->age = tmp.age;
    for (i = 0; i < 3; i++) b->grades[i] = tmp.grades[i];
}

void bubble_sort_by_age(struct Student *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].age > arr[j + 1].age) {
                swap_students(&arr[j], &arr[j + 1]);
            }
        }
    }
}

int find_max_age(struct Student *arr, int n) {
    int max = arr[0].age;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].age > max) max = arr[i].age;
    }
    return max;
}

int find_min_age(struct Student *arr, int n) {
    int min = arr[0].age;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].age < min) min = arr[i].age;
    }
    return min;
}

int find_by_name(struct Student *arr, int n, char *name) {
    int i;
    for (i = 0; i < n; i++) {
        if (str_equal(arr[i].name, name)) return i;
    }
    return -1;
}

void modify_age(struct Student *s, int new_age) {
    s->age = new_age;
}

void modify_grade(struct Student *s, int idx, int val) {
    s->grades[idx] = val;
}

int main() {
    struct Student students[5];
    struct Vec v1;
    struct Vec v2;
    struct Vec vecs[3];
    struct Class cls;
    struct Student copy;
    int i;
    int j;
    int sum;
    int idx;

    printf("=== Array of Structs Test ===\n");

    /* 1. Initialize array of structs */
    init_student(&students[0], "Alice", 20, 85, 90, 78);
    init_student(&students[1], "Bob",   18, 92, 88, 95);
    init_student(&students[2], "Carol", 22, 70, 75, 80);
    init_student(&students[3], "Dave",  19, 60, 65, 70);
    init_student(&students[4], "Eve",   21, 98, 92, 88);

    /* 2. Double indexing: students[i].grades[1] */
    printf("Grades[1]:");
    for (i = 0; i < 5; i++) {
        printf(" %d", students[i].grades[1]);
    }
    printf("\n");

    /* 3. Pass struct array element by pointer */
    printf("--- print_student ---\n");
    print_student(&students[0]);
    print_student(&students[1]);

    /* 4. Grade sums and averages */
    printf("--- grade sums ---\n");
    for (i = 0; i < 5; i++) {
        printf("%s total: %d avg: %d\n",
               students[i].name, grade_sum(&students[i]), grade_avg(&students[i]));
    }

    /* 5. Bubble sort by age */
    bubble_sort_by_age(students, 5);
    printf("--- sorted by age ---\n");
    for (i = 0; i < 5; i++) {
        printf("%s %d\n", students[i].name, students[i].age);
    }

    /* 6. Max/min age (after sort: 18,19,20,21,22) */
    printf("--- max/min age ---\n");
    printf("Max age: %d\n", find_max_age(students, 5));
    printf("Min age: %d\n", find_min_age(students, 5));

    /* 7. Vec structs and dot product */
    printf("--- Vec dot product ---\n");
    v1.data[0] = 1; v1.data[1] = 2; v1.data[2] = 3;
    v2.data[0] = 4; v2.data[1] = 5; v2.data[2] = 6;
    printf("v1: %d %d %d\n", v1.data[0], v1.data[1], v1.data[2]);
    printf("v2: %d %d %d\n", v2.data[0], v2.data[1], v2.data[2]);
    printf("Dot product: %d\n", dot_product(&v1, &v2));

    /* 8. Array of Vec - column sums */
    printf("--- Vec array sums ---\n");
    vecs[0].data[0] = 1; vecs[0].data[1] = 0; vecs[0].data[2] = 0;
    vecs[1].data[0] = 0; vecs[1].data[1] = 1; vecs[1].data[2] = 0;
    vecs[2].data[0] = 0; vecs[2].data[1] = 0; vecs[2].data[2] = 1;
    for (j = 0; j < 3; j++) {
        sum = 0;
        for (i = 0; i < 3; i++) {
            sum = sum + vecs[i].data[j];
        }
        printf("Sum col %d: %d\n", j, sum);
    }

    /* 9. Scale a vec through pointer */
    printf("--- Vec scale ---\n");
    scale_vec(&v1, 3);
    printf("Scaled: %d %d %d\n", v1.data[0], v1.data[1], v1.data[2]);

    /* 10. Nested struct: Class containing array of Students */
    printf("--- Nested Class ---\n");
    cls.count = 3;
    init_student(&cls.members[0], "Tom", 15, 80, 85, 90);
    init_student(&cls.members[1], "Sue", 16, 95, 90, 88);
    init_student(&cls.members[2], "Pat", 14, 70, 72, 68);
    printf("Class count: %d\n", cls.count);
    for (i = 0; i < cls.count; i++) {
        printf("Member %d: ", i);
        print_student(&cls.members[i]);
    }

    /* Class grade averages */
    printf("--- Class grade avg ---\n");
    for (i = 0; i < cls.count; i++) {
        printf("%s class avg: %d\n", cls.members[i].name, grade_avg(&cls.members[i]));
    }

    /* 11. Modify struct array elements through pointer */
    /* After sort: students[0]=Bob(18), students[1]=Dave(19), students[2]=Alice(20), ... */
    printf("--- Modify through pointer ---\n");
    printf("Before: %s %d\n", students[0].name, students[0].age);
    modify_age(&students[0], 25);
    printf("After: %s %d\n", students[0].name, students[0].age);

    /* 12. Modify grades through pointer */
    printf("--- Modify grades through pointer ---\n");
    printf("Before: %d %d %d\n", students[0].grades[0], students[0].grades[1], students[0].grades[2]);
    modify_grade(&students[0], 0, 100);
    printf("After: %d %d %d\n", students[0].grades[0], students[0].grades[1], students[0].grades[2]);

    /* 13. Search by name */
    printf("--- Search by name ---\n");
    /* After sort: [0]=Bob, [1]=Dave, [2]=Alice, [3]=Eve, [4]=Carol */
    idx = find_by_name(students, 5, "Bob");
    printf("Found Bob at index %d\n", idx);
    idx = find_by_name(students, 5, "Dave");
    printf("Found Dave at index %d\n", idx);

    /* 14. Copy a struct */
    printf("--- Copy struct ---\n");
    copy_name(copy.name, students[0].name);
    copy.age = students[0].age;
    for (i = 0; i < 3; i++) copy.grades[i] = students[0].grades[i];
    printf("Copy: %s age %d grades %d %d %d\n",
           copy.name, copy.age, copy.grades[0], copy.grades[1], copy.grades[2]);

    /* 15. Swap two struct array elements */
    printf("--- Struct array element swap ---\n");
    /* students[0]=Bob, students[2]=Alice */
    printf("Before swap: %s %s\n", students[0].name, students[2].name);
    swap_students(&students[0], &students[2]);
    /* After swap: students[0]=Alice, students[2]=Bob */
    printf("After swap: %s %s\n", students[0].name, students[2].name);

    printf("--- Final check ---\n");
    printf("All tests passed!\n");

    return 0;
}
