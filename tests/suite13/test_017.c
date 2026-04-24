int printf(const char *fmt, ...);

// Counting sort for small ranges
// Supports both positive values and shifted ranges

void counting_sort(int *arr, int n, int max_val, int *count_buf, int *output) {
    int i;
    // Clear count buffer
    for (i = 0; i <= max_val; i = i + 1) {
        count_buf[i] = 0;
    }
    // Count occurrences
    for (i = 0; i < n; i = i + 1) {
        count_buf[arr[i]] = count_buf[arr[i]] + 1;
    }
    // Build prefix sums
    for (i = 1; i <= max_val; i = i + 1) {
        count_buf[i] = count_buf[i] + count_buf[i - 1];
    }
    // Build output (stable, go backward)
    for (i = n - 1; i >= 0; i = i - 1) {
        count_buf[arr[i]] = count_buf[arr[i]] - 1;
        output[count_buf[arr[i]]] = arr[i];
    }
    // Copy back
    for (i = 0; i < n; i = i + 1) {
        arr[i] = output[i];
    }
}

// Counting sort with range shift (supports min..max)
void counting_sort_range(int *arr, int n, int min_val, int max_val,
                          int *count_buf, int *output) {
    int range = max_val - min_val + 1;
    int i;
    for (i = 0; i < range; i = i + 1) {
        count_buf[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        count_buf[arr[i] - min_val] = count_buf[arr[i] - min_val] + 1;
    }
    for (i = 1; i < range; i = i + 1) {
        count_buf[i] = count_buf[i] + count_buf[i - 1];
    }
    for (i = n - 1; i >= 0; i = i - 1) {
        int shifted = arr[i] - min_val;
        count_buf[shifted] = count_buf[shifted] - 1;
        output[count_buf[shifted]] = arr[i];
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = output[i];
    }
}

// Find min and max in an array
void find_range(int *arr, int n, int *min_out, int *max_out) {
    *min_out = arr[0];
    *max_out = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < *min_out) *min_out = arr[i];
        if (arr[i] > *max_out) *max_out = arr[i];
    }
}

// Frequency histogram
void histogram(int *arr, int n, int max_val, int *freq) {
    int i;
    for (i = 0; i <= max_val; i = i + 1) {
        freq[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        freq[arr[i]] = freq[arr[i]] + 1;
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Mode: most frequent element (returns first one if tie)
int find_mode(int *arr, int n, int max_val, int *freq) {
    histogram(arr, n, max_val, freq);
    int mode = 0;
    int best = freq[0];
    int i;
    for (i = 1; i <= max_val; i = i + 1) {
        if (freq[i] > best) {
            best = freq[i];
            mode = i;
        }
    }
    return mode;
}

int main() {
    int count_buf[100];
    int output[30];
    int freq[100];

    // Test 1: Basic counting sort
    int a1[8];
    a1[0] = 4; a1[1] = 2; a1[2] = 2; a1[3] = 8;
    a1[4] = 3; a1[5] = 3; a1[6] = 1; a1[7] = 5;
    counting_sort(a1, 8, 9, count_buf, output);
    printf("sorted: %d %d %d %d %d %d %d %d\n",
           a1[0], a1[1], a1[2], a1[3], a1[4], a1[5], a1[6], a1[7]);
    // EXPECT: sorted: 1 2 2 3 3 4 5 8
    printf("is_sorted: %d\n", is_sorted(a1, 8));
    // EXPECT: is_sorted: 1

    // Test 2: All zeros
    int a2[5];
    a2[0] = 0; a2[1] = 0; a2[2] = 0; a2[3] = 0; a2[4] = 0;
    counting_sort(a2, 5, 0, count_buf, output);
    printf("zeros: %d %d %d %d %d\n", a2[0], a2[1], a2[2], a2[3], a2[4]);
    // EXPECT: zeros: 0 0 0 0 0

    // Test 3: Already sorted
    int a3[6];
    a3[0] = 1; a3[1] = 3; a3[2] = 5; a3[3] = 7; a3[4] = 9; a3[5] = 9;
    counting_sort(a3, 6, 9, count_buf, output);
    printf("presorted: %d %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4], a3[5]);
    // EXPECT: presorted: 1 3 5 7 9 9

    // Test 4: Histogram analysis
    int a4[10];
    a4[0] = 1; a4[1] = 2; a4[2] = 3; a4[3] = 2; a4[4] = 1;
    a4[5] = 3; a4[6] = 3; a4[7] = 1; a4[8] = 2; a4[9] = 3;
    histogram(a4, 10, 5, freq);
    printf("freq 0:%d 1:%d 2:%d 3:%d 4:%d 5:%d\n",
           freq[0], freq[1], freq[2], freq[3], freq[4], freq[5]);
    // EXPECT: freq 0:0 1:3 2:3 3:4 4:0 5:0

    // Test 5: Find mode
    int mode = find_mode(a4, 10, 5, freq);
    printf("mode: %d\n", mode);
    // EXPECT: mode: 3

    // Test 6: Sort grades (0-100 range)
    int grades[8];
    grades[0] = 95; grades[1] = 72; grades[2] = 88; grades[3] = 95;
    grades[4] = 60; grades[5] = 88; grades[6] = 72; grades[7] = 99;
    int grade_min;
    int grade_max;
    find_range(grades, 8, &grade_min, &grade_max);
    printf("range: %d to %d\n", grade_min, grade_max);
    // EXPECT: range: 60 to 99
    counting_sort_range(grades, 8, grade_min, grade_max, count_buf, output);
    printf("grades: %d %d %d %d %d %d %d %d\n",
           grades[0], grades[1], grades[2], grades[3],
           grades[4], grades[5], grades[6], grades[7]);
    // EXPECT: grades: 60 72 72 88 88 95 95 99

    // Test 7: Single element
    int a7[1];
    a7[0] = 5;
    counting_sort(a7, 1, 5, count_buf, output);
    printf("single: %d\n", a7[0]);
    // EXPECT: single: 5

    // Test 8: Large range of values
    int a8[6];
    a8[0] = 50; a8[1] = 10; a8[2] = 90; a8[3] = 30; a8[4] = 70; a8[5] = 20;
    counting_sort_range(a8, 6, 10, 90, count_buf, output);
    printf("large range: %d %d %d %d %d %d\n",
           a8[0], a8[1], a8[2], a8[3], a8[4], a8[5]);
    // EXPECT: large range: 10 20 30 50 70 90

    // Test 9: Stability check - sort by first digit
    int vals[6];
    vals[0] = 4; vals[1] = 3; vals[2] = 4; vals[3] = 2; vals[4] = 3; vals[5] = 1;
    counting_sort(vals, 6, 5, count_buf, output);
    printf("stable: %d %d %d %d %d %d\n",
           vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);
    // EXPECT: stable: 1 2 3 3 4 4

    printf("done\n");
    // EXPECT: done

    return 0;
}
