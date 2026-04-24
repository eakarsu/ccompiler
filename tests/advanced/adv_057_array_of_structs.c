
// EXPECT: === Arrays of Structs ===
// EXPECT: init: Alice 85, Bob 92, Carol 78, Dave 95, Eve 88
// EXPECT: highest: Dave with score 95
// EXPECT: lowest: Carol with score 78
// EXPECT: average: 87
// EXPECT: above_avg: Bob Dave Eve
// EXPECT: sorted: Dave(95) Bob(92) Eve(88) Alice(85) Carol(78)
// EXPECT: search_Bob: found at index 1
// EXPECT: search_Zara: not found
// EXPECT: count_above_90: 2
// EXPECT: total_points: 438
// EXPECT: modified: Dave 100, Bob 97, Eve 93, Alice 90, Carol 83
// EXPECT: rank 1: Dave(100)
// EXPECT: rank 2: Bob(97)
// EXPECT: rank 3: Eve(93)
// EXPECT: inventory: sword(10,5) shield(8,15) potion(2,1)
// EXPECT: heaviest: shield weighing 15
// EXPECT: total_weight: 21
// EXPECT: reversed: Carol Bob Alice
int printf(const char *fmt, ...);

struct Student {
    char name[20];
    int score;
};

struct Item {
    char name[16];
    int power;
    int weight;
};

void str_copy(char *dst, const char *src) {
    while (*src) {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
}

int str_equal(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

void swap_students(struct Student *a, struct Student *b) {
    struct Student tmp;
    str_copy(tmp.name, a->name);
    tmp.score = a->score;
    str_copy(a->name, b->name);
    a->score = b->score;
    str_copy(b->name, tmp.name);
    b->score = tmp.score;
}

// Bubble sort by score descending
void sort_students(struct Student *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].score < arr[j + 1].score) {
                swap_students(&arr[j], &arr[j + 1]);
            }
        }
    }
}

int find_student(struct Student *arr, int n, const char *name) {
    int i;
    for (i = 0; i < n; i++) {
        if (str_equal(arr[i].name, name)) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    printf("=== Arrays of Structs ===\n");

    struct Student students[5];

    // Initialize
    str_copy(students[0].name, "Alice");
    students[0].score = 85;
    str_copy(students[1].name, "Bob");
    students[1].score = 92;
    str_copy(students[2].name, "Carol");
    students[2].score = 78;
    str_copy(students[3].name, "Dave");
    students[3].score = 95;
    str_copy(students[4].name, "Eve");
    students[4].score = 88;

    // Print all
    printf("init: %s %d, %s %d, %s %d, %s %d, %s %d\n",
           students[0].name, students[0].score,
           students[1].name, students[1].score,
           students[2].name, students[2].score,
           students[3].name, students[3].score,
           students[4].name, students[4].score);

    // Find highest score
    {
        int best = 0;
        int i;
        for (i = 1; i < 5; i++) {
            if (students[i].score > students[best].score) {
                best = i;
            }
        }
        printf("highest: %s with score %d\n", students[best].name, students[best].score);
    }

    // Find lowest score
    {
        int worst = 0;
        int i;
        for (i = 1; i < 5; i++) {
            if (students[i].score < students[worst].score) {
                worst = i;
            }
        }
        printf("lowest: %s with score %d\n", students[worst].name, students[worst].score);
    }

    // Average
    {
        int sum = 0;
        int i;
        for (i = 0; i < 5; i++) {
            sum += students[i].score;
        }
        int avg = sum / 5;
        printf("average: %d\n", avg);

        // Print above average
        printf("above_avg:");
        for (i = 0; i < 5; i++) {
            if (students[i].score > avg) {
                printf(" %s", students[i].name);
            }
        }
        printf("\n");
    }

    // Sort by score descending
    sort_students(students, 5);
    printf("sorted: %s(%d) %s(%d) %s(%d) %s(%d) %s(%d)\n",
           students[0].name, students[0].score,
           students[1].name, students[1].score,
           students[2].name, students[2].score,
           students[3].name, students[3].score,
           students[4].name, students[4].score);

    // Search
    {
        // Re-init for search (currently sorted)
        // Search for Bob in sorted array
        int idx = find_student(students, 5, "Bob");
        if (idx >= 0) {
            printf("search_Bob: found at index %d\n", idx);
        } else {
            printf("search_Bob: not found\n");
        }

        idx = find_student(students, 5, "Zara");
        if (idx >= 0) {
            printf("search_Zara: found at index %d\n", idx);
        } else {
            printf("search_Zara: not found\n");
        }
    }

    // Count above threshold
    {
        int count = 0;
        int i;
        for (i = 0; i < 5; i++) {
            if (students[i].score > 90) {
                count++;
            }
        }
        printf("count_above_90: %d\n", count);
    }

    // Total points
    {
        int total = 0;
        int i;
        for (i = 0; i < 5; i++) {
            total += students[i].score;
        }
        printf("total_points: %d\n", total);
    }

    // Modify all scores (+5 bonus)
    {
        int i;
        for (i = 0; i < 5; i++) {
            students[i].score += 5;
        }
        printf("modified: %s %d, %s %d, %s %d, %s %d, %s %d\n",
               students[0].name, students[0].score,
               students[1].name, students[1].score,
               students[2].name, students[2].score,
               students[3].name, students[3].score,
               students[4].name, students[4].score);
    }

    // Print top 3 with rank
    {
        sort_students(students, 5);
        int i;
        for (i = 0; i < 3; i++) {
            printf("rank %d: %s(%d)\n", i + 1, students[i].name, students[i].score);
        }
    }

    // Inventory system with Items
    {
        struct Item inv[3];
        str_copy(inv[0].name, "sword");
        inv[0].power = 10;
        inv[0].weight = 5;
        str_copy(inv[1].name, "shield");
        inv[1].power = 8;
        inv[1].weight = 15;
        str_copy(inv[2].name, "potion");
        inv[2].power = 2;
        inv[2].weight = 1;

        printf("inventory: %s(%d,%d) %s(%d,%d) %s(%d,%d)\n",
               inv[0].name, inv[0].power, inv[0].weight,
               inv[1].name, inv[1].power, inv[1].weight,
               inv[2].name, inv[2].power, inv[2].weight);

        // Find heaviest
        int heaviest = 0;
        int i;
        for (i = 1; i < 3; i++) {
            if (inv[i].weight > inv[heaviest].weight) {
                heaviest = i;
            }
        }
        printf("heaviest: %s weighing %d\n", inv[heaviest].name, inv[heaviest].weight);

        // Total weight
        int tw = 0;
        for (i = 0; i < 3; i++) {
            tw += inv[i].weight;
        }
        printf("total_weight: %d\n", tw);
    }

    // Reverse an array of structs
    {
        struct Student arr[3];
        str_copy(arr[0].name, "Alice");
        arr[0].score = 1;
        str_copy(arr[1].name, "Bob");
        arr[1].score = 2;
        str_copy(arr[2].name, "Carol");
        arr[2].score = 3;

        // Reverse
        int left = 0;
        int right = 2;
        while (left < right) {
            swap_students(&arr[left], &arr[right]);
            left++;
            right--;
        }
        printf("reversed: %s %s %s\n", arr[0].name, arr[1].name, arr[2].name);
    }

    return 0;
}
