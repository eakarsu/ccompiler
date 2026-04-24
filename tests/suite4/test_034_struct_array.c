int printf(const char *fmt, ...);

// Test: Array of structs, iterating over struct array, struct containing array

struct Item {
    int id;
    int value;
};

struct Container {
    int data[5];
    int count;
};

struct Student {
    int scores[4];
    int total;
};

struct Matrix {
    int rows;
    int cols;
    int vals[9];
};

int main(void) {
    // Array of structs: initialize and iterate
    struct Item items[5];
    int i;
    for (i = 0; i < 5; i = i + 1) {
        items[i].id = i + 1;
        items[i].value = (i + 1) * 10;
    }
    // EXPECT: 1 10
    printf("%d %d\n", items[0].id, items[0].value);
    // EXPECT: 2 20
    printf("%d %d\n", items[1].id, items[1].value);
    // EXPECT: 3 30
    printf("%d %d\n", items[2].id, items[2].value);
    // EXPECT: 4 40
    printf("%d %d\n", items[3].id, items[3].value);
    // EXPECT: 5 50
    printf("%d %d\n", items[4].id, items[4].value);

    // Sum of all values in struct array
    int total_val = 0;
    for (i = 0; i < 5; i = i + 1) {
        total_val = total_val + items[i].value;
    }
    // EXPECT: 150
    printf("%d\n", total_val);

    // Struct containing array: Container
    struct Container c;
    c.count = 5;
    for (i = 0; i < 5; i = i + 1) {
        c.data[i] = i * i;
    }
    // EXPECT: 0 1 4 9 16
    printf("%d %d %d %d %d\n", c.data[0], c.data[1], c.data[2], c.data[3], c.data[4]);

    // Sum of container data
    int csum = 0;
    for (i = 0; i < c.count; i = i + 1) {
        csum = csum + c.data[i];
    }
    // EXPECT: 30
    printf("%d\n", csum);

    // Student with scores array
    struct Student s;
    s.scores[0] = 85;
    s.scores[1] = 92;
    s.scores[2] = 78;
    s.scores[3] = 95;
    s.total = 0;
    for (i = 0; i < 4; i = i + 1) {
        s.total = s.total + s.scores[i];
    }
    // EXPECT: 350
    printf("%d\n", s.total);

    // Average (integer division)
    int avg = s.total / 4;
    // EXPECT: 87
    printf("%d\n", avg);

    // Array of Students
    struct Student students[3];
    int j;
    for (i = 0; i < 3; i = i + 1) {
        students[i].total = 0;
        for (j = 0; j < 4; j = j + 1) {
            students[i].scores[j] = (i + 1) * 10 + j;
            students[i].total = students[i].total + students[i].scores[j];
        }
    }
    // Student 0: scores 10,11,12,13 -> total 46
    // EXPECT: 46
    printf("%d\n", students[0].total);
    // Student 1: scores 20,21,22,23 -> total 86
    // EXPECT: 86
    printf("%d\n", students[1].total);
    // Student 2: scores 30,31,32,33 -> total 126
    // EXPECT: 126
    printf("%d\n", students[2].total);

    // Matrix struct with array
    struct Matrix mat;
    mat.rows = 3;
    mat.cols = 3;
    for (i = 0; i < 9; i = i + 1) {
        mat.vals[i] = i + 1;
    }
    // Print diagonal: vals[0], vals[4], vals[8] = 1, 5, 9
    int diag_sum = mat.vals[0] + mat.vals[4] + mat.vals[8];
    // EXPECT: 15
    printf("%d\n", diag_sum);

    // Total of all matrix values: 1+2+...+9 = 45
    int matsum = 0;
    for (i = 0; i < mat.rows * mat.cols; i = i + 1) {
        matsum = matsum + mat.vals[i];
    }
    // EXPECT: 45
    printf("%d\n", matsum);

    // Find max in items array
    int max_val = items[0].value;
    for (i = 1; i < 5; i = i + 1) {
        if (items[i].value > max_val) {
            max_val = items[i].value;
        }
    }
    // EXPECT: 50
    printf("%d\n", max_val);

    // Modify struct array element and verify
    items[2].value = 999;
    // EXPECT: 999
    printf("%d\n", items[2].value);

    // Other elements unchanged
    // EXPECT: 10 20
    printf("%d %d\n", items[0].value, items[1].value);

    return 0;
}
