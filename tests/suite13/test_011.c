int printf(const char *fmt, ...);

// Bubble sort with comparison counting
// Tracks comparisons and swaps during sorting

void bubble_sort(int *arr, int n, int *comps, int *swaps) {
    *comps = 0;
    *swaps = 0;
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        int swapped = 0;
        for (j = 0; j < n - 1 - i; j = j + 1) {
            *comps = *comps + 1;
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                *swaps = *swaps + 1;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void copy_array(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

int count_inversions(int *arr, int n) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[i] > arr[j]) {
                count = count + 1;
            }
        }
    }
    return count;
}

int main() {
    // Test 1: Basic unsorted array
    int a1[6];
    a1[0] = 64; a1[1] = 34; a1[2] = 25;
    a1[3] = 12; a1[4] = 22; a1[5] = 11;
    int comps;
    int swaps;

    int inv = count_inversions(a1, 6);
    printf("inversions before: %d\n", inv);
    // EXPECT: inversions before: 14

    bubble_sort(a1, 6, &comps, &swaps);
    printf("sorted: %d %d %d %d %d %d\n", a1[0], a1[1], a1[2], a1[3], a1[4], a1[5]);
    // EXPECT: sorted: 11 12 22 25 34 64
    printf("comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: comps: 15 swaps: 14
    printf("is_sorted: %d\n", is_sorted(a1, 6));
    // EXPECT: is_sorted: 1

    // Test 2: Already sorted array (early termination)
    int a2[5];
    a2[0] = 1; a2[1] = 2; a2[2] = 3; a2[3] = 4; a2[4] = 5;
    bubble_sort(a2, 5, &comps, &swaps);
    printf("already sorted comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: already sorted comps: 4 swaps: 0

    // Test 3: Reverse sorted (worst case)
    int a3[5];
    a3[0] = 5; a3[1] = 4; a3[2] = 3; a3[3] = 2; a3[4] = 1;
    bubble_sort(a3, 5, &comps, &swaps);
    printf("reverse sorted: %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4]);
    // EXPECT: reverse sorted: 1 2 3 4 5
    printf("reverse comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: reverse comps: 10 swaps: 10

    // Test 4: Array with duplicates
    int a4[7];
    a4[0] = 3; a4[1] = 1; a4[2] = 4; a4[3] = 1;
    a4[4] = 5; a4[5] = 9; a4[6] = 2;
    bubble_sort(a4, 7, &comps, &swaps);
    printf("dupes: %d %d %d %d %d %d %d\n", a4[0], a4[1], a4[2], a4[3], a4[4], a4[5], a4[6]);
    // EXPECT: dupes: 1 1 2 3 4 5 9
    printf("dupes comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: dupes comps: 20 swaps: 7

    // Test 5: Single element
    int a5[1];
    a5[0] = 42;
    bubble_sort(a5, 1, &comps, &swaps);
    printf("single: %d comps: %d swaps: %d\n", a5[0], comps, swaps);
    // EXPECT: single: 42 comps: 0 swaps: 0

    // Test 6: Two elements needing swap
    int a6[2];
    a6[0] = 99; a6[1] = 1;
    bubble_sort(a6, 2, &comps, &swaps);
    printf("two: %d %d comps: %d swaps: %d\n", a6[0], a6[1], comps, swaps);
    // EXPECT: two: 1 99 comps: 1 swaps: 1

    // Test 7: Larger array
    int a7[8];
    a7[0] = 10; a7[1] = 80; a7[2] = 30; a7[3] = 90;
    a7[4] = 40; a7[5] = 50; a7[6] = 70; a7[7] = 20;
    bubble_sort(a7, 8, &comps, &swaps);
    printf("big: %d %d %d %d %d %d %d %d\n", a7[0], a7[1], a7[2], a7[3], a7[4], a7[5], a7[6], a7[7]);
    // EXPECT: big: 10 20 30 40 50 70 80 90
    printf("big comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: big comps: 28 swaps: 13

    // Test 8: Four elements reverse
    int a8[4];
    a8[0] = 4; a8[1] = 3; a8[2] = 2; a8[3] = 1;
    bubble_sort(a8, 4, &comps, &swaps);
    printf("four rev: %d %d %d %d\n", a8[0], a8[1], a8[2], a8[3]);
    // EXPECT: four rev: 1 2 3 4
    printf("four comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: four comps: 6 swaps: 6

    // Test 9: Nearly sorted (one element out of place)
    int a9[6];
    a9[0] = 1; a9[1] = 2; a9[2] = 5; a9[3] = 3; a9[4] = 4; a9[5] = 6;
    bubble_sort(a9, 6, &comps, &swaps);
    printf("nearly: %d %d %d %d %d %d\n", a9[0], a9[1], a9[2], a9[3], a9[4], a9[5]);
    // EXPECT: nearly: 1 2 3 4 5 6
    printf("nearly comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: nearly comps: 9 swaps: 2

    printf("done\n");
    // EXPECT: done

    return 0;
}
