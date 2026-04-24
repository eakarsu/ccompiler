int printf(const char *fmt, ...);

/* Struct used in arrays */
struct Student {
    int id;
    int grade;
    int age;
};

/* Struct containing a fixed array */
struct Matrix2x2 {
    int data[4];   /* row-major: [0][0],[0][1],[1][0],[1][1] */
};

/* Struct containing a small array of ints */
struct Stats {
    int vals[5];
    int count;
};

/* Sum grades in an array of structs */
int sum_grades(struct Student arr[], int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total += arr[i].grade;
    }
    return total;
}

/* Find index of max grade */
int max_grade_idx(struct Student arr[], int n) {
    int best = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].grade > arr[best].grade) {
            best = i;
        }
    }
    return best;
}

/* Bubble-sort array of structs by grade ascending */
void sort_by_grade(struct Student arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].grade > arr[j+1].grade) {
                struct Student tmp = arr[j];
                arr[j]   = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

/* Matrix multiply 2x2 (stored in data[4]) into result */
void mat_mul(struct Matrix2x2 *a, struct Matrix2x2 *b, struct Matrix2x2 *out) {
    /* a00*b00 + a01*b10 */
    out->data[0] = a->data[0]*b->data[0] + a->data[1]*b->data[2];
    /* a00*b01 + a01*b11 */
    out->data[1] = a->data[0]*b->data[1] + a->data[1]*b->data[3];
    /* a10*b00 + a11*b10 */
    out->data[2] = a->data[2]*b->data[0] + a->data[3]*b->data[2];
    /* a10*b01 + a11*b11 */
    out->data[3] = a->data[2]*b->data[1] + a->data[3]*b->data[3];
}

int main(void) {
    /* --- Array of structs --- */
    struct Student students[5];
    students[0].id = 1; students[0].grade = 78; students[0].age = 20;
    students[1].id = 2; students[1].grade = 92; students[1].age = 21;
    students[2].id = 3; students[2].grade = 65; students[2].age = 19;
    students[3].id = 4; students[3].grade = 88; students[3].age = 22;
    students[4].id = 5; students[4].grade = 73; students[4].age = 20;

    /* Print all ids and grades */
    int i;
    for (i = 0; i < 5; i++) {
        printf("%d %d\n", students[i].id, students[i].grade);
    }
    // EXPECT: 1 78
    // EXPECT: 2 92
    // EXPECT: 3 65
    // EXPECT: 4 88
    // EXPECT: 5 73

    /* Sum of grades: 78+92+65+88+73 = 396 */
    printf("%d\n", sum_grades(students, 5));  // EXPECT: 396

    /* Index of max grade: index 1 (grade 92) */
    printf("%d\n", max_grade_idx(students, 5)); // EXPECT: 1

    /* Sort by grade ascending */
    sort_by_grade(students, 5);
    /* Sorted grades: 65 73 78 88 92 */
    for (i = 0; i < 5; i++) {
        printf("%d\n", students[i].grade);
    }
    // EXPECT: 65
    // EXPECT: 73
    // EXPECT: 78
    // EXPECT: 88
    // EXPECT: 92

    /* Corresponding ids after sort: 3,5,1,4,2 */
    for (i = 0; i < 5; i++) {
        printf("%d\n", students[i].id);
    }
    // EXPECT: 3
    // EXPECT: 5
    // EXPECT: 1
    // EXPECT: 4
    // EXPECT: 2

    /* --- Struct containing array (Matrix2x2) --- */
    struct Matrix2x2 m1, m2, m3;
    /* m1 = [[1,2],[3,4]] */
    m1.data[0] = 1; m1.data[1] = 2;
    m1.data[2] = 3; m1.data[3] = 4;
    /* m2 = [[5,6],[7,8]] */
    m2.data[0] = 5; m2.data[1] = 6;
    m2.data[2] = 7; m2.data[3] = 8;

    mat_mul(&m1, &m2, &m3);
    /* m3[0][0] = 1*5+2*7 = 5+14 = 19 */
    printf("%d\n", m3.data[0]);              // EXPECT: 19
    /* m3[0][1] = 1*6+2*8 = 6+16 = 22 */
    printf("%d\n", m3.data[1]);              // EXPECT: 22
    /* m3[1][0] = 3*5+4*7 = 15+28 = 43 */
    printf("%d\n", m3.data[2]);              // EXPECT: 43
    /* m3[1][1] = 3*6+4*8 = 18+32 = 50 */
    printf("%d\n", m3.data[3]);              // EXPECT: 50

    /* --- Struct with internal array (Stats) --- */
    struct Stats s;
    s.vals[0] = 10;
    s.vals[1] = 20;
    s.vals[2] = 30;
    s.vals[3] = 40;
    s.vals[4] = 50;
    s.count = 5;

    int total = 0;
    for (i = 0; i < s.count; i++) {
        total += s.vals[i];
    }
    /* 10+20+30+40+50 = 150 */
    printf("%d\n", total);                   // EXPECT: 150

    /* Access individual elements */
    printf("%d\n", s.vals[2]);               // EXPECT: 30
    printf("%d\n", s.vals[4]);               // EXPECT: 50

    /* Modify element and re-sum */
    s.vals[2] = 100;
    int total2 = 0;
    for (i = 0; i < s.count; i++) {
        total2 += s.vals[i];
    }
    /* 10+20+100+40+50 = 220 */
    printf("%d\n", total2);                  // EXPECT: 220

    /* --- Array of structs with initializer list --- */
    struct Student trio[3] = {
        {10, 55, 18},
        {11, 77, 19},
        {12, 66, 20}
    };
    for (i = 0; i < 3; i++) {
        printf("%d\n", trio[i].grade);
    }
    // EXPECT: 55
    // EXPECT: 77
    // EXPECT: 66

    return 0;
}
