int printf(const char *fmt, ...);

int group_keys[32];
int group_sums[32];
int group_counts[32];
int group_used[32];
int group_size;

void group_init(void) {
    group_size = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        group_keys[i] = 0;
        group_sums[i] = 0;
        group_counts[i] = 0;
        group_used[i] = 0;
    }
}

int hash_int(int x) {
    if (x < 0) x = -x;
    return x % 32;
}

void group_add(int key, int value) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (group_used[idx] == 0) {
            group_keys[idx] = key;
            group_sums[idx] = value;
            group_counts[idx] = 1;
            group_used[idx] = 1;
            group_size = group_size + 1;
            return;
        }
        if (group_keys[idx] == key) {
            group_sums[idx] = group_sums[idx] + value;
            group_counts[idx] = group_counts[idx] + 1;
            return;
        }
    }
}

int group_get_sum(int key) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (group_used[idx] == 0) return 0;
        if (group_keys[idx] == key) return group_sums[idx];
    }
    return 0;
}

int group_get_count(int key) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 32; i = i + 1) {
        int idx = (h + i) % 32;
        if (group_used[idx] == 0) return 0;
        if (group_keys[idx] == key) return group_counts[idx];
    }
    return 0;
}

int group_get_avg(int key) {
    int c = group_get_count(key);
    if (c == 0) return 0;
    return group_get_sum(key) / c;
}

int main(void) {
    group_init();

    int data[15];
    data[0] = 10; data[1] = 21; data[2] = 32;
    data[3] = 43; data[4] = 54; data[5] = 15;
    data[6] = 26; data[7] = 37; data[8] = 48;
    data[9] = 9;  data[10] = 20; data[11] = 31;
    data[12] = 42; data[13] = 53; data[14] = 14;

    int i;
    for (i = 0; i < 15; i = i + 1) {
        int key = data[i] % 5;
        group_add(key, data[i]);
    }

    // EXPECT: num groups: 5
    printf("num groups: %d\n", group_size);

    // EXPECT: group 0 sum: 45
    printf("group 0 sum: %d\n", group_get_sum(0));

    // EXPECT: group 1 sum: 78
    printf("group 1 sum: %d\n", group_get_sum(1));

    // EXPECT: group 2 sum: 111
    printf("group 2 sum: %d\n", group_get_sum(2));

    // EXPECT: group 3 sum: 144
    printf("group 3 sum: %d\n", group_get_sum(3));

    // EXPECT: group 4 sum: 77
    printf("group 4 sum: %d\n", group_get_sum(4));

    // EXPECT: group 0 count: 3
    printf("group 0 count: %d\n", group_get_count(0));

    // EXPECT: group 1 count: 3
    printf("group 1 count: %d\n", group_get_count(1));

    // EXPECT: group 2 count: 3
    printf("group 2 count: %d\n", group_get_count(2));

    // EXPECT: group 3 count: 3
    printf("group 3 count: %d\n", group_get_count(3));

    // EXPECT: group 4 count: 3
    printf("group 4 count: %d\n", group_get_count(4));

    // EXPECT: group 0 avg: 15
    printf("group 0 avg: %d\n", group_get_avg(0));

    // EXPECT: group 1 avg: 26
    printf("group 1 avg: %d\n", group_get_avg(1));

    // EXPECT: group 2 avg: 37
    printf("group 2 avg: %d\n", group_get_avg(2));

    group_init();
    int scores[10];
    scores[0] = 85; scores[1] = 92; scores[2] = 78;
    scores[3] = 88; scores[4] = 95; scores[5] = 72;
    scores[6] = 90; scores[7] = 83; scores[8] = 91;
    scores[9] = 76;
    for (i = 0; i < 10; i = i + 1) {
        int grade;
        if (scores[i] >= 90) grade = 1;
        else if (scores[i] >= 80) grade = 2;
        else grade = 3;
        group_add(grade, scores[i]);
    }

    // EXPECT: grade groups: 3
    printf("grade groups: %d\n", group_size);

    // EXPECT: grade A count: 4
    printf("grade A count: %d\n", group_get_count(1));

    // EXPECT: grade B count: 3
    printf("grade B count: %d\n", group_get_count(2));

    // EXPECT: grade C count: 3
    printf("grade C count: %d\n", group_get_count(3));

    // EXPECT: done
    printf("done\n");

    return 0;
}
