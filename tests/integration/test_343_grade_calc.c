int printf(const char *fmt, ...);
// EXPECT: Math:\n  A1: 85/100 = 85% (weight 20)\n  A2: 72/100 = 72% (weight 20)\n  A3: 90/100 = 90% (weight 30)\n  A4: 88/100 = 88% (weight 30)\n  weighted avg: 84%\n  grade: B\n  highest: 90%\n  lowest: 72%\n  need for A: 6\nScience:\n  weighted avg: 81%\n  grade: B\nOverall: 82% = B
// Test: grade calculator with structs

struct Assignment {
    int score;
    int max_score;
    int weight;
};

struct Course {
    struct Assignment assignments[8];
    int num_assignments;
    int total_weight;
};

void course_init(struct Course *c) {
    c->num_assignments = 0;
    c->total_weight = 0;
}

void course_add(struct Course *c, int score, int max_score, int weight) {
    if (c->num_assignments >= 8) return;
    int idx = c->num_assignments;
    c->assignments[idx].score = score;
    c->assignments[idx].max_score = max_score;
    c->assignments[idx].weight = weight;
    c->total_weight = c->total_weight + weight;
    c->num_assignments = c->num_assignments + 1;
}

int course_weighted_avg(struct Course *c) {
    if (c->total_weight == 0) return 0;
    int weighted_sum = 0;
    int i;
    for (i = 0; i < c->num_assignments; i++) {
        int pct = (c->assignments[i].score * 100) / c->assignments[i].max_score;
        weighted_sum = weighted_sum + pct * c->assignments[i].weight;
    }
    return weighted_sum / c->total_weight;
}

int course_highest(struct Course *c) {
    if (c->num_assignments == 0) return 0;
    int max = (c->assignments[0].score * 100) / c->assignments[0].max_score;
    int i;
    for (i = 1; i < c->num_assignments; i++) {
        int pct = (c->assignments[i].score * 100) / c->assignments[i].max_score;
        if (pct > max) max = pct;
    }
    return max;
}

int course_lowest(struct Course *c) {
    if (c->num_assignments == 0) return 0;
    int min = (c->assignments[0].score * 100) / c->assignments[0].max_score;
    int i;
    for (i = 1; i < c->num_assignments; i++) {
        int pct = (c->assignments[i].score * 100) / c->assignments[i].max_score;
        if (pct < min) min = pct;
    }
    return min;
}

char grade_letter(int pct) {
    if (pct >= 90) return 'A';
    if (pct >= 80) return 'B';
    if (pct >= 70) return 'C';
    if (pct >= 60) return 'D';
    return 'F';
}

int points_needed(struct Course *c, int target) {
    int current = course_weighted_avg(c);
    return target - current;
}

int main(void) {
    struct Course math;
    course_init(&math);
    course_add(&math, 85, 100, 20);
    course_add(&math, 72, 100, 20);
    course_add(&math, 90, 100, 30);
    course_add(&math, 88, 100, 30);

    printf("Math:\n");
    int i;
    for (i = 0; i < math.num_assignments; i++) {
        int pct = (math.assignments[i].score * 100) / math.assignments[i].max_score;
        printf("  A%d: %d/%d = %d%% (weight %d)\n",
               i + 1, math.assignments[i].score, math.assignments[i].max_score,
               pct, math.assignments[i].weight);
    }
    int avg = course_weighted_avg(&math);
    printf("  weighted avg: %d%%\n", avg);
    printf("  grade: %c\n", grade_letter(avg));
    printf("  highest: %d%%\n", course_highest(&math));
    printf("  lowest: %d%%\n", course_lowest(&math));
    printf("  need for A: %d\n", points_needed(&math, 90));

    struct Course science;
    course_init(&science);
    course_add(&science, 45, 50, 25);
    course_add(&science, 38, 50, 25);
    course_add(&science, 80, 100, 50);

    int sci_avg = course_weighted_avg(&science);
    printf("Science:\n");
    printf("  weighted avg: %d%%\n", sci_avg);
    printf("  grade: %c\n", grade_letter(sci_avg));

    int overall = (avg + sci_avg) / 2;
    printf("Overall: %d%% = %c\n", overall, grade_letter(overall));

    return 0;
}
