int printf(const char *fmt, ...);
// EXPECT: v1+v2 = (4, 6)\nv1*3 = (9, 12)\ndot = 11\nmag^2(v1) = 25\navg = 85\npassed = 1\ng1 avg = 85\ng2 avg = 71\ng1 better = 1\nweighted = 85\nmath diff = 15\ntotal diff = 40\nsum = (0, 0)
// Test: typedef'd types in arithmetic

typedef int score_t;
typedef int coord_t;
typedef int bool_t;

typedef struct {
    coord_t x;
    coord_t y;
} vec2;

typedef struct {
    score_t math;
    score_t science;
    score_t english;
} grades;

vec2 vec2_add(vec2 a, vec2 b) {
    vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2 vec2_scale(vec2 v, int s) {
    vec2 result;
    result.x = v.x * s;
    result.y = v.y * s;
    return result;
}

coord_t vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

score_t average_grade(grades g) {
    return (g.math + g.science + g.english) / 3;
}

int main(void) {
    vec2 v1, v2, v3;
    grades g1, g2;
    score_t avg;
    bool_t passed;
    int i;

    /* Vector operations */
    v1.x = 3; v1.y = 4;
    v2.x = 1; v2.y = 2;

    v3 = vec2_add(v1, v2);
    printf("v1+v2 = (%d, %d)\n", v3.x, v3.y);

    v3 = vec2_scale(v1, 3);
    printf("v1*3 = (%d, %d)\n", v3.x, v3.y);

    printf("dot = %d\n", vec2_dot(v1, v2));

    /* Magnitude squared */
    printf("mag^2(v1) = %d\n", vec2_dot(v1, v1));

    /* Grade computations */
    g1.math = 85;
    g1.science = 92;
    g1.english = 78;

    avg = average_grade(g1);
    printf("avg = %d\n", avg);

    passed = avg >= 60;
    printf("passed = %d\n", passed);

    /* Compare grades */
    g2.math = 70;
    g2.science = 65;
    g2.english = 80;

    printf("g1 avg = %d\n", average_grade(g1));
    printf("g2 avg = %d\n", average_grade(g2));
    printf("g1 better = %d\n", average_grade(g1) > average_grade(g2));

    /* Weighted average */
    {
        score_t weighted;
        weighted = (g1.math * 40 + g1.science * 35 + g1.english * 25) / 100;
        printf("weighted = %d\n", weighted);
    }

    /* Grade difference */
    printf("math diff = %d\n", g1.math - g2.math);
    printf("total diff = %d\n",
           (g1.math + g1.science + g1.english) -
           (g2.math + g2.science + g2.english));

    /* Array of vectors */
    {
        vec2 pts[4];
        vec2 sum;
        pts[0].x = 1; pts[0].y = 0;
        pts[1].x = 0; pts[1].y = 1;
        pts[2].x = -1; pts[2].y = 0;
        pts[3].x = 0; pts[3].y = -1;

        sum.x = 0; sum.y = 0;
        for (i = 0; i < 4; i++) {
            sum = vec2_add(sum, pts[i]);
        }
        printf("sum = (%d, %d)\n", sum.x, sum.y);
    }

    return 0;
}
