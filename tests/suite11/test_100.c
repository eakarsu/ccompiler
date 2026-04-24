int printf(const char *fmt, ...);

// Test: Complex initializer lists for arrays of structs

struct Point {
    int x;
    int y;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Student {
    int id;
    int grade;
    int score;
};

struct Pair {
    int first;
    int second;
};

struct Color {
    int r;
    int g;
    int b;
};

void print_point(struct Point *p) {
    printf("%d %d\n", p->x, p->y);
}

int rect_area(struct Rect *r) {
    return r->w * r->h;
}

int main() {
    // Array of Point structs with initializer
    struct Point points[5] = {
        {0, 0},
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };

    int i;
    for (i = 0; i < 5; i = i + 1) {
        printf("%d %d\n", points[i].x, points[i].y);
    }
    // EXPECT: 0 0
    // EXPECT: 1 2
    // EXPECT: 3 4
    // EXPECT: 5 6
    // EXPECT: 7 8

    // Sum of all coordinates
    int sum;
    sum = 0;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + points[i].x + points[i].y;
    }
    printf("%d\n", sum); // EXPECT: 36

    // Array of Rect structs
    struct Rect rects[4] = {
        {0, 0, 10, 20},
        {5, 5, 15, 25},
        {10, 10, 30, 40},
        {0, 0, 100, 100}
    };

    // Print areas
    for (i = 0; i < 4; i = i + 1) {
        printf("%d\n", rect_area(&rects[i]));
    }
    // EXPECT: 200
    // EXPECT: 375
    // EXPECT: 1200
    // EXPECT: 10000

    // Total area
    int total;
    total = 0;
    for (i = 0; i < 4; i = i + 1) {
        total = total + rect_area(&rects[i]);
    }
    printf("%d\n", total); // EXPECT: 11775

    // Array of Student structs
    struct Student students[6] = {
        {1001, 10, 85},
        {1002, 10, 92},
        {1003, 11, 78},
        {1004, 11, 95},
        {1005, 12, 88},
        {1006, 12, 91}
    };

    // Average score
    sum = 0;
    for (i = 0; i < 6; i = i + 1) {
        sum = sum + students[i].score;
    }
    printf("%d\n", sum / 6); // EXPECT: 88

    // Find highest score
    int max_score;
    int max_id;
    max_score = students[0].score;
    max_id = students[0].id;
    for (i = 1; i < 6; i = i + 1) {
        if (students[i].score > max_score) {
            max_score = students[i].score;
            max_id = students[i].id;
        }
    }
    printf("%d\n", max_id);    // EXPECT: 1004
    printf("%d\n", max_score); // EXPECT: 95

    // Count students in grade 11
    int count;
    count = 0;
    for (i = 0; i < 6; i = i + 1) {
        if (students[i].grade == 11) {
            count = count + 1;
        }
    }
    printf("%d\n", count); // EXPECT: 2

    // Array of pairs with sorting (bubble sort by first element)
    struct Pair pairs[5] = {
        {50, 5},
        {20, 2},
        {40, 4},
        {10, 1},
        {30, 3}
    };

    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 4 - i; j = j + 1) {
            if (pairs[j].first > pairs[j + 1].first) {
                struct Pair temp;
                temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }

    // Print sorted pairs
    for (i = 0; i < 5; i = i + 1) {
        printf("%d %d\n", pairs[i].first, pairs[i].second);
    }
    // EXPECT: 10 1
    // EXPECT: 20 2
    // EXPECT: 30 3
    // EXPECT: 40 4
    // EXPECT: 50 5

    // Array of colors
    struct Color colors[4] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {128, 128, 128}
    };

    // Print color intensities (r+g+b)
    for (i = 0; i < 4; i = i + 1) {
        int intensity;
        intensity = colors[i].r + colors[i].g + colors[i].b;
        printf("%d\n", intensity);
    }
    // EXPECT: 255
    // EXPECT: 255
    // EXPECT: 255
    // EXPECT: 384

    // Find brightest color (highest total)
    int max_intensity;
    int max_idx;
    max_intensity = 0;
    max_idx = 0;
    for (i = 0; i < 4; i = i + 1) {
        int intensity;
        intensity = colors[i].r + colors[i].g + colors[i].b;
        if (intensity > max_intensity) {
            max_intensity = intensity;
            max_idx = i;
        }
    }
    printf("%d\n", max_idx);       // EXPECT: 3
    printf("%d\n", max_intensity); // EXPECT: 384

    // Modify struct array elements
    points[0].x = 100;
    points[0].y = 200;
    printf("%d %d\n", points[0].x, points[0].y); // EXPECT: 100 200

    // Copy struct from array
    struct Point p;
    p = points[2];
    printf("%d %d\n", p.x, p.y); // EXPECT: 3 4

    return 0;
}
